using namespace std;
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

int main() {

    int width, height, channels;
    unsigned char *img = stbi_load("C:/Users/Swaroop/dev/projects/image filter engine/samples/input/test.png",&width,&height,&channels,0);

    if (img == NULL) {
        std::cout << "Error loading image: " << stbi_failure_reason() << "\n";
        return 1;
    }

    unsigned char* result = (unsigned char*)malloc(width*height*channels);

    for(int i=0;i<width*height*channels;i = i+channels){
        int r = img[i];
        int g = img[i+1];
        int b = img[i+2];

        int gray = 0.299*r+0.587*g+0.114*b;

        result[i]=gray;
        result[i+1]=gray;
        result[i+2]=gray;
    }

    stbi_write_png("grayscale.png",width,height,channels,result,width*channels);

    stbi_image_free(img);
    free(result);

    return 0;
}