// Shell starter file
// You may make any changes to any part of this file.
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define COMMAND_LENGTH 1024
#define HISTORY_NUM 10
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
char history [HISTORY_NUM][COMMAND_LENGTH];
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
int internal_command(char *buff, char *tokens[], _Bool *in_background)
{
    //exit
    if (strcmp(tokens[0], "exit") == 0) 
    { 
		return -1;
	}
    else if(strcmp(tokens[0], "cd") == 0)
    {

    }
    else if(strcmp(tokens[0], "pwd") == 0)
    {

    }
    else if(strcmp(tokens[0], "help") == 0)
    {

    }
    return 0; 
}
/**
 * Main and Execute Commands
 */
int main(int argc, char *argv[])
{
    char input_buffer[COMMAND_LENGTH];
    char *tokens[NUM_TOKENS];
    while (true)
    {

        // Get command
        // Use write because we need to use read() to work with
        // signals, and read() is incompatible with printf().
        write(STDOUT_FILENO, "$ ", strlen("$ "));
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
        if (Pid <= -1)
        {
            write(STDERR_FILENO, "Error: fork failed\n", strlen("Error: fork failed\n"));
        }
        // 0 means to wait for any child process whose process group ID is equal to that of the calling process
        else if (Pid == 0)
        {
            if (execvp(tokens[0], tokens) == -1)
            {
                write(STDERR_FILENO, "Unsuccesful call to execvp\n", strlen("Unsuccesful call to execvp\n"));
                exit(-1); 
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