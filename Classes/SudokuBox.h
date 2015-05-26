/*
 * SudokuBox.h
 *
 *  Created on: 2015年5月25日
 *      Author: kerlw
 */

#ifndef SUDOKUBOX_H_
#define SUDOKUBOX_H_

#include "cocos2d.h"
#include "StageData.h"
#include "ui/UILayout.h"
#include <vector>

USING_NS_CC;

class SudokuBox: public ui::Layout {
public:
	SudokuBox();
	virtual ~SudokuBox();

	static SudokuBox* create();

	bool initWithStageData(const StageData& data);
	void onItemDragedAtPoint(const Vec2& point, int numberIndex);

	bool checkResult();
	bool checkData(const std::vector<int>& array);

CC_CONSTRUCTOR_ACCESS:
	 virtual bool init() override;
	 bool containsPoint(const Vec2& point) const;

	 int* m_pOrgData;
	 int* m_pData;
	 int m_iCols;
	 int m_iRows;
	 StageData m_stagedata;
	 std::map<int, Sprite*> m_mapSprites;
};

#endif /* SUDOKUBOX_H_ */
