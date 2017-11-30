#include <sys/ptrace.h>

#include "register.h"

uint64_t get_register_value(pid_t pid, reg r) {
        user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

        switch (r) {
                case reg::rip:
                        return regs.rip;
                case reg::orig_rax:
                        return regs.orig_rax;
                case reg::rax:
                        return regs.rax;
                case reg::rdx:
                        return regs.rdx;
                case reg::rcx:
                        return regs.rcx;
                case reg::rbx:
                        return regs.rbx;
                case reg::rsi:
                        return regs.rsi;
                case reg::rdi:
                        return regs.rdi;
                case reg::rbp:
                        return regs.rbp;
                case reg::rsp:
                        return regs.rsp;
                case reg::r8:
                        return regs.r8;
                case reg::r9:
                        return regs.r9;
                case reg::r10:
                        return regs.r10;
                case reg::r11:
                        return regs.r11;
                case reg::r12:
                        return regs.r12;
                case reg::r13:
                        return regs.r13;
                case reg::r14:
                        return regs.r14;
                case reg::r15:
                        return regs.r15;
                case reg::rflags:
                        return regs.eflags;
                case reg::es:
                        return regs.es;
                case reg::cs:
                        return regs.cs;
                case reg::ss:
                        return regs.ss;
                case reg::ds:
                        return regs.ds;
                case reg::fs:
                        return regs.fs;
                case reg::gs:
                        return regs.gs;
                case reg::fs_base:
                        return regs.fs_base;
                case reg::gs_base:
                        return regs.gs_base;
        }
}

void set_register_value(pid_t pid, reg r, uint64_t value) {
        user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

        switch (r) {
                case reg::rip:
                        regs.rip = value;
                        break;
                case reg::orig_rax:
                        regs.orig_rax = value;
                        break;
                case reg::rax:
                        regs.rax = value;
                        break;
                case reg::rdx:
                        regs.rdx = value;
                        break;
                case reg::rcx:
                        regs.rcx = value;
                        break;
                case reg::rbx:
                        regs.rbx = value;
                        break;
                case reg::rsi:
                        regs.rsi = value;
                        break;
                case reg::rdi:
                        regs.rdi = value;
                        break;
                case reg::rbp:
                        regs.rbp = value;
                        break;
                case reg::rsp:
                        regs.rsp = value;
                        break;
                case reg::r8:
                        regs.r8 = value;
                        break;
                case reg::r9:
                        regs.r9 = value;
                        break;
                case reg::r10:
                        regs.r10 = value;
                        break;
                case reg::r11:
                        regs.r11 = value;
                        break;
                case reg::r12:
                        regs.r12 = value;
                        break;
                case reg::r13:
                        regs.r13 = value;
                        break;
                case reg::r14:
                        regs.r14 = value;
                        break;
                case reg::r15:
                        regs.r15 = value;
                        break;
                case reg::rflags:
                        regs.eflags = value;
                        break;
                case reg::es:
                        regs.es = value;
                        break;
                case reg::cs:
                        regs.cs = value;
                        break;
                case reg::ss:
                        regs.ss = value;
                        break;
                case reg::ds:
                        regs.ds = value;
                        break;
                case reg::fs:
                        regs.fs = value;
                        break;
                case reg::gs:
                        regs.gs = value;
                        break;
                case reg::fs_base:
                        regs.fs_base = value;
                        break;
                case reg::gs_base:
                        regs.gs_base = value;
                        break;
                default:
                        break;
        }

        ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

uint64_t get_register_value_by_dwarf_num(pid_t pid, unsigned dwarf_num) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                        [dwarf_num](auto&& rd) { return rd.dwarf_r == dwarf_num; });

        if (it == end(g_register_descriptors)) {
                throw std::out_of_range{"Unknown dwarf register"};
        }

        return get_register_value(pid, it->r);
}

std::string get_register_name(reg r) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                        [r](auto&& rd) { return rd.r == r; });

        return it->name;
}

reg get_register_from_name(const std::string& name) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                        [name](auto&& rd) { return rd.name == name; });

        return it->r;
}
