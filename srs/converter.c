#include "bmp_func.h"
#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int theirs_implementation_converter( int argc, char* argv[] ) {
    UCHAR   r, g, b;
    UINT    width, height;
    UINT    x, y;
    BMP*    bmp;

    /* Read an image file */
    bmp = BMP_ReadFile( argv[ 2 ] );
    BMP_CHECK_ERROR( stdout, -3 );

    /* Get image's dimensions */
    width = BMP_GetWidth( bmp );
    height = BMP_GetHeight( bmp );
    
    /* Iterate through all the image's pixels */
    if( bmp->Header.BitsPerPixel == 24 ) {
        for ( x = 0 ; x < width ; ++x )
        {
            for ( y = 0 ; y < height ; ++y )
             {
                    /* Get pixel's RGB values */
                BMP_GetPixelRGB( bmp, x, y, &r, &g, &b );
        
                /* Invert RGB values */
                 BMP_SetPixelRGB( bmp, x, y, ~r, ~g, ~b );
            }
        }
    }
    else if( bmp->Header.BitsPerPixel == 8 ) {
        for ( int i = 0; i < BMP_PALETTE_SIZE_8bpp; ++i ) {
            bmp->Palette[i] = ~bmp->Palette[i];
        }
    }

    /* Save result */
    BMP_WriteFile( bmp, argv[ 3 ] );
    BMP_CHECK_ERROR( stdout, -3 );


    /* Free all memory allocated for the image */
    BMP_Free( bmp );

    return 0;
}

int mine_implementation_converter( int argc, char* argv[] ) {
    BMPMY* bmp;
    int return_value = 0;      
    bmp = Read_file(argv[2], &return_value);
    if(return_value != 0) {
        free_bmp(bmp);
        return return_value;
    }

    /*перевод в негатив*/
    if (bmp->Header.BitsPerPixel == 24) {
        for (int i = 0; i < bmp->Header.SizeImage; i++) {
            bmp->Data[i] = ~bmp->Data[i];
        }
    }
    else {
        for (int i = 0; i < bmp->Header.ColorUsed * BYTES_PALETTE; i++) {
            bmp->Palette[i] = ~bmp->Palette[i];
        }
    }

    Write_file(bmp, argv[3]);

    free_bmp(bmp);
}

int main( int argc, char* argv[] ) {  
    if(argc != 4) fprintf(stdout, "Call the function like this: %s --mine (or --theirs) <input file>.bmp <output file>.bmp\n", argv[0]);
    if(strncmp(argv[1], "--mine", MINE_LENGTH) == 0) {
        return mine_implementation_converter(argc, argv);
    }
    else if(strncmp(argv[1], "--theirs", THEIRS_LENGTH) == 0) {
        return theirs_implementation_converter(argc, argv);
    }
    else fprintf(stdout, "Call the function like this: %s --mine (or --theirs) <input file>.bmp <output file>.bmp\n", argv[0]);
    return 0;
}
