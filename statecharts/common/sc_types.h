/* Copyright (C) Steffen A. Mork */

#ifndef SC_TYPES_H_
#define SC_TYPES_H_

/* As <cstdint> is not available on all platforms we choose to include <stdint.h>. You can switch to <cstdint> by commenting and uncommenting the lines below.*/
// #include <cstdint>
#include <stdint.h>

#include <string>

namespace sc {
		
// typedef int_fast16_t   short;
typedef uint_fast16_t  ushort;
typedef int32_t        integer;
typedef int16_t		   errorCode;
typedef double         real;

typedef uint_fast16_t  eventid;

}

#endif /* SC_TYPES_H_ */
