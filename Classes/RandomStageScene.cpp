/*
 * RandomStageScene.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: kerlw
 */

#include "RandomStageScene.h"
#include "GameController.h"

#include "ui/UIButton.h"

RandomStageScene::RandomStageScene() {
	m_vctSize.push_back("3x3");
	m_vctSize.push_back("4x4");
	m_vctSize.push_back("6x6");
	m_vctSize.push_back("8x8");
	m_vctSize.push_back("9x9");

	m_vctDifficulty.push_back("Easy");
	m_vctDifficulty.push_back("Normal");
	m_vctDifficulty.push_back("Hard");

	m_iIndexSize = 0;
	m_iIndexDifficulty = 0;
}

RandomStageScene::~RandomStageScene() {
	// TODO Auto-generated destructor stub
}

bool RandomStageScene::init() {
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	//size
	m_pLabelSize = Label::createWithTTF(m_vctSize[m_iIndexSize], "fonts/Marker Felt.ttf", 120);
	m_pLabelSize->setTextColor(Color4B(0xff, 0xff, 0x00, 0xff));
	m_pLabelSize->setPosition(Vec2(origin.x + visibleSize.width / 2,
						 	origin.y + visibleSize.height / 2 + m_pLabelSize->getContentSize().height)
						);
	this->addChild(m_pLabelSize);

	auto decSize = MenuItemImage::create("left-0.png", "left-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuDecreaseSizeCallback, this));
	decSize->setPosition(Vec2(origin.x + visibleSize.width / 2 - m_pLabelSize->getContentSize().width / 2 - 120,
							  origin.y + visibleSize.height / 2 + m_pLabelSize->getContentSize().height));

	auto incSize = MenuItemImage::create("right-0.png", "right-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuIncreaseSizeCallback, this));
	incSize->setPosition(Vec2(origin.x + visibleSize.width / 2 + m_pLabelSize->getContentSize().width / 2 + 120,
			  	  	  	  	  origin.y + visibleSize.height / 2 + m_pLabelSize->getContentSize().height));

	//difficulty
	m_pLabelDifficulty = Label::createWithTTF(m_vctDifficulty[m_iIndexDifficulty], "fonts/Marker Felt.ttf", 120);
	m_pLabelDifficulty->setTextColor(Color4B(0xff, 0xff, 0x00, 0xff));
	m_pLabelDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 2 - m_pLabelDifficulty->getContentSize().height / 2)
						);
	this->addChild(m_pLabelDifficulty);

	auto decDifficulty = MenuItemImage::create("left-0.png", "left-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuDecreaseDifficultyCallback, this));
	decDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2 - m_pLabelDifficulty->getContentSize().width / 2 - 120,
							  origin.y + visibleSize.height / 2 - m_pLabelDifficulty->getContentSize().height / 2));

	auto incDifficulty = MenuItemImage::create("right-0.png", "right-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuIncreaseDifficultyCallback, this));
	incDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2 + m_pLabelDifficulty->getContentSize().width / 2 + 120,
			  	  	  	  	  origin.y + visibleSize.height / 2 - m_pLabelDifficulty->getContentSize().height / 2));

	//Go Button
	auto btnGo = ui::Button::create("btn-menu-0.png", "btn-menu-1.png", "btn-menu-2.png");
	btnGo->setScale9Enabled(true);
	btnGo->setCapInsets(Rect(15, 15, 4, 4));
	btnGo->setTitleText("Go!");
	btnGo->setTitleFontSize(40);
	btnGo->setContentSize(Size(300, 90));
	btnGo->setPosition(Vec2(origin.x + visibleSize.width / 2,
							m_pLabelDifficulty->getPosition().y - m_pLabelDifficulty->getContentSize().height / 2 - btnGo->getContentSize().height - 20)
							);
	btnGo->addClickEventListener(CC_CALLBACK_1(RandomStageScene::onBtnGoClicked, this));
	this->addChild(btnGo, 1);


	//back menu item
    auto backItem = MenuItemImage::create("back.png", "back.png",
                                           CC_CALLBACK_1(RandomStageScene::menuBackCallback, this));

	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width/2 + 10,
                                origin.y + visibleSize.height - 10 - backItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, decSize, incSize, decDifficulty, incDifficulty, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
}

void RandomStageScene::menuDecreaseSizeCallback(Ref* pSender) {
	m_iIndexSize = (m_iIndexSize + m_vctSize.size() - 1) % m_vctSize.size();
	m_pLabelSize->setString(m_vctSize[m_iIndexSize]);
}

void RandomStageScene::menuIncreaseSizeCallback(Ref* pSender) {
	m_iIndexSize = (m_iIndexSize + 1) % m_vctSize.size();
	m_pLabelSize->setString(m_vctSize[m_iIndexSize]);
}

void RandomStageScene::menuDecreaseDifficultyCallback(Ref* pSender) {
	m_iIndexDifficulty = (m_iIndexDifficulty + m_vctDifficulty.size() - 1) % m_vctDifficulty.size();
	m_pLabelDifficulty->setString(m_vctDifficulty[m_iIndexDifficulty]);
}

void RandomStageScene::menuIncreaseDifficultyCallback(Ref* pSender) {
	m_iIndexDifficulty = (m_iIndexDifficulty + 1) % m_vctDifficulty.size();
	m_pLabelDifficulty->setString(m_vctDifficulty[m_iIndexDifficulty]);
}

void RandomStageScene::menuBackCallback(Ref* pSender) {
	GameController::getInstance()->leaveScene();
}

void RandomStageScene::onBtnGoClicked(Ref* pSender) {

}
