#include<cstdio>

#define SIZE (7+3)

int
scan_bp() {
  int c, res;
  res = 0;
  for(;;) {
    c = getchar();	
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
	  case EOF:
		return -1;
	  default:
		dprintf(2, "Illegal char %c\n", c);
		return -2;
	}
  }
}

int
t1() {
  int max = -1, c;
  while ((c = scan_bp()) >= 0) 
	max = max > c ? max : c;
  return max;
}

int
main(int argc, char **argv) {
  int res;
  res = t1();
  printf("%d\n", res);
  return 0;
}
