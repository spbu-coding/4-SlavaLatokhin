#ifndef BMP_func
#define BMP_func

#include <stdio.h>
#include <stdlib.h>

#ifndef UINT
    #define UINT    unsigned long int
#endif

#ifndef USHORT
    #define USHORT  unsigned short
#endif

#ifndef UCHAR
    #define UCHAR   unsigned char
#endif

#define BYTES_PALETTE 4
#define MINE_LENGTH   6
#define THEIRS_LENGTH 8 

typedef struct _BMP_FILE_HEADER
{
    USHORT              FileType;
    UINT                FileSize;
    UINT                Reserved;
    UINT                DataOffset;
    UINT                Header_Size;
    UINT                Width;
    long int            Height;
    USHORT              Planes;
    USHORT              BitsPerPixel;
    UINT                Compression;
    UINT                SizeImage;
    UINT                XPixelsPerMeter;
    UINT                YPixelsPerMeter;
    UINT                ColorUsed;
    UINT                ColorImportant;
}  BMP_FILE_HEADER;

typedef struct BMPMY
{
    BMP_FILE_HEADER     Header;
    UCHAR*              Palette;
    UCHAR*              Data;
} BMPMY;


BMPMY* Read_file(char* filename, int* return_value);

void Write_file(BMPMY* new_image, char* name_of_new_file);

void free_bmp(BMPMY *bmp);

#endif
