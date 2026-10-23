#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 1024
#define MAX_BUFFER_SIZE 2048

typedef struct {
  int e, i, v, c, l, n;

  char* patterns[MAX_PATTERNS];
  int pattern_count;
} Flags;

typedef struct {
  FILE* file;
  const char* file_name;

  int print_file_name, line_count, matching_lines_count, has_match;
} Result;

int parse_flags(int argc, char* argv[], Flags* flags);
int preparation_regex(Flags* flags, regex_t* regex);
void process_file(int argc, char* argv[], Flags* flags, regex_t* regex);
void process_line(Flags* flags, regex_t* regex, Result* result);

int main(int argc, char* argv[]) {
  Flags flags = {0};
  regex_t regex;

  if (parse_flags(argc, argv, &flags) == 0) {
    if (preparation_regex(&flags, &regex) == 0) {
      process_file(argc, argv, &flags, &regex);
      regfree(&regex);
    };
  }

  return 0;
}

int parse_flags(int argc, char* argv[], Flags* flags) {
  int opt;
  int error = 0;

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        if (flags->pattern_count < MAX_PATTERNS) {
          flags->patterns[flags->pattern_count] = optarg;
          flags->pattern_count++;
          flags->e = 1;
        } else {
          error = 1;
        }
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      default:
        error = 1;
        break;
    }
  }

  if (flags->pattern_count == 0) {
    if (optind < argc) {
      flags->patterns[0] = argv[optind];
      flags->pattern_count = 1;
      optind++;
    } else {
      error = 1;
    }
  }

  return error;
}

int preparation_regex(Flags* flags, regex_t* regex) {
  int error = 0;
  char result_pattern[MAX_BUFFER_SIZE] = {0};

  for (int i = 0; i < flags->pattern_count; i++) {
    size_t result_length = strlen(result_pattern);
    size_t pattern_length = strlen(flags->patterns[i]);
    size_t add_len = pattern_length;

    if (i > 0) add_len += 1;

    if (result_length + add_len < MAX_BUFFER_SIZE) {
      if (i > 0) strcat(result_pattern, "|");
      strcat(result_pattern, flags->patterns[i]);
    } else {
      error = 1;
    }
  }

  int cflags = REG_EXTENDED | REG_NOSUB;
  if (flags->i) {
    cflags = cflags | REG_ICASE;
  }

  if (regcomp(regex, result_pattern, cflags) != 0) error = 1;

  return error;
}

void process_file(int argc, char* argv[], Flags* flags, regex_t* regex) {
  Result result = {0};
  result.print_file_name = argc - optind > 1;

  for (int i = optind; i < argc; i++) {
    result.file = fopen(argv[i], "r");
    if (result.file != NULL) {
      result.file_name = argv[i];
      process_line(flags, regex, &result);
      fclose(result.file);
    }
  }
}

void process_line(Flags* flags, regex_t* regex, Result* result) {
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  result->line_count = 0;
  result->matching_lines_count = 0;
  result->has_match = 0;

  while ((read = getline(&line, &len, result->file)) != -1) {
    result->line_count++;

    int res = (regexec(regex, line, 0, NULL, 0) == 0);

    if (flags->v) res = !res;

    if (res) {
      result->matching_lines_count++;
      result->has_match = 1;

      if (!flags->c && !flags->l) {
        if (result->print_file_name && result->file_name != NULL) {
          printf("%s:", result->file_name);
        }

        if (flags->n) {
          printf("%d:", result->line_count);
        }
        printf("%s", line);
      }
    }
  }

  free(line);

  if (flags->l && result->has_match && result->file_name != NULL) {
    printf("%s\n", result->file_name);
  } else if (flags->c) {
    if (result->print_file_name && result->file_name != NULL) {
      printf("%s:", result->file_name);
    }
    printf("%d\n", result->matching_lines_count);
  }
}