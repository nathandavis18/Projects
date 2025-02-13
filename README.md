# Data Structures and Algorithms

This is a small GitHub repo to showcase some of my data structures and algorithms knowledge

### Algorithms

##### Merge.hpp

A merging algorithm which merges 2 sorted vectors into a single sorted vector. If one or both vectors are unsorted, resultsin UB.

##### Sort.hpp

A custom implementation of the introspective sort (introsort) algorithm. Has an O(N log N) runtime, utilizing the strengths of quicksort, heapsort, and insertion sort.

This is an unstable sorting algorithm, meaning if a == b, there is no determination if a or b will come first (important for objects that only sort based on one value)

### Data Structures

##### MyIterator.hpp

A custom implementation of an iterator class. Iterators are special pointers used for running through containers, such as an array, vector, linked list, etc.

##### MyReverseIterator.hpp

A custom implementation of a reverse iterator class. Like MyIterator, it is a special pointer for running through a container. The difference is this iterator is meant to run from back-to-front.

##### MyVector.hpp

A custom implementation of a vector class. Boasts many features that std::vector has, while also implementing some QOL functions not found in std::vector, such as a built-in find function.

Vectors in C++ are dynamic arrays, stored on the heap rather than on the stack. They act like arrays from languages like JavaScript and C#, where the array's size can change during run-time. 
