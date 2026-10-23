# Bash Utilities: s21_cat & s21_grep

A reimplementation of the standard Unix command-line utilities `cat` and `grep`, written from scratch in C (C11 standard). This project was developed as part of the School 21 curriculum.

The main goal of this project is to deeply understand how file I/O, command-line argument parsing (`getopt`), and POSIX regular expressions work under the hood in C.

## Features

Both utilities are fully compatible with their GNU counterparts and support standard and GNU-specific flags.

### `s21_cat` (Supported Flags)
*   `-b` (--number-nonblank) - number non-empty output lines
*   `-e` - equivalent to `-vE`
*   `-E` - display `$` at the end of each line
*   `-n` (--number) - number all output lines
*   `-s` (--squeeze-blank) - suppress repeated empty output lines
*   `-t` - equivalent to `-vT`
*   `-T` - display TAB characters as `^I`
*   `-v` - use non-printing characters representation

### `s21_grep` (Supported Flags)
*   `-e` - specify multiple patterns
*   `-i` - ignore case distinctions
*   `-v` - select non-matching lines
*   `-c` - print only a count of matching lines per file
*   `-l` - print only names of files with matching lines
*   `-n` - prefix each line of output with the 1-based line number

## Project Structure

```text
.
|-- src/
|   |-- cat/
|   |   |-- s21_cat.c       # Source code for the cat utility
|   |   `-- test_cat.sh     # Integration testing script
|   `-- grep/
|       |-- s21_grep.c      # Source code for the grep utility
|       `-- test_grep.sh    # Integration testing script
|-- .clang-format           # Code style configuration
`-- Makefile                # Build instructions
```

## Build and Run

The project uses a `Makefile` for easy compilation with strict flags (`-Wall -Wextra -Werror -std=c11`).

### 1. Compilation
Open your terminal, navigate to the root of the project, and run:

```bash
# Build both utilities at once:
make

# OR build them individually:
make s21_cat
make s21_grep
```

### 2. Usage (How to Run)
Once compiled, you can run the utilities just like standard `cat` or `grep`. Use `./` before the program name to run it from the current directory.

**Running `s21_cat`:**
```bash
# Create a quick test file
echo -e "Hello\n\nWorld" > demo.txt

# Run the utility with flags (e.g., number non-empty lines)
./s21_cat -b demo.txt
```

**Running `s21_grep`:**
```bash
# Create a test file
echo -e "apple\nBanana\ncherry" > fruits.txt

# Search for a word (case-insensitive)
./s21_grep -i "banana" fruits.txt
```

### 3. Clean up
To remove the compiled executables and keep the folder clean:
```bash
make clean
```

## Automated Testing

The project includes integration testing scripts that compare the output of my implementations against the standard `/bin/cat` and `/bin/grep` utilities using `diff`.

To run the tests, make sure you are in the root directory of the project and execute:

```bash
# Run tests for s21_cat
bash cat/test_cat.sh

# Run tests for s21_grep
bash grep/test_grep.sh
```