#include<cstdio>
#include<set>

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
t2() {
  int c;
  int last = -10;
  std::set<int> s;
  while ((c = scan_bp()) >= 0) 
	s.insert(c);
  for (auto a : s) {
	if (a - last == 2)
	  return a - 1;
	last = a;
  }
  return -1;
}

int
main(int argc, char **argv) {
  int res;
  res = t2();
  printf("%d\n", res);
  return 0;
}
