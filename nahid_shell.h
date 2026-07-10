#ifndef NAHID_SHELL_H
#define NAHID_SHELL_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* helpers.c */
int my_strcmp(const char *str1, const char *str2);

/* main.c */
void shell_loop(char **env);

#endif /* NAHID_SHELL_H */
