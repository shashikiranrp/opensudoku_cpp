#include <getopt.h>
#include <iostream>

#include "OpenSudoku.h"

int main(int argc, char* argv[])
{
    int option_char;
    while ((option_char = getopt(argc, argv, "hvx:")) != EOF) {
        switch (option_char) {
            case 'v':
                OpenSudoku::INTERACT_VERBOSE = true;
                break;
            case 'x':
                OpenSudoku::MAX_RESULTS = atoi(optarg);
                break;
                
            case 'h':
            default:
                std::cerr << "usage: " << argv[0] << " [-h] [-v] [-x <max_results>]" << std::endl;
                std::cerr << "\t-v " << "verbose (default: false)." << std::endl;
                std::cerr << "\t-x " << "maxresults (default: 1)." << std::endl;
                std::cerr << "\t-h " << "show this help and exit." << std::endl;
                exit(1);
        }
    }
    return OpenSudoku::interact<OpenSudoku::BoardSize::NINE>();
}