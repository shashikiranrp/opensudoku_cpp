#ifndef __Sudoku__LimitedNaturalNumberSet__
#define __Sudoku__LimitedNaturalNumberSet__

#include <bitset>
#include <set>
#include <ostream>
#include <vector>
namespace OpenSudoku {
    
    template<size_t Size>
    class LimitedNatSet {
        
    private:
        size_t limit = Size;
        std::bitset<Size> natSet;
        
    public:
        LimitedNatSet(bool universal = false);
        LimitedNatSet& operator&=(const LimitedNatSet& rhs);
        LimitedNatSet& operator|=(const LimitedNatSet& rhs);
        LimitedNatSet& operator^=(const LimitedNatSet& rhs);
        LimitedNatSet& operator-=(const LimitedNatSet& rhs);
        
        std::set<int> to_set() const;
        std::vector<int> to_vector() const;
        size_t size() const;
        LimitedNatSet& add(int);
        LimitedNatSet& addAll(const std::set<int>&);
        LimitedNatSet& set(const std::set<int>&);
        bool remove(int);
        void clear();
        
        friend LimitedNatSet<Size> operator&(const LimitedNatSet<Size>& lhs, const LimitedNatSet<Size>& rhs)
        {
            LimitedNatSet<Size> result;
            result.natSet |= lhs.natSet & rhs.natSet;
            return result;
        }
        
        friend LimitedNatSet<Size> operator|(const LimitedNatSet<Size>& lhs, const LimitedNatSet<Size>& rhs)
        {
            LimitedNatSet<Size> result;
            result.natSet |= lhs.natSet | rhs.natSet;
            return result;
        }
        
        friend LimitedNatSet<Size> operator^(const LimitedNatSet<Size>& lhs, const LimitedNatSet<Size>& rhs)
        {
            LimitedNatSet<Size> result;
            result.natSet |= lhs.natSet ^ rhs.natSet;
            return result;
        }
        
        friend LimitedNatSet<Size> operator-(const LimitedNatSet<Size>& lhs, const LimitedNatSet<Size>& rhs)
        {
            LimitedNatSet<Size> result(lhs);
            result.natSet &= lhs.natSet ^ rhs.natSet;
            return result;
        }
        
        friend std::ostream& operator<<(std::ostream& os, const LimitedNatSet<Size>& lns)
        {
            for(auto& ele : lns.to_set()) {
                os << ele;
            }
            return os;
        }
    };
    
    template<size_t Size>
    LimitedNatSet<Size>::LimitedNatSet(bool univeral)
    {
        if (univeral) {
            this->natSet.set();
        } else {
            this->natSet.reset();
        }
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::operator&=(const LimitedNatSet& rhs)
    {
        this->natSet &= rhs.natSet;
        return *this;
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::operator|=(const LimitedNatSet& rhs)
    {
        this->natSet |= rhs.natSet;
        return *this;
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::operator^=(const LimitedNatSet& rhs)
    {
        this->natSet ^= rhs.natSet;
        return *this;
    }

    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::operator-=(const LimitedNatSet& rhs)
    {
        std::bitset<Size> temp = this->natSet ^ rhs.natSet;
        this->natSet &= temp;
        return *this;
    }
    
    template<size_t Size>
    std::set<int> LimitedNatSet<Size>::to_set() const
    {
        std::set<int> result;
        for (int nat = 1; nat <= this->limit; ++nat) {
            if (this->natSet.test(nat - 1)) {
                result.insert(nat);
            }
        }
        return result;
    }
    
    template<size_t Size>
    std::vector<int> LimitedNatSet<Size>::to_vector() const
    {
        std::vector<int> result;
        for (int nat = 1; nat <= this->limit; ++nat) {
            if (this->natSet.test(nat - 1)) {
                result.push_back(nat);
            }
        }
        return result;
    }
    
    
    template<size_t Size>
    size_t LimitedNatSet<Size>::size() const
    {
        return this->natSet.count();
    }
    
    template<size_t Size>
    void LimitedNatSet<Size>::clear()
    {
        this->natSet.reset();
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::add(int ele)
    {
        if (!this->natSet.test(ele - 1)) {
            this->natSet.set(ele - 1);
        }
        
        return *this;
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::addAll(const std::set<int>& inSet)
    {
        for(auto& ele : inSet) {
            add(ele);
        }
        return *this;
    }
    
    template<size_t Size>
    LimitedNatSet<Size>& LimitedNatSet<Size>::set(const std::set<int>& inSet)
    {
        this->clear();
        return this->addAll(inSet);
    }
    
    template<size_t Size>
    bool LimitedNatSet<Size>::remove(int ele)
    {
        if (this->natSet.test(ele - 1)) {
            this->natSet.reset(ele - 1);
            return true;
        }
        
        return false;
    }
}
#endif /* defined(__Sudoku__LimitedNaturalNumberSet__) */
