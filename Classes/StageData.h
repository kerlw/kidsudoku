/*
 * StageData.h
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#ifndef STAGEDATA_H_
#define STAGEDATA_H_

#include "cocos2d.h"

#include <string>
#include <vector>
#include <cstdint>

USING_NS_CC;

class CampaignData;

class StageData {
public:
	std::string plt_file;
	std::string res_file;
	/** count of number */
	int numbers;

	int rows_per_grid;
	int cols_per_grid;

	int grids_in_row;
	int grids_in_col;

	std::uint8_t* cell_data;

	bool done;
	CampaignData* campaign;

	StageData() : done(false), cell_data(nullptr), campaign(nullptr) {}
	virtual ~StageData() {	delete cell_data;	}
	StageData& operator=(const StageData& dst);

	static StageData* create(int rows_per_grid, int cols_per_grid, int grids_in_row, int grids_in_col, std::uint8_t* puzzle);
};

/**
 * Helper class for StageData
 */
class StageDataHelper {
public:
	//TODO change the design resolution to support more resolutions.
//	static void setDesignResolution(float width, float height);

	/** calculate the stage scene's scale ratio from StageData */
	static float calcScaleRatio(const StageData* data);
};

inline StageData& StageData::operator =(const StageData& dst) {
	this->plt_file = dst.plt_file;
	this->res_file = dst.res_file;
	this->numbers = dst.numbers;
	this->rows_per_grid = dst.rows_per_grid;
	this->cols_per_grid = dst.cols_per_grid;
	this->grids_in_col = dst.grids_in_col;
	this->grids_in_row = dst.grids_in_row;

	int len = dst.numbers * dst.grids_in_row * dst.grids_in_col;
	this->cell_data = new std::uint8_t[len];
	memcpy(this->cell_data, dst.cell_data, len);
	return *this;
}

#endif /* STAGEDATA_H_ */
