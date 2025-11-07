/*Documentation 
Name: Shobharaj H L
Date: 28/10/2025
Description: MP3 Tag Reader & Editor */

#include <stdio.h>
#include <string.h>
#include "mp3_header.h"

int main(int argc, char *argv[]) {
    if (argc == 3 && strcmp(argv[1], "-v") == 0) {
        view_mp3(argv[2]);
    }
    else if (argc == 5 && strcmp(argv[1], "-e") == 0) {
        if (strcmp(argv[2], "-t") == 0)
            edit_mp3("TIT2", argv[3], argv[4]);
        else if (strcmp(argv[2], "-a") == 0)
            edit_mp3("TPE1", argv[3], argv[4]);
        else if (strcmp(argv[2], "-A") == 0)
            edit_mp3("TALB", argv[3], argv[4]);
        else if (strcmp(argv[2], "-y") == 0)
            edit_mp3("TYER", argv[3], argv[4]);
        else if (strcmp(argv[2], "-C") == 0)
            edit_mp3("TCON", argv[3], argv[4]);
        else if (strcmp(argv[2], "-c") == 0)
            edit_mp3("COMM", argv[3], argv[4]);
        else
            help_main();
    }
    else if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        help_main();
    }
    else {
        Eror();
    }
}

void help_main() {
    printf("\n=================> HELP <=======================\n\n");
    printf("1. -v  --> to view mp3 file data\n");
    printf("2. -e  --> to edit mp3 file data\n");
    printf("\t-t --> edit song title\n");
    printf("\t-a --> edit artist name\n");
    printf("\t-A --> edit album name\n");
    printf("\t-y --> edit year\n");
    printf("\t-C --> edit content type\n");
    printf("\t-c --> edit comment\n");
    printf("\n================================================\n\n");
}

void Eror() {
    printf("\n=================> ERROR <=======================\n\n");
    printf("ERROR: INVALID ARGUMENTS.\n");
    printf("\nUSAGE:\nTo VIEW : ./a.out -v <filename>\n");
    printf("To EDIT : ./a.out -e <option> new_data <filename>\n");
    printf("Use ./a.out --help for help.\n");
    printf("\n================================================\n\n");
}
