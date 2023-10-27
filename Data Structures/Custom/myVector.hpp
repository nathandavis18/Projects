#ifndef MYVEC
#define MYVEC
#include "myIterator.hpp"
#include <vector>
#include <string>
namespace custom{

    template <typename T>
    class myVector{
    public:
        using value_type = T;

        myVector() : size(0), buffer(nullptr) {} //Default Constructor

        myVector(const myVector& vec) : size(vec.getSize()), buffer(new value_type[vec.getSize()]) { //Copy Constructor
            for(int i = 0; i < size; ++i){
                buffer[i] = vec[i];
            }
        }
        myVector(const size_t capacity) : size(capacity), buffer(new value_type[capacity]) {} //Capacity constructor 

        ~myVector(){ //Destructor
            delete[] buffer;
        }

        [[nodiscard]] const myVector& operator=(myVector& tmp) const noexcept { //copies all the elements from tmp into the current vector
            this->size = tmp.size;
            this->buffer = tmp.buffer;
            return *this;
        }

        [[nodiscard]] bool operator==(const myVector& b) noexcept { //Tests if two custom vectors are equal
            if(this->size != b.size) return false;
            for(int i = 0; i < this->size; ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }
        [[nodiscard]] bool operator==(const std::vector<T>& b) noexcept { //Tests if a std::vector and myVector are equal
            if(this->size != b.size()) return false;
            for(int i = 0; i < this->size; ++i) { 
                if((*this)[i] != b[i]) return false;
                
            }
            return true;
        }

        [[nodiscard]] value_type& operator[](size_t index) noexcept {
            return *(this->buffer + index);
        }

        [[nodiscard]] const value_type& operator[](size_t index) const noexcept { //Gets the value at the given index, without exception handling
            return *(this->buffer + index);
        }

        [[nodiscard]] const value_type& at(int index) const { //Gets the value at the given index, with exception handling
            if(index > 0 && index < size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        void push_back(const value_type& data) noexcept { //Adds a new value to the back of the vector
            size_t newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];

            for(size_t i = 0; i < size; ++i){
                newBuffer[i] = buffer[i];
            }
            std::swap(size, newSize);
            std::swap(buffer, newBuffer);
            delete[] newBuffer;

            buffer[size - 1] = data;
        }
        void push_front(const value_type& data) noexcept { //Adds the new value to the front of the vector, after moving everything forward one position
            size_t newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];
            
            newBuffer[0] = data;
            for(size_t i = 0; i < size; ++i){
                newBuffer[i + 1] = buffer[i];
            }
            std::swap(size, newSize);
            std::swap(buffer, newBuffer);
            delete[] newBuffer;
        }

        void pop_back() noexcept { //Removes the last element and decreases the size of the vector
            --this->size;
        }
        void pop_front() noexcept { //Removes the first element, moves everything forward, and decreases the size
            for(int i = 1; i < size; ++i){
                buffer[i - 1] = buffer[i];
            }
            --this->size;
        }

        void resize(size_t newSize) noexcept {
            this->size = newSize;
        }
        myIterator<value_type> begin() noexcept {
            return myIterator<value_type>(this->buffer);
        }

        const myIterator<value_type> cbegin() const noexcept{ //Returns a custom iterator to the first element in the vector
            return myIterator<value_type>(this->buffer);
        }

        myIterator<value_type> end() noexcept {
            return myIterator<value_type>(this->buffer + size);
        }

        const myIterator<value_type> cend() const noexcept { //Returns a custom iterator just beyond the vector
            return myIterator<value_type>(this->buffer + size);
        }

        const size_t getSize() const { return size; }

    private:
        value_type* buffer;
        size_t size;
    };
}
#endif //MYVEC