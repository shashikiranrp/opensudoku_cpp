#ifndef __Sudoku__VirtualSudokuBoard__
#define __Sudoku__VirtualSudokuBoard__

#include <functional>
#include "TypeConsumer.h"
#include "Common.h"

namespace OpenSudoku {
        
    class VirtualSudokuBoard {
        
    private:
        int **board = nullptr;
        BoardSize boardSize;
        VirtualSudokuBoard(const BoardSize&); // seize the board creation
        
    protected:
        // iterating internal messages
        template<class T>
        T& iterateOverRow(int row, T&, std::function<T&(T&, int, int, int)>) const;
        
        template<class T>
        T& iterateOverColumn(int column, T&, std::function<T&(T&, int, int, int)>) const;

        template<class T>
        T& iterateOverSquare(int square, T&, std::function<T&(T&, int, int, int)>) const;
        
        template<class T>
        T& iterateOverBoard(T&, std::function<T&(T&, int, int, int)>) const;
        
    public:
        VirtualSudokuBoard() = delete; // no default constructor
        VirtualSudokuBoard(VirtualSudokuBoard&) = delete; // can't copy
        VirtualSudokuBoard(VirtualSudokuBoard&&) = delete; // can't move
        VirtualSudokuBoard& operator=(VirtualSudokuBoard&) = delete; // can't assign
        virtual ~VirtualSudokuBoard();
        
        // Constant board messages
        int getNumberOfVoids() const;
        int getEntry(int row, int col) const;
        void consumeRow(int row, TypeConsumer<int>& consumer) const;
        void consumeColumn(int col, TypeConsumer<int>& consumer) const;
        void consumeSquare(int square, TypeConsumer<int>& consumer) const;
        void consumeBoard(TypeConsumer<int>& consumer) const;
        bool isInRow(int ele, int row) const;
        bool isInColumn(int ele, int column) const;
        bool isInSquare(int ele, int square) const;
        bool varientPreserved() const;
        bool isFull() const;
        BoardSize getBoardSize() const;
        
        // Mutable messages
        void update(int row, int col, int ele);
        void clear(int row, int col);
        void clearRow(int row);
        void clearColumn(int col);
        void clearAll();
        
        // Static messages
        static VirtualSudokuBoard* getBoardOfSize(const BoardSize& boardSize);
        static int squareIndex(BoardSize& size, int row, int col);
        static int boxIndex(BoardSize& size, int roq, int col);
    };
    
}

#endif /* defined(__Sudoku__VirtualSudokuBoard__) */
