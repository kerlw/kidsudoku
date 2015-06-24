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
#include <cstdint>
#include <vector>

enum class Difficulty {
	Easy = 0,
	Normal,
	Hard,
	Evil
};

class SudokuSolver {
    friend class SudokuGenerator;
    friend class CellDigger;
public:
    static SudokuSolver* getInstance();
    virtual ~SudokuSolver();

    void setSize(short rows_per_grid, short cols_per_grid, short grids_in_row, short grids_in_col);

    void setNumber(int r, int c, int value);
    int unsetNumber(int row, int col, bool exclude = false);

    short numbers() { return m_uNumbers; }
    short gridRows() { return m_uGridRows;	}
    short gridCols() { return m_uGridCols;	}
    short gridsInRow() { return m_uGridsInRow;	}
    short gridsInCol() { return m_uGridsInCol;	}
    short rows() { return m_uRows;	}
    short cols() { return m_uCols;	}

    void setMaxSolutionCount(int value) {
        m_iMaxSCount = value;
    }
    int getSolutionCount() {
        return m_iSolutionCount;
    }

    int try_solve(bool oneShot = true);

private:
    SudokuSolver();
    SudokuSolver(const SudokuSolver* solver);

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

    void printBox();

protected:
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

	CellDigger(SudokuSolver* solver);
	virtual ~CellDigger();
	unsigned char* getPuzzle() { return m_pPuzzle;	}

	bool tryDig();

	static CellDigger* create(SudokuSolver* solver, Difficulty& difficulty);

protected:
	virtual void prepare();
	bool dig(int pos);
	bool tryDig(int pos);

protected:
	SudokuSolver* m_pSolver;
	std::uint8_t* m_pSolution;
	std::uint8_t m_pPuzzle[MAX_CELLS];
	std::uint8_t m_pDiggable[MAX_CELLS];
	std::vector<int> m_vctPos;

	short m_sMaxKeeps;	//the maximum number of cells to keep
	short m_sMinKeeps;	//the minimum number of cells to keep
	short m_sKeeps;
};

class SudokuGenerator {
public:
    static SudokuGenerator* getInstance();
    virtual ~SudokuGenerator() {}

    void setSize(short rows_per_grid, short cols_per_grid, short grids_in_row, short grids_in_col);
    void setPuzzleDifficulty(Difficulty difficulty);

    bool generate();
    std::uint8_t* getSolution() { return m_solver.m_pSolution; }
    std::uint8_t* getPuzzle() {
    	if (m_pDigger)
    		return m_pDigger->getPuzzle();
    	return nullptr;
    }

private:
    SudokuGenerator() : m_pDigger(nullptr) {}
    bool generateBox();

private:
    Difficulty m_eDifficulty;
    SudokuSolver m_solver;
    CellDigger* m_pDigger;
};

class CellDiggerRandom : public CellDigger {
public:
	CellDiggerRandom(SudokuSolver* solver) : CellDigger(solver) {}
	virtual void doFirstDig() override;

protected:
	virtual void prepare() override;
};

class CellDiggerOneByOne : public CellDigger {
public:
	CellDiggerOneByOne(SudokuSolver* solver) : CellDigger(solver) {}
	virtual void doFirstDig() override;

protected:
	virtual void prepare() override;
};

class CellDiggerByNumber : public CellDigger {
public:
	CellDiggerByNumber(SudokuSolver* solver) : CellDigger(solver) {}
	virtual void doFirstDig() override;

protected:
	virtual void prepare() override;

private:
	short m_pNumberPos[MAX_CELLS];
};

#endif // __SUDOKU_UTIL__

