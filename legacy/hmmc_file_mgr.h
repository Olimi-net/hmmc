#pragma once
#include <stdio.h>
#include <vector>
#include <string>

const int HMMC_FILE_MGR_READ = 4;
const int HMMC_FILE_MGR_WRITE = 2;
const int HMMC_FILE_MGR_APPEND = 3;
const int HMMC_FILE_MGR_BINARY = 8;

class hmmc_file_mgr{
    public:
        hmmc_file_mgr();
        ~hmmc_file_mgr();
        int open_file(const char *fname, int flag);
        void write_char(char c);
        int read_line(int count, char* row);
        int get_files(const char* path);
    private:
        FILE* file;
        int file_flag;
        std::vector<std::string> file_array;

};

