#include "nahid_shell.h"

#define SHELL_PROMPT "nahid_shell$ "
#define SHELL_ARG_DELIMS " \t\r\n\a"
#define SHELL_ARGV_INIT_CAP 64

/*
 * Splits line in place on whitespace and returns a NULL-terminated argv.
 * Stopgap tokenizer: no quoting/escaping yet (see Milestone 3).
 */
static char **tokenize_line(char *line){
    size_t cap = SHELL_ARGV_INIT_CAP;
    size_t i = 0;
    char **argv = malloc(cap * sizeof(char *));

    if(!argv){
        perror("nahid_shell: malloc");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, SHELL_ARG_DELIMS);
    while(token != NULL){
        if(i >= cap - 1){
            cap *= 2;
            char **grown = realloc(argv, cap * sizeof(char *));
            if(!grown){
                perror("nahid_shell: realloc");
                free(argv);
                exit(EXIT_FAILURE);
            }
            argv = grown;
        }
        argv[i++] = token;
        token = strtok(NULL, SHELL_ARG_DELIMS);
    }
    argv[i] = NULL;

    return argv;
}

/* Forks and execs argv[0], waiting for it to finish. */
static void execute_command(char **argv){
    pid_t pid = fork();

    if(pid < 0){
        perror("nahid_shell: fork");
        return;
    }

    if(pid == 0){
        execvp(argv[0], argv);
        /* only reached if execvp failed */
        fprintf(stderr, "nahid_shell: %s: %s\n", argv[0], strerror(errno));
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
}

void shell_loop(char **env){
    (void)env;

    char *line = NULL;
    size_t line_cap = 0;

    while(1){
        printf("%s", SHELL_PROMPT);
        fflush(stdout);

        ssize_t nread = getline(&line, &line_cap, stdin);
        if(nread == -1){
            putchar('\n');
            break;
        }

        char **argv = tokenize_line(line);
        if(argv[0] != NULL && !try_builtin(argv)){
            execute_command(argv);
        }
        free(argv);
    }

    free(line);
}

int main(int argc, char **argv, char **env){
    (void)argc;
    (void)argv;

    shell_loop(env);

    return 0;
}
