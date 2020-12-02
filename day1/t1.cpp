
#include<vector>
#include<algorithm>
#include<ranges>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<x86intrin.h>

using vi=std::vector<int>;

int
t1(vi& v) {
  std::sort(v.begin(), v.end());

  for (int i = 0; i < v.size(); i++) {
	for (int j = 0; j < i; j++) {
	  if (v[i] + v[j] == 2020) {
		return v[i] * v[j];
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
  std::ifstream in("t1.dat");

  while (std::getline(in, s))
	v.push_back(atoi(s.c_str()));
  
  t1(v);
  d1 = __rdtsc();
  res = t1(v);
  d2 = __rdtsc();

  printf("%lu\n", d2 - d1);

  std::printf("%d\n", res); 

  return 0;
}
