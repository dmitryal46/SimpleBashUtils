#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grepflags_and_pattern {
  int e, i, v, c, l, n, h, s, f, o;
  char pattern[1024];
  int len_pattern;
} flags;

void add_pattern(flags* flag, char* pattern);
void add_reg_from_file(flags* flag, char* path);
void flags_parser(flags* flag, int argc, char** argv);
void output_line(char* line, int n);
void print_match(regex_t* re, char* line);
int files_processing(flags* flag, char* path, regex_t* re);
void main_output(flags* flag, int argc, char** argv);

int main(int argc, char** argv) {
  flags flag = {0};
  flags_parser(&flag, argc, argv);
  main_output(&flag, argc, argv);
  return 0;
}

void add_pattern(flags* flag, char* pattern) {
  if (flag->len_pattern != 0) {
    strcat(flag->pattern + flag->len_pattern, "|");
    flag->len_pattern++;
  }
  flag->len_pattern +=
      sprintf(flag->pattern + flag->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(flags* flag, char* path) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    if (!flag->s) perror(path);
    exit(1);
  }
  char* line = NULL;
  size_t memline = 0;  // memlen in example
  int read = 0;
  while ((read = getline(&line, &memline, fp)) != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    add_pattern(flag, line);
  }
  // if (line)
  free(line);
  // line = NULL;
  fclose(fp);
}

void flags_parser(flags* flag, int argc, char** argv) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        flag->e = 1;
        add_pattern(flag, optarg);
        break;
      case 'i':
        flag->i = REG_ICASE;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->c = 1;
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        add_reg_from_file(flag, optarg);
        break;
      case 'o':
        flag->o = 1;
        break;
      case '?':
        perror("pattern ERROR");
        exit(1);
        break;
      default:
        break;
    }
  }
  if (flag->len_pattern == 0) {
    add_pattern(flag, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) flag->h = 1;
}

void output_line(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void print_match(regex_t* re, char* line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) break;
    for (int i = match.rm_so; i < match.rm_eo; i++) putchar(line[i]);
    putchar('\n');
    offset += match.rm_eo;
  }
}

int files_processing(flags* flag, char* path, regex_t* re) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    if (!flag->s) perror(path);
    // exit(1);
    return 1;
  }
  char* line = NULL;
  size_t memline = 0;
  int read = 0;
  int line_count = 1;
  int c = 0;  // flag c - string match count
  while ((read = getline(&line, &memline, fp)) != -1) {
    int result = regexec(re, line, 0, NULL, 0);
    if ((result == 0 && !(flag->v)) || (flag->v && result != 0)) {
      if (!flag->c && !flag->l) {
        if (!flag->h) printf("%s:", path);
        if (flag->n) printf("%d:", line_count);
        if (flag->o) {
          print_match(re, line);
        } else {
          output_line(line, read);
        }
      }
      c++;
    }
    line_count++;
    // free(line);
  }
  if (flag->c && !flag->l) {
    if (!flag->h) printf("%s:", path);
    printf("%d\n", c);
  }
  if (flag->l && c > 0) printf("%s\n", path);
  // if (line)
  free(line);
  // line = NULL;
  fclose(fp);
  return 0;
}

void main_output(flags* flag, int argc, char** argv) {
  regex_t re;
  int reError = regcomp(&re, flag->pattern, REG_EXTENDED | flag->i);
  if (reError) perror("re ERROR");
  if (optind >= argc) {
    if (flag->s == 0) perror("file error");
    exit(1);
  }
  for (int i = optind; i < argc; i++) {
    files_processing(flag, argv[i], &re);
  }
  regfree(&re);
}
