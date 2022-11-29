/****
* Project 04: Shell
* COSC 208, Introduction to Computer Systems, Spring 2022
****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "shell> "
#define MAX_BACKGROUND 50

/*
 * Break input string into an array of strings.
 * @param input the string to tokenize
 * @param delimiters the characters that delimite tokens
 * @return the array of strings with the last element of the array set to NULL
 */
char** tokenize(const char *input, const char *delimiters) {    
    char *token = NULL;

    // make a copy of the input string, because strtok
    // likes to mangle strings.  
    char *input_copy = strdup(input);

    // find out exactly how many tokens we have
    int count = 0;
    for (token = strtok(input_copy, delimiters); token; 
            token = strtok(NULL, delimiters)) {
        count++ ;
    }
    free(input_copy);

    input_copy = strdup(input);

    // allocate the array of char *'s, with one additional
    char **array = (char **)malloc(sizeof(char *)*(count+1));
    int i = 0;
    for (token = strtok(input_copy, delimiters); token;
            token = strtok(NULL, delimiters)) {
        array[i] = strdup(token);
        i++;
    }
    array[i] = NULL;
    free(input_copy);
    return array;
}

/*
 * Free all memory used to store an array of tokens.
 * @param tokens the array of tokens to free
 */
void free_tokens(char **tokens) {    
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

void builtInExit(char *command){
    if(strcmp(command, "exit") == 0){
        exit(EXIT_SUCCESS);
    }
}
void builtInFg(char *command){
}

void nonBuiltIn(char **command ){
    //find the index of either the NULL or the ampersand
    int i = 0;
    while(command[i]!= NULL && strcmp(command[i], "&") != 0){
        i++;
    }
    char *last = command[i];    
    int pid = fork();

    char *args[i+1];
    for(int j = 0; j < i; j++){
        args[j] = command[j];
    }
    args[i] = NULL;
   
    if(pid < 0){
        perror("fork failed");
    }
    else if(pid == 0){
        if(execv(args[0], args) < 0){
            perror("execv failed");
        }
    }
    else{    
        if(last == NULL){
            waitpid(pid, NULL, 0);
        }
    }
    
}

int main(int argc, char **argv) {    
    printf("%s", PROMPT);
    fflush(stdout);  // Display the prompt immediately
    char buffer[1024];
    while (fgets(buffer, 1024, stdin) != NULL) {
        char **command = tokenize(buffer, " \t\n");

        if (command[0] == NULL) {
            // Do nothing
        }
        else {
            // TODO: run commands
            if(strcmp(command[0], "exit") == 0){
                builtInExit(command[0]);
            }
            else if(strcmp(command[0], "fg") == 0){
            }
            else{
                nonBuiltIn(command);
                
                
            }
        }

        free_tokens(command);
        printf("%s", PROMPT);
        fflush(stdout);  // Display the prompt immediately
    }

    return EXIT_SUCCESS;
}
