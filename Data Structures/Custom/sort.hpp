#ifndef SORT
#define SORT
namespace custom{
    template<class input>
    void _sort(input& in, int left, int mid, int right){ //Merge sort algorithm
        const int subArray1 = mid - left + 1;
        const int subArray2 = right - mid;

        input leftArray(subArray1); //An array or vector which is the same type as the inputted vector, a type-independent array
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
#endif //SORT