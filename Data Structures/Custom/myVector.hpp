#include <iterator>
#include "myIterator.hpp"

namespace custom{
    template <typename T>
    class myVector{
    public:
        myVector() : size(0), buffer(new T[0]) {}
        myVector(const myVector& vec){ //Copy Constructor
            size = vec.getSize();
            buffer = new T[size];
            for(int i = 0; i < size; ++i){
                buffer[i] = vec[i];
            }
        }
        myVector(const unsigned long long capacity){ //Capacity constructor
            size = capacity;
            buffer = new T[size];
        }
        ~myVector(){
            delete[] buffer;
        }

        myVector& operator=(myVector tmp){
            tmp.swap(*this);
            return *this;
        }

        T& operator[](int index) const& {
            return buffer[index];
        }

        T& at(int index){
            if(index > 0 && index < size){
                return buffer[index];
            }
            throw std::out_of_range("Invalid index");
        }

        void push_back(T data){
            unsigned long long newSize = size + 1;
            T* newBuffer = new T[newSize];

            for(unsigned long long i = 0; i < size; ++i){
                newBuffer[i] = buffer[i];
            }
            std::swap(size, newSize);
            std::swap(buffer, newBuffer);
            delete[] newBuffer;

            buffer[size - 1] = data;
        }
        void push_front(T data){
            unsigned long long newSize = size + 1;
            T* newBuffer = new T[newSize];
            
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
        myIterator<T> begin(){
            return myIterator<T>(&buffer[0]);
        }
        myIterator<T> end(){
            return myIterator<T>(&buffer[size]);
        }
        constexpr unsigned long long getSize() const& { return size; }
    private:
        T* buffer;
        unsigned long long size;

        void swap(myVector& cur) noexcept{
            std::swap(size, cur.size);
            std::swap(buffer, cur.buffer);
        }
    };
}