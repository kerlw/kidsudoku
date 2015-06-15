/*
 * RealTableView.h
 *
 *  Created on: 2015年6月15日
 *      Author: kerlw
 */

#ifndef REALTABLEVIEW_H_
#define REALTABLEVIEW_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RealTableView;

/**
 * Sole purpose of this delegate is to single touch event in this version.
 */
class RealTableViewDelegate : public ScrollViewDelegate
{
public:
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     * @js NA
     * @lua NA
     */
    virtual void tableCellTouched(RealTableView* table, TableViewCell* cell) = 0;

    /**
     * Delegate to respond a table cell press event.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     * @js NA
     * @lua NA
     */
    virtual void tableCellHighlight(RealTableView* table, TableViewCell* cell){};

    /**
     * Delegate to respond a table cell release event
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     * @js NA
     * @lua NA
     */
    virtual void tableCellUnhighlight(RealTableView* table, TableViewCell* cell){};

    /**
     * Delegate called when the cell is about to be recycled. Immediately
     * after this call the cell will be removed from the scene graph and
     * recycled.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     * @js NA
     * @lua NA
     */
    virtual void tableCellWillRecycle(RealTableView* table, TableViewCell* cell){};

};


/**
 * Data source that governs table backend data.
 */
class RealTableViewDataSource
{
public:
    /**
     * @js NA
     * @lua NA
     */
    virtual ~RealTableViewDataSource() {}

    /**
     * cell size for a given index
     *
     * @param idx the index of a cell to get a size
     * @return size of a cell at given index
     */
    virtual Size tableCellSizeForIndex(RealTableView *table, ssize_t idx) {
        return cellSizeForTable(table);
    };
    /**
     * cell height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return cell size
     */
    virtual Size cellSizeForTable(RealTableView *table) {
        return Size::ZERO;
    };
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual TableViewCell* tableCellAtIndex(RealTableView *table, ssize_t idx) = 0;
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual ssize_t numberOfCellsInTableView(RealTableView *table) = 0;

};

class RealTableView: public ScrollView, public ScrollViewDelegate {
public:
	RealTableView();
	virtual ~RealTableView();

    /** Empty contructor of TableView */
    static RealTableView* create();

    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     * @code
     * when this function bound to js or lua,the input params are changed
     * in js:var create(var jsObject,var size)
     * in lua:local create(var size)
     * in lua:
     * @endcode
     */
    static RealTableView* create(RealTableViewDataSource* dataSource, Size size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     * @code
     * when this function bound to js or lua,the input params are changed
     * in js:var create(var jsObject,var size,var container)
     * in lua:local create(var size, var container)
     * in lua:
     * @endcode
     */
    static RealTableView* create(RealTableViewDataSource* dataSource, Size size, Node *container);

    bool initWithViewSize(Size size, Node* container = NULL);

    /**
     * data source
     * @js NA
     * @lua NA
     */
    RealTableViewDataSource* getDataSource() { return _dataSource; }
    /**
     * when this function bound to js or lua,the input params are changed
     * in js:var setDataSource(var jsSource)
     * in lua:local setDataSource()
     * @endcode
     */
    void setDataSource(RealTableViewDataSource* source) { _dataSource = source; }
    /**
     * delegate
     * @js NA
     * @lua NA
     */
    RealTableViewDelegate* getDelegate() { return _tableViewDelegate; }
    /**
     * @code
     * when this function bound to js or lua,the input params are changed
     * in js:var setDelegate(var jsDelegate)
     * in lua:local setDelegate()
     * @endcode
     */
    void setDelegate(RealTableViewDelegate* pDelegate) { _tableViewDelegate = pDelegate; }

    /**
     * determines how cell is ordered and filled in the view.
     */
    void setVerticalFillOrder(TableView::VerticalFillOrder order);
    TableView::VerticalFillOrder getVerticalFillOrder() { return _vordering; }

    /**
     * Updates the content of the cell at a given index.
     *
     * @param idx index to find a cell
     */
    void updateCellAtIndex(ssize_t idx);
    /**
     * Inserts a new cell at a given index
     *
     * @param idx location to insert
     */
    void insertCellAtIndex(ssize_t idx);
    /**
     * Removes a cell at a given index
     *
     * @param idx index to find a cell
     */
    void removeCellAtIndex(ssize_t idx);
    /**
     * reloads data from data source.  the view will be refreshed.
     */
    void reloadData();
    /**
     * Dequeues a free cell if available. nil if not.
     *
     * @return free cell
     */
    TableViewCell *dequeueCell();

    /**
     * Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
     *
     * @param idx index
     * @return a cell at a given index
     */
    TableViewCell *cellAtIndex(ssize_t idx);

    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view)  override {}
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;

protected:
//    long __indexFromOffset(Vec2 offset);
    long _indexFromOffset(Vec2 offset);
//    Vec2 __offsetFromIndex(ssize_t index);
    Vec2 _offsetFromIndex(ssize_t index);

    void _moveCellOutOfSight(TableViewCell *cell);
    void _setIndexForCell(ssize_t index, TableViewCell *cell);
    void _addCellIfNecessary(TableViewCell * cell);

//    void _updateCellPositions();


    TableViewCell *_touchedCell;
    /**
     * vertical direction of cell filling
     */
    TableView::VerticalFillOrder _vordering;

    /**
     * index set to query the indexes of the cells used.
     */
    std::set<ssize_t>* _indices;

//    /**
//     * vector with all cell positions
//     */
//    std::vector<float> _vCellsPositions;
    //NSMutableIndexSet *indices_;
    /**
     * cells that are currently in the table
     */
    Vector<TableViewCell*> _cellsUsed;
    /**
     * free list of cells
     */
    Vector<TableViewCell*> _cellsFreed;
    /**
     * weak link to the data source object
     */
    RealTableViewDataSource* _dataSource;
    /**
     * weak link to the delegate object
     */
    RealTableViewDelegate* _tableViewDelegate;

    Direction _oldDirection;

    bool _isUsedCellsDirty;

    int m_iCols;

public:
    void _updateContentSize();

};

#endif /* REALTABLEVIEW_H_ */
