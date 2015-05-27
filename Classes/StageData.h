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

USING_NS_CC;

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

	virtual ~StageData() {}
	StageData& operator=(const StageData& dst);
};

inline StageData& StageData::operator =(const StageData& dst) {
	this->plt_file = dst.plt_file;
	this->res_file = dst.res_file;
	this->scale = dst.scale;
	this->numbers = dst.numbers;
	this->rows_per_grid = dst.rows_per_grid;
	this->cols_per_grid = dst.cols_per_grid;
	this->grids_in_col = dst.grids_in_col;
	this->grids_in_row = dst.grids_in_row;
	this->cell_data = dst.cell_data;
	return *this;
}

class StageDataManager : public Ref {
public:
	virtual ~StageDataManager();
	static StageDataManager* getInstance();

	bool init();
	StageData* getNextStageData();

protected:
	std::vector<StageData> m_vctData;
	int m_iCurrentIndex;

private:
	StageDataManager();
};

#endif /* STAGEDATA_H_ */
