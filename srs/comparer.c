#include "bmp_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compare(BMPMY* bmp1, BMPMY* bmp2, int pixel_coordinates[], int *value_of_error_pixel) {
    int is_100_pixels = 0;
    for (int i = 0; i < bmp1->Header.Height; i++) {
        for (int j = 0; j < bmp1->Header.Width; j++) {
            int pixel_n = 3 * (i * bmp1->Header.Width + j);
            if (( bmp1->Header.BitsPerPixel == 24 && (bmp1->Data[pixel_n] != bmp2->Data[pixel_n] || bmp1->Data[pixel_n  + 1] != bmp2->Data[pixel_n  + 1] || bmp1->Data[pixel_n  + 2] != bmp2->Data[pixel_n  + 2] ))
            || ( bmp1->Header.BitsPerPixel == 8 && bmp1->Data[pixel_n / 3] != bmp2->Data[pixel_n / 3] )) {
                pixel_coordinates[2 * *value_of_error_pixel] = j;
                pixel_coordinates[2 * *value_of_error_pixel + 1] = i;
                *value_of_error_pixel += 1;
                if (*value_of_error_pixel > 99) {
                    is_100_pixels = 1;
                    break;
                }
            }
        }
        if (is_100_pixels == 1) break;
    }
}

int main( int argc, char* argv[] ) {  
    if(argc != 3) {
        fprintf(stdout, "Call the function like this: %s <file1_name>.bmp <file2_name>.bmp\n", argv[0]);
        return -1;
    }
    int return_value = 0;
    int value_of_error_pixel = 0;
    int pixel_coordinates[200]; 
    BMPMY* bmp1;
    BMPMY* bmp2;
    bmp1 = Read_file(argv[1], &return_value);
    if (return_value != 0) {
        free_bmp(bmp1);
        return -1;
    }
    bmp2 = Read_file(argv[2], &return_value);
    if (return_value != 0) {
        free_bmp(bmp1);
        free_bmp(bmp2);
        return -1;
    }
    if (bmp1->Header.FileSize != bmp2->Header.FileSize) {
        fprintf(stdout, "Files size error. File sizes are not equal\n");
        free_bmp(bmp1);
        free_bmp(bmp2);
        return -1;
    }
    if (bmp1->Header.BitsPerPixel == 24 && bmp1->Header.BitsPerPixel == 24) {
        compare(bmp1, bmp2, pixel_coordinates, &value_of_error_pixel);
    }
    else if (bmp1->Header.BitsPerPixel == 8 && bmp1->Header.BitsPerPixel == 8) {
        if (bmp1->Header.ColorUsed != bmp2->Header.ColorUsed) {
            fprintf(stdout, "Files palette error. Palettes in 8 bit files must be equal\n");
            free_bmp(bmp1);
            free_bmp(bmp2);
            return -1;
        }
        for (int i = 0; i < bmp1->Header.ColorUsed * BYTES_PALETTE; i++) {
            if (bmp1->Palette[i] != bmp2->Palette[i]) {
                fprintf(stdout, "Files palette error. The palettes in 8 bit files don't match\n");
                free_bmp(bmp1);
                free_bmp(bmp2);
            return -1;
            }
        }
        compare(bmp1, bmp2, pixel_coordinates, &value_of_error_pixel);
    }
    else {
        fprintf(stdout, "Files bits per pixel error. Bits per pixel must be equal in these files\n");
        free_bmp(bmp1);
        free_bmp(bmp2);
        return -1;
    }

    if (value_of_error_pixel > 0) {
        for (int i = 0; i < value_of_error_pixel; i++) {
            fprintf(stderr, "%d %d\n", pixel_coordinates[2*i], pixel_coordinates[2*i+1]);
        }
        free_bmp(bmp1);
        free_bmp(bmp2);
        return -1;
    }

    free_bmp(bmp1);
    free_bmp(bmp2);
    return 0;
}
