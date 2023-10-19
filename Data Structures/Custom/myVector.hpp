#include <iterator>
#include "myIterator.hpp"

namespace custom{
    template <typename T>
    class myVector{
    public:
        using value_type = T;

        myVector() : size(0), buffer(new value_type[0]) {}
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
        ~myVector(){
            delete[] buffer;
        }

        myVector& operator=(myVector tmp){
            tmp.swap(*this);
            return *this;
        }

        void operator=(value_type obj){
            this = obj;
        }

        value_type& operator[](int index) const& {
            return buffer[index];
        }

        value_type& at(int index){
            if(index > 0 && index < size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        void push_back(value_type data){
            unsigned long long newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];

            for(unsigned long long i = 0; i < size; ++i){
                newBuffer[i] = buffer[i];
            }
            std::swap(size, newSize);
            std::swap(buffer, newBuffer);
            delete[] newBuffer;

            buffer[size - 1] = data;
        }
        void push_front(value_type data){
            unsigned long long newSize = size + 1;
            value_type* newBuffer = new value_type[newSize];
            
            newBuffer[0] = data;
            for(unsigned long long i = 0; i < size; ++i){
                newBuffer[i + 1] = buffer[i];
            }
            std::swap(size, newSize);
            std::swap(buffer, newBuffer);
            delete[] newBuffer;
        }
        void pop_back(){
            --size;
            buffer[size] = 0;
        }
        void pop_front(){
            for(int i = 1; i < size; ++i){
                buffer[i - 1] = buffer[i];
            }
            --size;
            buffer[size] = 0;
        }
        myIterator<value_type> begin(){
            return myIterator<value_type>(&buffer[0]);
        }
        myIterator<value_type> end(){
            return myIterator<value_type>(&buffer[size]);
        }
        constexpr unsigned long long getSize() const& { return size; }
    private:
        value_type* buffer;
        unsigned long long size;

        void swap(myVector& cur) noexcept{
            std::swap(size, cur.size);
            std::swap(buffer, cur.buffer);
        }
    };
}