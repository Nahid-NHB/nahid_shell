#include "nahid_shell.h"

/*
 * Builtins run in the shell's own process (never forked) because their
 * whole purpose is to mutate the shell's state: working directory,
 * environment, or lifetime. A forked child could never do that.
 */

static int builtin_cd(char **argv){
    char oldpwd[PATH_MAX];
    const char *target = argv[1];

    if(getcwd(oldpwd, sizeof(oldpwd)) == NULL){
        perror("nahid_shell: cd");
        return 1;
    }

    if(target == NULL){
        target = getenv("HOME");
        if(target == NULL){
            fprintf(stderr, "nahid_shell: cd: HOME not set\n");
            return 1;
        }
    } else if(strcmp(target, "-") == 0){
        target = getenv("OLDPWD");
        if(target == NULL){
            fprintf(stderr, "nahid_shell: cd: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", target);
    }

    if(chdir(target) != 0){
        fprintf(stderr, "nahid_shell: cd: %s: %s\n", target, strerror(errno));
        return 1;
    }

    setenv("OLDPWD", oldpwd, 1);

    char newpwd[PATH_MAX];
    if(getcwd(newpwd, sizeof(newpwd)) != NULL){
        setenv("PWD", newpwd, 1);
    }

    return 0;
}

static int builtin_pwd(char **argv){
    (void)argv;
    char cwd[PATH_MAX];

    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("nahid_shell: pwd");
        return 1;
    }
    printf("%s\n", cwd);
    return 0;
}

static int builtin_exit(char **argv){
    int code = argv[1] != NULL ? atoi(argv[1]) : 0;

    exit(code);
}

static int builtin_env(char **argv){
    (void)argv;

    for(char **e = environ; *e != NULL; e++){
        printf("%s\n", *e);
    }
    return 0;
}

static int builtin_export(char **argv){
    if(argv[1] == NULL){
        return builtin_env(argv);
    }

    char *eq = strchr(argv[1], '=');
    if(eq == NULL){
        /* "export VAR" with no value and no prior shell-var tracking: no-op */
        return 0;
    }

    *eq = '\0';
    int rc = setenv(argv[1], eq + 1, 1);
    if(rc != 0){
        perror("nahid_shell: export");
        return 1;
    }
    return 0;
}

static int builtin_unset(char **argv){
    if(argv[1] == NULL){
        fprintf(stderr, "nahid_shell: unset: not enough arguments\n");
        return 1;
    }
    if(unsetenv(argv[1]) != 0){
        perror("nahid_shell: unset");
        return 1;
    }
    return 0;
}

static int builtin_echo(char **argv){
    for(int i = 1; argv[i] != NULL; i++){
        fputs(argv[i], stdout);
        if(argv[i + 1] != NULL){
            fputc(' ', stdout);
        }
    }
    fputc('\n', stdout);
    return 0;
}

typedef struct {
    const char *name;
    int (*fn)(char **argv);
} builtin_t;

static const builtin_t BUILTINS[] = {
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit", builtin_exit},
    {"env", builtin_env},
    {"export", builtin_export},
    {"unset", builtin_unset},
    {"echo", builtin_echo},
};

#define BUILTIN_COUNT (sizeof(BUILTINS) / sizeof(BUILTINS[0]))

int try_builtin(char **argv){
    for(size_t i = 0; i < BUILTIN_COUNT; i++){
        if(strcmp(argv[0], BUILTINS[i].name) == 0){
            BUILTINS[i].fn(argv);
            return 1;
        }
    }
    return 0;
}
