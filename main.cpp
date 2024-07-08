#include "lodepng.h"
#include "foart.h"
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;

void writePng(const std::string& filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
    unsigned error = lodepng::encode(filename + ".png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        cout << "HELP:\nTo use this tool drag&drop needed Fonline Art file to program, or type file name as arg in console.\n\nABOUT:\nThe format has no name, but used in FOnline engine.\nThe author of format is Anton 'Cvet' Tsvetinsky.\nThe author of FoUnbaker is APAMk2. 2024.";
        return 0;
    }
    filesystem::path filename = argv[1];
    ByteReader* reader = new ByteReader;
    reader->Reset(filename.string(), ByteReader::BigEndian);
    file_t file;
    file.read(reader);

    for (size_t currData = 0; currData < file.hdr.dirs; currData++)
    {
        for (size_t currFrame = 0; currFrame < file.hdr.frames_count; currFrame++)
        {
            frame_t* currFrame_ptr = &file.data[currData].frames[currFrame];
            int width = currFrame_ptr->width;
            int height = currFrame_ptr->height;

            std::vector<unsigned char> image;
            image.resize(width * height * 4);
            for (size_t i = 0; i < width * height; i++)
            {
                image[i * 4] = currFrame_ptr->pixels[i].r;
                image[i * 4 + 1] = currFrame_ptr->pixels[i].g;
                image[i * 4 + 2] = currFrame_ptr->pixels[i].b;
                image[i * 4 + 3] = currFrame_ptr->pixels[i].a;
            }
            writePng(filename.stem().string() + to_string(currData) + "_" + to_string(currFrame), image, width, height);
        }
    }

    return 0;
}