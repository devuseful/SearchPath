/*
 * ImageCell.cpp
 *
 *  Created on: 12.07.2016
 *      Author: User
 */

#include "ImageCell.h"

ImageCell::ImageCell():
statusFree_(true)
{
	// TODO Auto-generated constructor stub
	//CCLog("ImageCell()");
}



ImageCell::~ImageCell() {
	// TODO Auto-generated destructor stub
	//CCLog("~ImageCell()");
}

ImageCell* ImageCell::create()
{
    ImageCell* pointSprite = new ImageCell();
	if (pointSprite && pointSprite->init())
    {
		pointSprite->autorelease();

		pointSprite->loadTexture("cell.png", UI_TEX_TYPE_PLIST);
		pointSprite->setColor(ccWHITE);
		pointSprite->setTouchEnabled(true);

		return pointSprite;
    }

	CC_SAFE_DELETE(pointSprite);
    return NULL;
}


pair<int, int> ImageCell::getPositionInMap() {

	int j = round(-(LEFT_EDGE - this->getPositionX()) / this->getContentSize().width);
	int i = round(-(LOW_EDGE - this->getPositionY()) / this->getContentSize().height);
	//return pair<int, int>(j, i);
	return pair<int, int>(j, COUNT_MAP_ELEMENT_Y - 1 - i);
}



void ImageCell::setCellStatusFree(bool statusFree) {
	statusFree_ = statusFree;
	if (statusFree_)
		this->setColor(ccRED);
	else
		this->setColor(ccWHITE);

	float duration = 0.2f;
	this->setScale(0);
	CCActionInterval *scaleToBig = CCScaleTo::create(duration, 1.0f);
	CCActionInterval *move_ease = CCEaseBackOut::create(scaleToBig);
	this->runAction(move_ease);
}

bool ImageCell::isCellStatusFree() {
	return statusFree_;
}