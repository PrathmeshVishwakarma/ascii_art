#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>

int main() {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load("google.png", &width, &height, &nrChannels, 0);
    printf("%s", data[5][5]);
}
