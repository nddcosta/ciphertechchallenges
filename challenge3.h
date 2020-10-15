#ifndef CHALLENGE_3_H
#define CHALLENGE_3_H

#include "challenge2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "md5.h"
#include <unistd.h>

#define ENTRY_NAME "MAGIC"



struct JPEG_info{
    unsigned int offset;
    unsigned int size;
    char filepath[40];
};


int is_magic_bytes(unsigned char *magic_bytes, unsigned char *value);

int is_end_of_jpeg(unsigned char *value);

void write_jpeg_head(FILE *fp);

int move_fp_to_next_jpeg(FILE *fp, unsigned char *magic_bytes);

int move_fp_to_end_jpeg(FILE *fp);

void get_filename_no_ext(char *str);

void load_jpeg_info(FILE *fp, struct JPEG_info* jpeg, char* input_filename);

void write_jpeg_file(FILE *fp, struct JPEG_info* jpeg);

int repair_jpeg_file(FILE *fp, unsigned char *magic_bytes, struct JPEG_info* jpeg, char* input_filename);

void make_directory(char* input_filename);

void print_file_info(int jpeg_index, struct JPEG_info *jpeg);

#endif//CHALLENGE_3_H
