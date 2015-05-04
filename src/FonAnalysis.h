#ifndef __Sudoku__FonAnalysis__
#define __Sudoku__FonAnalysis__

#include <vector>
#include <iostream>
#include <algorithm>
#include "VirtualSudokuBoard.h"
#include "VoidBox.h"
#include "TypeConsumer.h"

namespace OpenSudoku {
    
    template<BoardSize Size>
    struct FonResult {
        VirtualSudokuBoard& sudokuBoard;
        int noOfVoidsBeforeFon{0}, noOfVoidsAfterFon{0}, fonSteps{0};
        bool concreteFlag{false};
        std::vector<VoidBox<Size>> voidsAfterFon;
        FonResult(VirtualSudokuBoard& vsb) : sudokuBoard(vsb) {}
        void printResults() const
        {
            std::cout << "Board status after FON" << std::endl;
            std::cout << "Number of Voids before FON: " << noOfVoidsBeforeFon << std::endl;
            std::cout << "Number of Voids after FON: " << noOfVoidsAfterFon << std::endl;
            std::cout << "Status of concrete flag: " << std::boolalpha << concreteFlag << std:: endl;
            std::cout << "Fon steps: " << fonSteps << std::endl;
            ConsoleConsumer cConsumer = ConsoleConsumer("Sudoku Board", BoardSize::NINE);
            sudokuBoard.consumeBoard(cConsumer);
        }
    };
    
    /*
     * FON = First One Normalize
     */
    template<BoardSize Size>
    class Fon {
        
    protected:
        void fonAnalysis(VirtualSudokuBoard& vsb, std::vector<VoidBox<Size>>& boardVoids) const
        {
            for (int row = 1; row <= Size; ++row) {
                for (int column = 1; column <= Size; ++column) {
                    if (0 == vsb.getEntry(row, column)) {
                        VoidBox<Size> vb(row, column);
                        vb.setPossibilities(vb.fonCP_AI(vsb).to_set());
                        boardVoids.push_back(vb);
                    }
                }
            }
            
        }
        
        int fonCurrentNormalize(VirtualSudokuBoard& vsb, std::vector<VoidBox<Size>>& boardVoids) const
        {
            int count{0};
            auto nextFonPredicate = [](VoidBox<Size>& vb) -> bool
            {
                return 1 == vb.getPossibilityCount() && vb.fonNotApplied();;
            };
            typename std::vector<VoidBox<Size>>::iterator it;
            while ((it = find_if(boardVoids.begin(), boardVoids.end(), nextFonPredicate)) != boardVoids.end()) {
                updateFonVoid(vsb, *it);
                ++count;
            }
            return count;
        }
        
        void updateFonVoid(VirtualSudokuBoard& vsb, VoidBox<Size>& vb) const
        {
            vb.setFonApplied(true);
            vsb.update(vb.getMyRow(), vb.getMyColumn(), vb.getNthPossibility(1));
        }
        
    public:
        FonResult<Size> doFon(VirtualSudokuBoard& vsb) const
        {
            FonResult<Size> result(vsb);
            std::vector<VoidBox<Size>> *boardVoids = new std::vector<VoidBox<Size>>();
            int count{0};
            
            this->fonAnalysis(vsb, *boardVoids);
            
            result.noOfVoidsBeforeFon = vsb.getNumberOfVoids();
            
            while (this->fonCurrentNormalize(vsb, *boardVoids) != 0) {
                delete boardVoids;
                boardVoids = new std::vector<VoidBox<Size>>();
                this->fonAnalysis(vsb, *boardVoids);
                count++;
            }
            
            result.noOfVoidsAfterFon = vsb.getNumberOfVoids();
            if (result.noOfVoidsAfterFon > 0) {
                delete boardVoids;
                boardVoids = new std::vector<VoidBox<Size>>();
                this->fonAnalysis(vsb, *boardVoids);
                result.concreteFlag = true;
                result.voidsAfterFon = *boardVoids;
                delete boardVoids;
            }
            result.fonSteps = count;
            return result;
        }
        
    };
}

#endif /* defined(__Sudoku__FonAnalysis__) */
