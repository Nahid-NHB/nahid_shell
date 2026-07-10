#ifndef NAHID_SHELL_H
#define NAHID_SHELL_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

extern char **environ;

/* helpers.c */
int my_strcmp(const char *str1, const char *str2);

/* main.c */
void shell_loop(char **env);

/* builtins.c */
int try_builtin(char **argv);

#endif /* NAHID_SHELL_H */
