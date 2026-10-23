#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int b, e, n, s, t, v;
} Flags;

typedef struct {
  int line_count, at_line_start, prev_was_empty;
} Line_info;

int parse_flags(int argc, char* argv[], Flags* flags);
void process_file(int argc, char* argv[], Flags* flags);
void process_line(FILE* file, Flags* flags, Line_info* line_info);
void process_char(char ch, Flags* flags);

int main(int argc, char* argv[]) {
  Flags flags = {0};

  if (parse_flags(argc, argv, &flags) == 0) {
    process_file(argc, argv, &flags);
  }

  return 0;
}

int parse_flags(int argc, char* argv[], Flags* flags) {
  int opt;
  int error = 0;

  static struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "beEnstTv", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        printf("Try 'cat --help' for more information.\n");
        error = 1;
        break;
    }
  }

  if (flags->b) flags->n = 0;
  return error;
}

void process_file(int argc, char* argv[], Flags* flags) {
  Line_info line_info = {0};

  line_info.line_count = 1;
  line_info.at_line_start = 1;
  line_info.prev_was_empty = 0;

  for (int i = optind; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");

    if (file != NULL) {
      process_line(file, flags, &line_info);
      fclose(file);
    }
  }
}

void process_line(FILE* file, Flags* flags, Line_info* line_info) {
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file)) != -1) {
    int is_empty_line = (read == 1 && line[0] == '\n') ? 1 : 0;

    int skip_line = (flags->s && is_empty_line && line_info->prev_was_empty);
    if (!skip_line) {
      if (line_info->at_line_start) {
        if ((flags->b && !is_empty_line) || flags->n) {
          printf("%6d\t", line_info->line_count++);
        }
      }

      for (ssize_t i = 0; i < read; i++) {
        char ch = line[i];
        if (flags->e && ch == '\n') printf("$");

        process_char(ch, flags);
      }

      line_info->at_line_start = (read > 0 && line[read - 1] == '\n') ? 1 : 0;
      line_info->prev_was_empty = is_empty_line;
    }
  }

  free(line);
}

void process_char(char ch, Flags* flags) {
  if (flags->t && ch == '\t') {
    printf("^I");
  } else if (ch == '\n' || ch == '\t') {
    printf("%c", ch);
  } else if (flags->v) {
    if ((unsigned char)ch >= 128) {
      printf("M-");
      ch = ch - 128;
    }

    if ((unsigned char)ch == 127) {
      printf("^?");
    } else if ((unsigned char)ch < 32) {
      printf("^%c", ch + 64);
    } else {
      printf("%c", ch);
    }
  } else {
    printf("%c", ch);
  }
}