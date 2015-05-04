#ifndef __Sudoku__Exceptions__
#define __Sudoku__Exceptions__

#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>

namespace OpenSudoku {
    class SudokuException : public std::runtime_error {
    public:
        SudokuException(const std::string& reason) : std::runtime_error("SudokuException: " + reason){}
    };
    
    class InvalidBoxEntry : public SudokuException {
    public:
        InvalidBoxEntry(int row, int col, int value) :
        SudokuException(std::string("InvalidDimension: [r,c,v] => [")
                        + std::to_string(row) + std::string(",")
                        + std::to_string(col) + std::string(",")
                        + std::to_string(value) + std::string("]")) {}
    };
    
    class InvalidDimension : public SudokuException {
        
        
    public:
        InvalidDimension(int dim) :
        SudokuException(std::string("InvalidDimension: dimension => "
                                    + std::to_string(dim))) {}
    };
    
    class BacktrackNotNeeded : public SudokuException {
        
    public:
        BacktrackNotNeeded() :
        SudokuException(std::string("BacktrackNotNeeded: Current status of VirtualSudokuBoard doesn't need Backtrack Analysis")) {}
    };
    
    class NomorePossibility : public SudokuException {
        
    public:
        NomorePossibility(int row, int column) :
        SudokuException(std::string("NomorePossibility: Possibility list is empty for box at (row, column) => (")
                        + std::to_string(row)
                        + std::string(", ")
                        + std::to_string(column)
                        + std::string(").")) {}
    };
    
    class VarientNotPreserved : public SudokuException {
        
    public:
        VarientNotPreserved() :
        SudokuException("can't solve a board with varients not preserved!") {}
    };
}

#endif /* defined(__Sudoku__Exceptions__) */
