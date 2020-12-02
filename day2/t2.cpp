
#include <cstddef>
#include<vector>
#include<iostream>
#include<fstream>
#include<cstdio>

typedef struct {
  int first;
  int second;
  char c;
  char *pwd;
} dp;

void
read_input(std::vector<dp>& v) {
  std::string s;
  dp d;
  while (scanf("%d-%d %c: %ms", &d.first, &d.second, &d.c, &d.pwd) != EOF)
	v.push_back(d);
}

int
t1(const std::vector<dp>& v) {
  int num = 0;
  for (auto &a : v) 
	num += (a.pwd[a.first - 1] == a.c) ^ (a.pwd[a.second - 1] == a.c);
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
