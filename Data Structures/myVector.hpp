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
        /**
         * Default constructor 
         * Creates an empty vector
        */
        myVector() : m_capacity(0), m_buffer(nullptr), m_finish(nullptr) {}

        /**
         * Copy constructor 
         * Copies the capacity of the passed in vector and constructs each object to ensure it is properly stored
        */
        myVector(const myVector& vec) : m_capacity(vec.capacity()), m_buffer(nullptr), m_finish(nullptr) {
            m_buffer = alloc_traits::allocate(allocator, m_capacity);
            m_finish = m_buffer;
            for (int i = 0; i < vec.size(); ++i) {
                alloc_traits::construct(allocator, m_buffer + i, *(vec.m_buffer + i));
                ++m_finish;
            }
        }

        /**
         * Move constructor
         * Moves an existing vector into current vector
        */
        myVector(myVector&& moveVec) noexcept : m_capacity(std::move(moveVec.m_capacity)), 
                 m_buffer(std::move(moveVec.m_buffer)), m_finish(std::move(moveVec.m_finish)) {}

        /**
         * Capacity constructor 
         * Allocates @param capacity amount of space, and default initializes the vector
        */
        myVector(const size_t capacity) : m_capacity(capacity), m_buffer(alloc_traits::allocate(allocator, capacity)), m_finish(m_buffer + capacity) {
            for(myIterator<value_type> it = begin(); it != end(); ++it){
                std::uninitialized_fill(begin(), end(), typename std::iterator_traits<myIterator<value_type>>::value_type());
            }
        }

        /**
         * Initializer List Constructor
         * Takes in an initializer list and builds a vector with the data.
         * Initializer lists defined as {x, y, z}
        */
        myVector(std::initializer_list<value_type> il) : m_capacity(il.end() - il.begin()), m_buffer(alloc_traits::allocate(allocator, il.size())), m_finish(m_buffer) {
            auto it = il.begin();
            for(int i = 0; i < il.size(); ++i){
                alloc_traits::construct(allocator, (m_buffer + i), *it);
                ++it;
                ++m_finish;
            }
        }

        /**
         * Destructor
         * Destroys each object by calling its destructor and deallocates all of the used space
        */
        ~myVector(){
            destroyObjects(m_buffer, m_finish);
            alloc_traits::deallocate(allocator, m_buffer, m_capacity);
        }


        /**
         * Copy assignment
         * Copies the contents of the right-hand side vector into the left-hand side vector
        */
        myVector& operator=(const myVector& cpy) noexcept {
            m_capacity = cpy.capacity();
            m_buffer = alloc_traits::allocate(allocator, m_capacity);
            m_finish = m_buffer;
            for (int i = 0; i < cpy.size(); ++i) {
                alloc_traits::construct(allocator, m_buffer + i, *(cpy.m_buffer + i));
                ++m_finish;
            }
            return *this;
        }

        /**
         * Move assignment
         * Checks if the left-hand side vector == right-hand side vector
         * Moves right-hand side vector into left-hand side vector
        */
        myVector& operator=(myVector&& moveVec) noexcept {
            if(this == &moveVec) return *this; //Moving something into itself doesn't make sense

            m_capacity = std::move(moveVec.m_capacity);
            m_buffer = std::move(moveVec.m_buffer);
            m_finish = std::move(moveVec.m_finish);
            return *this;
        }

        /**
         * Initializer List assignment
         * Builds a vector from an initializer list 
        */
        myVector& operator=(std::initializer_list<value_type> il) noexcept {
            m_capacity = il.end() - il.begin(); //The length of the initializer list
            m_buffer = alloc_traits::allocate(allocator, m_capacity);
            m_finish = m_buffer;
            auto it = il.begin();
            for(int i = 0; i < il.size(); ++i){
                alloc_traits::construct(allocator, m_buffer + i, *it);
                ++it;
                ++m_finish;
            }
            return *this;
        }

        /**
         * Equal Comparison Operator
         * Compares two custom::myVector objects to test for equality
        */
        [[nodiscard]] bool operator==(const myVector& b) noexcept {
            if(size() != b.size()) return false;
            for(int i = 0; i < size(); ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }

        /**
         * Equality Comparison Operator
         * Compares a custom::myVector object with an std::vector to test for equality
        */
        [[nodiscard]] bool operator==(const std::vector<T>& b) noexcept {
            if(size() != b.size()) return false;
            for(int i = 0; i < size(); ++i) { 
                if((*this)[i] != b[i]) return false;
            }
            return true;
        }

        /**
         * Index Operator
         * Returns a read/write value at a given index.
         * Index operator does not provide index safety
        */
        [[nodiscard]] reference operator[](size_t index) noexcept {
            return *(m_buffer + index);
        }

        /**
         * Const Index Operator
         * Returns a read-only value at a given index.
         * Index operator does not provide index safety
        */
        [[nodiscard]] const_reference operator[](size_t index) const noexcept {
            return *(m_buffer + index);
        }

        /**
         * Returns a read/write value at the given index.
         * This function provides the index safety the index operator lacks
        */
        [[nodiscard]] reference at(int index) {
            try{
                return *(m_buffer + index);
            }
            catch(std::out_of_range exception){
                throw exception = "Invalid Index";
            }
        }

        /**
         * Returns a read-only value at the given index.
         * This function provides the index safety the index operator lacks
        */
        [[nodiscard]] const_reference at(int index) const {
            try{
                return *(m_buffer + index);
            }
            catch(std::out_of_range exception){
                throw exception = "Invalid Index";
            }
        }

        /**
         * Adds a new item to the back of the vector.
         * If vector is at full capacity, more space is allocated.
        */
        void push_back(const_reference data) {
            if(size() == m_capacity){
                realloc();
            }
            emplace_back(data);
        }

        /**
         * Adds a r-value/moved object to the back of the vector
        */
        void push_back(T&& data) {
            emplace_back(std::move(data));
        }

        /**
         * Adds an object to the front of the vector and moves everything else back
         * If vector is at full capacity, more space is allocated
        */
        void push_front(const_reference data) {
            if(size() == m_capacity){
                realloc();
            }
            move_forward(begin(), end());
            alloc_traits::construct(allocator, m_buffer, data);
            ++m_finish;
        }

        /**
         * Adds a r-value/moved object to the front of the vector
        */
        void push_front(T&& data) {
            emplace(begin(), std::move(data));
        }

        /**
         * Removes the last element from the vector and properly destroying the item as needed.
         * Does not affect capacity
        */
        void pop_back() noexcept {
            std::destroy_at(--m_finish);
        }

        /**
         * Removes the first element from the vector and properly destroys the item as needed.
         * Moves everything forward one index.
         * Does not affect capacity
        */
        void pop_front() noexcept {
            std::destroy_at(m_buffer);
            --m_finish;
            move_backward(begin(), end());
        }

        /**
         * Reserves and allocates extra space for the vector.
         * Cannot reserve less space than is already allocated.
        */
        void reserve(size_t capacity){
            if(capacity <= m_capacity) return;
            realloc(capacity);
        }

        /**
         * Resizes the vector.
         * If new size == current size, return.
         * If new size < current size, the excess objects are destroyed.
         * If new size > capacity, more space is allocated and capacity is set to the new size.
         * If new size > current size, default initialize the new space
        */
        void resize(size_t newSize) {
            if(newSize == size()) return;
            if(newSize < size()){
                while(m_finish != m_buffer + newSize){
                    std::destroy_at(--m_finish);
                }
            }
            else if(newSize > m_capacity){
                realloc(newSize);
            }
            std::uninitialized_fill(begin() + size(), begin() + newSize, typename std::iterator_traits<myIterator<value_type>>::value_type());
            m_finish = m_buffer + newSize;
        }

        /**
         * Reverses the current vector using a 2 pointer approach
        */
        void reverse() noexcept {
            if(begin() == end() || begin() + 1 == end()) return;

            auto fIt = begin(), bIt = end() - 1;
            while(fIt < bIt){
                std::iter_swap(fIt, bIt);
                ++fIt; --bIt;
            }
        }

        /**
         * Shrinks the vector to have the exact amount of space allocated as needed.
         * If capacity == size, vector is already properly shrunk.
         * Otherwise, if capacity > size, the vector is reallocated to have capacity == size
        */
        void shrink_to_fit() {
            if(m_capacity == size()) return; 

            size_t tmpCapacity = size();
            pointer newBuffer = alloc_traits::allocate(allocator, tmpCapacity);
            pointer newFinish = newBuffer;
            for(int i = 0; i < size(); ++i){
                alloc_traits::construct(allocator, newBuffer + i, std::move_if_noexcept(*(m_buffer + i)));
                ++newFinish;
            }
            std::swap(m_capacity, tmpCapacity);
            std::swap(m_buffer, newBuffer);
            std::swap(m_finish, newFinish);
            destroyObjects(newBuffer, newFinish);
            alloc_traits::deallocate(allocator, newBuffer, tmpCapacity);
        }

        /**
         * Swaps two vectors by swapping their capacities and their pointers, rather than swapping all of the data
        */
        void swap(myVector& v) noexcept{
            std::swap(m_capacity, v.m_capacity);
            std::swap(m_finish, v.m_finish);
            std::swap(m_buffer, v.m_buffer);
        }

        /**
         * Empties the vector.
         * Does not affect capacity
        */
        void clear() noexcept {
            while(m_finish > m_buffer){
                std::destroy_at(--m_finish);
            }
        }

        /**
         * Inserts an object at a specified location.
         * Location must be provided by an iterator, e.g. vec.begin() + x, or vec.end() - y.
         * Moves all objects from it - begin() forward to accomodate for the new objects position.
         * Allocates more space as needed
        */
        template<class Iter>
        void insert(Iter it, const_reference value){
            if(size() == m_capacity){
                size_t offset = it - begin(); 
                realloc();
                it = begin() + offset;
            }
            move_forward(it, end());
            alloc_traits::construct(allocator, m_buffer + (it - begin()), value);
            ++m_finish;
        }

        /**
         * Emplaces an object at a specified location.
         * Emplace can take an object or object values and construct them in-place.
         * I.e. if you have struct Person {int age, std::string name, Person(int age, std::string name) }, 
         *    emplace will construct the Person object if given integer and string values
        */
        template<class Iter, class... Args>
        void emplace(Iter it, Args&&... args){
            if(size() == m_capacity){
                size_t offset = it - begin();
                realloc();
                it = begin() + offset;
            }
            move_forward(it, end());
            alloc_traits::construct(allocator, m_buffer + (it - begin()), std::forward<Args>(args)...);
            ++m_finish;
        }

        /**
         * Emplaces an object at the back of the vector.
         * Much like push_back, but can construct an object in-place.
        */
        template<class... Args>
        void emplace_back(Args&&... args){
            if(size() == m_capacity){
                realloc();
            }
            alloc_traits::construct(allocator, m_finish++, std::forward<Args>(args)...);
        }

        /**
         * Attemps to find an object based on a value.
         * Returns true if found, otherwise returns false.
        */
        bool find(const_reference value_to_find){
            for(auto it = begin(); it != end(); ++it){
                if(*it == value_to_find) return true;
            }
            return false;
        }

        /**
         * Attemps to find an object based on a value.
         * Returns the iterator position if found.
        */
        template<class Iterator>
        Iterator find(const_reference value_to_find){
            for(auto it = begin(); it != end(); ++it){
                if(*it == value_to_find) return it;
            }
            return nullptr;
        }

        reference front() noexcept { return *begin(); } //Returns a read/write value for the object at the front of the vector
        const_reference front() const noexcept { return *cbegin(); } //Returns a read-only value for the object at the front of the vector
        reference back() noexcept { return *(end() - 1); } //Returns a read/write value for the object at the back of the vector
        const_reference back() const noexcept { return *(end() - 1); } //Returns a read-only value for the object at the back of the vector

        myIterator<value_type> begin() noexcept { //Returns a random-access iterator pointing to the front of the vector
            return myIterator<value_type>(m_buffer);
        }

        const myIterator<value_type> cbegin() const noexcept{ //Returns a const random-access iterator to the first element in the vector
            return myIterator<value_type>(m_buffer);
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
            return myReverseIterator<value_type>(m_buffer - 1);
        }

        const myReverseIterator<value_type> crend() const noexcept { //Returns a const random-access reverse iterator pointing in front of the vector
            return myReverseIterator<value_type>(m_buffer - 1);
        }

        /**
         * Checks if the vector is empty by checking if the start of the vector == end.
         * Returns true if empty.
        */
        [[nodiscard]] bool isEmpty() const noexcept { return begin() == end(); }

        /**
         * Calculates the size of the vector by finding the distance between the end and start of the vector's objects.
        */
        const size_t size() const noexcept { return size_t(m_finish - m_buffer); }

        /**
         * Returns the capacity of the vector
        */
        const size_t capacity() const noexcept {return m_capacity; }

    private:

        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        std::allocator<value_type> allocator; //The allocator for the current object type. Uses generic allocator if type does not have a custom allocator
        typedef std::allocator_traits<std::allocator<value_type>> alloc_traits; //The allocator traits used to allocate and construct objects

        pointer m_buffer; //The pointer to where data is stored on the heap
        size_t m_capacity; //Actual capacity. Capacity will always be >= size.

        pointer m_finish; //A pointer to T bytes past the last item (1 item's worth of space passed the last item)

        const size_t newCapacity() const noexcept { return m_capacity * 3 / 2 + 1; } //Geometric growth so a new allocation doesn't have to be made for every new element

        /**
         * Destroys objects from the starting point to the ending point
         * Asserts that the iterators are actually iterators
        */
       template<class Iter>
        void destroyObjects(Iter start, Iter end) noexcept {
            std::destroy(start, end);
        }

        /**
         * Reallocates space when the vector's capacity is full and the user tries to add more objects.
         * Also used when reserve is called
        */
        void realloc(size_t capacity = 0){
            pointer newFinish, newBuffer;
            size_t tmpCapacity;
            try{
                tmpCapacity = capacity == 0 ? newCapacity() : capacity; //If capacity is 0 (default), get the new capacity value
                newBuffer = alloc_traits::allocate(allocator, tmpCapacity); //Uses the current type's allocator to allocate the proper amount of space.
                newFinish = newBuffer; //Temporarily sets the new finish to the start of the vector
                for(size_t i = 0; i < size(); ++i){
                    alloc_traits::construct(allocator, newBuffer + i, std::move_if_noexcept(*(m_buffer + i))); //Properly adds elements to the new vector
                    ++newFinish;
                }
                std::swap(m_capacity, tmpCapacity);
                std::swap(m_buffer, newBuffer);
                std::swap(m_finish, newFinish);

                destroyObjects(newBuffer, newFinish); //Destroy the old vector after the new vector has been swapped
                alloc_traits::deallocate(allocator, newBuffer, tmpCapacity); //Free the old space
            }
            catch(...){ //If something went wrong while allocating more space, deallocate the newly created space and throw the exception.
                destroyObjects(newBuffer, newFinish);
                alloc_traits::deallocate(allocator, newBuffer, tmpCapacity);
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