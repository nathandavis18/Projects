#ifndef MYITER
#define MYITER
#include <iterator>
#include <cstddef>
namespace custom{
    template <typename T>
    class myIterator{
    public:
        using value_type = T;
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        myIterator(T* p) : ptr(p) {}
        T& operator*() const { return *ptr; } //Dereference operator
        T* operator->() { return ptr; }
        myIterator& operator++() { ptr++; return *this; } //Increment operators
        myIterator operator++(int) { myIterator tmp = *this; ++(*this); return tmp; }
        myIterator& operator--() { ptr--; return *this; } //Increment operators 
        myIterator operator--(int) { myIterator tmp = *this; --(*this); return tmp; }
        myIterator& operator+=(const difference_type m) { ptr += m; return *this; }
        myIterator& operator-=(const difference_type m) { ptr -= m; return *this; }
        


        friend myIterator operator-(const myIterator& it, const difference_type m) { myIterator tmp = it; tmp -= m; return tmp;  }
        friend myIterator operator+(const myIterator& it, const difference_type m) { myIterator tmp = it; tmp += m; return tmp; }
        friend myIterator operator-(const difference_type m, const myIterator& it) { return it - m; }
        friend myIterator operator+(const difference_type m, const myIterator& it) { return it + m; }
        friend difference_type operator-(const myIterator& a, const myIterator& b) { return (a.ptr - b.ptr); }
        friend auto operator<=>( const myIterator&, const myIterator& ) = default;
    private:
        T* ptr;
    };
}
#endif //MYITER