#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h>

#include <sys/stat.h>

typedef short vss __attribute__((vector_size(16)));

#define PASS_SIZE (8*8 + 2)
// returns a vector of the ones compliment of candidates for the maximum of the input
vss parse_f(const char *in, int n);

const char *const ifile = "bigboy";

void *
read_input(const char *file, size_t *l) {
  char *buffer;
  int fd = open(file, O_RDONLY);

  struct stat st;
  fstat(fd, &st);
  *l = st.st_size;
  buffer = malloc(*l);
  read(fd, buffer, *l);
  return buffer;
}

int
parse_s(const char *peel_buf, int curr) {
  int c, res;
  res = 0;
  for (int i = 0;; i++) {
	c = peel_buf[i];
	switch (c) {
	  case 'B':
		res = res | 1;
		__attribute__((fallthrough));
	  case 'F':
		res = res << 1;
		break;
	  case 'R':
		res = res | 1;
		__attribute__((fallthrough));
	  case 'L':
		res = res << 1;
		break;
	  case '\n':
		res >>= 1;
		res = res > curr ? res : curr;
		return res;
	  default:
		dprintf(2, "Illegal char %c\n", c);
		return -2;
	}
  }
}



int
t1_fast(const char *buf, size_t len) {
  vss x;
  int intermed_res;
  size_t peel = (len % PASS_SIZE)/11;
  size_t bulk = len / PASS_SIZE;
  const char *peel_buf = buf + PASS_SIZE * bulk;

  x = parse_f(buf, bulk);
#define min(x,y) x < y ? x : y
  x[0] = min(x[0], x[1]);
  x[4] = min(x[4], x[5]);
  x[0] = min(x[0], x[2]);
  x[4] = min(x[4], x[6]);
  intermed_res = min(x[0], x[4]);
  intermed_res = (~intermed_res) & 0x3ff;
  for (int i = 0; i < peel; i++) {
	intermed_res = parse_s(peel_buf, intermed_res);
	peel_buf += 11;
  }
  return intermed_res;
}

const char test_input[] = "FFFFFFFLLR\n"
						  "FFFFFFFLRL\n"
						  "FFFFFFFRLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFFFFFLLL\n"
						  "FFFBBBBLLL\n"
						  "FFFFFFFLLL\n";


int
main(int argc, char **argv) {
  size_t len;
  uint64_t t1, t2;
  int res;
  void *buf;

  buf = read_input(ifile, &len);
  (void) buf;

  res = t1_fast(buf, len);
  t1 = __rdtsc();
  res = t1_fast(buf, len);
  t2 = __rdtsc();
  //res = t1_fast(test_input, sizeof(test_input) - 1);

  printf("%d (%x) in %lu (%f per char input)\n", res, res, t2 - t1, ((double) t2 -t1)/len);
}
