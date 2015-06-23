/*
 * SudokuUtil.h
 *
 *  Created on: 2015年6月23日
 *      Author: kerlw
 */

#ifndef __SUDOKU_UTIL__
#define __SUDOKU_UTIL__

#define MAX_ROWS 9
#define MAX_COLS 9
#define MAX_BLOCKS 9
#define MAX_CELLS 81
#define MAX_NUMBER 9

#include <stdlib.h>

class SudokuSolver {
    friend class SudokuGenerator;
    friend class CellDigger;
public:
    static SudokuSolver* getInstance();
    virtual ~SudokuSolver();

    void setSize(short rows_per_grid, short cols_per_grid, short grids_in_row, short grids_in_col);

    void setNumber(int r, int c, int value);
    void unsetNumber(int row, int col);

    void setMaxSolutionCount(int value) {
        m_iMaxSCount = value;
    }
    int getSolutionCount() {
        return m_iSolutionCount;
    }

    int try_solve(bool oneShot = true);

private:
    SudokuSolver();

    void initConstraints();
    int search(int k, bool oneShot = true);


    bool supose(int row, int col, unsigned short number);
    void resume(int row, int col, unsigned short number);

    unsigned short getValue(unsigned short constraint, int index) {
        for (int i = 0; i < m_uNumbers; i++) {
            unsigned short mask = 1 << i;
            if ((constraint & mask) != 0 && index-- == 0)
                return (i + 1);
        }
        return 0;
    }

    void printBox(){};

private:
    int m_iMaxSCount;

    unsigned short m_uMask;

    unsigned short m_uNumbers;
    unsigned short m_uRows;
    unsigned short m_uCols;

    unsigned short m_uGridRows;
    unsigned short m_uGridCols;

    unsigned short m_uGridsInRow;
    unsigned short m_uGridsInCol;

    int m_iSolutionCount;
    unsigned char m_pSolution[MAX_CELLS];

    unsigned short m_pRowConstraints[MAX_ROWS];    // constrains for rows, bit 0 indicates whether this row has number 1 or not
    unsigned short m_pColConstraints[MAX_COLS];    // constrains for cols
    unsigned short m_pBlockConstraints[MAX_BLOCKS];  // constrains for blocks
    unsigned short m_pCellConstraints[MAX_CELLS];   // constrains for each cell value

};

class CellDigger {
public:
	virtual void doFirstDig() = 0;

	CellDigger(SudokuSolver* solver) : m_pSolver(solver) {}
	virtual ~CellDigger() {};
	unsigned char* getPuzzle();

protected:
	SudokuSolver* m_pSolver;
	unsigned char* m_pSolution;
	unsigned char* m_pPuzzle;
	unsigned char* m_pDiggable;
};

class SudokuGenerator {
public:
    static SudokuGenerator* getInstance();
    virtual ~SudokuGenerator() {}

    void setSize(short rows_per_grid, short cols_per_grid, short grids_in_row, short grids_in_col);
    void setPuzzleDifficulty(int difficulty);
    bool generateBox();
    bool digoutPuzzle();
    unsigned char* getSolution() { return m_solver.m_pSolution; }
    unsigned char* getPuzzle() {
    	if (m_pDigger)
    		return m_pDigger->getPuzzle();
    	return nullptr;
    }

private:
    SudokuGenerator() : m_pDigger(nullptr) {}

private:
    SudokuSolver m_solver;
    CellDigger* m_pDigger;
};


class CellDiggerOneByOne : public CellDigger {
	virtual void doFirstDig() override;
};

class CellDiggerByNumber : public CellDigger {
	virtual void doFirstDig() override;
};

#endif // __SUDOKU_UTIL__

