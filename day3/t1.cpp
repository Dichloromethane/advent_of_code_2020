
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <x86intrin.h>

using vi = std::vector<int>;
using vvi = std::vector<std::vector<int>>;

#define DX 1
#define DY 3

int
t1(vvi &v) {
  int x, y;
  int ntrees = 0;
  int nmul = 1;
  std::vector<std::pair<int, int>> vp = {{1, 3}};
  for (auto &a : vp) {
	ntrees = 0;
	for (x = 0, y = 0; x < v.size(); x += a.first, y += a.second) {
	  auto &xvec = v[x % v.size()];
	  if (xvec[y % xvec.size()])
		ntrees++;
	}
	nmul *= ntrees;
  }
  return nmul;
}

int
main(int argc, char **argv) {
  int res;
  std::string s;
  std::ifstream in("t1.dat");
  std::vector<std::vector<int>> v;

  while (std::getline(in, s)) {
	v.emplace_back();
	std::transform(s.begin(), s.end(), std::back_inserter(*(v.end() - 1)),
				   [](char c) -> int { return c == '#'; });
  }

  res = t1(v);

  std::printf("%d\n", res);

  return 0;
}
