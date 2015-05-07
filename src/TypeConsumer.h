#ifndef __Sudoku__TypeConsumer__
#define __Sudoku__TypeConsumer__

#include <vector>
#include <iostream>
#include <string>

#include "Common.h"

namespace OpenSudoku {
    
    template<class T>
    class TypeConsumer {
    public:
        virtual ~TypeConsumer() {}
        virtual void start() = 0;
        virtual bool consume(T&) = 0;
        virtual void done() = 0;
    };
    
    class StreamConsumer : public TypeConsumer<int> {
    private:
        std::string title;
        int state;
        BoardSize boardSize;
        int solutionCount{0};
        bool dev_null;
        std::ostream& out;
        
    public:
        
        StreamConsumer(const char* title, BoardSize boardSize, bool dev_null = false, std::ostream& out = std::cout)
        : title(title),boardSize(boardSize), dev_null(dev_null), out(out) {}
        
        int solutionsCount() const
        {
            return solutionCount;
        }
        
        void start()
        {
            state = 0;
            if (dev_null) {
                return;
            }
            out << title;
        }
        
        bool consume(int& val)
        {
            if (dev_null) {
                return true;
            }
            if (0 == (state++ % boardSize)) {
                out << std::endl;
            }
            out << val << " ";
            return true;
        }
        
        void done()
        {
            ++solutionCount;
            if (dev_null) {
                return;
            }
            out << std::endl;
        }
        
    };
    
    class ConsoleConsumer : public StreamConsumer {
        
        
    public:
        ConsoleConsumer(const char* title, BoardSize boardSize, bool dev_null = false)
        : StreamConsumer(title, boardSize, dev_null) {}
    };
    
    template<BoardSize Size>
    class LimitedNatSetConsumer : public TypeConsumer<int> {
        
    private:
        LimitedNatSet<Size> sink;
        
    public:
        void start()
        {
            this->sink.clear();
        }
        
        bool consume(int& val)
        {
            if (0 == val) {
                return true;
            }
            this->sink.add(val);
            return true;
        }
        
        void done()
        {
        }
        
        LimitedNatSet<Size> getContents() const
        {
            return this->sink;
        }
        
        LimitedNatSet<Size>& getContentsReference() const
        {
            return this->sink;
        }
        
    };
}

#endif /* defined(__Sudoku__TypeConsumer__) */
