
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

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
  void intern_apply_from_node_nodes(int start, auto &f,
									std::set<int> &alread_visited) {
	if (alread_visited.contains(start))
	  return;
	auto &curr = n[start];
	alread_visited.insert(start);

	if constexpr ((Policy::value & policy::NotDepthFirst::value) != 0) {
	  f(curr.d);
	}

	if constexpr ((Policy::value & policy::Parents::value) == 0) {
	  for (auto &e : curr.children) {
		intern_apply_from_node_nodes<Policy>(e.first, f, alread_visited);
	  }
	} else {
	  for (auto &e : curr.parents) {
		intern_apply_from_node_nodes<Policy>(e.first, f, alread_visited);
	  }
	}

	if constexpr ((Policy::value & policy::NotDepthFirst::value) == 0) {
	  f(curr.d);
	}
  }

  template<typename Policy>
  void intern_apply_from_node_edges(int start, auto &f,
									std::set<int> &alread_visited) {
	if (alread_visited.contains(start))
	  return;
	auto &curr = n[start];
	alread_visited.insert(start);

	if constexpr ((Policy::value & policy::Parents::value) == 0) {
	  for (auto &e : curr.children) {
		if constexpr ((Policy::value & policy::NotDepthFirst::value) == 0) {
		  intern_apply_from_node_edges<Policy>(e.first, f, alread_visited);
		  f(curr.d, n[e.first].d, e.second);
		} else {
		  f(curr.d, n[e.first].d, e.second);
		  intern_apply_from_node_edges<Policy>(e.first, f, alread_visited);
		}
	  }
	} else {
	  for (auto &e : curr.parents) {
		if constexpr ((Policy::value & policy::NotDepthFirst::value) == 0) {
		  intern_apply_from_node_edges<Policy>(e.first, f, alread_visited);
		  f(curr.d, n[e.first].d, *e.second);
		} else {
		  f(curr.d, n[e.first].d, *e.second);
		  intern_apply_from_node_edges<Policy>(e.first, f, alread_visited);
		}
	  }
	}
  }


public:
  dag() {}

  __attribute__((noinline)) void dump() {
	std::cout << "["
			  << "\n";
	for (auto &no : n) {
	  std::cout << "  \"" << no.d << "\" children:\n";
	  for (auto &ch : no.children)
		std::cout << "      " << n[ch.first].d << ":" << ch.second << "\n";
	  std::cout << "    parents\n";
	  for (auto &pa : no.parents)
		std::cout << "      " << n[pa.first].d << ":" << *pa.second << "\n";
	}
	std::cout << "]"
			  << "\n";
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

  template<
		  typename Policy = typename policy::template comb<
				  typename policy::Children, typename policy::DepthFirst>::type>
  void apply_from_node_edges(int start, auto &f) {
	std::set<int> s;
	intern_apply_from_node_edges<Policy>(start, f, s);
  }

  template<
		  typename Policy = typename policy::template comb<
				  typename policy::Children, typename policy::DepthFirst>::type>
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
	  } else {
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
  outer_cont:
	(void) 0;
  }

  return ret;
}

int
main(int argc, char **argv) {
  dag<string, int> g;
  using dagsi = dag<string, int>;
  std::ifstream input("t2test.dat");
  auto m = read_input(g, input);

  g.dump();

  std::map<string, int> num_bags; // Yes, I know I could put this in the nodes

  auto f = [&](const string &f, const string &t, int num) {
	if (num_bags.find(t) == num_bags.end())
	  num_bags[t] = 0;
	auto num_bags_t = num_bags[t] + 1;
	num_bags[f] += num * num_bags_t;
  };

  for (auto& [k, v] : num_bags)
    std::cout << k << " " << v;

  g.apply_from_node_edges<dagsi::policy::DepthFirst>(m["shiny gold"], f);

  std::cout << num_bags["shiny gold"] << "\n";

  return 0;
}
