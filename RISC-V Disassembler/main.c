#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LINE_LENGTH 9

int label_count=0;
int pc=0;

int stringToValue(char *string,int s){
    int n=strlen(string);
    int value=0;

    if(s==1){
        if(string[0]=='1'){
            value=-1*pow(2,n-1);
            for(int i=1;i<n;i++){
                value+=((string[i]-'0')*pow(2,n-i-1));
            }
        }
        else{
            for(int i=0;i<n;i++){
                value+=((string[i]-'0')*pow(2,n-i-1));        
            }
        }
    }

    else if(s==0){
        for(int i=0;i<n;i++){
            value+=((string[i]-'0')*pow(2,n-i-1));        
        }
    }

    return value;
}

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


int main(){

	FILE *file;
    char hexLine[MAX_LINE_LENGTH];
    char *instruction;
	
	file=fopen("input.txt","r");

    int sameLabelcheck[20] = {0};

	while (fgets(hexLine, MAX_LINE_LENGTH, file) != NULL) {
       
        instruction = hexToBinary(hexLine);
        
        if(instruction!=NULL){
            for(int i=1;i<=label_count;i++){
                if(pc==sameLabelcheck[i]){
                    printf("L%d: ",i);
                    break;
                }
            }
            //printf("label_count=%d \n",label_count);
            //printf("%d PC\n",pc);
            
            if(strcmp(instruction + strlen(instruction)-7 , "0110011")==0){
                char func7[8];
                char func3[4];
                char rs2[6];
                char rs1[6];
                char rd[6];

                strncpy(func7,instruction+0,7);
                func7[7] = '\0';
                strncpy(rs2,instruction+7,5);
                rs2[5] = '\0';
                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';
                strncpy(rd,instruction+20,5);
                rd[5] = '\0';

                if(strcmp(func7,"0000000")==0){

                    if(strcmp(func3,"000")==0){
                        printf("add x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }
                    else if(strcmp(func3,"111")==0){
                        printf("and x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }
                    else if(strcmp(func3,"110")==0){
                        printf("or x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }
                    else if(strcmp(func3,"100")==0){
                        printf("xor x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }
                    else if(strcmp(func3,"001")==0){
                        printf("sll x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }
                    else if(strcmp(func3,"101")==0){
                        printf("srl x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }

                }

                else if(strcmp(func7,"0100000")==0){
                    
                    if(strcmp(func3,"000")==0){
                        printf("sub x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0)); 
                    }
                    else if(strcmp(func3,"101")==0){
                        printf("sra x%d x%d x%d\n",stringToValue(rd,0),stringToValue(rs1,0),stringToValue(rs2,0));
                    }

                }

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "0010011")==0){
                
                char func3[4];
                char rs1[6];
                char rd[6];
                char immediate[13];
                char func6[7];
                char immed0_5[7];

                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';
                strncpy(rd,instruction+20,5);
                rd[5] = '\0';
                strncpy(immediate,instruction+0,12);
                immediate[12] = '\0';

                if(strcmp(func3,"000")==0){
                    printf("addi x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immediate,1));
                }
                else if(strcmp(func3,"111")==0){
                    printf("andi x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immediate,1));
                }
                else if(strcmp(func3,"110")==0){
                    printf("ori x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immediate,1));
                }
                else if(strcmp(func3,"100")==0){
                    printf("xori x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immediate,1));
                }
                else if(strcmp(func3,"001")==0){

                    strncpy(immed0_5,immediate+6,6);
                    immed0_5[6]='\0';

                    printf("slli x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immed0_5,0));
                }
                else if(strcmp(func3,"101")==0){
                    strncpy(func6,immediate+0,6);
                    func6[6]='\0';
                    strncpy(immed0_5,immediate+6,6);
                    immed0_5[6]='\0';

                    if(strcmp(func6,"000000")==0){
                        printf("srli x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immed0_5,0));
                    }
                    if(strcmp(func6,"010000")==0){
                        printf("srai x%d x%d %d\n",stringToValue(rd,0) ,stringToValue(rs1,0) ,stringToValue(immed0_5,0));
                    }

                }
                
            }

            else if(strcmp(instruction + strlen(instruction)-7 , "1100111")==0){
                char func3[4];
                char rs1[6];
                char rd[6];
                char immediate[13];

                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';
                strncpy(rd,instruction+20,5);
                rd[5] = '\0';
                strncpy(immediate,instruction+0,12);
                immediate[12] = '\0';

                if(strcmp(func3,"000")==0){
                    printf("jalr x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,0) ,stringToValue(rs1,0));
                }

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "0000011")==0){
                char func3[4];
                char rs1[6];
                char rd[6];
                char immediate[13];

                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';
                strncpy(rd,instruction+20,5);
                rd[5] = '\0';
                strncpy(immediate,instruction+0,12);
                immediate[12] = '\0';

                if(strcmp(func3,"000")==0){
                    printf("lb x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"011")==0){
                    printf("ld x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"010")==0){
                    printf("lw x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"001")==0){
                    printf("lh x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"110")==0){
                    printf("lwu x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"101")==0){
                    printf("lhu x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"100")==0){
                    printf("lbu x%d %d(x%d)\n",stringToValue(rd,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "0100011")==0){
                char func3[4];
                char rs2[6];
                char rs1[6];
                char rd[6];
                char immediate[13];
                
                strncpy(immediate,instruction+0,7);
                immediate[7] = '\0';
                strncpy(rs2,instruction+7,5);
                rs2[5] = '\0';
                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';
                strncpy(rd,instruction+20,5);
                rd[5] = '\0';
                strcat(immediate,rd);
                immediate[12] = '\0';


                if(strcmp(func3,"000")==0){
                    printf("sb x%d %d(x%d)\n",stringToValue(rs2,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"001")==0){
                    printf("sh x%d %d(x%d)\n",stringToValue(rs2,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"010")==0){
                    printf("sw x%d %d(x%d)\n",stringToValue(rs2,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }
                else if(strcmp(func3,"011")==0){
                    printf("sd x%d %d(x%d)\n",stringToValue(rs2,0) ,stringToValue(immediate,1) ,stringToValue(rs1,0));
                }

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "1101111")==0){
                char rd[6];
                char immediate[22];
                
                strncpy(rd, instruction + 20, 5);
                rd[5] = '\0';

                strncpy(immediate, instruction + 0, 1);
                strncpy(immediate + 1, instruction + 12, 8);
                strncat(immediate,&instruction[11],1);
                strncpy(immediate + 10, instruction + 1, 10);
                immediate[20] = '0';
                immediate[21] = '\0';

                int offset=stringToValue(immediate,0);

                int temp=0;
                for(int i=0;i<=label_count;i++){
                    if(pc+offset==sameLabelcheck[i]){
                        temp=i;
                    }
                }
                if(temp==0){
                    label_count++;
                    sameLabelcheck[label_count]=pc+offset;
                    temp=label_count;
                }
              
                printf("jal x%d L%d\n",stringToValue(rd,0) ,temp);

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "0110111")==0){
                char rd[6];
                char immediate[33];
                char address[6];
        
                strncpy(address,hexLine+0,5);
                address[5]='\0';

                strncpy(rd, instruction + 20, 5);
                rd[5] = '\0';

                strncpy(immediate,instruction+0,20);
                strcat(immediate,"000000000000");
                immediate[32]='\0';

                printf("lui x%d 0x%s\n",stringToValue(rd,0) ,address);

            }

            else if(strcmp(instruction + strlen(instruction)-7 , "1100011")==0){
                char func3[4];
                char rs2[6];
                char rs1[6];
                char immediate[14]={0};
                char ch[2]="0";
                strncpy(rs2,instruction+7,5);
                rs2[5] = '\0';
                strncpy(rs1,instruction+12,5);
                rs1[5] = '\0';
                strncpy(func3,instruction+17,3);
                func3[3] = '\0';

                strncat(immediate,&instruction[0],1);                           
                strncat(immediate,&instruction[24],1);                             
                strncat(immediate,&instruction[1],6);                             
                strncat(immediate,&instruction[20],4);

                immediate[12]='0';
                immediate[13]='\0';
                int offset=stringToValue(immediate,0);

                int temp=0;
                for(int i=0;i<=label_count;i++){
                    if(pc+offset==sameLabelcheck[i]){
                        temp=i;
                    }
                }
                if(temp==0){
                    label_count++;
                    sameLabelcheck[label_count]=pc+offset;
                    temp=label_count;
                }
              

                if(strcmp(func3,"000")==0){
                    printf("beq x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }
                else if(strcmp(func3,"001")==0){
                    printf("bne x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }
                else if(strcmp(func3,"100")==0){
                    printf("blt x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }
                else if(strcmp(func3,"101")==0){
                    printf("bge x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }
                else if(strcmp(func3,"110")==0){
                    printf("bltu x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }
                else if(strcmp(func3,"111")==0){
                    printf("bgeu x%d x%d L%d\n",stringToValue(rs1,0), stringToValue(rs2,0), temp);
                }

            }
            pc=pc+4;

            free(instruction); 

        }       
       
    }

	fclose(file);

	return 0;
}