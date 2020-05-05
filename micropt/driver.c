#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "converter.h"

// Compute a simplified checksum for an array of numbers:
uint16_t checksum(quote_t nums[], unsigned n)
{
  uint16_t csum = 0;
  for (unsigned i = 1; i < n; i++) {
    csum += nums[i] * (nums[i - 1]) % 1879;  // Overflow is intentional
  }
  return csum;
}

const int QUOTES_CSUM = 52036;


int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Requires arguments: quotes filename and no. of repetitions\n");
    exit(-1);
  }

  // Find out length of input:
  int64_t datasize;
  FILE *fin;
  if ((fin = fopen(argv[1], "r")) == NULL) {
    perror("open input file");
    exit(-2);
  }
  if (fseek(fin, 0L, SEEK_END) == -1) {
    perror("fseek input file");
    exit(-3);
  }
  if ((datasize = ftell(fin)) == -1) {
    perror("ftell input file");
    exit(-4);
  }
  rewind(fin);

  // Read in file and parse EOL tokens:
  char *data = (char *)malloc(datasize);
  if (fread(data, 1, datasize, fin) < 1) {
    perror("read input file");
    exit(-5);
  }
  fclose(fin);

  char **lines = (char **)malloc(sizeof(char *) * datasize);  // A bit overkill allocation, but easy.
  unsigned nlines = 0;
  lines[nlines++] = strtok(data, "\n");
  while ((lines[nlines] = strtok(NULL, "\n"))) {
    nlines++;
  }


  // Finally, main timing loop:
  double min_t = 1000000000.;
  uint32_t *nums = (uint32_t *)calloc(sizeof(uint32_t), nlines);
  for (int iter = atoi(argv[2]); iter > 0; --iter) {
    clock_t t0 = clock();
    convert_all(nlines, lines, nums);
    double t = (double)(clock() - t0) / CLOCKS_PER_SEC;
    assert(checksum(nums, nlines) == QUOTES_CSUM);
    if (t < min_t) {
      min_t = t;
    }
  }

  printf("Best run time overall: %gs\n", min_t);
  free(nums);
  free(lines);
  free(data);
  return 0;
}
