
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>

template<typename T>
using v = std::vector<T>;

template<typename T, typename S>
using p = std::pair<T, S>;

template<typename T, typename S>
using map = std::map<T, S>;

using string = std::string;

template<std::size_t N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...>>::type;

template<typename NodeData, typename EdgeData>
class dag {
  using nodeptr = int;
  struct dag_node {
	v<p<nodeptr, EdgeData>> children;
	v<p<nodeptr, EdgeData *>> parents;
	NodeData d;
	template<typename U, typename V, typename W>
	dag_node(U &&ch, V &&pa, W &&d):
		children(std::forward<U>(ch)), parents(std::forward<V>(pa)),
		d(std::forward<W>(d)) {}
  };

  v<dag_node> n;

  template<typename Policy>
  void intern_apply_from_node_nodes(int start, auto &f, std::set<int>& alread_visited) {
	if (alread_visited.contains(start))
	  return;
	auto &curr = n[start];
	alread_visited.insert(start);

	if constexpr ((Policy::value & policy::DepthFirst::value) == 0) {
	  f(curr.d);
	}

	if constexpr ((Policy::value & policy::Children::value) != 0) {
	  for (auto &e : curr.children) {
		intern_apply_from_node_nodes<Policy>(e.first, f, alread_visited);
	  }
	} else {
	  for (auto &e : curr.parents) {
		intern_apply_from_node_nodes<Policy>(e.first, f, alread_visited);
	  }
	}

	if constexpr ((Policy::value & policy::DepthFirst::value) != 0) {
	  f(curr.d);
	}
  }


public:
  dag() {}

  __attribute__((noinline)) void dump() {
	std::cout << "[" << "\n";
	for (auto& no : n) {
	  std::cout << "  \"" << no.d << "\" children:\n";
	  for (auto& ch : no.children)
		std::cout << "      " << n[ch.first].d << ":" << ch.second << "\n";
	  std::cout << "    parents\n";
	  for (auto& pa : no.parents)
		std::cout << "      " << n[pa.first].d << ":" << *pa.second << "\n";
	}
	std::cout << "]" << "\n";
  }

  template<typename U>
  int insert_node(U &&d) {
	int ret = n.size();
	n.emplace_back(v<p<int, EdgeData>>{}, v<p<int, EdgeData *>>{},
				   std::forward<U>(d));
	return ret;
  }

  template<typename U>
  void insert_edge(int from, int to, U &&d) {
	auto &f = n[from];
	auto &t = n[to];
	auto &ins_edge = f.children.emplace_back(to, std::forward<U>(d));
	t.parents.emplace_back(from, &ins_edge.second);
  }

  struct policy {
	struct Children: std::integral_constant<int, 0x0> {};
	struct Parents: std::integral_constant<int, 0x1> {};
	struct DepthFirst: std::integral_constant<int, 0x0> {};
	struct NotDepthFirst: std::integral_constant<int, 0x2> {};
	template<typename p1, typename p2>
	struct comb {
	  using type = std::integral_constant<int, p1::value | p2::value>;
	};
  };

  /*
  template<typename F, typename Policy = typename policy::template comb<
							   typename policy::Children,
							   typename policy::DepthFirst>::type>
  void apply_from_node_edges(int start, F &f) {
	auto &curr = n[start];

	if constexpr (Policy() & policy::Children()) {
	  for (auto &e : curr.children) {
		if constexpr (Policy() & policy::DepthFirst()) {
		  apply_from_node_edges<F, Policy>(e.first, f);
		  f(curr.d, n[e.first].d, e.second);
		} else {
		  f(curr.d, n[e.first].d, e.second);
		  apply_from_node_edges<F, Policy>(e.first, f);
		}
	  }
	} else {
	  for (auto &e : curr.parents) {
		if constexpr (Policy() & policy::DepthFirst()) {
		  apply_from_node_edges<F, Policy>(e.first, f);
		  f(curr.d, n[e.first].d, *e.second);
		} else {
		  f(curr.d, n[e.first].d, *e.second);
		  apply_from_node_edges<F, Policy>(e.first, f);
		}
	  }
	}
  }
  */

  template<typename Policy = typename policy::template comb<
							   typename policy::Children,
							   typename policy::DepthFirst>::type>
  void apply_from_node_nodes(int start, auto &f) {
	std::set<int> s;
	intern_apply_from_node_nodes<Policy>(start, f, s);
  }
};

map<string, int>
read_input(auto &g, std::ifstream &input) {
  map<string, int> ret;
  string line;

  ret["shiny gold"] = g.insert_node("shiny gold");

  while (std::getline(input, line)) {
	string tmp;
	string arg;
	int num;
	int nodeid;
	int nodeid_arg;
	string first = "";
	std::istringstream iss(line);

	iss >> tmp;
	first += tmp;
	iss >> tmp;
	first += " " + tmp;
	iss >> tmp;// bag[s]
	iss >> tmp;// contain
	if (ret.find(first) == ret.end()) 
	  ret[first] = g.insert_node(first);
	nodeid = ret[first];
	do {
	  iss >> tmp;
	  if (tmp == "no") {
		goto outer_cont;
	  }	else {
	    num = atoi(tmp.c_str());
	  }
	  iss >> tmp;
	  arg = tmp;
	  iss >> tmp;
	  arg += " " + tmp;
	  if (ret.find(arg) == ret.end()) 
		ret[arg] = g.insert_node(arg);
	  nodeid_arg = ret[arg];
	  g.insert_edge(nodeid, nodeid_arg, num);
	  iss >> tmp;
	} while (tmp[tmp.size() - 1] != '.');
	outer_cont: (void) 0;
  }

  return ret;
}

int
main(int argc, char **argv) {
  dag<string, int> g;
  using dagsi = dag<string, int>;
  std::ifstream input("t1.dat");
  auto m = read_input(g, input);

  std::set<string> reachable;

  g.dump();

  auto f = [&](const string &s) {
	reachable.insert(s);
  };

  g.apply_from_node_nodes<dagsi::policy::Parents>(m["shiny gold"], f);

  for (auto& k : reachable) {
    std::cout << k << "\n";
  }

  std::cout << reachable.size() - 1 << "\n";

  return 0;
}
