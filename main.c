#include "challenge3.h"

int main(int argc, char *argv[]) {
        FILE *KDBfp;
        FILE *INfp;
        struct JPEG_info jpegs[127]; //can find up to 127 jpegs, only 3 in this case

        if(argc != 3){
            printf("Input contains wrong number of args\n");
            exit(1);
        }

        KDBfp = fopen(argv[1], "rb");
        INfp = fopen(argv[2], "rb");

        if(KDBfp == NULL){
            printf("Error cannot open KDB file\n");
            exit(1);
        }

        if(INfp == NULL){
            printf("Error cannot open INPUT file\n");
            exit(1);
        }

        if(!valid_KDB_header(KDBfp)){
            printf("Header is not correct, file is not a valid KDB file\n");
            exit(1);
        }

        struct KDB kdb;
        initialize_kdb_struct(&kdb);
        load_data(&kdb, KDBfp);
        struct Entry* entry = get_entry_by_name(&kdb, ENTRY_NAME);
        int header_length = get_total_data_size(entry);
        unsigned char magic_bytes[header_length];
        decode_entry_data(entry, KDBfp, magic_bytes);
        fclose(KDBfp);

        char* filename = argv[2];
        get_filename_no_ext(filename);
        make_directory(filename);
		
		int i = 0;
        while(repair_jpeg_file(INfp, magic_bytes, &jpegs[i], filename)){
            print_file_info(i, &jpegs[i]);
			i++;
        }
        fclose(INfp);
        return(0);
}
