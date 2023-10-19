#include <iterator>
#include <cstddef>
namespace custom{
    template <typename T>
    class myIterator{
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        myIterator(pointer p) : ptr(p) {}
        reference operator*() const { return *ptr; } //Dereference operator
        pointer operator->() { return ptr; }
        void operator=(pointer t) { ptr = t; } //current pointer points to what t is pointing to
        void operator=(reference t) { *ptr = t; } //Current value ptr is pointing to is given the value t
        myIterator& operator++() { ptr++; return *this; } //Increment operators
        myIterator operator++(value_type) { myIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const myIterator& a, const myIterator& b) { return a.ptr == b.ptr; }
        friend bool operator!= (const myIterator& a, const myIterator& b) { return a.ptr != b.ptr; }
    private:
        pointer ptr;
    };
}