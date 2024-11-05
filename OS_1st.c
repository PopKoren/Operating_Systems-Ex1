#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_CMD_LEN 100
#define MAX_PATH_LEN 100

int main() {
    char cmd[MAX_CMD_LEN];
    char *path = getenv("PATH");
    char pathcopy[MAX_PATH_LEN];
    strcpy(pathcopy, path);

    while (1) {
        printf("$ ");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = '\0'; // Remove trailing newline if present

        if (strcmp(cmd, "leave") == 0) {
            break;
        }

        int pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            char fullcmd[MAX_PATH_LEN];
            char *params[] = {cmd, NULL}; // Command and its arguments

            char *token = strtok(pathcopy, ":");
            while (token) {
                strcpy(fullcmd, token);
                strcat(fullcmd, "/");
                strcat(fullcmd, cmd);
                execv(fullcmd, params);
                token = strtok(NULL, ":");
            }
            printf("Could not run %s\n", cmd);
            exit(EXIT_FAILURE);
        } else { // Parent process
            wait(NULL);
        }
    }

    return 0;
}

