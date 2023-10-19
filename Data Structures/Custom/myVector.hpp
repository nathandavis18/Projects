#include <iterator>
#include "myIterator.hpp"

namespace custom{
    template <typename T>
    class myVector{
    public:
        using value_type = T;

        myVector() : size(0), buffer(new value_type[0]) {} //Default Constructor

        myVector(const myVector& vec){ //Copy Constructor
            size = vec.getSize();
            buffer = new value_type[size];
            for(int i = 0; i < size; ++i){
                buffer[i] = vec[i];
            }
        }
        myVector(const unsigned long long capacity){ //Capacity constructor
            size = capacity;
            buffer = new value_type[size];
        }
        ~myVector(){ //Destructor
            delete[] buffer;
        }

        myVector& operator=(myVector tmp){ //moves all the elements from tmp into the current vector
            tmp.swap(*this);
            return *this;
        }

        value_type& operator[](int index) const& { //Gets the value at the given index, without exception handling
            return buffer[index];
        }

        value_type& at(int index){ //Gets the value at the given index, with exception handling
            if(index > 0 && index < size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        void push_back(value_type data){ //Adds a new value to the back of the vector
            unsigned long long newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];

            for(unsigned long long i = 0; i < size; ++i){
                newBuffer[i] = buffer[i];
            }
            size = newSize;
            std::swap(buffer, newBuffer);
            delete[] newBuffer;

            buffer[size - 1] = data;
        }
        void push_front(value_type data){ //Adds the new value to the front of the vector, after moving everything forward one position
            unsigned long long newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];
            
            newBuffer[0] = data;
            for(unsigned long long i = 0; i < size; ++i){
                newBuffer[i + 1] = buffer[i];
            }
            size = newSize;
            std::swap(buffer, newBuffer);
            delete[] newBuffer;
        }

        void pop_back(){ //Removes the last element and decreases the size of the vector
            --size;
            buffer[size] = 0;
        }
        void pop_front(){ //Removes the first element, moves everything forward, and decreases the size
            for(int i = 1; i < size; ++i){
                buffer[i - 1] = buffer[i];
            }
            --size;
            buffer[size] = 0;
        }

        myIterator<value_type> begin(){ //Returns a custom iterator to the first element in the vector
            return myIterator<value_type>(&buffer[0]);
        }

        myIterator<value_type> end(){ //Returns a custom iterator just beyond the vector
            return myIterator<value_type>(&buffer[size]);
        }

        constexpr unsigned long long getSize() const& { return size; }

    private:
        value_type* buffer;
        unsigned long long size;

        void swap(myVector& cur) noexcept{
            size = cur.size;
            std::swap(buffer, cur.buffer);
        }
    };
}