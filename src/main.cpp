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

    cout<<"Select Filter :\n1.Grayscale\n2.Gaussian Blur\n3.Sharpen\n4.Sobel Edge Detection\n5.Emboss\n";
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
        float v = 1.0f / 16.0f;
            float GBlur[3][3] = {
            {1*v, 2*v, 1*v},
            {2*v, 4*v, 2*v},
            {1*v, 2*v, 1*v}
        };

        applyKernel(img, result, width, height, channels, GBlur);
        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/gaussian_blur.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }
    
    else if(choice == 3) {
        float Sharpen[3][3] = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
        };

        applyKernel(img, result, width, height, channels, Sharpen);
        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/sharpen.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if(choice == 4) {
        float LeftSobel[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };

        for(int i=0;i<width*height*channels;i = i+channels){
            int r = img[i];
            int g = img[i+1];
            int b = img[i+2];

                int gray = 0.299*r+0.587*g+0.114*b;

            img[i]=img[i+1]=img[i+2]=(unsigned char)gray;
        }

        applyKernel(img, result, width, height, channels, LeftSobel);
        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/Sobel.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if(choice == 5) {
        float Emboss[3][3] = {
            {-2, -1, 0},
            {-1, 1, 1},
            {0, 1, 2}
        };

         for(int i=0;i<width*height*channels;i = i+channels){
            int r = img[i];
            int g = img[i+1];
            int b = img[i+2];

                int gray = 0.299*r+0.587*g+0.114*b;

            img[i]=img[i+1]=img[i+2]=(unsigned char)gray;
        }

        applyKernel(img, result, width, height, channels, Emboss);
        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/Emboss.png", width, height, channels, result, width * channels);

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

        result[outIndex]     = (unsigned char)std::min(std::max((int)abs(r), 0), 255);
        result[outIndex + 1] = (unsigned char)std::min(std::max((int)abs(g), 0), 255);
        result[outIndex + 2] = (unsigned char)std::min(std::max((int)abs(b), 0), 255);
        }
    }

}