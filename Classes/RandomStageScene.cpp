/*
 * RandomStageScene.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: kerlw
 */

#include "RandomStageScene.h"
#include "GameController.h"
#include "SudokuUtil.h"
#include "StageData.h"
#include "CampaignData.h"

#include "ui/UIButton.h"

static const int NUMBER_OF_BOX_SIZE = 5;
static const struct BoxSizeData g_boxes[NUMBER_OF_BOX_SIZE] = {
		{"3x3", 1, 3, 3, 1},
		{"4x4", 2, 2, 2, 2},
		{"6x6", 2, 3, 3, 2},
		{"8x8", 2, 4, 4, 2},
		{"9x9", 3, 3, 3, 3},
};

RandomStageScene::RandomStageScene() {
	for (int i = 0; i < NUMBER_OF_BOX_SIZE; i++)
		m_vctSize.push_back(g_boxes[i].desc);

	m_vctDifficulty.push_back("Easy");
	m_vctDifficulty.push_back("Normal");
	m_vctDifficulty.push_back("Hard");
	m_vctDifficulty.push_back("Evil");

	m_vctResource.push_back("Number");
	m_vctResource.push_back("Animal");
	m_vctResource.push_back("Fruit");

	m_iIndexSize = 0;
	m_iIndexDifficulty = 0;
	m_iIndexResource = 0;
}

RandomStageScene::~RandomStageScene() {
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
						 	origin.y + visibleSize.height * 4 / 5)
						);
	this->addChild(m_pLabelSize);

	auto decSize = MenuItemImage::create("left-0.png", "left-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuDecreaseSizeCallback, this));
	decSize->setPosition(Vec2(origin.x + visibleSize.width / 2 - m_pLabelSize->getContentSize().width / 2 - 120,
							  origin.y + visibleSize.height * 4 / 5));

	auto incSize = MenuItemImage::create("right-0.png", "right-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuIncreaseSizeCallback, this));
	incSize->setPosition(Vec2(origin.x + visibleSize.width / 2 + m_pLabelSize->getContentSize().width / 2 + 120,
			  	  	  	  	  origin.y + visibleSize.height * 4 / 5));

	//difficulty
	m_pLabelDifficulty = Label::createWithTTF(m_vctDifficulty[m_iIndexDifficulty], "fonts/Marker Felt.ttf", 120);
	m_pLabelDifficulty->setTextColor(Color4B(0xff, 0xff, 0x00, 0xff));
	m_pLabelDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height * 3 / 5)
						);
	this->addChild(m_pLabelDifficulty);

	auto decDifficulty = MenuItemImage::create("left-0.png", "left-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuDecreaseDifficultyCallback, this));
	decDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2 - m_pLabelDifficulty->getContentSize().width / 2 - 120,
							  origin.y + visibleSize.height * 3 / 5));

	auto incDifficulty = MenuItemImage::create("right-0.png", "right-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuIncreaseDifficultyCallback, this));
	incDifficulty->setPosition(Vec2(origin.x + visibleSize.width / 2 + m_pLabelDifficulty->getContentSize().width / 2 + 120,
			  	  	  	  	  origin.y + visibleSize.height * 3 / 5));

	//resource
	m_pLabelResource = Label::createWithTTF(m_vctResource[m_iIndexResource], "fonts/Marker Felt.ttf", 120);
	m_pLabelResource->setTextColor(Color4B(0xff, 0xff, 0x00, 0xff));
	m_pLabelResource->setPosition(Vec2(origin.x + visibleSize.width / 2,
						 	origin.y + visibleSize.height  * 2 / 5)
						);
	this->addChild(m_pLabelResource);

	auto prevRes = MenuItemImage::create("left-0.png", "left-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuPrevResourceCallback, this));
	prevRes->setPosition(Vec2(decDifficulty->getPosition().x,
							  origin.y + visibleSize.height * 2 / 5));

	auto nextRes = MenuItemImage::create("right-0.png", "right-1.png",
										  CC_CALLBACK_1(RandomStageScene::menuNextResourceCallback, this));
	nextRes->setPosition(Vec2(incDifficulty->getPosition().x,
			  	  	  	  	  origin.y + visibleSize.height * 2 / 5));

	//Go Button
	auto btnGo = ui::Button::create("btn-menu-0.png", "btn-menu-1.png", "btn-menu-2.png");
	btnGo->setScale9Enabled(true);
	btnGo->setCapInsets(Rect(15, 15, 4, 4));
	btnGo->setTitleText("Go!");
	btnGo->setTitleFontSize(40);
	btnGo->setContentSize(Size(300, 90));
	btnGo->setPosition(Vec2(origin.x + visibleSize.width / 2,
							m_pLabelResource->getPosition().y - m_pLabelResource->getContentSize().height / 2 - btnGo->getContentSize().height - 20)
							);
	btnGo->addClickEventListener(CC_CALLBACK_1(RandomStageScene::onBtnGoClicked, this));
	this->addChild(btnGo, 1);


	//back menu item
    auto backItem = MenuItemImage::create("back.png", "back.png",
                                           CC_CALLBACK_1(RandomStageScene::menuBackCallback, this));

	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width/2 + 10,
                                origin.y + visibleSize.height - 10 - backItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, decSize, incSize, decDifficulty, incDifficulty, prevRes, nextRes, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
}

//size
void RandomStageScene::menuDecreaseSizeCallback(Ref* pSender) {
	m_iIndexSize = (m_iIndexSize + m_vctSize.size() - 1) % m_vctSize.size();
	m_pLabelSize->setString(m_vctSize[m_iIndexSize]);
}

void RandomStageScene::menuIncreaseSizeCallback(Ref* pSender) {
	m_iIndexSize = (m_iIndexSize + 1) % m_vctSize.size();
	m_pLabelSize->setString(m_vctSize[m_iIndexSize]);
}

//difficulty
void RandomStageScene::menuDecreaseDifficultyCallback(Ref* pSender) {
	m_iIndexDifficulty = (m_iIndexDifficulty + m_vctDifficulty.size() - 1) % m_vctDifficulty.size();
	m_pLabelDifficulty->setString(m_vctDifficulty[m_iIndexDifficulty]);
}

void RandomStageScene::menuIncreaseDifficultyCallback(Ref* pSender) {
	m_iIndexDifficulty = (m_iIndexDifficulty + 1) % m_vctDifficulty.size();
	m_pLabelDifficulty->setString(m_vctDifficulty[m_iIndexDifficulty]);
}

//resource
void RandomStageScene::menuPrevResourceCallback(Ref* pSender) {
	m_iIndexResource = (m_iIndexResource + m_vctResource.size() - 1) % m_vctResource.size();
	m_pLabelResource->setString(m_vctResource[m_iIndexResource]);
}

void RandomStageScene::menuNextResourceCallback(Ref* pSender) {
	m_iIndexResource = (m_iIndexResource + 1) % m_vctResource.size();
	m_pLabelResource->setString(m_vctResource[m_iIndexResource]);
}

//back
void RandomStageScene::menuBackCallback(Ref* pSender) {
	GameController::getInstance()->leaveScene();
}

//go
void RandomStageScene::onBtnGoClicked(Ref* pSender) {
	auto generator = SudokuGenerator::getInstance();
	generator->setPuzzleDifficulty(static_cast<Difficulty>(m_iIndexDifficulty));
	generator->setSize(g_boxes[m_iIndexSize].rows_per_grid, g_boxes[m_iIndexSize].cols_per_grid,
				g_boxes[m_iIndexSize].grids_in_row, g_boxes[m_iIndexSize].grids_in_col);

	if (generator->generate()) {
		auto data = StageData::create(g_boxes[m_iIndexSize].rows_per_grid, g_boxes[m_iIndexSize].cols_per_grid,
									g_boxes[m_iIndexSize].grids_in_row, g_boxes[m_iIndexSize].grids_in_col,
									generator->getPuzzle());
		switch (m_iIndexResource) {
		case 0:
			//default res, do nothing.
			break;
		case 1:
			data->plt_file = "animals.plist";
			data->res_file = "animals.png";
			break;
		case 2:
			data->plt_file = "fruits.plist";
			data->res_file = "fruits.png";
			break;
		}
		auto campaign = CampaignData::createRandomCampaign(data);
		GameController::getInstance()->setCampaignData(campaign);
		GameController::getInstance()->enterScene(GameController::SceneType::eStageScene);
	}
}
