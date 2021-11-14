#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <cstddef>

// compile time type size printer
#define CHECK_SIZE(type) char __checker(type);char __checkSize[sizeof(type)]={__checker(&__checkSize)};


template <typename T>
constexpr size_t type_size() {
    return sizeof(T);
}

template< size_t N >
constexpr size_t strLength( char const (&)[N] )
{
  return N;
}

#endif //COMMONFUNCTIONS_H