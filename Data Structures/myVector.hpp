#ifndef MYVEC
#define MYVEC
#include "myIterator.hpp"
#include "myReverseIterator.hpp"
#include <vector> //Only used for comparing my vector to an std::vector
#include <algorithm>

/**
 * This is my custom vector class. This has many of the same functionalities as std::vector. Has support for both standard and custom objects.
 * A couple of differences may be how space is allocated/deallocated, as well as the push_front and pop_front methods for this custom vector, which is something
 * std::vector is lacking.
 * 
 * I built this custom vector class to learn some memory management through heap allocation/deallocation and to learn templated classes
*/
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
        std::allocator<value_type> allocator;
        typedef std::allocator_traits<std::allocator<value_type>> alloc_traits;

        myVector() : m_capacity(0), buffer(nullptr), m_finish(nullptr) {} //Default Constructor

        myVector(const myVector& vec) : m_capacity(vec.capacity()), buffer(nullptr), m_finish(nullptr) {
            buffer = alloc_traits::allocate(allocator, m_capacity);
            m_finish = buffer;
            for (int i = 0; i < vec.size(); ++i) {
                alloc_traits::construct(allocator, buffer + i, *(vec.buffer + i));
                ++m_finish;
            }
        } //Copy Constructor

        myVector(myVector&& moveVec) noexcept : m_capacity(std::move(moveVec.m_capacity)), 
                 buffer(std::move(moveVec.buffer)), m_finish(std::move(moveVec.m_finish)) {} //Move constructor

        myVector(const size_t capacity) : m_capacity(capacity), buffer(alloc_traits::allocate(allocator, capacity)), m_finish(buffer + capacity) {
            for(myIterator<value_type> it = begin(); it != end(); ++it){
                std::uninitialized_fill(begin(), end(), typename std::iterator_traits<myIterator<value_type>>::value_type());
            }
        } //Capacity constructor 

        //Initializer List Constructor
        myVector(std::initializer_list<value_type> il) : m_capacity(il.end() - il.begin()), buffer(alloc_traits::allocate(allocator, il.size())), m_finish(buffer) {
            auto it = il.begin();
            for(int i = 0; i < il.size(); ++i){
                alloc_traits::construct(allocator, (buffer + i), *it);
                ++it;
                ++m_finish;
            }
        }

        ~myVector(){ //Destructor
            destroyObjects(buffer, m_finish);
            alloc_traits::deallocate(allocator, buffer, m_capacity);
        }

        myVector& operator=(const myVector& cpy) noexcept { //Copy assignment
            m_capacity = cpy.capacity();
            buffer = alloc_traits::allocate(m_capacity);
            m_finish = buffer;
            for (int i = 0; i < cpy.size(); ++i) {
                alloc_traits::construct(allocator, buffer + i, *(cpy.buffer + i));
                ++m_finish;
            }
            return *this;
        }

        myVector& operator=(myVector&& moveVec) noexcept { //Move assignment
            if(this == &moveVec) return *this; //Moving something into itself doesn't make sense

            m_capacity = std::move(moveVec.m_capacity);
            buffer = std::move(moveVec.buffer);
            m_finish = std::move(moveVec.m_finish);
            return *this;
        }

        myVector& operator=(std::initializer_list<value_type> il) noexcept { //Initializer List assignment
            m_capacity = il.end() - il.begin();
            buffer = alloc_traits::allocate(m_capacity);
            m_finish = buffer;
            auto it = il.begin();
            for(int i = 0; i < il.size(); ++i){
                alloc_traits::construct(allocator, buffer + i, *it);
                ++it;
                ++m_finish;
            }
            return *this;
        }

        [[nodiscard]] bool operator==(const myVector& b) noexcept { //Tests if two custom vectors are equal
            if(size() != b.size()) return false;
            for(int i = 0; i < size(); ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }
        [[nodiscard]] bool operator==(const std::vector<T>& b) noexcept { //Tests if a std::vector and myVector are equal
            if(size() != b.size()) return false;
            for(int i = 0; i < size(); ++i) { 
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
            try{
                return *(buffer + index);
            }
            catch(std::out_of_range exception){
                throw exception = "Invalid Index";
            }
        }

        [[nodiscard]] const_reference at(int index) const { //Returns a read-only value at the given index
            try{
                return *(buffer + index);
            }
            catch(std::out_of_range exception){
                throw exception = "Invalid Index";
            }
        }

        void push_back(const_reference data) { //Adds a new value to the back of the vector. 
            if(size() == m_capacity){ //If the new size > capacity, allocate more space
                realloc();
            }
            emplace_back(data);
        }

        void push_back(T&& data) { //Adds a moved value to the back of the vector
            emplace_back(std::move(data));
        }

        void push_front(const_reference data) { //Adds the new value to the front of the vector, after moving everything forward one position
            if(size() == m_capacity){ //Allocates more space if needed
                realloc();
            }
            move_forward(begin(), end());
            alloc_traits::construct(allocator, buffer, data);
            ++m_finish;
        }
        void push_front(T&& data) { //Adds the new value to the front of the vector, after moving everything forward one position
            emplace(begin(), std::move(data));
        }

        void pop_back() noexcept { //Removes the last element and decreases size() of the vector
            std::destroy_at(--m_finish); //Calls the destructor on the object if required
        }
        void pop_front() noexcept { //Removes the first element, moves everything forward, and decreases size()
            std::destroy_at(buffer);
            --m_finish;
            move_backward(begin(), end());
        }

        void reserve(size_t capacity){
            if(capacity <= m_capacity) return; //Reserve doesn't shrink capacity, only makes it grow
            realloc(capacity);
        }

        void resize(size_t newSize) {
            if(newSize < size()){
                while(m_finish != buffer + newSize){
                    std::destroy_at(--m_finish);
                }
            }
            else if(newSize > m_capacity){
                realloc(newSize);
            }
            std::uninitialized_fill(begin() + size(), begin() + newSize, typename std::iterator_traits<myIterator<value_type>>::value_type());
            m_finish = buffer + newSize;
        }

        void reverse() noexcept {
            if(begin() == end() || begin() + 1 == end()) return; //Vectors of size 0 and 1 are already reversed

            auto fIt = begin(), bIt = end() - 1;
            while(fIt < bIt){
                std::iter_swap(fIt, bIt);
                ++fIt; --bIt;
            }
        }

        void shrink_to_fit() { //Deallocates space such that the vector will have size == capacity
            if(m_capacity == size()) return; //Already properly shrunk

            size_t tmpCapacity = size();
            pointer newBuffer = alloc_traits::allocate(tmpCapacity);
            pointer newFinish = newBuffer;
            for(int i = 0; i < size(); ++i){
                alloc_traits::construct(allocator, newBuffer + i, std::move_if_noexcept(*(buffer + i)));
                ++newFinish;
            }
            std::swap(m_capacity, tmpCapacity);
            std::swap(buffer, newBuffer);
            std::swap(m_finish, newFinish);
            destroyObjects(newBuffer, newFinish);
            alloc_traits::deallocate(newBuffer, tmpCapacity);
        }

        void swap(myVector& v) noexcept{ //Swaps two vectors
            std::swap(m_capacity, v.m_capacity);
            std::swap(m_finish, v.m_finish);
            std::swap(buffer, v.buffer);
        }

        void clear() noexcept { //Removes all the data from a vector. Doesn't affect capacity
            while(m_finish > buffer){
                std::destroy_at(--m_finish);
            }
        }

        template<class Iter>
        void insert(Iter it, const_reference value){
            if(size() == m_capacity){
                size_t offset = it - begin(); //Get the iterator's offset so we don't lose it after reallocation
                realloc();
                it = begin() + offset; //Set the iterator back to the correct offset, since data may have been moved to a new location after reallocation.
            }
            move_forward(it, end());
            alloc_traits::construct(allocator, buffer + (it - begin()), value);
            ++m_finish;
        }

        template<class Iter, class... Args>
        void emplace(Iter it, Args&&... args){
            if(size() == m_capacity){
                size_t offset = it - begin();
                realloc();
                it = begin() + offset;
            }
            move_forward(it, end());
            alloc_traits::construct(allocator, buffer + (it - begin()), std::forward<Args>(args)...);
            ++m_finish;
        }
        template<class... Args>
        void emplace_back(Args&&... args){
            if(size() == m_capacity){
                realloc();
            }
            alloc_traits::construct(allocator, m_finish++, std::forward<Args>(args)...);
        }

        bool find(const_reference value_to_find){
            for(auto it = begin(); it != end(); ++it){
                if(*it == value_to_find) return true;
            }
            return false;
        }

        reference front() noexcept { return *begin(); } //Returns a read/write value for the object at the front of the vector
        const_reference front() const noexcept { return *cbegin(); } //Returns a read-only value for the object at the front of the vector
        reference back() noexcept { return *(begin() + (size()  - 1)); } //Returns a read/write value for the object at the back of the vector
        const_reference back() const noexcept { return *(begin() + (size() - 1)); } //Returns a read-only value for the object at the back of the vector

        myIterator<value_type> begin() noexcept { //Returns a random-access iterator pointing to the front of the vector
            return myIterator<value_type>(buffer);
        }

        const myIterator<value_type> cbegin() const noexcept{ //Returns a conset random-access iterator to the first element in the vector
            return myIterator<value_type>(buffer);
        }

        myIterator<value_type> end() noexcept { //Returns a random-access iterator pointing just beyond the vector
            return myIterator<value_type>(m_finish);
        }

        const myIterator<value_type> cend() const noexcept { //Returns a const random-access iterator just beyond the vector
            return myIterator<value_type>(m_finish);
        }

        myReverseIterator<value_type> rbegin() noexcept { //Returns a random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(m_finish - 1);
        }

        const myReverseIterator<value_type> crbegin() const noexcept { //Returns a const random-access reverse iterator pointing to the last element in the vector
            return myReverseIterator<value_type>(m_finish - 1);
        }

        myReverseIterator<value_type> rend() noexcept { //Returns a random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(buffer - 1);
        }

        const myReverseIterator<value_type> crend() const noexcept { //Returns a const random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(buffer - 1);
        }

        [[nodiscard]] bool isEmpty() const noexcept { return begin() == end(); } 

        const size_t size() const noexcept { return size_t(m_finish - buffer); }

        const size_t capacity() const noexcept {return m_capacity; }

    private:
        pointer buffer; //The pointer to where data is stored on the heap
        size_t m_capacity; //Actual capacity. Capacity will always be >= size, unless shrink_to_fit is called. Then capacity will always equal size

        pointer m_finish; //A pointer to T bytes past the last item (1 item's worth of space passed the last item)

        const size_t newCapacity() const noexcept { return m_capacity * 3 / 2 + 1; } //Geometric growth so a new allocation doesn't have to be made for every new element

        void destroyObjects(pointer& buff, pointer& finish) noexcept { //Destroys all elements in the vector as needed.
            std::destroy(begin(), end());
        }
        void realloc(size_t capacity = 0){ //Reallocates the vector when extra space is needed
            pointer newFinish, newBuffer;
            size_t tmpCapacity;
            try{
                tmpCapacity = capacity == 0 ? newCapacity() : capacity; //If capacity is 0 (default), get the new capacity value
                newBuffer = alloc_traits::allocate(tmpCapacity); //Uses the current type's allocator to allocate the proper amount of space.
                newFinish = newBuffer; //Temporarily sets the new finish to the start of the vector
                for(size_t i = 0; i < size(); ++i){
                    alloc_traits::construct(allocator, newBuffer + i, std::move_if_noexcept(*(buffer + i))); //Properly adds elements to the new vector
                    ++newFinish;
                }
                std::swap(m_capacity, tmpCapacity);
                std::swap(buffer, newBuffer);
                std::swap(m_finish, newFinish);

                destroyObjects(newBuffer, newFinish); //Destroy the old vector after the new vector has been swapped
                alloc_traits::deallocate(newBuffer, tmpCapacity); //Free the old space
            }
            catch(...){ //If something went wrong while allocating more space, deallocate the newly created space and throw the exception.
                destroyObjects(newBuffer, newFinish);
                alloc_traits::deallocate(newBuffer, tmpCapacity);
                throw;
            }
        }
        template<class Iter>
        void move_forward(Iter start, Iter end) noexcept {
            if(start == end) return;

            while(end != start){
                *end = *(end - 1);
                --end;
            }
        }
        template<class Iter>
        void move_backward(Iter start, Iter end) noexcept {
            if(start == end) return;

            while(start != end){
                *start = *(start + 1);
                ++start;
            }
        }
    };
}
#endif //MYVEC