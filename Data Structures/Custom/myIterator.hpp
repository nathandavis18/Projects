#include <iterator>
namespace custom{
    template <typename T>
    class myIterator{
    public:
        using iteratorCategory = std::forward_iterator_tag;
        using diffType = std::ptrdiff_t;
        using valueType = T;
        using pointer = T*;
        using reference = T&;

        myIterator(pointer p) : ptr(p) {}
        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }
        void operator=(pointer t) { ptr = t; }
        myIterator& operator++() { ptr++; return *this; }
        myIterator operator++(T) { myIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const myIterator& a, const myIterator& b) { return a.ptr == b.ptr; }
        friend bool operator!= (const myIterator& a, const myIterator& b) { return a.ptr != b.ptr; }
    private:
        pointer ptr;
    };
}