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

void builtInExit(char** command){
    //free all memory before exiting
    free_tokens(command);
    exit(EXIT_SUCCESS);
}

void builtInFg(int background[], int* num_elements){
    //obtain the pid of the most recent background process
    int recent_pid = background[*num_elements -1];
    //make the parent wait for that process to finish
    waitpid(recent_pid, NULL, 0); 
}

void nonBuiltIn(char **command, int background[], int* num_elements){
    //find the index of either the NULL or the ampersand
    int i = 0;
    while(command[i]!= NULL && strcmp(command[i], "&") != 0){
        i++;
    }
    //the last character will either be NULL or the ampersand
    char *last = command[i];    
    int pid = fork();

    //populate the args array with everything before NULL or the ampersand
    char *args[i+1];
    for(int j = 0; j < i; j++){
        args[j] = command[j];
    }
    //NULL will always be the last element of the array
    args[i] = NULL;

   //if the fork failed, print an error message
    if(pid < 0){
        perror("fork failed");
    }
    //if the child process is running, execute the file contained in args but print an error message if this is file does not exist
    else if(pid == 0){
        if(execv(args[0], args) < 0){
            perror("execv failed");
        }
    }
    else{
        //only make the parent wait if there is no ampersand (foreground process)     
        if(last == NULL){
            waitpid(pid, NULL, 0);
        }
        else{
            //store the PID of the child in the array of PIDs
            background[*num_elements] = pid;
            *num_elements += 1;
        }
    }
    
}

int main(int argc, char **argv) {    
    printf("%s", PROMPT);
    fflush(stdout);  // Display the prompt immediately
    char buffer[1024];
    //initialize an array that can hold all the background processes
    int background[MAX_BACKGROUND];
    int curr_index = 0;
    //keep a tracker of the current number of elements in the arry
    int* num_elements = &curr_index;
    while (fgets(buffer, 1024, stdin) != NULL) {
        char **command = tokenize(buffer, " \t\n");

        if (command[0] == NULL) {
            // Do nothing
        }
        else {
            // TODO: run commands
            
             //run exit if the first command line argument is exit
            if(strcmp(command[0], "exit") == 0){
                builtInExit(command);
            }
            //run fg if the first command line argument is fg
            else if(strcmp(command[0], "fg") == 0){
                builtInFg(background, num_elements);
            }
            //call the non-built in function if it's anything else
            else{
                nonBuiltIn(command, background, num_elements);
                
            }
        }

        free_tokens(command);
        printf("%s", PROMPT);
        fflush(stdout);  // Display the prompt immediately
    }

    return EXIT_SUCCESS;
}
