/*Documentation 
Name: Shobharaj H L
Date: 28/10/2025
Description: MP3 Tag Reader & Editor */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3_header.h"

void edit_mp3(char *tag, char *newname, char *filename) {

    FILE *fptr = fopen(filename, "rb");
    if (!fptr) {
        printf("File not found!\n");
        return;
    }

    FILE *ftemp = fopen("temp.mp3", "wb");
    if (!ftemp) {
        printf("Cannot create temp file!\n");
        fclose(fptr);
        return;
    }

    unsigned char header[10];
    if (fread(header, 1, 10, fptr) != 10) {
        printf("Failed to read header!\n");
        fclose(fptr);
        fclose(ftemp);
        return;
    }
    fwrite(header, 1, 10, ftemp);

    while (!feof(fptr)) {
        char TAG[5];
        if (fread(TAG, 1, 4, fptr) != 4) break;
        TAG[4] = '\0';

        unsigned char sizebytes[4];
        if (fread(sizebytes, 1, 4, fptr) != 4) break;
        unsigned int size = big_to_little(sizebytes);
        if (size == 0 || size > 100000) break;

        unsigned char flag_bytes[2];
        if (fread(flag_bytes, 1, 2, fptr) != 2) break;

        char *data = malloc(size);
        if (!data) break;
        fread(data, 1, size, fptr);

        if (strcmp(tag, TAG) == 0) {
            unsigned int new_len = strlen(newname) + 1;
            unsigned char new_size_bytes[4];
            little_to_big(new_len, new_size_bytes);
            fwrite(TAG, 1, 4, ftemp);
            fwrite(new_size_bytes, 1, 4, ftemp);
            fwrite(flag_bytes, 1, 2, ftemp);

            unsigned char encoding = 0x00;
            fwrite(&encoding, 1, 1, ftemp);
            fwrite(newname, 1, new_len - 1, ftemp);
            printf("Changed %s : %s\n", TAG, newname);
        } else {
            fwrite(TAG, 1, 4, ftemp);
            fwrite(sizebytes, 1, 4, ftemp);
            fwrite(flag_bytes, 1, 2, ftemp);
            fwrite(data, 1, size, ftemp);
        }
        free(data);
    }

    char ch;
    printf("Copying remaining data...\n");
    while (fread(&ch, 1, 1, fptr) == 1)
        fwrite(&ch, 1, 1, ftemp);

    fclose(fptr);
    fclose(ftemp);
    remove(filename);
    rename("temp.mp3", filename);
    printf("Tag edited successfully.\n");
}

unsigned int big_to_little(unsigned char bytes[4]) {
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

void little_to_big(unsigned int size, unsigned char sizeBytes[4]) {
    sizeBytes[0] = (size >> 24) & 0xFF;
    sizeBytes[1] = (size >> 16) & 0xFF;
    sizeBytes[2] = (size >> 8) & 0xFF;
    sizeBytes[3] = size & 0xFF;
}
