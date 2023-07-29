/*
MIT License

Copyright (c) 2019 DeanoC

Permission is hereby granted, afree of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "framework/01core/utils/utils.h"

namespace ante::gal {

enum class gal_texture_format {
    UNDEFINED = 0,
    R1_UNORM = 1,
    R2_UNORM = 2,
    R4_UNORM = 3,
    R4G4_UNORM = 4,
    G4R4_UNORM = 5,
    A8_UNORM = 6,
    R8_UNORM = 7,
    R8_SNORM = 8,
    R8_UINT = 9,
    R8_SINT = 10,
    R8_SRGB = 11,
    B2G3R3_UNORM = 12,
    R4G4B4A4_UNORM = 13,
    R4G4B4X4_UNORM = 14,
    B4G4R4A4_UNORM = 15,
    B4G4R4X4_UNORM = 16,
    A4R4G4B4_UNORM = 17,
    X4R4G4B4_UNORM = 18,
    A4B4G4R4_UNORM = 19,
    X4B4G4R4_UNORM = 20,
    R5G6B5_UNORM = 21,
    B5G6R5_UNORM = 22,
    R5G5B5A1_UNORM = 23,
    B5G5R5A1_UNORM = 24,
    A1B5G5R5_UNORM = 25,
    A1R5G5B5_UNORM = 26,
    R5G5B5X1_UNORM = 27,
    B5G5R5X1_UNORM = 28,
    X1R5G5B5_UNORM = 29,
    X1B5G5R5_UNORM = 30,
    B2G3R3A8_UNORM = 31,
    R8G8_UNORM = 32,
    R8G8_SNORM = 33,
    G8R8_UNORM = 34,
    G8R8_SNORM = 35,
    R8G8_UINT = 36,
    R8G8_SINT = 37,
    R8G8_SRGB = 38,
    R16_UNORM = 39,
    R16_SNORM = 40,
    R16_UINT = 41,
    R16_SINT = 42,
    R16_SFLOAT = 43,
    R16_SBFLOAT = 44,
    R8G8B8_UNORM = 45,
    R8G8B8_SNORM = 46,
    R8G8B8_UINT = 47,
    R8G8B8_SINT = 48,
    R8G8B8_SRGB = 49,
    B8G8R8_UNORM = 50,
    B8G8R8_SNORM = 51,
    B8G8R8_UINT = 52,
    B8G8R8_SINT = 53,
    B8G8R8_SRGB = 54,
    R8G8B8A8_UNORM = 55,
    R8G8B8A8_SNORM = 56,
    R8G8B8A8_UINT = 57,
    R8G8B8A8_SINT = 58,
    R8G8B8A8_SRGB = 59,
    B8G8R8A8_UNORM = 60,
    B8G8R8A8_SNORM = 61,
    B8G8R8A8_UINT = 62,
    B8G8R8A8_SINT = 63,
    B8G8R8A8_SRGB = 64,
    R8G8B8X8_UNORM = 65,
    B8G8R8X8_UNORM = 66,
    R16G16_UNORM = 67,
    G16R16_UNORM = 68,
    R16G16_SNORM = 69,
    G16R16_SNORM = 70,
    R16G16_UINT = 71,
    R16G16_SINT = 72,
    R16G16_SFLOAT = 73,
    R16G16_SBFLOAT = 74,
    R32_UINT = 75,
    R32_SINT = 76,
    R32_SFLOAT = 77,
    A2R10G10B10_UNORM = 78,
    A2R10G10B10_UINT = 79,
    A2R10G10B10_SNORM = 80,
    A2R10G10B10_SINT = 81,
    A2B10G10R10_UNORM = 82,
    A2B10G10R10_UINT = 83,
    A2B10G10R10_SNORM = 84,
    A2B10G10R10_SINT = 85,
    R10G10B10A2_UNORM = 86,
    R10G10B10A2_UINT = 87,
    R10G10B10A2_SNORM = 88,
    R10G10B10A2_SINT = 89,
    B10G10R10A2_UNORM = 90,
    B10G10R10A2_UINT = 91,
    B10G10R10A2_SNORM = 92,
    B10G10R10A2_SINT = 93,
    B10G11R11_UFLOAT = 94,
    E5B9G9R9_UFLOAT = 95,
    R16G16B16_UNORM = 96,
    R16G16B16_SNORM = 97,
    R16G16B16_UINT = 98,
    R16G16B16_SINT = 99,
    R16G16B16_SFLOAT = 100,
    R16G16B16_SBFLOAT = 101,
    R16G16B16A16_UNORM = 102,
    R16G16B16A16_SNORM = 103,
    R16G16B16A16_UINT = 104,
    R16G16B16A16_SINT = 105,
    R16G16B16A16_SFLOAT = 106,
    R16G16B16A16_SBFLOAT = 107,
    R32G32_UINT = 108,
    R32G32_SINT = 109,
    R32G32_SFLOAT = 110,
    R32G32B32_UINT = 111,
    R32G32B32_SINT = 112,
    R32G32B32_SFLOAT = 113,
    R32G32B32A32_UINT = 114,
    R32G32B32A32_SINT = 115,
    R32G32B32A32_SFLOAT = 116,
    R64_UINT = 117,
    R64_SINT = 118,
    R64_SFLOAT = 119,
    R64G64_UINT = 120,
    R64G64_SINT = 121,
    R64G64_SFLOAT = 122,
    R64G64B64_UINT = 123,
    R64G64B64_SINT = 124,
    R64G64B64_SFLOAT = 125,
    R64G64B64A64_UINT = 126,
    R64G64B64A64_SINT = 127,
    R64G64B64A64_SFLOAT = 128,
    D16_UNORM = 129,
    X8_D24_UNORM = 130,
    D32_SFLOAT = 131,
    S8_UINT = 132,
    D16_UNORM_S8_UINT = 133,
    D24_UNORM_S8_UINT = 134,
    D32_SFLOAT_S8_UINT = 135,
    DXBC1_RGB_UNORM = 136,
    DXBC1_RGB_SRGB = 137,
    DXBC1_RGBA_UNORM = 138,
    DXBC1_RGBA_SRGB = 139,
    DXBC2_UNORM = 140,
    DXBC2_SRGB = 141,
    DXBC3_UNORM = 142,
    DXBC3_SRGB = 143,
    DXBC4_UNORM = 144,
    DXBC4_SNORM = 145,
    DXBC5_UNORM = 146,
    DXBC5_SNORM = 147,
    DXBC6H_UFLOAT = 148,
    DXBC6H_SFLOAT = 149,
    DXBC7_UNORM = 150,
    DXBC7_SRGB = 151,
    PVRTC1_2BPP_UNORM = 152,
    PVRTC1_4BPP_UNORM = 153,
    PVRTC2_2BPP_UNORM = 154,
    PVRTC2_4BPP_UNORM = 155,
    PVRTC1_2BPP_SRGB = 156,
    PVRTC1_4BPP_SRGB = 157,
    PVRTC2_2BPP_SRGB = 158,
    PVRTC2_4BPP_SRGB = 159,
    ETC2_R8G8B8_UNORM = 160,
    ETC2_R8G8B8_SRGB = 161,
    ETC2_R8G8B8A1_UNORM = 162,
    ETC2_R8G8B8A1_SRGB = 163,
    ETC2_R8G8B8A8_UNORM = 164,
    ETC2_R8G8B8A8_SRGB = 165,
    ETC2_EAC_R11_UNORM = 166,
    ETC2_EAC_R11_SNORM = 167,
    ETC2_EAC_R11G11_UNORM = 168,
    ETC2_EAC_R11G11_SNORM = 169,
    ASTC_4x4_UNORM = 170,
    ASTC_4x4_SRGB = 171,
    ASTC_5x4_UNORM = 172,
    ASTC_5x4_SRGB = 173,
    ASTC_5x5_UNORM = 174,
    ASTC_5x5_SRGB = 175,
    ASTC_6x5_UNORM = 176,
    ASTC_6x5_SRGB = 177,
    ASTC_6x6_UNORM = 178,
    ASTC_6x6_SRGB = 179,
    ASTC_8x5_UNORM = 180,
    ASTC_8x5_SRGB = 181,
    ASTC_8x6_UNORM = 182,
    ASTC_8x6_SRGB = 183,
    ASTC_8x8_UNORM = 184,
    ASTC_8x8_SRGB = 185,
    ASTC_10x5_UNORM = 186,
    ASTC_10x5_SRGB = 187,
    ASTC_10x6_UNORM = 188,
    ASTC_10x6_SRGB = 189,
    ASTC_10x8_UNORM = 190,
    ASTC_10x8_SRGB = 191,
    ASTC_10x10_UNORM = 192,
    ASTC_10x10_SRGB = 193,
    ASTC_12x10_UNORM = 194,
    ASTC_12x10_SRGB = 195,
    ASTC_12x12_UNORM = 196,
    ASTC_12x12_SRGB = 197,
    CLUT_P4 = 198,
    CLUT_P4A4 = 199,
    CLUT_P8 = 200,
    CLUT_P8A8 = 201,
    R4G4B4A4_UNORM_PACK16 = 202,
    B4G4R4A4_UNORM_PACK16 = 203,
    R5G6B5_UNORM_PACK16 = 204,
    B5G6R5_UNORM_PACK16 = 205,
    R5G5B5A1_UNORM_PACK16 = 206,
    B5G5R5A1_UNORM_PACK16 = 207,
    A1R5G5B5_UNORM_PACK16 = 208,
    G16B16G16R16_422_UNORM = 209,
    B16G16R16G16_422_UNORM = 210,
    R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 211,
    G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 212,
    B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 213,
    R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 214,
    G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 215,
    B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 216,
    G8B8G8R8_422_UNORM = 217,
    B8G8R8G8_422_UNORM = 218,
    G8_B8_R8_3PLANE_420_UNORM = 219,
    G8_B8R8_2PLANE_420_UNORM = 220,
    G8_B8_R8_3PLANE_422_UNORM = 221,
    G8_B8R8_2PLANE_422_UNORM = 222,
    G8_B8_R8_3PLANE_444_UNORM = 223,
    G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 224,
    G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 225,
    G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 226,
    G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 227,
    G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 228,
    G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 229,
    G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 230,
    G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 231,
    G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 232,
    G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 233,
    G16_B16_R16_3PLANE_420_UNORM = 234,
    G16_B16_R16_3PLANE_422_UNORM = 235,
    G16_B16_R16_3PLANE_444_UNORM = 236,
    G16_B16R16_2PLANE_420_UNORM = 237,
    G16_B16R16_2PLANE_422_UNORM = 238,
};

constexpr u64 gal_tf_code(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 0x0000000000000000;
    case gal_texture_format::R1_UNORM:
        return 0x0000D9000000C000;
    case gal_texture_format::R2_UNORM:
        return 0x0000D90000013000;
    case gal_texture_format::R4_UNORM:
        return 0x0000D90000022000;
    case gal_texture_format::R4G4_UNORM:
        return 0x0008D84000421000;
    case gal_texture_format::G4R4_UNORM:
        return 0x0008D80800421000;
    case gal_texture_format::A8_UNORM:
        return 0x0000C91800040000;
    case gal_texture_format::R8_UNORM:
        return 0x0000D90000040000;
    case gal_texture_format::R8_SNORM:
        return 0x0001590000040000;
    case gal_texture_format::R8_UINT:
        return 0x0001D90000040000;
    case gal_texture_format::R8_SINT:
        return 0x0002590000040000;
    case gal_texture_format::R8_SRGB:
        return 0x0003D90000040000;
    case gal_texture_format::B2G3R3_UNORM:
        return 0x0088D05006311000;
    case gal_texture_format::R4G4B4A4_UNORM:
        return 0x0888B44108421000;
    case gal_texture_format::R4G4B4X4_UNORM:
        return 0x0888D44108421000;
    case gal_texture_format::B4G4R4A4_UNORM:
        return 0x0888B05108421000;
    case gal_texture_format::B4G4R4X4_UNORM:
        return 0x0888D05108421000;
    case gal_texture_format::A4R4G4B4_UNORM:
        return 0x0888A21908421000;
    case gal_texture_format::X4R4G4B4_UNORM:
        return 0x0889A22908421000;
    case gal_texture_format::A4B4G4R4_UNORM:
        return 0x0888829908421000;
    case gal_texture_format::X4B4G4R4_UNORM:
        return 0x088982A908421000;
    case gal_texture_format::R5G6B5_UNORM:
        return 0x0088D4400A629000;
    case gal_texture_format::B5G6R5_UNORM:
        return 0x0088D0500A629000;
    case gal_texture_format::R5G5B5A1_UNORM:
        return 0x0888B4404A529000;
    case gal_texture_format::B5G5R5A1_UNORM:
        return 0x0888B0504A529000;
    case gal_texture_format::A1B5G5R5_UNORM:
        return 0x088882994A509000;
    case gal_texture_format::A1R5G5B5_UNORM:
        return 0x0888A2194A509000;
    case gal_texture_format::R5G5B5X1_UNORM:
        return 0x1888D4404A529000;
    case gal_texture_format::B5G5R5X1_UNORM:
        return 0x1888D0504A529000;
    case gal_texture_format::X1R5G5B5_UNORM:
        return 0x0889A2294A509000;
    case gal_texture_format::X1B5G5R5_UNORM:
        return 0x088982A94A509000;
    case gal_texture_format::B2G3R3A8_UNORM:
        return 0x0888B05206311000;
    case gal_texture_format::R8G8_UNORM:
        return 0x0008D84000840000;
    case gal_texture_format::R8G8_SNORM:
        return 0x0011584000840000;
    case gal_texture_format::G8R8_UNORM:
        return 0x0008D80800840000;
    case gal_texture_format::G8R8_SNORM:
        return 0x0011580800840000;
    case gal_texture_format::R8G8_UINT:
        return 0x0019D84000840000;
    case gal_texture_format::R8G8_SINT:
        return 0x0022584000840000;
    case gal_texture_format::R8G8_SRGB:
        return 0x003BD84000840000;
    case gal_texture_format::R16_UNORM:
        return 0x0000D90000068000;
    case gal_texture_format::R16_SNORM:
        return 0x0001590000068000;
    case gal_texture_format::R16_UINT:
        return 0x0001D90000068000;
    case gal_texture_format::R16_SINT:
        return 0x0002590000068000;
    case gal_texture_format::R16_SFLOAT:
        return 0x0003590000068000;
    case gal_texture_format::R16_SBFLOAT:
        return 0x0004590000068000;
    case gal_texture_format::R8G8B8_UNORM:
        return 0x0088D44010840000;
    case gal_texture_format::R8G8B8_SNORM:
        return 0x0111544010840000;
    case gal_texture_format::R8G8B8_UINT:
        return 0x0199D44010840000;
    case gal_texture_format::R8G8B8_SINT:
        return 0x0222544010840000;
    case gal_texture_format::R8G8B8_SRGB:
        return 0x03BBD44010840000;
    case gal_texture_format::B8G8R8_UNORM:
        return 0x0088D05010840000;
    case gal_texture_format::B8G8R8_SNORM:
        return 0x0111505010840000;
    case gal_texture_format::B8G8R8_UINT:
        return 0x0199D05010840000;
    case gal_texture_format::B8G8R8_SINT:
        return 0x0222505010840000;
    case gal_texture_format::B8G8R8_SRGB:
        return 0x03BBD05010840000;
    case gal_texture_format::R8G8B8A8_UNORM:
        return 0x0888B44210840000;
    case gal_texture_format::R8G8B8A8_SNORM:
        return 0x1111344210840000;
    case gal_texture_format::R8G8B8A8_UINT:
        return 0x1999B44210840000;
    case gal_texture_format::R8G8B8A8_SINT:
        return 0x2222344210840000;
    case gal_texture_format::R8G8B8A8_SRGB:
        return 0x0BBBB44210840000;
    case gal_texture_format::B8G8R8A8_UNORM:
        return 0x0888B05210840000;
    case gal_texture_format::B8G8R8A8_SNORM:
        return 0x1111305210840000;
    case gal_texture_format::B8G8R8A8_UINT:
        return 0x1999B05210840000;
    case gal_texture_format::B8G8R8A8_SINT:
        return 0x2222305210840000;
    case gal_texture_format::B8G8R8A8_SRGB:
        return 0x0BBBB05210840000;
    case gal_texture_format::R8G8B8X8_UNORM:
        return 0x0888D44210840000;
    case gal_texture_format::B8G8R8X8_UNORM:
        return 0x0888D05210840000;
    case gal_texture_format::R16G16_UNORM:
        return 0x0008D84000D68000;
    case gal_texture_format::G16R16_UNORM:
        return 0x0008D80800D68000;
    case gal_texture_format::R16G16_SNORM:
        return 0x0011584000D68000;
    case gal_texture_format::G16R16_SNORM:
        return 0x0011580800D68000;
    case gal_texture_format::R16G16_UINT:
        return 0x0019D84000D68000;
    case gal_texture_format::R16G16_SINT:
        return 0x0022584000D68000;
    case gal_texture_format::R16G16_SFLOAT:
        return 0x0033584000D68000;
    case gal_texture_format::R16G16_SBFLOAT:
        return 0x0044584000D68000;
    case gal_texture_format::R32_UINT:
        return 0x0001D90000078000;
    case gal_texture_format::R32_SINT:
        return 0x0002590000078000;
    case gal_texture_format::R32_SFLOAT:
        return 0x0003590000078000;
    case gal_texture_format::A2R10G10B10_UNORM:
        return 0x0888A21A94A11000;
    case gal_texture_format::A2R10G10B10_UINT:
        return 0x1999A21A94A11000;
    case gal_texture_format::A2R10G10B10_SNORM:
        return 0x1111221A94A11000;
    case gal_texture_format::A2R10G10B10_SINT:
        return 0x2222221A94A11000;
    case gal_texture_format::A2B10G10R10_UNORM:
        return 0x0888829A94A11000;
    case gal_texture_format::A2B10G10R10_UINT:
        return 0x1999829A94A11000;
    case gal_texture_format::A2B10G10R10_SNORM:
        return 0x1111029A94A11000;
    case gal_texture_format::A2B10G10R10_SINT:
        return 0x2222029A94A11000;
    case gal_texture_format::R10G10B10A2_UNORM:
        return 0x0888B44094A51000;
    case gal_texture_format::R10G10B10A2_UINT:
        return 0x1999B44094A51000;
    case gal_texture_format::R10G10B10A2_SNORM:
        return 0x1111344094A51000;
    case gal_texture_format::R10G10B10A2_SINT:
        return 0x2222344094A51000;
    case gal_texture_format::B10G10R10A2_UNORM:
        return 0x0888B05094A51000;
    case gal_texture_format::B10G10R10A2_UINT:
        return 0x1999B05094A51000;
    case gal_texture_format::B10G10R10A2_SNORM:
        return 0x1111305094A51000;
    case gal_texture_format::B10G10R10A2_SINT:
        return 0x2222305094A51000;
    case gal_texture_format::B10G11R11_UFLOAT:
        return 0x02AAD05016B51000;
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return 0x2AA9829A52929000;
    case gal_texture_format::R16G16B16_UNORM:
        return 0x0088D4401AD68000;
    case gal_texture_format::R16G16B16_SNORM:
        return 0x011154401AD68000;
    case gal_texture_format::R16G16B16_UINT:
        return 0x0199D4401AD68000;
    case gal_texture_format::R16G16B16_SINT:
        return 0x022254401AD68000;
    case gal_texture_format::R16G16B16_SFLOAT:
        return 0x033354401AD68000;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return 0x044454401AD68000;
    case gal_texture_format::R16G16B16A16_UNORM:
        return 0x0888B4435AD68000;
    case gal_texture_format::R16G16B16A16_SNORM:
        return 0x111134435AD68000;
    case gal_texture_format::R16G16B16A16_UINT:
        return 0x1999B4435AD68000;
    case gal_texture_format::R16G16B16A16_SINT:
        return 0x222234435AD68000;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return 0x333334435AD68000;
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return 0x444434435AD68000;
    case gal_texture_format::R32G32_UINT:
        return 0x0019D84000F78000;
    case gal_texture_format::R32G32_SINT:
        return 0x0022584000F78000;
    case gal_texture_format::R32G32_SFLOAT:
        return 0x0033584000F78000;
    case gal_texture_format::R32G32B32_UINT:
        return 0x0199D4401EF78000;
    case gal_texture_format::R32G32B32_SINT:
        return 0x022254401EF78000;
    case gal_texture_format::R32G32B32_SFLOAT:
        return 0x033354401EF78000;
    case gal_texture_format::R32G32B32A32_UINT:
        return 0x1999B443DEF78000;
    case gal_texture_format::R32G32B32A32_SINT:
        return 0x22223443DEF78000;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return 0x33333443DEF78000;
    case gal_texture_format::R64_UINT:
        return 0x0001D90000080000;
    case gal_texture_format::R64_SINT:
        return 0x0002590000080000;
    case gal_texture_format::R64_SFLOAT:
        return 0x0003590000080000;
    case gal_texture_format::R64G64_UINT:
        return 0x0019D84001080000;
    case gal_texture_format::R64G64_SINT:
        return 0x0022584001080000;
    case gal_texture_format::R64G64_SFLOAT:
        return 0x0033584001080000;
    case gal_texture_format::R64G64B64_UINT:
        return 0x0199D44021080000;
    case gal_texture_format::R64G64B64_SINT:
        return 0x0222544021080000;
    case gal_texture_format::R64G64B64_SFLOAT:
        return 0x0333544021080000;
    case gal_texture_format::R64G64B64A64_UINT:
        return 0x1999B44421080000;
    case gal_texture_format::R64G64B64A64_SINT:
        return 0x2222344421080000;
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return 0x3333344421080000;
    case gal_texture_format::D16_UNORM:
        return 0x0000000001809001;
    case gal_texture_format::X8_D24_UNORM:
        return 0x0000000006266001;
    case gal_texture_format::D32_SFLOAT:
        return 0x0000000003812001;
    case gal_texture_format::S8_UINT:
        return 0x0000000002904001;
    case gal_texture_format::D16_UNORM_S8_UINT:
        return 0x000000000942A001;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return 0x000000000942E001;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return 0x000000000B433001;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return 0x0000000000100002;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return 0x0000000000108002;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return 0x0000000000181002;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return 0x0000000000189002;
    case gal_texture_format::DXBC2_UNORM:
        return 0x00000000001A3002;
    case gal_texture_format::DXBC2_SRGB:
        return 0x00000000001AB002;
    case gal_texture_format::DXBC3_UNORM:
        return 0x00000000001A2002;
    case gal_texture_format::DXBC3_SRGB:
        return 0x00000000001AA002;
    case gal_texture_format::DXBC4_UNORM:
        return 0x0000000000000002;
    case gal_texture_format::DXBC4_SNORM:
        return 0x0000000000004002;
    case gal_texture_format::DXBC5_UNORM:
        return 0x00000000000A0002;
    case gal_texture_format::DXBC5_SNORM:
        return 0x00000000000A4002;
    case gal_texture_format::DXBC6H_UFLOAT:
        return 0x0000000000530002;
    case gal_texture_format::DXBC6H_SFLOAT:
        return 0x000000000052C002;
    case gal_texture_format::DXBC7_UNORM:
        return 0x00000000003A3002;
    case gal_texture_format::DXBC7_SRGB:
        return 0x00000000003AB002;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return 0x0000000000000003;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return 0x0000000000004003;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return 0x0000000000001003;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return 0x0000000000005003;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return 0x0000000000010003;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return 0x0000000000014003;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return 0x0000000000011003;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return 0x0000000000015003;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return 0x0000000000080004;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return 0x00000000000A0004;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return 0x00000000000C4004;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return 0x00000000000E4004;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return 0x00000000000C8004;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return 0x00000000000E8004;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return 0x0000000000001004;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return 0x0000000000011004;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return 0x0000000000041004;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return 0x0000000000051004;
    case gal_texture_format::ASTC_4x4_UNORM:
        return 0x0000000000012005;
    case gal_texture_format::ASTC_4x4_SRGB:
        return 0x0000000000212005;
    case gal_texture_format::ASTC_5x4_UNORM:
        return 0x0000000000013005;
    case gal_texture_format::ASTC_5x4_SRGB:
        return 0x0000000000213005;
    case gal_texture_format::ASTC_5x5_UNORM:
        return 0x000000000001B005;
    case gal_texture_format::ASTC_5x5_SRGB:
        return 0x000000000021B005;
    case gal_texture_format::ASTC_6x5_UNORM:
        return 0x000000000001C005;
    case gal_texture_format::ASTC_6x5_SRGB:
        return 0x000000000021C005;
    case gal_texture_format::ASTC_6x6_UNORM:
        return 0x0000000000024005;
    case gal_texture_format::ASTC_6x6_SRGB:
        return 0x0000000000224005;
    case gal_texture_format::ASTC_8x5_UNORM:
        return 0x000000000001D005;
    case gal_texture_format::ASTC_8x5_SRGB:
        return 0x000000000021D005;
    case gal_texture_format::ASTC_8x6_UNORM:
        return 0x0000000000025005;
    case gal_texture_format::ASTC_8x6_SRGB:
        return 0x0000000000225005;
    case gal_texture_format::ASTC_8x8_UNORM:
        return 0x000000000002D005;
    case gal_texture_format::ASTC_8x8_SRGB:
        return 0x000000000022D005;
    case gal_texture_format::ASTC_10x5_UNORM:
        return 0x000000000001E005;
    case gal_texture_format::ASTC_10x5_SRGB:
        return 0x000000000021E005;
    case gal_texture_format::ASTC_10x6_UNORM:
        return 0x0000000000026005;
    case gal_texture_format::ASTC_10x6_SRGB:
        return 0x0000000000226005;
    case gal_texture_format::ASTC_10x8_UNORM:
        return 0x000000000002E005;
    case gal_texture_format::ASTC_10x8_SRGB:
        return 0x000000000022E005;
    case gal_texture_format::ASTC_10x10_UNORM:
        return 0x0000000000036005;
    case gal_texture_format::ASTC_10x10_SRGB:
        return 0x0000000000236005;
    case gal_texture_format::ASTC_12x10_UNORM:
        return 0x0000000000037005;
    case gal_texture_format::ASTC_12x10_SRGB:
        return 0x0000000000237005;
    case gal_texture_format::ASTC_12x12_UNORM:
        return 0x000000000003F005;
    case gal_texture_format::ASTC_12x12_SRGB:
        return 0x000000000023F005;
    case gal_texture_format::CLUT_P4:
        return 0x000000000010D006;
    case gal_texture_format::CLUT_P4A4:
        return 0x0000000000D6C006;
    case gal_texture_format::CLUT_P8:
        return 0x0000000000110006;
    case gal_texture_format::CLUT_P8A8:
        return 0x0000000000D90006;
    default:
        return 0ULL;
    }
}

constexpr u64 MaxPixelCountOfBlock = 144u;

constexpr bool gal_tf_is_depth_only(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::D16_UNORM:
    case gal_texture_format::X8_D24_UNORM:
    case gal_texture_format::D32_SFLOAT:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_stencil_only(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::S8_UINT:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_depth_and_stencil(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::D16_UNORM_S8_UINT:
        return true;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return true;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_has_depth(gal_texture_format fmt) { return gal_tf_is_depth_only(fmt) || gal_tf_is_depth_and_stencil(fmt); }

constexpr bool gal_tf_has_stencil(gal_texture_format fmt) {
    return gal_tf_is_stencil_only(fmt) || gal_tf_is_depth_and_stencil(fmt);
}

constexpr bool gal_tf_has_depth_or_stencil(gal_texture_format fmt) {
    return gal_tf_is_depth_only(fmt) || gal_tf_is_stencil_only(fmt) || gal_tf_is_depth_and_stencil(fmt);
}

constexpr bool gal_tf_is_clut(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::CLUT_P4:
        return true;
    case gal_texture_format::CLUT_P4A4:
        return true;
    case gal_texture_format::CLUT_P8:
        return true;
    case gal_texture_format::CLUT_P8A8:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_float(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R16_SFLOAT:
        return true;
    case gal_texture_format::R16_SBFLOAT:
        return true;
    case gal_texture_format::R16G16_SFLOAT:
        return true;
    case gal_texture_format::R16G16_SBFLOAT:
        return true;
    case gal_texture_format::R32_SFLOAT:
        return true;
    case gal_texture_format::B10G11R11_UFLOAT:
        return true;
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return true;
    case gal_texture_format::R16G16B16_SFLOAT:
        return true;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return true;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return true;
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return true;
    case gal_texture_format::R32G32_SFLOAT:
        return true;
    case gal_texture_format::R32G32B32_SFLOAT:
        return true;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return true;
    case gal_texture_format::R64_SFLOAT:
        return true;
    case gal_texture_format::R64G64_SFLOAT:
        return true;
    case gal_texture_format::R64G64B64_SFLOAT:
        return true;
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return true;
    case gal_texture_format::D32_SFLOAT:
        return true;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return true;
    case gal_texture_format::DXBC6H_UFLOAT:
        return true;
    case gal_texture_format::DXBC6H_SFLOAT:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_normalized(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R1_UNORM:
        return true;
    case gal_texture_format::R2_UNORM:
        return true;
    case gal_texture_format::R4_UNORM:
        return true;
    case gal_texture_format::R4G4_UNORM:
        return true;
    case gal_texture_format::G4R4_UNORM:
        return true;
    case gal_texture_format::A8_UNORM:
        return true;
    case gal_texture_format::R8_UNORM:
        return true;
    case gal_texture_format::R8_SNORM:
        return true;
    case gal_texture_format::R8_SRGB:
        return true;
    case gal_texture_format::B2G3R3_UNORM:
        return true;
    case gal_texture_format::R4G4B4A4_UNORM:
        return true;
    case gal_texture_format::R4G4B4X4_UNORM:
        return true;
    case gal_texture_format::B4G4R4A4_UNORM:
        return true;
    case gal_texture_format::B4G4R4X4_UNORM:
        return true;
    case gal_texture_format::A4R4G4B4_UNORM:
        return true;
    case gal_texture_format::X4R4G4B4_UNORM:
        return true;
    case gal_texture_format::A4B4G4R4_UNORM:
        return true;
    case gal_texture_format::X4B4G4R4_UNORM:
        return true;
    case gal_texture_format::R5G6B5_UNORM:
        return true;
    case gal_texture_format::B5G6R5_UNORM:
        return true;
    case gal_texture_format::R5G5B5A1_UNORM:
        return true;
    case gal_texture_format::B5G5R5A1_UNORM:
        return true;
    case gal_texture_format::A1B5G5R5_UNORM:
        return true;
    case gal_texture_format::A1R5G5B5_UNORM:
        return true;
    case gal_texture_format::R5G5B5X1_UNORM:
        return true;
    case gal_texture_format::B5G5R5X1_UNORM:
        return true;
    case gal_texture_format::X1R5G5B5_UNORM:
        return true;
    case gal_texture_format::X1B5G5R5_UNORM:
        return true;
    case gal_texture_format::B2G3R3A8_UNORM:
        return true;
    case gal_texture_format::R8G8_UNORM:
        return true;
    case gal_texture_format::R8G8_SNORM:
        return true;
    case gal_texture_format::G8R8_UNORM:
        return true;
    case gal_texture_format::G8R8_SNORM:
        return true;
    case gal_texture_format::R8G8_SRGB:
        return true;
    case gal_texture_format::R16_UNORM:
        return true;
    case gal_texture_format::R16_SNORM:
        return true;
    case gal_texture_format::R8G8B8_UNORM:
        return true;
    case gal_texture_format::R8G8B8_SNORM:
        return true;
    case gal_texture_format::R8G8B8_SRGB:
        return true;
    case gal_texture_format::B8G8R8_UNORM:
        return true;
    case gal_texture_format::B8G8R8_SNORM:
        return true;
    case gal_texture_format::B8G8R8_SRGB:
        return true;
    case gal_texture_format::R8G8B8A8_UNORM:
        return true;
    case gal_texture_format::R8G8B8A8_SNORM:
        return true;
    case gal_texture_format::R8G8B8A8_SRGB:
        return true;
    case gal_texture_format::B8G8R8A8_UNORM:
        return true;
    case gal_texture_format::B8G8R8A8_SNORM:
        return true;
    case gal_texture_format::B8G8R8A8_SRGB:
        return true;
    case gal_texture_format::R8G8B8X8_UNORM:
        return true;
    case gal_texture_format::B8G8R8X8_UNORM:
        return true;
    case gal_texture_format::R16G16_UNORM:
        return true;
    case gal_texture_format::G16R16_UNORM:
        return true;
    case gal_texture_format::R16G16_SNORM:
        return true;
    case gal_texture_format::G16R16_SNORM:
        return true;
    case gal_texture_format::A2R10G10B10_UNORM:
        return true;
    case gal_texture_format::A2R10G10B10_SNORM:
        return true;
    case gal_texture_format::A2B10G10R10_UNORM:
        return true;
    case gal_texture_format::A2B10G10R10_SNORM:
        return true;
    case gal_texture_format::R10G10B10A2_UNORM:
        return true;
    case gal_texture_format::R10G10B10A2_SNORM:
        return true;
    case gal_texture_format::B10G10R10A2_UNORM:
        return true;
    case gal_texture_format::B10G10R10A2_SNORM:
        return true;
    case gal_texture_format::R16G16B16_UNORM:
        return true;
    case gal_texture_format::R16G16B16_SNORM:
        return true;
    case gal_texture_format::R16G16B16A16_UNORM:
        return true;
    case gal_texture_format::R16G16B16A16_SNORM:
        return true;
    case gal_texture_format::D16_UNORM:
        return true;
    case gal_texture_format::X8_D24_UNORM:
        return true;
    case gal_texture_format::D16_UNORM_S8_UINT:
        return true;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return true;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return true;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return true;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return true;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return true;
    case gal_texture_format::DXBC2_UNORM:
        return true;
    case gal_texture_format::DXBC2_SRGB:
        return true;
    case gal_texture_format::DXBC3_UNORM:
        return true;
    case gal_texture_format::DXBC3_SRGB:
        return true;
    case gal_texture_format::DXBC4_UNORM:
        return true;
    case gal_texture_format::DXBC4_SNORM:
        return true;
    case gal_texture_format::DXBC5_UNORM:
        return true;
    case gal_texture_format::DXBC5_SNORM:
        return true;
    case gal_texture_format::DXBC7_UNORM:
        return true;
    case gal_texture_format::DXBC7_SRGB:
        return true;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return true;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return true;
    case gal_texture_format::ASTC_4x4_UNORM:
        return true;
    case gal_texture_format::ASTC_4x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x4_UNORM:
        return true;
    case gal_texture_format::ASTC_5x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x5_UNORM:
        return true;
    case gal_texture_format::ASTC_5x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x5_UNORM:
        return true;
    case gal_texture_format::ASTC_6x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x6_UNORM:
        return true;
    case gal_texture_format::ASTC_6x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x5_UNORM:
        return true;
    case gal_texture_format::ASTC_8x5_SRGB:
        return true;
    case gal_texture_format::ASTC_8x6_UNORM:
        return true;
    case gal_texture_format::ASTC_8x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x8_UNORM:
        return true;
    case gal_texture_format::ASTC_8x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x5_UNORM:
        return true;
    case gal_texture_format::ASTC_10x5_SRGB:
        return true;
    case gal_texture_format::ASTC_10x6_UNORM:
        return true;
    case gal_texture_format::ASTC_10x6_SRGB:
        return true;
    case gal_texture_format::ASTC_10x8_UNORM:
        return true;
    case gal_texture_format::ASTC_10x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x10_UNORM:
        return true;
    case gal_texture_format::ASTC_10x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x10_UNORM:
        return true;
    case gal_texture_format::ASTC_12x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x12_UNORM:
        return true;
    case gal_texture_format::ASTC_12x12_SRGB:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_signed(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R8_SNORM:
        return true;
    case gal_texture_format::R8_SINT:
        return true;
    case gal_texture_format::R8G8_SNORM:
        return true;
    case gal_texture_format::G8R8_SNORM:
        return true;
    case gal_texture_format::R8G8_SINT:
        return true;
    case gal_texture_format::R16_SNORM:
        return true;
    case gal_texture_format::R16_SINT:
        return true;
    case gal_texture_format::R16_SFLOAT:
        return true;
    case gal_texture_format::R16_SBFLOAT:
        return true;
    case gal_texture_format::R8G8B8_SNORM:
        return true;
    case gal_texture_format::R8G8B8_SINT:
        return true;
    case gal_texture_format::B8G8R8_SNORM:
        return true;
    case gal_texture_format::B8G8R8_SINT:
        return true;
    case gal_texture_format::R8G8B8A8_SNORM:
        return true;
    case gal_texture_format::R8G8B8A8_SINT:
        return true;
    case gal_texture_format::B8G8R8A8_SNORM:
        return true;
    case gal_texture_format::B8G8R8A8_SINT:
        return true;
    case gal_texture_format::R16G16_SNORM:
        return true;
    case gal_texture_format::G16R16_SNORM:
        return true;
    case gal_texture_format::R16G16_SINT:
        return true;
    case gal_texture_format::R16G16_SFLOAT:
        return true;
    case gal_texture_format::R16G16_SBFLOAT:
        return true;
    case gal_texture_format::R32_SINT:
        return true;
    case gal_texture_format::R32_SFLOAT:
        return true;
    case gal_texture_format::A2R10G10B10_SNORM:
        return true;
    case gal_texture_format::A2R10G10B10_SINT:
        return true;
    case gal_texture_format::A2B10G10R10_SNORM:
        return true;
    case gal_texture_format::A2B10G10R10_SINT:
        return true;
    case gal_texture_format::R10G10B10A2_SNORM:
        return true;
    case gal_texture_format::R10G10B10A2_SINT:
        return true;
    case gal_texture_format::B10G10R10A2_SNORM:
        return true;
    case gal_texture_format::B10G10R10A2_SINT:
        return true;
    case gal_texture_format::R16G16B16_SNORM:
        return true;
    case gal_texture_format::R16G16B16_SINT:
        return true;
    case gal_texture_format::R16G16B16_SFLOAT:
        return true;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return true;
    case gal_texture_format::R16G16B16A16_SNORM:
        return true;
    case gal_texture_format::R16G16B16A16_SINT:
        return true;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return true;
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return true;
    case gal_texture_format::R32G32_SINT:
        return true;
    case gal_texture_format::R32G32_SFLOAT:
        return true;
    case gal_texture_format::R32G32B32_SINT:
        return true;
    case gal_texture_format::R32G32B32_SFLOAT:
        return true;
    case gal_texture_format::R32G32B32A32_SINT:
        return true;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return true;
    case gal_texture_format::R64_SINT:
        return true;
    case gal_texture_format::R64_SFLOAT:
        return true;
    case gal_texture_format::R64G64_SINT:
        return true;
    case gal_texture_format::R64G64_SFLOAT:
        return true;
    case gal_texture_format::R64G64B64_SINT:
        return true;
    case gal_texture_format::R64G64B64_SFLOAT:
        return true;
    case gal_texture_format::R64G64B64A64_SINT:
        return true;
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return true;
    case gal_texture_format::D32_SFLOAT:
        return true;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return true;
    case gal_texture_format::DXBC4_SNORM:
        return true;
    case gal_texture_format::DXBC5_SNORM:
        return true;
    case gal_texture_format::DXBC6H_SFLOAT:
        return true;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_srgb(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R8_SRGB:
        return true;
    case gal_texture_format::R8G8_SRGB:
        return true;
    case gal_texture_format::R8G8B8_SRGB:
        return true;
    case gal_texture_format::B8G8R8_SRGB:
        return true;
    case gal_texture_format::R8G8B8A8_SRGB:
        return true;
    case gal_texture_format::B8G8R8A8_SRGB:
        return true;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return true;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return true;
    case gal_texture_format::DXBC2_SRGB:
        return true;
    case gal_texture_format::DXBC3_SRGB:
        return true;
    case gal_texture_format::DXBC7_SRGB:
        return true;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return true;
    case gal_texture_format::ASTC_4x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x5_SRGB:
        return true;
    case gal_texture_format::ASTC_8x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x5_SRGB:
        return true;
    case gal_texture_format::ASTC_10x6_SRGB:
        return true;
    case gal_texture_format::ASTC_10x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x12_SRGB:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_ycbcr(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::G8B8G8R8_422_UNORM:
    case gal_texture_format::B8G8R8G8_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
    case gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
    case gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
    case gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
    case gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
    case gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G16B16G16R16_422_UNORM:
    case gal_texture_format::B16G16R16G16_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
        return true;

    default:
        return false;
    }
}

constexpr bool gal_tf_is_compressed(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::DXBC1_RGB_UNORM:
        return true;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return true;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return true;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return true;
    case gal_texture_format::DXBC2_UNORM:
        return true;
    case gal_texture_format::DXBC2_SRGB:
        return true;
    case gal_texture_format::DXBC3_UNORM:
        return true;
    case gal_texture_format::DXBC3_SRGB:
        return true;
    case gal_texture_format::DXBC4_UNORM:
        return true;
    case gal_texture_format::DXBC4_SNORM:
        return true;
    case gal_texture_format::DXBC5_UNORM:
        return true;
    case gal_texture_format::DXBC5_SNORM:
        return true;
    case gal_texture_format::DXBC6H_UFLOAT:
        return true;
    case gal_texture_format::DXBC6H_SFLOAT:
        return true;
    case gal_texture_format::DXBC7_UNORM:
        return true;
    case gal_texture_format::DXBC7_SRGB:
        return true;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return true;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return true;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return true;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return true;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return true;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return true;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return true;
    case gal_texture_format::ASTC_4x4_UNORM:
        return true;
    case gal_texture_format::ASTC_4x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x4_UNORM:
        return true;
    case gal_texture_format::ASTC_5x4_SRGB:
        return true;
    case gal_texture_format::ASTC_5x5_UNORM:
        return true;
    case gal_texture_format::ASTC_5x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x5_UNORM:
        return true;
    case gal_texture_format::ASTC_6x5_SRGB:
        return true;
    case gal_texture_format::ASTC_6x6_UNORM:
        return true;
    case gal_texture_format::ASTC_6x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x5_UNORM:
        return true;
    case gal_texture_format::ASTC_8x5_SRGB:
        return true;
    case gal_texture_format::ASTC_8x6_UNORM:
        return true;
    case gal_texture_format::ASTC_8x6_SRGB:
        return true;
    case gal_texture_format::ASTC_8x8_UNORM:
        return true;
    case gal_texture_format::ASTC_8x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x5_UNORM:
        return true;
    case gal_texture_format::ASTC_10x5_SRGB:
        return true;
    case gal_texture_format::ASTC_10x6_UNORM:
        return true;
    case gal_texture_format::ASTC_10x6_SRGB:
        return true;
    case gal_texture_format::ASTC_10x8_UNORM:
        return true;
    case gal_texture_format::ASTC_10x8_SRGB:
        return true;
    case gal_texture_format::ASTC_10x10_UNORM:
        return true;
    case gal_texture_format::ASTC_10x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x10_UNORM:
        return true;
    case gal_texture_format::ASTC_12x10_SRGB:
        return true;
    case gal_texture_format::ASTC_12x12_UNORM:
        return true;
    case gal_texture_format::ASTC_12x12_SRGB:
        return true;
    default:
        return false;
    }
}

constexpr bool gal_tf_is_planer(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
        return true;

    default:
        return false;
    }
}

constexpr bool gal_tf_is_packed(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::G8B8G8R8_422_UNORM:
    case gal_texture_format::B8G8R8G8_422_UNORM:
    case gal_texture_format::G16B16G16R16_422_UNORM:
    case gal_texture_format::B16G16R16G16_422_UNORM:
    case gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
    case gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
    case gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
    case gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
    case gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
    case gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
        return true;

    default:
        return false;
    }
}

constexpr u32 gal_tf_num_of_planes(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
        return 3;

    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
        return 2;

    default:
        return 1;
    }
}

constexpr u32 gal_tf_plane_width(gal_texture_format fmt, uint32_t plane, uint32_t width) {
    if (0 == plane) {
        return width;
}

    switch (fmt) {
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
        return width >> 1;

    default:
        return width;
    }
}

constexpr u32 gal_tf_plane_height(gal_texture_format fmt, uint32_t plane, uint32_t height) {
    if (0 == plane)
        return height;

    switch (fmt) {
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
        return height >> 1;

    default:
        return height;
    }
}

constexpr u32 gal_tf_plane_size_of_block(gal_texture_format fmt, uint32_t plane) {
    switch (fmt) {
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
        return 1;

    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
        return 2;

    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
        return 0 == plane ? 1 : 2;

    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
        return 0 == plane ? 2 : 4;

    default:
        return 1;
    }
}

constexpr u32 gal_tf_is_single_plane(gal_texture_format fmt) {
    return !gal_tf_is_planer(fmt) || gal_tf_num_of_planes(fmt) < 2;
}

constexpr bool gal_tf_is_homogenous(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return false;
    case gal_texture_format::B2G3R3_UNORM:
        return false;
    case gal_texture_format::X4R4G4B4_UNORM:
        return false;
    case gal_texture_format::X4B4G4R4_UNORM:
        return false;
    case gal_texture_format::R5G6B5_UNORM:
        return false;
    case gal_texture_format::B5G6R5_UNORM:
        return false;
    case gal_texture_format::R5G5B5A1_UNORM:
        return false;
    case gal_texture_format::B5G5R5A1_UNORM:
        return false;
    case gal_texture_format::A1B5G5R5_UNORM:
        return false;
    case gal_texture_format::A1R5G5B5_UNORM:
        return false;
    case gal_texture_format::R5G5B5X1_UNORM:
        return false;
    case gal_texture_format::B5G5R5X1_UNORM:
        return false;
    case gal_texture_format::X1R5G5B5_UNORM:
        return false;
    case gal_texture_format::X1B5G5R5_UNORM:
        return false;
    case gal_texture_format::B2G3R3A8_UNORM:
        return false;
    case gal_texture_format::R8G8B8A8_SRGB:
        return false;
    case gal_texture_format::B8G8R8A8_SRGB:
        return false;
    case gal_texture_format::A2R10G10B10_UNORM:
        return false;
    case gal_texture_format::A2R10G10B10_UINT:
        return false;
    case gal_texture_format::A2R10G10B10_SNORM:
        return false;
    case gal_texture_format::A2R10G10B10_SINT:
        return false;
    case gal_texture_format::A2B10G10R10_UNORM:
        return false;
    case gal_texture_format::A2B10G10R10_UINT:
        return false;
    case gal_texture_format::A2B10G10R10_SNORM:
        return false;
    case gal_texture_format::A2B10G10R10_SINT:
        return false;
    case gal_texture_format::R10G10B10A2_UNORM:
        return false;
    case gal_texture_format::R10G10B10A2_UINT:
        return false;
    case gal_texture_format::R10G10B10A2_SNORM:
        return false;
    case gal_texture_format::R10G10B10A2_SINT:
        return false;
    case gal_texture_format::B10G10R10A2_UNORM:
        return false;
    case gal_texture_format::B10G10R10A2_UINT:
        return false;
    case gal_texture_format::B10G10R10A2_SNORM:
        return false;
    case gal_texture_format::B10G10R10A2_SINT:
        return false;
    case gal_texture_format::B10G11R11_UFLOAT:
        return false;
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return false;
    case gal_texture_format::X8_D24_UNORM:
        return false;
    case gal_texture_format::D16_UNORM_S8_UINT:
        return false;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return false;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return false;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return false;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return false;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return false;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return false;
    case gal_texture_format::DXBC2_UNORM:
        return false;
    case gal_texture_format::DXBC2_SRGB:
        return false;
    case gal_texture_format::DXBC3_UNORM:
        return false;
    case gal_texture_format::DXBC3_SRGB:
        return false;
    case gal_texture_format::DXBC4_UNORM:
        return false;
    case gal_texture_format::DXBC4_SNORM:
        return false;
    case gal_texture_format::DXBC5_UNORM:
        return false;
    case gal_texture_format::DXBC5_SNORM:
        return false;
    case gal_texture_format::DXBC6H_UFLOAT:
        return false;
    case gal_texture_format::DXBC6H_SFLOAT:
        return false;
    case gal_texture_format::DXBC7_UNORM:
        return false;
    case gal_texture_format::DXBC7_SRGB:
        return false;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return false;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return false;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return false;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return false;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return false;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return false;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return false;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return false;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return false;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return false;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return false;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return false;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return false;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return false;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return false;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return false;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return false;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return false;
    case gal_texture_format::ASTC_4x4_UNORM:
        return false;
    case gal_texture_format::ASTC_4x4_SRGB:
        return false;
    case gal_texture_format::ASTC_5x4_UNORM:
        return false;
    case gal_texture_format::ASTC_5x4_SRGB:
        return false;
    case gal_texture_format::ASTC_5x5_UNORM:
        return false;
    case gal_texture_format::ASTC_5x5_SRGB:
        return false;
    case gal_texture_format::ASTC_6x5_UNORM:
        return false;
    case gal_texture_format::ASTC_6x5_SRGB:
        return false;
    case gal_texture_format::ASTC_6x6_UNORM:
        return false;
    case gal_texture_format::ASTC_6x6_SRGB:
        return false;
    case gal_texture_format::ASTC_8x5_UNORM:
        return false;
    case gal_texture_format::ASTC_8x5_SRGB:
        return false;
    case gal_texture_format::ASTC_8x6_UNORM:
        return false;
    case gal_texture_format::ASTC_8x6_SRGB:
        return false;
    case gal_texture_format::ASTC_8x8_UNORM:
        return false;
    case gal_texture_format::ASTC_8x8_SRGB:
        return false;
    case gal_texture_format::ASTC_10x5_UNORM:
        return false;
    case gal_texture_format::ASTC_10x5_SRGB:
        return false;
    case gal_texture_format::ASTC_10x6_UNORM:
        return false;
    case gal_texture_format::ASTC_10x6_SRGB:
        return false;
    case gal_texture_format::ASTC_10x8_UNORM:
        return false;
    case gal_texture_format::ASTC_10x8_SRGB:
        return false;
    case gal_texture_format::ASTC_10x10_UNORM:
        return false;
    case gal_texture_format::ASTC_10x10_SRGB:
        return false;
    case gal_texture_format::ASTC_12x10_UNORM:
        return false;
    case gal_texture_format::ASTC_12x10_SRGB:
        return false;
    case gal_texture_format::ASTC_12x12_UNORM:
        return false;
    case gal_texture_format::ASTC_12x12_SRGB:
        return false;
    case gal_texture_format::CLUT_P4:
        return false;
    case gal_texture_format::CLUT_P4A4:
        return false;
    case gal_texture_format::CLUT_P8:
        return false;
    case gal_texture_format::CLUT_P8A8:
        return false;
    default:
        return true;
    }
}

constexpr u32 gal_tf_width_of_block(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 1;
    case gal_texture_format::R1_UNORM:
        return 8;
    case gal_texture_format::R2_UNORM:
        return 4;
    case gal_texture_format::R4_UNORM:
        return 2;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return 4;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return 4;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return 4;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return 4;
    case gal_texture_format::DXBC2_UNORM:
        return 4;
    case gal_texture_format::DXBC2_SRGB:
        return 4;
    case gal_texture_format::DXBC3_UNORM:
        return 4;
    case gal_texture_format::DXBC3_SRGB:
        return 4;
    case gal_texture_format::DXBC4_UNORM:
        return 4;
    case gal_texture_format::DXBC4_SNORM:
        return 4;
    case gal_texture_format::DXBC5_UNORM:
        return 4;
    case gal_texture_format::DXBC5_SNORM:
        return 4;
    case gal_texture_format::DXBC6H_UFLOAT:
        return 4;
    case gal_texture_format::DXBC6H_SFLOAT:
        return 4;
    case gal_texture_format::DXBC7_UNORM:
        return 4;
    case gal_texture_format::DXBC7_SRGB:
        return 4;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return 8;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return 8;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return 8;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return 4;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return 8;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return 4;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return 4;
    case gal_texture_format::ASTC_4x4_UNORM:
        return 4;
    case gal_texture_format::ASTC_4x4_SRGB:
        return 4;
    case gal_texture_format::ASTC_5x4_UNORM:
        return 5;
    case gal_texture_format::ASTC_5x4_SRGB:
        return 5;
    case gal_texture_format::ASTC_5x5_UNORM:
        return 5;
    case gal_texture_format::ASTC_5x5_SRGB:
        return 5;
    case gal_texture_format::ASTC_6x5_UNORM:
        return 6;
    case gal_texture_format::ASTC_6x5_SRGB:
        return 6;
    case gal_texture_format::ASTC_6x6_UNORM:
        return 6;
    case gal_texture_format::ASTC_6x6_SRGB:
        return 6;
    case gal_texture_format::ASTC_8x5_UNORM:
        return 8;
    case gal_texture_format::ASTC_8x5_SRGB:
        return 8;
    case gal_texture_format::ASTC_8x6_UNORM:
        return 8;
    case gal_texture_format::ASTC_8x6_SRGB:
        return 8;
    case gal_texture_format::ASTC_8x8_UNORM:
        return 8;
    case gal_texture_format::ASTC_8x8_SRGB:
        return 8;
    case gal_texture_format::ASTC_10x5_UNORM:
        return 10;
    case gal_texture_format::ASTC_10x5_SRGB:
        return 10;
    case gal_texture_format::ASTC_10x6_UNORM:
        return 10;
    case gal_texture_format::ASTC_10x6_SRGB:
        return 10;
    case gal_texture_format::ASTC_10x8_UNORM:
        return 10;
    case gal_texture_format::ASTC_10x8_SRGB:
        return 10;
    case gal_texture_format::ASTC_10x10_UNORM:
        return 10;
    case gal_texture_format::ASTC_10x10_SRGB:
        return 10;
    case gal_texture_format::ASTC_12x10_UNORM:
        return 12;
    case gal_texture_format::ASTC_12x10_SRGB:
        return 12;
    case gal_texture_format::ASTC_12x12_UNORM:
        return 12;
    case gal_texture_format::ASTC_12x12_SRGB:
        return 12;
    case gal_texture_format::CLUT_P4:
        return 2;
    default:
        return 1;
    }
}

constexpr u32 gal_tf_height_of_block(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 1;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return 4;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return 4;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return 4;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return 4;
    case gal_texture_format::DXBC2_UNORM:
        return 4;
    case gal_texture_format::DXBC2_SRGB:
        return 4;
    case gal_texture_format::DXBC3_UNORM:
        return 4;
    case gal_texture_format::DXBC3_SRGB:
        return 4;
    case gal_texture_format::DXBC4_UNORM:
        return 4;
    case gal_texture_format::DXBC4_SNORM:
        return 4;
    case gal_texture_format::DXBC5_UNORM:
        return 4;
    case gal_texture_format::DXBC5_SNORM:
        return 4;
    case gal_texture_format::DXBC6H_UFLOAT:
        return 4;
    case gal_texture_format::DXBC6H_SFLOAT:
        return 4;
    case gal_texture_format::DXBC7_UNORM:
        return 4;
    case gal_texture_format::DXBC7_SRGB:
        return 4;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return 4;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return 4;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return 4;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return 4;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return 4;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return 4;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return 4;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return 4;
    case gal_texture_format::ASTC_4x4_UNORM:
        return 4;
    case gal_texture_format::ASTC_4x4_SRGB:
        return 4;
    case gal_texture_format::ASTC_5x4_UNORM:
        return 4;
    case gal_texture_format::ASTC_5x4_SRGB:
        return 4;
    case gal_texture_format::ASTC_5x5_UNORM:
        return 5;
    case gal_texture_format::ASTC_5x5_SRGB:
        return 5;
    case gal_texture_format::ASTC_6x5_UNORM:
        return 5;
    case gal_texture_format::ASTC_6x5_SRGB:
        return 5;
    case gal_texture_format::ASTC_6x6_UNORM:
        return 6;
    case gal_texture_format::ASTC_6x6_SRGB:
        return 6;
    case gal_texture_format::ASTC_8x5_UNORM:
        return 5;
    case gal_texture_format::ASTC_8x5_SRGB:
        return 5;
    case gal_texture_format::ASTC_8x6_UNORM:
        return 6;
    case gal_texture_format::ASTC_8x6_SRGB:
        return 6;
    case gal_texture_format::ASTC_8x8_UNORM:
        return 8;
    case gal_texture_format::ASTC_8x8_SRGB:
        return 8;
    case gal_texture_format::ASTC_10x5_UNORM:
        return 5;
    case gal_texture_format::ASTC_10x5_SRGB:
        return 5;
    case gal_texture_format::ASTC_10x6_UNORM:
        return 6;
    case gal_texture_format::ASTC_10x6_SRGB:
        return 6;
    case gal_texture_format::ASTC_10x8_UNORM:
        return 8;
    case gal_texture_format::ASTC_10x8_SRGB:
        return 8;
    case gal_texture_format::ASTC_10x10_UNORM:
        return 10;
    case gal_texture_format::ASTC_10x10_SRGB:
        return 10;
    case gal_texture_format::ASTC_12x10_UNORM:
        return 10;
    case gal_texture_format::ASTC_12x10_SRGB:
        return 10;
    case gal_texture_format::ASTC_12x12_UNORM:
        return 12;
    case gal_texture_format::ASTC_12x12_SRGB:
        return 12;
    default:
        return 1;
    }
}

constexpr u32 gal_tf_depth_of_block(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 1;
    default:
        return 1;
    }
}

constexpr u32 gal_tf_bit_size_of_block(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 0;
    case gal_texture_format::R1_UNORM:
        return 8;
    case gal_texture_format::R2_UNORM:
        return 8;
    case gal_texture_format::R4_UNORM:
        return 8;
    case gal_texture_format::R4G4_UNORM:
        return 8;
    case gal_texture_format::G4R4_UNORM:
        return 8;
    case gal_texture_format::A8_UNORM:
        return 8;
    case gal_texture_format::R8_UNORM:
        return 8;
    case gal_texture_format::R8_SNORM:
        return 8;
    case gal_texture_format::R8_UINT:
        return 8;
    case gal_texture_format::R8_SINT:
        return 8;
    case gal_texture_format::R8_SRGB:
        return 8;
    case gal_texture_format::B2G3R3_UNORM:
        return 8;
    case gal_texture_format::R4G4B4A4_UNORM:
        return 16;
    case gal_texture_format::R4G4B4X4_UNORM:
        return 16;
    case gal_texture_format::B4G4R4A4_UNORM:
        return 16;
    case gal_texture_format::B4G4R4X4_UNORM:
        return 16;
    case gal_texture_format::A4R4G4B4_UNORM:
        return 16;
    case gal_texture_format::X4R4G4B4_UNORM:
        return 16;
    case gal_texture_format::A4B4G4R4_UNORM:
        return 16;
    case gal_texture_format::X4B4G4R4_UNORM:
        return 16;
    case gal_texture_format::R5G6B5_UNORM:
        return 16;
    case gal_texture_format::B5G6R5_UNORM:
        return 16;
    case gal_texture_format::R5G5B5A1_UNORM:
        return 16;
    case gal_texture_format::B5G5R5A1_UNORM:
        return 16;
    case gal_texture_format::A1B5G5R5_UNORM:
        return 16;
    case gal_texture_format::A1R5G5B5_UNORM:
        return 16;
    case gal_texture_format::R5G5B5X1_UNORM:
        return 16;
    case gal_texture_format::B5G5R5X1_UNORM:
        return 16;
    case gal_texture_format::X1R5G5B5_UNORM:
        return 16;
    case gal_texture_format::X1B5G5R5_UNORM:
        return 16;
    case gal_texture_format::B2G3R3A8_UNORM:
        return 16;
    case gal_texture_format::R8G8_UNORM:
        return 16;
    case gal_texture_format::R8G8_SNORM:
        return 16;
    case gal_texture_format::G8R8_UNORM:
        return 16;
    case gal_texture_format::G8R8_SNORM:
        return 16;
    case gal_texture_format::R8G8_UINT:
        return 16;
    case gal_texture_format::R8G8_SINT:
        return 16;
    case gal_texture_format::R8G8_SRGB:
        return 16;
    case gal_texture_format::R16_UNORM:
        return 16;
    case gal_texture_format::R16_SNORM:
        return 16;
    case gal_texture_format::R16_UINT:
        return 16;
    case gal_texture_format::R16_SINT:
        return 16;
    case gal_texture_format::R16_SFLOAT:
        return 16;
    case gal_texture_format::R16_SBFLOAT:
        return 16;
    case gal_texture_format::R8G8B8_UNORM:
        return 24;
    case gal_texture_format::R8G8B8_SNORM:
        return 24;
    case gal_texture_format::R8G8B8_UINT:
        return 24;
    case gal_texture_format::R8G8B8_SINT:
        return 24;
    case gal_texture_format::R8G8B8_SRGB:
        return 24;
    case gal_texture_format::B8G8R8_UNORM:
        return 24;
    case gal_texture_format::B8G8R8_SNORM:
        return 24;
    case gal_texture_format::B8G8R8_UINT:
        return 24;
    case gal_texture_format::B8G8R8_SINT:
        return 24;
    case gal_texture_format::B8G8R8_SRGB:
        return 24;
    case gal_texture_format::R16G16B16_UNORM:
        return 48;
    case gal_texture_format::R16G16B16_SNORM:
        return 48;
    case gal_texture_format::R16G16B16_UINT:
        return 48;
    case gal_texture_format::R16G16B16_SINT:
        return 48;
    case gal_texture_format::R16G16B16_SFLOAT:
        return 48;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return 48;
    case gal_texture_format::R16G16B16A16_UNORM:
        return 64;
    case gal_texture_format::R16G16B16A16_SNORM:
        return 64;
    case gal_texture_format::R16G16B16A16_UINT:
        return 64;
    case gal_texture_format::R16G16B16A16_SINT:
        return 64;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return 64;
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return 64;
    case gal_texture_format::R32G32_UINT:
        return 64;
    case gal_texture_format::R32G32_SINT:
        return 64;
    case gal_texture_format::R32G32_SFLOAT:
        return 64;
    case gal_texture_format::R32G32B32_UINT:
        return 96;
    case gal_texture_format::R32G32B32_SINT:
        return 96;
    case gal_texture_format::R32G32B32_SFLOAT:
        return 96;
    case gal_texture_format::R32G32B32A32_UINT:
        return 128;
    case gal_texture_format::R32G32B32A32_SINT:
        return 128;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return 128;
    case gal_texture_format::R64_UINT:
        return 64;
    case gal_texture_format::R64_SINT:
        return 64;
    case gal_texture_format::R64_SFLOAT:
        return 64;
    case gal_texture_format::R64G64_UINT:
        return 128;
    case gal_texture_format::R64G64_SINT:
        return 128;
    case gal_texture_format::R64G64_SFLOAT:
        return 128;
    case gal_texture_format::R64G64B64_UINT:
        return 192;
    case gal_texture_format::R64G64B64_SINT:
        return 192;
    case gal_texture_format::R64G64B64_SFLOAT:
        return 192;
    case gal_texture_format::R64G64B64A64_UINT:
        return 256;
    case gal_texture_format::R64G64B64A64_SINT:
        return 256;
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return 256;
    case gal_texture_format::D16_UNORM:
        return 16;
    case gal_texture_format::S8_UINT:
        return 8;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return 64;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return 64;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return 64;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return 64;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return 64;
    case gal_texture_format::DXBC2_UNORM:
        return 128;
    case gal_texture_format::DXBC2_SRGB:
        return 128;
    case gal_texture_format::DXBC3_UNORM:
        return 128;
    case gal_texture_format::DXBC3_SRGB:
        return 128;
    case gal_texture_format::DXBC4_UNORM:
        return 64;
    case gal_texture_format::DXBC4_SNORM:
        return 64;
    case gal_texture_format::DXBC5_UNORM:
        return 128;
    case gal_texture_format::DXBC5_SNORM:
        return 128;
    case gal_texture_format::DXBC6H_UFLOAT:
        return 128;
    case gal_texture_format::DXBC6H_SFLOAT:
        return 128;
    case gal_texture_format::DXBC7_UNORM:
        return 128;
    case gal_texture_format::DXBC7_SRGB:
        return 128;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return 64;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return 64;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return 64;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return 64;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return 64;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return 64;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return 64;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return 64;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return 64;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return 64;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return 64;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return 64;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return 128;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return 128;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return 64;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return 64;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return 128;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return 128;
    case gal_texture_format::ASTC_4x4_UNORM:
        return 128;
    case gal_texture_format::ASTC_4x4_SRGB:
        return 128;
    case gal_texture_format::ASTC_5x4_UNORM:
        return 128;
    case gal_texture_format::ASTC_5x4_SRGB:
        return 128;
    case gal_texture_format::ASTC_5x5_UNORM:
        return 128;
    case gal_texture_format::ASTC_5x5_SRGB:
        return 128;
    case gal_texture_format::ASTC_6x5_UNORM:
        return 128;
    case gal_texture_format::ASTC_6x5_SRGB:
        return 128;
    case gal_texture_format::ASTC_6x6_UNORM:
        return 128;
    case gal_texture_format::ASTC_6x6_SRGB:
        return 128;
    case gal_texture_format::ASTC_8x5_UNORM:
        return 128;
    case gal_texture_format::ASTC_8x5_SRGB:
        return 128;
    case gal_texture_format::ASTC_8x6_UNORM:
        return 128;
    case gal_texture_format::ASTC_8x6_SRGB:
        return 128;
    case gal_texture_format::ASTC_8x8_UNORM:
        return 128;
    case gal_texture_format::ASTC_8x8_SRGB:
        return 128;
    case gal_texture_format::ASTC_10x5_UNORM:
        return 128;
    case gal_texture_format::ASTC_10x5_SRGB:
        return 128;
    case gal_texture_format::ASTC_10x6_UNORM:
        return 128;
    case gal_texture_format::ASTC_10x6_SRGB:
        return 128;
    case gal_texture_format::ASTC_10x8_UNORM:
        return 128;
    case gal_texture_format::ASTC_10x8_SRGB:
        return 128;
    case gal_texture_format::ASTC_10x10_UNORM:
        return 128;
    case gal_texture_format::ASTC_10x10_SRGB:
        return 128;
    case gal_texture_format::ASTC_12x10_UNORM:
        return 128;
    case gal_texture_format::ASTC_12x10_SRGB:
        return 128;
    case gal_texture_format::ASTC_12x12_UNORM:
        return 128;
    case gal_texture_format::ASTC_12x12_SRGB:
        return 128;
    case gal_texture_format::CLUT_P4:
        return 8;
    case gal_texture_format::CLUT_P4A4:
        return 8;
    case gal_texture_format::CLUT_P8:
        return 8;
    case gal_texture_format::CLUT_P8A8:
        return 16;
    case gal_texture_format::G16B16G16R16_422_UNORM:
        return 8;
    case gal_texture_format::B16G16R16G16_422_UNORM:
        return 8;
    case gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
        return 8;
    case gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
        return 8;
    case gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
        return 8;
    case gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
        return 8;
    case gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
        return 8;
    case gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
        return 8;
    case gal_texture_format::G8B8G8R8_422_UNORM:
        return 4;
    case gal_texture_format::B8G8R8G8_422_UNORM:
        return 4;
    default:
        return 32;
    }
}

constexpr u32 gal_tf_channel_count(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return 0;
    case gal_texture_format::R1_UNORM:
        return 1;
    case gal_texture_format::R2_UNORM:
        return 1;
    case gal_texture_format::R4_UNORM:
        return 1;
    case gal_texture_format::R4G4_UNORM:
        return 2;
    case gal_texture_format::G4R4_UNORM:
        return 2;
    case gal_texture_format::A8_UNORM:
        return 1;
    case gal_texture_format::R8_UNORM:
        return 1;
    case gal_texture_format::R8_SNORM:
        return 1;
    case gal_texture_format::R8_UINT:
        return 1;
    case gal_texture_format::R8_SINT:
        return 1;
    case gal_texture_format::R8_SRGB:
        return 1;
    case gal_texture_format::B2G3R3_UNORM:
        return 3;
    case gal_texture_format::R5G6B5_UNORM:
        return 3;
    case gal_texture_format::B5G6R5_UNORM:
        return 3;
    case gal_texture_format::R8G8_UNORM:
        return 2;
    case gal_texture_format::R8G8_SNORM:
        return 2;
    case gal_texture_format::G8R8_UNORM:
        return 2;
    case gal_texture_format::G8R8_SNORM:
        return 2;
    case gal_texture_format::R8G8_UINT:
        return 2;
    case gal_texture_format::R8G8_SINT:
        return 2;
    case gal_texture_format::R8G8_SRGB:
        return 2;
    case gal_texture_format::R16_UNORM:
        return 1;
    case gal_texture_format::R16_SNORM:
        return 1;
    case gal_texture_format::R16_UINT:
        return 1;
    case gal_texture_format::R16_SINT:
        return 1;
    case gal_texture_format::R16_SFLOAT:
        return 1;
    case gal_texture_format::R16_SBFLOAT:
        return 1;
    case gal_texture_format::R8G8B8_UNORM:
        return 3;
    case gal_texture_format::R8G8B8_SNORM:
        return 3;
    case gal_texture_format::R8G8B8_UINT:
        return 3;
    case gal_texture_format::R8G8B8_SINT:
        return 3;
    case gal_texture_format::R8G8B8_SRGB:
        return 3;
    case gal_texture_format::B8G8R8_UNORM:
        return 3;
    case gal_texture_format::B8G8R8_SNORM:
        return 3;
    case gal_texture_format::B8G8R8_UINT:
        return 3;
    case gal_texture_format::B8G8R8_SINT:
        return 3;
    case gal_texture_format::B8G8R8_SRGB:
        return 3;
    case gal_texture_format::R16G16_UNORM:
        return 2;
    case gal_texture_format::G16R16_UNORM:
        return 2;
    case gal_texture_format::R16G16_SNORM:
        return 2;
    case gal_texture_format::G16R16_SNORM:
        return 2;
    case gal_texture_format::R16G16_UINT:
        return 2;
    case gal_texture_format::R16G16_SINT:
        return 2;
    case gal_texture_format::R16G16_SFLOAT:
        return 2;
    case gal_texture_format::R16G16_SBFLOAT:
        return 2;
    case gal_texture_format::R32_UINT:
        return 1;
    case gal_texture_format::R32_SINT:
        return 1;
    case gal_texture_format::R32_SFLOAT:
        return 1;
    case gal_texture_format::B10G11R11_UFLOAT:
        return 3;
    case gal_texture_format::R16G16B16_UNORM:
        return 3;
    case gal_texture_format::R16G16B16_SNORM:
        return 3;
    case gal_texture_format::R16G16B16_UINT:
        return 3;
    case gal_texture_format::R16G16B16_SINT:
        return 3;
    case gal_texture_format::R16G16B16_SFLOAT:
        return 3;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return 3;
    case gal_texture_format::R32G32_UINT:
        return 2;
    case gal_texture_format::R32G32_SINT:
        return 2;
    case gal_texture_format::R32G32_SFLOAT:
        return 2;
    case gal_texture_format::R32G32B32_UINT:
        return 3;
    case gal_texture_format::R32G32B32_SINT:
        return 3;
    case gal_texture_format::R32G32B32_SFLOAT:
        return 3;
    case gal_texture_format::R64_UINT:
        return 1;
    case gal_texture_format::R64_SINT:
        return 1;
    case gal_texture_format::R64_SFLOAT:
        return 1;
    case gal_texture_format::R64G64_UINT:
        return 2;
    case gal_texture_format::R64G64_SINT:
        return 2;
    case gal_texture_format::R64G64_SFLOAT:
        return 2;
    case gal_texture_format::R64G64B64_UINT:
        return 3;
    case gal_texture_format::R64G64B64_SINT:
        return 3;
    case gal_texture_format::R64G64B64_SFLOAT:
        return 3;
    case gal_texture_format::D16_UNORM:
        return 1;
    case gal_texture_format::X8_D24_UNORM:
        return 2;
    case gal_texture_format::D32_SFLOAT:
        return 1;
    case gal_texture_format::S8_UINT:
        return 1;
    case gal_texture_format::D16_UNORM_S8_UINT:
        return 2;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return 2;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return 2;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return 3;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return 3;
    case gal_texture_format::DXBC4_UNORM:
        return 1;
    case gal_texture_format::DXBC4_SNORM:
        return 1;
    case gal_texture_format::DXBC5_UNORM:
        return 2;
    case gal_texture_format::DXBC5_SNORM:
        return 2;
    case gal_texture_format::DXBC6H_UFLOAT:
        return 3;
    case gal_texture_format::DXBC6H_SFLOAT:
        return 3;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return 3;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return 3;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return 1;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return 1;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return 2;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return 2;
    case gal_texture_format::CLUT_P4:
        return 1;
    case gal_texture_format::CLUT_P4A4:
        return 2;
    case gal_texture_format::CLUT_P8:
        return 1;
    case gal_texture_format::CLUT_P8A8:
        return 2;
    default:
        return 4;
    }
}

constexpr u32 gal_tf_channel_bit_width_physical(gal_texture_format fmt, uint32_t channel) {
    if (gal_tf_is_homogenous(fmt) || channel == 0) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0;
        case gal_texture_format::R1_UNORM:
            return 1;
        case gal_texture_format::R2_UNORM:
            return 2;
        case gal_texture_format::R4_UNORM:
            return 4;
        case gal_texture_format::R4G4_UNORM:
            return 4;
        case gal_texture_format::G4R4_UNORM:
            return 4;
        case gal_texture_format::B2G3R3_UNORM:
            return 2;
        case gal_texture_format::R4G4B4A4_UNORM:
            return 4;
        case gal_texture_format::R4G4B4X4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4A4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4X4_UNORM:
            return 4;
        case gal_texture_format::A4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::X4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::A4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::X4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::R5G6B5_UNORM:
            return 5;
        case gal_texture_format::B5G6R5_UNORM:
            return 5;
        case gal_texture_format::R5G5B5A1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5A1_UNORM:
            return 5;
        case gal_texture_format::A1B5G5R5_UNORM:
            return 1;
        case gal_texture_format::A1R5G5B5_UNORM:
            return 1;
        case gal_texture_format::R5G5B5X1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5X1_UNORM:
            return 5;
        case gal_texture_format::X1R5G5B5_UNORM:
            return 1;
        case gal_texture_format::X1B5G5R5_UNORM:
            return 1;
        case gal_texture_format::B2G3R3A8_UNORM:
            return 2;
        case gal_texture_format::R16_UNORM:
            return 16;
        case gal_texture_format::R16_SNORM:
            return 16;
        case gal_texture_format::R16_UINT:
            return 16;
        case gal_texture_format::R16_SINT:
            return 16;
        case gal_texture_format::R16_SFLOAT:
            return 16;
        case gal_texture_format::R16_SBFLOAT:
            return 16;
        case gal_texture_format::R16G16_UNORM:
            return 16;
        case gal_texture_format::G16R16_UNORM:
            return 16;
        case gal_texture_format::R16G16_SNORM:
            return 16;
        case gal_texture_format::G16R16_SNORM:
            return 16;
        case gal_texture_format::R16G16_UINT:
            return 16;
        case gal_texture_format::R16G16_SINT:
            return 16;
        case gal_texture_format::R16G16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16_SBFLOAT:
            return 16;
        case gal_texture_format::R32_UINT:
            return 32;
        case gal_texture_format::R32_SINT:
            return 32;
        case gal_texture_format::R32_SFLOAT:
            return 32;
        case gal_texture_format::A2R10G10B10_UNORM:
            return 2;
        case gal_texture_format::A2R10G10B10_UINT:
            return 2;
        case gal_texture_format::A2R10G10B10_SNORM:
            return 2;
        case gal_texture_format::A2R10G10B10_SINT:
            return 2;
        case gal_texture_format::A2B10G10R10_UNORM:
            return 2;
        case gal_texture_format::A2B10G10R10_UINT:
            return 2;
        case gal_texture_format::A2B10G10R10_SNORM:
            return 2;
        case gal_texture_format::A2B10G10R10_SINT:
            return 2;
        case gal_texture_format::R10G10B10A2_UNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_UINT:
            return 10;
        case gal_texture_format::R10G10B10A2_SNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_SINT:
            return 10;
        case gal_texture_format::B10G10R10A2_UNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_UINT:
            return 10;
        case gal_texture_format::B10G10R10A2_SNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_SINT:
            return 10;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 10;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 5;
        case gal_texture_format::R16G16B16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16_UINT:
            return 16;
        case gal_texture_format::R16G16B16_SINT:
            return 16;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_UINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 16;
        case gal_texture_format::R32G32_UINT:
            return 32;
        case gal_texture_format::R32G32_SINT:
            return 32;
        case gal_texture_format::R32G32_SFLOAT:
            return 32;
        case gal_texture_format::R32G32B32_UINT:
            return 32;
        case gal_texture_format::R32G32B32_SINT:
            return 32;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 32;
        case gal_texture_format::R32G32B32A32_UINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 32;
        case gal_texture_format::R64_UINT:
            return 64;
        case gal_texture_format::R64_SINT:
            return 64;
        case gal_texture_format::R64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64_UINT:
            return 64;
        case gal_texture_format::R64G64_SINT:
            return 64;
        case gal_texture_format::R64G64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64B64_UINT:
            return 64;
        case gal_texture_format::R64G64B64_SINT:
            return 64;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64B64A64_UINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 64;
        case gal_texture_format::D16_UNORM:
            return 16;
        case gal_texture_format::D32_SFLOAT:
            return 32;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 16;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 24;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 32;
        case gal_texture_format::DXBC1_RGB_UNORM:
            return 5;
        case gal_texture_format::DXBC1_RGB_SRGB:
            return 5;
        case gal_texture_format::DXBC1_RGBA_UNORM:
            return 5;
        case gal_texture_format::DXBC1_RGBA_SRGB:
            return 5;
        case gal_texture_format::DXBC2_UNORM:
            return 5;
        case gal_texture_format::DXBC2_SRGB:
            return 5;
        case gal_texture_format::DXBC3_UNORM:
            return 5;
        case gal_texture_format::DXBC3_SRGB:
            return 5;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 16;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 16;
        case gal_texture_format::ETC2_EAC_R11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return 11;
        case gal_texture_format::CLUT_P4:
            return 4;
        case gal_texture_format::CLUT_P4A4:
            return 4;
        default:
            return 8;
        }
    } else if (channel == 1) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0;
        case gal_texture_format::R1_UNORM:
            return 0;
        case gal_texture_format::R2_UNORM:
            return 0;
        case gal_texture_format::R4_UNORM:
            return 0;
        case gal_texture_format::R4G4_UNORM:
            return 4;
        case gal_texture_format::G4R4_UNORM:
            return 4;
        case gal_texture_format::A8_UNORM:
            return 0;
        case gal_texture_format::R8_UNORM:
            return 0;
        case gal_texture_format::R8_SNORM:
            return 0;
        case gal_texture_format::R8_UINT:
            return 0;
        case gal_texture_format::R8_SINT:
            return 0;
        case gal_texture_format::R8_SRGB:
            return 0;
        case gal_texture_format::B2G3R3_UNORM:
            return 3;
        case gal_texture_format::R4G4B4A4_UNORM:
            return 4;
        case gal_texture_format::R4G4B4X4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4A4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4X4_UNORM:
            return 4;
        case gal_texture_format::A4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::X4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::A4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::X4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::R5G6B5_UNORM:
            return 6;
        case gal_texture_format::B5G6R5_UNORM:
            return 6;
        case gal_texture_format::R5G5B5A1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5A1_UNORM:
            return 5;
        case gal_texture_format::A1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::A1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::R5G5B5X1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5X1_UNORM:
            return 5;
        case gal_texture_format::X1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::X1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::B2G3R3A8_UNORM:
            return 3;
        case gal_texture_format::R16_UNORM:
            return 0;
        case gal_texture_format::R16_SNORM:
            return 0;
        case gal_texture_format::R16_UINT:
            return 0;
        case gal_texture_format::R16_SINT:
            return 0;
        case gal_texture_format::R16_SFLOAT:
            return 0;
        case gal_texture_format::R16_SBFLOAT:
            return 0;
        case gal_texture_format::R16G16_UNORM:
            return 16;
        case gal_texture_format::G16R16_UNORM:
            return 16;
        case gal_texture_format::R16G16_SNORM:
            return 16;
        case gal_texture_format::G16R16_SNORM:
            return 16;
        case gal_texture_format::R16G16_UINT:
            return 16;
        case gal_texture_format::R16G16_SINT:
            return 16;
        case gal_texture_format::R16G16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16_SBFLOAT:
            return 16;
        case gal_texture_format::R32_UINT:
            return 0;
        case gal_texture_format::R32_SINT:
            return 0;
        case gal_texture_format::R32_SFLOAT:
            return 0;
        case gal_texture_format::A2R10G10B10_UNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_UINT:
            return 10;
        case gal_texture_format::A2R10G10B10_SNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_SINT:
            return 10;
        case gal_texture_format::A2B10G10R10_UNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_UINT:
            return 10;
        case gal_texture_format::A2B10G10R10_SNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_SINT:
            return 10;
        case gal_texture_format::R10G10B10A2_UNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_UINT:
            return 10;
        case gal_texture_format::R10G10B10A2_SNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_SINT:
            return 10;
        case gal_texture_format::B10G10R10A2_UNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_UINT:
            return 10;
        case gal_texture_format::B10G10R10A2_SNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_SINT:
            return 10;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 11;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 9;
        case gal_texture_format::R16G16B16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16_UINT:
            return 16;
        case gal_texture_format::R16G16B16_SINT:
            return 16;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_UINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 16;
        case gal_texture_format::R32G32_UINT:
            return 32;
        case gal_texture_format::R32G32_SINT:
            return 32;
        case gal_texture_format::R32G32_SFLOAT:
            return 32;
        case gal_texture_format::R32G32B32_UINT:
            return 32;
        case gal_texture_format::R32G32B32_SINT:
            return 32;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 32;
        case gal_texture_format::R32G32B32A32_UINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 32;
        case gal_texture_format::R64_UINT:
            return 0;
        case gal_texture_format::R64_SINT:
            return 0;
        case gal_texture_format::R64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64_UINT:
            return 64;
        case gal_texture_format::R64G64_SINT:
            return 64;
        case gal_texture_format::R64G64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64B64_UINT:
            return 64;
        case gal_texture_format::R64G64B64_SINT:
            return 64;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64B64A64_UINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 64;
        case gal_texture_format::D16_UNORM:
            return 0;
        case gal_texture_format::X8_D24_UNORM:
            return 24;
        case gal_texture_format::D32_SFLOAT:
            return 0;
        case gal_texture_format::S8_UINT:
            return 0;
        case gal_texture_format::DXBC1_RGB_UNORM:
            return 6;
        case gal_texture_format::DXBC1_RGB_SRGB:
            return 6;
        case gal_texture_format::DXBC1_RGBA_UNORM:
            return 6;
        case gal_texture_format::DXBC1_RGBA_SRGB:
            return 6;
        case gal_texture_format::DXBC2_UNORM:
            return 6;
        case gal_texture_format::DXBC2_SRGB:
            return 6;
        case gal_texture_format::DXBC3_UNORM:
            return 6;
        case gal_texture_format::DXBC3_SRGB:
            return 6;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 16;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 16;
        case gal_texture_format::ETC2_EAC_R11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return 11;
        case gal_texture_format::CLUT_P4:
            return 0;
        case gal_texture_format::CLUT_P4A4:
            return 4;
        case gal_texture_format::CLUT_P8:
            return 0;
        default:
            return 8;
        }
    } else if (channel == 2) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0;
        case gal_texture_format::R1_UNORM:
            return 0;
        case gal_texture_format::R2_UNORM:
            return 0;
        case gal_texture_format::R4_UNORM:
            return 0;
        case gal_texture_format::R4G4_UNORM:
            return 0;
        case gal_texture_format::G4R4_UNORM:
            return 0;
        case gal_texture_format::A8_UNORM:
            return 0;
        case gal_texture_format::R8_UNORM:
            return 0;
        case gal_texture_format::R8_SNORM:
            return 0;
        case gal_texture_format::R8_UINT:
            return 0;
        case gal_texture_format::R8_SINT:
            return 0;
        case gal_texture_format::R8_SRGB:
            return 0;
        case gal_texture_format::B2G3R3_UNORM:
            return 3;
        case gal_texture_format::R4G4B4A4_UNORM:
            return 4;
        case gal_texture_format::R4G4B4X4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4A4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4X4_UNORM:
            return 4;
        case gal_texture_format::A4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::X4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::A4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::X4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::R5G6B5_UNORM:
            return 5;
        case gal_texture_format::B5G6R5_UNORM:
            return 5;
        case gal_texture_format::R5G5B5A1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5A1_UNORM:
            return 5;
        case gal_texture_format::A1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::A1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::R5G5B5X1_UNORM:
            return 5;
        case gal_texture_format::B5G5R5X1_UNORM:
            return 5;
        case gal_texture_format::X1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::X1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::B2G3R3A8_UNORM:
            return 3;
        case gal_texture_format::R8G8_UNORM:
            return 0;
        case gal_texture_format::R8G8_SNORM:
            return 0;
        case gal_texture_format::G8R8_UNORM:
            return 0;
        case gal_texture_format::G8R8_SNORM:
            return 0;
        case gal_texture_format::R8G8_UINT:
            return 0;
        case gal_texture_format::R8G8_SINT:
            return 0;
        case gal_texture_format::R8G8_SRGB:
            return 0;
        case gal_texture_format::R16_UNORM:
            return 0;
        case gal_texture_format::R16_SNORM:
            return 0;
        case gal_texture_format::R16_UINT:
            return 0;
        case gal_texture_format::R16_SINT:
            return 0;
        case gal_texture_format::R16_SFLOAT:
            return 0;
        case gal_texture_format::R16_SBFLOAT:
            return 0;
        case gal_texture_format::R16G16_UNORM:
            return 0;
        case gal_texture_format::G16R16_UNORM:
            return 0;
        case gal_texture_format::R16G16_SNORM:
            return 0;
        case gal_texture_format::G16R16_SNORM:
            return 0;
        case gal_texture_format::R16G16_UINT:
            return 0;
        case gal_texture_format::R16G16_SINT:
            return 0;
        case gal_texture_format::R16G16_SFLOAT:
            return 0;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0;
        case gal_texture_format::R32_UINT:
            return 0;
        case gal_texture_format::R32_SINT:
            return 0;
        case gal_texture_format::R32_SFLOAT:
            return 0;
        case gal_texture_format::A2R10G10B10_UNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_UINT:
            return 10;
        case gal_texture_format::A2R10G10B10_SNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_SINT:
            return 10;
        case gal_texture_format::A2B10G10R10_UNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_UINT:
            return 10;
        case gal_texture_format::A2B10G10R10_SNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_SINT:
            return 10;
        case gal_texture_format::R10G10B10A2_UNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_UINT:
            return 10;
        case gal_texture_format::R10G10B10A2_SNORM:
            return 10;
        case gal_texture_format::R10G10B10A2_SINT:
            return 10;
        case gal_texture_format::B10G10R10A2_UNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_UINT:
            return 10;
        case gal_texture_format::B10G10R10A2_SNORM:
            return 10;
        case gal_texture_format::B10G10R10A2_SINT:
            return 10;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 11;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 9;
        case gal_texture_format::R16G16B16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16_UINT:
            return 16;
        case gal_texture_format::R16G16B16_SINT:
            return 16;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_UINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 16;
        case gal_texture_format::R32G32_UINT:
            return 0;
        case gal_texture_format::R32G32_SINT:
            return 0;
        case gal_texture_format::R32G32_SFLOAT:
            return 0;
        case gal_texture_format::R32G32B32_UINT:
            return 32;
        case gal_texture_format::R32G32B32_SINT:
            return 32;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 32;
        case gal_texture_format::R32G32B32A32_UINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 32;
        case gal_texture_format::R64_UINT:
            return 0;
        case gal_texture_format::R64_SINT:
            return 0;
        case gal_texture_format::R64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64_UINT:
            return 0;
        case gal_texture_format::R64G64_SINT:
            return 0;
        case gal_texture_format::R64G64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64B64_UINT:
            return 64;
        case gal_texture_format::R64G64B64_SINT:
            return 64;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 64;
        case gal_texture_format::R64G64B64A64_UINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 64;
        case gal_texture_format::D16_UNORM:
            return 0;
        case gal_texture_format::X8_D24_UNORM:
            return 0;
        case gal_texture_format::D32_SFLOAT:
            return 0;
        case gal_texture_format::S8_UINT:
            return 0;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 0;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 0;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 0;
        case gal_texture_format::DXBC1_RGB_UNORM:
            return 5;
        case gal_texture_format::DXBC1_RGB_SRGB:
            return 5;
        case gal_texture_format::DXBC1_RGBA_UNORM:
            return 5;
        case gal_texture_format::DXBC1_RGBA_SRGB:
            return 5;
        case gal_texture_format::DXBC2_UNORM:
            return 5;
        case gal_texture_format::DXBC2_SRGB:
            return 5;
        case gal_texture_format::DXBC3_UNORM:
            return 5;
        case gal_texture_format::DXBC3_SRGB:
            return 5;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 16;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 16;
        case gal_texture_format::ETC2_EAC_R11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return 11;
        case gal_texture_format::CLUT_P4:
            return 0;
        case gal_texture_format::CLUT_P4A4:
            return 0;
        case gal_texture_format::CLUT_P8:
            return 0;
        case gal_texture_format::CLUT_P8A8:
            return 0;
        default:
            return 8;
        }
    } else if (channel == 3) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0;
        case gal_texture_format::R1_UNORM:
            return 0;
        case gal_texture_format::R2_UNORM:
            return 0;
        case gal_texture_format::R4_UNORM:
            return 0;
        case gal_texture_format::R4G4_UNORM:
            return 0;
        case gal_texture_format::G4R4_UNORM:
            return 0;
        case gal_texture_format::A8_UNORM:
            return 0;
        case gal_texture_format::R8_UNORM:
            return 0;
        case gal_texture_format::R8_SNORM:
            return 0;
        case gal_texture_format::R8_UINT:
            return 0;
        case gal_texture_format::R8_SINT:
            return 0;
        case gal_texture_format::R8_SRGB:
            return 0;
        case gal_texture_format::B2G3R3_UNORM:
            return 0;
        case gal_texture_format::R4G4B4A4_UNORM:
            return 4;
        case gal_texture_format::R4G4B4X4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4A4_UNORM:
            return 4;
        case gal_texture_format::B4G4R4X4_UNORM:
            return 4;
        case gal_texture_format::A4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::X4R4G4B4_UNORM:
            return 4;
        case gal_texture_format::A4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::X4B4G4R4_UNORM:
            return 4;
        case gal_texture_format::R5G6B5_UNORM:
            return 0;
        case gal_texture_format::B5G6R5_UNORM:
            return 0;
        case gal_texture_format::R5G5B5A1_UNORM:
            return 1;
        case gal_texture_format::B5G5R5A1_UNORM:
            return 1;
        case gal_texture_format::A1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::A1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::R5G5B5X1_UNORM:
            return 1;
        case gal_texture_format::B5G5R5X1_UNORM:
            return 1;
        case gal_texture_format::X1R5G5B5_UNORM:
            return 5;
        case gal_texture_format::X1B5G5R5_UNORM:
            return 5;
        case gal_texture_format::R8G8_UNORM:
            return 0;
        case gal_texture_format::R8G8_SNORM:
            return 0;
        case gal_texture_format::G8R8_UNORM:
            return 0;
        case gal_texture_format::G8R8_SNORM:
            return 0;
        case gal_texture_format::R8G8_UINT:
            return 0;
        case gal_texture_format::R8G8_SINT:
            return 0;
        case gal_texture_format::R8G8_SRGB:
            return 0;
        case gal_texture_format::R16_UNORM:
            return 0;
        case gal_texture_format::R16_SNORM:
            return 0;
        case gal_texture_format::R16_UINT:
            return 0;
        case gal_texture_format::R16_SINT:
            return 0;
        case gal_texture_format::R16_SFLOAT:
            return 0;
        case gal_texture_format::R16_SBFLOAT:
            return 0;
        case gal_texture_format::R8G8B8_UNORM:
            return 0;
        case gal_texture_format::R8G8B8_SNORM:
            return 0;
        case gal_texture_format::R8G8B8_UINT:
            return 0;
        case gal_texture_format::R8G8B8_SINT:
            return 0;
        case gal_texture_format::R8G8B8_SRGB:
            return 0;
        case gal_texture_format::B8G8R8_UNORM:
            return 0;
        case gal_texture_format::B8G8R8_SNORM:
            return 0;
        case gal_texture_format::B8G8R8_UINT:
            return 0;
        case gal_texture_format::B8G8R8_SINT:
            return 0;
        case gal_texture_format::B8G8R8_SRGB:
            return 0;
        case gal_texture_format::R16G16_UNORM:
            return 0;
        case gal_texture_format::G16R16_UNORM:
            return 0;
        case gal_texture_format::R16G16_SNORM:
            return 0;
        case gal_texture_format::G16R16_SNORM:
            return 0;
        case gal_texture_format::R16G16_UINT:
            return 0;
        case gal_texture_format::R16G16_SINT:
            return 0;
        case gal_texture_format::R16G16_SFLOAT:
            return 0;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0;
        case gal_texture_format::R32_UINT:
            return 0;
        case gal_texture_format::R32_SINT:
            return 0;
        case gal_texture_format::R32_SFLOAT:
            return 0;
        case gal_texture_format::A2R10G10B10_UNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_UINT:
            return 10;
        case gal_texture_format::A2R10G10B10_SNORM:
            return 10;
        case gal_texture_format::A2R10G10B10_SINT:
            return 10;
        case gal_texture_format::A2B10G10R10_UNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_UINT:
            return 10;
        case gal_texture_format::A2B10G10R10_SNORM:
            return 10;
        case gal_texture_format::A2B10G10R10_SINT:
            return 10;
        case gal_texture_format::R10G10B10A2_UNORM:
            return 2;
        case gal_texture_format::R10G10B10A2_UINT:
            return 2;
        case gal_texture_format::R10G10B10A2_SNORM:
            return 2;
        case gal_texture_format::R10G10B10A2_SINT:
            return 2;
        case gal_texture_format::B10G10R10A2_UNORM:
            return 2;
        case gal_texture_format::B10G10R10A2_UINT:
            return 2;
        case gal_texture_format::B10G10R10A2_SNORM:
            return 2;
        case gal_texture_format::B10G10R10A2_SINT:
            return 2;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 0;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 9;
        case gal_texture_format::R16G16B16_UNORM:
            return 0;
        case gal_texture_format::R16G16B16_SNORM:
            return 0;
        case gal_texture_format::R16G16B16_UINT:
            return 0;
        case gal_texture_format::R16G16B16_SINT:
            return 0;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 0;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 0;
        case gal_texture_format::R16G16B16A16_UNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_SNORM:
            return 16;
        case gal_texture_format::R16G16B16A16_UINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SINT:
            return 16;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 16;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 16;
        case gal_texture_format::R32G32_UINT:
            return 0;
        case gal_texture_format::R32G32_SINT:
            return 0;
        case gal_texture_format::R32G32_SFLOAT:
            return 0;
        case gal_texture_format::R32G32B32_UINT:
            return 0;
        case gal_texture_format::R32G32B32_SINT:
            return 0;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 0;
        case gal_texture_format::R32G32B32A32_UINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SINT:
            return 32;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 32;
        case gal_texture_format::R64_UINT:
            return 0;
        case gal_texture_format::R64_SINT:
            return 0;
        case gal_texture_format::R64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64_UINT:
            return 0;
        case gal_texture_format::R64G64_SINT:
            return 0;
        case gal_texture_format::R64G64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64B64_UINT:
            return 0;
        case gal_texture_format::R64G64B64_SINT:
            return 0;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 0;
        case gal_texture_format::R64G64B64A64_UINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SINT:
            return 64;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 64;
        case gal_texture_format::D16_UNORM:
            return 0;
        case gal_texture_format::X8_D24_UNORM:
            return 0;
        case gal_texture_format::D32_SFLOAT:
            return 0;
        case gal_texture_format::S8_UINT:
            return 0;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 0;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 0;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 0;
        case gal_texture_format::DXBC1_RGB_UNORM:
            return 0;
        case gal_texture_format::DXBC1_RGB_SRGB:
            return 0;
        case gal_texture_format::DXBC1_RGBA_UNORM:
            return 0;
        case gal_texture_format::DXBC1_RGBA_SRGB:
            return 0;
        case gal_texture_format::DXBC2_UNORM:
            return 4;
        case gal_texture_format::DXBC2_SRGB:
            return 4;
        case gal_texture_format::DXBC3_UNORM:
            return 4;
        case gal_texture_format::DXBC3_SRGB:
            return 4;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 16;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 16;
        case gal_texture_format::DXBC7_UNORM:
            return 4;
        case gal_texture_format::DXBC7_SRGB:
            return 4;
        case gal_texture_format::ETC2_EAC_R11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
            return 11;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return 11;
        case gal_texture_format::CLUT_P4:
            return 0;
        case gal_texture_format::CLUT_P4A4:
            return 0;
        case gal_texture_format::CLUT_P8:
            return 0;
        case gal_texture_format::CLUT_P8A8:
            return 0;
        default:
            return 8;
        }
    }
    //assert(false);
    return 0;
}

constexpr double MinAtPhysical(gal_texture_format fmt, uint32_t channel) {
    if (gal_tf_is_homogenous(fmt) || channel == 0) {
        switch (fmt) {
        case gal_texture_format::R8_SNORM:
            return -1.000000;
        case gal_texture_format::R8_SINT:
            return -128.000000;
        case gal_texture_format::R8G8_SNORM:
            return -1.000000;
        case gal_texture_format::G8R8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8_SINT:
            return -128.000000;
        case gal_texture_format::R16_SNORM:
            return -1.000000;
        case gal_texture_format::R16_SINT:
            return -32768.000000;
        case gal_texture_format::R16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R8G8B8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8_SINT:
            return -128.000000;
        case gal_texture_format::R8G8B8A8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8A8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return -128.000000;
        case gal_texture_format::R16G16_SNORM:
            return -1.000000;
        case gal_texture_format::G16R16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::A2R10G10B10_SNORM:
            return -1.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return -2.000000;
        case gal_texture_format::A2B10G10R10_SNORM:
            return -1.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return -2.000000;
        case gal_texture_format::R10G10B10A2_SNORM:
            return -1.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return -512.000000;
        case gal_texture_format::B10G10R10A2_SNORM:
            return -1.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return -512.000000;
        case gal_texture_format::R16G16B16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R16G16B16A16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R32G32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::D32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::DXBC4_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC5_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return -1.000000;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return -1.000000;
        default:
            return 0.000000;
        }
    } else if (channel == 1) {
        switch (fmt) {
        case gal_texture_format::R8G8_SNORM:
            return -1.000000;
        case gal_texture_format::G8R8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8_SINT:
            return -128.000000;
        case gal_texture_format::R8G8B8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8_SINT:
            return -128.000000;
        case gal_texture_format::R8G8B8A8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8A8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return -128.000000;
        case gal_texture_format::R16G16_SNORM:
            return -1.000000;
        case gal_texture_format::G16R16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::A2R10G10B10_SNORM:
            return -1.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return -512.000000;
        case gal_texture_format::A2B10G10R10_SNORM:
            return -1.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return -512.000000;
        case gal_texture_format::R10G10B10A2_SNORM:
            return -1.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return -512.000000;
        case gal_texture_format::B10G10R10A2_SNORM:
            return -1.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return -512.000000;
        case gal_texture_format::R16G16B16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R16G16B16A16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R32G32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64G64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::DXBC4_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC5_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return -1.000000;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return -1.000000;
        default:
            return 0.000000;
        }
    } else if (channel == 2) {
        switch (fmt) {
        case gal_texture_format::R8G8B8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8_SINT:
            return -128.000000;
        case gal_texture_format::R8G8B8A8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8A8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return -128.000000;
        case gal_texture_format::A2R10G10B10_SNORM:
            return -1.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return -512.000000;
        case gal_texture_format::A2B10G10R10_SNORM:
            return -1.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return -512.000000;
        case gal_texture_format::R10G10B10A2_SNORM:
            return -1.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return -512.000000;
        case gal_texture_format::B10G10R10A2_SNORM:
            return -1.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return -512.000000;
        case gal_texture_format::R16G16B16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R16G16B16A16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R32G32B32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64G64B64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::DXBC4_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC5_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return -1.000000;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return -1.000000;
        default:
            return 0.000000;
        }
    } else if (channel == 3) {
        switch (fmt) {
        case gal_texture_format::R8G8B8A8_SNORM:
            return -1.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return -128.000000;
        case gal_texture_format::B8G8R8A8_SNORM:
            return -1.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return -128.000000;
        case gal_texture_format::A2R10G10B10_SNORM:
            return -1.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return -512.000000;
        case gal_texture_format::A2B10G10R10_SNORM:
            return -1.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return -512.000000;
        case gal_texture_format::R10G10B10A2_SNORM:
            return -1.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return -2.000000;
        case gal_texture_format::B10G10R10A2_SNORM:
            return -1.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return -2.000000;
        case gal_texture_format::R16G16B16A16_SNORM:
            return -1.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return -32768.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return -65504.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return -2147483648.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return -9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::DXBC4_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC5_SNORM:
            return -1.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return -340282346638528859811704183484516925440.000000;
        case gal_texture_format::ETC2_EAC_R11_SNORM:
            return -1.000000;
        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
            return -1.000000;
        default:
            return 0.000000;
        }
    }
    //ASSERT(false);
    return 0.0;
}

constexpr double MaxAtPhysical(gal_texture_format fmt, uint32_t channel) {
    if (gal_tf_is_homogenous(fmt) || channel == 0) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0.000000;
        case gal_texture_format::R8_UINT:
            return 255.000000;
        case gal_texture_format::R8_SINT:
            return 127.000000;
        case gal_texture_format::X4R4G4B4_UNORM:
            return 15.000000;
        case gal_texture_format::X4B4G4R4_UNORM:
            return 15.000000;
        case gal_texture_format::R8G8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8_SINT:
            return 127.000000;
        case gal_texture_format::R16_UINT:
            return 65535.000000;
        case gal_texture_format::R16_SINT:
            return 32767.000000;
        case gal_texture_format::R16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R8G8B8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8_SINT:
            return 127.000000;
        case gal_texture_format::R8G8B8A8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8A8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return 127.000000;
        case gal_texture_format::R16G16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::A2R10G10B10_UINT:
            return 3.000000;
        case gal_texture_format::A2B10G10R10_UINT:
            return 3.000000;
        case gal_texture_format::R10G10B10A2_UINT:
            return 1023.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G10R10A2_UINT:
            return 1023.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 65000.000000;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 31.000000;
        case gal_texture_format::R16G16B16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16B16A16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::X8_D24_UNORM:
            return 255.000000;
        case gal_texture_format::D32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::S8_UINT:
            return 255.000000;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 131008.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 65504.000000;
        default:
            return 1.000000;
        }
    } else if (channel == 1) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0.000000;
        case gal_texture_format::R1_UNORM:
            return 0.000000;
        case gal_texture_format::R2_UNORM:
            return 0.000000;
        case gal_texture_format::R4_UNORM:
            return 0.000000;
        case gal_texture_format::A8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_SNORM:
            return 0.000000;
        case gal_texture_format::R8_UINT:
            return 0.000000;
        case gal_texture_format::R8_SINT:
            return 0.000000;
        case gal_texture_format::R8_SRGB:
            return 0.000000;
        case gal_texture_format::R8G8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8_SINT:
            return 127.000000;
        case gal_texture_format::R16_UNORM:
            return 0.000000;
        case gal_texture_format::R16_SNORM:
            return 0.000000;
        case gal_texture_format::R16_UINT:
            return 0.000000;
        case gal_texture_format::R16_SINT:
            return 0.000000;
        case gal_texture_format::R16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R8G8B8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8_SINT:
            return 127.000000;
        case gal_texture_format::R8G8B8A8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8A8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return 127.000000;
        case gal_texture_format::R16G16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32_UINT:
            return 0.000000;
        case gal_texture_format::R32_SINT:
            return 0.000000;
        case gal_texture_format::R32_SFLOAT:
            return 0.000000;
        case gal_texture_format::A2R10G10B10_UINT:
            return 1023.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return 511.000000;
        case gal_texture_format::A2B10G10R10_UINT:
            return 1023.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return 511.000000;
        case gal_texture_format::R10G10B10A2_UINT:
            return 1023.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G10R10A2_UINT:
            return 1023.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 65500.000000;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 65408.000000;
        case gal_texture_format::R16G16B16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16B16A16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64_UINT:
            return 0.000000;
        case gal_texture_format::R64_SINT:
            return 0.000000;
        case gal_texture_format::R64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::D16_UNORM:
            return 0.000000;
        case gal_texture_format::D32_SFLOAT:
            return 0.000000;
        case gal_texture_format::S8_UINT:
            return 0.000000;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 255.000000;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 255.000000;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 255.000000;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 131008.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 65504.000000;
        default:
            return 1.000000;
        }
    } else if (channel == 2) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0.000000;
        case gal_texture_format::R1_UNORM:
            return 0.000000;
        case gal_texture_format::R2_UNORM:
            return 0.000000;
        case gal_texture_format::R4_UNORM:
            return 0.000000;
        case gal_texture_format::R4G4_UNORM:
            return 0.000000;
        case gal_texture_format::G4R4_UNORM:
            return 0.000000;
        case gal_texture_format::A8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_SNORM:
            return 0.000000;
        case gal_texture_format::R8_UINT:
            return 0.000000;
        case gal_texture_format::R8_SINT:
            return 0.000000;
        case gal_texture_format::R8_SRGB:
            return 0.000000;
        case gal_texture_format::R8G8_UNORM:
            return 0.000000;
        case gal_texture_format::R8G8_SNORM:
            return 0.000000;
        case gal_texture_format::G8R8_UNORM:
            return 0.000000;
        case gal_texture_format::G8R8_SNORM:
            return 0.000000;
        case gal_texture_format::R8G8_UINT:
            return 0.000000;
        case gal_texture_format::R8G8_SINT:
            return 0.000000;
        case gal_texture_format::R8G8_SRGB:
            return 0.000000;
        case gal_texture_format::R16_UNORM:
            return 0.000000;
        case gal_texture_format::R16_SNORM:
            return 0.000000;
        case gal_texture_format::R16_UINT:
            return 0.000000;
        case gal_texture_format::R16_SINT:
            return 0.000000;
        case gal_texture_format::R16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R8G8B8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8_SINT:
            return 127.000000;
        case gal_texture_format::R8G8B8A8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8A8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return 127.000000;
        case gal_texture_format::R16G16_UNORM:
            return 0.000000;
        case gal_texture_format::G16R16_UNORM:
            return 0.000000;
        case gal_texture_format::R16G16_SNORM:
            return 0.000000;
        case gal_texture_format::G16R16_SNORM:
            return 0.000000;
        case gal_texture_format::R16G16_UINT:
            return 0.000000;
        case gal_texture_format::R16G16_SINT:
            return 0.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32_UINT:
            return 0.000000;
        case gal_texture_format::R32_SINT:
            return 0.000000;
        case gal_texture_format::R32_SFLOAT:
            return 0.000000;
        case gal_texture_format::A2R10G10B10_UINT:
            return 1023.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return 511.000000;
        case gal_texture_format::A2B10G10R10_UINT:
            return 1023.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return 511.000000;
        case gal_texture_format::R10G10B10A2_UINT:
            return 1023.000000;
        case gal_texture_format::R10G10B10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G10R10A2_UINT:
            return 1023.000000;
        case gal_texture_format::B10G10R10A2_SINT:
            return 511.000000;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 65500.000000;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 65408.000000;
        case gal_texture_format::R16G16B16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16B16A16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32_UINT:
            return 0.000000;
        case gal_texture_format::R32G32_SINT:
            return 0.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32B32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R32G32B32A32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64_UINT:
            return 0.000000;
        case gal_texture_format::R64_SINT:
            return 0.000000;
        case gal_texture_format::R64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64_UINT:
            return 0.000000;
        case gal_texture_format::R64G64_SINT:
            return 0.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64B64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::R64G64B64A64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::D16_UNORM:
            return 0.000000;
        case gal_texture_format::X8_D24_UNORM:
            return 0.000000;
        case gal_texture_format::D32_SFLOAT:
            return 0.000000;
        case gal_texture_format::S8_UINT:
            return 0.000000;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 0.000000;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 0.000000;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 0.000000;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 131008.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 65504.000000;
        default:
            return 1.000000;
        }
    } else if (channel == 3) {
        switch (fmt) {
        case gal_texture_format::UNDEFINED:
            return 0.000000;
        case gal_texture_format::R1_UNORM:
            return 0.000000;
        case gal_texture_format::R2_UNORM:
            return 0.000000;
        case gal_texture_format::R4_UNORM:
            return 0.000000;
        case gal_texture_format::R4G4_UNORM:
            return 0.000000;
        case gal_texture_format::G4R4_UNORM:
            return 0.000000;
        case gal_texture_format::A8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_UNORM:
            return 0.000000;
        case gal_texture_format::R8_SNORM:
            return 0.000000;
        case gal_texture_format::R8_UINT:
            return 0.000000;
        case gal_texture_format::R8_SINT:
            return 0.000000;
        case gal_texture_format::R8_SRGB:
            return 0.000000;
        case gal_texture_format::B2G3R3_UNORM:
            return 0.000000;
        case gal_texture_format::R5G6B5_UNORM:
            return 0.000000;
        case gal_texture_format::B5G6R5_UNORM:
            return 0.000000;
        case gal_texture_format::R8G8_UNORM:
            return 0.000000;
        case gal_texture_format::R8G8_SNORM:
            return 0.000000;
        case gal_texture_format::G8R8_UNORM:
            return 0.000000;
        case gal_texture_format::G8R8_SNORM:
            return 0.000000;
        case gal_texture_format::R8G8_UINT:
            return 0.000000;
        case gal_texture_format::R8G8_SINT:
            return 0.000000;
        case gal_texture_format::R8G8_SRGB:
            return 0.000000;
        case gal_texture_format::R16_UNORM:
            return 0.000000;
        case gal_texture_format::R16_SNORM:
            return 0.000000;
        case gal_texture_format::R16_UINT:
            return 0.000000;
        case gal_texture_format::R16_SINT:
            return 0.000000;
        case gal_texture_format::R16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R8G8B8_UNORM:
            return 0.000000;
        case gal_texture_format::R8G8B8_SNORM:
            return 0.000000;
        case gal_texture_format::R8G8B8_UINT:
            return 0.000000;
        case gal_texture_format::R8G8B8_SINT:
            return 0.000000;
        case gal_texture_format::R8G8B8_SRGB:
            return 0.000000;
        case gal_texture_format::B8G8R8_UNORM:
            return 0.000000;
        case gal_texture_format::B8G8R8_SNORM:
            return 0.000000;
        case gal_texture_format::B8G8R8_UINT:
            return 0.000000;
        case gal_texture_format::B8G8R8_SINT:
            return 0.000000;
        case gal_texture_format::B8G8R8_SRGB:
            return 0.000000;
        case gal_texture_format::R8G8B8A8_UINT:
            return 255.000000;
        case gal_texture_format::R8G8B8A8_SINT:
            return 127.000000;
        case gal_texture_format::B8G8R8A8_UINT:
            return 255.000000;
        case gal_texture_format::B8G8R8A8_SINT:
            return 127.000000;
        case gal_texture_format::R16G16_UNORM:
            return 0.000000;
        case gal_texture_format::G16R16_UNORM:
            return 0.000000;
        case gal_texture_format::R16G16_SNORM:
            return 0.000000;
        case gal_texture_format::G16R16_SNORM:
            return 0.000000;
        case gal_texture_format::R16G16_UINT:
            return 0.000000;
        case gal_texture_format::R16G16_SINT:
            return 0.000000;
        case gal_texture_format::R16G16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32_UINT:
            return 0.000000;
        case gal_texture_format::R32_SINT:
            return 0.000000;
        case gal_texture_format::R32_SFLOAT:
            return 0.000000;
        case gal_texture_format::A2R10G10B10_UINT:
            return 1023.000000;
        case gal_texture_format::A2R10G10B10_SINT:
            return 511.000000;
        case gal_texture_format::A2B10G10R10_UINT:
            return 1023.000000;
        case gal_texture_format::A2B10G10R10_SINT:
            return 511.000000;
        case gal_texture_format::R10G10B10A2_UINT:
            return 3.000000;
        case gal_texture_format::B10G10R10A2_UINT:
            return 3.000000;
        case gal_texture_format::B10G11R11_UFLOAT:
            return 0.000000;
        case gal_texture_format::E5B9G9R9_UFLOAT:
            return 65408.000000;
        case gal_texture_format::R16G16B16_UNORM:
            return 0.000000;
        case gal_texture_format::R16G16B16_SNORM:
            return 0.000000;
        case gal_texture_format::R16G16B16_UINT:
            return 0.000000;
        case gal_texture_format::R16G16B16_SINT:
            return 0.000000;
        case gal_texture_format::R16G16B16_SFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16B16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R16G16B16A16_UINT:
            return 65535.000000;
        case gal_texture_format::R16G16B16A16_SINT:
            return 32767.000000;
        case gal_texture_format::R16G16B16A16_SFLOAT:
            return 65504.000000;
        case gal_texture_format::R16G16B16A16_SBFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32_UINT:
            return 0.000000;
        case gal_texture_format::R32G32_SINT:
            return 0.000000;
        case gal_texture_format::R32G32_SFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32B32_UINT:
            return 0.000000;
        case gal_texture_format::R32G32B32_SINT:
            return 0.000000;
        case gal_texture_format::R32G32B32_SFLOAT:
            return 0.000000;
        case gal_texture_format::R32G32B32A32_UINT:
            return 4294967295.000000;
        case gal_texture_format::R32G32B32A32_SINT:
            return 2147483647.000000;
        case gal_texture_format::R32G32B32A32_SFLOAT:
            return 340282346638528859811704183484516925440.000000;
        case gal_texture_format::R64_UINT:
            return 0.000000;
        case gal_texture_format::R64_SINT:
            return 0.000000;
        case gal_texture_format::R64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64_UINT:
            return 0.000000;
        case gal_texture_format::R64G64_SINT:
            return 0.000000;
        case gal_texture_format::R64G64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64B64_UINT:
            return 0.000000;
        case gal_texture_format::R64G64B64_SINT:
            return 0.000000;
        case gal_texture_format::R64G64B64_SFLOAT:
            return 0.000000;
        case gal_texture_format::R64G64B64A64_UINT:
            return 18446744073709551616.000000;
        case gal_texture_format::R64G64B64A64_SINT:
            return 9223372036854775808.000000;
        case gal_texture_format::R64G64B64A64_SFLOAT:
            return 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000;
        case gal_texture_format::D16_UNORM:
            return 0.000000;
        case gal_texture_format::X8_D24_UNORM:
            return 0.000000;
        case gal_texture_format::D32_SFLOAT:
            return 0.000000;
        case gal_texture_format::S8_UINT:
            return 0.000000;
        case gal_texture_format::D16_UNORM_S8_UINT:
            return 0.000000;
        case gal_texture_format::D24_UNORM_S8_UINT:
            return 0.000000;
        case gal_texture_format::D32_SFLOAT_S8_UINT:
            return 0.000000;
        case gal_texture_format::DXBC6H_UFLOAT:
            return 131008.000000;
        case gal_texture_format::DXBC6H_SFLOAT:
            return 65504.000000;
        default:
            return 1.000000;
        }
    }
    //ASSERT(false);
    return 0.0;
}

//LogicalChannel PhysicalChannelToLogical(gal_texture_format  fmt,
//                                                                                         int8_t  channel) {
//    ASSERT(channel != PC_CONST_0);
//    ASSERT(channel != PC_CONST_1);
//    if (channel == PC_0) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B4G4R4A4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B5G5R5A1_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B2G3R3A8_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8A8_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8A8_SNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8A8_UINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8A8_SINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8A8_SRGB:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::B10G10R10A2_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B10G10R10A2_UINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B10G10R10A2_SNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B10G10R10A2_SINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::LC_Green;
//        default:
//            return gal_texture_format::LC_Red;
//        }
//    } else if (channel == PC_1) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::LC_Alpha;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::LC_Alpha;
//        default:
//            return gal_texture_format::LC_Green;
//        }
//    } else if (channel == PC_2) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R4G4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B4G4R4A4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B5G5R5A1_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::B2G3R3A8_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R8G8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8G8_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8G8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8G8_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8G8_SRGB:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8A8_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8A8_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8A8_UINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8A8_SINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8A8_SRGB:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R16G16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16G16_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16G16_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16G16_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16G16_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R16G16_SBFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::B10G10R10A2_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B10G10R10A2_UINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B10G10R10A2_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B10G10R10A2_SINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::LC_Green;
//        case gal_texture_format::R32G32_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32G32_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R32G32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64G64_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64G64_SINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R64G64_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D16_UNORM_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D24_UNORM_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D32_SFLOAT_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC5_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC5_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::LC_0;
//        default:
//            return gal_texture_format::LC_Blue;
//        }
//    } else if (channel == PC_3) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R4G4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R4G4B4X4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R5G6B5_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::R5G5B5X1_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R8G8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R8G8B8X8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16_SBFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::LC_Blue;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::LC_Red;
//        case gal_texture_format::R16G16B16_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16B16_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16B16_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16B16_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16B16_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R16G16B16_SBFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32B32_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32B32_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R32G32B32_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64B64_UINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64B64_SINT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::R64G64B64_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D16_UNORM_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D24_UNORM_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::D32_SFLOAT_S8_UINT:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::DXBC1_RGB_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC1_RGB_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC5_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC5_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC6H_UFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::DXBC6H_SFLOAT:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_R8G8B8_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_R8G8B8_SRGB:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
//            return gal_texture_format::LC_1;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::LC_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::LC_0;
//        default:
//            return gal_texture_format::LC_Alpha;
//        }
//    }
//    ASSERT(false);
//    return gal_texture_format::LC_0;
//}

//int8_t LogicalChannelToPhysical(gal_texture_format  fmt,
//                                                                 LogicalChannel  channel) {
//    ASSERT(channel != LC_0);
//    ASSERT(channel != LC_1);
//    if (channel == LC_Red) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B4G4R4A4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B5G5R5A1_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::B2G3R3A8_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8A8_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8A8_SNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8A8_UINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8A8_SINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8A8_SRGB:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::B10G10R10A2_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B10G10R10A2_UINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B10G10R10A2_SNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B10G10R10A2_SINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        default:
//            return gal_texture_format::PC_0;
//        }
//    } else if (channel == LC_Green) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::PC_2;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::PC_CONST_0;
//        default:
//            return gal_texture_format::PC_1;
//        }
//    } else if (channel == LC_Blue) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R4G4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B4G4R4A4_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B5G5R5A1_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::B2G3R3A8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R8G8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8G8_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8G8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8G8_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R8G8_SRGB:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8A8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8A8_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8A8_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8A8_SINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8A8_SRGB:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R16G16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16G16_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16G16_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16G16_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16G16_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R16G16_SBFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::PC_3;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::B10G10R10A2_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B10G10R10A2_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B10G10R10A2_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B10G10R10A2_SINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::R32G32_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32G32_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R32G32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64G64_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64G64_SINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R64G64_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D16_UNORM_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D24_UNORM_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D32_SFLOAT_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC5_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC5_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::PC_CONST_0;
//        default:
//            return gal_texture_format::PC_2;
//        }
//    } else if (channel == LC_Alpha) {
//        switch (fmt) {
//        case gal_texture_format::UNDEFINED:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::R1_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R2_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R4G4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::G4R4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::A8_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B2G3R3_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R4G4B4X4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B4G4R4X4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::A4R4G4B4_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::X4R4G4B4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::A4B4G4R4_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::X4B4G4R4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R5G6B5_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B5G6R5_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::A1B5G5R5_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A1R5G5B5_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R5G5B5X1_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B5G5R5X1_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::X1R5G5B5_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::X1B5G5R5_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::G8R8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::G8R8_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16_SBFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R8G8B8X8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::B8G8R8X8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::G16R16_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::G16R16_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16_SBFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::A2R10G10B10_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2R10G10B10_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2R10G10B10_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2R10G10B10_SINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2B10G10R10_UNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2B10G10R10_UINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2B10G10R10_SNORM:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::A2B10G10R10_SINT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::B10G11R11_UFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::E5B9G9R9_UFLOAT:
//            return gal_texture_format::PC_0;
//        case gal_texture_format::R16G16B16_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16B16_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16B16_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16B16_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16B16_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R16G16B16_SBFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32B32_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32B32_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R32G32B32_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64B64_UINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64B64_SINT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::R64G64B64_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::D16_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::X8_D24_UNORM:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D32_SFLOAT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D16_UNORM_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D24_UNORM_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::D32_SFLOAT_S8_UINT:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::DXBC1_RGB_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC1_RGB_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC4_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC4_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC5_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC5_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC6H_UFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::DXBC6H_SFLOAT:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_R8G8B8_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_R8G8B8_SRGB:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_EAC_R11_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_EAC_R11_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_EAC_R11G11_UNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::ETC2_EAC_R11G11_SNORM:
//            return gal_texture_format::PC_CONST_1;
//        case gal_texture_format::CLUT_P4:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P4A4:
//            return gal_texture_format::PC_1;
//        case gal_texture_format::CLUT_P8:
//            return gal_texture_format::PC_CONST_0;
//        case gal_texture_format::CLUT_P8A8:
//            return gal_texture_format::PC_1;
//        default:
//            return gal_texture_format::PC_3;
//        }
//    }
//    ASSERT(false);
//    return gal_texture_format::PC_CONST_0;
//}

constexpr gal_texture_format ToSRGB(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R8_UNORM:
        return gal_texture_format::R8_SRGB;
    case gal_texture_format::R8_SNORM:
        return gal_texture_format::R8_SRGB;
    case gal_texture_format::R8_UINT:
        return gal_texture_format::R8_SRGB;
    case gal_texture_format::R8_SINT:
        return gal_texture_format::R8_SRGB;
    case gal_texture_format::R8_SRGB:
        return gal_texture_format::R8_SRGB;
    case gal_texture_format::R8G8_UNORM:
        return gal_texture_format::R8G8_SRGB;
    case gal_texture_format::R8G8_SNORM:
        return gal_texture_format::R8G8_SRGB;
    case gal_texture_format::R8G8_UINT:
        return gal_texture_format::R8G8_SRGB;
    case gal_texture_format::R8G8_SINT:
        return gal_texture_format::R8G8_SRGB;
    case gal_texture_format::R8G8_SRGB:
        return gal_texture_format::R8G8_SRGB;
    case gal_texture_format::R8G8B8_UNORM:
        return gal_texture_format::R8G8B8_SRGB;
    case gal_texture_format::R8G8B8_SNORM:
        return gal_texture_format::R8G8B8_SRGB;
    case gal_texture_format::R8G8B8_UINT:
        return gal_texture_format::R8G8B8_SRGB;
    case gal_texture_format::R8G8B8_SINT:
        return gal_texture_format::R8G8B8_SRGB;
    case gal_texture_format::R8G8B8_SRGB:
        return gal_texture_format::R8G8B8_SRGB;
    case gal_texture_format::B8G8R8_UNORM:
        return gal_texture_format::B8G8R8_SRGB;
    case gal_texture_format::B8G8R8_SNORM:
        return gal_texture_format::B8G8R8_SRGB;
    case gal_texture_format::B8G8R8_UINT:
        return gal_texture_format::B8G8R8_SRGB;
    case gal_texture_format::B8G8R8_SINT:
        return gal_texture_format::B8G8R8_SRGB;
    case gal_texture_format::B8G8R8_SRGB:
        return gal_texture_format::B8G8R8_SRGB;
    case gal_texture_format::R8G8B8A8_UNORM:
        return gal_texture_format::R8G8B8A8_SRGB;
    case gal_texture_format::R8G8B8A8_SNORM:
        return gal_texture_format::R8G8B8A8_SRGB;
    case gal_texture_format::R8G8B8A8_UINT:
        return gal_texture_format::R8G8B8A8_SRGB;
    case gal_texture_format::R8G8B8A8_SINT:
        return gal_texture_format::R8G8B8A8_SRGB;
    case gal_texture_format::R8G8B8A8_SRGB:
        return gal_texture_format::R8G8B8A8_SRGB;
    case gal_texture_format::B8G8R8A8_UNORM:
        return gal_texture_format::B8G8R8A8_SRGB;
    case gal_texture_format::B8G8R8A8_SNORM:
        return gal_texture_format::B8G8R8A8_SRGB;
    case gal_texture_format::B8G8R8A8_UINT:
        return gal_texture_format::B8G8R8A8_SRGB;
    case gal_texture_format::B8G8R8A8_SINT:
        return gal_texture_format::B8G8R8A8_SRGB;
    case gal_texture_format::B8G8R8A8_SRGB:
        return gal_texture_format::B8G8R8A8_SRGB;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return gal_texture_format::DXBC1_RGB_SRGB;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return gal_texture_format::DXBC1_RGB_SRGB;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return gal_texture_format::DXBC1_RGBA_SRGB;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return gal_texture_format::DXBC1_RGBA_SRGB;
    case gal_texture_format::DXBC2_UNORM:
        return gal_texture_format::DXBC2_SRGB;
    case gal_texture_format::DXBC2_SRGB:
        return gal_texture_format::DXBC2_SRGB;
    case gal_texture_format::DXBC3_UNORM:
        return gal_texture_format::DXBC3_SRGB;
    case gal_texture_format::DXBC3_SRGB:
        return gal_texture_format::DXBC3_SRGB;
    case gal_texture_format::DXBC7_UNORM:
        return gal_texture_format::DXBC7_SRGB;
    case gal_texture_format::DXBC7_SRGB:
        return gal_texture_format::DXBC7_SRGB;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return gal_texture_format::PVRTC1_2BPP_SRGB;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return gal_texture_format::PVRTC1_4BPP_SRGB;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return gal_texture_format::PVRTC2_2BPP_SRGB;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return gal_texture_format::PVRTC2_4BPP_SRGB;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return gal_texture_format::PVRTC1_2BPP_SRGB;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return gal_texture_format::PVRTC1_4BPP_SRGB;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return gal_texture_format::PVRTC2_2BPP_SRGB;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return gal_texture_format::PVRTC2_4BPP_SRGB;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return gal_texture_format::ETC2_R8G8B8_SRGB;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return gal_texture_format::ETC2_R8G8B8_SRGB;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return gal_texture_format::ETC2_R8G8B8A1_SRGB;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return gal_texture_format::ETC2_R8G8B8A1_SRGB;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return gal_texture_format::ETC2_R8G8B8A8_SRGB;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return gal_texture_format::ETC2_R8G8B8A8_SRGB;
    case gal_texture_format::ASTC_4x4_UNORM:
        return gal_texture_format::ASTC_4x4_SRGB;
    case gal_texture_format::ASTC_4x4_SRGB:
        return gal_texture_format::ASTC_4x4_SRGB;
    case gal_texture_format::ASTC_5x4_UNORM:
        return gal_texture_format::ASTC_5x4_SRGB;
    case gal_texture_format::ASTC_5x4_SRGB:
        return gal_texture_format::ASTC_5x4_SRGB;
    case gal_texture_format::ASTC_5x5_UNORM:
        return gal_texture_format::ASTC_5x5_SRGB;
    case gal_texture_format::ASTC_5x5_SRGB:
        return gal_texture_format::ASTC_5x5_SRGB;
    case gal_texture_format::ASTC_6x5_UNORM:
        return gal_texture_format::ASTC_6x5_SRGB;
    case gal_texture_format::ASTC_6x5_SRGB:
        return gal_texture_format::ASTC_6x5_SRGB;
    case gal_texture_format::ASTC_6x6_UNORM:
        return gal_texture_format::ASTC_6x6_SRGB;
    case gal_texture_format::ASTC_6x6_SRGB:
        return gal_texture_format::ASTC_6x6_SRGB;
    case gal_texture_format::ASTC_8x5_UNORM:
        return gal_texture_format::ASTC_8x5_SRGB;
    case gal_texture_format::ASTC_8x5_SRGB:
        return gal_texture_format::ASTC_8x5_SRGB;
    case gal_texture_format::ASTC_8x6_UNORM:
        return gal_texture_format::ASTC_8x6_SRGB;
    case gal_texture_format::ASTC_8x6_SRGB:
        return gal_texture_format::ASTC_8x6_SRGB;
    case gal_texture_format::ASTC_8x8_UNORM:
        return gal_texture_format::ASTC_8x8_SRGB;
    case gal_texture_format::ASTC_8x8_SRGB:
        return gal_texture_format::ASTC_8x8_SRGB;
    case gal_texture_format::ASTC_10x5_UNORM:
        return gal_texture_format::ASTC_10x5_SRGB;
    case gal_texture_format::ASTC_10x5_SRGB:
        return gal_texture_format::ASTC_10x5_SRGB;
    case gal_texture_format::ASTC_10x6_UNORM:
        return gal_texture_format::ASTC_10x6_SRGB;
    case gal_texture_format::ASTC_10x6_SRGB:
        return gal_texture_format::ASTC_10x6_SRGB;
    case gal_texture_format::ASTC_10x8_UNORM:
        return gal_texture_format::ASTC_10x8_SRGB;
    case gal_texture_format::ASTC_10x8_SRGB:
        return gal_texture_format::ASTC_10x8_SRGB;
    case gal_texture_format::ASTC_10x10_UNORM:
        return gal_texture_format::ASTC_10x10_SRGB;
    case gal_texture_format::ASTC_10x10_SRGB:
        return gal_texture_format::ASTC_10x10_SRGB;
    case gal_texture_format::ASTC_12x10_UNORM:
        return gal_texture_format::ASTC_12x10_SRGB;
    case gal_texture_format::ASTC_12x10_SRGB:
        return gal_texture_format::ASTC_12x10_SRGB;
    case gal_texture_format::ASTC_12x12_UNORM:
        return gal_texture_format::ASTC_12x12_SRGB;
    case gal_texture_format::ASTC_12x12_SRGB:
        return gal_texture_format::ASTC_12x12_SRGB;
    default:
        return gal_texture_format::UNDEFINED;
    }
}

constexpr gal_texture_format ToUNORM(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R8_SNORM:
        return gal_texture_format::R8_UNORM;
    case gal_texture_format::R8_UINT:
        return gal_texture_format::R8_UNORM;
    case gal_texture_format::R8_SINT:
        return gal_texture_format::R8_UNORM;
    case gal_texture_format::R8_SRGB:
        return gal_texture_format::R8_UNORM;
    case gal_texture_format::R8G8_SNORM:
        return gal_texture_format::R8G8_UNORM;
    case gal_texture_format::G8R8_SNORM:
        return gal_texture_format::G8R8_UNORM;
    case gal_texture_format::R8G8_UINT:
        return gal_texture_format::R8G8_UNORM;
    case gal_texture_format::R8G8_SINT:
        return gal_texture_format::R8G8_UNORM;
    case gal_texture_format::R8G8_SRGB:
        return gal_texture_format::R8G8_UNORM;
    case gal_texture_format::R16_SNORM:
        return gal_texture_format::R16_UNORM;
    case gal_texture_format::R16_UINT:
        return gal_texture_format::R16_UNORM;
    case gal_texture_format::R16_SINT:
        return gal_texture_format::R16_UNORM;
    case gal_texture_format::R16_SFLOAT:
        return gal_texture_format::R16_UNORM;
    case gal_texture_format::R16_SBFLOAT:
        return gal_texture_format::R16_UNORM;
    case gal_texture_format::R8G8B8_SNORM:
        return gal_texture_format::R8G8B8_UNORM;
    case gal_texture_format::R8G8B8_UINT:
        return gal_texture_format::R8G8B8_UNORM;
    case gal_texture_format::R8G8B8_SINT:
        return gal_texture_format::R8G8B8_UNORM;
    case gal_texture_format::R8G8B8_SRGB:
        return gal_texture_format::R8G8B8_UNORM;
    case gal_texture_format::B8G8R8_SNORM:
        return gal_texture_format::B8G8R8_UNORM;
    case gal_texture_format::B8G8R8_UINT:
        return gal_texture_format::B8G8R8_UNORM;
    case gal_texture_format::B8G8R8_SINT:
        return gal_texture_format::B8G8R8_UNORM;
    case gal_texture_format::B8G8R8_SRGB:
        return gal_texture_format::B8G8R8_UNORM;
    case gal_texture_format::R8G8B8A8_SNORM:
        return gal_texture_format::R8G8B8A8_UNORM;
    case gal_texture_format::R8G8B8A8_UINT:
        return gal_texture_format::R8G8B8A8_UNORM;
    case gal_texture_format::R8G8B8A8_SINT:
        return gal_texture_format::R8G8B8A8_UNORM;
    case gal_texture_format::R8G8B8A8_SRGB:
        return gal_texture_format::R8G8B8A8_UNORM;
    case gal_texture_format::B8G8R8A8_SNORM:
        return gal_texture_format::B8G8R8A8_UNORM;
    case gal_texture_format::B8G8R8A8_UINT:
        return gal_texture_format::B8G8R8A8_UNORM;
    case gal_texture_format::B8G8R8A8_SINT:
        return gal_texture_format::B8G8R8A8_UNORM;
    case gal_texture_format::B8G8R8A8_SRGB:
        return gal_texture_format::B8G8R8A8_UNORM;
    case gal_texture_format::R16G16_SNORM:
        return gal_texture_format::R16G16_UNORM;
    case gal_texture_format::G16R16_SNORM:
        return gal_texture_format::G16R16_UNORM;
    case gal_texture_format::R16G16_UINT:
        return gal_texture_format::R16G16_UNORM;
    case gal_texture_format::R16G16_SINT:
        return gal_texture_format::R16G16_UNORM;
    case gal_texture_format::R16G16_SFLOAT:
        return gal_texture_format::R16G16_UNORM;
    case gal_texture_format::R16G16_SBFLOAT:
        return gal_texture_format::R16G16_UNORM;
    case gal_texture_format::A2R10G10B10_UINT:
        return gal_texture_format::A2R10G10B10_UNORM;
    case gal_texture_format::A2R10G10B10_SNORM:
        return gal_texture_format::A2R10G10B10_UNORM;
    case gal_texture_format::A2R10G10B10_SINT:
        return gal_texture_format::A2R10G10B10_UNORM;
    case gal_texture_format::A2B10G10R10_UINT:
        return gal_texture_format::A2B10G10R10_UNORM;
    case gal_texture_format::A2B10G10R10_SNORM:
        return gal_texture_format::A2B10G10R10_UNORM;
    case gal_texture_format::A2B10G10R10_SINT:
        return gal_texture_format::A2B10G10R10_UNORM;
    case gal_texture_format::R10G10B10A2_UINT:
        return gal_texture_format::R10G10B10A2_UNORM;
    case gal_texture_format::R10G10B10A2_SNORM:
        return gal_texture_format::R10G10B10A2_UNORM;
    case gal_texture_format::R10G10B10A2_SINT:
        return gal_texture_format::R10G10B10A2_UNORM;
    case gal_texture_format::B10G10R10A2_UINT:
        return gal_texture_format::B10G10R10A2_UNORM;
    case gal_texture_format::B10G10R10A2_SNORM:
        return gal_texture_format::B10G10R10A2_UNORM;
    case gal_texture_format::B10G10R10A2_SINT:
        return gal_texture_format::B10G10R10A2_UNORM;
    case gal_texture_format::R16G16B16_SNORM:
        return gal_texture_format::R16G16B16_UNORM;
    case gal_texture_format::R16G16B16_UINT:
        return gal_texture_format::R16G16B16_UNORM;
    case gal_texture_format::R16G16B16_SINT:
        return gal_texture_format::R16G16B16_UNORM;
    case gal_texture_format::R16G16B16_SFLOAT:
        return gal_texture_format::R16G16B16_UNORM;
    case gal_texture_format::R16G16B16_SBFLOAT:
        return gal_texture_format::R16G16B16_UNORM;
    case gal_texture_format::R16G16B16A16_SNORM:
        return gal_texture_format::R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_UINT:
        return gal_texture_format::R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_SINT:
        return gal_texture_format::R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return gal_texture_format::R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return gal_texture_format::R16G16B16A16_UNORM;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return gal_texture_format::DXBC1_RGB_UNORM;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return gal_texture_format::DXBC1_RGBA_UNORM;
    case gal_texture_format::DXBC2_SRGB:
        return gal_texture_format::DXBC2_UNORM;
    case gal_texture_format::DXBC3_SRGB:
        return gal_texture_format::DXBC3_UNORM;
    case gal_texture_format::DXBC4_SNORM:
        return gal_texture_format::DXBC4_UNORM;
    case gal_texture_format::DXBC5_SNORM:
        return gal_texture_format::DXBC5_UNORM;
    case gal_texture_format::DXBC7_SRGB:
        return gal_texture_format::DXBC7_UNORM;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return gal_texture_format::PVRTC1_2BPP_UNORM;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return gal_texture_format::PVRTC1_4BPP_UNORM;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return gal_texture_format::PVRTC2_2BPP_UNORM;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return gal_texture_format::PVRTC2_4BPP_UNORM;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return gal_texture_format::ETC2_R8G8B8_UNORM;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return gal_texture_format::ETC2_R8G8B8A1_UNORM;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return gal_texture_format::ETC2_R8G8B8A8_UNORM;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return gal_texture_format::ETC2_EAC_R11_UNORM;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return gal_texture_format::ETC2_EAC_R11G11_UNORM;
    case gal_texture_format::ASTC_4x4_SRGB:
        return gal_texture_format::ASTC_4x4_UNORM;
    case gal_texture_format::ASTC_5x4_SRGB:
        return gal_texture_format::ASTC_5x4_UNORM;
    case gal_texture_format::ASTC_5x5_SRGB:
        return gal_texture_format::ASTC_5x5_UNORM;
    case gal_texture_format::ASTC_6x5_SRGB:
        return gal_texture_format::ASTC_6x5_UNORM;
    case gal_texture_format::ASTC_6x6_SRGB:
        return gal_texture_format::ASTC_6x6_UNORM;
    case gal_texture_format::ASTC_8x5_SRGB:
        return gal_texture_format::ASTC_8x5_UNORM;
    case gal_texture_format::ASTC_8x6_SRGB:
        return gal_texture_format::ASTC_8x6_UNORM;
    case gal_texture_format::ASTC_8x8_SRGB:
        return gal_texture_format::ASTC_8x8_UNORM;
    case gal_texture_format::ASTC_10x5_SRGB:
        return gal_texture_format::ASTC_10x5_UNORM;
    case gal_texture_format::ASTC_10x6_SRGB:
        return gal_texture_format::ASTC_10x6_UNORM;
    case gal_texture_format::ASTC_10x8_SRGB:
        return gal_texture_format::ASTC_10x8_UNORM;
    case gal_texture_format::ASTC_10x10_SRGB:
        return gal_texture_format::ASTC_10x10_UNORM;
    case gal_texture_format::ASTC_12x10_SRGB:
        return gal_texture_format::ASTC_12x10_UNORM;
    case gal_texture_format::ASTC_12x12_SRGB:
        return gal_texture_format::ASTC_12x12_UNORM;
    case gal_texture_format::R1_UNORM:
    case gal_texture_format::R2_UNORM:
    case gal_texture_format::R4_UNORM:
    case gal_texture_format::R4G4_UNORM:
    case gal_texture_format::G4R4_UNORM:
    case gal_texture_format::A8_UNORM:
    case gal_texture_format::R8_UNORM:
    case gal_texture_format::B2G3R3_UNORM:
    case gal_texture_format::R4G4B4A4_UNORM:
    case gal_texture_format::R4G4B4X4_UNORM:
    case gal_texture_format::B4G4R4A4_UNORM:
    case gal_texture_format::B4G4R4X4_UNORM:
    case gal_texture_format::A4R4G4B4_UNORM:
    case gal_texture_format::X4R4G4B4_UNORM:
    case gal_texture_format::A4B4G4R4_UNORM:
    case gal_texture_format::X4B4G4R4_UNORM:
    case gal_texture_format::R5G6B5_UNORM:
    case gal_texture_format::B5G6R5_UNORM:
    case gal_texture_format::R5G5B5A1_UNORM:
    case gal_texture_format::B5G5R5A1_UNORM:
    case gal_texture_format::A1B5G5R5_UNORM:
    case gal_texture_format::A1R5G5B5_UNORM:
    case gal_texture_format::R5G5B5X1_UNORM:
    case gal_texture_format::B5G5R5X1_UNORM:
    case gal_texture_format::X1R5G5B5_UNORM:
    case gal_texture_format::X1B5G5R5_UNORM:
    case gal_texture_format::B2G3R3A8_UNORM:
    case gal_texture_format::R8G8_UNORM:
    case gal_texture_format::G8R8_UNORM:
    case gal_texture_format::R16_UNORM:
    case gal_texture_format::R8G8B8_UNORM:
    case gal_texture_format::B8G8R8_UNORM:
    case gal_texture_format::R8G8B8A8_UNORM:
    case gal_texture_format::B8G8R8A8_UNORM:
    case gal_texture_format::R8G8B8X8_UNORM:
    case gal_texture_format::B8G8R8X8_UNORM:
    case gal_texture_format::R16G16_UNORM:
    case gal_texture_format::G16R16_UNORM:
    case gal_texture_format::A2R10G10B10_UNORM:
    case gal_texture_format::A2B10G10R10_UNORM:
    case gal_texture_format::R10G10B10A2_UNORM:
    case gal_texture_format::B10G10R10A2_UNORM:
    case gal_texture_format::R16G16B16_UNORM:
    case gal_texture_format::R16G16B16A16_UNORM:
    case gal_texture_format::D16_UNORM:
    case gal_texture_format::X8_D24_UNORM:
    case gal_texture_format::D16_UNORM_S8_UINT:
    case gal_texture_format::D24_UNORM_S8_UINT:
    case gal_texture_format::DXBC1_RGB_UNORM:
    case gal_texture_format::DXBC1_RGBA_UNORM:
    case gal_texture_format::DXBC2_UNORM:
    case gal_texture_format::DXBC3_UNORM:
    case gal_texture_format::DXBC4_UNORM:
    case gal_texture_format::DXBC5_UNORM:
    case gal_texture_format::DXBC7_UNORM:
    case gal_texture_format::PVRTC1_2BPP_UNORM:
    case gal_texture_format::PVRTC1_4BPP_UNORM:
    case gal_texture_format::PVRTC2_2BPP_UNORM:
    case gal_texture_format::PVRTC2_4BPP_UNORM:
    case gal_texture_format::ETC2_R8G8B8_UNORM:
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
    case gal_texture_format::ETC2_EAC_R11_UNORM:
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
    case gal_texture_format::ASTC_4x4_UNORM:
    case gal_texture_format::ASTC_5x4_UNORM:
    case gal_texture_format::ASTC_5x5_UNORM:
    case gal_texture_format::ASTC_6x5_UNORM:
    case gal_texture_format::ASTC_6x6_UNORM:
    case gal_texture_format::ASTC_8x5_UNORM:
    case gal_texture_format::ASTC_8x6_UNORM:
    case gal_texture_format::ASTC_8x8_UNORM:
    case gal_texture_format::ASTC_10x5_UNORM:
    case gal_texture_format::ASTC_10x6_UNORM:
    case gal_texture_format::ASTC_10x8_UNORM:
    case gal_texture_format::ASTC_10x10_UNORM:
    case gal_texture_format::ASTC_12x10_UNORM:
    case gal_texture_format::ASTC_12x12_UNORM:
    case gal_texture_format::R4G4B4A4_UNORM_PACK16:
    case gal_texture_format::B4G4R4A4_UNORM_PACK16:
    case gal_texture_format::R5G6B5_UNORM_PACK16:
    case gal_texture_format::B5G6R5_UNORM_PACK16:
    case gal_texture_format::R5G5B5A1_UNORM_PACK16:
    case gal_texture_format::B5G5R5A1_UNORM_PACK16:
    case gal_texture_format::A1R5G5B5_UNORM_PACK16:
    case gal_texture_format::G16B16G16R16_422_UNORM:
    case gal_texture_format::B16G16R16G16_422_UNORM:
    case gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
    case gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
    case gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
    case gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
    case gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
    case gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
    case gal_texture_format::G8B8G8R8_422_UNORM:
    case gal_texture_format::B8G8R8G8_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
        return fmt;
    default:
        return gal_texture_format::UNDEFINED;
    }
}

constexpr const char *gal_tf_name(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::UNDEFINED:
        return "UNDEFINED";
    case gal_texture_format::R1_UNORM:
        return "R1_UNORM";
    case gal_texture_format::R2_UNORM:
        return "R2_UNORM";
    case gal_texture_format::R4_UNORM:
        return "R4_UNORM";
    case gal_texture_format::R4G4_UNORM:
        return "R4G4_UNORM";
    case gal_texture_format::G4R4_UNORM:
        return "G4R4_UNORM";
    case gal_texture_format::A8_UNORM:
        return "A8_UNORM";
    case gal_texture_format::R8_UNORM:
        return "R8_UNORM";
    case gal_texture_format::R8_SNORM:
        return "R8_SNORM";
    case gal_texture_format::R8_UINT:
        return "R8_UINT";
    case gal_texture_format::R8_SINT:
        return "R8_SINT";
    case gal_texture_format::R8_SRGB:
        return "R8_SRGB";
    case gal_texture_format::B2G3R3_UNORM:
        return "B2G3R3_UNORM";
    case gal_texture_format::R4G4B4A4_UNORM:
        return "R4G4B4A4_UNORM";
    case gal_texture_format::R4G4B4X4_UNORM:
        return "R4G4B4X4_UNORM";
    case gal_texture_format::B4G4R4A4_UNORM:
        return "B4G4R4A4_UNORM";
    case gal_texture_format::B4G4R4X4_UNORM:
        return "B4G4R4X4_UNORM";
    case gal_texture_format::A4R4G4B4_UNORM:
        return "A4R4G4B4_UNORM";
    case gal_texture_format::X4R4G4B4_UNORM:
        return "X4R4G4B4_UNORM";
    case gal_texture_format::A4B4G4R4_UNORM:
        return "A4B4G4R4_UNORM";
    case gal_texture_format::X4B4G4R4_UNORM:
        return "X4B4G4R4_UNORM";
    case gal_texture_format::R5G6B5_UNORM:
        return "R5G6B5_UNORM";
    case gal_texture_format::B5G6R5_UNORM:
        return "B5G6R5_UNORM";
    case gal_texture_format::R5G5B5A1_UNORM:
        return "R5G5B5A1_UNORM";
    case gal_texture_format::B5G5R5A1_UNORM:
        return "B5G5R5A1_UNORM";
    case gal_texture_format::A1B5G5R5_UNORM:
        return "A1B5G5R5_UNORM";
    case gal_texture_format::A1R5G5B5_UNORM:
        return "A1R5G5B5_UNORM";
    case gal_texture_format::R5G5B5X1_UNORM:
        return "R5G5B5X1_UNORM";
    case gal_texture_format::B5G5R5X1_UNORM:
        return "B5G5R5X1_UNORM";
    case gal_texture_format::X1R5G5B5_UNORM:
        return "X1R5G5B5_UNORM";
    case gal_texture_format::X1B5G5R5_UNORM:
        return "X1B5G5R5_UNORM";
    case gal_texture_format::B2G3R3A8_UNORM:
        return "B2G3R3A8_UNORM";
    case gal_texture_format::R8G8_UNORM:
        return "R8G8_UNORM";
    case gal_texture_format::R8G8_SNORM:
        return "R8G8_SNORM";
    case gal_texture_format::G8R8_UNORM:
        return "G8R8_UNORM";
    case gal_texture_format::G8R8_SNORM:
        return "G8R8_SNORM";
    case gal_texture_format::R8G8_UINT:
        return "R8G8_UINT";
    case gal_texture_format::R8G8_SINT:
        return "R8G8_SINT";
    case gal_texture_format::R8G8_SRGB:
        return "R8G8_SRGB";
    case gal_texture_format::R16_UNORM:
        return "R16_UNORM";
    case gal_texture_format::R16_SNORM:
        return "R16_SNORM";
    case gal_texture_format::R16_UINT:
        return "R16_UINT";
    case gal_texture_format::R16_SINT:
        return "R16_SINT";
    case gal_texture_format::R16_SFLOAT:
        return "R16_SFLOAT";
    case gal_texture_format::R16_SBFLOAT:
        return "R16_SBFLOAT";
    case gal_texture_format::R8G8B8_UNORM:
        return "R8G8B8_UNORM";
    case gal_texture_format::R8G8B8_SNORM:
        return "R8G8B8_SNORM";
    case gal_texture_format::R8G8B8_UINT:
        return "R8G8B8_UINT";
    case gal_texture_format::R8G8B8_SINT:
        return "R8G8B8_SINT";
    case gal_texture_format::R8G8B8_SRGB:
        return "R8G8B8_SRGB";
    case gal_texture_format::B8G8R8_UNORM:
        return "B8G8R8_UNORM";
    case gal_texture_format::B8G8R8_SNORM:
        return "B8G8R8_SNORM";
    case gal_texture_format::B8G8R8_UINT:
        return "B8G8R8_UINT";
    case gal_texture_format::B8G8R8_SINT:
        return "B8G8R8_SINT";
    case gal_texture_format::B8G8R8_SRGB:
        return "B8G8R8_SRGB";
    case gal_texture_format::R8G8B8A8_UNORM:
        return "R8G8B8A8_UNORM";
    case gal_texture_format::R8G8B8A8_SNORM:
        return "R8G8B8A8_SNORM";
    case gal_texture_format::R8G8B8A8_UINT:
        return "R8G8B8A8_UINT";
    case gal_texture_format::R8G8B8A8_SINT:
        return "R8G8B8A8_SINT";
    case gal_texture_format::R8G8B8A8_SRGB:
        return "R8G8B8A8_SRGB";
    case gal_texture_format::B8G8R8A8_UNORM:
        return "B8G8R8A8_UNORM";
    case gal_texture_format::B8G8R8A8_SNORM:
        return "B8G8R8A8_SNORM";
    case gal_texture_format::B8G8R8A8_UINT:
        return "B8G8R8A8_UINT";
    case gal_texture_format::B8G8R8A8_SINT:
        return "B8G8R8A8_SINT";
    case gal_texture_format::B8G8R8A8_SRGB:
        return "B8G8R8A8_SRGB";
    case gal_texture_format::R8G8B8X8_UNORM:
        return "R8G8B8X8_UNORM";
    case gal_texture_format::B8G8R8X8_UNORM:
        return "B8G8R8X8_UNORM";
    case gal_texture_format::R16G16_UNORM:
        return "R16G16_UNORM";
    case gal_texture_format::G16R16_UNORM:
        return "G16R16_UNORM";
    case gal_texture_format::R16G16_SNORM:
        return "R16G16_SNORM";
    case gal_texture_format::G16R16_SNORM:
        return "G16R16_SNORM";
    case gal_texture_format::R16G16_UINT:
        return "R16G16_UINT";
    case gal_texture_format::R16G16_SINT:
        return "R16G16_SINT";
    case gal_texture_format::R16G16_SFLOAT:
        return "R16G16_SFLOAT";
    case gal_texture_format::R16G16_SBFLOAT:
        return "R16G16_SBFLOAT";
    case gal_texture_format::R32_UINT:
        return "R32_UINT";
    case gal_texture_format::R32_SINT:
        return "R32_SINT";
    case gal_texture_format::R32_SFLOAT:
        return "R32_SFLOAT";
    case gal_texture_format::A2R10G10B10_UNORM:
        return "A2R10G10B10_UNORM";
    case gal_texture_format::A2R10G10B10_UINT:
        return "A2R10G10B10_UINT";
    case gal_texture_format::A2R10G10B10_SNORM:
        return "A2R10G10B10_SNORM";
    case gal_texture_format::A2R10G10B10_SINT:
        return "A2R10G10B10_SINT";
    case gal_texture_format::A2B10G10R10_UNORM:
        return "A2B10G10R10_UNORM";
    case gal_texture_format::A2B10G10R10_UINT:
        return "A2B10G10R10_UINT";
    case gal_texture_format::A2B10G10R10_SNORM:
        return "A2B10G10R10_SNORM";
    case gal_texture_format::A2B10G10R10_SINT:
        return "A2B10G10R10_SINT";
    case gal_texture_format::R10G10B10A2_UNORM:
        return "R10G10B10A2_UNORM";
    case gal_texture_format::R10G10B10A2_UINT:
        return "R10G10B10A2_UINT";
    case gal_texture_format::R10G10B10A2_SNORM:
        return "R10G10B10A2_SNORM";
    case gal_texture_format::R10G10B10A2_SINT:
        return "R10G10B10A2_SINT";
    case gal_texture_format::B10G10R10A2_UNORM:
        return "B10G10R10A2_UNORM";
    case gal_texture_format::B10G10R10A2_UINT:
        return "B10G10R10A2_UINT";
    case gal_texture_format::B10G10R10A2_SNORM:
        return "B10G10R10A2_SNORM";
    case gal_texture_format::B10G10R10A2_SINT:
        return "B10G10R10A2_SINT";
    case gal_texture_format::B10G11R11_UFLOAT:
        return "B10G11R11_UFLOAT";
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return "E5B9G9R9_UFLOAT";
    case gal_texture_format::R16G16B16_UNORM:
        return "R16G16B16_UNORM";
    case gal_texture_format::R16G16B16_SNORM:
        return "R16G16B16_SNORM";
    case gal_texture_format::R16G16B16_UINT:
        return "R16G16B16_UINT";
    case gal_texture_format::R16G16B16_SINT:
        return "R16G16B16_SINT";
    case gal_texture_format::R16G16B16_SFLOAT:
        return "R16G16B16_SFLOAT";
    case gal_texture_format::R16G16B16_SBFLOAT:
        return "R16G16B16_SBFLOAT";
    case gal_texture_format::R16G16B16A16_UNORM:
        return "R16G16B16A16_UNORM";
    case gal_texture_format::R16G16B16A16_SNORM:
        return "R16G16B16A16_SNORM";
    case gal_texture_format::R16G16B16A16_UINT:
        return "R16G16B16A16_UINT";
    case gal_texture_format::R16G16B16A16_SINT:
        return "R16G16B16A16_SINT";
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return "R16G16B16A16_SFLOAT";
    case gal_texture_format::R16G16B16A16_SBFLOAT:
        return "R16G16B16A16_SBFLOAT";
    case gal_texture_format::R32G32_UINT:
        return "R32G32_UINT";
    case gal_texture_format::R32G32_SINT:
        return "R32G32_SINT";
    case gal_texture_format::R32G32_SFLOAT:
        return "R32G32_SFLOAT";
    case gal_texture_format::R32G32B32_UINT:
        return "R32G32B32_UINT";
    case gal_texture_format::R32G32B32_SINT:
        return "R32G32B32_SINT";
    case gal_texture_format::R32G32B32_SFLOAT:
        return "R32G32B32_SFLOAT";
    case gal_texture_format::R32G32B32A32_UINT:
        return "R32G32B32A32_UINT";
    case gal_texture_format::R32G32B32A32_SINT:
        return "R32G32B32A32_SINT";
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return "R32G32B32A32_SFLOAT";
    case gal_texture_format::R64_UINT:
        return "R64_UINT";
    case gal_texture_format::R64_SINT:
        return "R64_SINT";
    case gal_texture_format::R64_SFLOAT:
        return "R64_SFLOAT";
    case gal_texture_format::R64G64_UINT:
        return "R64G64_UINT";
    case gal_texture_format::R64G64_SINT:
        return "R64G64_SINT";
    case gal_texture_format::R64G64_SFLOAT:
        return "R64G64_SFLOAT";
    case gal_texture_format::R64G64B64_UINT:
        return "R64G64B64_UINT";
    case gal_texture_format::R64G64B64_SINT:
        return "R64G64B64_SINT";
    case gal_texture_format::R64G64B64_SFLOAT:
        return "R64G64B64_SFLOAT";
    case gal_texture_format::R64G64B64A64_UINT:
        return "R64G64B64A64_UINT";
    case gal_texture_format::R64G64B64A64_SINT:
        return "R64G64B64A64_SINT";
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return "R64G64B64A64_SFLOAT";
    case gal_texture_format::D16_UNORM:
        return "D16_UNORM";
    case gal_texture_format::X8_D24_UNORM:
        return "X8_D24_UNORM";
    case gal_texture_format::D32_SFLOAT:
        return "D32_SFLOAT";
    case gal_texture_format::S8_UINT:
        return "S8_UINT";
    case gal_texture_format::D16_UNORM_S8_UINT:
        return "D16_UNORM_S8_UINT";
    case gal_texture_format::D24_UNORM_S8_UINT:
        return "D24_UNORM_S8_UINT";
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return "D32_SFLOAT_S8_UINT";
    case gal_texture_format::DXBC1_RGB_UNORM:
        return "DXBC1_RGB_UNORM";
    case gal_texture_format::DXBC1_RGB_SRGB:
        return "DXBC1_RGB_SRGB";
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return "DXBC1_RGBA_UNORM";
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return "DXBC1_RGBA_SRGB";
    case gal_texture_format::DXBC2_UNORM:
        return "DXBC2_UNORM";
    case gal_texture_format::DXBC2_SRGB:
        return "DXBC2_SRGB";
    case gal_texture_format::DXBC3_UNORM:
        return "DXBC3_UNORM";
    case gal_texture_format::DXBC3_SRGB:
        return "DXBC3_SRGB";
    case gal_texture_format::DXBC4_UNORM:
        return "DXBC4_UNORM";
    case gal_texture_format::DXBC4_SNORM:
        return "DXBC4_SNORM";
    case gal_texture_format::DXBC5_UNORM:
        return "DXBC5_UNORM";
    case gal_texture_format::DXBC5_SNORM:
        return "DXBC5_SNORM";
    case gal_texture_format::DXBC6H_UFLOAT:
        return "DXBC6H_UFLOAT";
    case gal_texture_format::DXBC6H_SFLOAT:
        return "DXBC6H_SFLOAT";
    case gal_texture_format::DXBC7_UNORM:
        return "DXBC7_UNORM";
    case gal_texture_format::DXBC7_SRGB:
        return "DXBC7_SRGB";
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return "PVRTC1_2BPP_UNORM";
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return "PVRTC1_4BPP_UNORM";
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return "PVRTC2_2BPP_UNORM";
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return "PVRTC2_4BPP_UNORM";
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return "PVRTC1_2BPP_SRGB";
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return "PVRTC1_4BPP_SRGB";
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return "PVRTC2_2BPP_SRGB";
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return "PVRTC2_4BPP_SRGB";
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return "ETC2_R8G8B8_UNORM";
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return "ETC2_R8G8B8_SRGB";
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return "ETC2_R8G8B8A1_UNORM";
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return "ETC2_R8G8B8A1_SRGB";
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return "ETC2_R8G8B8A8_UNORM";
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return "ETC2_R8G8B8A8_SRGB";
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return "ETC2_EAC_R11_UNORM";
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return "ETC2_EAC_R11_SNORM";
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return "ETC2_EAC_R11G11_UNORM";
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return "ETC2_EAC_R11G11_SNORM";
    case gal_texture_format::ASTC_4x4_UNORM:
        return "ASTC_4x4_UNORM";
    case gal_texture_format::ASTC_4x4_SRGB:
        return "ASTC_4x4_SRGB";
    case gal_texture_format::ASTC_5x4_UNORM:
        return "ASTC_5x4_UNORM";
    case gal_texture_format::ASTC_5x4_SRGB:
        return "ASTC_5x4_SRGB";
    case gal_texture_format::ASTC_5x5_UNORM:
        return "ASTC_5x5_UNORM";
    case gal_texture_format::ASTC_5x5_SRGB:
        return "ASTC_5x5_SRGB";
    case gal_texture_format::ASTC_6x5_UNORM:
        return "ASTC_6x5_UNORM";
    case gal_texture_format::ASTC_6x5_SRGB:
        return "ASTC_6x5_SRGB";
    case gal_texture_format::ASTC_6x6_UNORM:
        return "ASTC_6x6_UNORM";
    case gal_texture_format::ASTC_6x6_SRGB:
        return "ASTC_6x6_SRGB";
    case gal_texture_format::ASTC_8x5_UNORM:
        return "ASTC_8x5_UNORM";
    case gal_texture_format::ASTC_8x5_SRGB:
        return "ASTC_8x5_SRGB";
    case gal_texture_format::ASTC_8x6_UNORM:
        return "ASTC_8x6_UNORM";
    case gal_texture_format::ASTC_8x6_SRGB:
        return "ASTC_8x6_SRGB";
    case gal_texture_format::ASTC_8x8_UNORM:
        return "ASTC_8x8_UNORM";
    case gal_texture_format::ASTC_8x8_SRGB:
        return "ASTC_8x8_SRGB";
    case gal_texture_format::ASTC_10x5_UNORM:
        return "ASTC_10x5_UNORM";
    case gal_texture_format::ASTC_10x5_SRGB:
        return "ASTC_10x5_SRGB";
    case gal_texture_format::ASTC_10x6_UNORM:
        return "ASTC_10x6_UNORM";
    case gal_texture_format::ASTC_10x6_SRGB:
        return "ASTC_10x6_SRGB";
    case gal_texture_format::ASTC_10x8_UNORM:
        return "ASTC_10x8_UNORM";
    case gal_texture_format::ASTC_10x8_SRGB:
        return "ASTC_10x8_SRGB";
    case gal_texture_format::ASTC_10x10_UNORM:
        return "ASTC_10x10_UNORM";
    case gal_texture_format::ASTC_10x10_SRGB:
        return "ASTC_10x10_SRGB";
    case gal_texture_format::ASTC_12x10_UNORM:
        return "ASTC_12x10_UNORM";
    case gal_texture_format::ASTC_12x10_SRGB:
        return "ASTC_12x10_SRGB";
    case gal_texture_format::ASTC_12x12_UNORM:
        return "ASTC_12x12_UNORM";
    case gal_texture_format::ASTC_12x12_SRGB:
        return "ASTC_12x12_SRGB";
    case gal_texture_format::CLUT_P4:
        return "CLUT_P4";
    case gal_texture_format::CLUT_P4A4:
        return "CLUT_P4A4";
    case gal_texture_format::CLUT_P8:
        return "CLUT_P8";
    case gal_texture_format::CLUT_P8A8:
        return "CLUT_P8A8";
    default:
        return "The_Format_With_No_Name";
    }
}

constexpr gal_texture_format FromName(char *p) {
    // FNV-1a hashing algorithm.
    uint64_t hash = 0xcbf29ce484222325ULL;
    while ((*p) != 0) {
        hash = (((uint64_t)*p) ^ hash) * 1099511628211ULL;
        p++;
    }
    switch (hash) {
    case 0x2BF653266B186C03:
        return gal_texture_format::UNDEFINED;
    case 0x9C45001516D833DA:
        return gal_texture_format::R1_UNORM;
    case 0x6C00F001F570B433:
        return gal_texture_format::R2_UNORM;
    case 0xDF531A87DF5CFA59:
        return gal_texture_format::R4_UNORM;
    case 0xCBA3F653C82E1E9E:
        return gal_texture_format::R4G4_UNORM;
    case 0x9B897AC5679B790E:
        return gal_texture_format::G4R4_UNORM;
    case 0x63DBCDF5F194196A:
        return gal_texture_format::A8_UNORM;
    case 0x6ECBF1EF06BCE325:
        return gal_texture_format::R8_UNORM;
    case 0x5C766FFB8DE1F1E3:
        return gal_texture_format::R8_SNORM;
    case 0x2B467E1FD60E7DD8:
        return gal_texture_format::R8_UINT;
    case 0xE9ABC60F283D0A72:
        return gal_texture_format::R8_SINT;
    case 0x358F830EC2BB0208:
        return gal_texture_format::R8_SRGB;
    case 0x7706DB5BF249BD5A:
        return gal_texture_format::B2G3R3_UNORM;
    case 0xE2805E85156D9F31:
        return gal_texture_format::R4G4B4A4_UNORM;
    case 0xB8C356FBFD0E908:
        return gal_texture_format::R4G4B4X4_UNORM;
    case 0xC5A6BE73259B0031:
        return gal_texture_format::B4G4R4A4_UNORM;
    case 0xEEB2955DCFFE4A08:
        return gal_texture_format::B4G4R4X4_UNORM;
    case 0x220561C5779CB911:
        return gal_texture_format::A4R4G4B4_UNORM;
    case 0xD30FAF6090EC7818:
        return gal_texture_format::X4R4G4B4_UNORM;
    case 0x3CFC9293744E0831:
        return gal_texture_format::A4B4G4R4_UNORM;
    case 0xE725A84DF23BFD38:
        return gal_texture_format::X4B4G4R4_UNORM;
    case 0xE1AFBA335D26A8DA:
        return gal_texture_format::R5G6B5_UNORM;
    case 0x7A0E74B498D4FAFA:
        return gal_texture_format::B5G6R5_UNORM;
    case 0xA15971F29DD0621:
        return gal_texture_format::R5G5B5A1_UNORM;
    case 0x8EFDC73AA126BFA1:
        return gal_texture_format::B5G5R5A1_UNORM;
    case 0xC742A70EF53AC9F5:
        return gal_texture_format::A1B5G5R5_UNORM;
    case 0xD5B003D5851331F5:
        return gal_texture_format::A1R5G5B5_UNORM;
    case 0x7269A347036A9034:
        return gal_texture_format::R5G5B5X1_UNORM;
    case 0xF751D3627AB449B4:
        return gal_texture_format::B5G5R5X1_UNORM;
    case 0x42335ADEA4EFC0AC:
        return gal_texture_format::X1R5G5B5_UNORM;
    case 0xF4330B957F4D6A4C:
        return gal_texture_format::X1B5G5R5_UNORM;
    case 0x9A6812074759CB0B:
        return gal_texture_format::B2G3R3A8_UNORM;
    case 0xA8082655C8EFA87E:
        return gal_texture_format::R8G8_UNORM;
    case 0x8B68D9060D1834FC:
        return gal_texture_format::R8G8_SNORM;
    case 0x3304A6DE2E40A66:
        return gal_texture_format::G8R8_UNORM;
    case 0x76ED014D8BD59584:
        return gal_texture_format::G8R8_SNORM;
    case 0x15A01776FAC06AE1:
        return gal_texture_format::R8G8_UINT;
    case 0x810697A68C3D3FFB:
        return gal_texture_format::R8G8_SINT;
    case 0xAC3EAAA6A4B1CBB5:
        return gal_texture_format::R8G8_SRGB;
    case 0x2E5F1FF55A762DFC:
        return gal_texture_format::R16_UNORM;
    case 0x4AFE6D45164DA17E:
        return gal_texture_format::R16_SNORM;
    case 0x733B292CB050E2FB:
        return gal_texture_format::R16_UINT;
    case 0x7D4A8FD1ED40DE1:
        return gal_texture_format::R16_SINT;
    case 0x5181789E57BBA72A:
        return gal_texture_format::R16_SFLOAT;
    case 0xC56CFEC3560B6C7C:
        return gal_texture_format::R16_SBFLOAT;
    case 0x9B3BE5AE0416CB88:
        return gal_texture_format::R8G8B8_UNORM;
    case 0x5705279F0768C4D2:
        return gal_texture_format::R8G8B8_SNORM;
    case 0x8E06FA3EEA5892D7:
        return gal_texture_format::R8G8B8_UINT;
    case 0x43570A4F483894A5:
        return gal_texture_format::R8G8B8_SINT;
    case 0x8A0FD4F27827FAF:
        return gal_texture_format::R8G8B8_SRGB;
    case 0xE97F84F8095F6908:
        return gal_texture_format::B8G8R8_UNORM;
    case 0xA548C6E90CB16252:
        return gal_texture_format::B8G8R8_SNORM;
    case 0xF4DDD5D490E7BF57:
        return gal_texture_format::B8G8R8_UINT;
    case 0xAA2DE5E4EEC7C125:
        return gal_texture_format::B8G8R8_SINT;
    case 0x6F77D8E4CE11AC2F:
        return gal_texture_format::B8G8R8_SRGB;
    case 0x5DE537C5CEE18C99:
        return gal_texture_format::R8G8B8A8_UNORM;
    case 0xE25912D12E286C9F:
        return gal_texture_format::R8G8B8A8_SNORM;
    case 0x8725F84C50561C6C:
        return gal_texture_format::R8G8B8A8_UINT;
    case 0x9C079080C9D1C54E:
        return gal_texture_format::R8G8B8A8_SINT;
    case 0xE7B4D5806421618C:
        return gal_texture_format::R8G8B8A8_SRGB;
    case 0xE2CD67E1462B4619:
        return gal_texture_format::B8G8R8A8_UNORM;
    case 0x674142ECA572261F:
        return gal_texture_format::B8G8R8A8_SNORM;
    case 0xCCAB23134AB9BCEC:
        return gal_texture_format::B8G8R8A8_UINT;
    case 0xE18CBB47C43565CE:
        return gal_texture_format::B8G8R8A8_SINT;
    case 0x2D3A00475E85020C:
        return gal_texture_format::B8G8R8A8_SRGB;
    case 0x98E2B1CDECAE73E8:
        return gal_texture_format::R8G8B8X8_UNORM;
    case 0x1DCAE1E963F82D68:
        return gal_texture_format::B8G8R8X8_UNORM;
    case 0xA3FD1100F830F62E:
        return gal_texture_format::R16G16_UNORM;
    case 0xEF345ACC71CAFA68:
        return gal_texture_format::G16R16_UNORM;
    case 0x7F7DCC076FEC8E6C:
        return gal_texture_format::R16G16_SNORM;
    case 0x5736988FE4CCE7B2:
        return gal_texture_format::G16R16_SNORM;
    case 0xD347EDE3A8782E71:
        return gal_texture_format::R16G16_UINT;
    case 0xE8296E1821F3AE8B:
        return gal_texture_format::R16G16_SINT;
    case 0x2DA5B5DBEAB009F0:
        return gal_texture_format::R16G16_SFLOAT;
    case 0x1EBDD86AD61EECA:
        return gal_texture_format::R16G16_SBFLOAT;
    case 0xC89CD2CBE8076CC9:
        return gal_texture_format::R32_UINT;
    case 0x339692FB7927DCC3:
        return gal_texture_format::R32_SINT;
    case 0x79FC5AE23681AB28:
        return gal_texture_format::R32_SFLOAT;
    case 0xFB3BEE0BC19BCDC0:
        return gal_texture_format::A2R10G10B10_UNORM;
    case 0x258D79F3CBE9042F:
        return gal_texture_format::A2R10G10B10_UINT;
    case 0x4CDA31047B01F04A:
        return gal_texture_format::A2R10G10B10_SNORM;
    case 0xF00CCA04C6C8A4FD:
        return gal_texture_format::A2R10G10B10_SINT;
    case 0x477903BFE709CD40:
        return gal_texture_format::A2B10G10R10_UNORM;
    case 0x2274BF26D27846AF:
        return gal_texture_format::A2B10G10R10_UINT;
    case 0x991746B8A06FEFCA:
        return gal_texture_format::A2B10G10R10_SNORM;
    case 0xECF40F37CD57E77D:
        return gal_texture_format::A2B10G10R10_SINT;
    case 0xC370B0AC1F93F676:
        return gal_texture_format::R10G10B10A2_UNORM;
    case 0x171461D7B1633919:
        return gal_texture_format::R10G10B10A2_UINT;
    case 0xE6CC635E3B189E94:
        return gal_texture_format::R10G10B10A2_SNORM;
    case 0x805B220741121493:
        return gal_texture_format::R10G10B10A2_SINT;
    case 0xBB10FC1669F59756:
        return gal_texture_format::B10G10R10A2_UNORM;
    case 0xCAAD5E1DF82DE439:
        return gal_texture_format::B10G10R10A2_UINT;
    case 0x87FBAEC610087BF4:
        return gal_texture_format::B10G10R10A2_SNORM;
    case 0x35A69E4D894D7AB3:
        return gal_texture_format::B10G10R10A2_SINT;
    case 0x5E8DCDA0219DCB1:
        return gal_texture_format::B10G11R11_UFLOAT;
    case 0x1DBB77A7372DFC97:
        return gal_texture_format::E5B9G9R9_UFLOAT;
    case 0xEA85D5CCC5E081E7:
        return gal_texture_format::R16G16B16_UNORM;
    case 0xB57B831A10558861:
        return gal_texture_format::R16G16B16_SNORM;
    case 0x734C0B9981BA0FD6:
        return gal_texture_format::R16G16B16_UINT;
    case 0x851D369F098D3F4:
        return gal_texture_format::R16G16B16_SINT;
    case 0xD1D0593BFAADABEF:
        return gal_texture_format::R16G16B16_SFLOAT;
    case 0x6FDE47BBA7BEA90F:
        return gal_texture_format::R16G16B16_SBFLOAT;
    case 0xA20A282E75CABD2D:
        return gal_texture_format::R16G16B16A16_UNORM;
    case 0x8F48663AFC94404B:
        return gal_texture_format::R16G16B16A16_SNORM;
    case 0xC02442503D4CE820:
        return gal_texture_format::R16G16B16A16_UINT;
    case 0x7E89AA3F8F7BAB1A:
        return gal_texture_format::R16G16B16A16_SINT;
    case 0xBBE9BB01E53ECD91:
        return gal_texture_format::R16G16B16A16_SFLOAT;
    case 0xF170487E732A86C9:
        return gal_texture_format::R16G16B16A16_SBFLOAT;
    case 0x300B9BAFAA9826B9:
        return gal_texture_format::R32G32_UINT;
    case 0x9B04DBDF3BB7BD33:
        return gal_texture_format::R32G32_SINT;
    case 0x1FDED9A9A6421118:
        return gal_texture_format::R32G32_SFLOAT;
    case 0xFFFC8DBFFAC96F58:
        return gal_texture_format::R32G32B32_UINT;
    case 0xBE61D5AF4CF7FBF2:
        return gal_texture_format::R32G32B32_SINT;
    case 0x886FF275EC1787A9:
        return gal_texture_format::R32G32B32_SFLOAT;
    case 0x7648F8DE0F31709C:
        return gal_texture_format::R32G32B32A32_UINT;
    case 0x8B2A511288ACACBE:
        return gal_texture_format::R32G32B32A32_SINT;
    case 0xC1C2788592D0E665:
        return gal_texture_format::R32G32B32A32_SFLOAT;
    case 0x8FAC507F3E91D2F8:
        return gal_texture_format::R64_UINT;
    case 0xA635187107A2DE12:
        return gal_texture_format::R64_SINT;
    case 0x43149901E509F949:
        return gal_texture_format::R64_SFLOAT;
    case 0xA1ECCC57631904C7:
        return gal_texture_format::R64G64_UINT;
    case 0x573CDC67C0F90695:
        return gal_texture_format::R64G64_SINT;
    case 0xE09F2F9190B7A44E:
        return gal_texture_format::R64G64_SFLOAT;
    case 0x1E505D1E0F7F67:
        return gal_texture_format::R64G64B64_UINT;
    case 0xBDEE06FF1606B35:
        return gal_texture_format::R64G64B64_SINT;
    case 0x8B6FB89954C14C6E:
        return gal_texture_format::R64G64B64_SFLOAT;
    case 0x6E24CB511E0FC194:
        return gal_texture_format::R64G64B64A64_UINT;
    case 0xD91E8380AF3023F6:
        return gal_texture_format::R64G64B64A64_SINT;
    case 0x2B3FE15FC07EF07D:
        return gal_texture_format::R64G64B64A64_SFLOAT;
    case 0x54D2FD13F48C6DF6:
        return gal_texture_format::D16_UNORM;
    case 0xC01BE1F2E68AD96E:
        return gal_texture_format::X8_D24_UNORM;
    case 0xD032BE522E700032:
        return gal_texture_format::D32_SFLOAT;
    case 0xCF3FC9D0CC3D3B3B:
        return gal_texture_format::S8_UINT;
    case 0xE48C6BDE0E1BDCB9:
        return gal_texture_format::D16_UNORM_S8_UINT;
    case 0x882CBC689033B818:
        return gal_texture_format::D24_UNORM_S8_UINT;
    case 0x8F4B0A1BC125EADD:
        return gal_texture_format::D32_SFLOAT_S8_UINT;
    case 0x1AA6B819AB24AB4F:
        return gal_texture_format::DXBC1_RGB_UNORM;
    case 0xA0508EF228A561DA:
        return gal_texture_format::DXBC1_RGB_SRGB;
    case 0x71A3B9E5FE557FAE:
        return gal_texture_format::DXBC1_RGBA_UNORM;
    case 0xB9884F4F748AE445:
        return gal_texture_format::DXBC1_RGBA_SRGB;
    case 0x32F4805474A831D2:
        return gal_texture_format::DXBC2_UNORM;
    case 0xB38A3E758BE52A09:
        return gal_texture_format::DXBC2_SRGB;
    case 0x3D1459BB143AA545:
        return gal_texture_format::DXBC3_UNORM;
    case 0xBE7A8D5D66E49268:
        return gal_texture_format::DXBC3_SRGB;
    case 0x8F2AE8FFFA7C726C:
        return gal_texture_format::DXBC4_UNORM;
    case 0xB3AA2DF982C0DA2E:
        return gal_texture_format::DXBC4_SNORM;
    case 0x9FF9DF3FAAA96027:
        return gal_texture_format::DXBC5_UNORM;
    case 0x6CDE8436BC805FA1:
        return gal_texture_format::DXBC5_SNORM;
    case 0xC44BF30D194202E2:
        return gal_texture_format::DXBC6H_UFLOAT;
    case 0xAB754D40F2829738:
        return gal_texture_format::DXBC6H_SFLOAT;
    case 0x3694D3DE28B7BD51:
        return gal_texture_format::DXBC7_UNORM;
    case 0x25F07F23AA734B04:
        return gal_texture_format::DXBC7_SRGB;
    case 0x8DB1509C891B79CC:
        return gal_texture_format::PVRTC1_2BPP_UNORM;
    case 0xA909A9CC15D983BE:
        return gal_texture_format::PVRTC1_4BPP_UNORM;
    case 0x92E357BAD5FE499B:
        return gal_texture_format::PVRTC2_2BPP_UNORM;
    case 0xD1A7C043408183C1:
        return gal_texture_format::PVRTC2_4BPP_UNORM;
    case 0xFB26F7CE7F6C7153:
        return gal_texture_format::PVRTC1_2BPP_SRGB;
    case 0x14B5F7A00D275AF5:
        return gal_texture_format::PVRTC1_4BPP_SRGB;
    case 0x56D223B86BC012D6:
        return gal_texture_format::PVRTC2_2BPP_SRGB;
    case 0x3B616527F2F78AD4:
        return gal_texture_format::PVRTC2_4BPP_SRGB;
    case 0xA222B6AEF8F6342F:
        return gal_texture_format::ETC2_R8G8B8_UNORM;
    case 0xF7F74F04283C85FA:
        return gal_texture_format::ETC2_R8G8B8_SRGB;
    case 0xBDF6B48E8BA852A1:
        return gal_texture_format::ETC2_R8G8B8A1_UNORM;
    case 0xBA6524ED755FF874:
        return gal_texture_format::ETC2_R8G8B8A1_SRGB;
    case 0x1462C4700CE54CF6:
        return gal_texture_format::ETC2_R8G8B8A8_UNORM;
    case 0xB011F915A61397ED:
        return gal_texture_format::ETC2_R8G8B8A8_SRGB;
    case 0xB7BD64A57C276EFC:
        return gal_texture_format::ETC2_EAC_R11_UNORM;
    case 0xD45CB1F537FEE27E:
        return gal_texture_format::ETC2_EAC_R11_SNORM;
    case 0x4AAA81E45F616ADB:
        return gal_texture_format::ETC2_EAC_R11G11_UNORM;
    case 0x60548007F77B43D:
        return gal_texture_format::ETC2_EAC_R11G11_SNORM;
    case 0x42BDD8AC55AFC9ED:
        return gal_texture_format::ASTC_4x4_UNORM;
    case 0x8016161C7AB56E10:
        return gal_texture_format::ASTC_4x4_SRGB;
    case 0x2B5386538D919E02:
        return gal_texture_format::ASTC_5x4_UNORM;
    case 0xA8D99F174DB11719:
        return gal_texture_format::ASTC_5x4_SRGB;
    case 0x915C196DFECCFF75:
        return gal_texture_format::ASTC_5x5_UNORM;
    case 0xDB79A577D6446078:
        return gal_texture_format::ASTC_5x5_SRGB;
    case 0x1160A2285ABAED98:
        return gal_texture_format::ASTC_6x5_UNORM;
    case 0x920DC8F697217E9F:
        return gal_texture_format::ASTC_6x5_SRGB;
    case 0xB16AD1AF5DE6EBCD:
        return gal_texture_format::ASTC_6x6_UNORM;
    case 0x2745169EA3B7D5B0:
        return gal_texture_format::ASTC_6x6_SRGB;
    case 0x8E0E209D6C1311DE:
        return gal_texture_format::ASTC_8x5_UNORM;
    case 0x4ED6E5BA6D481E55:
        return gal_texture_format::ASTC_8x5_SRGB;
    case 0x2E56A25E2CED3A27:
        return gal_texture_format::ASTC_8x6_UNORM;
    case 0x991F492892D765F2:
        return gal_texture_format::ASTC_8x6_SRGB;
    case 0x4C114CE1A220870D:
        return gal_texture_format::ASTC_8x8_UNORM;
    case 0x18043CB1FA4302F0:
        return gal_texture_format::ASTC_8x8_SRGB;
    case 0xDAD7C1D8F81107E9:
        return gal_texture_format::ASTC_10x5_UNORM;
    case 0x6C33C4CA9B25A7BC:
        return gal_texture_format::ASTC_10x5_SRGB;
    case 0xC9992EE75BB14BC4:
        return gal_texture_format::ASTC_10x6_UNORM;
    case 0xFE70841B7B9AEF2B:
        return gal_texture_format::ASTC_10x6_SRGB;
    case 0x9ABA2EE82583DC42:
        return gal_texture_format::ASTC_10x8_UNORM;
    case 0x7F1CDE06C9B3C759:
        return gal_texture_format::ASTC_10x8_SRGB;
    case 0xDE0FA47A2B73AF7B:
        return gal_texture_format::ASTC_10x10_UNORM;
    case 0xCB795EFA4A237676:
        return gal_texture_format::ASTC_10x10_SRGB;
    case 0x5C03E046D11AEA6D:
        return gal_texture_format::ASTC_12x10_UNORM;
    case 0xC68986D3DC188B90:
        return gal_texture_format::ASTC_12x10_SRGB;
    case 0xCE0C328A06CC3333:
        return gal_texture_format::ASTC_12x12_UNORM;
    case 0x6C2617E4D8F0DC0E:
        return gal_texture_format::ASTC_12x12_SRGB;
    case 0xAFF9FCAB198F3D50:
        return gal_texture_format::CLUT_P4;
    case 0x9CF31C2F9538C655:
        return gal_texture_format::CLUT_P4A4;
    case 0xAFFA08AB198F51B4:
        return gal_texture_format::CLUT_P8;
    case 0x698142FD18AA335:
        return gal_texture_format::CLUT_P8A8;
    default:
        return gal_texture_format::UNDEFINED;
    }
}

// Helpers
constexpr uint32_t PixelCountOfBlock(gal_texture_format fmt) {
    return gal_tf_width_of_block(fmt) * gal_tf_height_of_block(fmt) * gal_tf_depth_of_block(fmt);
}


} // namespace ante::gal
