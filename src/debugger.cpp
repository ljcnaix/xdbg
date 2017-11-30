#include <sys/ptrace.h>
#include <sys/wait.h>
#include <iostream>
#include <iomanip>
#include <vector>

#include "linenoise.h"
#include "debugger.h"
#include "utils.h"
#include "breakpoint.h"
#include "register.h"

using namespace xdbg;

void debugger::handle_command(const std::string &line) {
        auto args = split(line, ' ');
        auto command = args[0];

        if (is_prefix(command, "c"))
        {
                continue_execution();
        }
        else if (is_prefix(command, "b"))
        {
                std::string addr{args[1], 2};
                set_breakpoint_at_address(std::stol(addr, 0, 16));
        }
        else if (is_prefix(command, "reg"))
        {
                if (is_prefix(args[1], "d"))
                {
                        dump_registers();
                }
                else if (is_prefix(args[1], "r"))
                {
                        std::cout << get_register_value(m_pid, get_register_from_name(args[2])) << std::endl;
                }
                else if (is_prefix(args[1], "w"))
                {
                        std::string val {args[3], 2};
                        set_register_value(m_pid, get_register_from_name(args[2]), std::stol(val, 0, 16));
                }
        }
        else if (is_prefix(command, "mem"))
        {
                std::string addr {args[2], 2};

                if (is_prefix(args[1], "r"))
                {
                        std::cout << std::hex << read_memory(std::stol(addr, 0, 16)) << std::endl;
                }
                else if (is_prefix(args[1], "w"))
                {
                        std::string val {args[3], 2};
                        write_memory(std::stol(addr, 0, 16), std::stol(val, 0, 16));
                }
        }
        else if (is_prefix(command, "help"))
        {
                std::cout << "xdbg commands:" << std::endl << std::endl << std::setfill(' ');
                std::cout << std::left << std::setw(36) << "help" << "show this help message" << std::endl;
                std::cout << std::left << std::setw(36) << "c" << "continue execute subprocess" << std::endl;
                std::cout << std::left << std::setw(36) << "b [addr]" << "set breakpoint" << std::endl;
                std::cout << std::left << std::setw(36) << "reg d" << "dump registers" << std::endl;
                std::cout << std::left << std::setw(36) << "reg r [reg]" << "read register" << std::endl;
                std::cout << std::left << std::setw(36) << "reg w [reg] [val]" << "set register" << std::endl;
                std::cout << std::left << std::setw(36) << "mem r [addr]" << "read memory" << std::endl;
                std::cout << std::left << std::setw(36) << "mem w [addr] [val]" << "set memory" << std::endl;
                std::cout << std::left << std::setw(36) << "exit" << "exit debugger" << std::endl;

        }
        else if (is_prefix(command, "exit"))
        {
                ptrace(PTRACE_KILL, m_pid, 0, 0);
                exit(0);
        }
        else
        {
                std::cerr << "Unknown command\n";
        }
}

void debugger::run() {
        int wait_status;
        auto options = 0;
        waitpid(m_pid, &wait_status, options);

        char *line = nullptr;
        while ((line = linenoise("xdbg> ")) != nullptr) {
                handle_command(line);
                linenoiseHistoryAdd(line);
                linenoiseFree(line);
        }
}

uint64_t debugger::get_pc() {
        return get_register_value(m_pid, reg::rip);
}

void debugger::set_pc(uint64_t pc) {
        set_register_value(m_pid, reg::rip, pc);
}

void debugger::continue_execution() {
        step_over_breakpoint();
        ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

        wait_for_signal();
}

void debugger::set_breakpoint_at_address(std::intptr_t addr) {
        std::cout << "Set breakpoint at address 0x" << std::hex << addr << std::endl;

        breakpoint bp{m_pid, addr};
        bp.enable();
        m_breakpoints[addr] = bp;
}

void debugger::step_over_breakpoint() {
        auto possible_breakpoint_location = get_pc() - 1;

        if (m_breakpoints.count(possible_breakpoint_location)) {
                auto& bp = m_breakpoints[possible_breakpoint_location];

                if (bp.is_enable()) {
                        auto previous_instruction_address = possible_breakpoint_location;
                        set_pc(previous_instruction_address);

                        bp.disable();
                        ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
                        wait_for_signal();
                        bp.enable();
                }
        }
}

void debugger::wait_for_signal() {
        int wait_status;
        auto options = 0;
        waitpid(m_pid, &wait_status, options);
}

void debugger::dump_registers() {
        for (const auto& rd : g_register_descriptors) {
                std::cout << std::setfill(' ') << std::setw(8) << rd.name << " 0x"
                          << std::setfill('0') << std::setw(16) << std::hex << get_register_value(m_pid, rd.r)
                          << std::endl;
        }
}

uint64_t debugger::read_memory(uint64_t address) {
        return ptrace(PTRACE_PEEKDATA, m_pid, address, nullptr);
}

void debugger::write_memory(uint64_t address, uint64_t value) {
        ptrace(PTRACE_POKEDATA, m_pid, address, value);
}
