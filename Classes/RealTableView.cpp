/*
 * RealTableView.cpp
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#include "RealTableView.h"

RealTableView* RealTableView::create() {
    return RealTableView::create(nullptr, Size::ZERO);
}

RealTableView* RealTableView::create(RealTableViewDataSource* dataSource, Size size) {
    return RealTableView::create(dataSource, size, nullptr);
}

RealTableView* RealTableView::create(RealTableViewDataSource* dataSource, Size size, Node *container) {
	RealTableView *table = new (std::nothrow) RealTableView();
    table->m_iCols = size.width / dataSource->cellSizeForTable(table).width;

    log("cols is %d", table->m_iCols);
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
//    table->_updateCellPositions();
    table->_updateContentSize();

    return table;
}

bool RealTableView::initWithViewSize(Size size, Node* container/* = nullptr*/) {
    if (ScrollView::initWithViewSize(size, container)) {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        _vordering      = TableView::VerticalFillOrder::TOP_DOWN;
        this->setDirection(Direction::VERTICAL);

        ScrollView::setDelegate(this);
        return true;
    }
    return false;
}

RealTableView::RealTableView()
	: _touchedCell(nullptr)
	, _indices(nullptr)
	, _dataSource(nullptr)
	, _tableViewDelegate(nullptr)
	, _oldDirection(Direction::NONE)
	, _isUsedCellsDirty(false) {
}

RealTableView::~RealTableView() {
    CC_SAFE_DELETE(_indices);
}

void RealTableView::setVerticalFillOrder(TableView::VerticalFillOrder fillOrder) {
	if (_vordering != fillOrder) {
		_vordering = fillOrder;
		if (!_cellsUsed.empty()) {
			this->reloadData();
		}
	}
}

void RealTableView::reloadData() {
    _oldDirection = Direction::NONE;

    for(const auto &cell : _cellsUsed) {
        if(_tableViewDelegate != nullptr) {
            _tableViewDelegate->tableCellWillRecycle(this, cell);
        }

        _cellsFreed.pushBack(cell);

        cell->reset();
        if (cell->getParent() == this->getContainer()) {
            this->getContainer()->removeChild(cell, true);
        }
    }

    _indices->clear();
    _cellsUsed.clear();

//    this->_updateCellPositions();
    this->_updateContentSize();
    if (_dataSource->numberOfCellsInTableView(this) > 0) {
        this->scrollViewDidScroll(this);
    }
}

TableViewCell *RealTableView::cellAtIndex(ssize_t idx)
{
    if (_indices->find(idx) != _indices->end())
    {
        for (const auto& cell : _cellsUsed)
        {
            if (cell->getIdx() == idx)
            {
                return cell;
            }
        }
    }

    return nullptr;
}

void RealTableView::updateCellAtIndex(ssize_t idx) {
    if (idx == CC_INVALID_INDEX) {
        return;
    }
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx >= countOfItems) {
        return;
    }

    TableViewCell* cell = this->cellAtIndex(idx);
    if (cell) {
        this->_moveCellOutOfSight(cell);
    }
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void RealTableView::insertCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }

    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }

    long newIdx = 0;

    auto cell = cellAtIndex(idx);
    if (cell)
    {
        newIdx = _cellsUsed.getIndex(cell);
        // Move all cells behind the inserted position
        for (long i = newIdx; i < _cellsUsed.size(); i++)
        {
            cell = _cellsUsed.at(i);
            this->_setIndexForCell(cell->getIdx()+1, cell);
        }
    }

    //insert a new cell
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);

//    this->_updateCellPositions();
    this->_updateContentSize();
}

void RealTableView::removeCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }

    long uCountOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }

    ssize_t newIdx = 0;

    TableViewCell* cell = this->cellAtIndex(idx);
    if (!cell)
    {
        return;
    }

    newIdx = _cellsUsed.getIndex(cell);

    //remove first
    this->_moveCellOutOfSight(cell);

    _indices->erase(idx);
//    this->_updateCellPositions();

    for (ssize_t i = _cellsUsed.size()-1; i > newIdx; i--)
    {
        cell = _cellsUsed.at(i);
        this->_setIndexForCell(cell->getIdx()-1, cell);
    }
}

TableViewCell *RealTableView::dequeueCell()
{
    TableViewCell *cell;

    if (_cellsFreed.empty()) {
        cell = nullptr;
    } else {
        cell = _cellsFreed.at(0);
        cell->retain();
        _cellsFreed.erase(0);
        cell->autorelease();
    }
    return cell;
}

void RealTableView::_addCellIfNecessary(TableViewCell * cell) {
    if (cell->getParent() != this->getContainer()) {
        this->getContainer()->addChild(cell);
    }
    _cellsUsed.pushBack(cell);
    _indices->insert(cell->getIdx());
    _isUsedCellsDirty = true;
}

void RealTableView::_updateContentSize() {
	ssize_t count = _dataSource->numberOfCellsInTableView(this);
	int rows = (count + m_iCols - 1) / m_iCols;

	Size size = _dataSource->cellSizeForTable(this);
	this->setContentSize(Size(size.width * m_iCols, size.height * rows));
	this->setContentOffset(Vec2(0,0));
}

Vec2 RealTableView::_offsetFromIndex(ssize_t index) {
	int rows = index / m_iCols;
	int cols = index % m_iCols;

	Size size = _dataSource->cellSizeForTable(this);
	Vec2 offset(size.width * cols, size.height * rows);
	log("%d cell offset is %d,%d", index, offset.x, offset.y);
	return offset;
}

//Vec2 RealTableView::__offsetFromIndex(ssize_t index)
//{
//    Vec2 offset;
//    Size  cellSize;
//
//    switch (this->getDirection())
//    {
//        case Direction::HORIZONTAL:
//            offset.set(_vCellsPositions[index], 0.0f);
//            break;
//        default:
//            offset.set(0.0f, _vCellsPositions[index]);
//            break;
//    }
//
//    return offset;
//}

long RealTableView::_indexFromOffset(Vec2 offset)
{
//    long index = 0;
//    const long maxIdx = _dataSource->numberOfCellsInTableView(this) - 1;
//
//    if (_vordering == VerticalFillOrder::TOP_DOWN)
//    {
//        offset.y = this->getContainer()->getContentSize().height - offset.y;
//    }
//    index = this->__indexFromOffset(offset);
//    if (index != -1)
//    {
//        index = MAX(0, index);
//        if (index > maxIdx)
//        {
//            index = CC_INVALID_INDEX;
//        }
//    }
//
//    return index;
	Size size = _dataSource->cellSizeForTable(this);
	int rows = offset.y / size.height;
	int cols = offset.x / size.width;
	return rows * m_iCols + cols;
}

//long RealTableView::__indexFromOffset(Vec2 offset)
//{
//    long low = 0;
//    long high = _dataSource->numberOfCellsInTableView(this) - 1;
//    float search;
//    switch (this->getDirection())
//    {
//        case Direction::HORIZONTAL:
//            search = offset.x;
//            break;
//        default:
//            search = offset.y;
//            break;
//    }
//
//    while (high >= low)
//    {
//        long index = low + (high - low) / 2;
//        float cellStart = _vCellsPositions[index];
//        float cellEnd = _vCellsPositions[index + 1];
//
//        if (search >= cellStart && search <= cellEnd)
//        {
//            return index;
//        }
//        else if (search < cellStart)
//        {
//            high = index - 1;
//        }
//        else
//        {
//            low = index + 1;
//        }
//    }
//
//    if (low <= 0) {
//        return 0;
//    }
//
//    return -1;
//}

void RealTableView::_moveCellOutOfSight(TableViewCell *cell)
{
    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->tableCellWillRecycle(this, cell);
    }

    _cellsFreed.pushBack(cell);
    _cellsUsed.eraseObject(cell);
    _isUsedCellsDirty = true;

    _indices->erase(cell->getIdx());
    cell->reset();

    if (cell->getParent() == this->getContainer())
    {
        this->getContainer()->removeChild(cell, true);;
    }
}

void RealTableView::_setIndexForCell(ssize_t index, TableViewCell *cell) {
    cell->setAnchorPoint(Vec2(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}

//void RealTableView::_updateCellPositions()
//{
//    long cellsCount = _dataSource->numberOfCellsInTableView(this);
//    _vCellsPositions.resize(cellsCount + 1, 0.0);
//
//    if (cellsCount > 0)
//    {
//        float currentPos = 0;
//        Size cellSize;
//        for (int i=0; i < cellsCount; i++)
//        {
//            _vCellsPositions[i] = currentPos;
//            cellSize = _dataSource->tableCellSizeForIndex(this, i);
//            switch (this->getDirection())
//            {
//                case Direction::HORIZONTAL:
//                    currentPos += cellSize.width;
//                    break;
//                default:
//                    currentPos += cellSize.height;
//                    break;
//            }
//        }
//        _vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
//    }
//
//}

void RealTableView::scrollViewDidScroll(ScrollView* view) {
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems)
        return;

    if (_isUsedCellsDirty) {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](TableViewCell *a, TableViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }

    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->scrollViewDidScroll(this);
    }

    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);

    if (_vordering == TableView::VerticalFillOrder::TOP_DOWN) {
        offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX) {
		startIdx = countOfItems - 1;
	}

    if (_vordering == TableView::VerticalFillOrder::TOP_DOWN) {
        offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    } else {
        offset.y += _viewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += _viewSize.width/this->getContainer()->getScaleX();

    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX) {
		endIdx = countOfItems - 1;
	}

#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.at(0);
        idx = cell->getIdx();

        while(idx < startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.back();
        idx = cell->getIdx();

        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }

    for (long i = startIdx; i <= endIdx; i++)
    {
        if (_indices->find(i) != _indices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}

void RealTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }

    if (_touchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);

		if (bb.containsPoint(pTouch->getLocation()) && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
            _tableViewDelegate->tableCellTouched(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }

    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool RealTableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    for (Node *c = this; c != nullptr; c = c->getParent())
    {
        if (!c->isVisible())
        {
            return false;
        }
    }

    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);

    if(_touches.size() == 1)
    {
        long index;
        Vec2 point;

        point = this->getContainer()->convertTouchToNodeSpace(pTouch);

        index = this->_indexFromOffset(point);
		if (index == CC_INVALID_INDEX)
		{
			_touchedCell = nullptr;
		}
        else
		{
			_touchedCell  = this->cellAtIndex(index);
		}

        if (_touchedCell && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellHighlight(this, _touchedCell);
        }
    }
    else if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }

    return touchResult;
}

void RealTableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);

    if (_touchedCell && isTouchMoved())
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }
}

void RealTableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);

    if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }
}
