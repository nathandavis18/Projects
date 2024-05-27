#ifndef MERGE
#define MERGE

/*
 * custom::merge merges two sorted vectors together.
 * Attempting to merge two unsorted vectors will result in UB (undefined behavior)
*/
namespace custom{
    template<typename InputIterator, class OutputIterator>
    /**
     * first1 = the start of vector1. last1 = the end of vector1.
     * first2 = the start of vector2. last2 = the end of vector2.
     * d_first = the start of the resulting vector.
    */
    OutputIterator merge(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator d_first){ //Merges two sorted vectors together, much like std::merge
        while(first1 != last1){
            if(first2 == last2){
                return std::copy(first1, last1, d_first); //If we have reached the end of the 2nd vector, then we just move the remaining elements from vector 1 into the new vector
            }
            if(*first2 < *first1){ //If the value pointed to by first2 is < first1, it comes first
                *d_first = *first2;
                ++first2;
            }
            else{
                *d_first = *first1;
                ++first1;
            }
            ++d_first;
        }
        return std::copy(first2, last2, d_first); //Copy the remaining elements from vector 2 to the new vector
    }
}
#endif //MERGE
