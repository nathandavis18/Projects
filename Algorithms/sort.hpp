#ifndef SORT
#define SORT
#include <cmath>

/**
 * This is my implementation of introsort (introspective sort).
 * Introsort is a hybrid sort, implementing quicksort as the base,
 * Heapsort when a specific recursion depth has been reached,
 * and Insertion Sort for final touches.
 * 
 * Introsort is an unstable sorting algorithm, but is an in-place sorting algorithm.
 * The Time Complexity for introsort is O(n log n), both best and worst case.
 *    To shorten the best-case time complexity, PDQ (pattern-defeating quicksort) is a viable alternative
 * 
 * The Space Complexity for introsort is O(log n), due to quicksort recursion
*/
namespace detail{

    //Heapsort Section ------------------------------------------------------------------------
    /**
     * Removes the sorted item from the heap
    */
    template<typename Iter>
    constexpr void pop_heap(Iter first, Iter result){
        *result = std::move(*first);
    }

    /**
     * Loops through the heap and removes each sorted item
    */
    template<typename Iter>
    constexpr void sort_heap(Iter first, Iter last){
        while(last - first > 1){
            --last;
            detail::pop_heap(first, last);
        }
    }

    /**
     * Moves nodes around to insert the current val into the correct place into the heap
    */
    template<class Iter, class Distance, class Type>
    constexpr void push_heap(Iter first, Distance index, Distance topIndex, Type val){
        Distance parent = (index - 1) / 2; //The current index's parent node
        while(index > topIndex && *(first + parent) < val){ //While the current index is not at the top of the tree and the current index's parent is < val
            *(first + index) = std::move(*(first + parent)); //Move the current index's parent into the index
            index = parent; //The index is now the parent node
            parent = (index - 1) / 2; //New parent node for current index
        }
        *(first + index) = std::move(val); //Value belongs in this node of the heap after the loop finishes
    }

    /**
     * Organizes the heap into a binary tree
    */
    template<class Iter, class Distance, class Type>
    constexpr void adjust_heap(Iter first, Distance index, Distance length, Type val){
        const Distance topIndex = index;
        Distance child = index;
        while(child < (length - 1) / 2){ //While the child is not to its parent node
            child = 2 * (child + 1); //Move to the next child node
            if(*(first + child) < *(first + (child - 1))) --child; //Get the larger of the two child nodes
            *(first + index) = std::move(*(first + child)); //Move the larger child node into the current index
            index = child;
        }
        if((length & 1) == 0 && child == (length - 2) / 2){ //If unbalanced heap and the child is the right child node
            child = 2 * (child + 1); //Move the child down and make it a left child
            *(first + index) = std::move(*(first + (child - 1)));
            index = child - 1;
            push_heap(first, index, topIndex, std::move(val));
        }
    }

    /**
     * Creates an unorganized heap
    */
    template<typename Iter>
    constexpr void make_heap(Iter first, Iter last){
        if(last - first < 2) return; //Arrays of size 0 and 1 are sorted
        typedef typename std::iterator_traits<Iter>::value_type ValueType;
        typedef typename std::iterator_traits<Iter>::difference_type DistanceType;

        const DistanceType length = last - first; //The length of this sub array
        DistanceType parent = (length - 2) / 2;
        while(true){//While the parent is not first
            ValueType val = std::move(*(first + parent));
            adjust_heap(first, parent, length, std::move(val)); //Move the parent to where it should be
            if(parent == 0) return;
            --parent;
        }
    }

    /**
     * Make the heap. Then, for each node,
     * starting with the midpoint -> end, if it is smaller than the first node remove it.
    */
    template<typename Iter>
    constexpr void heap_select(Iter first, Iter middle, Iter last){
        detail::make_heap(first, middle);
        for(Iter i = middle; i < last; ++i){
            if(*i < *first) detail::pop_heap(first, i);
        }
    }

    /**
     * Start the heap process
    */
    template<typename Iter>
    constexpr void partial_sort(Iter first, Iter middle, Iter last){
        heap_select(first, middle, last);
        detail::sort_heap(first, middle);
    }

    //End Heapsort Section----------------------------------------------------------------------------


    //Quicksort Section--------------------------------------------------------------------------------

    /**
     * Moves items based on a predetermined pivot until the starting pointer passes the last pointer
    */
    template<typename Iter>
    constexpr Iter unguarded_partition(Iter first, Iter last, Iter pivot){
        while(true){
            while(*first < *pivot){ //While the items on the left of the pivot are smaller than the pivot
                ++first;
            }
            --last;
            while(*pivot < *last){ //While the items on the right of the pivot are greater than the pivot
                --last;
            }
            if(!(first < last)) return first; 
            std::iter_swap(first, last);
            ++first;
        }
    }

    /**
     * Moves the median of 3 items to the front.
    */
    template<typename Iter>
    constexpr void move_median_to_first(Iter result, Iter a, Iter b, Iter c){
        if(*a < *b){
            if(*b < *c) std::iter_swap(result, b);// If a < b < c, b is the median
            else if(*a < *c) std::iter_swap(result, c); //If a < c < b, c is the median
            else std::iter_swap(result, a); //Else, c < a < b, and a is the median
        }
        else if(*a < *c) std::iter_swap(result, a); //If a > b and a < c, a is the median
        else if(*b < *c) std::iter_swap(result, c); //If a > b and a > c and b < c, c is the median
        else std::iter_swap(result, b); //If none of the above are true, b is the median
    }

    /**
     * Sets the pivot point to the front and starts sorting the partial array based on the pivot
    */
    template<typename Iter>
    constexpr Iter get_pivot(Iter first, Iter last){
        Iter mid = first + (last - first) / 2;
        move_median_to_first(first, first + 1, mid, last - 1);
        return unguarded_partition(first + 1, last, first);
    }
    //End Quicksort Section ------------------------------------------------------------------------------------

    
    //Insertion Sort Section -----------------------------------------------------------------------------------
    /**
     * Moves items backward until the last item can be properly placed
    */
    template<typename Iter>
    constexpr void linear_insert(Iter last){
        typename std::iterator_traits<Iter>::value_type 
            val = std::move(*last); //Move the last item into a temp value

        Iter next = last;
        --next; //Set the next item to the second to last item
        while(val < *next){ //While next > val, move the items backward to make space for the temp value
            *last = std::move(*next);
            last = next;
            --next;
        }
        *last = std::move(val); //Insert the temp value into its place
    }

    /**
     *Moves the items in the array backwards to fit the current item
    */
    template<typename Iter>
    constexpr void insertion_sort(Iter first, Iter last){
        if(first == last) return;
        for(Iter i = first + 1; i != last; ++i){
            if(*i < *first){
                typename std::iterator_traits<Iter>::value_type 
                    val = std::move(*i);
                std::move_backward(first, i, i + 1); //Moves the range [first, i] into [first + 1, i + 1]
                *first = std::move(val); //The array gets shifted so that [first, i] becomes [i, first->i+1]
            }
            else{
                linear_insert(i);
            }
        }
    }
    /**
     * For each item, shift the array as needed
    */
    template<typename Iter>
    constexpr void unguarded_insertion_sort(Iter first, Iter last){
        for(Iter i = first; i != last; ++i){
            linear_insert(i);
        }
    }
    /**
     * Breaks the array down into a chunk of 16 and the remaining chunk. Insertion sort gets ran on the first 16 items, and linear insertion sort for the rest.
     * If the sub-array is only 16 or less items long, only run insertion sort.
     * Insertion sort is faster on an array up to a certain amount (16 items is the average max). Otherwise, linear insertion sort is faster on average.
    */
    template<typename Iter>
    constexpr void final_insertion_sort(Iter first, Iter last){
        if(last - first > 16){
            insertion_sort(first, first + 16);
            unguarded_insertion_sort(first + 16, last);
        }
        else insertion_sort(first, last);
    }
    //End Insertion Sort Section ---------------------------------------------------------------
}

/**
 * Entry Point for user is custom::sort
*/
namespace custom{
    template<typename Iter>
    void introsort(Iter first, Iter last, size_t max_depth){
        while(last - first > 16){ //While the sub array size is > 16
            if(max_depth == 0){ //If we have hit the max recursion depth for the array, start doing heap sort instead of quicksort
                detail::partial_sort(first, last, last);
                return; //Stop the quicksort after heapsort
            }
            --max_depth; //Counting recursion calls
            Iter p = detail::get_pivot(first, last);
            introsort(p, last, max_depth);
            last = p; //Splitting the array into [first, p) and [p, last), where p is the quicksort pivot
        }
    }
    template<typename Iter>
    void sort(Iter first, Iter last){
        if(first >= last) return; //Empty array
        size_t max_depth = std::log2(last - first) * 2; //Gets the max recursion depth based on the array's size
        introsort(first, last, max_depth); //Start the bulk sorting
        detail::final_insertion_sort(first, last); //Do a quick run through to finish sorting the array
    }
}
#endif //SORT