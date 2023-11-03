#ifndef SORT
#define SORT
#include <cmath>
namespace detail{
    /**
     * Heapsort
    */
    template<typename Iter>
    constexpr void pop_heap(Iter first, Iter last, Iter result){
        typedef typename std::iterator_traits<Iter>::value_type ValueType;
        typedef typename std::iterator_traits<Iter>::difference_type DistanceType;

        ValueType val = std::move(*result);
        *result = std::move(*first);
    }

    template<typename Iter>
    constexpr void my_sort_heap(Iter first, Iter last){
        while(last - first > 1){
            --last;
            pop_heap(first, last, last);
        }
    }

    template<class Iter, class Distance, class Type>
    constexpr void push_heap(Iter first, Distance index, Distance topIndex, Type val){
        Distance parent = (index - 1) / 2;
        while(index > topIndex && *(first + parent) < val){
            *(first + index) = std::move(*(first + parent));
            index = parent;
            parent = (index - 1) / 2;
        }
        *(first + index) = std::move(val);
    }

    template<class Iter, class Distance, class Type>
    constexpr void adjust_heap(Iter first, Distance index, Distance length, Type val){
        const Distance topIndex = index;
        Distance child = index;
        while(child < (length - 1) / 2){
            child = 2 * (child + 1);
            if(*(first + child) < *(first + (child - 1))) --child;
            *(first + index) = std::move(*(first + child));
            index = child;
        }
        if((length & 1) == 0 && child == (length - 2) / 2){
            child = 2 * (child + 1);
            *(first + index) = std::move(*(first + (child - 1)));
            index = child - 1;
            push_heap(first, index, topIndex, std::move(val));
        }
    }

    template<typename Iter>
    constexpr void my_make_heap(Iter first, Iter last){
        if(last - first < 2) return; //Arrays of size 0 and 1 are sorted
        typedef typename std::iterator_traits<Iter>::value_type ValueType;
        typedef typename std::iterator_traits<Iter>::difference_type DistanceType;

        const DistanceType length = last - first;
        DistanceType parent = (length - 2) / 2;
        while(true){
            ValueType val = std::move(*(first + parent));
            adjust_heap(first, parent, length, std::move(val));
            if(parent == 0) return;
            --parent;
        }
    }

    template<typename Iter>
    constexpr void heap_select(Iter first, Iter middle, Iter last){
        my_make_heap(first, middle);
        for(Iter i = middle; i < last; ++i){
            if(*i < *first) pop_heap(first, middle, i);
        }
    }

    template<typename Iter>
    constexpr void partial_sort(Iter first, Iter middle, Iter last){
        heap_select(first, middle, last);
        my_sort_heap(first, middle);
    }

    /**
     * Quicksort
    */
    template<typename Iter>
    constexpr Iter unguarded_partition(Iter first, Iter last, Iter pivot){
        while(true){
            while(*first < *pivot){
                ++first;
            }
            --last;
            while(*pivot < *last){
                --last;
            }
            if(!(first < last)) return first;
            std::iter_swap(first, last);
            ++first;
        }
    }

    template<typename Iter>
    constexpr void move_median_to_first(Iter result, Iter a, Iter b, Iter c){
        if(*a < *b){
            if(*b < *c) std::iter_swap(result, b);
            else if(*a < *c) std::iter_swap(result, c);
            else std::iter_swap(result, a);
        }
        else if(*a < *c) std::iter_swap(result, a);
        else if(*b < *c) std::iter_swap(result, c);
        else std::iter_swap(result, b);
    }

    template<typename Iter>
    constexpr Iter get_pivot(Iter first, Iter last){
        Iter mid = first + (last - first) / 2;
        move_median_to_first(first, first + 1, mid, last - 1);
        return unguarded_partition(first + 1, last, first);
    }

    /**
     * Insertion sort stuff
    */
    template<typename Iter>
    constexpr void linear_insert(Iter last){
        typename std::iterator_traits<Iter>::value_type
        val = std::move(*last);
        Iter next = last;
        --next;
        while(val < *next){
            *last = std::move(*next);
            last = next;
            --next;
        }
        *last = std::move(val);
    }
    template<typename Iter>
    constexpr void insertion_sort(Iter first, Iter last){
        if(first == last) return;
        for(Iter i = first + 1; i != last; ++i){
            if(*i < *first){
                typename std::iterator_traits<Iter>::value_type 
                val = std::move(*i);
                std::move_backward(first, i, i + 1);
                *first = std::move(val);
            }
            else{
                linear_insert(i);
            }
        }
    }
    template<typename Iter>
    constexpr void unguarded_insertion_sort(Iter first, Iter last){
        for(Iter i = first; i != last; ++i){
            linear_insert(i);
        }
    }
    template<typename Iter>
    constexpr void final_insertion_sort(Iter first, Iter last){
        if(last - first > 16){
            insertion_sort(first, first + 16);
            unguarded_insertion_sort(first + 16, last);
        }
        else insertion_sort(first, last);
    }
}

/**
 * Entry Point for user is custom::sort
*/
namespace custom{
    template<typename Iter>
    void introsort(Iter first, Iter last, size_t max_depth){
        while(last - first > 16){
            if(max_depth == 0){
                detail::partial_sort(first, last, last);
                return;
            }
            --max_depth;
            Iter p = detail::get_pivot(first, last);
            introsort(p, last, max_depth);
            last = p;
        }
    }
    template<typename Iter>
    void sort(Iter first, Iter last){
        size_t max_depth = std::log2(last - first) * 2;
        if(first >= last) return;
        introsort(first, last, max_depth);
        detail::final_insertion_sort(first, last);
    }
}
#endif //SORT