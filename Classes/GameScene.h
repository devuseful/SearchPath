#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "ImagePoint.h"
#include "ImageCell.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace std;


class Game : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(Game);

protected:

	void _callbackCreateLevelMapAnimation(CCNode * pNode);

	vector<pair<int, int> >_getPathInWaveAlgorithm(pair<int, int> startPos, pair<int, int> endPos);

	void _onTouchCellAB(CCObject* pSender, TouchEventType type);
	void _onTouchCellMap(CCObject* pSender, TouchEventType type);
	void _onTouchButton(CCObject* pSender, TouchEventType type);

	void _updatePoint();

	void _createLevelMap(UIImageView *background);
	void _generateMap();
	void _addBorders();

	void _loadMapFromFile(const char* file);

	UIImageView *_getBackgroundImage();

	void _setVisiblePath(const vector<pair<int, int> > &path, CCSize cellSize);

	enum enumGame
	{
		kCellFreeTag,
		kCellNotFreeTag,
		kPathNodeTag,
		kPathStepTag,
		kPointOrderTag = 1000,
		kPointATag,
		kPointBTag,
	};

	int map_[COUNT_MAP_ELEMENT_X][COUNT_MAP_ELEMENT_Y];
	int level_;

};

#endif // __Game_SCENE_H__
