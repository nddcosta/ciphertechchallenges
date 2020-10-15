#ifndef CHALLENGE2_H_
#define CHALLENGE2_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "challenge1.h"

#define END_OF_LIST  0xFFFFFFFF
#define INITIAL_LSFR 0x4F574154
#define MAGIC_BYTES "CT2018"



struct Block{
    unsigned short size;
    unsigned int data_ptr;
};

struct Entry{
    char name[16];
    unsigned int block_list_ptr;
    int num_blocks;
    struct Block block_list[255];

};

struct KDB{
    struct Entry entry_list[127];
    int num_entries;
};

void initialize_kdb_struct(struct KDB *kdb);

struct Entry* get_entry_by_name(struct KDB *kdb, char *name);

int get_total_data_size(struct Entry *entry);

int end_of_list(FILE *fp);

void decode_entry_data(struct Entry *entry, FILE *fp, unsigned char *decoded_data);

int load_block_info(struct Entry *entry, FILE *fp);

void load_entry_blocks(struct Entry* entry, FILE* fp, int i);

int load_entry_info(struct KDB* kdb, FILE *fp);

void move_fp_to_entry_list(FILE* fp);

void load_data(struct KDB* kdb, FILE *fp);

int valid_KDB_header(FILE *fp);


#endif //CHALLENGE2_H_
