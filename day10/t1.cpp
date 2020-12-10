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

struct distribution {
  int diff[3];
};

distribution
t1(vi& v) {
  distribution ret = {};
  int last = 0;

  std::sort(v.begin(), v.end());

  for (auto a : v) {
	ret.diff[a - last - 1]++;
	last = a;
  }
  ret.diff[2]++;
  return ret;
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
  printf("%d:%d:%d (1*3 = %d)\n", res.diff[0], res.diff[1], res.diff[2], res.diff[0] * res.diff[2]);
  return 0;
}
