#include "GameScene.h"

USING_NS_CC;

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();

    // add layer as a child to scene
    scene->addChild(layer);
	layer->setTag(0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "scene/GUI/CloseNormal.png",
                                        "scene/GUI/CloseSelected.png",
                                        this,
                                        menu_selector(Game::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

	// предварительная загрузка графики
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("graphics/SearchPath0.plist");

	// загрузка сцены
	CCNode * pNode = SceneReader::sharedSceneReader()->createNodeWithSceneFile("scene/SPMain.json"); //cocos2dx 2.2
	this->addChild(pNode, 0, 0);
	level_ = 1;

	CCNode *bgNode = pNode->getChildByTag(10004);
	UILayer * bgLayer = static_cast<UILayer *>(bgNode->getChildren()->objectAtIndex(0));
	UIImageView *background = static_cast<ImageView *>(bgLayer->getWidgetByName("ImageViewBackGround"));

	bgLayer->getWidgetByName("ButtonGenerate")->addTouchEventListener(this, toucheventselector(Game::_onTouchButton));
	bgLayer->getWidgetByName("ButtonNext")->addTouchEventListener(this, toucheventselector(Game::_onTouchButton));

	this->_loadMapFromFile(CCString::createWithFormat("data/level%d.txt", level_)->getCString());
	this->_createLevelMap(background);

    return true;
}


// функция генерирует случайный уровень
void Game::_generateMap() {

	srand(time(NULL));
	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {
		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j)	{
			
			if (rand() % 4 == 0)
				map_[j][i] = kCellNotFreeTag;
			else
				map_[j][i] = kCellFreeTag;
		}
	}
	this->_addBorders();
}

// функция добавляет бордюр к уровню
void Game::_addBorders() {

	for (int i = 0; i < COUNT_MAP_ELEMENT_X; ++i) {
		map_[i][0] = kCellNotFreeTag;
		map_[i][COUNT_MAP_ELEMENT_Y - 1] = kCellNotFreeTag;
	}
	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {
		map_[0][i] = kCellNotFreeTag;
		map_[COUNT_MAP_ELEMENT_X - 1][i] = kCellNotFreeTag;
	}

}


// функция содает и выводит уровень с картой на экран
void Game::_createLevelMap(UIImageView *background) {

	background->removeAllChildren();
	CCArray *arraySeq = CCArray::create();

	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {
		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j) {

			ImageCell *cell = ImageCell::create();
			cell->setPosition(ccp(LEFT_EDGE + cell->getContentSize().width * j, LOW_EDGE + cell->getContentSize().height * (COUNT_MAP_ELEMENT_Y - 1 - i)));
			cell->addTouchEventListener(this, toucheventselector(Game::_onTouchCellMap));
			background->addChild(cell, 1);

			if (map_[j][i] == kCellNotFreeTag)
				cell->setCellStatusFree(true);
			else
				cell->setCellStatusFree(false);

			float duration = 0.2f * (rand() % 4 + 1);
			cell->setScale(0);
			CCActionInterval *scaleToBig = CCScaleTo::create(duration, 1.0f);
			CCActionInterval *move_ease = CCEaseBackOut::create(scaleToBig);
			arraySeq->addObject(CCTargetedAction::create(cell, move_ease));
		}
	}

	CCSequence* seq = CCSequence::create(CCSpawn::create(arraySeq), CCCallFuncN::create(this, callfuncN_selector(Game::_callbackCreateLevelMapAnimation)), NULL);
	this->runAction(seq);

}


// функция загрузки уровней из текстовых файлов
void Game::_loadMapFromFile(const char* file) {
	unsigned long size = 0;
	unsigned char *pData = CCFileUtils::sharedFileUtils()->getFileData(file, "r", &size);

	int j = 0;
	int i = 0;
	for (size_t k = 0; k < size; ++k, ++i)
	{
		if (pData[k] == '\n') {
			i = -1;
			++j;
		}
		else
			map_[i][j] = pData[k] - '0' == 0 ? kCellFreeTag : kCellNotFreeTag;
	}
    
    delete [] pData;
    pData = NULL;
}


void Game::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


// функция обработки обратного вызова анимации создания уровня
// функция добавляет точки А и Б в поле
void Game::_callbackCreateLevelMapAnimation(CCNode * node) {

	UIImageView *background = this->_getBackgroundImage();

	pair<int, int> start = pair<int, int>(1, 1);
	pair<int, int> end = pair<int, int>(COUNT_MAP_ELEMENT_X - 2, 1);

	ImagePoint *cell = ImagePoint::create();
	cell->setPosition(ccp(LEFT_EDGE + cell->getContentSize().width * start.first, LOW_EDGE + cell->getContentSize().height * (COUNT_MAP_ELEMENT_Y - 1 - start.second)));
	cell->addTouchEventListener(this, toucheventselector(Game::_onTouchCellAB));
	background->addChild(cell, kPointOrderTag, kPointATag);
	cell->setCorrectPosition();

	cell = ImagePoint::create();
	cell->setPosition(ccp(LEFT_EDGE + cell->getContentSize().width * end.first, LOW_EDGE + cell->getContentSize().height * (COUNT_MAP_ELEMENT_Y - 1 - end.second)));
	cell->addTouchEventListener(this, toucheventselector(Game::_onTouchCellAB));
	background->addChild(cell, kPointOrderTag, kPointBTag);
	cell->setCorrectPosition();

	this->_setVisiblePath(this->_getPathInWaveAlgorithm(start, end), cell->getContentSize());
}



// функция получает путь от начальной позиции до конечной при помощи волнового алгоритма
// startPos - начальная позиция
// endPos - конечная позиция
vector<pair<int, int> >Game::_getPathInWaveAlgorithm(pair<int, int> startPos, pair<int, int> endPos) {

	vector<pair<int, int> > oldwave;
	vector<pair<int, int> > wave;

	oldwave.push_back(std::pair<int, int>(startPos.first, startPos.second));

	int map[COUNT_MAP_ELEMENT_X][COUNT_MAP_ELEMENT_Y];
	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {
		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j) {

			if (map_[j][i] == kCellFreeTag)
				map[j][i] = -1;
			else
				map[j][i] = WALL;
		}
	}

	int nstep = 0;
	map[startPos.first][startPos.second] = nstep;
	const int dx[] = { 0, 1, 0, -1 };
	const int dy[] = { -1, 0, 1, 0 };

	bool pathFind = false;

	// волновой проход
	while (oldwave.size() > 0 && !pathFind) {

		++nstep;
		wave.clear();
		for (vector<pair<int, int> >::iterator i = oldwave.begin(); i != oldwave.end(); ++i) {
			for (int d = 0; d < 4; ++d) {

				int nx = i->first + dx[d];
				int ny = i->second + dy[d];
				if (map[nx][ny] == -1) {

					wave.push_back(pair<int, int>(nx, ny));
					map[nx][ny] = nstep;

					// если удалось дойти до цели
					if (nx == endPos.first && ny == endPos.second)
						pathFind = true;
				}
			}
		}
		oldwave = wave;
	}

	// обратный проход
	if (pathFind) {
		int x = endPos.first;
		int y = endPos.second;
		wave.clear();
		wave.push_back(pair<int, int>(x, y));
		while (map[x][y] != 0) {

			for (int d = 0; d < 4; ++d) {

				int nx = x + dx[d];
				int ny = y + dy[d];
				if (map[x][y] - 1 == map[nx][ny]) {
					x = nx;
					y = ny;
					wave.push_back(pair<int, int>(x, y));
					break;
				}

			}
		}
	}

	return wave;
}


// функция обрабатывает нажатия на точки A и B
void Game::_onTouchCellAB(CCObject* pSender, TouchEventType type) {

	float scalePersent = 1.05f;
	ImagePoint *image = static_cast<ImagePoint *>(pSender);

	if (type == TOUCH_EVENT_BEGAN) {
		image->setScale(image->getScale()*scalePersent);
		image->setVisibleShadow(true);
	}

	if (type == TOUCH_EVENT_ENDED) {
		image->setScale(image->getScale() / scalePersent);
		image->setCorrectPosition();
		image->setVisibleShadow(false);
		this->_updatePoint();
	}

	if (type == TOUCH_EVENT_CANCELED) {
		image->setScale(image->getScale() / scalePersent);
	}

	if (type == TOUCH_EVENT_MOVED) {
		CCPoint touchPoint = image->getTouchMovePos();
		CCPoint posPoint = image->getParent()->getPosition();
		image->setPosition(ccp(-posPoint.x + touchPoint.x, -posPoint.y + touchPoint.y));
	}
}


// функция облабатывает нажатия на клетки уровня
void Game::_onTouchCellMap(CCObject* pSender, TouchEventType type) {

	float scalePersent = 1.05f;
	ImageCell *cell = static_cast<ImageCell *>(pSender);

	if (type == TOUCH_EVENT_BEGAN) {
		cell->setScale(cell->getScale()*scalePersent);
	}

	if (type == TOUCH_EVENT_ENDED) {
		cell->setScale(cell->getScale() / scalePersent);

		cell->setCellStatusFree(!cell->isCellStatusFree());

		pair<int, int> posInMap = cell->getPositionInMap();
		map_[posInMap.first][posInMap.second] = cell->isCellStatusFree();

		this->_updatePoint();
	}

	if (type == TOUCH_EVENT_CANCELED) {
		cell->setScale(cell->getScale() / scalePersent);
	}
}


// функция обрабатывает нажатия на кнопки "сгенерировать уровень" и "следующий уровень"
void Game::_onTouchButton(CCObject* pSender, TouchEventType type) {

	UIImageView *button = static_cast<UIImageView *>(pSender);
	if (type == TOUCH_EVENT_ENDED) {

		if (button->getTag() == 4) {// tag кнопки generate
			this->_generateMap();
			this->_createLevelMap(this->_getBackgroundImage());
		}

		if (button->getTag() == 5) { // tag кнопки next
			++level_;
			if (level_ > 3) 
				level_ = 1;
			this->_loadMapFromFile(CCString::createWithFormat("data/level%d.txt", level_)->getCString());
			this->_createLevelMap(this->_getBackgroundImage());
		}
	}
}


// функция обновления позиций точек A и B
void Game::_updatePoint() {

	UIImageView *background = this->_getBackgroundImage();

	ImagePoint *imagePointA = static_cast<ImagePoint *>(background->getChildByTag(kPointATag));
	ImagePoint *imagePointB = static_cast<ImagePoint *>(background->getChildByTag(kPointBTag));

	vector<pair<int, int> > path = this->_getPathInWaveAlgorithm(imagePointA->getPositionInMap(), imagePointB->getPositionInMap());
	this->_setVisiblePath(path, imagePointA->getContentSize());
}


// получить фоновый рисунок
// т.е. родителя всех элементов уровня
UIImageView *Game::_getBackgroundImage() {
	
	CCNode *pNode = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(0);
	CCNode *bgNode = pNode->getChildByTag(0)->getChildByTag(10004); // top
	UILayer * bgLayer = static_cast<UILayer *>(bgNode->getChildren()->objectAtIndex(0));
	UIImageView *background = static_cast<ImageView *>(bgLayer->getWidgetByName("ImageViewBackGround"));

	return background;
}


// функция выводит путь на поле в виде анимации
void Game::_setVisiblePath(const vector<pair<int, int> > &path, CCSize cellSize) {

	UIImageView *background = this->_getBackgroundImage();
	// удалить ноду с путем если она создана
	if (background->getNodeByTag(kPathNodeTag))
		background->removeNodeByTag(kPathNodeTag);

	CCNode *nodePath = CCNode::create();
	background->addNode(nodePath, 1, kPathNodeTag);

	for (size_t i = 0; i < path.size(); i++) {

		// создать путь и его анимацию
		if (i) {

			// создать элемeнт пути
			float duration = 0.5f;
			UIImageView* pathImage = UIImageView::create();
			pathImage->loadTexture("pathstep.png", UI_TEX_TYPE_PLIST);
			pathImage->setScale(0.7f);

			CCPoint curPos = ccp(LEFT_EDGE + cellSize.width * path[i].first, -(LOW_EDGE + cellSize.height * path[i].second));
			CCPoint prevPos = ccp(LEFT_EDGE + cellSize.width * path[i - 1].first, -(LOW_EDGE + cellSize.height * path[i - 1].second));

			pathImage->setPosition(curPos);
			pathImage->setColor(ccGREEN);
			pathImage->setOpacity(150);
			nodePath->addChild(pathImage, 1, kPathStepTag);

			// добавить анимацию
			CCActionInterval* movePath = CCMoveTo::create(duration, prevPos);
			CCActionInterval* movePathReset = CCMoveTo::create(0, curPos);
			CCAction* sequence = CCRepeatForever::create(CCSequence::create(movePath, movePathReset, NULL));
			pathImage->runAction(sequence);
		}
	}
}


