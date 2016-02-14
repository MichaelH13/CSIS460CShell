#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Max number */
#define MAX_SIZE 4096

void CD(char* options, char* directory) {
    char path[MAX_SIZE];
    char CWD[MAX_SIZE];
    char* up = "..";
    
    if (strcmp(up,args)) {
        strcpy(path, args);
        getcwd(CWD, sizeof(CWD));
    
        strcat(CWD, "/");
        strcat(CWD, path);
    }
    else {
        *CWD = *up;
    }
    
    chdir(CWD);
    printf("%s\n", CWD);
}

int main(int argc, char* argv[]) {
    
    /* Variable declarations */
    char* commandArgs[MAX_SIZE];
    char command[MAX_SIZE];
    
    char* presentWorkingDirectory[MAX_SIZE];
    char* _shell_name = "seashell";
    char* strExit = "exit";
    char* returnPointer;
    char* CWD = "cwd";
    char* PWD = "pwd";
    char* _last_dir;
    char* cd = "cd";
    char* amp = "&";
    char* arg;
    
    char returnChar = '\n';
    char nullChar = '\0';
    
    int blnRunBackground = 0;
    int blnIsCWD = 0;
    int blnIsPWD = 0;
    int blnIsCD = 0;
    int childPid = 0;
    int i = 0;
    
    while(1) /* Repeat forever */ {
        
        i = 0;
        
        /* Display a prompt */
        printf("%s%% ", _shell_name);

        /* Read a command from the user */
        fgets(command, MAX_SIZE, stdin);

        /* Parse the string into the command portion and
         * an array of pointers to argument strings using the blank
         * character as the separator. Note: the first argument
         * in the array of argument strings must be the same as
         * the command and the array must have a NULL pointer
         * after the last argument.
         */
        arg = strtok(command, " ");
        
        while (arg) {
            
            // If the last character in the string is a return, throw it away.
            if ((returnPointer = strchr(arg, returnChar)) != NULL) {
                *returnPointer = nullChar;
            }
            
            // If the current string is an '&' then we need to print the
            // job ID so set our flag.
            if (strcmp(arg, amp) == 0) {
                blnRunBackground = -1;
            }
            else {
                // Save the token to our char * array.
                commandArgs[i++] = arg;
                
                if (strcmp(arg, cd) == 0) {
                    blnIsCD = -1;
                }
                else if (strcmp(arg, CWD) == 0) {
                    blnIsCWD = -1;
                }
                else if (strcmp(arg, PWD) == 0) {
                    blnIsPWD = -1;
                }
            }
            
            arg = strtok(NULL, " ");
        }
        
        /* If the user entered 'exit' then call the exit() system call
         * to terminate the process
         */
        if (!strcmp(commandArgs[0], strExit)) {
            exit(0);
        }

        /* Fork a child process to execute the command and return 
         * the result of the fork() in the childPid variable so 
         * we know whether we're now executing as the parent 
         * or child and whether or not the fork() succeeded
         */
        if (!(blnIsCWD || blnIsCD || blnIsPWD)) {
            childPid = fork();
        }
        
        /* We forked no child, we ARE the child */
        if (!(blnIsCD || blnIsPWD || blnIsCWD || childPid)) {
            /* We're now executing in the context of the child process.
             * Use execvp() or execlp() to replace this program in 
             * the process' memory with the executable command the 
             * user has asked for.  
             */
            
//            puts("commandArgs[0]:");
//            puts(commandArgs[0]);
//            puts("commandArgs[1]:");
//            puts(commandArgs[1]);
//            puts("commandArgs[2]:");
//            puts(commandArgs[2]);
            
            execvp(*commandArgs, commandArgs);
        }
        /* An error occured during the fork - print it */
        else if (childPid < 0) {
            printf("Error: %d%c", childPid, returnChar);
            childPid = 0;
        }
        /* childPid is the PID of the child */
        else  {
            /* We're still executing in the parent process.
             * Wait for the child to finish before we prompt
             * again.
             */
            if (blnRunBackground) {
                printf("Job %d%c", childPid, returnChar);
            }
            else if (blnIsCD) {
                // Call our cd command
                CD(commandArgs[1], commandArgs[2]);
            }
            else if (blnIsPWD) {
                // Call our PWD command
                *presentWorkingDirectory = getcwd(*presentWorkingDirectory, sizeof(*presentWorkingDirectory));
                printf("%s%c", *presentWorkingDirectory, returnChar);
            }
            else {
                wait(NULL);
            }
        }
        
        // Reset the command args pointer.
        blnRunBackground = 0;
        blnIsCWD = 0;
        blnIsPWD = 0;
        blnIsCD = 0;

    } /* while */
    
    return 0;

} /* my shell */
