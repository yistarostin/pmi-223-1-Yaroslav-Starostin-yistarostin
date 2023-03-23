
#include "io.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "bmp_chunks.h"
#include "pixel.h"

static constexpr size_t FileHeaderSize = 14;
static constexpr uint16_t BitsPerPixel = 24;
static constexpr size_t DIBHeaderSize = 40;

template <typename T>
void PushChunk(std::ofstream &fstream, const T &value) {
    fstream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

IO::IO(const std::string &path) : path_(path) {
}

Image IO::Read() const {
    auto s = std::filesystem::current_path().string();
    std::cout << s << std::endl;
    std::ifstream bmp(path_, std::ios::binary);
    BitMapHeader bit_map_header;
    bmp.read(reinterpret_cast<char *>(&bit_map_header), sizeof(BitMapHeader));

    InfoHeader info_header;
    bmp.read(reinterpret_cast<char *>(&info_header), sizeof(InfoHeader));

    std::vector<std::vector<Pixel>> pixels(info_header.height, std::vector<Pixel>(info_header.width));
    for (uint32_t i = 0; i < info_header.height; ++i) {
        for (uint32_t j = 0; j < info_header.width; ++j) {
            Pixel current;
            bmp.read(reinterpret_cast<char *>(&current), sizeof(Pixel));
            pixels[info_header.height - 1 - i][j] = Pixel{
                // lines are stored from bottom to top
                .red = static_cast<Color>(current.blue),
                .green = static_cast<Color>(current.green),
                .blue = static_cast<Color>(current.red),
            };
        }
        uint32_t to_read = ((3 * info_header.width - 1) / 4 + 1) * 4 - 3 * info_header.width;
        for (uint32_t j = 0; j < to_read; ++j) {
            bmp.get();
        }
    }

    std::cout << info_header.size << " " << info_header.width << " " << info_header.height << "\n";
    return {pixels, bit_map_header, info_header};
}

bool IO::Write(const Image &image) const {
    std::ofstream image_write_stream(path_, std::ios_base::binary);
    if (!image_write_stream.is_open()) {
        throw std::invalid_argument(path_);  // TODO: throw something custom (derived)
    }
    const uint32_t row_size = ((BitsPerPixel * image.Width() + 32 - 1) / 32) * 4;
    const uint32_t image_size = row_size * image.Height();
    const uint32_t file_size = FileHeaderSize + DIBHeaderSize + image_size;
    BitMapHeader bitMapHeader{.type = image.bitMapHeader_.type,
                              .size = file_size,
                              .reserved1 = 0,
                              .reserved2 = 0,
                              .off_bits = ::FileHeaderSize + ::DIBHeaderSize};
    ::PushChunk(image_write_stream, bitMapHeader);
    InfoHeader infoHeader = {
        .size = ::DIBHeaderSize,
        .width = static_cast<uint32_t>(image.Width()),
        .height = static_cast<uint32_t>(image.Height()),
        .planes = 1,
        .bit_count = ::BitsPerPixel,
        .compression = 0,
        .size_image = image_size,
        .x_pels = 0,
        .y_pels = 0,
        .crl_used = 0,
        .crl_important = 0,
    };
    ::PushChunk(image_write_stream, infoHeader);
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            auto [r, g, b] = image.colors_.at(image.Height() - 1 - y)[x];
            for (auto &color : {b, g, r}) {
                ::PushChunk(image_write_stream, color);
            }
        }
        for (size_t written_row = 0; written_row < row_size - 3 * sizeof(uint8_t) * image.Width(); ++written_row) {
            image_write_stream.write("\0", 1);
        }
        image_write_stream.flush();
    }
    image_write_stream.close();

    return false;
}
