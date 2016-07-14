/*
 * ImagePoint.cpp
 *
 *  Created on: 12.07.2016
 *      Author: User
 */

#include "ImagePoint.h"

ImagePoint::ImagePoint():
visibleShadow_(false)
{
	// TODO Auto-generated constructor stub
	//CCLog("ImagePoint()");
	posInMap_.first = round(-(LEFT_EDGE - this->getPositionX()) / this->getContentSize().width);
	posInMap_.second = round(-(LOW_EDGE - this->getPositionY()) / this->getContentSize().height);
}



ImagePoint::~ImagePoint() {
	// TODO Auto-generated destructor stub
	//CCLog("~ImagePoint()");
}

ImagePoint* ImagePoint::create()
{
    ImagePoint* pointSprite = new ImagePoint();
	if (pointSprite && pointSprite->init())
    {
		pointSprite->autorelease();

		pointSprite->loadTexture("cell.png", UI_TEX_TYPE_PLIST);
		pointSprite->setColor(ccYELLOW);
		pointSprite->setTouchEnabled(true);

		return pointSprite;
    }

	CC_SAFE_DELETE(pointSprite);
    return NULL;
}


// функция плавно устанавливает точку в нужное место и тем самым корректирует ее позицию
void ImagePoint::setCorrectPosition() {
	float duration = 0.1f;

	int j = round(-(LEFT_EDGE - this->getPositionX()) / this->getContentSize().width);
	int i = round(-(LOW_EDGE - this->getPositionY()) / this->getContentSize().height);
	
	this->_setPositionInMap(pair<int, int>(j, i));

	CCActionInterval* move = CCMoveTo::create(duration, ccp(LEFT_EDGE + this->getContentSize().width * j, LOW_EDGE + this->getContentSize().height * i));
	this->runAction(move);
}


void ImagePoint::_setPositionInMap(pair<int, int> posInMap) {
	posInMap_ = posInMap;
}


pair<int, int> ImagePoint::getPositionInMap() {

	return pair<int, int>(posInMap_.first, COUNT_MAP_ELEMENT_Y - 1 - posInMap_.second);
}


void ImagePoint::setVisibleShadow(bool visible) {
	visibleShadow_ = visible;
	if (visibleShadow_)
		this->_addShadow();
	else
		this->_removeShadow();
}

// функция создает и выдвигает тень
void ImagePoint::_addShadow() {

	UIImageView *shadowImage = static_cast<UIImageView *>(this->getChildByTag(kShadowTag));
	float duration = 0.05f;
	
	if (!shadowImage) {
		shadowImage = UIImageView::create();
		shadowImage->loadTexture("shadow.png", UI_TEX_TYPE_PLIST);
		shadowImage->setAnchorPoint(ccp(0.5f, 0.5f));
		shadowImage->setPosition(CCPointZero);
		shadowImage->setOpacity(kShadowOpacity);
		this->addChild(shadowImage, -2, kShadowTag);
	}
	CCActionInterval* moveShadow = CCMoveTo::create(duration, ccp(kShiftShadow, -kShiftShadow));
	shadowImage->runAction(moveShadow);
}

// функция задвигает тень и удаляет
void ImagePoint::_removeShadow() {

	UIImageView *shadowImage = static_cast<UIImageView *>(this->getChildByTag(kShadowTag));

	if (shadowImage) {
		float duration = 0.05f;
		CCActionInterval* moveShadow = CCMoveTo::create(duration, ccp(kShiftShadow, -kShiftShadow));
		CCAction * seq = CCSequence::create(moveShadow, CCRemoveSelf::create(), NULL);
		shadowImage->runAction(seq);
	}
}