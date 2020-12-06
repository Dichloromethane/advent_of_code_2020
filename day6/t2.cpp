
#include <istream>
#include<iostream>
#include<vector>
#include<string>
#include<cstdio>

using s = std::string;
using vs = std::vector<s>;
using vvs = std::vector<vs>; 

void
get_input(vvs& dat, std::istream& inf) {
  s cp;
  vs *cg = &dat.emplace_back();
  while (std::getline(inf, cp)) {
	if (cp == "")
	  cg = &dat.emplace_back();
	else 
	  cg->push_back(std::move(cp));
  }
}

int
t1(const vvs& dat) {
  int c, total = 0;
  int tp;
  for (auto &g : dat) {
	c = -1;
	for (auto &p : g) {
	  tp = 0;
	  for (char a : p) {
		tp |= 1 << (a - 'a');
	  }
	  c &= tp;
	}
	total += __builtin_popcount(c);
  }
  return total;
}

int
main(int argc, char **argv) {
  vvs dat;
  int res;
  get_input(dat, std::cin);
  res = t1(dat);
  std::cout << res << "\n";
  return 0;
}
