#ifndef __Sudoku__VirtualSudokuBoard__
#define __Sudoku__VirtualSudokuBoard__

#include <functional>
#include "TypeConsumer.h"
#include "Common.h"
#include "Exceptions.h"

namespace OpenSudoku {
        
    class VirtualSudokuBoard {
        
    private:
        int **board = nullptr;
        BoardSize boardSize;
        VirtualSudokuBoard(const BoardSize&); // seize the board creation
        
    protected:
        // iterating internal messages
        template<class T>
        T& iterateOverRow(int row, T&, std::function<T&(T&, int, int, int)>&) const throw(InvalidDimension);
        
        template<class T>
        T& iterateOverColumn(int column, T&, std::function<T&(T&, int, int, int)>&) const throw(InvalidDimension);

        template<class T>
        T& iterateOverSquare(int square, T&, std::function<T&(T&, int, int, int)>&) const throw(InvalidDimension);
        
        template<class T>
        T& iterateOverBoard(T&, std::function<T&(T&, int, int, int)>&) const;
        
    public:
        VirtualSudokuBoard() = delete; // no default constructor
        VirtualSudokuBoard(VirtualSudokuBoard&) = delete; // can't copy
        VirtualSudokuBoard(VirtualSudokuBoard&&) = delete; // can't move
        VirtualSudokuBoard& operator=(VirtualSudokuBoard&) = delete; // can't assign
        virtual ~VirtualSudokuBoard();
        
        // Constant board messages
        int getNumberOfVoids() const;
        int getEntry(int row, int col) const throw(InvalidDimension);
        void consumeRow(int row, TypeConsumer<int>& consumer) const throw(InvalidDimension);
        void consumeColumn(int col, TypeConsumer<int>& consumer) const throw(InvalidDimension);
        void consumeSquare(int square, TypeConsumer<int>& consumer) const throw(InvalidDimension);
        void consumeBoard(TypeConsumer<int>& consumer) const;
        bool isInRow(int ele, int row) const throw(InvalidDimension);
        bool isInColumn(int ele, int column) const throw(InvalidDimension);
        bool isInSquare(int ele, int square) const throw(InvalidDimension);
        bool varientPreserved() const;
        bool isFull() const;
        BoardSize getBoardSize() const;
        
        // Mutable messages
        void update(int row, int col, int ele) throw(InvalidDimension, InvalidBoxEntry);
        void clear(int row, int col) throw(InvalidDimension);
        void clearRow(int row) throw(InvalidDimension);
        void clearColumn(int col) throw(InvalidDimension);
        void clearAll();
        
        // Static messages
        static VirtualSudokuBoard* getBoardOfSize(const BoardSize& boardSize);
        static int squareIndex(BoardSize& size, int row, int col) throw(SudokuException, InvalidDimension);
        static int boxIndex(BoardSize& size, int row, int col) throw(SudokuException, InvalidDimension);
    };
    
}

#endif /* defined(__Sudoku__VirtualSudokuBoard__) */
