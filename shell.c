#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_SIZE 256

int main(int argc, char* argv[])
{
    // NOTE: strtok(char * str, " ") 
    //
    /* Variable declarations */
    char command[MAX_SIZE];
    char* commandArgs =  malloc(256); /* Max number */
    int childPid = 0;
    char* foo;
    
    while(1) /* Repeat forever */ {
        /* Display a prompt */
        printf("myShell%% ");

        /* Read a command from the user */
        fgets(command,sizeof(command), stdin);   

        /* Parse the string into the command portion and
         * an array of pointers to argument strings using the blank
         * character as the separator. Note: the first argument
         * in the array of argument strings must be the same as
         * the command and the array must have a NULL pointer
         * after the last argument.
         */
        //puts(command);

        /* If the user entered 'exit' then call the exit() system call
         * to terminate the process
         */
        foo = strtok(command, " ");
        
        while (foo) {
            strcat(commandArgs, foo);
            strcat(commandArgs, " ");
            puts(commandArgs);
            foo = strtok(NULL, " ");
        }

        /* Fork a child process to execute the command and return 
         * the result of the fork() in the childPid variable so 
         * we know whether we're now executing as the parent 
         * or child and whether or not the fork() succeeded
         */
        childPid = fork();


        if (!childPid) /* We forked no child, we ARE the child */ {
            /* We're now executing in the context of the child process.
             * Use execvp() or execlp() to replace this program in 
             * the process' memory with the executable command the 
             * user has asked for.  
             */
            printf("command: %s\ncommandArgs: %s", command, commandArgs);
            system(commandArgs);
            //            return execv(command, &commandArgs);
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
            wait(NULL);
        }
        
        // Reset the command args pointer.
        *commandArgs = '\0';

    } /* while */

} /* my shell */
