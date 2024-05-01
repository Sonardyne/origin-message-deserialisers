#ifndef RDIPD0INTERFACE_H
#define RDIPD0INTERFACE_H

#include <cstdint>

namespace sonardyne_edge
{

namespace PDN_messages
{

class RDIPD0Interface
{
public:
    
    virtual ~RDIPD0Interface() {}
    virtual RDIPD0Interface* clone() const = 0;
    virtual std::uint16_t    get_ID() const = 0;
    virtual std::size_t      calc_length() const = 0;
    virtual int              serialise(char* buf, const int bytes) const = 0;
    virtual int              deserialise(const char* buf, const int bytes) = 0;

};

} // close PDN_messages namespace

} // close sonardyne_edge namespace

#endif
