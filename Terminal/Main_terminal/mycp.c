#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int do_cp(char *source_file ,char *dest_file, char *option){
  

      FILE *file1;
      FILE *file2;
      char each;
      struct stat file_stat;
      
          
    stat(dest_file,&file_stat);
  
      
    if(S_ISDIR(file_stat.st_mode)){
             
             char * ptr=strrchr(source_file,'/');
             char actual[20];
            if(ptr!=NULL)
            { 
                strcpy(actual,ptr+1); 
                strcat(dest_file,"/");
                strcat(dest_file,actual);
                
               }
         
            
    }
    printf("%s\n",dest_file);

    file1=fopen(source_file,"r");
   if(file1==NULL){printf("Error in reading file\n");}
   else{ 


  
    if(strcmp(option,"-i")==0){
    	
    	char response;

         if(S_ISDIR(file_stat.st_mode)){
         	file2=fopen(dest_file,"r");
         	if(file2 != NULL){
               fclose(file2); 
               printf("%s\n",dest_file);
              printf("File %s already exists. Do you want to overwrite it? (y/n)\n", dest_file);
              scanf("%c\n", &response);
              if(response=='y'){
             	file2=fopen(dest_file,"w");
             	while((each=fgetc(file1))!=EOF){
      	        fputc(each,file2);
                }
                fclose(file2);

              }   
              else if(response=='n'){
                printf("not overwritten\n");
                fclose(file2);
                fclose(file1);
             	return 0;}         	
              }
            else if(file2==NULL){
                file2=fopen(dest_file,"w");
                while((each=fgetc(file1))!=EOF){
                fputc(each,file2);
                }
                fclose(file2);
            } 
         }

         else if(S_ISREG(file_stat.st_mode)){

            file2=fopen(dest_file,"r");
            if(file2!=NULL){
                fclose(file2);
                printf("%s\n",dest_file);
            	printf("File %s already exists. Do you want to overwrite it? (y/n)\n", dest_file);
               scanf("%c\n", &response);
               if(response=='y'){
               	 file2=fopen(dest_file,"w");
             	 while((each=fgetc(file1))!=EOF){
      	         fputc(each,file2);
                 }fclose(file2);
               }
               else{
               	fclose(file1);
               	fclose(file2);
               	printf("not overwritten\n");
               	return 0;}
            }
           else{
            file2=fopen(dest_file,"w");
            while((each=fgetc(file1))!=EOF){
                 fputc(each,file2);
                 }fclose(file2);
           } 
            
            
         }
         fclose(file1);
         
    }     

    else if(strcmp(option,"x")==0){
        

    	 file2=fopen(dest_file,"w");
    	while((each=(fgetc(file1)))!=EOF){
      	fputc(each,file2);
        }
        fclose(file2);
       
    }

    else if(strcmp(option,"-n")==0){
                     
            file2=fopen(dest_file,"r");
            if(file2 != NULL){
                fclose(file2);
                fclose(file1);
                return 0; 
            }
            else{
             file2=fopen(dest_file,"w");
              while((each=fgetc(file1))!=EOF){
                 fputc(each,file2);  
              }
              fclose(file2);
            }         
    }         

     else if(strcmp(option,"-v")==0){
      
            file2=fopen(dest_file,"w");

            while((each=fgetc(file1))!=EOF){
                 fputc(each,file2);  
              }
               fclose(file2);  

         printf("%s -> %s\n",source_file,dest_file); 

     }

    else if(strcmp(option,"-f")==0){
         
         file2=fopen(dest_file,"w");
        while((each=(fgetc(file1)))!=EOF){
        fputc(each,file2);
        }
        fclose(file2);

    }     

  }
     fclose(file1);
          
                     
      return 0;
  }