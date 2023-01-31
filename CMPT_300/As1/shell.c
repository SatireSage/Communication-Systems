// Shell starter file
// You may make any changes to any part of this file.
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define COMMAND_LENGTH 1024
#define HISTORY_DEPTH 10
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
char history [HISTORY_DEPTH][COMMAND_LENGTH];
int command_num=0;  

/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
    int token_count = 0;
    _Bool in_token = false;
    int num_chars = strnlen(buff, COMMAND_LENGTH);
    for (int i = 0; i < num_chars; i++)
    {
        switch (buff[i])
        {
        // Handle token delimiters (ends):
        case ' ':
        case '\t':
        case '\n':
            buff[i] = '\0';
            in_token = false;
            break;

        // Handle other characters (may be start)
        default:
            if (!in_token)
            {
                tokens[token_count] = &buff[i];
                token_count++;
                in_token = true;
            }
        }
    }
    tokens[token_count] = NULL;
    return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of
 * tokens). in_background: pointer to a boolean variable. Set to true if user
 * entered an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
    *in_background = false;

    // Read input
    int length = read(STDIN_FILENO, buff, COMMAND_LENGTH - 1);

    if (length < 0)
    {
        perror("Unable to read command from keyboard. Terminating.\n");
        exit(-1);
    }

    // Null terminate and strip \n.
    buff[length] = '\0';
    if (buff[strlen(buff) - 1] == '\n')
    {
        buff[strlen(buff) - 1] = '\0';
    }

    // Tokenize (saving original command string)
    int token_count = tokenize_command(buff, tokens);
    if (token_count == 0)
    {
        return;
    }

    // Extract if running in background:
    if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0)
    {
        *in_background = true;
        tokens[token_count - 1] = 0;
    }
}

// print history: up to 10 most recent entered commands
    // # user commands starting at 0
    // display commands in DESCENDING ORDER
    // output includes:
        // external application commands and internal commands
    // display command # on the LEFT, arguments on RIGHT
        // print "tab" between 2 outputs to lineup correctly
        // command uses &, must be added to history with &
    // 
void history_print()
{
    // printing history of commands
    // less than 10 commands
    if(command_num < 10)
    {
        printf("printing most recent 10 commands...\n");
        for(int indx = command_num; indx >= 0; indx--) 
        {
            printf("%d \t", indx);
            printf("%s \n", history[indx]);
        }
    }
    // 10+ commands
    else 
    {
        printf("printing most recent 10 commands...\n");
        int count = 0;
        for(int indx = 9; indx >= 0; indx--) 
        {
            printf("%d \t", (command_num-count));
            printf("%s \n", history[indx]);
            count++;
        }
    }

    // running !n command
}
int internal_command(char *buff, char *tokens[], _Bool *in_background)
{
    //exit
    if (strcmp(tokens[0], "exit") == 0) 
    { 
		return -1;
	}
    else if (strcmp(tokens[0], "history") == 0)
    {
        int command_number=command_num % HISTORY_DEPTH; 
        strcpy(history[command_number], tokens[0]); 
        command_num++;
        history_print(); 
        return 1; 

    }
    else if(strcmp(tokens[0], "cd") == 0)
    {
        //if chdir returns error
        if (chdir(tokens[1])!=0)
        {
            write(STDOUT_FILENO, "Error: Could not change directory.", strlen("Error: Could not change directory."));
        }
        //ADD COMMAND TO THE HISTORY
        if (tokens[1]!=NULL)
        {
            int command_number=command_num % HISTORY_DEPTH;
            strcat(tokens[0], " "); 
            strcat (tokens[0] ,tokens[1]); 
            strcpy(history[command_number], tokens[0]); 
            command_num++;       
        }
        return 1; 
    }
    else if(strcmp(tokens[0], "pwd") == 0)
    {
        char curr_dir[COMMAND_LENGTH]; 
        char *current_dir = getcwd(curr_dir, sizeof(curr_dir)); 
        if (current_dir !=NULL)
        {
            write(STDOUT_FILENO, curr_dir, strlen(curr_dir));
        } 
        else 
        {
            write(STDOUT_FILENO, "Error: Could not display current directory.", strlen("Error: Could not display current directory."));
        }
        //add command to the history 
        int command_number=command_num % HISTORY_DEPTH; 
        strcpy(history[command_number], tokens[0]); 
        command_num++;

        return 1; 
        
    }
    else if(strcmp(tokens[0], "help") == 0)
    {   
        if ((tokens[2])!= NULL)
        {
            write(STDOUT_FILENO, "Error: More than 1 argument.\n", strlen("Error: More than 1 argument.\n"));  
        }
        if ((tokens[1])== NULL)
        {
            write(STDOUT_FILENO, "1. help\nDisplays information about the passed command. If no argument provided then it displays all the commands if no argument is provided. Takes only one argument.\n", strlen("1. help\nDisplays information about the passed command. If no argument provided then it displays all the commands if no argument is provided. Takes only one argument.\n"));
            write(STDOUT_FILENO, "2. exit\nexit: Exits the shell. Takes no other arguments.\n", strlen("2. exit\n exit: Exits the shell. Takes no other arguments.\n"));
            write(STDOUT_FILENO, "3. pwd\nPrints the current working directory. Takes no other arguments.\n", strlen("3. pwd\nPrints the current working directory. Takes no other arguments.\n"));
            write(STDOUT_FILENO, "4. cd\nChanges the current working directory to the directory specified. Takes only one argument.\n", strlen("4. cd\nChanges the current working directory to the directory specified. Takes only one argument.\n"));
            write(STDOUT_FILENO, "5. history\nDisplays the 10 most recent commands entered. If less than 10 commands were entered then only those are displayed. Takes no other arguments.", strlen("5. history\nDisplays the 10 most recent commands entered. If less than 10 commands were entered then only those are displayed. Takes no other arguments."));

        }
        else if (strcmp(tokens[1], "exit") == 0)
        {
            write(STDOUT_FILENO, "'exit' is a builtin command for quitting the shell.\n", strlen("'exit' is a builtin command for quitting the shell.\n"));
        }
        else if (strcmp(tokens[1], "cd") == 0)
        {
            write(STDOUT_FILENO, "'cd' is a builtin command for changing the directory.\n", strlen("'cd' is a builtin command for changing the directory.\n"));
        }
        else if (strcmp(tokens[1], "pwd") == 0)
        {
            write(STDOUT_FILENO, "'pwd' is a builtin command for finding the current path.\n", strlen("'pwd' is a builtin command for finding the current path.\n"));
        }
        else if (strcmp(tokens[1], "history") == 0)
        {
            write(STDOUT_FILENO, "'history' is a builtin command for showing the 10 most recent commands.\n", strlen("'history' is a builtin command for showing the 10 most recent commands.\n"));
        }
        else 
        {
            char arr[100];
            sprintf(arr, "%s", tokens[1]);
            strcat(arr, " is an external command.\n"); 
            write(STDOUT_FILENO,(arr), strlen(arr));
        }
        //add command to history
        int command_number=command_num % HISTORY_DEPTH;
        strcat(tokens[0], " "); 
        if (tokens[1] !=NULL)
        {
            strcat (tokens[0] ,tokens[1]); 
        }
        strcpy(history[command_number], tokens[0]); 
        command_num++; 

        return 1; 
    }
    command_num++; 
    return 0; 
}
void get_input()
{
    write(STDOUT_FILENO, "$ ", strlen("$ "));
    char curr_dir[COMMAND_LENGTH]; 
    char *current_dir = getcwd(curr_dir, sizeof(curr_dir)); 
    if (current_dir !=NULL)
    {
        write(STDOUT_FILENO, curr_dir, strlen(curr_dir));
    } 

}
void handle_SIGINT(int sig)
{
	get_input();
}
void signal_handling()
{
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);
}

/**
 * Main and Execute Commands
 */
int main(int argc, char *argv[])
{
    char input_buffer[COMMAND_LENGTH];
    char *tokens[NUM_TOKENS];
    signal_handling();
    while (true)
    {

        // Get command
        // Use write because we need to use read() to work with
        // signals, and read() is incompatible with printf().
        get_input(); 

        _Bool in_background = false;
        read_command(input_buffer, tokens, &in_background);
   

        // DEBUG: Dump out arguments:
        for (int i = 0; tokens[i] != NULL; i++)
        {
            write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
            write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
            write(STDOUT_FILENO, "\n", strlen("\n"));
        }
        if (in_background)
        {
            write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
        }
        if (input_buffer[0] == '\0' || tokens[0] == NULL) {
			continue;
		}
        int command = internal_command(input_buffer, tokens, &in_background);
        if (command==-1)
        {
            return 0; 
        }
        if (command==1)
        {
            continue; 
        }
        
        /**
         * Steps For Basic Shell:
         * 1. Fork a child process
         * 2. Child process invokes execvp() using results in token array.
         * 3. If in_background is false, parent waits for
         *    child to finish. Otherwise, parent loops back to
         *    read_command() again immediately.
         */
        pid_t Pid=fork(); 
        //<-1, which means to wait for any child process whose process group ID is equal to the absolute value of pid.
        if (Pid < 0)
        {
            write(STDERR_FILENO, "Error: fork failed\n", strlen("Error: fork failed\n"));
        }
        // 0 means to wait for any child process whose process group ID is equal to that of the calling process
        else if (Pid == 0)
        {
            if (execvp(tokens[0], tokens) == -1)
            {
                write(STDERR_FILENO, "Unsuccesful call to execvp\n", strlen("Unsuccesful call to execvp\n"));
            }
            exit(0); 
        }
        if (!in_background)
        {
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }
      
    }
    return 0;
}