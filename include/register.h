#ifndef XDBG_REGISTER_H
#define XDBG_REGISTER_H

#include <sys/user.h>
#include <algorithm>
#include <array>

enum class reg {
        rax, rbx, rcx, rdx,
        rsi, rdi, rbp, rsp,
        r8,  r9,  r10, r11,
        r12, r13, r14, r15,
        rflags,         es,
        cs, ss, ds, fs, gs,
        fs_base,   gs_base,
        orig_rax,      rip,
};

constexpr std::size_t n_registers = 27;

struct reg_descriptor {
        reg r;
        int dwarf_r;
        std::string name;
};

const std::array<reg_descriptor, n_registers> g_register_descriptors {{
        { reg::gs_base, 59, "gs_base" },
        { reg::fs_base, 58, "fs_base" },
        { reg::gs,  55, "gs"  },
        { reg::fs,  54, "fs"  },
        { reg::ds,  53, "ds"  },
        { reg::ss,  52, "ss"  },
        { reg::cs,  51, "cs"  },
        { reg::es,  50, "es"  },
        { reg::rflags, 49, "eflags" },
        { reg::r15, 15, "r15" },
        { reg::r14, 14, "r14" },
        { reg::r13, 13, "r13" },
        { reg::r12, 12, "r12" },
        { reg::r11, 11, "r11" },
        { reg::r10, 10, "r10" },
        { reg::r9,  9,  "r9"  },
        { reg::r8,  8,  "r8"  },
        { reg::rsp, 7,  "rsp" },
        { reg::rbp, 6,  "rbp" },
        { reg::rdi, 5,  "rdi" },
        { reg::rsi, 4,  "rsi" },
        { reg::rbx, 3,  "rbx" },
        { reg::rcx, 2,  "rcx" },
        { reg::rdx, 1,  "rdx" },
        { reg::rax, 0,  "rax" },
        { reg::orig_rax, -1, "orig_rax" },
        { reg::rip, -1, "rip" },
}};

uint64_t get_register_value(pid_t, reg r);
void set_register_value(pid_t pid, reg r, uint64_t value);
uint64_t get_register_value_by_dwarf_num(pid_t pid, unsigned dwarf_num);
std::string get_register_name(reg r);
reg get_register_from_name(const std::string& name);

#endif
