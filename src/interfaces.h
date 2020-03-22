#ifndef INTERFACES_H
#define INTERFACES_H

#include <cstddef>

struct ISizable
{
    virtual ~ISizable() = default;
    virtual std::size_t size() const = 0;
};


#endif // INTERFACES_H
