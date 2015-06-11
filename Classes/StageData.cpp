/*
 * StageData.cpp
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#include "StageData.h"
#include "Const.h"

float StageDataHelper::calcScaleRatio(const StageData& data) {
	int rows = data.rows_per_grid * data.grids_in_row;
	CCASSERT(rows > 0 && rows < 9, "Invalid rows calculated from stage data");
	if (rows <= 4) {
		return 1.0f;
	} else if (rows <= 6) {
		return 1.6f;
	} else if (rows <= 9) {
		return 2.0f;
	}
	return 1.0f;
}
