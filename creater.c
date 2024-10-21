#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define TRY                                                                    \
    do {                                                                       \
        jmp_buf ex_buf__;                                                      \
        if (!setjmp(ex_buf__)) {
#define CATCH                                                                  \
    }                                                                          \
    else {
#define ETRY                                                                   \
    }                                                                          \
    }                                                                          \
    while (0)
#define THROW longjmp(ex_buf__, 1)

struct HEADER {
    char name[2];
    unsigned int size;
    unsigned int garbage;
    unsigned int starting_address;
};

struct DIB_HEADER {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short int color_panes;
    unsigned short int bits_per_pixel;
    unsigned int compression;
    unsigned int image_size;
};

char creater(int x) {
    unsigned char characters[] = {'@', '#', 'S', '%', '?', '*',
                                  '+', ';', ':', ',', '.', ' '};
    TRY { return characters[x / 23]; }
    CATCH { return characters[0]; }
    ETRY;
}

unsigned int *open() {
    FILE *fP = fopen("dog.bmp", "rb");
    struct HEADER header;
    struct DIB_HEADER dibheader;

    fread(&header.name, 2, 1, fP);
    fread(&header.size, 4, 1, fP);
    fread(&header.garbage, 4, 1, fP);
    fread(&header.starting_address, 4, 1, fP);
    fread(&dibheader, sizeof(dibheader), 1, fP);
    printf("%s %d %d %d %d %d %d %d %d %d %d ", header.name, header.size,
           header.garbage, header.starting_address, dibheader.size,
           dibheader.width, dibheader.height, dibheader.color_panes,
           dibheader.bits_per_pixel, dibheader.compression,
           dibheader.image_size);

    fseek(fP, header.starting_address, SEEK_SET);
    printf("%d\n", ftell(fP));
    int size = ((dibheader.width * dibheader.bits_per_pixel + 31) / 32) * 4 *
               dibheader.height;
    int padding = dibheader.width % 4;
    unsigned char *data = malloc(size);
    unsigned int *averaged_data = malloc(size / 3);
    fread(data, 1, size, fP);
    for (int i = 0; i <= size; i += 3) {
        int average_pixel_value = (data[i] + data[i + 1] + data[i + 2]) / 3;
        if (i == 0)
            averaged_data[i] = average_pixel_value;
        else
            averaged_data[i / 3] = average_pixel_value;
        /*printf("%d %d %d %d\n", x, i, i / 3, size / 3);*/ // for debugging,
                                                            // working as of now
    }
    return averaged_data;
}

int main() { unsigned int *data = open(); }
