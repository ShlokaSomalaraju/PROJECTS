#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAX_LENGTH1 8
#define MAX_LINE_LENGTH 14

int timer=0;

struct Block{
	int tag;
	//int data;
	int validBit; //0 for not valid and 1 for valid
	int lastAccessed;
};

const char *hexCharToBinary(char c) {
    switch (c) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': case 'a': return "1010";
        case 'B': case 'b': return "1011";
        case 'C': case 'c': return "1100";
        case 'D': case 'd': return "1101";
        case 'E': case 'e': return "1110";
        case 'F': case 'f': return "1111";
        default: return NULL; 
    }
}

char *hexToBinary(const char *hexString) {
    int hexLength = strlen(hexString);
    int binaryLength = hexLength * 4; 

    char *binaryString = malloc(binaryLength + 1); // +1 for null terminator

    if (binaryString == NULL) {
        return NULL; 
    }

    for (int i = 0; i < hexLength; i++) {
        const char *binaryValue = hexCharToBinary(hexString[i]);
        if (binaryValue == NULL) {
            free(binaryString); 
            return NULL; // Invalid hex character
        }
        strcat(binaryString, binaryValue);
    }

    return binaryString;
}

int stringToValue(char *string){
	int count=0;
	while(string[count]!='\n'){
		count++;
	}

	int value=0;
	for (int i = 0; i < count; i++) {
        value = value * 10 + (string[i] - '0');
    }
	return value;
}

int stringToDecimal(char * string){
	int n = strlen(string);
	//printf("n=%d\n",n);
    int value = 0;

	for (int i = 0; i < n; i++) {
        value = value * 2 + (string[i] - '0');
    }

    return value;
}

int main(){

	FILE *file1;
	char read[MAX_LENGTH1+1];

	file1=fopen("cache.config","r");
	
	int SIZE_OF_CACHE = stringToValue(fgets(read,MAX_LENGTH1+1,file1));
	//printf("%d...\n",SIZE_OF_CACHE );

	int BLOCK_SIZE = stringToValue(fgets(read,5,file1));
	//printf("%d...\n",BLOCK_SIZE);

	int ASSOCIATIVITY = stringToValue(fgets(read,4,file1));
	//printf("%d... associativity\n",ASSOCIATIVITY);

	char* replacement= malloc((MAX_LENGTH1+1)*sizeof(char));
	fgets(replacement,MAX_LENGTH1,file1);
	replacement[MAX_LENGTH1]='\0';
	//printf("%s...\n",replacement);

	char writeback_policy[3];
	fgets(writeback_policy,3,file1);
	writeback_policy[2]='\0';
	//printf("%s...\n",writeback_policy);

	int offsetBits = log2(BLOCK_SIZE);
	int blocks = SIZE_OF_CACHE/BLOCK_SIZE;
	int indexBits;

	if(ASSOCIATIVITY==0){
		indexBits = 0;
	}
	else if(ASSOCIATIVITY==1){
		indexBits = log2(blocks);
	}
	else{
		indexBits = log2(blocks/ASSOCIATIVITY);
	}
	int tagBits = 32 - offsetBits - indexBits;

	/*printf("offsetBits=%d\n",offsetBits);
	printf("blocks=%d\n",blocks);
	printf("indexBits=%d\n",indexBits);
	printf("tagBits=%d\n",tagBits);*/

	FILE* file2;
	char line[MAX_LINE_LENGTH+1];

	file2=fopen("cache.access","r");

	//Only for Direct-Mapped cache
	if(ASSOCIATIVITY==1){
		struct Block cache[blocks];
		for(int i=0; i<blocks; i++){
			cache[i].validBit=0;
		}

		while( fgets( line, MAX_LINE_LENGTH+1, file2) != NULL ){

			char mode = line[0];
			//printf("%c\n",mode);

			char hexAddress[9];
			strncpy(hexAddress,&line[5],8);
			hexAddress[8]='\0';
			//printf("%s...\n",hexAddress);

			char address[33];
			strcpy(address,hexToBinary(hexAddress));
			address[32]='\0';
			//printf("%s...\n",address);

			char tagString[tagBits+1];
			strncpy(tagString , address , tagBits);
			tagString[tagBits]='\0';
			int tag = stringToDecimal(tagString);
			//printf("tagString=%s....tag=%d\n",tagString,tag);

			char indexString[indexBits+1];
			strncpy(indexString , address+tagBits , indexBits);
			indexString[indexBits]='\0';
			int index = stringToDecimal(indexString);
			//printf("index=%d\n",index);

			//char offsetString[offsetBits];
			//strncpy(offsetString , address+tagBits+indexBits , offsetBits);
			if(mode=='R'){
				if( cache[index].validBit==1 && cache[index].tag == tag ){
					printf("Address: 0x%s, Set: 0x%X, Hit, Tag: 0x%X\n",hexAddress,index,tag);
				}
				else{
					printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
					cache[index].validBit = 1;
					cache[index].tag = tag;
				}
			}
			else if(mode=='W'){
				if(writeback_policy[1]=='T'){//Write through no-allocate
					if( cache[index].validBit==1 && cache[index].tag == tag ){
						printf("Address: 0x%s, Set: 0x%X, Hit, Tag: 0x%X\n",hexAddress,index,tag);
					}
					else{
						printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
					}
				}
				else if(writeback_policy[1]=='B'){//Write back with allocate
					if( cache[index].validBit==1 && cache[index].tag == tag ){
						printf("Address: 0x%s, Set: 0x%X, Hit, Tag: 0x%X\n",hexAddress,index,tag);
					}
					else{
						printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
						cache[index].validBit = 1;
						cache[index].tag = tag;
					}
				}
			}
		}
	}

	//Fully Associative cache
	else if(ASSOCIATIVITY==0){
		//printf("fully\n");
		struct Block cache[blocks];
		for(int i=0; i<blocks; i++){
			cache[i].validBit=0;
			cache[i].lastAccessed=0;
		}
		int head=0;

		while( fgets( line, MAX_LINE_LENGTH+1, file2) != NULL ){

			char mode = line[0];
			//printf("%c\n",mode);

			char hexAddress[9];
			strncpy(hexAddress,&line[5],8);
			hexAddress[8]='\0';
			//printf("%s...\n",hexAddress);

			char address[33];
			strcpy(address,hexToBinary(hexAddress));
			address[32]='\0';
			//printf("%s...\n",address);

			char tagString[tagBits];
			strncpy(tagString , address , tagBits);
			tagString[tagBits]='\0';
			int tag = stringToDecimal(tagString);
			//printf("tag=%d\n",tag);

			//read mode or write mode with write back allocate policy
			if(mode=='R' || (mode=='W' && writeback_policy[1]=='B') ){
				int hit=0;
				int vv=-1;
				//for first in first out
				for(int i=0; i<blocks; i++){
					if(vv==-1){ 
						if(cache[i].validBit==0){vv=i;} 
					}
					if(cache[i].validBit==1 && cache[i].tag==tag){
						printf("Address: 0x%s, Set: 0x0, Hit, Tag: 0x%X\n",hexAddress,tag);
						hit++;
						timer++;
						cache[i].lastAccessed = timer;
					}
				}

				if(hit==0){
					//if all the blocks are full
					if(vv==-1){
						//LRU
						if(replacement[0]=='L'){
							int temp = timer + 1;
							int i=0;
							int rr;
							while(i<blocks){
								if(cache[i].lastAccessed < temp){
									temp = cache[i].lastAccessed;
									rr=i;
								}							
								i++;
							}
							cache[rr].tag=tag;
							timer++;
							cache[rr].lastAccessed = timer;
							printf("Address: 0x%s, Set: 0x0, Miss, Tag: 0x%X\n",hexAddress,tag);
						}
						//RANDOM
						else if(replacement[0]=='R'){
							srand(time(0));
							int randomIndex = (rand()%blocks);
							cache[randomIndex].tag = tag;
							cache[randomIndex].validBit = 1;
							printf("Address: 0x%s, Set: 0x0, Miss, Tag: 0x%X\n",hexAddress,tag);
						}
						//FIFO
						else if(replacement[0]=='F'){
							cache[head].tag=tag;
							head++;
							if(head == blocks){head=0;}
							printf("Address: 0x%s, Set: 0x0, Miss, Tag: 0x%X\n",hexAddress,tag);
						}
					}
					//If there are empty blocks.
					else{
						cache[vv].validBit = 1;
						cache[vv].tag = tag;
						timer++;
						cache[vv].lastAccessed = timer;
						printf("Address: 0x%s, Set: 0x0, Miss, Tag: 0x%X\n",hexAddress,tag);
					}
				}
			}
			//Write through with no-allocate
			else if(mode=='W' && writeback_policy[1]=='T'){
				int hit=0;
				int vv=-1;
				//for first in first out
				for(int i=0; i<blocks; i++){
					if(vv==-1){ 
						if(cache[i].validBit==0){vv=i;} 
					}
					if(cache[i].validBit==1 && cache[i].tag==tag){
						printf("Address: 0x%s, Set: 0x0, Hit, Tag: 0x%X\n",hexAddress,tag);
						hit=1;
						timer++;
						cache[i].lastAccessed = timer;
					}
				}

				if(hit==0){
					printf("Address: 0x%s, Set: 0x0, Miss, Tag: 0x%X\n",hexAddress,tag);
				}
			}

		}

	}

	//Set Associative cache
	else{
		int temp = pow(2,indexBits);
		struct Block cache[temp][ASSOCIATIVITY];
		for(int i=0; i<temp; i++){
			for(int j=0; j<ASSOCIATIVITY; j++){
				cache[i][j].validBit=0;
				cache[i][j].lastAccessed=0;
			}
		}

		int head[temp];
		for(int i=0; i<temp; i++){
			head[i]=0;
		}

		while( fgets( line, MAX_LINE_LENGTH+1, file2) != NULL ){

			char mode = line[0];
			//printf("%c\n",mode);

			char hexAddress[9];
			strncpy(hexAddress,&line[5],8);
			hexAddress[8]='\0';
			//printf("%s...\n",hexAddress);

			char address[33];
			strcpy(address,hexToBinary(hexAddress));
			address[32]='\0';
			//printf("%s...\n",address);

			char tagString[tagBits];
			strncpy(tagString , address , tagBits);
			tagString[tagBits]='\0';
			int tag = stringToDecimal(tagString);
			//printf("tag=%d\n",tag);

			char indexString[indexBits];
			strncpy(indexString , address+tagBits , indexBits);
			indexString[indexBits]='\0';
			int index = stringToDecimal(indexString);
			//printf("index=%d\n",index);

			if(mode=='R' || (mode=='W' && writeback_policy[1]=='B') ){
				int hit=0;
				int vv=-1;
				for(int i=0; i<ASSOCIATIVITY; i++){
					//printf("%d tag %d valid\n",cache[index][i].tag,cache[index][i].validBit);
					if(vv==-1){ 
						if(cache[index][i].validBit==0){vv=i;} 
					}
					if( cache[index][i].tag==tag && cache[index][i].validBit==1 ){
						hit++;
						timer++;
						cache[index][i].lastAccessed = timer;
						printf("Address: 0x%s, Set: 0x%X, Hit, Tag: 0x%X\n",hexAddress,index,tag);
						continue;
					}
				}
				//printf("%d vv\n",vv);
				if(hit==0){
					if(vv==-1){
						if(replacement[0]=='L'){
							int temp = timer+1;
							int i=0;
							int rr;
							while(i<ASSOCIATIVITY){
								if(cache[index][i].lastAccessed < temp){
									temp = cache[index][i].lastAccessed;
									rr=i;
								}							
								i++;
							}
							cache[index][rr].tag=tag;
							timer++;
							cache[index][rr].lastAccessed = timer;
							printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
						}
						else if(replacement[0]=='R'){
							srand(time(0));
							int randomCol = (rand()%ASSOCIATIVITY);
							cache[index][randomCol].tag=tag; 
							cache[index][randomCol].validBit=1;
							printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
						}
						else if(replacement[0]=='F'){
							cache[index][head[index]].tag=tag;
							head[index]++;
							if(head[index]==ASSOCIATIVITY){
								head[index]=0;
							}
							printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
						}
					}
					//If there are empty blocks
					else{
						cache[index][vv].tag = tag;
						cache[index][vv].validBit = 1;
						timer++;
						cache[index][vv].lastAccessed = timer;
						printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
					}
				}
			}

			else if(mode=='W' && writeback_policy[1]=='T'){
				int hit=0;
				int vv=-1;
				for(int i=0; i<ASSOCIATIVITY; i++){
					//printf("%d tag %d valid\n",cache[index][i].tag,cache[index][i].validBit);
					if(vv==-1){ 
						if(cache[index][i].validBit==0){vv=i;} 
					}
					if( cache[index][i].tag==tag && cache[index][i].validBit==1 ){
						hit++;
						timer++;
						cache[index][i].lastAccessed = timer;
						printf("Address: 0x%s, Set: 0x%X, Hit, Tag: 0x%X\n",hexAddress,index,tag);
						continue;
					}
				}
				
				if(hit==0){
					printf("Address: 0x%s, Set: 0x%X, Miss, Tag: 0x%X\n",hexAddress,index,tag);
				}
			}

		}

	}

	fclose(file1);
	fclose(file2);

	free(replacement);

	return 0;
}