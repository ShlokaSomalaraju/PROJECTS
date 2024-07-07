#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define MAX_LINES 1000
#define MAX_LENGTH 1024


int do_grep(char *pattern,char *filename,char *option){ 


    FILE* file;
    
    char each;
    char all_line[MAX_LINES][MAX_LENGTH];
    int line_index = 0, char_index = 0;
    int match_line_count=0;
    file=fopen(filename,"r");

 if(file==NULL){printf("Error to read file\n");}
 else{   

    while((each=fgetc(file))!=EOF){
        if(each=='\n'){
            all_line[line_index][char_index] = '\0';
            line_index++;
            char_index = 0; 
        }
        else{
            all_line[line_index][char_index] = each ; 
            char_index++;
        }
        
    }

    all_line[line_index][char_index] = '\0';
    line_index++;

    fclose(file);


   if(strcmp(option,"x")==0){
      for(int j=0;j < line_index ;j++){
         if(strstr(all_line[j],pattern)!=NULL){
            printf("%s\n",all_line[j]);
         }
      }
   }  

   else if(strcmp(option,"-c")==0){
    for(int j=0;j< line_index;j++){
         if(strstr(all_line[j],pattern)!=NULL){
            match_line_count++;
         }

    }
      printf("%d\n",match_line_count);
   }

   else if(strcmp(option,"-v")==0){
      for(int j=0;j < line_index ;j++){
         if(strstr(all_line[j],pattern)==NULL){
            printf("%s\n",all_line[j]);
         }
      }

   }


   else if(strcmp(option,"-n")==0){
        for(int j=0;j < line_index ;j++){
         if(strstr(all_line[j],pattern)!=NULL){
            printf("%d: %s\n",j+1,all_line[j]);
         }
      }
   }


   else if(strcmp(option,"-i")==0){
        for(int j=0;j < line_index ;j++){
         if(strcasestr(all_line[j],pattern)!=NULL){
            printf("%s\n",all_line[j]);
         }
      }
   }


   

 }  

return 0;   


}   
 
