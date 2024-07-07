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
#include <strings.h>
#include <sys/stat.h>


struct dirent *de;
DIR *dr;
struct file{
            struct stat file_stat;
            char filename[100];
          };

char* display(mode_t mode) {
             char *rwx = (char*)malloc(10);

     sprintf(rwx, "%c%c%c%c%c%c%c%c%c%c",
        (S_ISDIR(mode))  ? 'd' : '-',
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-'
    );

    return rwx;
}

int filenamesorting(const void* m,const void* n){
  struct file* m_file = (struct file*) m;
  struct file* n_file = (struct file*) n;
  return strcasecmp(m_file->filename,n_file->filename);
}

int reverseOfls(const void* m,const void* n){
  struct file* m_file = (struct file*) m;
  struct file* n_file = (struct file*) n;
  return strcasecmp(n_file->filename,m_file->filename);
}

int filesizeSorting(const void* m,const void* n){
  struct file* m_file = (struct file*) m;
  struct file* n_file = (struct file*) n;
  return (n_file->file_stat.st_size)-(m_file->file_stat.st_size);
}

int lastaccessfirst(const void* m,const void* n){
  struct file* m_file = (struct file*) m;
  struct file* n_file = (struct file*) n;
  return (n_file->file_stat.st_atime)-(m_file->file_stat.st_atime);
}



void do_ls(char *token,int check){
  
  int NoOfFiles=0;

         dr = opendir(".");
             if(dr==NULL){
                printf("Could not open current directory");
             }
             while((de=readdir(dr))!=NULL){
                if(de->d_type == DT_DIR || de->d_type == DT_REG ){
                    if(strncmp(de->d_name,".",1)==0 ){
                        continue;
                    }    
                    else{
                        NoOfFiles++;
                    }    
                }
             }
             closedir(dr);
            

          struct file props[NoOfFiles]; 

          char temp_filename[100][100];
       
          int index=0;   
         dr = opendir(".");
         while((de=readdir(dr))!=NULL){
          if(de->d_type == DT_DIR || de->d_type == DT_REG ){
                    if(strncmp(de->d_name,".",1)==0 ){
                        continue;
                    }    
                    else {
           sprintf(temp_filename[index],"./%s",de->d_name);
           stat(temp_filename[index],&props[index].file_stat);
           strcpy(props[index].filename,de->d_name);
           index++;

         }}}  
         closedir(dr);
        
      if(check==0){
        qsort(props,NoOfFiles,sizeof(struct file),filenamesorting);
        for(index=0;index<NoOfFiles;index++){
          printf("%s\n",props[index].filename);
        }
      }

      else if(strcmp(token,"-u")==0){
        qsort(props,NoOfFiles,sizeof(struct file),lastaccessfirst);
        for(index=0;index<NoOfFiles;index++){
          printf("%s\n",props[index].filename);
        }
      } 

      else if(strcmp(token,"-r")==0){
        qsort(props,NoOfFiles,sizeof(struct file),reverseOfls);
        for(index=0;index<NoOfFiles;index++){
          printf("%s\n",props[index].filename);
        }
      } 

      else if(strcmp(token,"-S")==0){
        qsort(props,NoOfFiles,sizeof(struct file),filesizeSorting);
        for(index=0;index<NoOfFiles;index++){
          printf("%s\n",props[index].filename);
        }
      } 
         
      else if(strcmp(token,"-l")==0){
        
             for(index=0;index<NoOfFiles;index++){
                time_t current_time=props[index].file_stat.st_mtime;
                struct tm* time_info;
                char month[4];
                 time(&current_time);
                 time_info = localtime(&current_time);
                 switch(time_info->tm_mon + 1){
                             case 1:
                               strcpy(month,"Jan");
                               break;
                             case 2:
                               strcpy(month,"Feb");
                               break;
                             case 3:
                               strcpy(month,"Mar");
                               break;  
                             case 4:
                               strcpy(month,"Apr");
                               break;
                             case 5:
                               strcpy(month,"May");
                               break;
                             case 6:
                               strcpy(month,"Jun");
                               break;
                             case 7:
                               strcpy(month,"Jul");
                               break;
                             case 8:
                               strcpy(month,"Aug");
                               break;
                             case 9:
                               strcpy(month,"Sep");
                               break;
                             case 10:
                               strcpy(month,"Oct");
                               break;
                             case 11:
                               strcpy(month,"Nov");
                               break;
                             case 12:
                               strcpy(month,"Dec");
                               break;
                 }
                 uid_t uid = getuid(); 
                 gid_t gid = getgid(); 

                    struct passwd *pwd = getpwuid(uid);
                    char *username = pwd->pw_name;
                    struct group *grp = getgrgid(gid);
                    char *groupname = grp->gr_name;

            qsort(props,NoOfFiles,sizeof(struct file),filenamesorting);        
       printf("%s  %d  %s %s %lld   %d %s %d %d:%d %s\n", display(props[index].file_stat.st_mode),
        props[index].file_stat.st_nlink, username, groupname, props[index].file_stat.st_size, time_info->tm_year + 1900,
          month, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, props[index].filename);                
               }        
       }                
              
 }   