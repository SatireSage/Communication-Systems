// Shell starter file
// You may make any changes to any part of this file.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define PATH_MAX 4096
#define HISTORY_DEPTH 10
char history[HISTORY_DEPTH][COMMAND_LENGTH];
char currentDirectory[PATH_MAX];
char previousDirectory[PATH_MAX];
int commandCounter = 0;
int historySpace = 0;
int itteration = 0;
int invalidFlag = 0;

void color(char *colorValue)
{
  if (strcmp(colorValue, "red") == 0)
    write(STDOUT_FILENO, "\033[1;31m", strlen("\033[1;31m"));
  else if (strcmp(colorValue, "green") == 0)
    write(STDOUT_FILENO, "\033[1;32m", strlen("\033[1;32m"));
  else if (strcmp(colorValue, "yellow") == 0)
    write(STDOUT_FILENO, "\033[1;33m", strlen("\033[1;33m"));
  else if (strcmp(colorValue, "blue") == 0)
    write(STDOUT_FILENO, "\033[1;34m", strlen("\033[1;34m"));
  else if (strcmp(colorValue, "magenta") == 0)
    write(STDOUT_FILENO, "\033[1;35m", strlen("\033[1;35m"));
  else if (strcmp(colorValue, "reset") == 0)
    write(STDOUT_FILENO, "\033[0m", strlen("\033[0m"));
}

void writeString(char *string, char *type)
{
  if (strcmp(type, "error") == 0)
    color("red");
  else if (strcmp(type, "warning") == 0)
    color("yellow");
  else if (strcmp(type, "info") == 0)
    color("green");
  else if (strcmp(type, "header") == 0)
    color("blue");
  else if (strcmp(type, "other") == 0)
    color("magenta");
  else if (strcmp(type, "normal") == 0)
    color("reset");
  write(STDOUT_FILENO, string, strlen(string));
  color("reset");
}

int cd(char *path)
{
  memset(previousDirectory, 0, PATH_MAX);
  strcpy(previousDirectory, currentDirectory);
  return chdir(path);
}
char *pwd()
{
  memset(currentDirectory, 0, PATH_MAX);
  return getcwd(currentDirectory, PATH_MAX);
}

void printHistory()
{
  char indexString[12];
  int loopSize = (commandCounter < HISTORY_DEPTH) ? commandCounter : HISTORY_DEPTH;
  int commandIndex = commandCounter;
  for (int i = loopSize - 1; i >= 0; i--)
  {
    memset(indexString, 0, 12);
    sprintf(indexString, "%d", commandIndex--);
    strcat(indexString, ". ");
    writeString(indexString, "header");
    writeString(history[i], "normal");
    writeString("\n", "normal");
  }
}

void addToHistory(char **command)
{
  char commandString[COMMAND_LENGTH];
  memset(commandString, 0, COMMAND_LENGTH);
  for (int i = 0; command[i] != NULL; i++)
  {
    strcat(commandString, command[i]);
    strcat(commandString, " ");
  }
  commandString[strlen(commandString) - 1] = '\0';

  if (historySpace < HISTORY_DEPTH)
    strcpy(history[historySpace++], commandString);
  else
  {
    for (int i = 0; i < HISTORY_DEPTH - 1; i++)
      strcpy(history[i], history[i + 1]);
    strcpy(history[HISTORY_DEPTH - 1], commandString);
    itteration++;
  }
}

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

  // if (length < 0)
  // {
  //   perror("Unable to read command from keyboard. Terminating.\n");
  //   exit(-1);
  // }

  if ((length < 0) && (errno != EINTR))
  {
    perror("Unable to read command. Terminating.\n");
    exit(-1);
  }
  if (length == -1) // Signal caught hence we dont want to do anything else
    return;

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

/* Signal handler function */
void SIGINT_handler()
{
  invalidFlag = 1;
  writeString("\n", "normal");
  writeString("The following commands are supported:\n", "info");
  writeString("1. help:\n", "info");
  writeString("Displays information about the passed command. If no argument provided then it displays all the commands if no argument is provided. Takes only one argument.\n", "normal");
  writeString("2. exit:\n", "info");
  writeString("exit: Exits the shell. Takes no other arguments.\n", "normal");
  writeString("3. pwd:\n", "info");
  writeString("Prints the current working directory. Takes no other arguments.\n", "normal");
  writeString("4. cd:\n", "info");
  writeString("Changes the current working directory to the directory specified. Takes only one argument.\n", "normal");
  writeString("5. history:\n", "info");
  writeString("Displays the 10 most recent commands entered. If less than 10 commands were entered then only those are displayed. Takes no other arguments.\n", "normal");
}

/**
 * Main and Execute Commands
 */
int main(int argc, char *argv[])
{

  /*Change your shell program to display the help information when the user presses ctrl-c (which is the SIGINT signal)*
   */

  // Register signal handlers
  struct sigaction handler;
  handler.sa_handler = SIGINT_handler;
  handler.sa_flags = 0;
  sigemptyset(&handler.sa_mask);
  sigaction(SIGINT, &handler, NULL);

  char input_buffer[COMMAND_LENGTH];
  char *tokens[NUM_TOKENS];
  while (true)
  {
    invalidFlag = 0;
    // Get command
    // Use write because we need to use read() to work with
    // signals, and read() is incompatible with printf().
    if (pwd() == NULL)
    {
      writeString("Error getcwd failed to get current directory\n", "error");
      exit(-1);
    }
    else
    {
      writeString(currentDirectory, "header");
      writeString("$ ", "header");
    }
    _Bool in_background = false;
    read_command(input_buffer, tokens, &in_background);

    /*DEBUG: Dump out arguments:
    for (int i = 0; tokens[i] != NULL; i++) {
      write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
      write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
      write(STDOUT_FILENO, "\n", strlen("\n"));
    }
    if (in_background) {
      write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
   } */

    if (invalidFlag)
      continue;
    else if (tokens[0] == NULL)
      invalidFlag = 1;
    else if (tokens[0][0] == '!')
    {
      if (tokens[1] != NULL)
      {
        writeString("Uh-oh !! does not take any arguments!\n", "warning");
        invalidFlag = 1;
      }
      else if (commandCounter == 0)
      {
        writeString("Uh-oh No commands in history!\n", "warning");
        invalidFlag = 1;
      }
      else if (strcmp(tokens[0], "!!") == 0)
      {
        if (historySpace == 0)
          strcpy(input_buffer, history[HISTORY_DEPTH - 1]);
        else
          strcpy(input_buffer, history[historySpace - 1]);
        writeString(input_buffer, "normal");
        writeString("\n", "normal");
        tokenize_command(input_buffer, tokens);
      }
      else
      {
        if (tokens[0][1] >= '0' && tokens[0][1] <= '9')
        {
          int commandNumber = atoi(&tokens[0][1]);
          if (commandNumber > commandCounter)
          {
            writeString("Uh-oh No such command in history!\n", "warning");
            invalidFlag = 1;
          }
          else
          {
            commandNumber = commandNumber - itteration;
            strcpy(input_buffer, history[commandNumber]);
            writeString(input_buffer, "normal");
            writeString("\n", "normal");
            tokenize_command(input_buffer, tokens);
          }
        }
        else
        {
          writeString("Uh-oh The ! operator should not be followed by a letter!\n", "warning");
          invalidFlag = 1;
        }
      }
    }

    if (!invalidFlag)
    {
      addToHistory(tokens);
      commandCounter++;

      if (strcmp(tokens[0], "exit") == 0)
      {
        if (tokens[1] == NULL)
          exit(0);
        else
          writeString("Uh-oh exit does not take any arguments!\n", "warning");
      }
      else if (strcmp(tokens[0], "pwd") == 0)
      {
        if (tokens[1] == NULL)
        {
          writeString(currentDirectory, "normal");
          writeString("\n", "normal");
        }
        else
          writeString("Uh-oh pwd does not take any arguments!\n", "warning");
      }
      else if (strcmp(tokens[0], "cd") == 0)
      {
        if (tokens[1] == NULL)
        {
          if (cd(getenv("HOME")) == -1)
          {
            color("red");
            perror("Error chdir failed to run command");
            color("reset");
          }
        }
        else if (tokens[2] != NULL)
          writeString("Uh-oh cd does not take more than two argument!\n", "warning");
        else
        {
          if (strcmp(tokens[1], "-") == 0)
            strcpy(tokens[1], previousDirectory);
          else if (tokens[1][0] == '~')
          {
            char *temp = getenv("HOME");
            strcat(temp, &tokens[1][1]);
            strcpy(tokens[1], temp);
          }
          if (cd(tokens[1]) == -1)
          {
            color("red");
            perror("Error chdir failed to run command");
            color("reset");
          }
        }
      }
      else if (strcmp(tokens[0], "history") == 0)
      {
        if (tokens[1] == NULL)
        {
          printHistory();
        }
        else
          writeString("Uh-oh history does not take any arguments!\n", "warning");
      }
      else if (strcmp(tokens[0], "help") == 0)
      {
        if (tokens[1] == NULL)
        {
          writeString("The following commands are supported:\n", "info");
          writeString("1. help:\n", "info");
          writeString("Displays information about the passed command. If no argument provided then it displays all the commands if no argument is provided. Takes only one argument.\n", "normal");
          writeString("2. exit:\n", "info");
          writeString("exit: Exits the shell. Takes no other arguments.\n", "normal");
          writeString("3. pwd:\n", "info");
          writeString("Prints the current working directory. Takes no other arguments.\n", "normal");
          writeString("4. cd:\n", "info");
          writeString("Changes the current working directory to the directory specified. Takes only one argument.\n", "normal");
          writeString("5. history:\n", "info");
          writeString("Displays the 10 most recent commands entered. If less than 10 commands were entered then only those are displayed. Takes no other arguments.\n", "normal");
        }
        else if (tokens[2] != NULL)
          writeString("Uh-oh help does not take more than two argument!\n", "warning");
        else
        {
          if (strcmp(tokens[1], "help") == 0)
          {
            writeString("help:\n", "info");
            writeString("Displays information about the passed command. If no argument provided then it displays all the commands if no argument is provided. Takes only one argument.\n", "normal");
          }
          else if (strcmp(tokens[1], "exit") == 0)
          {
            writeString("exit:\n", "info");
            writeString("exit: Exits the shell. Takes no other arguments.\n", "normal");
          }
          else if (strcmp(tokens[1], "pwd") == 0)
          {
            writeString("pwd:\n", "info");
            writeString("Prints the current working directory. Takes no other arguments.\n", "normal");
          }
          else if (strcmp(tokens[1], "cd") == 0)
          {
            writeString("cd:\n", "info");
            writeString("Changes the current working directory to the directory specified. Takes only one argument.\n", "normal");
          }
          else if (strcmp(tokens[1], "history") == 0)
          {
            writeString("history:\n", "info");
            writeString("Displays the 10 most recent commands entered. If less than 10 commands were entered then only those are displayed. Takes no other arguments.\n", "normal");
          }
          else
          {
            writeString(tokens[1], "other");
            writeString(":\n", "other");
            writeString(tokens[1], "other");
            writeString(" is an external command or application\n", "other");
          }
        }
      }
      else
      {
        // Reference for Colors: https://www.codeproject.com/Messages/5685657/Re-cout-in-color-and-perror-revisited.aspx
        pid_t process = fork();
        if (process < 0)
        {
          color("red");
          perror("Error failed to fork");
          color("reset");
          exit(-1);
        }
        else if (process == 0)
        {
          if (execvp(tokens[0], tokens) == -1)
          {
            color("red");
            perror("Error execvp failed to run command");
            color("reset");
            exit(-1);
          }
          // Cleanup any previously exited background child processes
          // (The zombies)
          while (waitpid(-1, NULL, WNOHANG) > 0)
            ; // do nothing.
        }
        else if (!in_background)
          waitpid(process, NULL, 0);
      }
    }

    /**
     * Steps For Basic Shell:
     * 1. Fork a child process
     * 2. Child process invokes execvp() using results in token array.
     * 3. If in_background is false, parent waits for
     *    child to finish. Otherwise, parent loops back to
     *    read_command() again immediately.
     */
  }

  return 0;
}