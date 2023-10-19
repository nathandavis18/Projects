namespace custom{
    template<class input, class output>
    output merge(input first1, input last1, input first2, input last2, output d_first){
        for(; first1 != last1; ++d_first){
            if(first2 == last2){
                return std::copy(first1, last1, d_first);
            }
            if(*first2 < *first1){
                *d_first = *first2;
                ++first2;
            }
            else{
                *d_first = *first1;
                ++first1;
            }
        }
        return std::copy(first2, last2, d_first);
    }
}
