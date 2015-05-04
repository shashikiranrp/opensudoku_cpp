#include <iostream>
#include "VirtualSudokuBoard.h"
#include "Exceptions.h"
#include "TypeConsumer.h"
#include "LimitedNaturalNumberSet.h"
#include "VoidBox.h"
#include "FonAnalysis.h"
#include "LCFBacktrack.h"

namespace os = OpenSudoku;

int main(int argc, const char * argv[]) {
    try {
        os::VirtualSudokuBoard *vsb = os::VirtualSudokuBoard::getBoardOfSize(os::BoardSize::NINE);
        vsb->update(9, 1, 1);
        vsb->update(9, 2, 2);
        vsb->update(9, 3, 3);
        vsb->update(9, 4, 4);
        vsb->update(9, 5, 5);
        vsb->update(9, 6, 6);
        vsb->update(9, 7, 7);
        vsb->update(9, 8, 8);
        vsb->update(4, 7, 1);
        vsb->update(4, 8, 2);
        vsb->update(4, 9, 3);
        vsb->update(5, 7, 4);
        vsb->update(5, 9, 5);
        vsb->update(6, 7, 6);
        vsb->update(6, 8, 9);
        vsb->update(6, 9, 8);
        os::Fon<os::BoardSize::NINE> fon;
        os::FonResult<os::BoardSize::NINE> frs = fon.doFon(*vsb);
        frs.printResults();
        std::cout << (frs.sudokuBoard.varientPreserved() ? "GOOD BOARD" : "BAD BOARD") << std::endl;
        os::LCFBacktrack<os::BoardSize::NINE> lcfBt;
        os::ConsoleConsumer cConsumer = os::ConsoleConsumer("Sudoku Board", os::BoardSize::NINE ,true);
        lcfBt.doLCFBacktrack(frs, cConsumer, 10000);
        std::cout << "Total solutions: " << cConsumer.solutionsCount() <<  std::endl;
        delete vsb;
        return 0;
    } catch (OpenSudoku::SudokuException& exp) {
        std::cerr << "EXEPTION: " << exp.what() << std::endl;
    } catch(std::exception& ex) {
        std::cerr << "uncaught exception " << ex.what() << std::endl;
    }
}
