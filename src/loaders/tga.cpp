#include <algorithm>
#include <fstream>
#include <iostream>
#include "tga.hpp"

/* NOTE: Does not handle all types of TGA */

/* 
 * Used tutorial:
 * http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/
 */

TGA readHeader(std::istream &file, const std::string &path)
{
    TGA tga;

    std::array<unsigned char, 6> &header = tga.header;
    file.read(reinterpret_cast<char *>(header.data()), header.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    tga.width = header[1] * 256 + header[0];
    tga.height = header[3] * 256 + header[2];
    tga.bitsPerPixel = header[4];
    tga.bytesPerPixel = header[4] / 8;

    /* Validate */
    if (tga.width <= 0 || tga.height <= 0 ||
        (tga.bitsPerPixel != 24 && tga.bitsPerPixel != 32))
    {
        throw std::runtime_error("Invalid TGA: " + path);
    }

    if (tga.bitsPerPixel == 24)
        tga.type = TGA::RGB;
    else // 32
        tga.type = TGA::RGBA;

    tga.imageSize = tga.bytesPerPixel * tga.width * tga.height;

    return tga;
}

TGA loadUncompressedTGA(std::istream &file, const std::string &path)
{
    TGA tga = readHeader(file, path);
    tga.imageData = std::vector<unsigned char>(tga.imageSize);
    file.read(reinterpret_cast<char *>(tga.imageData.data()), tga.imageData.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    // TGA stores colors in BGR, convert to RGB
    for (int i = 0; i < tga.imageSize; i += tga.bytesPerPixel)
        std::swap(tga.imageData[i], tga.imageData[i + 2]);

    //std::reverse(tga.imageData.begin(), tga.imageData.end());
    return tga;
}

TGA loadCompressedTGA(std::istream &file, const std::string &path)
{
    TGA tga = readHeader(file, path);
    tga.imageData = std::vector<unsigned char>(tga.imageSize);

    /* Decompress */
    int currentPixel = 0;
    int pixelCount = tga.width * tga.height;
    while (currentPixel < pixelCount)
    {
        /* Get chunkHeader */
        unsigned char chunkHeader = 0;
        chunkHeader = static_cast<unsigned char>(file.get());
        if (file.gcount() != 1)
            throw std::runtime_error("Unexpected error while reading file: " + path);

        /* Handle section */
        if (chunkHeader <= 127) // Section is raw
        {
            chunkHeader += 1; // Number of pixels in section
            std::vector<unsigned char> sectionBuffer(chunkHeader * tga.bytesPerPixel);
            file.read(reinterpret_cast<char *>(sectionBuffer.data()), sectionBuffer.size());
            if (!file.good())
                throw std::runtime_error("Unexpected error while reading file: " + path);

            std::move(sectionBuffer.begin(), sectionBuffer.end(), tga.imageData.begin() + currentPixel * tga.bytesPerPixel);
        }
        else // Section is run-length encoded
        {
            chunkHeader -= 127; // Number of times next pixel is repeated
            std::vector<unsigned char> pixelBuffer(tga.bytesPerPixel);
            file.read(reinterpret_cast<char *>(pixelBuffer.data()), tga.bytesPerPixel);
            if (!file.good())
                throw std::runtime_error("Unexpected error while reading file: " + path);

            for (auto i = 0; i < chunkHeader; ++i)
                std::copy(pixelBuffer.begin(), pixelBuffer.end(), tga.imageData.begin() + (currentPixel + i) * tga.bytesPerPixel);
        }

        currentPixel += chunkHeader;
    }

    // TGA stores colors in BGR, convert to RGB
    for (int i = 0; i < tga.imageSize; i += tga.bytesPerPixel)
        std::swap(tga.imageData[i], tga.imageData[i + 2]);

    //std::reverse(tga.imageData.begin(), tga.imageData.end());

    return tga;
}

TGA loadTGA(const std::string &path)
{
    const std::array<unsigned char, 12> uncompressedTGAHeader = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const std::array<unsigned char, 12> compressedTGAHeader = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

    std::array<unsigned char, 12> header;
    file.read(reinterpret_cast<char *>(header.data()), header.size());
    if (!file.good())
        throw std::runtime_error("Unexpected error while reading file: " + path);

    if (header == uncompressedTGAHeader)
        return loadUncompressedTGA(file, path);
    else if (header == compressedTGAHeader)
        return loadCompressedTGA(file, path);
    else
        throw std::runtime_error("Unhandled TGA header in file: " + path);
}