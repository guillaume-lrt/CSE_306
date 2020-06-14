#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <chrono>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std::chrono;

class Image{
    public:
        unsigned char *img;
        int width, height, channels;
        int size;
        std::vector<double> cum_energy_map;

    explicit Image(const char *name){
        img = stbi_load(name, &width, &height, &channels, 0);
        if (img == NULL){
            printf("Error in loading the image\n");
            exit(1);
        }
        printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
        size = width * height * channels;
        for (int i = 0; i < size; i++){
            cum_energy_map.push_back(-1);
        }
    }

    double intensity(int x, int y){
        if (x >= width || x < 0 || y >= height || y < 0){
            return 0;
        }
        return (img[(y * this->width + x) * 3 + 0] + img[(y * this->width + x) * 3 + 1] + img[(y * this->width + x) * 3 + 2])/3;
    }

    double energy(int x, int y){
        return abs(intensity(x+1,y) - intensity(x-1,y)) + abs(intensity(x,y+1) - intensity(x,y-1));
    }

    double cumulated_energy(int x, int y){
        if (x < 0 || x >= width || y < 0){
            return 0;
        }
        double res;
        int pos = y * this->width + x;
        if (cum_energy_map[pos] != -1){
            res = this->cum_energy_map[pos];
            return res;
        }
        res = std::min(cumulated_energy(x-1,y-1),std::min(cumulated_energy(x,y-1),cumulated_energy(x+1,y-1))) + energy(x,y);
        this->cum_energy_map[pos] = res;
        return res;
    }
};

int main(int argc, char **argv)
{
    auto start = high_resolution_clock::now();
    
    auto image = Image("image.jpg");

    const int W = image.width;
    const int H = image.height;

    unsigned char new_image[1280*960*3];
    printf("%d",image.size);

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {   
            double val = image.energy(j,i);
            // double val = image.cumulated_energy(j,i);
            std::cout << val << std::endl;
            new_image[(i * W + j) * 3 + 0] = val;
            new_image[(i * W + j) * 3 + 1] = val;
            new_image[(i * W + j) * 3 + 2] = val;
        }
    }
    // std::cout << new_image[0];

    // stbi_write_jpg("new_image.jpg", W, H, 3, new_image, 0);

    // delete[] new_image;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    duration = duration / 1000;
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
    return 0;
}
