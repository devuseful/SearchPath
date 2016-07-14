/*
 * ImagePoint.h
 *
 *  Created on: 12.07.2016
 *      Author: User
 */

#ifndef __ImagePoint_H__
#define __ImagePoint_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "spine/Json.h"
#include "Constant.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace std;


class ImagePoint : public UIImageView {
	
public:

	ImagePoint();

	static ImagePoint *create();

	void setCorrectPosition();

	pair<int, int> getPositionInMap();

	void setVisibleShadow(bool visible);

	virtual ~ImagePoint();

protected:

	void _setPositionInMap(pair<int, int> posInMap);
	void _addShadow();
	void _removeShadow();


enum enumImagePoint
	{
		kShadowTag,
		kShiftShadow = 5,
		kShadowOpacity = 150,
	};

	pair<int, int> posInMap_;
	bool visibleShadow_;

};

#endif // __ImagePoint_H__
