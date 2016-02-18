#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Max number */
#define MAX_SIZE 4096

int main(int argc, char* argv[]) {
    
    /* Variable declarations */
    char* commandArgs[MAX_SIZE];
    char command[MAX_SIZE];
    
    char* _shell_name = "seashell";
    char* _old_pwd = NULL;
    char* _pwd = NULL;
    char* return_pointer;
    char* cmd_exit = "exit";
    char* cmd_cwd = "cwd";
    char* cmd_pwd = "pwd";
    char* cmd_cd = "cd";
    char* cmd_last_dir = "-";
    char* cmd_home_dir = "~";
    char* amp = "&";
    char* up_dir = "..";
    char* arg;
    
    char RETURN_CHAR = '\n';
    char NULL_CHAR = '\0';
    
    int bln_run_background = 0;
    int size_of_args = 0;
    int bln_is_cwd = 0;
    int bln_is_pwd = 0;
    int bln_is_cd = 0;
    int child_pid = 0;
    int i = 0;
    
    // Default the OLDPWD to be the HOME directory.
    _old_pwd = getenv("HOME");
    setenv("OLDPWD", _old_pwd, 1);
    
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
            
            // If the last character in the string is a return, change it
            // to be a line terminator. YOU HAVE BEEN TERMINATED.
            if ((return_pointer = strchr(arg, RETURN_CHAR)) != NULL) {
                *return_pointer = NULL_CHAR;
            }
            
            // If the current string is an '&' then we need to print the
            // job ID so set our flag.
            if (!strcmp(arg, amp)) {
                bln_run_background = -1;
            }
            else {
                // Save the token to our char * array.
                commandArgs[i++] = arg;
                
                if (!strcmp(arg, cmd_cd)) {
                    bln_is_cd = -1;
                }
                else if (!strcmp(arg, cmd_cwd)) {
                    bln_is_cwd = -1;
                }
                else if (!strcmp(arg, cmd_pwd)) {
                    bln_is_pwd = -1;
                }
            }
            
            arg = strtok(NULL, " ");
        }
        
        /* If the user entered 'exit' then call the exit() system call
         * to terminate the process
         */
        if (!strcmp(commandArgs[0], cmd_exit)) {
            exit(0);
            return 0;
        }

        /* Fork a child process to execute the command and return 
         * the result of the fork() in the child_pid variable so 
         * we know whether we're now executing as the parent 
         * or child and whether or not the fork() succeeded
         */
        if (!(bln_is_cwd || bln_is_cd || bln_is_pwd)) {
            child_pid = fork();
        }
        
        /* We forked no child, we ARE the child */
        if (!(bln_is_cd || bln_is_pwd || bln_is_cwd || child_pid)) {
            /* We're now executing in the context of the child process.
             * Use execvp() or execlp() to replace this program in 
             * the process' memory with the executable command the 
             * user has asked for.  
             */
            execvp(*commandArgs, commandArgs);
        }
        /* An error occured during the fork - print it */
        else if (child_pid < 0) {
            printf("Error: %d%c", child_pid, RETURN_CHAR);
            child_pid = 0;
        }
        /* child_pid is the PID of the child */
        else  {
            /* We're still executing in the parent process.
             * Wait for the child to finish before we prompt
             * again.
             */
            // First, check if we are going to run this in the
            // background, if so print the pid.
            if (bln_run_background) {
                printf("Job %d%c", child_pid, RETURN_CHAR);
            }
            // If CD is called, run CD.
            if (bln_is_cd) {
                
                // Change to the Home directory if no args or args match "~".
                if (!commandArgs[1] || !strcmp(commandArgs[1], cmd_home_dir)) {
                    
                    // Get the HOME ENV variable.
                    // Get the current PWD.
                    _pwd = getenv("HOME");
                    _old_pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Change the current working directory.
                    chdir(_pwd);
                    
                    // Updated our ENV variables.
                    setenv("OLDPWD", _old_pwd, 1);
                    setenv("PWD", _pwd, 1);
                }
                // Change back to the last dir.
                else if (!strcmp(commandArgs[1], cmd_last_dir)) {
                
                    // Save OLDPWD, PWD.
                    _pwd = getenv("OLDPWD");
                    _old_pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Updated the current working directory and
                    // get the updated current working directory.
                    chdir(_pwd);
                    _pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Overwrite OLDPWD with new PWD and the PWD
                    // with the new OLDPWD.
                    setenv("OLDPWD", _old_pwd, 1);
                    setenv("PWD", _pwd, 1);
                }
                // Change to up a directory.
                else if (!strcmp(commandArgs[1], up_dir)) {
                    
                    // Save OLDPWD.
                    _old_pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Updated the current working directory and
                    // get the updated current working directory.
                    chdir(commandArgs[1]);
                    _pwd = getcwd(NULL, MAX_SIZE);

                    // Updated our ENV variables.
                    setenv("OLDPWD", _old_pwd, 1);
                    setenv("PWD", _pwd, 1);
                }
                else {
                    
                    // Save OLDPWD, PWD.
                    _pwd = commandArgs[1];
                    _old_pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Updated the current working directory and
                    // get the updated current working directory.
                    chdir(_pwd);
                    _pwd = getcwd(NULL, MAX_SIZE);
                    
                    // Updated our ENV variables.
                    setenv("OLDPWD", _old_pwd, 1);
                    setenv("PWD", _pwd, 1);
                }
                printf("%s%c", _pwd, RETURN_CHAR);
            }
            // If PWD is called, run PWD.
            else if (bln_is_pwd) {
                _pwd = getenv("PWD");
                printf("%s%c", _pwd, RETURN_CHAR);
            }
            else {
                wait(NULL);
            }
            
            
//            _old_pwd = getenv("OLDPWD");
//            _pwd = getenv("PWD");
//                              
//            printf("PWD: %s\n", _pwd);
//            printf("OLDPWD: %s\n", _old_pwd);
            
            size_of_args = sizeof(commandArgs[0]);
            
            // Clear out our previous arguments.
            for (int i = 0; i < size_of_args; i++) {
                commandArgs[i] = NULL;
            }
            
            size_of_args = 0;
        }
        
        // Reset the command args pointer.
        bln_run_background = 0;
        bln_is_cwd = 0;
        bln_is_pwd = 0;
        bln_is_cd = 0;

    } /* while */
    
    return 0;

} /* my shell */
