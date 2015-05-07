#include <iostream>

#include "VirtualSudokuBoard.h"
#include "Exceptions.h"
#include "TypeConsumer.h"
#include "LimitedNaturalNumberSet.h"
#include "VoidBox.h"
#include "FonAnalysis.h"
#include "LCFBacktrack.h"


namespace OpenSudoku {
    
    bool INTERACT_VERBOSE = false;
    size_t MAX_RESULTS = 1;
    
    template<BoardSize Size>
    int interact(std::istream& in = std::cin, std::ostream& out = std::cout, std::ostream& err = std::cerr)
    {
        int status{0};
        StreamConsumer consumer(INTERACT_VERBOSE ? "Sudoku Board" : "", Size, false, out);
        Fon<Size> fon;
        LCFBacktrack<Size> lcfBackTrack;
        while (true) {
            std::unique_ptr<VirtualSudokuBoard> vsbPtr(VirtualSudokuBoard::getBoardOfSize(Size));
            try {
                int nextNumber{0};
                int nextRow{1}, nextColumn{1};
                while (in >> nextNumber) {
                    vsbPtr->update(nextRow, nextColumn, nextNumber);
                    if (++nextColumn > Size) {
                        nextColumn = 1;
                        if (++nextRow > Size) {
                            break;
                        }
                    }
                }
                
                if (nextColumn == 1 && nextRow == Size + 1) {
                    if (INTERACT_VERBOSE) {
                        out << "Problem:" << std::endl;
                        vsbPtr->consumeBoard(consumer);
                    }
                    if (INTERACT_VERBOSE) {
                        out << "Solution: " << std::endl;
                    }
                    FonResult<Size> result = fon.doFon(*vsbPtr);
                    if (result.concreteFlag) {
                        lcfBackTrack.doLCFBacktrack(result, consumer, MAX_RESULTS);
                    } else {
                        vsbPtr->consumeBoard(consumer);
                    }
                } else if (!in.eof()){
                    status = 1;
                    err << "Unexpected input while reading input to the box: " << nextRow << ", " << nextColumn << std::endl;
                    break;
                } else {
                    if (nextRow == 1 && nextColumn == 1) {
                        status = 0;
                        break;
                    }
                    status = 1;
                    err << "EOF reached!" << std::endl;
                    break;
                }
            } catch (SudokuException& exp) {
                err << "Error: " << exp.what() << std::endl << "start again!" << std::endl;
            }
        }
        return status;
    }
}