#ifndef SORT
#define SORT
namespace detail{
    template<class inputIter>
    void merge(inputIter& start, inputIter& mid, inputIter& last){      
        const size_t subLeft = (mid - start) + 1;
        const size_t subRight = (last - mid);

        using T = typename std::iterator_traits<inputIter>::value_type;
        T* left = new T[subLeft];
        T* right = new T[subRight];

        for(int i = 0; i < subLeft; ++i){
            left[i] = *(start + i);
        }
        for(int i = 0; i < subRight; ++i){
            right[i] = *(mid + 1 + i);
        }

        size_t indexLeft = 0, indexRight = 0, indexMerge = 0;
        while(indexLeft < subLeft && indexRight < subRight){
            if(left[indexLeft] <= right[indexRight]){
                *(start + indexMerge) = left[indexLeft];
                ++indexLeft;
            }
            else{
                *(start + indexMerge) = right[indexRight];
                ++indexRight;
            }
            ++indexMerge;
        }

        while(indexLeft < subLeft){
            *(start + indexMerge) = left[indexLeft];
            ++indexLeft;
            ++indexMerge;
        } 

        while(indexRight < subRight){
            *(start + indexMerge) = right[indexRight];
            ++indexRight;
            ++indexMerge;
        }

        delete[] left;
        delete[] right;
    }
}

namespace custom{
    template<class inputIter>
    void _sort(inputIter& start, inputIter& last){
        if(start >= last)
            return;
        
        inputIter mid = start + ((last - 1) - start) / 2;
        inputIter mid2 = mid + 1;

        _sort(start, mid);
        _sort(mid2, last);
        detail::merge(start, mid, last);
    }

    template<class inputIter>
    void sort(inputIter& start, inputIter& last){
        if(start >= last) return;

        inputIter end = last - 1;
        _sort(start, end);
    }
}
#endif //SORT