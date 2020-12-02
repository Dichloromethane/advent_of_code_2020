
#include <cstddef>
#include<vector>
#include<iostream>
#include<fstream>
#include<cstdio>

typedef struct {
  int min;
  int max;
  char c;
  char *pwd;
} dp;

void
read_input(std::vector<dp>& v) {
  std::string s;
  dp d;
  while (scanf("%d-%d %c: %ms", &d.min, &d.max, &d.c, &d.pwd) != EOF)
	v.push_back(d);
}

int
t1(const std::vector<dp>& v) {
  int num = 0;
  for (auto &a : v) {
	int cnt = 0;
    for (int i = 0; a.pwd[i]; i++) {
	  if (a.pwd[i] == a.c)
		cnt++;
	}
	if (a.min <= cnt && cnt <= a.max)
	  num++;
  }
  return num;
}

int
main(int argc, char **argv) {
  std::vector<dp> v;
  int res;
  read_input(v);
  res = t1(v);
  printf("%d\n", res);
  return 0;
}
