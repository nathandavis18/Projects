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

    template<class input>
    void _sort(input& in, int left, int mid, int right){
        const int subArray1 = mid - left + 1;
        const int subArray2 = right - mid;

        input leftArray(subArray1);
        input rightArray(subArray2);

        for(int i = 0; i < subArray1; ++i)
            leftArray[i] = in[left + i];
        for(int j = 0; j < subArray2; ++j)
            rightArray[j] = in[mid + 1 + j];

        int indexSub1 = 0, indexSub2 = 0;
        int indexMerged = left;

        while(indexSub1 < subArray1 && indexSub2 < subArray2){
            if(leftArray[indexSub1] <= rightArray[indexSub2]){
                in[indexMerged] = leftArray[indexSub1];
                ++indexSub1;
            }
            else{
                in[indexMerged] = rightArray[indexSub2];
                ++indexSub2;
            }
            ++indexMerged;
        }
        while(indexSub1 < subArray1){
            in[indexMerged] = leftArray[indexSub1];
            ++indexSub1;
            ++indexMerged;
        }
        while(indexSub2 < subArray2){
            in[indexMerged] = rightArray[indexSub2];
            ++indexSub2;
            ++indexMerged;
        }
    }
    template<class input>
    void sort(input& in, int start, int last){
        if(start >= last)
            return;
        
        int mid = start + (last - start) / 2;
        sort(in, start, mid);
        sort(in, mid + 1, last);
        _sort(in, start, mid, last);
    }
}
