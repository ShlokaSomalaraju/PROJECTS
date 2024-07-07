#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

    char user[50];
    char hostname[50];
    char path[100];
    char prompt[300];

char* display(mode_t mode);
void do_ls(char *token,int check);
int do_grep(char *pattern,char *filename,char *option);
int do_cp(char *source_file ,char *dest_file,char *option);
int do_mv(char *source_file ,char *dest_file,char *option);

char* promptfunc(){
    
    getlogin_r(user,sizeof(user));
     strcpy(prompt,user);
     strcat(prompt,"@");
    gethostname(hostname,sizeof(hostname));
     strcat(prompt,hostname);
     strcat(prompt,":");
    getcwd(path,sizeof(path));
     strcat(prompt,path);
     strcat(prompt," $ ");
   return prompt;

}

int countNonspaces(char *str) {
    int count = 0;
    while (*str) {
        if (!isspace(*str)) {
            count++;
        }
        str++;
    }
    return count;
} 

int main(){
    
    
   struct dirent *de;
   DIR *dr;
   struct stat file_stat;
   char *input;
   int check=0;


   while(1){
 
    input=readline(promptfunc());

    if(countNonspaces(input)==0){continue;}
    else{add_history(input);} 

      char store_command[200];
      strcpy(store_command,input);
      char *token = strtok(input," ");


      if(strcmp(token,"exit")==0){break;}

      else if(strcmp(token,"ls")==0){

         token=strtok(NULL," ");
         if(token!=NULL){
         check++;
         do_ls(token,check);
         }
         else{
            do_ls(token,check);}
      }


      else if(strcmp(token,"cp")==0){


        char source_file[100];
          char dest_file[100];
          char option[5];
          char store_tokens[10][100];

          token=strtok(NULL," ");
         
          if(strncmp(token,"-",1)==0){
             strcpy(option,token);
             token=strtok(NULL," ");
           }
           else if(strncmp(token,"-",1)!=0){
              strcpy(option,"x");
           }

                  int i=0;
                
               while(token!=NULL){
                   strcpy(store_tokens[i],token);
                   token=strtok(NULL," ");
                   i++; 
               }
              
               for(int j=0;j<i-1;j++){
                  strcpy(source_file,store_tokens[j]);
                  strcpy(dest_file,store_tokens[i-1]); 
                  do_cp(source_file,dest_file,option);
                  
               } 

      }
  

     else if(strcmp(token,"mv")==0){

          char source_file[100];
          char dest_file[100];
          char option[5];
          char store_tokens[10][100];

          token=strtok(NULL," ");
         
          if(strncmp(token,"-",1)==0){
             strcpy(option,token);
             token=strtok(NULL," ");
           }
           else if(strncmp(token,"-",1)!=0){
              strcpy(option,"x");
           }

                  int i=0;
                
               while(token!=NULL){
                   strcpy(store_tokens[i],token);
                   token=strtok(NULL," ");
                   i++; 
               }
              
               for(int j=0;j<i-1;j++){
                  strcpy(source_file,store_tokens[j]);
                  strcpy(dest_file,store_tokens[i-1]); 
                  do_mv(source_file,dest_file,option);
                  
               } 

     }


    else if(strcmp(token,"grep")==0){
  

           char option[5];
           char pattern[100];
          char filename[500];

           token=strtok(NULL," ");
           if(strncmp(token,"-",1)==0){
             strcpy(option,token);
             token=strtok(NULL," ");
           }
           else if(strncmp(token,"-",1)!=0){
              strcpy(option,"x");
           }
 
        
         char *ptr=strchr(store_command,'"');
         char *ptr2;

         char second_input[500];


          if(ptr!=NULL){
               
              ptr2=strchr(ptr+1,'"');
              if(ptr2==NULL){printf("error\n");}
              else{ memcpy(pattern,ptr+1,(ptr2-ptr)-1);
              pattern[ptr2-ptr]='\0'; }
              strcpy(second_input,ptr2);
             


            char *token2 = strtok(second_input," ");   
            token2=strtok(NULL," ");
            char store_tokens[10][1024];
            int i=0;

              while(token2!=NULL){
                   strcpy(store_tokens[i],token2);
                   token2=strtok(NULL," ");
                   i++; 
               }
              for(int j=0;j<i;j++){
                                    
                  strcpy(filename,store_tokens[j]);
                  
                  do_grep(pattern,filename,option);
                                    
              }     


          } 

                      
          else {

              strcpy(pattern,token);
              
              token=strtok(NULL," ");
              char store_tokens2[10][1024];
              int i=0;
              while(token!=NULL){
                   strcpy(store_tokens2[i],token);
                   token=strtok(NULL," ");
                   i++; 
               }
              for(int j=0;j<i;j++){
                                   
                  strcpy(filename,store_tokens2[j]);
                  
                  do_grep(pattern,filename,option);
                                    
              }     

          }

    }
         
   }
  free(input);
   return 0;

}
