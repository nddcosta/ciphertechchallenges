#ifndef MD5_H
#define MD5_H
typedef unsigned int uint;
typedef unsigned char byte;
extern int enc64(char*,byte*,int);

typedef struct MD5state
{
    uint len;
    uint state[4];
}MD5state;
MD5state *nil;

int debug;
int hex;    /* print in hex?  (instead of default base64) */

void encode(byte*, uint*, uint);
void decode(uint*, byte*, uint);
MD5state* md5(byte*, uint, byte*, MD5state*);
void sum(FILE*, char*);

#endif
