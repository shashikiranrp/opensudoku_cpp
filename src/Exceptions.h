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
    
    class ConcreteNotNeeded : public SudokuException {
        
    public:
        ConcreteNotNeeded() :
        SudokuException(std::string("ConcreteNotNeeded: Current status of VirtualSudokuBoard doesn't need Concrete Analysis")) {}
    };
    
    class NomorePossibility : public SudokuException {
        
    public:
        NomorePossibility(int square) :
        SudokuException(std::string("NomorePossibility: Possibility list is empty for square ")
                        + std::to_string(square)
                        + std::string(".")) {}
    };
}

#endif /* defined(__Sudoku__Exceptions__) */
