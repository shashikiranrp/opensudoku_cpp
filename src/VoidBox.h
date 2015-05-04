#ifndef __Sudoku__VoidBox__
#define __Sudoku__VoidBox__

#include <set>
#include <iostream>
#include "Common.h"
#include "VirtualSudokuBoard.h"
#include "LimitedNaturalNumberSet.h"

namespace OpenSudoku {
    
    
    template<BoardSize Size>
    class VoidBox {
        
    private:
        int rowIndex, columnIndex, squareIndex, boxIndex;
        BoardSize boardSize = Size;
        LimitedNatSet<Size> possibilities;
        bool fonApplied;
        
    public:
        explicit VoidBox(const int row, const int column) :
        rowIndex(row), columnIndex(column), fonApplied(false)
        {
            squareIndex = VirtualSudokuBoard::squareIndex(boardSize, rowIndex, columnIndex);
            boxIndex = VirtualSudokuBoard::boxIndex(boardSize, rowIndex, columnIndex);
        }
        
        int getMyBox() const
        {
            return boxIndex;
        }
        
        int getMySquare() const
        {
            return squareIndex;
        }
        
        int getMyRow() const
        {
            return rowIndex;
        }
        
        int getMyColumn() const
        {
            return columnIndex;
        }
        
        bool fonNotApplied() const
        {
            return !fonApplied;
        }
        
        void setFonApplied(const bool flag)
        {
            fonApplied = flag;
        }
        
        void setPossibilities(const std::set<int>& possibilities)
        {
            this->possibilities.set(possibilities);
        }
        
        std::set<int> getPossibilities() const
        {
            return this->possibilities.to_set();
        }
        
        size_t getPossibilityCount() const
        {
            return this->possibilities.size();
        }
        
        int getNthPossibility(int n) const
        {
            return this->possibilities.to_vector()[n - 1];
        }
        
        LimitedNatSet<Size> fonCP_AI(VirtualSudokuBoard& vsb)
        
        {
            LimitedNatSetConsumer<Size> setConsumer = LimitedNatSetConsumer<Size>();

            vsb.consumeRow(rowIndex, setConsumer);
            LimitedNatSet<Size> rowSet = SUDOKU_UNIVERSAL_SET - setConsumer.getContents();
            
            vsb.consumeColumn(columnIndex, setConsumer);
            LimitedNatSet<Size> columnSet = SUDOKU_UNIVERSAL_SET - setConsumer.getContents();
            
            vsb.consumeSquare(squareIndex, setConsumer);
            LimitedNatSet<Size> squareSet = SUDOKU_UNIVERSAL_SET - setConsumer.getContents();
            
            return (rowSet & columnSet & squareSet);
        }
    };
}

#endif /* defined(__Sudoku__VoidBox__) */
