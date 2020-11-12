#include "bmp_func.h"

/* Reads an image from the specified file */
BMPMY* Read_file(char* filename, int* return_value) {

    FILE* photo_in;
    BMPMY* bmp;
    if ((photo_in = fopen(filename, "rb")) == NULL) {
        printf("Can not open file.\n");
        *return_value = -1;
        return NULL;
    }
    // reading data and checking it
    bmp = (BMPMY*)malloc( sizeof(BMPMY) );

    fseek(photo_in, 0, SEEK_END);
    long long int file_size = ftell(photo_in);
    fseek(photo_in, 0, SEEK_SET);

    fread(&bmp->Header.FileType, 2, 1, photo_in);
    fread(&bmp->Header.FileSize, 4, 1, photo_in);
    fread(&bmp->Header.Reserved, 4, 1, photo_in);
    fread(&bmp->Header.DataOffset, 4, 1, photo_in);
    fread(&bmp->Header.Header_Size, 4, 1, photo_in);
    fread(&bmp->Header.Width, 4, 1, photo_in);
    fread(&bmp->Header.Height, 4, 1, photo_in);
    fread(&bmp->Header.Planes, 2, 1, photo_in);
    fread(&bmp->Header.BitsPerPixel, 2, 1, photo_in);
    fread(&bmp->Header.Compression, 4, 1, photo_in);
    fread(&bmp->Header.SizeImage, 4, 1, photo_in);
    fread(&bmp->Header.XPixelsPerMeter, 4, 1, photo_in);
    fread(&bmp->Header.YPixelsPerMeter, 4, 1, photo_in);
    fread(&bmp->Header.ColorUsed, 4, 1, photo_in);
    fread(&bmp->Header.ColorImportant, 4, 1, photo_in);

    int no_error = 0;
    int height_mult_width = bmp->Header.Height * bmp->Header.Width * (bmp->Header.BitsPerPixel / 8);
    if(height_mult_width < 0) height_mult_width *= (-1);
    if (bmp->Header.FileType != 0x4D42) {
        fprintf(stdout, "Format error. Program supports only BMP format\n");
        free(bmp);
        no_error = 1; 
        *return_value = -1;
        return NULL;
    } 
    else if (bmp->Header.FileSize != file_size) {
        fprintf(stdout, "File size error. Actual size doesn't equal file size, written in header\n");
    }
    else if (bmp->Header.Reserved != 0) {
        fprintf(stdout, "Reserved error. Reserved bytes must be equal to 0\n");
    }
    else if (bmp->Header.Header_Size != 40) {
        fprintf(stdout, "Core version error. Program supports only BMP version 3\n");
    }
    else if (bmp->Header.Width <= 0) {
        fprintf(stdout, "Width error. Width must be positive\n");
    }
    else if (bmp->Header.Height == 0) {
        fprintf(stdout, "Height error. Height must not be equal to 0\n");
    }
    else if (bmp->Header.Planes != 1) {
        fprintf(stdout, "Planes error. Planes must be equal to 1\n"); 
    }
    else if (bmp->Header.BitsPerPixel != 8 && bmp->Header.BitsPerPixel != 24) {
        fprintf(stdout, "Bits Per Pixel error. Program supports only 8 or 24 bits images\n"); 
    }
    else if (bmp->Header.Compression != 0) {
        fprintf(stdout, "Compression error. Compressed images are not supported\n"); 
    }
    else if (bmp->Header.SizeImage != 0 && bmp->Header.SizeImage != bmp->Header.FileSize - bmp->Header.DataOffset) {
        fprintf(stdout, "Image size error. Actual size of pixel data is not equal to image size, written in header\n");
    }
    else if (!(bmp->Header.ColorUsed > 0 && bmp->Header.ColorUsed <=256) && bmp->Header.BitsPerPixel == 8) {
        fprintf(stdout, "Palette error. The number of colors in the palette of 8 bit image must be from 1 to 256\n"); 
    }
    else if (bmp->Header.ColorUsed != 0 && bmp->Header.BitsPerPixel == 24) {
        fprintf(stdout, "Palette error. 24 bit image doesn't have a palette\n"); 
    }
    else if (bmp->Header.ColorImportant > 256 && bmp->Header.BitsPerPixel == 8) {
        fprintf(stdout, "Palette error. Number of important colors over 256, but 8 bits images always have no more 256 of important colors in the palette\n"); 
    }
    else no_error = 1; 
    if(no_error == 0) {
        free(bmp);
        *return_value = -2;
        return NULL;
    }

    //creating a palette array and writing to it
    int size_palette = bmp->Header.ColorUsed * BYTES_PALETTE;
    bmp->Palette = (UCHAR*) malloc (size_palette);
    fread(bmp->Palette, sizeof(UCHAR), size_palette, photo_in);

    //creating an array of pixels and writing to it
    bmp->Data = (UCHAR*) malloc (bmp->Header.SizeImage);
    fread(bmp->Data, sizeof(UCHAR), bmp->Header.SizeImage, photo_in);
    fclose(photo_in);

    return bmp;
}


/* Writes an image to the specified file */
void Write_file(BMPMY* new_image, char* name_of_new_file){
    
    FILE* photo_out;
    if ((photo_out = fopen(name_of_new_file, "wb")) == NULL) {
        printf("Can not create file.\n");
    }

    fwrite(&new_image->Header.FileType, 2, 1, photo_out); 
    fwrite(&new_image->Header.FileSize, 4, 1, photo_out); 
    fwrite(&new_image->Header.Reserved, 4, 1, photo_out); 
    fwrite(&new_image->Header.DataOffset, 4, 1, photo_out); 
    fwrite(&new_image->Header.Header_Size, 4, 1, photo_out); 
    fwrite(&new_image->Header.Width, 4, 1, photo_out); 
    fwrite(&new_image->Header.Height, 4, 1, photo_out); 
    fwrite(&new_image->Header.Planes, 2, 1, photo_out); 
    fwrite(&new_image->Header.BitsPerPixel, 2, 1, photo_out); 
    fwrite(&new_image->Header.Compression, 4, 1, photo_out); 
    fwrite(&new_image->Header.SizeImage, 4, 1, photo_out); 
    fwrite(&new_image->Header.XPixelsPerMeter, 4, 1, photo_out); 
    fwrite(&new_image->Header.YPixelsPerMeter, 4, 1, photo_out); 
    fwrite(&new_image->Header.ColorUsed, 4, 1, photo_out); 
    fwrite(&new_image->Header.ColorImportant, 4, 1, photo_out); 
    fwrite(new_image->Palette, sizeof(UCHAR), new_image->Header.ColorUsed * BYTES_PALETTE, photo_out);
    fwrite(new_image->Data, sizeof(UCHAR), new_image->Header.SizeImage, photo_out);

    fclose(photo_out);
}

/* Free the structure */
void free_bmp(BMPMY *bmp) {
    if (bmp->Palette != NULL) {
        free(bmp->Palette);
    }
    if (bmp->Data != NULL) {
        free(bmp->Data);
    }
    if (bmp != NULL) {
        free(bmp);
    }
}