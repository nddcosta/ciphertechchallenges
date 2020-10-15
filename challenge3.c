#include "challenge3.h"



int is_magic_bytes(unsigned char *magic_bytes, unsigned char *value){
	return (0 == memcmp(magic_bytes, value, 3));
}


int is_end_of_jpeg(unsigned char *value){
	unsigned char end_jpeg[] = {'\xff', '\xd9'};
	return (0 == memcmp(end_jpeg, value, 2));
}


void write_jpeg_head(FILE *fp){
	unsigned char jpeg_head[] = {'\xff', '\xd8', '\xff'};
	fwrite(jpeg_head, 1, 3, fp);
}


int move_fp_to_next_jpeg(FILE *fp, unsigned char *magic_bytes){
	unsigned char current_bytes[3];
	do{
		if(fread(current_bytes, 1, 3, fp) != 3){
			return 0;
		}
		fseek(fp, -2, SEEK_CUR);
	}while(!is_magic_bytes(magic_bytes, current_bytes));
	fseek(fp, 2, SEEK_CUR);
	return 1;
}


int move_fp_to_end_jpeg(FILE *fp){
    unsigned char current_bytes[2];
    do{
        if(fread(current_bytes, 1, 2, fp) != 2){
            return 0;
        }
        fseek(fp, -1, SEEK_CUR);
    }while(!is_end_of_jpeg(current_bytes));
    fseek(fp, 1, SEEK_CUR);
    return 1;
}


void get_filename_no_ext(char *str){
	int i = 0;
	while(str[i++] != '.');
	str[i-1] = '\0';
}


void load_jpeg_info(FILE *fp, struct JPEG_info* jpeg, char* input_filename){
    jpeg->offset = ftell(fp) - 3;
    move_fp_to_end_jpeg(fp);
    jpeg->size = ftell(fp) - jpeg->offset;
	snprintf(jpeg->filepath, 40, "%s_Repaired/%d.jpeg", input_filename, jpeg->offset);
}


void write_jpeg_file(FILE *fp, struct JPEG_info* jpeg){
	unsigned char *buffer[jpeg->size-3];
	FILE* OUTfp = fopen(jpeg->filepath, "wb+");
	fseek(fp, jpeg->offset+3, SEEK_SET);
	fread(buffer, 1, jpeg->size-3, fp);
	write_jpeg_head(OUTfp);
	fwrite(buffer, 1, jpeg->size-3, OUTfp);
	fclose(OUTfp);
}


int repair_jpeg_file(FILE *fp, unsigned char *magic_bytes, struct JPEG_info* jpeg, char* input_filename){
	if(move_fp_to_next_jpeg(fp, magic_bytes)){
		load_jpeg_info(fp, jpeg, input_filename);
   		write_jpeg_file(fp, jpeg);
		return 1;
	}else{
		return 0;
	}
}


void print_file_info(int jpeg_index, struct JPEG_info *jpeg){
	printf("Offset: %d\nSize: %d\nMD5: ", jpeg->offset, jpeg->size);
	FILE* fp = fopen(jpeg->filepath, "rb");
	sum(fp, "");
	fclose(fp);
	printf("Filepath: %s\n\n", jpeg->filepath);
}


void make_directory(char* input_filename){
	char jpeg_folder[30];
	snprintf(jpeg_folder, 30, "%s_Repaired", input_filename);
	
	DIR* dir = opendir(jpeg_folder);
	if (dir){
    	closedir(dir);
	}else if (ENOENT == errno){ 
   		char command[40];
    	snprintf(command, 40, "mkdir %s", jpeg_folder);
    	system(command); 
	}else{
    	printf("something went wrong");
	}
}



