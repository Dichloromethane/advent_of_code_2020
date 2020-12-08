#include <cstdio>
#include <fstream>
#include <map>
#include <set>
#include <string.h>
#include <vector>
extern "C" {
#include <unistd.h>
}

enum instr_type {
  INSTR_INVALID = 0,
  INSTR_ADD,
  INSTR_JMP,
  INSTR_NOP,
  INSTR_UNREACHABLE,
  INSTR_EXIT
};

struct instr {
  instr_type type;
  union {
	int pc_off;
	int acc_change;
	int nop_num;
  };
};

struct interp_state {
  int acc;
  int pc;
  std::vector<instr> instr_stream;
};

void
dump_inter_state(const interp_state &state, const char *prefix) {
  const auto &ci = state.instr_stream[state.pc];
  printf("%s", prefix);
  switch (ci.type) {
	case INSTR_ADD:
	  printf("%d: add %d (acc %d)\n", state.pc, ci.acc_change, state.acc);
	  break;
	case INSTR_JMP:
	  printf("%d: jmp %d (acc %d)\n", state.pc, ci.pc_off, state.acc);
	  break;
	case INSTR_NOP:
	  printf("%d: nop\n", state.pc);
	  break;
	case INSTR_EXIT:
	  printf("%d: Exit\n", state.pc);
	default:
	  printf("%d: other ins\n", state.pc);
  }
}

#define INSTRUMENTATION_TERM INT32_MAX

int
interp(interp_state &state, auto &instrumentation, const char *prefix) {
  const instr *curr = &state.instr_stream[state.pc];
  for (;;) {
	dump_inter_state(state, prefix);
	if (!instrumentation(state, *curr))
	  return INSTRUMENTATION_TERM;
	switch (curr->type) {
	  case INSTR_ADD:
		state.acc += curr->acc_change;
		break;
	  case INSTR_JMP:
		state.pc += curr->pc_off - 1;
		break;
	  case INSTR_NOP:
		break;
	  case INSTR_EXIT:
		return state.acc;
	  case INSTR_UNREACHABLE:
		printf("Impossible to reach instruction reached\n");
		abort();
	  default:
		printf("Invalid instruction");
		abort();
	}
	state.pc++;
	curr = &state.instr_stream[state.pc];
  }
}

int
term_test(interp_state is) {
  std::set<int> seen;
  int res = -1;

  auto instrumentation = [&](const interp_state &state, const instr &curr) {
	if (seen.contains(state.pc)) {
	  res = state.acc;
	  return 0;
	}
	seen.insert(state.pc);
	return 1;
  };

  return interp(is, instrumentation, "  ");
}

int
change_test(interp_state is) {
  int terminating_acc = -1;
  std::set<int> seen;
  auto instrumentation = [&](const interp_state &state, const instr &curr) {
	if (seen.contains(state.pc)) {
	  return 0;
	}
	seen.insert(state.pc);

	interp_state is = state;
	auto &new_instr = is.instr_stream[is.pc];
	if (curr.type == INSTR_NOP || curr.type == INSTR_JMP) {
	  printf("------ ");
	  if (curr.type == INSTR_NOP) {
		printf(" NOP -> JMP\n");
		new_instr.type = INSTR_JMP;
		new_instr.pc_off = new_instr.nop_num;
	  } else if (curr.type == INSTR_JMP) {
		printf(" JMP -> NOP\n");
		new_instr.type = INSTR_NOP;
		new_instr.nop_num = new_instr.pc_off;
	  }
	  auto res = term_test(is);
	  if (res != INSTRUMENTATION_TERM) {
		terminating_acc = res;
		return 0;
	  }
	}
	return 1;
  };
  interp(is, instrumentation, "");
  return terminating_acc;
}

void
read_input(std::vector<instr> &v, std::ifstream &f) {
  std::string s;
  instr i;
  int num;
  char buf[3];
  while (std::getline(f, s)) {
	sscanf(s.c_str(), "%s %d", buf, &num);
	switch (buf[0]) {
	  case 'n':
		i.type = INSTR_NOP;
		i.nop_num = num;
		break;
	  case 'j':
		i.type = INSTR_JMP;
		i.pc_off = num;
		break;
	  case 'a':
		i.type = INSTR_ADD;
		i.acc_change = num;
		break;
	  default:
		printf("Illegal op code\n");
	}
	v.push_back(i);
  }
  i.type = INSTR_EXIT;
  v.push_back(i);
}


int
main(int argc, char **argv) {
  std::ifstream f("t1.dat");
  std::vector<instr> instr_stream;
  read_input(instr_stream, f);
  interp_state is = {0, 0, instr_stream};
  int res = change_test(is);
  printf("%d\n", res);
  return 0;
}
