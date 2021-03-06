//===------------------------------------------------------------*- C++ -*-===//
//
//                     Created by F8LEFT on 2017/6/3.
//                   Copyright (c) 2017. All rights reserved.
//===----------------------------------------------------------------------===//
//  Parse and read elf file.
//===----------------------------------------------------------------------===//

#ifndef SOFIXER_ELFREADER_H
#define SOFIXER_ELFREADER_H

#import "exelf.h"


#include <cstddef>


class ElfReader {
public:
    ElfReader(const char* name, int fd);
    ~ElfReader();

    bool Load();

    size_t phdr_count() { return phdr_num_; }
    Elf_Addr load_start() { return reinterpret_cast<Elf_Addr>(load_start_); }
    Elf_Addr load_size() { return load_size_; }
    Elf_Addr load_bias() { return load_bias_; }
    const Elf_Phdr* loaded_phdr() { return loaded_phdr_; }

    const Elf_Ehdr* record_ehdr() { return &header_; }
private:
    bool ReadElfHeader();
    bool VerifyElfHeader();
    bool ReadProgramHeader();
    bool ReserveAddressSpace();
    bool LoadSegments();
    bool FindPhdr();
    bool CheckPhdr(Elf_Addr);
    bool LoadFileData(void* addr, size_t len, int offset);

    const char* name_;
    int fd_;

    Elf_Ehdr header_;
    size_t phdr_num_;

    void* phdr_mmap_;
    Elf_Phdr* phdr_table_;
    Elf_Addr phdr_size_;

    // First page of reserved address space.
    void* load_start_;
    // Size in bytes of reserved address space.
    Elf_Addr load_size_;
    // Load bias.
    Elf_Addr load_bias_;

    // Loaded phdr.
    const Elf_Phdr* loaded_phdr_;

    // feature
public:
    void setDumpSoFile(bool b) { dump_so_file_ = b; }
private:
    bool dump_so_file_ = false;
};



size_t
phdr_table_get_load_size(const Elf_Phdr* phdr_table,
                         size_t phdr_count,
                         Elf_Addr* min_vaddr = NULL,
                         Elf_Addr* max_vaddr = NULL);

int
phdr_table_protect_segments(const Elf_Phdr* phdr_table,
                            int               phdr_count,
                            Elf_Addr        load_bias);

int
phdr_table_unprotect_segments(const Elf_Phdr* phdr_table,
                              int               phdr_count,
                              Elf_Addr        load_bias);

int
phdr_table_protect_gnu_relro(const Elf_Phdr* phdr_table,
                             int               phdr_count,
                             Elf_Addr        load_bias);


int phdr_table_get_arm_exidx(const Elf_Phdr* phdr_table,
                         int               phdr_count,
                         Elf_Addr        load_bias,
                         Elf_Addr**      arm_exidx,
                         unsigned*         arm_exidix_count);

void
phdr_table_get_dynamic_section(const Elf_Phdr* phdr_table,
                               int               phdr_count,
                               Elf_Addr        load_bias,
                               Elf_Dyn**       dynamic,
                               size_t*           dynamic_count,
                               Elf_Word*       dynamic_flags);


#endif //SOFIXER_ELFREADER_H
