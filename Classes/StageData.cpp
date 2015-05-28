/*
 * StageData.cpp
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#include "StageData.h"
#include "Const.h"

static StageDataManager* s_pStageDataManager;

float StageDataHelper::calcScaleRatio(const StageData& data) {
	int rows = data.rows_per_grid * data.grids_in_row;
	CCASSERT(rows > 0 && rows < 9, "Invalid rows calculated from stage data");
	if (rows <= 4) {
		return 1.0f;
	} else if (rows <= 9) {
		return 2.0f;
	}
	return 1.0f;
}

StageDataManager::StageDataManager()
	: m_iCurrentIndex(-1) {
}

StageDataManager::~StageDataManager() {
}

StageDataManager* StageDataManager::getInstance() {
	if (!s_pStageDataManager) {
		s_pStageDataManager = new (std::nothrow) StageDataManager();
		CCASSERT(s_pStageDataManager, "FATAL: Not enough memory");
		s_pStageDataManager->init();
	}
	return s_pStageDataManager;
}

bool StageDataManager::init() {
	//TODO StageData should load from file.
    StageData data;
    data.plt_file = "animals.plist";
    data.res_file = "animals.png";
    data.numbers = 3;
    data.rows_per_grid = 1;
    data.cols_per_grid = 3;
    data.grids_in_col = 1;
    data.grids_in_row = 2;
    data.cell_data = new int[6] {1, 0, 2, 3, 0, 0};
    m_vctData.push_back(data);

    data.plt_file = "fruits.plist";
    data.res_file = "fruits.png";
    data.numbers = 3;
    data.rows_per_grid = 1;
    data.cols_per_grid = 3;
    data.grids_in_col = 1;
    data.grids_in_row = 2;
    data.cell_data = new int[6] {0, 0, 2, 3, 0, 0};
    m_vctData.push_back(data);

    data.plt_file = "animals.plist";
    data.res_file = "animals.png";
    data.numbers = 3;
    data.rows_per_grid = 1;
    data.cols_per_grid = 3;
    data.grids_in_col = 1;
    data.grids_in_row = 3;
    data.cell_data = new int[9] {1, 0, 0, 0, 0, 3, 3, 0, 0};
    m_vctData.push_back(data);

	data.plt_file = "fruits.plist";
	data.res_file = "fruits.png";
	data.numbers = 4;
	data.rows_per_grid = 2;
	data.cols_per_grid = 2;
	data.grids_in_col = 2;
	data.grids_in_row = 1;
    data.cell_data = new int[8] {1, 0, 0, 4, 4, 3, 0, 2};
    m_vctData.push_back(data);

	data.plt_file = "fruits.plist";
	data.res_file = "fruits.png";
	data.numbers = 4;
	data.rows_per_grid = 2;
	data.cols_per_grid = 2;
	data.grids_in_col = 2;
	data.grids_in_row = 1;
    data.cell_data = new int[8] {1, 0, 0, 2, 0, 3, 1, 0};
    m_vctData.push_back(data);

	data.plt_file = "fruits.plist";
	data.res_file = "fruits.png";
	data.numbers = 4;
	data.rows_per_grid = 2;
	data.cols_per_grid = 2;
	data.grids_in_col = 2;
	data.grids_in_row = 2;
    data.cell_data = new int[16] {1, 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 0, 2, 3, 1};
    m_vctData.push_back(data);
}

StageData* StageDataManager::getNextStageData() {
	if (m_iCurrentIndex < 0)
		m_iCurrentIndex = 0;
	if (m_iCurrentIndex >= m_vctData.size())
		return nullptr;

	return &(m_vctData[m_iCurrentIndex++]);
}
