#ifndef MYVEC
#define MYVEC
#include "myIterator.hpp"
#include "myReverseIterator.hpp"
#include <vector>
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

        myVector(const myVector& vec) : m_size(vec.m_size), m_capacity(vec.m_size), buffer(new value_type[vec.m_size]) { //Copy Constructor
            for(int i = 0; i < m_size; ++i){
                buffer[i] = vec[i];
            }
        }
        myVector(myVector&& moveVec) : m_size(std::exchange(moveVec.m_size, 0)), m_capacity(std::exchange(moveVec.m_capacity, 0)), 
                 buffer(std::exchange(moveVec.buffer, nullptr)) {} //Move constructor

        myVector(const size_t capacity) : m_size(capacity), m_capacity(capacity), buffer(new value_type[capacity]) {} //Capacity constructor 

        //Initializer List Constructor
        myVector(std::initializer_list<value_type> il) : m_size(il.end() - il.begin()), m_capacity(il.end() - il.begin()), buffer(new value_type[il.end() - il.begin()]) {
            auto it = il.begin();
            auto insertIt = this->begin();
            while(it != il.end()){
                *insertIt = *it;
                ++insertIt; ++it;
            }
        }

        ~myVector(){ //Destructor
            delete[] buffer;
        }

        [[nodiscard]] myVector& operator=(const myVector& tmp) noexcept { //Copy assignment
            this->m_size = tmp.m_size;
            this->m_capacity = tmp.m_capacity;
            this->buffer = tmp.buffer;
            return *this;
        }

        myVector& operator=(myVector&& moveVec) noexcept { //Move assignment
            if(this == &moveVec) return *this; //Moving something into itself doesn't make sense

            std::swap(this->m_size, moveVec.m_size);
            std::swap(this->m_capacity, moveVec.m_capacity);
            std::swap(this->buffer, moveVec.buffer);
            return *this;
        }

        myVector& operator=(std::initializer_list<value_type> il) noexcept { //Initializer List assignment
            this->m_size = il.end() - il.begin();
            this->m_capacity = il.end() - il.begin();
            auto it = il.begin();
            auto insertIt = this->begin();
            while(it != il.end()){
                *insertIt = *it;
                ++insertIt; ++it;
            }
            return *this;
        }

        bool operator==(const myVector& b) noexcept { //Tests if two custom vectors are equal
            if(this->m_size != b.m_size) return false;
            for(int i = 0; i < this->m_size; ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }
        bool operator==(const std::vector<T>& b) noexcept { //Tests if a std::vector and myVector are equal
            if(this->m_size != b.size()) return false;
            for(int i = 0; i < this->m_size; ++i) { 
                if((*this)[i] != b[i]) return false;
                
            }
            return true;
        }

        [[nodiscard]] reference operator[](size_t index) noexcept { //Returns a read/write value at given index, no exception handling
            return *(this->buffer + index);
        }

        [[nodiscard]] const_reference operator[](size_t index) const noexcept { //Returns a read-only value at the given index, no exception handling
            return *(this->buffer + index);
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
                pointer newBuffer = new value_type[newCapacity];

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
                pointer newBuffer = new value_type[newCapacity];

                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i] = buffer[i];
                }
                std::swap(m_capacity, newCapacity);
                std::swap(m_size, newSize);
                std::swap(buffer, newBuffer);
                delete[] newBuffer;
            }
            buffer[m_size++] = std::move(data);
        }

        void push_front(const_reference data) noexcept { //Adds the new value to the front of the vector, after moving everything forward one position
            if(m_size + 1 > m_capacity){ //Allocates more space if needed
                size_t newSize = m_size + 1;
                pointer newBuffer = new value_type[newSize];
                
                newBuffer[0] = data;
                for(size_t i = 0; i < m_size; ++i){
                    newBuffer[i + 1] = buffer[i];
                }
                std::swap(m_size, newSize);
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

        void pop_back() noexcept { //Removes the last element and decreases m_size of the vector
            this->buffer[--this->m_size].~value_type(); //Calls the destructor on the object if required
        }
        void pop_front() noexcept { //Removes the first element, moves everything forward, and decreases m_size
            this->buffer[0].~value_type(); //Calls the destructor if the object requires it
            myIterator<value_type> it = this->begin() + 1;
            while(it != this->end()){
                *(it - 1) = *it;
                ++it;
            }
            --this->m_size;
        }

        void reserve(size_t capacity){
            if(capacity < m_capacity) return; //Reserve doesn't shrink capacity, only makes it grow

            pointer newBuffer = new value_type[capacity];
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
            m_size = newSize;
        }

        void shrink_to_fit() noexcept { //Deallocates space such that the vector will have size == capacity
            if(m_capacity == m_size) return; //Already properly shrunk
            
            pointer newBuffer = new value_type[m_size];
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

        reference front() noexcept { return *this->begin(); } //Returns a read/write value for the object at the front of the vector
        const_reference front() const noexcept { return *this->cbegin(); } //Returns a read-only value for the object at the front of the vector
        reference back() noexcept { return *(this->begin() + (m_size  -1)); } //Returns a read/write value for the object at the back of the vector
        const_reference back() const noexcept { return *(this->begin() + (m_size - 1)); } //Returns a read-only value for the object at the back of the vector

        myIterator<value_type> begin() noexcept { //Returns a random-access iterator pointing to the front of the vector
            return myIterator<value_type>(this->buffer);
        }

        const myIterator<value_type> cbegin() const noexcept{ //Returns a conset random-access iterator to the first element in the vector
            return myIterator<value_type>(this->buffer);
        }

        myIterator<value_type> end() noexcept { //Returns a random-access iterator pointing just beyond the vector
            return myIterator<value_type>(this->buffer + m_size);
        }

        const myIterator<value_type> cend() const noexcept { //Returns a const random-access iterator just beyond the vector
            return myIterator<value_type>(this->buffer + m_size);
        }

        myReverseIterator<value_type> rbegin() noexcept { //Returns a random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(this->buffer + this->m_size - 1);
        }

        const myReverseIterator<value_type> crbegin() const noexcept { //Returns a const random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(this->buffer + this->m_size - 1);
        }

        myReverseIterator<value_type> rend() noexcept { //Returns a random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(this->buffer - 1);
        }

        const myReverseIterator<value_type> crend() const noexcept { //Returns a const random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(this->buffer - 1);
        }

        [[nodiscard]] bool isEmpty() const noexcept { return this->begin() == this->end(); } 

        const size_t size() const noexcept { return size_t(cend() - cbegin()); }

        const size_t capacity() const noexcept {return m_capacity; }

    private:
        pointer buffer;
        size_t m_size;
        size_t m_capacity;
    };
}
#endif //MYVEC