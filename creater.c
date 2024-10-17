#include <stdio.h>
#include <stdlib.h>

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

int main() {
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
    unsigned long long int size =
        ((dibheader.width * dibheader.bits_per_pixel + 31) / 32) * 4 *
        dibheader.height;
    int padding = dibheader.width % 4;
    unsigned char *data = malloc(size);
    fread(data, 1, size, fP);
    for (int i = 0; i < size; i++) {
        printf("%d\n", data[i]);
    }
}
