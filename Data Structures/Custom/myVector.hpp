#ifndef MYVEC
#define MYVEC
#include "myIterator.hpp"
#include "myReverseIterator.hpp"
#include <vector> //Only used for comparing my vector to an std::vector
namespace custom{

    template <typename T>
    class myVector{
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        myVector() : m_size(0), m_capacity(0), buffer(nullptr) {} //Default Constructor

        myVector(const myVector& vec) : m_size(vec.m_size), m_capacity(vec.m_size), buffer(reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * vec.m_size))) { //Copy Constructor
            for(int i = 0; i < m_size; ++i){
                buffer[i] = vec[i];
            }
        }
        myVector(myVector&& moveVec) : m_size(std::move(moveVec.m_size)), m_capacity(std::move(moveVec.m_capacity)), 
                 buffer(std::move(moveVec.buffer)) {} //Move constructor

        myVector(const size_t capacity) : m_size(capacity), m_capacity(capacity), buffer(reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * capacity))) {} //Capacity constructor 

        //Initializer List Constructor
        myVector(std::initializer_list<value_type> il) : m_size(il.end() - il.begin()), m_capacity(il.end() - il.begin()), buffer(reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * (il.end() - il.begin())))) {
            auto it = il.begin();
            auto insertIt = begin();
            while(it != il.end()){
                *insertIt = *it;
                ++insertIt; ++it;
            }
        }

        ~myVector(){ //Destructor
            delete[] buffer;
        }

        [[nodiscard]] myVector& operator=(const myVector& tmp) noexcept { //Copy assignment
            m_size = tmp.m_size;
            m_capacity = tmp.m_capacity;
            buffer = tmp.buffer;
            return *this;
        }

        myVector& operator=(myVector&& moveVec) noexcept { //Move assignment
            if(this == &moveVec) return *this; //Moving something into itself doesn't make sense

            m_size = std::move(moveVec.m_size);
            m_capacity = std::move(moveVec.m_capacity);
            buffer = std::move(moveVec.buffer);
            return *this;
        }

        myVector& operator=(std::initializer_list<value_type> il) noexcept { //Initializer List assignment
            m_size = il.end() - il.begin();
            m_capacity = il.end() - il.begin();
            buffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * m_capacity));
            auto it = il.begin();
            auto insertIt = begin();
            while(it != il.end()){
                *insertIt = *it;
                ++insertIt; ++it;
            }
            return *this;
        }

        [[nodiscard]] bool operator==(const myVector& b) noexcept { //Tests if two custom vectors are equal
            if(m_size != b.m_size) return false;
            for(int i = 0; i < m_size; ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }
        [[nodiscard]] bool operator==(const std::vector<T>& b) noexcept { //Tests if a std::vector and myVector are equal
            if(m_size != b.size()) return false;
            for(int i = 0; i < m_size; ++i) { 
                if((*this)[i] != b[i]) return false;
                
            }
            return true;
        }

        [[nodiscard]] reference operator[](size_t index) noexcept { //Returns a read/write value at given index, no exception handling
            return *(buffer + index);
        }

        [[nodiscard]] const_reference operator[](size_t index) const noexcept { //Returns a read-only value at the given index, no exception handling
            return *(buffer + index);
        }

        [[nodiscard]] reference at(int index) { //Returns a read/write value at the given index
            if(index >= 0 && index < m_size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        [[nodiscard]] const_reference at(int index) const { //Returns a read-only value at the given index
            if(index >= 0 && index < m_size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        void push_back(const_reference data) noexcept { //Adds a new value to the back of the vector. 
            if(m_size + 1 > m_capacity){ //If the new size > capacity, allocate more space
                size_t newSize = m_size + 1;
                size_t newCapacity = newSize;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * newCapacity));

                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i] = buffer[i];
                }
                std::swap(m_capacity, newCapacity);
                std::swap(m_size, newSize);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            buffer[m_size - 1] = data;
        }
        void push_back(T&& data) noexcept { //Adds a moved value to the back of the vector
            if(m_size + 1 > m_capacity){
                size_t newSize = m_size + 1;
                size_t newCapacity = newSize;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * newCapacity));

                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i] = buffer[i];
                }
                std::swap(m_capacity, newCapacity);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            buffer[m_size++] = std::move(data);
        }

        void push_front(const_reference data) noexcept { //Adds the new value to the front of the vector, after moving everything forward one position
            if(m_size + 1 > m_capacity){ //Allocates more space if needed
                size_t newSize = m_size + 1;
                size_t newCapacity = newSize;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * newCapacity));
                
                newBuffer[0] = data;
                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i + 1] = buffer[i];
                }
                std::swap(m_size, newSize);
                std::swap(m_capacity, newCapacity);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            else{
                for(size_t i = 1; i < m_size; ++i){
                    buffer[i - 1] = buffer[i];
                }
                buffer[0] = data;
            }
        }
        void push_front(T&& data) noexcept { //Adds the new value to the front of the vector, after moving everything forward one position
            if(m_size + 1 > m_capacity){ //Allocates more space if needed
                size_t newSize = m_size + 1;
                size_t newCapacity = newSize;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * newCapacity));
                
                newBuffer[0] = data;
                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i + 1] = buffer[i];
                }
                std::swap(m_size, newSize);
                std::swap(m_capacity, newCapacity);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            else{
                for(size_t i = 1; i < m_size; ++i){
                    buffer[i - 1] = buffer[i];
                }
                buffer[0] = std::move(data);
            }
        }

        void pop_back() noexcept { //Removes the last element and decreases m_size of the vector
            buffer[--m_size].~value_type(); //Calls the destructor on the object if required
        }
        void pop_front() noexcept { //Removes the first element, moves everything forward, and decreases m_size
            buffer[0].~value_type(); //Calls the destructor if the object requires it
            myIterator<value_type> it = begin() + 1;
            while(it != end()){
                *(it - 1) = *it;
                ++it;
            }
            --m_size;
        }

        void reserve(size_t capacity){
            if(capacity < m_capacity) return; //Reserve doesn't shrink capacity, only makes it grow

            pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * capacity));
            for(int i = 0; i < m_size; ++i){
                newBuffer[i] = buffer[i];
            }

            m_capacity = capacity;
            std::swap(buffer, newBuffer);
            delete[] newBuffer;
        }

        void resize(size_t newSize) noexcept {
            if(newSize < m_size){
                for(int i = m_size - 1; i >= newSize; --i){
                    buffer[i].~value_type();
                }
            }
            if(newSize > m_capacity){
                m_capacity = newSize;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * m_capacity));
                for(int i = 0; i < m_size; ++i){
                    newBuffer[i] = buffer[i];
                }
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            m_size = newSize;
        }

        void reverse() noexcept {
            if(begin() == end() || begin() + 1 == end()) return; //Vectors of size 0 and 1 are already reversed

            auto fIt = begin(), bIt = end() - 1;
            while(fIt < bIt){
                std::iter_swap(fIt, bIt);
                ++fIt; --bIt;
            }
        }

        void shrink_to_fit() noexcept { //Deallocates space such that the vector will have size == capacity
            if(m_capacity == m_size) return; //Already properly shrunk
            
            pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * m_size));
            for(int i = 0; i < m_size; ++i){
                newBuffer[i] = buffer[i];
            }
            std::swap(buffer, newBuffer);
            m_capacity = m_size;
            delete[] newBuffer;
        }

        void swap(myVector& v) noexcept{ //Swaps two vectors
            std::swap(m_capacity, v.m_capacity);
            std::swap(m_size, v.m_size);
            std::swap(buffer, v.buffer);
        }

        void clear() noexcept { //Removes all the data from a vector. Doesn't affect capacity
            while(m_size > 0){
                buffer[--m_size].~value_type();
            }
        }

        void insert(myIterator<value_type> it, const_reference value){
            if(m_size == m_capacity){
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * ++m_capacity));
                for(int i = 0; i < it - begin(); ++i){
                    newBuffer[i] = buffer[i];
                }
                newBuffer[it - begin()] = value;
                for(int i = it - begin(); i < m_size; ++i){
                    newBuffer[i + 1] = buffer[i];
                }
                ++m_size;
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            else{
                for(int i = it - begin(); i < m_size; ++i){
                    buffer[i + 1] = buffer[i];
                }
                buffer[it - begin()] = value;
                ++m_size;
            }
        }

        template<class... Args>
        void emplace(myIterator<value_type> it, Args... args){
            if(m_size == m_capacity){
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * ++m_capacity));
                for(int i = 0; i < it - begin(); ++i){
                    newBuffer[i] = buffer[i];
                }
                newBuffer[it - begin()] = value_type(args...);
                for(int i = it - begin(); i < m_size; ++i){
                    newBuffer[i + 1] = buffer[i];
                }
                ++m_size;
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            else{
                for(int i = it - begin(); i < m_size; ++i){
                    buffer[i + 1] = buffer[i];
                }
                buffer[it - begin()] = value_type(args...);
                ++m_size;
            }
        }
        template<class... Args>
        void emplace_back(Args... args){
            if(m_size == m_capacity){
                size_t newCapacity = m_capacity + 1;
                pointer newBuffer = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * newCapacity));

                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i] = buffer[i];
                }
                std::swap(m_capacity, newCapacity);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            buffer[m_size++] = value_type(args...);
        }

        reference front() noexcept { return *begin(); } //Returns a read/write value for the object at the front of the vector
        const_reference front() const noexcept { return *cbegin(); } //Returns a read-only value for the object at the front of the vector
        reference back() noexcept { return *(begin() + (m_size  - 1)); } //Returns a read/write value for the object at the back of the vector
        const_reference back() const noexcept { return *(begin() + (m_size - 1)); } //Returns a read-only value for the object at the back of the vector

        myIterator<value_type> begin() noexcept { //Returns a random-access iterator pointing to the front of the vector
            return myIterator<value_type>(buffer);
        }

        const myIterator<value_type> cbegin() const noexcept{ //Returns a conset random-access iterator to the first element in the vector
            return myIterator<value_type>(buffer);
        }

        myIterator<value_type> end() noexcept { //Returns a random-access iterator pointing just beyond the vector
            return myIterator<value_type>(buffer + m_size);
        }

        const myIterator<value_type> cend() const noexcept { //Returns a const random-access iterator just beyond the vector
            return myIterator<value_type>(buffer + m_size);
        }

        myReverseIterator<value_type> rbegin() noexcept { //Returns a random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(buffer + m_size - 1);
        }

        const myReverseIterator<value_type> crbegin() const noexcept { //Returns a const random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(buffer + m_size - 1);
        }

        myReverseIterator<value_type> rend() noexcept { //Returns a random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(buffer - 1);
        }

        const myReverseIterator<value_type> crend() const noexcept { //Returns a const random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(buffer - 1);
        }

        [[nodiscard]] bool isEmpty() const noexcept { return begin() == end(); } 

        const size_t size() const noexcept { return size_t(cend() - cbegin()); }

        const size_t capacity() const noexcept {return m_capacity; }

    private:
        pointer buffer;
        size_t m_size;
        size_t m_capacity;
    };
}
#endif //MYVEC