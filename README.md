# Image-Compression
The main idea of the project is to reduce the size of the image by reducing the resolution and image quality. 
This has been done by using OpenCV. Here i had taken the image 'cameraman.jpeg' and reduced the resolution by x% percent as input parameter. 

## Compile and Running
#### Compiling
- g++ -ggdb `pkg-config --cflags opencv` -o `basename FinalImgProcess.cpp .cpp` FinalImgProcess.cpp `pkg-config --libs opencv`

#### Running
- ./FinalImgProcess /image-compression/cameraman.jpeg 20
