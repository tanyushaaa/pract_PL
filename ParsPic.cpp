#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <iomanip>

using namespace std;

int bin(int color){
	if (0 <= color && color < 64){
		return 0b00;
	} else if (color < 128){
		return 0b01;
	} else if (color < 192){
		return 0b10;
	} else {
		return 0b11;
	}
}

typedef struct
{
    unsigned int    bfType;
    unsigned long   bfSize;
    unsigned int    bfReserved1;
    unsigned int    bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
} RGBQUAD;

struct Pic{
	int Width;
	int Height;
	RGBQUAD** rgb;
};

static unsigned short read_u16(FILE* fp);
static unsigned int   read_u32(FILE* fp);
static int            read_s32(FILE* fp);

void GetInfo(char* pathpic, struct Pic* rgb_){
    FILE* pFile = fopen(pathpic, "rb"); //cat_grey mouse

    BITMAPFILEHEADER header;

    header.bfType = read_u16(pFile);
    header.bfSize = read_u32(pFile);
    header.bfReserved1 = read_u16(pFile);
    header.bfReserved2 = read_u16(pFile);
    header.bfOffBits = read_u32(pFile);

    BITMAPINFOHEADER bmiHeader;
    									// kakie-to strashnye vyvody
	bmiHeader.biSize = read_u32(pFile); //printf("width\theight\n%d\t%d\n", bmiHeader.biWidth, bmiHeader.biHeight);
	bmiHeader.biWidth = read_s32(pFile); //printf("bits per pixel\n%d\n", bmiHeader.biBitCount);
	bmiHeader.biHeight = read_s32(pFile); //printf("biCompression\n%d\n", bmiHeader.biCompression);
    bmiHeader.biPlanes = read_u16(pFile);
    bmiHeader.biBitCount = read_u16(pFile);
    bmiHeader.biCompression = read_u32(pFile);
    bmiHeader.biSizeImage = read_u32(pFile);
    bmiHeader.biXPelsPerMeter = read_s32(pFile);
    bmiHeader.biYPelsPerMeter = read_s32(pFile);
    bmiHeader.biClrUsed = read_u32(pFile);
    bmiHeader.biClrImportant = read_u32(pFile);

    RGBQUAD** rgb = new RGBQUAD * [bmiHeader.biWidth];
    for (int i = 0; i < bmiHeader.biWidth; i++) {
        rgb[i] = new RGBQUAD[bmiHeader.biHeight];
    }

    for (int i = 0; i < bmiHeader.biWidth; i++) {
        for (int j = 0; j < bmiHeader.biHeight; j++) {
            rgb[i][j].rgbBlue = getc(pFile);
            rgb[i][j].rgbGreen = getc(pFile);
            rgb[i][j].rgbRed = getc(pFile);
        }
        getc(pFile);
    }
    fclose(pFile);

    rgb_->Height = bmiHeader.biHeight;
    rgb_->Width = bmiHeader.biWidth;
    rgb_->rgb = rgb;
}

int* BuildGist(char* pathpic, Pic* rgb_){
    GetInfo(pathpic, rgb_);

    int* colors = new int [64];
    for (int i = 0; i < 64; i++) {
        colors[i] = 0;
    }

    for (int i = 0; i < rgb_->Width; i++) {
        for (int j = 0; j < rgb_->Height; j++) {
            colors[bin((rgb_->rgb[i][j]).rgbRed) + (bin((rgb_->rgb[i][j]).rgbGreen) << 2) + (bin((rgb_->rgb[i][j]).rgbBlue) << 4)] ++;
        }
    }

	return colors;
}

double distance (const int *colors1, const int *colors2){
	double distance = 0;
	for (int i = 0; i < 64; i++){
		distance += ((colors1[i] - colors2[i])*(colors1[i] - colors2[i]));
	}
	return sqrt(distance);
}

double** makematrix (int** all_colors, int number_of_pics){//pod main strashya realizaciya
	double** matrix = new double* [number_of_pics];
	for (int i = 0; i < number_of_pics; i++){
		matrix[i] = new double[number_of_pics];
	}

	for(int i = 0; i < number_of_pics; i++){
		for(int j = 0; j < number_of_pics; j++){
			matrix[i][j] = distance(all_colors[i], all_colors[j]);
		}
	}

	return matrix;
}

void print_matrix(double** matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			cout.fill(' ');
			cout.width(13);
			cout << matrix[i][j];
		}
		cout << endl;
	}
}

void interface(){//pod main strashya realizaciya
	int number_of_pics;
	char pathpic[256];
	cin >> number_of_pics;
	Pic pics[4];

	int** all_colors = new int *[number_of_pics];

	for(int i = 0; i < number_of_pics; i++){
		cin >> pathpic;
		int* colors = new int[64];
		colors = BuildGist(pathpic, &pics[i]);
		all_colors[i] = colors;
	}

	print_matrix(makematrix(all_colors, number_of_pics));
}

int main(){
	Pic place;
	Pic to_find;
	char pathpic1[256] = "C:/Users/MI/Downloads/mouse.bmp";
	char pathpic2[256] = "C:/Users/MI/Downloads/cat_grey.bmp";

	GetInfo(pathpic1, &to_find);
	GetInfo(pathpic2, &place);

	if (to_find.Width < place.Width) {

	}
	interface();

    return 0;
}


static unsigned short read_u16(FILE* fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}


static unsigned int read_u32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static int read_s32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


//	char pathpic1[256] = "C:/Users/MI/Downloads/apple_1.bmp";
//	char pathpic2[256] = "C:/Users/MI/Downloads/apple_2.bmp";
//	char pathpic3[256] = "C:/Users/MI/Downloads/apple_3.bmp";
//	char pathpic4[256] = "C:/Users/MI/Downloads/apple_4.bmp";
//	struct Pic pic;
//	pic = *GetInfo(pathpic, pic);
//	int* colors1 = new int[64];
//	int* colors2 = new int[64];
//	int* colors3 = new int[64];
//	int* colors4 = new int[64];
//	colors1 = BuildGist(pathpic1);
//	colors2 = BuildGist(pathpic2);
//	colors3 = BuildGist(pathpic3);
//	colors4 = BuildGist(pathpic4);




//	matrix[0][0] = distance(all_colors[0], all_colors[0]);
//	matrix[0][1] = distance(all_colors[0], colors2);
//	matrix[0][2] = distance(all_colors[0], colors3);
//	matrix[0][3] = distance(all_colors[0], colors4);
//
//	matrix[1][0] = distance(colors2, colors1);
//	matrix[1][1] = distance(colors2, colors2);
//	matrix[1][2] = distance(colors2, colors3);
//	matrix[1][3] = distance(colors2, colors4);
//
//	matrix[2][0] = distance(colors3, colors1);
//	matrix[2][1] = distance(colors3, colors2);
//	matrix[2][2] = distance(colors3, colors3);
//	matrix[2][3] = distance(colors3, colors4);
//
//	matrix[3][0] = distance(colors4, colors1);
//	matrix[3][1] = distance(colors4, colors2);
//	matrix[3][2] = distance(colors4, colors3);
//	matrix[3][3] = distance(colors4, colors4);
