*This project has been created as part of the 42 curriculum by ebin-ahm.*

# Pipex

## Overview

This implementation of **pipex** replicates the behaviour of the shell pipe operator, mimicking:

```bash
< infile cmd1 | cmd2 > outfile
```

It uses:

- **File descriptor redirection** via `dup2`
- **Pipe-based inter-process communication**
- **Process forking** to run two commands concurrently
- **PATH resolution** to locate executables

---

## Usage

```bash
./pipex infile cmd1 cmd2 outfile
```

Example:

```bash
./pipex infile.txt "grep hello" "wc -l" outfile.txt
```

This is equivalent to running:

```bash
< infile.txt grep hello | wc -l > outfile.txt
```

---

## 1. File Opening

The program opens two files at the start:

- `infile` — opened read-only; its file descriptor is stored in `pipex.infile`
- `outfile` — opened write-only, created if missing, truncated if existing; stored in `pipex.outfile`

If either file fails to open, an error is printed via `perror`. The fd is stored as `-1` and the children handle it via `check_fds`.

---

## 2. Pipe Creation

A single pipe is created using `pipe(pipex.fd)`:

- `pipex.fd[0]` — read end
- `pipex.fd[1]` — write end

The two child processes communicate through this pipe.

---

## 3. Forking Two Children

Two child processes are forked sequentially.

### Child One (cmd1)

1. Calls `check_fds` — exits if `infile` fd is `-1`
2. Redirects `infile` → stdin (`dup2(pipex.infile, 0)`)
3. Redirects pipe write-end → stdout (`dup2(pipex.fd[1], 1)`)
4. Closes unused file descriptors
5. Resolves and executes `cmd1` via `exec_cmd` → `execve`

### Child Two (cmd2)

1. Calls `check_fds` — exits if `outfile` fd is `-1`
2. Redirects pipe read-end → stdin (`dup2(pipex.fd[0], 0)`)
3. Redirects `outfile` → stdout (`dup2(pipex.outfile, 1)`)
4. Closes unused file descriptors
5. Resolves and executes `cmd2` via `exec_cmd` → `execve`

The parent closes all file descriptors after forking, then waits for both children.

---

## 4. Command Execution

`exec_cmd` (in `pipex.c`) handles command parsing and execution:

1. Splits the command string on spaces via `ft_split` to produce `args`
2. Passes `args[0]` to `find_path` to resolve the full executable path
3. Calls `execve(path, args, envp)`
4. If `execve` fails or the command/path is not found, exits with code `127`

---

## 5. PATH Resolution

When a command is passed (e.g. `grep`), the program:

1. Locates the `PATH=` entry in `envp` (`find_path` in `utils3.c`)
2. Splits the PATH string on `:` to get individual directories
3. Joins each directory with the command name via `try_path` (e.g. `/usr/bin/grep`)
4. Tests each candidate with `access(path, X_OK)`
5. Returns the first executable match, or `NULL` if none found

---

## 6. Exit Status

The program exits with the exit status of **cmd2**, matching standard shell pipe behaviour.

If a command is not found, the child exits with code `127`.

---

## File Structure

| File | Description |
|------|-------------|
| `main.c` | Entry point — argument validation, file opening, pipe creation, forking |
| `pipex.c` | Child process logic — `child_one`, `child_two`, `exec_cmd`, `free_args`, `check_fds` |
| `utils.c` | `ft_split` — splits a string by separator; helpers `count_words`, `extract_word2` |
| `utils2.c` | `ft_strjoin`, `ft_strncmp`, `get_length` |
| `utils3.c` | `find_path`, `try_path`, `search_dirs`, `free_split` — PATH resolution |
| `pipex.h` | Header — struct definition and function prototypes |
| `Makefile` | Build rules (`cc`, `-Wall -Wextra -Werror`) |

---

## Build

```bash
make        # compile
make clean  # remove object files
make fclean # remove object files and binary
make re     # full rebuild
```

---

## Testing

Expected results for edge cases:

1. Wrong number of arguments — prints usage and exits
```bash
./pipex
./pipex infile cmd1
```

2. Invalid infile — prints error, child one exits cleanly via `check_fds`
```bash
./pipex no_such_file "cat" "wc -l" outfile.txt
```

3. Invalid command — exits with code 127
```bash
./pipex infile.txt "notacommand" "wc -l" outfile.txt
```

#### Basic functional test
```bash
./pipex infile.txt "grep foo" "wc -l" outfile.txt
cat outfile.txt
```

#### Compare output against shell equivalent
```bash
./pipex infile.txt "cat" "wc -l" /tmp/pipex_out.txt
< infile.txt cat | wc -l > /tmp/shell_out.txt
diff /tmp/pipex_out.txt /tmp/shell_out.txt && echo "OK" || echo "DIFF"
```

#### Valgrind leak check
```bash
valgrind --leak-check=full ./pipex infile.txt "cat" "wc -l" outfile.txt
```
