
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <x86intrin.h>
#include <vector>

using vi = std::vector<int>;

int
t2(vi &v) {
  std::sort(v.begin(), v.end());

  for (int i = 0; i < v.size(); i++) {
	for (int j = 0; j < i; j++) {
	  for (int k = 0; k < j; k++) {
		if (v[i] + v[j] + v[k] == 2020) {
		  return v[i] * v[j] * v[k];
		}
	  }
	}
  }
  return -1;
}

int
main(int argc, char **argv) {
  uint64_t d1, d2;
  vi v;
  int res;
  std::string s;
  std::ifstream in("t2.dat");

  while (std::getline(in, s))
	v.push_back(atoi(s.c_str()));

  t2(v);
  d1 = __rdtsc();
  res = t2(v);
  d2 = __rdtsc();

  printf("%lu\n", d2 - d1);
  std::printf("%d\n", res);

  return 0;
}
