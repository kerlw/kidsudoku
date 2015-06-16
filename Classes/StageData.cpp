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
