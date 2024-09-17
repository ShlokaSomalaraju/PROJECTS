#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

    char user[50];
    char hostname[50];
    char path[50];
    char prompt[200];

//To fetch the details of the command propt of the terminal window opened
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

// To count the number of Non-space characters
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
    
    char *input;
    int outcome;   
       
    while(1){
 
        input=readline(promptfunc());

        // To check whether there is a command or else it isn't added to the command history
        if(countNonspaces(input)==0){
            continue;
        }
        else{
            add_history(input);
        } 
   	
        if(strcmp(input,"exit")==0){
            break;
        }
        else { 
   	        outcome = system(input);
            if(outcome==-1){
            	printf("Invalid command name\n");
            }
        }
    
        free(input);
    }
   
   return 0;
}
