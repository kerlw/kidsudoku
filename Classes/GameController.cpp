/*
 * GameController.cpp
 *
 *  Created on: 2015年5月26日
 *      Author: kerlw
 */

#include "GameController.h"
#include "StageScene.h"
#include "StageData.h"

static GameController* s_pGameController;

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}
GameController::GameController() {
	// TODO Auto-generated constructor stub

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
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = StageScene::create();

    //TODO stage data should load from file
    int cell_data[] = {1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 2, 3, 1};
    StageData data;
    data.plt_file = "animals.plist";
    data.res_file = "animals.png";
    data.numbers = 4;
    data.scale = 1.0f;
    data.rows_per_grid = 2;
    data.cols_per_grid = 2;
    data.grids_in_col = 2;
    data.grids_in_row = 2;
    data.cell_data = cell_data;

    layer->loadStageData(&data);

    // add layer as a child to scene
    scene->addChild(layer);

    Director::getInstance()->replaceScene(scene);
}
