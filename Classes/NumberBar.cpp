/*
 * NumberBar.cpp
 *
 *  Created on: 2015年5月22日
 *      Author: kerlw
 */

#include "NumberBar.h"
//#include "CCSpriteFrameCache.h"
#include "Const.h"

USING_NS_CC;

NumberBar::NumberBar() {

}

NumberBar::~NumberBar() {

}

NumberBar* NumberBar::create() {
	NumberBar* bar = new (std::nothrow) NumberBar();
	if (bar && bar->init()) {
		bar->autorelease();
		return bar;
	}
    CC_SAFE_DELETE(bar);
    return nullptr;
}

void NumberBar::attachSprites(int number, SpriteFrameCache* cache) {
	for (int index = 0; index < number; index++) {
		auto sprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(FRAME_NAME[index]));
		this->addChild(sprite);
	}
}
