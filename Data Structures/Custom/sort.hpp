#ifndef SORT
#define SORT

namespace detail{
    template<class inputIter>
    void insertion_sort(inputIter& start, inputIter& last){
        int i = 1;
        while(i < last - start){
            int j = i;
            while(j > 0 && *(start + (j - 1)) > *(start + j)){
                std::swap(*(start + (j - 1)), *(start + j));
                --j;
            }
            ++i;
        }
    }
    template<class inputIter>
    void merge(inputIter& begin, inputIter& mid, inputIter& end){
        size_t sizeLeft = mid - begin;
        size_t sizeRight = (end - 1) - mid;

        using T = std::iterator_traits<inputIter>::value_type;
        T* leftArray = new T[sizeLeft + 1]; //sub arrays of odd sizes can result in the extra element in either the left or the right sub array
        T* rightArray = new T[sizeRight + 1]; //So we must account for that 1 additional slot

        for(int i = 0; i < mid - begin; ++i){ //Array [begin, mid)
            leftArray[i] = *(begin + i);
        }
        for(int i = 0; i < end - mid; ++i){ //Array [mid, end)
            rightArray[i] = *(mid + i);
        }

        size_t indexLeft = 0, indexRight = 0, indexMerge = 0;
        while(indexLeft < sizeLeft && indexRight < sizeRight){
            if(leftArray[indexLeft] <= rightArray[indexRight]){
                *(begin + indexMerge) = leftArray[indexLeft];
                ++indexLeft;
            }
            else{
                *(begin + indexMerge) = rightArray[indexRight];
                ++indexRight;
            }
            ++indexMerge;
        }

        while(indexLeft < sizeLeft){
            *(begin + indexMerge) = leftArray[indexLeft];
            ++indexLeft;
            ++indexMerge;
        } 

        while(indexRight < sizeRight){
            *(begin + indexMerge) = rightArray[indexRight];
            ++indexRight;
            ++indexMerge;
        }

        delete[] leftArray;
        delete[] rightArray;
    }
}
namespace custom{
    template<class inputIter>
    void _sort(inputIter& begin, inputIter& end){
        size_t length = end - begin;
        if(length <= 20){
            detail::insertion_sort(begin, end);
        }
        else{
            inputIter mid = begin + ((end - begin) / 2);
            inputIter mid2 = mid + 1;
            _sort(begin, mid2);
            _sort(mid, end);
            detail::merge(begin, mid, end);
        }
    }
    template<class inputIter>
    void sort(inputIter& begin, inputIter& end){
        if(begin >= end) return;

        _sort(begin, end);
    }
}
#endif //SORT