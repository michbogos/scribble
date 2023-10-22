#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define PI 3.1415926
#define STEPS 1000000
#define DT 0.001f
#define WIDTH 512
#define HEIGHT 512
#define DT_MIN 0.01
#define T_MIN 0.5

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;
int frame = 0;

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);

float density[HEIGHT][WIDTH];
unsigned char image[HEIGHT][WIDTH][3];


int main ()
{
    float posx = (float)rand()/(float)(RAND_MAX);
    float posy = (float)rand()/(float)(RAND_MAX);
    char* imageFileName = (char*) "bitmapImage.bmp";
    float t0 = (float)rand()/(float)(RAND_MAX/T_MIN);
    float t1 = (float)rand()/(float)(RAND_MAX/T_MIN);
    float t2 = (float)rand()/(float)(RAND_MAX/T_MIN);
    float t3 = (float)rand()/(float)(RAND_MAX/T_MIN);
    float dt0 = (float)rand()/(float)(RAND_MAX/0.001);
    float dt1 = (float)rand()/(float)(RAND_MAX/0.001);
    float dt2 = (float)rand()/(float)(RAND_MAX/0.001);
    float dt3 = (float)rand()/(float)(RAND_MAX/0.001);
    

    for(long i = 0; i < STEPS; i++){
        float tmpx = posx + DT*cosf(t0+posy+cosf((t1+posx)*PI));
        float tmpy = posy - DT*cosf(t2+posx+cosf((t3+posy)*PI));
        posx =  (tmpx - (int)tmpx);
        posy = (tmpy - (int)tmpy);
        density[(int)((HEIGHT/2)*posx+(HEIGHT/2))][(int)((WIDTH/2)*posy+(WIDTH/2))] += 0.5;
        t0 += 0.00123395;
        t1 += 0.00132752035;
        t2 += 0.00215705;
        t3 += 0.03830;
        if(i%(STEPS/50)==0 && i > 0){
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                        image[i][j][0] = tanhf(density[i][j])*255;
                        image[i][j][1] = tanhf(density[i][j])*200;
                        image[i][j][2] = tanhf(density[i][j])*155;
                }
            }
            frame ++;
            char buf[255];
            sprintf(buf, "%d.bmp", frame);

            generateBitmapImage((unsigned char*) image, HEIGHT, WIDTH, buf);
            printf("%ld\n", i);
        }
    }
    printf("Image generated!!");
}


void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}