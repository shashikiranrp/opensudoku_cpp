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
    
    template<class T>
    class VectorTypeConsumer : public TypeConsumer<T> {
    private:
        std::vector<T> sink;
    public:
        VectorTypeConsumer()
        {
            this->sink = std::vector<T>();
        }
        
        void start()
        {
            this->sink.clear();
        }
        
        bool consume(T& val)
        {
            this->sink.push_back(val);
            return true;
        }
        
        void done() {}
        
        std::vector<T> getContents() const
        {
            return this->sink;
        }
        
        std::vector<T>& getContentsReference() const
        {
            return this->sink;
        }
    };
    
    class ConsoleConsumer : public TypeConsumer<int> {
        
    private:
        std::string title;
        int state;
        BoardSize boardSize;
        int solutionCount{0};
        bool dev_null;
        
    public:
        
        explicit ConsoleConsumer(const char* title, BoardSize boardSize, bool dev_null = false)
        : title(title),boardSize(boardSize), dev_null(dev_null) {}
        
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
            std::cout << title;
        }
        
        bool consume(int& val)
        {
            if (dev_null) {
                return true;
            }
            if (0 == (state++ % boardSize)) {
                std::cout << std::endl;
            }
            std::cout << val << " ";
            return true;
        }
        
        void done()
        {
            ++solutionCount;
            if (dev_null) {
                return;
            }
            std::cout << std::endl;
        }
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
