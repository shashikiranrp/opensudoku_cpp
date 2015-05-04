#include <new>
#include <iostream>
#include <set>
#include "VirtualSudokuBoard.h"
#include "Exceptions.h"
#include "Common.h"

namespace OpenSudoku {
    static std::function<int&(int&,int,int,int)> ZERO_VALUE_COUNTER =
    [](int& c_count, int row, int col, int val) -> int&
    {
        c_count = 0 == val ? c_count + 1 : c_count;
        return c_count;
    };
    
    VirtualSudokuBoard::VirtualSudokuBoard(const BoardSize& size)
    {
        this->boardSize = size;
        this->board = new int*[this->boardSize];
        for(int iix = 0; iix < size; ++iix) {
            this->board[iix] = new int[this->boardSize]{0};
        }
    }
    
    VirtualSudokuBoard::~VirtualSudokuBoard()
    {
        for(int iix = 0; iix < this->boardSize; ++iix) {
            delete[] this->board[iix];
        }
        delete[] this->board;
    }
    
    template<class T>
    T& VirtualSudokuBoard::iterateOverBoard(T& intialVal, std::function<T&(T&, int, int, int)> reducer) const
    {
        T& result = intialVal;
        for(int row = 1; row <= this->boardSize; ++row) {
            for(int column = 1; column <= this->boardSize; ++column) {
                result = reducer(result, row, column, this->getEntry(row, column));
            }
        }
        return result;
    }
    
    template<class T>
    T& VirtualSudokuBoard::iterateOverRow(int row, T& intialVal, std::function<T&(T&, int, int, int)> reducer) const
    {
        if (NOT(VAL_IN_RANGE(row, 1, this->boardSize))) {
            throw InvalidDimension(row);
        }
        T& result = intialVal;
        for(int column = 1; column <= this->boardSize; ++column) {
            result = reducer(result, row, column, this->getEntry(row, column));
        }
        return result;
    }
    
    template<class T>
    T& VirtualSudokuBoard::iterateOverColumn(int column, T& intialVal, std::function<T&(T&, int, int, int)> reducer) const
    {
        if (NOT(VAL_IN_RANGE(column, 1, this->boardSize))) {
            throw InvalidDimension(column);
        }
        T& result = intialVal;
        for(int row = 1; row <= this->boardSize; ++row) {
            result = reducer(result, row, column, this->getEntry(row, column));
        }
        return result;
    }
    
    template<class T>
    T& VirtualSudokuBoard::iterateOverSquare(int square, T& intialVal, std::function<T&(T&, int, int, int)> reducer) const
    {
        if (this->boardSize != NINE) {
            throw SudokuException("iterateOverSquare makes sense for board with size 9");
        }
        
        if (NOT(VAL_IN_RANGE(square, 1, this->boardSize))) {
            throw InvalidDimension(square);
        }
        T& result = intialVal;
        int s_row = ((int) (square - 1) / 3) * 3, s_col = ((square - 1) % 3) * 3;
        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                int row = i + s_row, column =j + s_col;
                result = reducer(result, row, column, this->getEntry(row, column));
            }
        }
        return result;
    }
    
    // Constant board messages
    int VirtualSudokuBoard::getNumberOfVoids() const
    {
        int count = 0;
        return iterateOverBoard(count, ZERO_VALUE_COUNTER);
    }
    
    int VirtualSudokuBoard::getEntry(int row, int col) const
    {
        if (NOT(VAL_IN_RANGE(row, 1, this->boardSize))) {
            throw InvalidDimension(row);
        }
        if (NOT(VAL_IN_RANGE(col, 1, this->boardSize))) {
            throw InvalidDimension(col);
        }
        return this->board[row - 1][col - 1];
    }
    
    void VirtualSudokuBoard::consumeRow(int row, TypeConsumer<int>& consumer) const
    {
        int count = 0;
        std::function<int&(int&, int,int,int)> consumerWrapper =
        [&consumer] (int& acc, int r, int c, int v) -> int&
        {
            if(!consumer.consume(v)) {
                throw SudokuException("consumption failed!");
            }
            return acc;
        };
        consumer.start();
        iterateOverRow(row, count, consumerWrapper);
        consumer.done();
    }
    
    void VirtualSudokuBoard::consumeColumn(int col, TypeConsumer<int>& consumer) const
    {
        int count = 0;
        std::function<int&(int&, int,int,int)> consumerWrapper =
        [&consumer] (int& acc, int r, int c, int v) -> int&
        {
            if(!consumer.consume(v)) {
                throw SudokuException("consumption failed!");
            }
            return acc;
        };
        consumer.start();
        iterateOverColumn(col, count, consumerWrapper);
        consumer.done();
    }
    
    void VirtualSudokuBoard::consumeSquare(int square, TypeConsumer<int>& consumer) const
    {
        int count = 0;
        std::function<int&(int&, int,int,int)> consumerWrapper =
        [&consumer] (int& acc, int r, int c, int v) -> int&
        {
            if(!consumer.consume(v)) {
                throw SudokuException("consumption failed!");
            }
            return acc;
        };
        consumer.start();
        iterateOverSquare(square, count, consumerWrapper);
        consumer.done();
    }
    
    void VirtualSudokuBoard::consumeBoard(TypeConsumer<int>& consumer) const
    {
        int count = 0;
        std::function<int&(int&, int,int,int)> consumerWrapper =
        [&consumer] (int& acc, int r, int c, int v) -> int&
        {
            if(!consumer.consume(v)) {
                throw SudokuException("consumption failed!");
            }
            return acc;
        };
        consumer.start();
        iterateOverBoard(count, consumerWrapper);
        consumer.done();
    }
    
    bool VirtualSudokuBoard::isInRow(int ele, int row) const
    {
        bool result = false;
        std::function<bool&(bool&,int,int,int)> elementFinder =
        [&ele](bool& found, int r, int c, int v) -> bool&
        {
            return found = found || ele == v;
        };
        return iterateOverRow(row, result, elementFinder);
    }
    
    bool VirtualSudokuBoard::isInColumn(int ele, int column) const
    {
        bool result = false;
        std::function<bool&(bool&,int,int,int)> elementFinder =
        [&ele](bool& found, int r, int c, int v) -> bool&
        {
            return found = found || ele == v;
        };
        return iterateOverColumn(column, result, elementFinder);
    }
    
    bool VirtualSudokuBoard::isInSquare(int ele, int square) const
    {
        bool result = false;
        std::function<bool&(bool&,int,int,int)> elementFinder =
        [&ele](bool& found, int r, int c, int v) -> bool&
        {
            return found = found || ele == v;
        };
        return iterateOverSquare(square, result, elementFinder);
    }
    
    bool VirtualSudokuBoard::varientPreserved() const
    {
        std::set<int> intSet;
        std::function<bool&(bool&,int,int,int)> varientObserver =
        [&intSet](bool& violated, int r, int c, int v) -> bool&
        {
            return violated =  violated || (0 != v && !intSet.insert(v).second);
        };
        bool violated = false;
        // Row varients
        for (int iix = 1; iix <= this->boardSize; ++iix) {
            intSet.clear();
            violated = false;
            violated = iterateOverRow(iix, violated, varientObserver);
            if (violated) {
                return false;
            }
        }
        
        // Column varients
        for (int iix = 1; iix <= this->boardSize; ++iix) {
            intSet.clear();
            violated = false;
            violated = iterateOverColumn(iix, violated, varientObserver);
            if (violated) {
                return false;
            }
        }
        
        // Square varients
        for (int iix = 1; iix <= this->boardSize; ++iix) {
            intSet.clear();
            violated = false;
            violated = iterateOverSquare(iix, violated, varientObserver);
            if (violated) {
                return false;
            }
        }
        
        return true;
    }
    
    bool VirtualSudokuBoard::isFull() const
    {
        for(int i = 1; i <= this->boardSize; ++i) {
            for (int j = 1; j <= this->boardSize; ++j) {
                if (0 == this->getEntry(i, j)) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    // Mutable methods
    void VirtualSudokuBoard::update(int row, int col, int ele)
    {
        if (NOT(VAL_IN_RANGE(row, 1, this->boardSize))) {
            throw InvalidDimension(row);
        }
        if (NOT(VAL_IN_RANGE(col, 1, this->boardSize))) {
            throw InvalidDimension(col);
        }
        if (NOT(VAL_IN_RANGE(ele, 0, this->boardSize))) {
            throw InvalidBoxEntry(row, col, ele);
        }
        
        this->board[row - 1][col - 1] = ele;
    }
    
    void VirtualSudokuBoard::clear(int row, int col)
    {
        this->update(row, col, 0);
    }
    
    void VirtualSudokuBoard::clearRow(int row)
    {
        int count = 0;
        std::function<int&(int&, int, int, int)> rowConsumer =
        [this](int& result, int r, int c, int v) -> int&
        {
            this->clear(r, c);
            return result;
        };
        iterateOverRow(row, count, rowConsumer);
    }
    
    void VirtualSudokuBoard::clearColumn(int col)
    {
        int count = 0;
        std::function<int&(int&, int, int, int)> colConsumer =
        [this](int& result, int r, int c, int v) -> int&
        {
            this->clear(r, c);
            return result;
        };
        iterateOverColumn(col, count, colConsumer);
    }
    
    void VirtualSudokuBoard::clearAll()
    {
        int count = 0;
        std::function<int&(int&, int, int, int)> boardConsumer =
        [this](int& result, int r, int c, int v) -> int&
        {
            this->clear(r, c);
            return result;
        };
        iterateOverBoard(count, boardConsumer);
    }
    
    // Static methods
    int VirtualSudokuBoard::squareIndex(BoardSize& size, int row, int col)
    {
        if (size != NINE) {
            throw SudokuException("square index makes sense for board with size 9");
        }
        if (NOT(VAL_IN_RANGE(row, 1, size))) {
            throw InvalidDimension(row);
        }
        if (NOT(VAL_IN_RANGE(col, 1, size))) {
            throw InvalidDimension(col);
        }
        
        int square = 1;
        
        while (row > 3) {
            row -= 3;
            square += 3;
        }
        
        while (col > 3) {
            col -= 3;
            square++;
        }
        
        return square;
    }
    
    int VirtualSudokuBoard::boxIndex(BoardSize& size, int row, int col)
    {
        if (size != NINE) {
            throw SudokuException("box index makes sense for board with size 9");
        }
        
        if (NOT(VAL_IN_RANGE(row, 1, size))) {
            throw InvalidDimension(row);
        }
        if (NOT(VAL_IN_RANGE(col, 1, size))) {
            throw InvalidDimension(col);
        }
        
        return (((row - 1) * 9) + col);
    }
    
    VirtualSudokuBoard* VirtualSudokuBoard::getBoardOfSize(const BoardSize& boardSize)
    {
        try {
            VirtualSudokuBoard* vsb = new VirtualSudokuBoard(boardSize);
            return vsb;
        } catch (std::bad_alloc& ba) {
            std::cerr << "bad_alloc caught while intializing sudoku board: " << ba.what() << '\n';
            return nullptr;
        }
    }    
}