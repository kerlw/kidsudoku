/*
 * SudokuBox.cpp
 *
 *  Created on: 2015年5月25日
 *      Author: kerlw
 */

#include "SudokuBox.h"
#include "Const.h"

SudokuBox::SudokuBox()
	: m_pData(nullptr),
	  m_pOrgData(nullptr) {
}

SudokuBox::~SudokuBox() {
	CC_SAFE_DELETE_ARRAY(m_pData);
	CC_SAFE_DELETE_ARRAY(m_pOrgData);
}

SudokuBox* SudokuBox::create() {
	auto box = new (std::nothrow) SudokuBox();
	if (box && box->init()) {
		box->autorelease();
		return box;
	}
	CC_SAFE_DELETE(box);
	return nullptr;
}

bool SudokuBox::init() {
	if (!Layout::init())
		return false;

	this->setBackGroundColor(Color3B(255, 255, 255));
	this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5, 0.5));
	return true;
}

bool SudokuBox::initWithStageData(const StageData& data) {
	m_stagedata = data;
	m_iCols = data.cols_per_grid * data.grids_in_col;
	m_iRows = data.rows_per_grid * data.grids_in_row;
	if (m_iCols <= 0 || m_iRows <= 0)
		return false;

	this->setContentSize(Size(m_iCols * CELL_SIZE, m_iRows * CELL_SIZE));

	int len = m_iCols * m_iRows;
	m_pData = new int[len];
	m_pOrgData = new int[len];

	memcpy(m_pData, data.cell_data, len * sizeof(int));
	memcpy(m_pOrgData, data.cell_data, len * sizeof(int));

	for (int index = 0; index < len; index++) {
		int value = m_pOrgData[index] - 1;
		if (value < 0 || value >= 9)
			continue;

		Sprite* sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_NAME[value]));
		sprite->ignoreAnchorPointForPosition(false);
		sprite->setAnchorPoint(Vec2(0, 1));
		sprite->setPosition(Vec2((index % m_iCols) * CELL_SIZE, (m_iRows - index / m_iCols) * CELL_SIZE));

		this->addChild(sprite);
	}
}

bool SudokuBox::containsPoint(const Vec2& point) const {
	log("check if sudokuBox contains point %f,%f", point.x, point.y);
	if (m_iCols <= 0 || m_iRows <= 0)
		return false;

	if (point.x < 0 || point.y < 0 || point.x > m_iCols * CELL_SIZE || point.y > m_iRows * CELL_SIZE)
		return false;

	return true;
}

void SudokuBox::onItemDragedAtPoint(const Vec2& point, int numberIndex) {
	log("SudokuBox::onItemDragedAtPoint with numberIndex is %d", numberIndex);
	//check if this point in the box at first.
	if (!containsPoint(point))
		return;

	//calculate the position in box for this point.
	int pos = m_iCols * (m_iRows - ((int)point.y / CELL_SIZE) - 1) + ((int) point.x / CELL_SIZE);
	log("calculated the pos is %d", pos);
	if (m_pOrgData[pos] <= 0) {
		// if not changed, return
		if (m_pData[pos] == numberIndex + 1)
			return;

		m_pData[pos] = numberIndex + 1;

		//create the sprite and set the position.
		Sprite* sprite = numberIndex >= 0 ?
				Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_NAME[numberIndex]))
					: nullptr;

		//remove the old one if there is an old sprite.
		Sprite* old = m_mapSprites[pos];
		m_mapSprites[pos] = sprite;
		if (old) {
			this->removeChild(old);
			old = nullptr;
		}

		//add the new one if it is not null.
		if (sprite) {
			sprite->ignoreAnchorPointForPosition(false);
			sprite->setAnchorPoint(Vec2(0, 1));
			sprite->setPosition(Vec2((pos % m_iCols) * CELL_SIZE, (m_iRows - pos / m_iCols) * CELL_SIZE));
			this->addChild(sprite, 0);
		}
	} else {
		//its a original cell, ignore
		log("position %d is a original cell", pos);
	}
}

bool SudokuBox::checkResult() {
	bool ret = true;
	//XXX optimization: only check the error index and changed index.
	m_setErrors.clear();

	std::vector<int> index_array;
	//1.check each row
	for (int row = 0; row < m_iRows; row++) {
		index_array.clear();
		for (int col = 0; col < m_iCols; col++) {
			index_array.push_back(m_iCols * row + col);
		}
		ret &= checkData(index_array);
	}

	//2.check each column
	for (int col = 0; col < m_iCols; col++) {
		index_array.clear();
		for (int row = 0; row < m_iRows; row++) {
			index_array.push_back(m_iCols * row + col);
		}

		ret &= checkData(index_array);
	}

	//3.check each grid
	if (m_stagedata.cols_per_grid > 1 && m_stagedata.rows_per_grid > 1) {
		int count = m_stagedata.grids_in_col * m_stagedata.grids_in_row;
//		int cell_count = m_stagedata.rows_per_grid * m_stagedata.cols_per_grid;
		for (int grid = 0; grid < count; grid++) {
			index_array.clear();
			int start_col = (grid % m_stagedata.grids_in_col) * m_stagedata.cols_per_grid;
			int start_row = (grid / m_stagedata.grids_in_col) * m_stagedata.rows_per_grid;
			int start = start_row * m_iCols + start_col;
			for (int row = 0; row < m_stagedata.rows_per_grid; row++) {
				for (int col = 0; col < m_stagedata.cols_per_grid; col++) {
					index_array.push_back(start + row * m_iCols + col);
				}
			}
			ret &= checkData(index_array);
		}
	}

	//4.check if each cell filled with valid number
	for (int index = 0; index < m_iRows*m_iCols; index++) {
		if (m_pData[index] <= 0)
			return false;
	}

	return ret;
}

bool SudokuBox::checkData(const std::vector<int>& array) {
	// value -> index map
	std::map<int, int> mapIndex;

	bool ret = true;
	auto it = array.begin();
	for (;it != array.end(); ++it) {
		int value = m_pData[*it];
		if (value > 0) {
			//if the value has already been mapped in the map, that means error.
			auto mit = mapIndex.find(value);
			if (mit != mapIndex.end()) {
				//add these indexes to error index set.
				m_setErrors.insert(*it);
				m_setErrors.insert(mit->second);
				ret = false;
			} else
				mapIndex[value] = *it;
		}
	}
	return ret;
}

void SudokuBox::refreshErrorTipsLayer() {
	auto it = m_setErrors.begin();
	for (; it != m_setErrors.end(); ++it) {
		int pos = *it;
		if (m_mapErrorMask.find(pos) == m_mapErrorMask.end()) {
			//add error mask over the sprite
			auto mask = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_NAME[ERROR_MASK_INDEX])
				);
			mask->ignoreAnchorPointForPosition(false);
			mask->setAnchorPoint(Vec2(0, 1));
			mask->setPosition(Vec2((pos % m_iCols) * CELL_SIZE, (m_iRows - pos / m_iCols) * CELL_SIZE));
			this->addChild(mask, 1);

			//add the mask to the position-mask map
			m_mapErrorMask[pos] = mask;
		}
	}

	//remove those mask which is not error any longer.
	auto mit = m_mapErrorMask.begin();
	while (mit != m_mapErrorMask.end()) {
		if (m_setErrors.find(mit->first) == m_setErrors.end()) {
			this->removeChild(mit->second, true);
			auto tmp = mit;
			mit++;
			m_mapErrorMask.erase(tmp);
		} else {
			mit++;
		}
	}
}
