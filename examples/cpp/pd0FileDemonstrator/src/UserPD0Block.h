#ifndef USERPD0BLOCK_H
#define USERPD0BLOCK_H

#include "PDN_messages.h"

namespace sonardyne_edge
{

namespace PDN_messages
{
    
class UserPD0Block : public RDIPD0Interface
{
public:
    UserPD0Block();
    UserPD0Block(const UserPD0Block& orig);
    virtual ~UserPD0Block();

    UserPD0Block * clone() const { return new UserPD0Block(*this); };
    size_t calc_length() const;
    uint16_t get_ID() const;
    int serialise(char* buf, const int bytes) const;
    int deserialise(const char* buf, const int bytes);

private:

    const uint16_t _length = 30;
    const uint16_t _id     = 0x3333; // 2
    int            _user_int;        // 6
    uint32_t       _user_uint;       // 10
    int64_t        _user_int64_t;    // 18
    float          _user_float;      // 22
    double         _user_double;     // 30
    
};

} // close PDN_messages namespace

} // close sonardyne_edge namespace

#endif /* USERPD0BLOCK_H */

