#include<cstdio>
extern "C" {
#include <sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdint.h>
#include<x86intrin.h>
}

#define SIZE (7+3)

void *
read_input(const char *file, size_t *l) {
  char *buffer;
  int fd = open(file, O_RDONLY);

  struct stat st;
  fstat(fd, &st);
  *l = st.st_size;
  buffer = (char *) malloc(*l);
  read(fd, buffer, *l);
  return buffer;
}

int
scan_bp(char *buf) {
  int res;
  res = 0;
  for(int i = 0;; i++) {
	char c = buf[i];
	switch(c) {
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
		return res >> 1;
	  default:
		dprintf(2, "Illegal char %c\n", c);
		return -2;
	}
  }
}

int
t(char *buf, int len) {
  int max = -1, c;
  for (size_t i = 0; i < len; i+=11) {
    c = scan_bp(&buf[i]);
	max = max > c ? max : c;
  }
  return max;
}

int
main(int argc, char **argv) {
  size_t len;
  uint64_t t1, t2;
  int res;
  void *buf;

  buf = read_input("t1.dat", &len);
  (void) buf;

  res = t((char *)buf, len);
  t1 = __rdtsc();
  res = t((char *)buf, len);
  t2 = __rdtsc();
  //res = t1_fast(test_input, sizeof(test_input) - 1);

  printf("%d (%x) in %lu (%f per char input)\n", res, res, t2 - t1, ((double) t2 -t1)/len);
}
