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

	if (point.x > m_iCols * CELL_SIZE || point.y > m_iRows * CELL_SIZE)
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
		//create the sprite and set the position.
		Sprite* sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FRAME_NAME[numberIndex]));
		sprite->ignoreAnchorPointForPosition(false);
		sprite->setAnchorPoint(Vec2(0, 1));
		sprite->setPosition(Vec2((pos % m_iCols) * CELL_SIZE, (m_iRows - pos / m_iCols) * CELL_SIZE));

		Sprite* old = m_mapSprites[pos];
		m_mapSprites[pos] = sprite;
		//remove the old one if there is an old sprite.
		if (old) {
			this->removeChild(old);
			old = nullptr;
		}
		this->addChild(sprite, 0);
	} else {
		//its a original cell, ignore
		log("position %d is a original cell", pos);
	}
}

