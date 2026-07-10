# nahid_shell

A UNIX command-line interpreter (shell) written from scratch in C, built
incrementally on top of raw POSIX primitives (`fork`, `execvp`, `waitpid`,
`chdir`, `setenv`, ...) — no readline or shell-building libraries.

The project is being built milestone by milestone: each milestone is a
small, independently testable, buildable step toward a fuller Unix-like
shell.

## Building

```sh
make
```

Produces the `nahid_shell` binary. Compiled with `-Wall -Wextra -pedantic
-std=c11`.

```sh
make clean
```

Removes build artifacts.

## Running

```sh
./nahid_shell
```

Starts an interactive prompt (`nahid_shell$ `). Exit with `exit`, `exit
<code>`, or Ctrl+D.

## Current features

- REPL: prompt, read a line, run it, repeat, until EOF (Ctrl+D)
- Command execution via `fork` + `execvp` (PATH lookup included) +
  `waitpid`
- Whitespace tokenizer (no quoting/escaping yet)
- Builtins, run in-process (not forked) since they mutate shell state:
  - `cd [dir | -]` — changes directory, updates `$PWD`/`$OLDPWD`,
    defaults to `$HOME` with no argument
  - `pwd` — prints the current working directory
  - `exit [code]` — exits the shell with an optional status code
  - `env` — lists the environment
  - `export VAR=value` — sets/exports an environment variable
  - `unset VAR` — removes an environment variable
  - `echo [args...]` — prints its arguments

## Known limitations (by design, addressed in later milestones)

- No quoting or escaping — `echo "a b"` is two arguments, not one
- No `$VAR` / `$?` expansion
- No pipes (`|`) or I/O redirection (`<`, `>`, `>>`)
- No `;`, `&&`, `||` command sequencing
- No background jobs (`&`) or job control
- No signal handling — Ctrl+C currently kills the shell itself (default
  `SIGINT` disposition), rather than just interrupting the running
  command
- No command history, globbing, subshells, here-documents, or aliases

## Roadmap

Ordered roughly easiest → hardest; each milestone only depends on
earlier ones:

1. ~~Foundation: header guards/prototypes, Makefile, `.gitignore`, bug fixes~~
2. ~~Minimal REPL: prompt, read, tokenize, `fork`/`execvp`/`waitpid`~~
3. ~~Builtins: `cd`, `pwd`, `exit`, `env`, `export`, `unset`, `echo`~~
4. Robust tokenizer with quoting/escaping
5. Proper exit-status tracking (`$?`)
6. I/O redirection: `<`, `>`, `>>`
7. Pipes: `|`
8. Command sequencing & logical operators: `;`, `&&`, `||`
9. Environment variable expansion: `$VAR`, `$?`, `$$`
10. Signal handling: Ctrl+C, Ctrl+\\, Ctrl+D semantics
11. Background jobs (`&`), job control, process groups, terminal control
12. Subshells `( )`, here-documents, globbing, command history, aliases,
    config file

## Project layout

- `main.c` — entry point and the REPL loop
- `builtins.c` — builtin command implementations and dispatch
- `helpers.c` — small utility functions
- `nahid_shell.h` — shared header (includes, prototypes)
- `Makefile` — build rules
