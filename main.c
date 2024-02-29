#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"
#include "pixel.h"
#include "info.h"

#define BLUE atoi(argv[4])
#define GREEN atoi(argv[5])
#define RED atoi(argv[6])
/* BPS - bytes per pixel = 3 */
#define BPS 3
#define OFFSET file_header.image_data_offset
#define LX -2
#define RX 2
#define DY -2
#define UY 2

void file_header_init(struct bmp_file_header *hdr, uint32_t w, uint32_t h, uint8_t padding){
	hdr->bmp_type = ('M' << 8) + 'B';
	hdr->size = (sizeof(struct bmp_file_header) + sizeof(struct bmp_info_header) + w*h*BPS + padding*h);
	hdr->reserved[0] = 0x00;
	hdr->reserved[1] = 0x00;
	hdr->image_data_offset = sizeof(struct bmp_file_header) + sizeof(struct bmp_info_header);
}

void info_header_init(struct bmp_info_header *hdr, uint32_t w, uint32_t h){
	hdr->size_of_info_header = sizeof(struct bmp_info_header);
	hdr->width = (int32_t)w;
	hdr->height = (int32_t)h;
	hdr->biPlanes = 0x1;
	hdr->biBitCount = 0x18;
	hdr->biCompression = 0x0;
	hdr->biSizeImage = 0x0;
	hdr->biXPelsPerMeter = 0x0ec4;
	hdr->biYPelsPerMeter = 0x0ec4;
	hdr->biClrUsed = 0x0;
	hdr->biClrImportant = 0x0; 
}

void fillBG(struct pixel **bitmap, uint32_t width, uint32_t height, uint8_t blue, uint8_t green, uint8_t red){
	for (size_t row = 0; row < height; row++){
		for (size_t column = 0; column < width; column++){
		bitmap[row][column].blue = blue;
		bitmap[row][column].green = green;
		bitmap[row][column].red = red;
		}
	}
}

double doubleX(size_t x, size_t w){
	double pixW = (double)(RX - LX)/w;
	double pixOf = pixW/2.0;
	return LX + pixW * x + pixOf;
}

double doubleY(size_t y, size_t h){
	double pixW = (double)(UY - DY)/h;
	double pixOf = pixW/2.0;
	return DY + pixW * y + pixOf;
}

bool InMandelbrot(double x0, double y0){
	double x_1 = 0;
	double y_1 = 0;
	double x = 0;
	
	for (size_t count = 0; count < 250; count ++){
		x = (x_1 * x_1) - ( y_1 * y_1) + x0;
		y_1 = 2 * x_1 * y_1 + y0;
		x_1 = x;
	}
	if (x_1 * x_1 + y_1 * y_1 <= 4){
		return true;
	}
	else{
		return false;
	}
}

void mandelbrotInit(struct pixel **bitmap, size_t w, size_t h){
	for (size_t row = 0; row < h; row++){
		for (size_t column = 0; column < w; column++){
			if (InMandelbrot(doubleX(column, w), doubleY(row, h))){
				bitmap[row][column].blue = column%row;
				bitmap[row][column].green = row%column;
				bitmap[row][column].red = column*row;
			}
		}
	}
}

int main(int argc, char *argv[]){
	if (argc != 7){	
		printf("Expected input format:\n<output_file_name> <width> <heigth> <BG_BLUE_0..255> <BG_GREEN_0..255> <BG_RED_0..255>\n");
		return 0;
	}
	
	FILE *fp;
	fp = fopen(argv[1], "w");
	uint8_t *buf;
	uint32_t  width = (uint32_t)atoi(argv[2]);
	uint32_t  height = (uint32_t)atoi(argv[3]);
	uint8_t pdng = 0;
	
	if (width % 4 != 0)
		pdng = 4 - (width*BPS)%4;
	
	struct bmp_file_header file_header;
	struct bmp_info_header info_header;
	struct pixel **row;
	row  = (struct pixel **)calloc((height) * sizeof(struct pixel *), 1);
		
	file_header_init(&file_header, width, height, pdng);
	info_header_init(&info_header, width, height);
	
	buf = (uint8_t*)calloc((size_t)file_header.size, 1);
	
	for (size_t i = 0; i < height; i++){
		row[i] = (struct pixel *)&buf[OFFSET + i * ( width * sizeof(struct pixel) + pdng )]; 
	}
	
	memcpy(buf, &file_header, sizeof(struct bmp_file_header));
	memcpy(&buf[sizeof(struct bmp_file_header)], &info_header, sizeof(struct bmp_info_header));

	fillBG(row, width, height, BLUE, GREEN, RED);  
	mandelbrotInit(row, width, height);

	fwrite(buf, file_header.size, 1, fp);
	free(row);
	free(buf);
	fclose(fp);
	printf("Done!\n");
	
	return 0;
}
