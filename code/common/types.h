#pragma once

#include <cstdint>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using S8 = int8_t;
using S16 = int16_t;
using S32 = int32_t;
using S64 = int64_t;
using B8 = U8;
using B16 = U16;
using B32 = U32;
using B64 = U64;
using F32 = float;
using F64 = double;

struct IVector2
{
    S32 x;
    S32 y;
};