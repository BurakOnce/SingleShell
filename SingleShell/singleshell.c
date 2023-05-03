#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
/*
Burak Önce - 20120205052
Meryem Talay - 20120205039
Single Shell
Commands;
exit -> to terminate the program
ls -> to find your location
copy -> to write the word in a txt file (if the file does not exist, it saves word, creates file and copy word to file.)
sudo -> to setting password for admin mode (we couldn't do admin mod)
cat -> to show file content
delete -> to delete file
*/

void ls(const char *way1, int hidden_takes, int details_takes);
/*for the ls command, help was taken from opengenus and implemented
for the initmem implements chatgpt used*/

/* these should be the same as multishell.c */
#define MY_FILE_SIZE 1024
#define MY_SHARED_FILE_NAME "/sharedlogfile"

char *addr = NULL;
int fd = -1;
int initmem()
{
    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);
    if (fd < 0){
        perror("singleshell.c:fd:line31");
        exit(1);
    }
    addr = mmap(NULL, MY_FILE_SIZE,
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL){
        perror("singleshell.c:mmap:");
        close(fd);
        exit(1);
    }
    return 0;
}
int main() {
    char command[100];
    while(1){ //shell start
      
    printf("$");
    
        /*get command*/
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

      /*if elses*/
      /*check what command was received and necessary action is taken*/
    if (strcmp(command, "copy") == 0) {
          fprintf(stdout, "#Copy to File ");
        fgets(command, sizeof(command), stdin);
      /*to avoid the newline character problem*/
        command[strcspn(command, "\n")] = '\0'; 

        
        FILE *f = fopen("for_copy_file.txt", "a");
      /*write to file*/
        fprintf(f, "%s\n", command);
        fclose(f);
        }
      else if(strcmp(command, "exit") == 0){
            exit(0);
      }
        else if(strcmp(command, "sudo") == 0){
        fprintf(stdout, "#set password\n");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; 
          fprintf(stdout, "#password set\n");
      }
          
    else if(strcmp(command, "delete") == 0){
          int ds = remove("for_copy_file.txt");
          if(ds == 0)
          {
          fprintf(stdout, "#file deleted\n");
          } 
          else
          {
          fprintf(stderr, "#file could not be deleted\n");
          return -1;
          }
        }
        else if(strcmp(command, "ls") == 0){
          int argc;
          char const *argv;
          const char *way1 = ".";
          int hidden_takes = 0;
          int details_takes = 0;
          if (argc == 2) {
          const char *option = argv;

          while (*option) 
          {
            switch (*option) {
                case 'a':
                    hidden_takes = 1;
                    break;
                case 'l':
                    details_takes = 1;
                    break;
                default:
                    fprintf(stderr, "Invalid option '%c'\n", *option);
                    exit(EXIT_FAILURE);
            }
            option++;
          }
    } else if (argc > 2) {
        /*fprintf(stderr, "Usage: %s [-al]\n", argv[0]);*/
        exit(EXIT_FAILURE);
    }
    ls(way1, hidden_takes, details_takes);
            }

          else if(strcmp(command, "cat") == 0){
            FILE *file = fopen("for_copy_file.txt", "r");
          while(! feof(file) ){
            putchar(fgetc(file));
               }
            fclose(file);
          
          }
    else{
      printf("error:command not found\n");
    }
  }
    munmap(addr, 1024);
    shm_unlink(MY_SHARED_FILE_NAME);
    return 0;
}

void ls(const char *way1, int hidden_takes, int details_takes) {
    struct dirent *dirent_1;
    DIR *directory = opendir(way1);
    if (!directory) 
    {
        if (errno == ENOENT) 
        {
            fprintf(stderr, "firectory '%s' not found\n", way1);
        } 
        else 
        {
            perror("failed to open directory");
        }
        exit(EXIT_FAILURE);
    }
    while ((dirent_1 = readdir(directory)) != NULL) 
    {
        if (!hidden_takes && dirent_1->d_name[0] == '.')
        {
            continue;
        }
            printf("%s", dirent_1->d_name);
        if (details_takes) 
        {
            printf("\n");
        } else {
            printf("  ");
        }
    }
    if (!details_takes) {
        printf("\n");
    }
    closedir(directory);
}
