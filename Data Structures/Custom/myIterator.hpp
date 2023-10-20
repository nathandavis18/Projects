#ifndef MYITER
#define MYITER
#include <iterator>
#include <cstddef>
namespace custom{
    template <typename T>
    class myIterator{
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type = std::ptrdiff_t;

        myIterator(T* p) : ptr(p) {}
        T& operator*() const { return *ptr; } //Dereference operator
        T* operator->() { return ptr; }
        myIterator& operator++() { ptr++; return *this; } //Increment operators
        myIterator operator++(int) { myIterator tmp = *this; ++(*this); return tmp; }

        friend auto operator<=>( const myIterator&, const myIterator& ) = default;
    private:
        T* ptr;
    };
}
#endif //MYITER