/*
 * StageData.h
 *
 *  Created on: 2015年5月21日
 *      Author: kerlw
 */

#ifndef STAGEDATA_H_
#define STAGEDATA_H_

#include <string>

class StageData {
public:
	std::string plt_file;
	std::string res_file;
	float scale;
	/** count of number */
	int numbers;

	int rows_per_grid;
	int cols_per_grid;

	int grids_in_row;
	int grids_in_col;

	int* cell_data;
};

#endif /* STAGEDATA_H_ */
