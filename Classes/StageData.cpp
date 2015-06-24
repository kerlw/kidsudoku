/*
 * StageData.cpp
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#include "StageData.h"
#include "Const.h"

float StageDataHelper::calcScaleRatio(const StageData* data) {
	if (!data)
		return 1.0f;

	int rows = data->rows_per_grid * data->grids_in_row;
	CCASSERT(rows > 0 && rows <= MAX_NUMBERS, "Invalid rows calculated from stage data");
	if (rows <= 4) {
		return 1.0f;
	} else if (rows <= 6) {
		return 0.6f;
	} else if (rows <= 9) {
		return 0.5f;
	}
	return 1.0f;
}

StageData* StageData::create(int rows_per_grid, int cols_per_grid, int grids_in_row, int grids_in_col, std::uint8_t* puzzle) {
	auto ret = new (std::nothrow) StageData();
	ret->numbers = rows_per_grid * cols_per_grid;
	ret->rows_per_grid = rows_per_grid;
	ret->cols_per_grid = cols_per_grid;
	ret->grids_in_row = grids_in_row;
	ret->grids_in_col = grids_in_col;

	int len = ret->numbers * grids_in_row * grids_in_col;
	ret->cell_data = new std::uint8_t[len];
	memcpy(ret->cell_data, puzzle, len);

	return ret;
}
