#pragma once

#pragma pack(push, 1)  // clang bug or smth?..

struct BitMapHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t off_bits;
};

struct InfoHeader {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    uint32_t x_pels;
    uint32_t y_pels;
    uint32_t crl_used;
    uint32_t crl_important;
};

#pragma pack(pop)

#pragma pack(push, 1)

struct RGBUint {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

#pragma pack(pop)
