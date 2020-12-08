#include <cstdio>
#include <fstream>
#include <map>
#include <string.h>
#include <vector>
#include <set>
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
  };
};

struct interp_state {
  int acc;
  int pc;
  std::vector<instr> instr_stream;
};

void
dump_inter_state(const interp_state &state) {
  const auto &ci = state.instr_stream[state.pc];
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
	default:
	  printf("%d: other ins\n", state.pc);
  }
}

void
interp(interp_state &state, auto &instrumentation) {
  instr *curr = &state.instr_stream[state.pc];
  for (;;) {
	if (!instrumentation(state, *curr))
	  return;
	dump_inter_state(state);
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
		printf("Exiting\n");
		return;
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
  interp_state is = {0, 0, std::vector<instr>()};
  read_input(is.instr_stream, f);
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

  interp(is, instrumentation);
  printf("res: %d\n", res);
  return 0;
}
