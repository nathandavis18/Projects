namespace custom{
    template<class input, class output>
    output merge(input first1, input last1, input first2, input last2, output d_first){ //Merges two vectors together, much like std::merge
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
