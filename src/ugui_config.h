#pragma once

#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */


// A special color which means "do not draw", used to let fonts have transparent backgrounds (also to save the cost of rendering when we know the area is already blank)
#define C_TRANSPARENT 0xC1C2

#define USE_FONT_16X26

/* Specify platform-dependent integer types here */

#define __UG_FONT_DATA const
typedef uint8_t UG_U8;
typedef int8_t UG_S8;
typedef uint16_t UG_U16;
typedef int16_t UG_S16;
typedef uint32_t UG_U32;
typedef int32_t UG_S32;

/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */

/* Feature enablers */
//#define USE_PRERENDER_EVENT
//#define USE_POSTRENDER_EVENT

