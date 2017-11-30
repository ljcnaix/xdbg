#include <sys/ptrace.h>
#include <unistd.h>

#include <iostream>

#include "linenoise.h"
#include "debugger.h"

using namespace xdbg;

void execute_debugee (const std::string &prog_name) {
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
                std::cerr << "Error in ptrace\n";
                return;
        } 

        execl(prog_name.c_str(), prog_name.c_str(), nullptr);
}

int main(int argc, char *argv[]) {
        if (argc < 2) {
                std::cerr << "Program name not specified";
                return -1;
        }
    
        auto prog = argv[1];
        auto pid = fork();

        if (pid == 0) {
                execute_debugee(prog);
        } else if (pid >= 1) {
                debugger dbg{prog, pid};
                dbg.run();
        }
}

