# My Shell Project

## Overview
I created a simple shell as a personal project to develop my skills with process management using the UNIX process API. The shell operates in the following way: when a command is typed in response to the shell prompt, it creates a child process to execute the command, then prompts for more user input when finished. This shell is similar to the ones used in Linux (e.g., `bash`), but with many simplifications.

### Learning Objectives
Through this project, I aimed to:
* Write programs that create and manage processes using the UNIX process API.

## Getting Started 
Clone the repository on your machine.
Compile the shell using `make`, and run the shell using `./shell`. 

The shell runs interactively. It displays a prompt (`shell>`) and waits for the user to enter a command. The user can enter a program to execute (e.g., `/bin/ls`) or a "built-in" command (e.g., `exit`). Non-built-in commands should contain the full path to the executable file (e.g., `/bin/ls` rather than just `ls`) and may include an arbitrary number of command-line options (e.g., `/bin/ls -l -r`). If the user enters a non-built-in command, a new process is created to run the specified program. The shell waits for the program to finish (unless the command includes an ampersand `&` to indicate it should run in the background), then displays the prompt again for the next command.

The main function in `main.c` includes code for:
1. Displaying the prompt (`shell>`)
2. Reading a line of user input into the char array `buffer`
3. Breaking the command into tokens, where tokens are delimited by one or more whitespace characters

## Non-built-in Commands
When a user enters a non-built-in command (e.g., `/bin/ls -l`), the shell creates a new process to invoke the specified executable program. 

Optionally, a user can indicate that a command should be executed as a *background* process by including the ampersand (`&`) character at the end of the command. For simplicity, the ampersand is assumed to be a separate token. For example:
```
shell> /bin/ls -l -r &
``
Commands without an ampersand are executed as *foreground* processes.

In this project, I ensured that the shell handles the situation gracefully where invoking the specified executable fails.

For **foreground** commands, the shell waits for the process to complete before prompting for a new command. I used `waitpid` instead of `wait` to avoid issues when commands are running in the background.

For **background** commands, the shell does not wait for the process to complete before prompting for a new command, allowing immediate input for the next command.

## Built-in Commands
There are two built-in commands I handled: `exit` and `fg`.

Built-in commands are handled differently than other commands; they are **not executed in a separate process** like `/bin/ls` or other programs invoked through the shell.  

### exit
To quit the shell, the user can type `exit`. This terminates the shell using the `exit()` system call.

### fg
The `fg` command moves the most recent background command to the foreground. The shell waits for this command to complete. If the most recent background command has already finished, the shell simply prompts for the next command.

## Testing the Shell
Below is an example shell session to test the shell.

```
shell> /bin/ls
Makefile  README.md  shell  shell.c  shell.o
shell> /bin/ls -a
.  ..  .git  .gitignore  Makefile  README.md  shell  shell.c  shell.o
shell> /bin/date
Fri Nov 11 20:15:10 UTC 2022
shell> /bin/sleep 5
shell> /bin/sleep 10 &
shell> /bin/ps
    PID TTY          TIME CMD
   5066 pts/1    00:00:00 bash
   5092 pts/1    00:00:00 shell
   5329 pts/1    00:00:00 sleep
   5330 pts/1    00:00:00 ps
shell> /bin/sleep 15
shell> /bin/ps
    PID TTY          TIME CMD
   5066 pts/1    00:00:00 bash
   5092 pts/1    00:00:00 shell
   5332 pts/1    00:00:00 ps
shell> /bin/sleep 7 &
shell> fg
shell> exit
```

## Program Design
I followed good program design and coding practices, including:
* Checking the return values of all system calls (excluding `printf`) to catch errors in system call invocation.
* Using multiple functions — **Not** putting all code in the `main` function.
* Freeing all heap-allocated memory before exiting — Running `valgrind` to confirm all memory is freed and no other memory errors exist.
* Properly indenting code — Indentation is not semantically significant in C but makes the code easier to read.
* Including comments — Adding comments before each conditional statement, loop, and set of statements that perform calculations.
