#ifndef __Sudoku__Common__
#define __Sudoku__Common__

#include <functional>
#include <vector>
#include <iostream>
#include "LimitedNaturalNumberSet.h"

namespace OpenSudoku {
    
    enum BoardSize {
        NINE = 9
    };
    
    static std::function<bool(int, int, int)> VAL_IN_RANGE =
    [](int val, int start, int end) -> bool
    {
        return start <= val && end >= val;
    };
    
    static std::function<bool(bool)> NOT =
    [](bool val) -> bool { return !val; };
    
    static LimitedNatSet<NINE> SUDOKU_UNIVERSAL_SET = LimitedNatSet<9>(true);
    template<class T>
    void printVector(const std::vector<T>& vec)
    {
        int state = 0;
        for(auto& ele : vec) {
            if (0 == (state++ % 9)) {
                std::cout << std::endl;
            }
            std::cout << ele << " ";
        }
    }
}
#endif /* defined(__Sudoku__Common__) */
