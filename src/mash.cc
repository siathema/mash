#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MASH_TOK_BUFSIZE 64
#define MASH_TOK_DELIM " \t\r\n\a"

char* mash_read_line() {
  char* line = NULL;
  size_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

char** mash_split_line(char* line) {
  int bufsize = MASH_TOK_BUFSIZE, position = 0;
  char** tokens = (char**)malloc(bufsize * sizeof(char*));
  char* token;

  if(!tokens) {
    fprintf(stderr, "mash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, MASH_TOK_DELIM);
  while(token != NULL) {
    tokens[position] = token;
    position++;

    if(position >= bufsize) {
      bufsize += MASH_TOK_BUFSIZE;
      tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
      if(!tokens) {
	fprintf(stderr, "mash: allocation error\n");
	exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, MASH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int mash_command(char** command) {

  pid_t wpid;
  pid_t child = fork();
  int status;

  /* if process is parent wait for child process to be done */
  if(child > 0) {
    do {
      wpid = waitpid(child, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    /* if process is child execute the command */
  } else if(child == 0) {
    if(execvp(command[0], command) == -1) {
      perror("mash");
    }
    exit(EXIT_FAILURE);
    /* if process fails */
  } else if(child < 0) {
    perror("mash");
  }
  return 1;
}

int mash_loop() {

  do {

    printf("mash <: ");
    char* line = mash_read_line();
    char** line_toks = mash_split_line(line);
    mash_command(line_toks);
  }while(1);
  
  return EXIT_SUCCESS;

}

int main( int argc, char** argv ) {

  
  return mash_loop();
  
}
