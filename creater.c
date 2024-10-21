#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct HEADER header;
struct DIB_HEADER dibheader;

unsigned int *open() {
    FILE *fP = fopen("dog.bmp", "rb");

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
        averaged_data[i / 3] = average_pixel_value;
        printf("%d %d %d %d\n", average_pixel_value, i, i / 3,
               size / 3); // for debugging,
                          // working as of now
    }
    return averaged_data;
}

unsigned int **sort(unsigned int *data) {
    unsigned int **nested_data =
        malloc(dibheader.height * sizeof(unsigned int *));
    for (int i = 0; i < dibheader.height; i++)
        nested_data[i] = malloc(dibheader.width * sizeof(unsigned int));
    for (int i = 0; i < dibheader.height * dibheader.width;
         i += dibheader.width) {
        for (int j = 0; j < dibheader.width; j++) {
            nested_data[i / dibheader.width][j] = data[i + j];
        }
    }
    return nested_data;
}

int main() {
    unsigned int *data = open();
    unsigned int **nested_data = sort(data);
    printf("%d", nested_data[2][2]);
}
