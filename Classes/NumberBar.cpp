/*
 * NumberBar.cpp
 *
 *  Created on: 2015年5月22日
 *      Author: kerlw
 */

#include "NumberBar.h"
#include "Const.h"

#include "extensions/cocos-ext.h"

USING_NS_CC;

USING_NS_CC_EXT;

const float NumberBar::SPRITE_GAP = 1.0f;

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

bool NumberBar::init() {
	if (!Layout::init())
		return false;

	this->ignoreAnchorPointForPosition(false);
//	this->setBackGroundColor(Color3B(0,255,0));
//	this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	this->setAnchorPoint(Vec2(0.5, 0));

	return true;
}

void NumberBar::attachSprites(int number, SpriteFrameCache* cache) {
	m_rectSprites.clear();

	float x_offset = CELL_SIZE/2;
	float width = 0, height = CELL_SIZE;
	for (int index = 0; index < number; index++) {
		auto sprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(FRAME_NAME[index]));
		Rect rt = sprite->getTextureRect();
		sprite->setPosition(Vec2(x_offset + rt.size.width/2, height - rt.size.height/2));

		Rect rtSprite(x_offset, 0, rt.size.width, rt.size.height);
		m_rectSprites.push_back(rtSprite);

//		log("rect of sprite %d is %f,%f,%f,%f", index, rtSprite.origin.x, rtSprite.origin.y, rtSprite.size.width, rtSprite.size.height);

//		height = (rt.size.height > height ? rt.size.height : height);
		width += rt.size.width + SPRITE_GAP;
		this->addChild(sprite);

		x_offset += rt.size.width + SPRITE_GAP;
	}
//	height += 2 * SPRITE_GAP;
	width += CELL_SIZE;
	this->setContentSize(Size(width, height));

	//add background
	auto frm = cache->getSpriteFrameByName(FRAME_NAME[SHELF_INDEX]);
	if (frm) {
		auto bkg = Scale9Sprite::createWithSpriteFrame(frm);
		bkg->setPreferredSize(Size(width, height));
		bkg->setCapInsets(Rect(70, 70, 1, 1));
		bkg->setPosition(Vec2(width/2, height/2));
		this->addChild(bkg, -1);
	} else {
		this->setBackGroundColor(Color3B(0,255,0));
		this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	}
//	log("this content size is %f, %f", width, height);
}

int NumberBar::findSpriteIndexAtLocation(Vec2 location) {
//	log("find sprite at location %f,%f", location.x, location.y);
	int size = m_rectSprites.size();
	for (int index = 0; index < size; index++) {
		if (m_rectSprites[index].containsPoint(location))
			return index;
	}
	return -1;
}
