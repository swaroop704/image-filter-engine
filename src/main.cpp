using namespace std;
#include <iostream>
#include <omp.h>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

void applyKernel(unsigned char *img, unsigned char *result, int width, int height, int channels, float kernel[3][3]);

int main()
{

    int choice;
    int width, height, channels;
    unsigned char *img = stbi_load("C:/Users/Swaroop/dev/projects/image filter engine/samples/input/test.png", &width, &height, &channels, 0);

    if (img == NULL)
    {
        std::cout << "Error loading image: " << stbi_failure_reason() << "\n";
        return 1;
    }

    unsigned char *result = (unsigned char *)malloc(width * height * channels);

    cout << "Select Filter :\n1.Grayscale\n2.Gaussian Blur\n3.Sharpen\n4.Sobel Edge Detection\n5.Emboss\n";
    cin >> choice;

    if (choice == 1)
    {
        omp_set_num_threads(1);
        auto startS = std::chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static)
        for (int i = 0; i < width * height * channels; i = i + channels)
        {
            int r = img[i];
            int g = img[i + 1];
            int b = img[i + 2];

            int gray = 0.299 * r + 0.587 * g + 0.114 * b;

            result[i] = gray;
            result[i + 1] = gray;
            result[i + 2] = gray;
        }

        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffS = endS - startS;

        omp_set_num_threads(omp_get_max_threads());
        auto startP = std::chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static)
        for (int i = 0; i < width * height * channels; i = i + channels)
        {
            int r = img[i];
            int g = img[i + 1];
            int b = img[i + 2];

            int gray = 0.299 * r + 0.587 * g + 0.114 * b;

            result[i] = gray;
            result[i + 1] = gray;
            result[i + 2] = gray;
        }

        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffP = endP - startP;

        cout << "\nGrayscale Filter Performance" << endl;
        cout << "Serial Time:   " << diffS.count() << " s" << endl;
        cout << "Parallel Time: " << diffP.count() << " s" << endl;
        cout << "Speedup:       " << diffS.count() / diffP.count() << "x" << endl;

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/grayscale.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if (choice == 2)
    {
        float v = 1.0f / 16.0f;
        float GBlur[3][3] = {
            {1 * v, 2 * v, 1 * v},
            {2 * v, 4 * v, 2 * v},
            {1 * v, 2 * v, 1 * v}};

        omp_set_num_threads(1);

        auto startS = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, GBlur);
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffS = endS - startS;

        omp_set_num_threads(8);

        auto startP = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, GBlur);
        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffP = endP - startP;

        cout << "Gaussian Blur Performance" << endl;
        cout << "Serial Time:   " << diffS.count() << "s" << endl;
        cout << "Parallel Time: " << diffP.count() << "s" << endl;
        cout << "Speedup:       " << diffS.count() / diffP.count() << "x" << endl;

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/gaussian_blur.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if (choice == 3)
    {
        float Sharpen[3][3] = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}};

        omp_set_num_threads(1);

        auto startS = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, Sharpen);
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffS = endS - startS;

        omp_set_num_threads(8);

        auto startP = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, Sharpen);
        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffP = endP - startP;

        cout << "Sharpen Performance" << endl;
        cout << "Serial Time:   " << diffS.count() << "s" << endl;
        cout << "Parallel Time: " << diffP.count() << "s" << endl;
        cout << "Speedup:       " << diffS.count() / diffP.count() << "x" << endl;

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/sharpen.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if (choice == 4)
    {
        float LeftSobel[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}};

        float TopSobel[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}};

#pragma omp parallel for schedule(static)
        for (int i = 0; i < width * height * channels; i += channels)
        {
            int gray = 0.299 * img[i] + 0.587 * img[i + 1] + 0.114 * img[i + 2];
            img[i] = img[i + 1] = img[i + 2] = (unsigned char)gray;
        }

        omp_set_num_threads(1);
        auto startS = std::chrono::high_resolution_clock::now();

        for (int y = 1; y < height - 1; y++)
        {
            for (int x = 1; x < width - 1; x++)
            {
                float rX = 0, gX = 0, bX = 0;
                float rY = 0, gY = 0, bY = 0;

                for (int KernelY = -1; KernelY <= 1; KernelY++)
                {
                    for (int KernelX = -1; KernelX <= 1; KernelX++)
                    {
                        int neighbourX = x + KernelX;
                        int neighbourY = y + KernelY;

                        int index = (neighbourY * width + neighbourX) * channels;

                        float KernelIndexX = LeftSobel[KernelY + 1][KernelX + 1];
                        float KernelIndexY = TopSobel[KernelY + 1][KernelX + 1];

                        rX += img[index] * KernelIndexX;
                        gX += img[index + 1] * KernelIndexX;
                        bX += img[index + 2] * KernelIndexX;

                        rY += img[index] * KernelIndexY;
                        gY += img[index + 1] * KernelIndexY;
                        bY += img[index + 2] * KernelIndexY;
                    }
                }

                int outIndex = (y * width + x) * channels;

                int finalR = (int)std::sqrt(rX * rX + rY * rY);
                int finalG = (int)std::sqrt(gX * gX + gY * gY);
                int finalB = (int)std::sqrt(bX * bX + bY * bY);

                result[outIndex] = (unsigned char)std::min(std::max(finalR, 0), 255);
                result[outIndex + 1] = (unsigned char)std::min(std::max(finalG, 0), 255);
                result[outIndex + 2] = (unsigned char)std::min(std::max(finalB, 0), 255);
            }
        }
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffS = endS - startS;

        omp_set_num_threads(8);
        auto startP = std::chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static)
        for (int y = 1; y < height - 1; y++)
        {
            for (int x = 1; x < width - 1; x++)
            {
                float rX = 0, gX = 0, bX = 0;
                float rY = 0, gY = 0, bY = 0;

                for (int KernelY = -1; KernelY <= 1; KernelY++)
                {
                    for (int KernelX = -1; KernelX <= 1; KernelX++)
                    {
                        int neighbourX = x + KernelX;
                        int neighbourY = y + KernelY;

                        int index = (neighbourY * width + neighbourX) * channels;

                        float KernelIndexX = LeftSobel[KernelY + 1][KernelX + 1];
                        float KernelIndexY = TopSobel[KernelY + 1][KernelX + 1];

                        rX += img[index] * KernelIndexX;
                        gX += img[index + 1] * KernelIndexX;
                        bX += img[index + 2] * KernelIndexX;

                        rY += img[index] * KernelIndexY;
                        gY += img[index + 1] * KernelIndexY;
                        bY += img[index + 2] * KernelIndexY;
                    }
                }

                int outIndex = (y * width + x) * channels;

                int finalR = (int)std::sqrt(rX * rX + rY * rY);
                int finalG = (int)std::sqrt(gX * gX + gY * gY);
                int finalB = (int)std::sqrt(bX * bX + bY * bY);

                result[outIndex] = (unsigned char)std::min(std::max(finalR, 0), 255);
                result[outIndex + 1] = (unsigned char)std::min(std::max(finalG, 0), 255);
                result[outIndex + 2] = (unsigned char)std::min(std::max(finalB, 0), 255);
            }
        }
        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffP = endP - startP;

        cout << "\n--- Directional Sobel Edge Detection Performance ---" << endl;
        cout << "Serial Time:   " << diffS.count() << "s" << endl;
        cout << "Parallel Time: " << diffP.count() << "s" << endl;
        cout << "Speedup:       " << diffS.count() / diffP.count() << "x" << endl;
        cout << "----------------------------------------------------\n"
             << endl;

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/Sobel.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    else if (choice == 5)
    {
        float Emboss[3][3] = {
            {-2, -1, 0},
            {-1, 1, 1},
            {0, 1, 2}};

#pragma omp parallel for schedule(static)

        for (int i = 0; i < width * height * channels; i = i + channels)
        {
            int r = img[i];
            int g = img[i + 1];
            int b = img[i + 2];

            int gray = 0.299 * r + 0.587 * g + 0.114 * b;

            img[i] = img[i + 1] = img[i + 2] = (unsigned char)gray;
        }

        omp_set_num_threads(1);

        auto startS = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, Emboss);
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffS = endS - startS;

        omp_set_num_threads(8);

        auto startP = std::chrono::high_resolution_clock::now();
        applyKernel(img, result, width, height, channels, Emboss);
        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diffP = endP - startP;

        cout << "Emboss detection Performance" << endl;
        cout << "Serial Time:   " << diffS.count() << "s" << endl;
        cout << "Parallel Time: " << diffP.count() << "s" << endl;
        cout << "Speedup:       " << diffS.count() / diffP.count() << "x" << endl;

        stbi_write_png("C:/Users/Swaroop/dev/projects/image filter engine/samples/output/Emboss.png", width, height, channels, result, width * channels);

        stbi_image_free(img);
        free(result);
    }

    return 0;
}

void applyKernel(unsigned char *img, unsigned char *result, int width, int height, int channels, float Kernel[3][3])
{
#pragma omp parallel for schedule(static)

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            float r = 0, g = 0, b = 0;

            for (int KernelY = -1; KernelY <= 1; KernelY++)
            {
                for (int KernelX = -1; KernelX <= 1; KernelX++)
                {
                    int neighbourX = x + KernelX;
                    int neighbourY = y + KernelY;

                    int index = (neighbourY * width + neighbourX) * channels;

                    float KernelIndex = Kernel[KernelY + 1][KernelX + 1];

                    r += img[index] * KernelIndex;
                    g += img[index + 1] * KernelIndex;
                    b += img[index + 2] * KernelIndex;
                }
            }

            int outIndex = (y * width + x) * channels;

            result[outIndex] = (unsigned char)std::min(std::max((int)abs(r), 0), 255);
            result[outIndex + 1] = (unsigned char)std::min(std::max((int)abs(g), 0), 255);
            result[outIndex + 2] = (unsigned char)std::min(std::max((int)abs(b), 0), 255);
        }
    }
}