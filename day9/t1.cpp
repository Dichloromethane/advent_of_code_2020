#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define DEBUG 0

using vi = std::vector<long>;

struct entry {
  long val;
  int pos;
};

entry
t1(const vi &v, int window_size) {
  for (int pos = window_size + 1; pos < v.size(); pos++) {
	bool has_sum = false;
	for (int i = 1; i <= window_size; i++) {
	  for (int j = 1; j <= window_size; j++) {
		if (i == j)
		  continue;
		if (v[pos] == v[pos - i] + v[pos - j]) {
		  has_sum = true;
		  goto end_window_scan;
		}
	  }
	}
  end_window_scan:
	if (!has_sum)
	  return {v[pos], pos};
  }
  return {-1, -1};
}

auto
t2(const vi &v, entry target) {
  int lower, upper;
  lower = 0;
  upper = 2;
  long sum;

  while ((sum = std::accumulate(v.begin() + lower, v.begin() + upper, 0))
		 != target.val) {
	if (sum < target.val)
	  upper++;
	if (sum > target.val)
	  lower++;
	if (lower + 1 == upper)
	  upper++;
  }

  auto mm = std::minmax_element(v.begin() + lower, v.begin() + upper);

  return std::make_pair(*mm.first, *mm.second);
}

void
read_input(vi &v, std::istream &in) {
  std::string s;
  while (std::getline(in, s)) {
	long num;
	sscanf(s.c_str(), "%ld", &num);
	v.push_back(num);
  }
}

int
main(int argc, char **argv) {
  vi v;
  entry res;
  std::pair<int, int> window;
  read_input(v, std::cin);
#if defined(DEBUG) && DEBUG
  res = t1(v, 5);
#else
  res = t1(v, 25);
#endif
  window = t2(v, res);
  printf("%ld\n", res.val);
  printf("%d: %d + %d\n", window.first + window.second, window.first,
		 window.second);
}
