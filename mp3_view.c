/*Documentation 
Name: Shobharaj H L
Date: 28/10/2025
Description: MP3 Tag Reader & Editor */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3_header.h"

char *tag_list[] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "TCOM", "COMM"};
char *desc[]     = {"Title", "Artist", "Album", "Year", "Content Type", "Composer", "Comment"};

void view_mp3(const char *filename) {
    unsigned char sizebytes[4];
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        printf("File not found!\n");
        exit(0);
    }

    char id3[4];
    fread(id3, sizeof(char), 3, fptr);
    id3[3] = '\0';
    if (strcmp(id3, "ID3") != 0) {
        printf("It is not an MP3 file.\n");
        fclose(fptr);
        return;
    }

    printf("\nIt is an MP3 file.\n");
    fseek(fptr, 7, SEEK_CUR);
    printf("\n==============================> MP3 TAG VIEW <====================================\n\n");

    while (!feof(fptr)) {
        char TAG[5];
        if (fread(TAG, 1, 4, fptr) != 4) break;
        TAG[4] = '\0';

        unsigned char sizebytes[4];
        if (fread(sizebytes, 1, 4, fptr) != 4) break;
        unsigned int size = big_to_little(sizebytes);
        if (size == 0 || size > 100000) break;

        fseek(fptr, 2, SEEK_CUR);

        char *data = malloc(size + 1);
        if (data == NULL) {
            printf("Memory not allocated!\n");
            return;
        }

        fread(data, 1, size, fptr);
        data[size] = '\0';

        for (int j = 0; j < 7; j++) {
            if (strcmp(TAG, tag_list[j]) == 0) {
                if (strcmp(TAG, "COMM") == 0) {
                    // Skip encoding (1 byte) + language (3 bytes) + short desc (variable, here assuming 1 null terminator)
                    char *comment_text = data + 4;
                    while (*comment_text != '\0' && (comment_text - data) < size) comment_text++;
                    comment_text++; // move past null
                    printf("%-20s : %s\n", desc[j], comment_text);
                } else {
                    printf("%-20s : %s\n", desc[j], data + 1);
                }
            }
        }
        free(data);
    }

    printf("\n=================================================================================\n\n");
    fclose(fptr);
}
