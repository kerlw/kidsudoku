/*
 * MainScene.cpp
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#include "MainScene.h"
#include "Const.h"
#include "GameController.h"

#include "ui/UIButton.h"

MainScene::MainScene() {
}

MainScene::~MainScene() {
}

bool MainScene::init() {
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	auto pView = director->getOpenGLView();
	pView->setDesignResolutionSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, ResolutionPolicy::SHOW_ALL);

	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	//Title 'Kid Sudoku'
	auto labelTitle = Label::createWithTTF("Kid Sudoku", "fonts/Marker Felt.ttf", 188);
	labelTitle->setTextColor(Color4B(0xe3, 0x54, 0xf4, 0xff));
	labelTitle->setPosition(Vec2(origin.x + visibleSize.width / 2,
						 	origin.y + visibleSize.height - labelTitle->getContentSize().height/2 - 40)
						);
	this->addChild(labelTitle);

	//Cool Papa-Legend announcement
	auto labelAnn = Label::createWithTTF("Cool Papa-Legend :   Produce product for our kids.", "fonts/Marker Felt.ttf", 30);
	labelAnn->setTextColor(Color4B(0x00, 0xff, 0x00, 0xff));
	labelAnn->setPosition(Vec2(origin.x + visibleSize.width / 2,
						 	labelTitle->getPosition().y - labelTitle->getContentSize().height/2 - 60 - labelAnn->getContentSize().height / 2)
						);
	this->addChild(labelAnn);

	//Campaign Button
	auto btnCampaign = ui::Button::create("btn-menu-0.png",
											  "btn-menu-1.png",
											  "btn-menu-2.png");
	btnCampaign->setScale9Enabled(true);
	btnCampaign->setCapInsets(Rect(15, 15, 4, 4));
	btnCampaign->setTitleText("Campaign");
	btnCampaign->setTitleFontSize(40);
	btnCampaign->setContentSize(Size(230, 100));
	btnCampaign->setPosition(Vec2(origin.x + visibleSize.width / 2,
			 	 	 	   	 	  labelAnn->getPosition().y - labelAnn->getContentSize().height / 2 - 130 - btnCampaign->getContentSize().height/2)
							);
	btnCampaign->addClickEventListener(CC_CALLBACK_1(MainScene::onMenuCampaignClicked, this));
	this->addChild(btnCampaign, 1);

	//Exit Button
	auto btnExit = ui::Button::create("btn-menu-0.png",
											  "btn-menu-1.png",
											  "btn-menu-2.png");
	btnExit->setScale9Enabled(true);
	btnExit->setCapInsets(Rect(15, 15, 4, 4));
	btnExit->setTitleText("Exit");
	btnExit->setTitleFontSize(40);
	btnExit->setContentSize(Size(230, 100));
	btnExit->setPosition(Vec2(origin.x + visibleSize.width / 2,
							  btnCampaign->getPosition().y - btnCampaign->getContentSize().height / 2 - 20 - btnExit->getContentSize().height/2)
							);
	btnExit->addClickEventListener(CC_CALLBACK_1(MainScene::onMenuExitClicked, this));
	this->addChild(btnExit, 1);
	return true;
}

void MainScene::onMenuCampaignClicked(Ref* pSender) {
	GameController::getInstance()->enterScene(GameController::eCampaignScene);
}

void MainScene::onMenuExitClicked(Ref* pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

