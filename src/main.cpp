using namespace std;
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

void applyKernel(unsigned char* img, unsigned char* result, int width, int height, int channels, float kernel[3][3]);

int main() {

    int choice;
    int width, height, channels;
    unsigned char *img = stbi_load("C:/Users/Swaroop/dev/projects/image filter engine/samples/input/test.png",&width,&height,&channels,0);

    if (img == NULL) {
        std::cout << "Error loading image: " << stbi_failure_reason() << "\n";
        return 1;
    }

    unsigned char* result = (unsigned char*)malloc(width*height*channels);

    cout<<"Select Filter :\n1.Grayscale\n2.Box Blur\n3.Gaussian Blur\n4.Sharpen\n5.Sobel Edge Detection\n6.Emboss\n";
    cin>>choice;

    if(choice==1){
        for(int i=0;i<width*height*channels;i = i+channels){
            int r = img[i];
            int g = img[i+1];
            int b = img[i+2];

                int gray = 0.299*r+0.587*g+0.114*b;

            result[i]=gray;
            result[i+1]=gray;
            result[i+2]=gray;
        }

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/grayscale.png",width,height,channels,result,width*channels);

        stbi_image_free(img);
        free(result);
    }

    else if(choice == 2) {
    float v = 1.0f / 9.0f;
    float boxBlur[3][3] = {
        {v, v, v},
        {v, v, v},
        {v, v, v}
    };

    applyKernel(img, result, width, height, channels, boxBlur);
    stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/blur.png", width, height, channels, result, width * channels);

    stbi_image_free(img);
    free(result);
    }
    

    return 0;
}

void applyKernel(unsigned char* img, unsigned char* result, int width, int height, int channels, float Kernel[3][3]){
    for(int y=1;y<height-1;y++){
        for(int x=1;x<width-1;x++){
            float r =0,g=0,b=0;

                for(int KernelY=-1;KernelY<=1;KernelY++){
                    for(int KernelX=-1;KernelX<=1;KernelX++){
                        int neighbourX=x+KernelX;
                        int neighbourY=y+KernelY;

                        int index = (neighbourY * width + neighbourX) * channels;

                        float KernelIndex = Kernel[KernelY+1][KernelX+1];

                        r += img[index]*KernelIndex;
                        g += img[index+1]*KernelIndex;
                        b += img[index+2]*KernelIndex;
                    }
                }

        int outIndex = (y * width + x) * channels;

        result[outIndex]     = (unsigned char)std::min(std::max((int)r, 0), 255);
        result[outIndex + 1] = (unsigned char)std::min(std::max((int)g, 0), 255);
        result[outIndex + 2] = (unsigned char)std::min(std::max((int)b, 0), 255);
        }
    }

}