#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SIZE 4096

int main(int argc, char* argv[])
{
    // NOTE: strtok(char * str, " ") 
    //
    /* Variable declarations */
    char command[MAX_SIZE];
    char* commandArgs[MAX_SIZE]; /* Max number */
    int childPid = 0;
    int blnRunBackground = 0;
    int blnIsCwd = 0;
    int blnIsCd = 0;
    char* foo;
    char* returnPointer;
    int i = 0;
    
    while(1) /* Repeat forever */ {
        i = 0;
        /* Display a prompt */
        printf("myShell%% ");

        /* Read a command from the user */
        fgets(command, MAX_SIZE, stdin);

        /* Parse the string into the command portion and
         * an array of pointers to argument strings using the blank
         * character as the separator. Note: the first argument
         * in the array of argument strings must be the same as
         * the command and the array must have a NULL pointer
         * after the last argument.
         */

        foo = strtok(command, " ");
        
        while (foo) {
            
            // If the last character in the string is a return, throw it away.
            commandArgs[i] = foo;
            
            if ((returnPointer = strchr(commandArgs[i++], '\n')) != NULL)
            {
                *returnPointer = '\0';
            }
            
            // If the current string is an '&' then we need to print the
            // job ID so set our flag.
//            if (strcmp(foo, "&") == 0) {
//                blnRunBackground = -1;
//            }
//            else
//            {
//                strcat(**commandArgs, foo);
//            }
            
//            if (strcmp(foo, "cd") == 0) {
//                blnIsCd = -1;
//            }
//            if (strcmp(foo, cwd) == 0) {
//                blnIsCwd = -1;
//            }
            
            foo = strtok(NULL, " ");
        }
        
        /* If the user entered 'exit' then call the exit() system call
         * to terminate the process
         */

        /* Fork a child process to execute the command and return 
         * the result of the fork() in the childPid variable so 
         * we know whether we're now executing as the parent 
         * or child and whether or not the fork() succeeded
         */
        if (!(blnIsCwd || blnIsCwd)) {
            childPid = fork();
        }

        if (!childPid) /* We forked no child, we ARE the child */ {
            /* We're now executing in the context of the child process.
             * Use execvp() or execlp() to replace this program in 
             * the process' memory with the executable command the 
             * user has asked for.  
             */
            
//            commandArgs[2] = NULL;
//            puts("commandArgs[0]:");
//            puts(commandArgs[0]);
//            puts("commandArgs[1]:");
//            puts(commandArgs[1]);
//            puts("commandArgs[2]:");
//            puts(commandArgs[2]);
            
//            execvp(cmd, args);
            execvp(*commandArgs, commandArgs);
            
            return 0;
//            return execlp(command, commandArgs);
        }
        else if (childPid < 0) {
            /* An error occured during the fork - print it */
            printf("Error: %d\n", childPid);
            childPid = 0;
        }
        else /* childPid is the PID of the child */ {
            /* We're still executing in the parent process.
             * Wait for the child to finish before we prompt
             * again.
             */
            if (blnRunBackground) {
                printf("Job %d\n", childPid);
            }
            else if (blnIsCd) {
                //call our cd command
            }
            else {
                wait(NULL);
            }
        }
        
        // Reset the command args pointer.
        blnRunBackground = 0;
        blnIsCwd = 0;

    } /* while */

} /* my shell */
