#ifndef MYREVERSEITER
#define MYREVERSEITER
#include <iterator>
#include <cstddef>
/**
 * This is a custom reverse iterator designed to work with my custom vector. Can be used as a standalone reverse iterator.
 * Iterators like this one are essentially wrappers around pointers to allow for quick operations, 
 * such as incrementing using ++, decrementing using --, and comparing iterators.
 * 
 * Difference between reverse iterator and normal iterator is reverse iterators are designed with moving from end->beginning
*/
namespace custom{
    template <typename T>
    class myReverseIterator{
    public:
        using value_type = T;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        myReverseIterator(T* p) : ptr(p) {}
        T& operator*() const { return *ptr; } //Dereference operator
        T* operator->() { return ptr; }
        myReverseIterator& operator++() { ptr--; return *this; } //Increment operators. Reverse iterator goes backwards when incremented
        myReverseIterator operator++(int) { myReverseIterator tmp = *this; --(*this); return tmp; }
        myReverseIterator& operator--() { ptr++; return *this; } //Decrement operators. Reverse iterator goes forwards when decremented
        myReverseIterator operator--(int) { myReverseIterator tmp = *this; ++(*this); return tmp; }
        myReverseIterator& operator+=(const difference_type m) { ptr -= m; return *this; }
        myReverseIterator& operator-=(const difference_type m) { ptr += m; return *this; }        

        constexpr myReverseIterator operator-(const difference_type m) const noexcept { return myReverseIterator(ptr + m); }
        constexpr myReverseIterator operator+(const difference_type m) const noexcept { return myReverseIterator(ptr - m); }
        friend auto operator-(const myReverseIterator& lhs, const myReverseIterator& rhs) { return (lhs.ptr + rhs.ptr); }
        friend auto operator<=>( const myReverseIterator&, const myReverseIterator& ) = default;
    private:
        T* ptr;
    };
}
#endif //MYITER