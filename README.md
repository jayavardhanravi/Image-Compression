# Image-Compression

## Compile and Running
#### Compiling
- g++ -ggdb `pkg-config --cflags opencv` -o `basename FinalImgProcess.cpp .cpp` FinalImgProcess.cpp `pkg-config --libs opencv`

#### Running
- ./FinalImgProcess /image-compression/cameraman.jpeg 20
