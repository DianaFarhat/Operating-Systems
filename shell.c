#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

void executeCommand(char* command) {
    char* arguments[BUFFER_SIZE];
    arguments[0] = strtok(command, " ");

    int i = 1;
    while (arguments[i - 1] != NULL && i < BUFFER_SIZE) {
        arguments[i] = strtok(NULL, " ");
        i++;
    }

    if (strcmp(arguments[0], "ls") == 0) {
        execvp("ls", arguments);
    } else if (strcmp(arguments[0], "generate") == 0) {
        execvp("java", (char*[]){"java", "GenerateSumSubtract", "generate", NULL});
    } else if (strcmp(arguments[0], "sum") == 0) {
        execvp("java", (char*[]){"java", "GenerateSumSubtract", "sum", NULL});
    } else if (strcmp(arguments[0], "subtract") == 0) {
        execvp("java", (char*[]){"java", "GenerateSumSubtract", "subtract", NULL});
    } else {
        printf("Invalid command: %s\n", arguments[0]);
        exit(1);
    }
}


void executePipeline(char* firstCommand, char* secondCommand) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process: first command
        close(pipefd[0]); // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(pipefd[1]); // Close the write end of the pipe

        executeCommand(firstCommand);
        exit(0);
    } else {
        // Parent process
        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {
            // Child process: second command
            close(pipefd[1]); // Close the write end of the pipe
            dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
            close(pipefd[0]); // Close the read end of the pipe

            executeCommand(secondCommand);
            exit(0);
        } else {
            // Parent process
            close(pipefd[0]); // Close the read end of the pipe
            close(pipefd[1]); // Close the write end of the pipe
            wait(NULL);
            wait(NULL);
        }
    }
}

int main() {
    char input[BUFFER_SIZE];

    while (1) {
        printf("basic240-shell> ");
        fgets(input, BUFFER_SIZE, stdin);

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char* pipeSymbol = strchr(input, '|');
        if (pipeSymbol != NULL) {
            *pipeSymbol = '\0';
            char* firstCommand = strtok(input, " ");
            char* secondCommand = strtok(NULL, " ");
            executePipeline(firstCommand, secondCommand);
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                executeCommand(input);
                exit(0);
            } else {
                wait(NULL);
            }
        }
    }

    return 0;
}
