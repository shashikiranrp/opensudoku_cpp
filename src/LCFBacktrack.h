#ifndef __Sudoku__LCFBacktrack__
#define __Sudoku__LCFBacktrack__

#include <vector>
#include <algorithm>
#include <iostream>
#include "Common.h"
#include "FonAnalysis.h"
#include "TypeConsumer.h"
#include "Exceptions.h"

namespace OpenSudoku {
    
    template<BoardSize Size>
    class SudokuBackTrack {
        
    private:
        VirtualSudokuBoard& sudokuBoard;
        std::vector<VoidBox<Size>>& voidBoxes;
        
        /**
         * state variables.
         */
        size_t maxResults{}, solutionIndex{};
        int currentVoid{};
        bool stateIntialized{false};
        size_t *indexVector{nullptr}, *lenVector{nullptr};
        bool stopFlag{};
        TypeConsumer<int>& solutionConsumer;
    protected:
        void resetState(size_t maxResults)
        {
            if (stateIntialized) {
                delete []indexVector;
                delete []lenVector;
            }
            this->maxResults = maxResults;
            this->currentVoid = 0;
            this->solutionIndex = 0;
            this->stopFlag = false;
            this->indexVector = new size_t[voidBoxes.size()]{0};
            this->lenVector = new size_t[voidBoxes.size()]{0};
            size_t *ptr = this->lenVector;
            for (auto& ele : voidBoxes) {
                *ptr++ = ele.getPossibilityCount();
                ele.getReadyForConcreteAnalysis();
            }
            this->stateIntialized = true;
        }
        
        VoidBox<Size>& currentVoidBox()
        {
            return this->voidBoxes[this->currentVoid];
        }
        
        bool checkPlacementForCurrentVoid()
        {
            VoidBox<Size>& currentVB = currentVoidBox();
            int row{currentVB.getMyRow()};
            int column{currentVB.getMyColumn()};
            int square{currentVB.getMySquare()};
            int value{currentVB.getNthPossibilityForLCFBacktrack(this->indexVector[this->currentVoid])};
            return (!sudokuBoard.isInRow(value, row))
            && (!sudokuBoard.isInColumn(value, column))
            && (!sudokuBoard.isInSquare(value, square));
        }
        
        void setCurrentVoid()
        {
            VoidBox<Size>& currentVB = currentVoidBox();
            sudokuBoard.update(currentVB.getMyRow(),
                               currentVB.getMyColumn(),
                               currentVB.getNthPossibilityForLCFBacktrack(this->indexVector[this->currentVoid]));
        }
        
        void unsetCurrentVoid()
        {
            VoidBox<Size>& currentVB = currentVoidBox();
            sudokuBoard.update(currentVB.getMyRow(), currentVB.getMyColumn(), 0);
        }
        
        void continueToNextLevel()
        {
            if (currentVoid == (voidBoxes.size() - 1)) {
                if (0 == maxResults || solutionIndex++ < maxResults) {
                    sudokuBoard.consumeBoard(solutionConsumer);
                    backTrack();
                } else {
                    stopFlag = true;
                }
            } else {
                currentVoid++;
            }
        }
        
        void backTrack()
        {
            bool continueFlag;
            do {
                unsetCurrentVoid();
                continueFlag = ((indexVector[currentVoid] = ((indexVector[currentVoid] + 1) % lenVector[currentVoid]))) == 0;
            } while (continueFlag && --currentVoid >= 0);
            stopFlag = (currentVoid == -1);
        }
        
    public:
        SudokuBackTrack(VirtualSudokuBoard& vsb, std::vector<VoidBox<Size>>& boxes, TypeConsumer<int>& solutionConsumer)
        : sudokuBoard(vsb), voidBoxes(boxes), solutionConsumer(solutionConsumer) {}
        
        /**
         * Solve and consume results.
         */
        void consumeResults(size_t maxResults)
        {
            resetState(maxResults);
            
            while (!this->stopFlag) {
                if (checkPlacementForCurrentVoid()) {
                    setCurrentVoid();
                    continueToNextLevel();
                } else {
                    backTrack();
                }
            }
        }
        
        ~SudokuBackTrack()
        {
            if (nullptr != indexVector) {
                delete []indexVector;
            }
            
            if (nullptr != lenVector) {
                delete []lenVector;
            }
        }
    };
    
    template<BoardSize Size>
    class LCFBacktrack {
        
    public:
        void doLCFBacktrack(FonResult<Size>& fonResult, TypeConsumer<int>& solutionConsumer, int maxSolutions) const
        {
            /**
             * Check back track is needed.
             */
            if (!fonResult.concreteFlag) {
                throw BacktrackNotNeeded();
            }
            
            /**
             * Check back track is possible.
             */
            auto noPossibilityBoxPred = [](VoidBox<Size>& vb) -> bool
            {
                return 0 == vb.getPossibilityCount();
            };
            typename std::vector<VoidBox<Size>>::iterator it;
            if ((it = find_if(fonResult.voidsAfterFon.begin(), fonResult.voidsAfterFon.end(), noPossibilityBoxPred))
                != fonResult.voidsAfterFon.end()) {
                throw NomorePossibility((*it).getMyRow(), (*it).getMyColumn());
            }
            
            /**
             * Back track by keeping least cardinality at top of state space tree (LCF: Least cardinality first.)
             */
            std::vector<VoidBox<Size>> lcfVoidBoxVector(fonResult.voidsAfterFon.begin(), fonResult.voidsAfterFon.end());
            auto lcfComparator = [](VoidBox<Size>& x, VoidBox<Size>& y) -> bool
            {
                return x.getPossibilityCount() < y.getPossibilityCount();
            };
            std::sort(lcfVoidBoxVector.begin(), lcfVoidBoxVector.end(), lcfComparator);
            
            SudokuBackTrack<Size> sbt(fonResult.sudokuBoard, lcfVoidBoxVector, solutionConsumer);
            sbt.consumeResults(maxSolutions);
        }
        
    };
}
#endif /* defined(__Sudoku__LCFBacktrack__) */
