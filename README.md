# 🖼️ Image Filter Engine

A multithreaded image processing engine written in C++ that applies convolution filters to image and compare how much faster multithreading makes it.

---

## What it does

You give it an image and it applies a filter, it tells you how long it took with one thread vs multiple threads.

Filters available:
- **Gaussian Blur** — smooths out noise and fine details (the blur can be imporved by using a larger size kernel)
- **Sharpen** — makes edges and details pop
- **Sobel Edge Detection** — finds and highlights the outlines of objects
- **Emboss** — gives the image a raised, 3D look
- **Grayscale** — get rid of all color, converts to black and white

---

## How it works

At the core is a convolution engine, it slides a small matrix (called a kernel) over every pixel in the image and computes a weighted sum of the surrounding pixels. Different kernels produce different effects.

For multithreading, OpenMP's #pragma omp parallel for directive is added before the pixel loops, and the compiler automatically distributes rows across 8 threads. No manual thread management needed.

After processing, it prints out the time taken for both single-threaded and multithreaded runs so you can see the speedup directly.

---

## Tech stack

- **C++**
- **stb_image / stb_image_write** — for loading and saving PNG/JPG images
- **OpenMP** — for multithreading (no external libraries needed)

---

## Running it

Compile with:
```bash
g++ -O3 -fopenmp main.cpp
```

Then run:
```bash
a.exe
```

You'll get a terminal menu to pick your filter and input image. Output is saved as a PNG.

---

## Performance results

Tested on a 1632 × 736 image with 8 threads:

| Filter | Single Thread | Multithreaded | Speedup |
|---|---|---|---|
| Grayscale | 0.00179s | 0.00113s | 1.58x |
| Gaussian Blur | 0.00987s | 0.00173s | 5.70x |
| Sharpen | 0.00897s | 0.00208s | 4.32x |
| Sobel Edge | 0.00941s | 0.00338s | 2.78x |
| Emboss | 0.00821s | 0.00189s | 4.35x |

Gaussian Blur sees the biggest gain because its workload is very uniform — perfect for splitting evenly across threads. Sobel is lower because it runs two kernel passes plus a square root per pixel, which is heavier work per thread.
