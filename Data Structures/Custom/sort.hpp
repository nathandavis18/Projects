#ifndef SORT
#define SORT
#include <cmath>
namespace detail{
    template<class inputIter>
    inputIter iLeftChild(inputIter i){
        return i + (2 * (i - inputIter(0)) + 1);
    }

    template<class inputIter>
    inputIter iParent(inputIter i){
        return i + (i - (i + 1)) / 2;
    }

    template<class inputIter>
    void siftDown(inputIter &begin, inputIter& end){
        while(iLeftChild(begin) < end){
            inputIter child = iLeftChild(begin);
            if(child + 1 < end && *child < *(child + 1)){
                ++child;
            }
            if(*begin < *child){
                std::swap(*begin, *child);
                begin = child;
            }
            else{
                return;
            }
        }
    }

    template<class inputIter>
    void heapify(inputIter& begin, inputIter& end){
        inputIter start = begin;
        start = iParent(end + 1);
        while(start > 0){
            --start;
            siftDown(start, end);
        }
    }

    template<class inputIter>
    inputIter partition(inputIter& begin, inputIter& end){
        inputIter mid = begin + (end - begin) / 2;
        inputIter start = begin;
        inputIter last = end;
        while(true){
            while(*start < *mid){
                ++start;
            }
            --last;
            while(*last > *mid){
                --last;
            }
            if(!(start < last)){
                return last;
            }
            std::swap(*start, *last);
            ++start;
        }
    }
    
    template<class inputIter>
    void insertionSort(inputIter& begin, inputIter& end){
        inputIter start = begin;
        ++start;
        while(start < end){
            inputIter it = start;
            while(it > begin){
                if(*(it - 1) > *it){
                    std::swap(*it, *(it - 1));
                }
                --it;
            }
            ++start;
        }
    }

    template<class inputIter>
    void heapSort(inputIter& begin, inputIter& end){
        size_t length = end - begin;
        heapify(begin, end);

        while(end > (begin + 1)){
            --end;
            std::swap(*end, *begin);
            siftDown(begin, end);
        }
    }
}

namespace custom{
    template<class inputIter>
    void introsortLoop(inputIter&begin, inputIter& end, size_t max_depth){
        size_t length = end - begin;
        if(length < 16){
            detail::insertionSort(begin, end);
        }
        else if(max_depth == 0){
            detail::heapSort(begin, end);
        }
        else{
            inputIter p = detail::partition(begin, end);
            introsortLoop(begin, p, max_depth - 1);
            inputIter p2 = p + 1;
            introsortLoop(p2, end, max_depth - 1);
        }
    }
    template<class inputIter>
    void _sort(inputIter& begin, inputIter& end, size_t max_depth){
        if(begin != end){
            introsortLoop(begin, end, max_depth);
            detail::insertionSort(begin, end);
        }
    }

    template<class inputIter>
    void sort(inputIter begin, inputIter end){
        size_t max_depth = log2(end - begin);
        _sort(begin, end, max_depth);
    }
}
#endif //SORT