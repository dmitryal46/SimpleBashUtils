#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct cat_flags {
  int b, n, s, E, T, v;
} flags;

void flags_parser(flags *flag, int argc, char **argv);
char v_output(char ch);
void outline(flags *flag, char *line, int n);
void cat_output(flags *flag, int argc, char **argv);

int main(int argc, char **argv) {
  flags flag = {0};
  flags_parser(&flag, argc, argv);

  cat_output(&flag, argc, argv);

  return 0;
}

void flags_parser(flags *flag, int argc, char **argv) {
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {0, 0, 0, 0}};
  int opt;
  opt = getopt_long(argc, argv, "bnsEeTt", long_options, 0);
  switch (opt) {
    case 'b':
      flag->b = 1;
      break;
    case 'n':
      flag->n = 1;
      break;
    case 's':
      flag->s = 1;
      break;
    case 'E':
      flag->E = 1;
      break;
    case 'e':
      flag->E = 1;
      flag->v = 1;
      break;
    case 'T':
      flag->T = 1;
      break;
    case 't':
      flag->T = 1;
      flag->v = 1;
      break;
    case '?':
      perror("ERROR");
      exit(1);
      break;
    default:
      break;
  }
}

char flag_v_process_and_out(char ch) {
  if (ch == '\n' || ch == '\t') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7F;
  }
  if (ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void output_line(flags *flag, char *line, int n) {
  for (int i = 0; i < n; i++) {
    if (flag->T && line[i] == '\t') {
      putchar('^');
      line[i] = 'I';
    }
    if (flag->E == 1 && line[i] == '\n') putchar('$');
    if (flag->v) line[i] = flag_v_process_and_out(line[i]);
    putchar(line[i]);
  }
}

void cat_output(flags *flag, int argc, char **argv) {
  FILE *fp = fopen(argv[argc - 1], "r");
  if (fp == NULL) {
    perror(argv[argc - 1]);
    exit(1);
  }
  char *line = NULL;
  size_t memline = 0;
  int read = 0;
  int line_count = 1;
  int empty_count = 0;
  while ((read = getline(&line, &memline, fp)) != -1) {
    if (line[0] == '\n')
      empty_count++;
    else
      empty_count = 0;
    if (flag->s && line[0] == '\n' && empty_count > 1) continue;
    if (flag->n && !(flag->b)) {
      printf("%6d\t", line_count);
      line_count++;
    } else if (flag->b && line[0] != '\n') {
      printf("%6d\t", line_count);
      line_count++;
    }
    output_line(flag, line, read);
  }
  if (line) free(line);
  line = NULL;
  fclose(fp);
}