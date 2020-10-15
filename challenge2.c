#include "challenge2.h"




struct Entry* get_entry_by_name(struct KDB *kdb, char* name){
	for(int i = 0; i < kdb->num_entries; i++){
		if(0 == strcmp(name, kdb->entry_list[i].name)){
			return &kdb->entry_list[i];
		}
	}
	return NULL;
}


int get_total_data_size(struct Entry *entry){
	int total_size = 0;
	for(int i = 0; i < entry->num_blocks; i++){
		total_size += entry->block_list[i].size;
	}
	return total_size;
}


int end_of_list(FILE *fp){
	unsigned int value;
	fread(&value, sizeof(unsigned int), 1, fp);
	fseek(fp, -sizeof(value), SEEK_CUR);
	return (value == END_OF_LIST);
}


void decode_entry_data(struct Entry *entry, FILE *fp, unsigned char *decoded_data){
	int k = 0;

	for(int i = 0; i < entry->num_blocks; i++){
		fseek(fp, entry->block_list[i].data_ptr, SEEK_SET);
		unsigned char data[entry->block_list[i].size];
		fread(data, sizeof(char), entry->block_list[i].size, fp);
		for(int j = 0; j < entry->block_list[i].size; j++){
			decoded_data[k++] = data[j]; 
		}
		decoded_data[k] = '\0'; 
	}
	Crypt(decoded_data, k, INITIAL_LSFR);
}


int load_block_info(struct Entry *entry, FILE *fp){
	if(end_of_list(fp)){
		return 0;
	}
	fread(&entry->block_list[entry->num_blocks].size, sizeof(short), 1, fp);
	fread(&entry->block_list[entry->num_blocks++].data_ptr, sizeof(unsigned int), 1, fp);
	return 1;
}


void load_entry_blocks(struct Entry* entry, FILE* fp, int i){
    fseek(fp, entry->block_list_ptr, SEEK_SET);
	while(load_block_info(entry, fp));
}


int load_entry_info(struct KDB* kdb, FILE *fp){
	if(end_of_list(fp)){
		return 0;
	}
	fread(&kdb->entry_list[kdb->num_entries].name, sizeof(char), 16, fp);
    fread(&kdb->entry_list[kdb->num_entries++].block_list_ptr, sizeof(unsigned int), 1, fp);
	return 1;
}


void move_fp_to_entry_list(FILE* fp){
    unsigned int addr_buff;
    fseek(fp, 6 , SEEK_SET);
    fread(&addr_buff, sizeof(unsigned int), 1, fp);
    fseek(fp, addr_buff, SEEK_SET);
}


void load_data(struct KDB* kdb, FILE *fp){
	move_fp_to_entry_list(fp);
	while(load_entry_info(kdb, fp));
	for(int i = 0; i < kdb->num_entries; i++){		
		load_entry_blocks(&kdb->entry_list[i], fp, i);
	}
}


void print_KDB_data(struct KDB *kdb, FILE *fp){
	for(int i = 0; i < kdb->num_entries; i++){
		unsigned char decoded_data[get_total_data_size(&kdb->entry_list[i])];
        decode_entry_data(&kdb->entry_list[i], fp, decoded_data);
		printf("%s %s\n", kdb->entry_list[i].name, decoded_data);
    }
}

void initialize_kdb_struct(struct KDB *kdb){
	kdb->num_entries = 0;
	for(int i = 0; i < 127; i++){
		kdb->entry_list[i].num_blocks = 0;
		for(int j = 0 ; j < 255; j++){
			kdb->entry_list[i].block_list[j].size = 0;
		}
	}
}

int valid_KDB_header(FILE *fp){
	char buff[sizeof(MAGIC_BYTES)];
	fseek(fp, 0, SEEK_SET);
    fread(buff, sizeof(MAGIC_BYTES), 1, fp);
    buff[sizeof(MAGIC_BYTES)-1] = '\0';
	return (0 == strcmp(buff, MAGIC_BYTES));
}

