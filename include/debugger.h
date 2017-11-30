#ifndef XDBG_DEBUGGER_H
#define XDBG_DEBUGGER_H

#include <utility>
#include <string>
#include <unordered_map>

#include <linux/types.h>

#include "breakpoint.h"

namespace xdbg {
        class debugger {
                public:
                        debugger (std::string prog_name, pid_t pid)
                        : m_prog_name{std::move(prog_name)}, m_pid{pid} {}

                        void run();

                private:
                        void handle_command(const std::string &line);

                        uint64_t get_pc();
                        void set_pc(uint64_t pc);

                        void continue_execution();

                        void set_breakpoint_at_address(std::intptr_t addr);
                        void step_over_breakpoint();

                        void wait_for_signal();

                        void dump_registers();

                        uint64_t read_memory(uint64_t address);
                        void write_memory(uint64_t address, uint64_t value);

                        std::string m_prog_name;
                        pid_t m_pid;
                        std::unordered_map<std::intptr_t,breakpoint> m_breakpoints;
        };
}

#endif
