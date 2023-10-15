#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <random>
#include <string>

std::default_random_engine gen;

class Pair{
public:
    std::string key;
    unsigned int value; 

    bool operator<(Pair& p1){
        return this->key < p1.key;
    }
};

void merge(std::vector<Pair>& v, int const left, int const mid, int const right){ //Merge sort algorithm
    int const sub1 = mid - left + 1;
    int const sub2 = right - mid;

    std::vector<Pair> leftVec(sub1);
    std::vector<Pair> rightVec(sub2);

    for(int i = 0; i < sub1; ++i){
        leftVec[i] = v[left + i];
    } 
    for(int j = 0; j < sub2; ++j){
        rightVec[j] = v[mid + 1 + j];
    }

    int indexSub1 = 0, indexSub2 = 0, mergeIndex = left;
    while(indexSub1 < sub1 && indexSub2 < sub2){
        if(leftVec[indexSub1].key <= rightVec[indexSub2].key){
            v[mergeIndex] = leftVec[indexSub1];
            ++indexSub1;
        }
        else{
            v[mergeIndex] = rightVec[indexSub2];
            ++indexSub2;
        }
        ++mergeIndex;
    }

    while(indexSub1 < sub1){
        v[mergeIndex] = leftVec[indexSub1];
        ++indexSub1;
        ++mergeIndex;
    }

    while(indexSub2 < sub2){
        v[mergeIndex] = rightVec[indexSub2];
        ++indexSub2;
        ++mergeIndex;
    }
}

void mergeSort(std::vector<Pair>& v, int const begin, int const end){ //Merge sort recursive call point
    if(begin >= end){
        return;
    }

    int mid = begin + (end - begin) / 2;
    mergeSort(v, begin, mid);
    mergeSort(v, mid + 1, end);
    merge(v, begin, mid, end);
}

std::vector<Pair> getResult(std::vector<Pair>& temp){ //Returns the vector with all the same key values added together
    std::map<std::string, unsigned int> m;
    std::vector<Pair> result;
    for(auto i : temp){
        int j = 0;
        if(m.find(i.key) != m.end()){
            result[m[i.key]].value += i.value;
        }
        else{
            m[i.key] = j++;
            result.push_back(i);
        }
    }
    return result;
}

void calcMergeRunTime(std::vector<Pair> v1, std::vector<Pair> v2){ //Using std::merge
    std::vector<Pair> temp;
    std::vector<Pair> result;

    clock_t start, end;
    double time;

    start = clock();

    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(temp));
    result = getResult(temp);

    end = clock();

    time = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time for std::merge algorithm approach: " << time << std::endl;
}

std::vector<Pair> umapTime(std::vector<Pair>& v1, std::vector<Pair>& v2){ //Using an unordered map
    std::unordered_map<std::string, unsigned int> um;

    for(int i = 0; i < v1.size(); ++i){
        um[v1[i].key] = i;
    }

    for(auto i : v2){
        if(um.find(i.key) != um.end()){
            v1[um[i.key]].value += i.value;
        }
        else{
            v1.push_back(i);
        }
    }

    mergeSort(v1, 0, v1.size() - 1); //Sort since unordered map may not be sorted

    return v1;

}

void calcUMapTime(std::vector<Pair> v1, std::vector<Pair> v2){ //Time for unordered map alg
    std::vector<Pair> result;

    clock_t start, end;
    double time;

    start = clock();
    result = umapTime(v1, v2);
    end = clock();

    time = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time for unordered map algorithm approach: " << time << std::endl;
}

std::vector<Pair> mapTime(std::vector<Pair>& v1, std::vector<Pair>& v2){ //Using std::map, which is sorted in place
    std::vector<Pair> result;
    std::map<std::string, unsigned int> m;

    for(auto i : v1){
        m[i.key] = i.value;
    }

    for(auto i : v2){
        m[i.key] = m[i.key] > 0 ? m[i.key] + i.value : i.value;
    }

    for(auto l : m){ //Moving everything from the map to a vector
        Pair p;
        p.key = l.first;
        p.value = l.second;
        result.push_back(p);
    }
    return result;

}

void calcMapTime(std::vector<Pair> v1, std::vector<Pair> v2){ //std::map time
    std::vector<Pair> result;

    clock_t start, end;
    double time;

    start = clock();
    result = mapTime(v1, v2);
    end = clock();

    time = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time for map algorithm approach: " << time << std::endl;
}

char randChar(){ //random character for the key values
    char const charset[] = "abcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<int> rChar(0, 25);

    return charset[rChar(gen)];
}

std::vector<Pair> genData(size_t size){
    std::vector<Pair> data;

    std::uniform_int_distribution<unsigned int> rand(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
    std::uniform_int_distribution<int> randCharAmount(1, 3);



    for(int i = 0; i < size; ++i){
        Pair pair;
        int rCAmount = randCharAmount(gen);
        std::string s(rCAmount, 0);
        for(int j = 0; j < rCAmount; ++j){
            s[j] = randChar();
        }
        pair.key = s;
        
        pair.value = rand(gen);
        data.push_back(pair);
    }

    return data;
}

int main(){
    std::vector<Pair> v1;
    std::vector<Pair> v2;

    v1 = genData(100000);
    v2 = genData(100000);
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());


    calcMergeRunTime(v1, v2); //.18 seconds average for 100k items in both vectors on my pc
    calcUMapTime(v1, v2);     //.12 seconds average for 100k items in both vectors on my pc
    calcMapTime(v1, v2);      //.14 seconds average for 100k items in both vectors on my pc
}