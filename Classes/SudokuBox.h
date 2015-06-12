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
#include <set>
#include <map>

USING_NS_CC;

class SudokuBox: public ui::Layout {
public:
	typedef struct operation {
		int pos;
		int oldValue;
		int value;
	} operation;
	SudokuBox();
	virtual ~SudokuBox();

	static SudokuBox* create();

	bool initWithStageData(const StageData& data);
	void onItemDragedAtPoint(const Vec2& point, int numberIndex);

	bool checkResult();
	bool checkData(const std::vector<int>& array);
	/** after check result, refresh error tips */
	void refreshErrorTipsLayer();

	void reset();
	void undo();

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
	void onDraw(const Mat4& transform);

CC_CONSTRUCTOR_ACCESS:
	virtual bool init() override	;
	bool containsPoint(const Vec2& point) const;
	void setNumber(int pos, int value);

private:
	int* m_pOrgData;
	int* m_pData;
	int m_iCols;
	int m_iRows;
	StageData m_stagedata;
	std::map<int, Sprite*> m_mapSprites;
    std::set<int> m_setErrors;
    std::map<int, Sprite*> m_mapErrorMask;
    CustomCommand m_command;

    std::vector<operation> m_vctOps;
};

#endif /* SUDOKUBOX_H_ */
