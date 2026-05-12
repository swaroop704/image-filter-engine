# Image Filter Engine

A multithreaded image processing engine in C++ that applies convolution filters (blur, sharpen, edge detection, emboss and more).

## Features

- Load and save PNG/JPG images
- Generic convolution engine that accepts any kernel
- Kernel library includes Gaussian Blur, Sharpen, Sobel Edge Detection, Emboss, Outline, and more
- Multithreaded processing using std::thread - splitting image into strips, one thread per strip
- Handles edge padding strategies - zero padding, clamp, mirror
- Simple CLI interface to apply any filter from the terminal
- Single-threaded vs multithreaded performance analysis included

 ## Tech Stack

- C++
- stb_image and stb_image_write for image I/O
- std::thread for multithreading
