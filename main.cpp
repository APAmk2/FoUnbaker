#include "lodepng.h"
#include "foart.h"
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;

void writePng(const std::string& filename, std::vector<ucolor>& pixels, unsigned width, unsigned height)
{
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (size_t i = 0; i < width * height; i++)
    {
        image[i * 4] = pixels[i].r;
        image[i * 4 + 1] = pixels[i].g;
        image[i * 4 + 2] = pixels[i].b;
        image[i * 4 + 3] = pixels[i].a;
    }

    unsigned error = lodepng::encode(filename + ".png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

bool decodeGraphics(filesystem::path& filename)
{
    ByteReader* reader = new ByteReader;
    reader->Reset(filename.string(), ByteReader::BigEndian);
    file_t file;
    file.read(reader);

    if (file.hdr.check_num1 != 42) return false;

    for (size_t currData = 0; currData < file.hdr.dirs; currData++)
    {
        for (size_t currFrame = 0; currFrame < file.hdr.frames_count; currFrame++)
        {
            frame_t* currFrame_ptr = &file.data[currData].frames[currFrame];
            writePng(filename.stem().string() + to_string(currData) + "_" + to_string(currFrame), currFrame_ptr->pixels, currFrame_ptr->width, currFrame_ptr->height);
        }
    }

    return true;
}

void decodeOldGraphics(filesystem::path& filename)
{
    ByteReader* reader = new ByteReader;
    reader->Reset(filename.string(), ByteReader::BigEndian);
    oldfile_t file;
    file.read(reader);

    writePng(filename.stem().string() + "_unbake", file.pixels, file.width, file.height);
}

int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        cout << "HELP:\nTo use this tool drag&drop needed Fonline Art file to program, or type file name as arg in console.\n\nABOUT:\nThe format has no name, but used in FOnline engine.\nThe author of format is Anton 'Cvet' Tsvetinsky.\nThe author of FoUnbaker is APAMk2. 2024.";
        return 0;
    }
    filesystem::path filename = argv[1];
    if (!decodeGraphics(filename))
    {
        decodeOldGraphics(filename);
    }

    return 0;
}