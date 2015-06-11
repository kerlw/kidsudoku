/*
 * GameController.cpp
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#include "GameController.h"
#include "StageScene.h"
#include "StageData.h"
#include "CampaignData.h"

static GameController* s_pGameController;

GameController::GameController()
	: m_pCampaign(nullptr) {
}
GameController::~GameController() {
	if (m_pCampaign)
		delete m_pCampaign;
}

GameController* GameController::getInstance() {
	if (!s_pGameController) {
		s_pGameController = new (std::nothrow) GameController();
		CCASSERT(s_pGameController, "FATAL: Not enough memory");
		s_pGameController->init();
	}
	return s_pGameController;
}

StageScene* GameController::getStageScene() {
	return nullptr;
}

bool GameController::init() {
	return false;
}

void GameController::nextStage() {
	if (!m_pCampaign)
		return;

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = StageScene::create();

    auto data = m_pCampaign->getNextStageData();
    if (data)
    	layer->loadStageData(data);

    // add layer as a child to scene
    scene->addChild(layer);

    Director::getInstance()->replaceScene(scene);
}
