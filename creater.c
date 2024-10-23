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
    /*char ret = 'x';*/
    /*return ret;*/
}

struct HEADER header;
struct DIB_HEADER dibheader;
char filename[50];

unsigned int *open() {
    printf("Please Enter the Filename\t");
    scanf("%s", &filename);
    FILE *fP = fopen(filename, "rb");

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
    if ((header.name[0] != 'B' && header.name[1] != 'M') ||
        dibheader.compression != 0) {
        printf("\nWRONG FILE\n");
        exit(0);
    }

    fseek(fP, header.starting_address, SEEK_SET);
    // printf("%d\n", ftell(fP));
    int size = ((dibheader.width * dibheader.bits_per_pixel + 31) / 32) * 4 *
               dibheader.height;
    int padding = dibheader.width % 4;
    unsigned char *data = malloc(size);
    unsigned int *averaged_data = malloc(size);
    fread(data, 1, size, fP);
    for (int i = 0; i <= size - 2; i += 3) {
        int average_pixel_value = (data[i] + data[i + 1] + data[i + 2]) / 3;
        averaged_data[i / 3] = average_pixel_value;
        // printf("%d %d %d %d\n", average_pixel_value, i, i / 3,
        // size / 3); // for debugging,
        // working as of now
    }
    fclose(fP);
    return averaged_data;
}

/*unsigned int **sort(unsigned int *data) {*/
/*    unsigned int **nested_data =*/
/*        malloc(dibheader.height * sizeof(unsigned int *));*/
/*    for (int i = 0; i < dibheader.height; i++)*/
/*        nested_data[i] = malloc(dibheader.width * sizeof(int));*/
/*    for (int i = 0; i < dibheader.height * dibheader.width;*/
/*         i += dibheader.width) {*/
/*        for (int j = 0; j < dibheader.width; j++) {*/
/*            nested_data[i / dibheader.width][j] = data[i + j];*/
/*        }*/
/*    }*/
/*    return nested_data;*/
/*}*/

void sort(unsigned int *data,
          unsigned int nested_data[dibheader.height][dibheader.width]) {
    memset(nested_data, 0, sizeof(int[dibheader.height][dibheader.width]));
    for (int i = 0; i < dibheader.height * dibheader.width;
         i += dibheader.width) {
        for (int j = 0; j < dibheader.width; j++) {
            nested_data[i / dibheader.width][j] = data[i + j];
        }
    }
}

void processing(unsigned int nested_data[dibheader.height][dibheader.width]) {
    FILE *fP = fopen("output.txt", "w");
    for (int i = dibheader.height - 1; i >= 0; i--) {
        for (int j = 0; j < dibheader.width; j++) {
            char character = creater(nested_data[i][j]);
            fputc(character, fP);
        }
        fprintf(fP, "\n");
    }
    fclose(fP);
}

int main() {
    unsigned int *data = open();
    unsigned int nested_data[dibheader.height][dibheader.width];
    sort(data, nested_data);
    processing(nested_data);
}
