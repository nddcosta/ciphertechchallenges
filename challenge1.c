#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define FEED_BACK_VALUE 0x87654321


unsigned char *Crypt(unsigned char* data, int dataLength, unsigned int initialValue){
	unsigned int currentLSFR = initialValue;
	unsigned char result[dataLength];
	int k = 0;

	for(int j = 0; j < dataLength; j++){
	    for(int i = 0; i < 8; i++){
			currentLSFR = (currentLSFR & 1) ? (currentLSFR >> 1) ^ FEED_BACK_VALUE : (currentLSFR >> 1);
	    }
		result[k++] = (data[j] ^ currentLSFR) & 0xFF;
	}
	result[k] = '\0';
	memcpy(data, result, dataLength);
	return result; //isn't used, instead the result is copied back into the data actual
}




