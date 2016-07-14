/*
 * ImageCell.h
 *
 *  Created on: 12.07.2016
 *      Author: User
 */

#ifndef __ImageCell_H__
#define __ImageCell_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Constant.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace std;


class ImageCell : public UIImageView {
	
public:

	ImageCell();

	static ImageCell *create();

	void setCellStatusFree(bool statusFree);
	bool isCellStatusFree();

	pair<int, int> getPositionInMap();

	virtual ~ImageCell();

protected:

	bool statusFree_;

};

#endif // __ImageCell_H__
