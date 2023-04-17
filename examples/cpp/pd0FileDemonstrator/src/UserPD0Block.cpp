#include <string.h>

#include "UserPD0Block.h"

using namespace sonardyne_edge;

using namespace PDN_messages;

UserPD0Block::UserPD0Block() :
    _user_int(0),
    _user_uint(0), 
    _user_int64_t(0), 
    _user_float(0), 
    _user_double(0)
{
}

UserPD0Block::UserPD0Block(const UserPD0Block& orig) :
    _user_int(orig._user_int), 
    _user_uint(orig._user_uint), 
    _user_int64_t(orig._user_int64_t), 
    _user_float(orig._user_float), 
    _user_double(orig._user_double)
{
}

UserPD0Block::~UserPD0Block()
{
}

size_t UserPD0Block::calc_length() const
{
    return _length;
}

uint16_t UserPD0Block::get_ID() const
{
        return _id;
}

int UserPD0Block::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &_id, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &_user_int, sizeof(int));
        offset += sizeof(int);

        memcpy(buf + offset, &_user_uint, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &_user_int64_t, sizeof(int64_t));
        offset += sizeof(int64_t);

        memcpy(buf + offset, &_user_float, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &_user_double, sizeof(double));
        offset += sizeof(double);
        
        return offset;
}

int UserPD0Block::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        //memcpy(&_id, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&_user_int, buf + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&_user_uint, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&_user_int64_t, buf + offset, sizeof(int64_t));
        offset += sizeof(int64_t);

        memcpy(&_user_float, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&_user_double, buf + offset, sizeof(double));
        offset += sizeof(double);
        
       return offset;

}
