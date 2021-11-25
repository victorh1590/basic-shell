#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
// #include "Shell.h"

#define TRUE 1

void type_prompt() {
    printf("\ncommand: ");
}

char *read_command() {
    char *line = NULL;
    size_t n = 0;
    ssize_t result = getline(&line, &n, stdin);
    line[result - 1] = '\0';
    return line;
}

void debug_print_command(char *str) {
    printf("%s", str);
}

int main() {
    char *command;
    wordexp_t p;
    int status;
    extern char **environ;

    while (TRUE) {
        type_prompt();
        command = read_command();
        // debug_print_command(command);
        wordexp(command, &p, 0);

        if (strcmp(p.we_wordv[0], "cd") != 0) {
            pid_t pid;
            pid = fork();

            if (pid == -1)
            {
                printf("\nFail.");
                break;
            }
            else if (pid == 0)
            {
                execvpe(p.we_wordv[0], p.we_wordv, environ);
                printf("%s: I can't recognize this command.\n", p.we_wordv[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                waitpid(pid, &status, 0);
            }
        }
        else
        {
            chdir(p.we_wordv[1]);
        }

        free(command);
        wordfree(&p);
    }
    return 0;
}
