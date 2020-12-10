#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define DEBUG 0

using vi = std::vector<int>;

long
t1(vi& v) {
  long ret = {};
  int last = 0;
  vi num_ways;

  std::sort(v.begin(), v.end());
  size_t end = v.emplace_back(*std::prev(v.end())+3);

  std::vector<long> nways(end+1);
  nways[0] = 1;

  for (auto a : v) {
	if (a - 3 >= 0)
	  nways[a] += nways[a - 3];
	if (a - 2 >= 0)
	  nways[a] += nways[a - 2];
	if (a - 1 >= 0)
	  nways[a] += nways[a - 1];
  }

  return *std::prev(nways.end());
}

template<typename T>
void
read_input(std::vector<T> &v, std::istream &in) {
  std::string s;
  while (std::getline(in, s)) {
	T num;
	sscanf(s.c_str(), "%d", &num);
	v.push_back(num);
  }
}

int
main(int argc, char **argv) {
  vi v;
  read_input(v, std::cin);
  auto res = t1(v);
  printf("%ld\n", res);
  return 0;
}
