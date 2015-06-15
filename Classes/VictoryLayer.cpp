/*
 * VictoryLayer.cpp
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#include "VictoryLayer.h"
#include "ui/UIButton.h"
#include "GameController.h"

VictoryLayer::VictoryLayer() {
}

VictoryLayer::~VictoryLayer() {
}

bool VictoryLayer::init() {
	if (!LayerColor::init())
		return false;

	this->setContentSize(Size(800,600));
	this->setColor(Color3B(0,255,255));
	this->setOpacity(200);

	auto label = Label::createWithTTF("Victory", "fonts/Marker Felt.ttf", 188);
	label->setTextColor(Color4B(255, 0, 0, 200));
	label->setPosition(Vec2(800/2, (600 + 80) / 2));
	this->addChild(label);

	auto btn = ui::Button::create();
	btn->setTitleText("Continue");
	btn->setTitleFontName("fonts/Marker Felt.ttf");
	btn->setTitleFontSize(60);
	btn->setTitleColor(Color3B(255,255,0));
	btn->setPosition(Vec2(800/2, 80 / 2));
	btn->addClickEventListener(CC_CALLBACK_1(VictoryLayer::onContinueClicked, this));
	this->addChild(btn);

	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5,0.5));
	return true;
}

void VictoryLayer::onContinueClicked(Ref* ref) {
	log("clicked");
	auto parent= this->getParent();
	if (parent)
		parent->removeChild(this);

	GameController::getInstance()->nextStage();
}


