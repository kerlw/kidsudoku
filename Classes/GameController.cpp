/*
 * GameController.cpp
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#include "GameController.h"
#include "MainScene.h"
#include "CampaignScene.h"
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

    auto data = m_pCampaign->getNextStageData();
    // 'layer' is an autorelease object
    auto layer = StageScene::create(data);

    // add layer as a child to scene
    scene->addChild(layer);

    Director::getInstance()->replaceScene(scene);
}

void GameController::enterScene(const SceneType& eType) {
	auto scene = Scene::create();

	Layer* layer = nullptr;
	switch (eType) {
	case eMainScene:
		layer = MainScene::create();
		break;
	case eCampaignScene:
		layer = CampaignScene::create(m_pCampaign);
		break;
	case eStageScene:
	{
		auto data = m_pCampaign->currentStageData();
		layer = StageScene::create(data);
		break;
	}
	}

	if (layer) {
		scene->addChild(layer);
		Director::getInstance()->pushScene(scene);
	}
}

void GameController::leaveScene() {
	Director::getInstance()->popScene();
}
