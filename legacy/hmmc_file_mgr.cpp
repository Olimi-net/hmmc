#include "hmmc_file_mgr.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <system_error>
namespace fs = std::filesystem;

hmmc_file_mgr::hmmc_file_mgr(){
    file_flag = 0;
}

hmmc_file_mgr::~hmmc_file_mgr(){
    if(file_flag & 2){
        fclose(file);
    }
    file_array.clear();
}

int hmmc_file_mgr::get_files(const char* dir){
    file_array.clear();
    std::string s = dir;
    fs::path myPath = s;
    std::error_code ec;
    if(!(fs::exists(myPath, ec) && fs::is_directory(myPath, ec))) return -1;
    if(ec) return -2;

    int count = 0;

    for (fs::directory_entry entry : fs::directory_iterator(myPath)){
        if (entry.is_directory()) {
            std::cout << "[Папка] " << entry.path() << std::endl;
        } else {
            file_array.push_back(entry.path());
            std::cout << "[Файл ] " << entry.path() << " (Размер: " << entry.file_size() << " байт)" << std::endl;
        }

        count++;
        //std::string f = entry.path();
        //std::cout << f << std::endl;
    }
    std::cout << count << std::endl;
    return count;
}

/*
    "r": Открывает файл для чтения (текстовый режим). 
    "w": Открывает файл для записи. Если файл существует, его содержимое удаляется; если нет — создается новый файл. 
    "a": Открывает файл для добавления данных в конец файла. 
    "b": Открывает файл в бинарном режиме (можно использовать вместе с другими режимами, например, "rb" или "wb"). 
    "+": Открывает файл для обновления (чтение и запись).
        "r+" открывает существующий файл для чтения и записи,
      а "w+" — для чтения и записи (создавая новый файл, если он не существует). 
*/

int hmmc_file_mgr::open_file(const char *fname, int flag){
    char c[3] = {'\0','\0','\0'};
    if(flag & HMMC_FILE_MGR_READ) c[0] = 'r';
    else if((flag & HMMC_FILE_MGR_APPEND) == HMMC_FILE_MGR_APPEND) c[0] = 'a';
    else if(flag & HMMC_FILE_MGR_WRITE) c[0] = 'w';
    if(flag & HMMC_FILE_MGR_BINARY) c[1] = 'b';
    file = fopen(fname, c);
    file_flag = (file == NULL) ? 1 : 2;
    return (file_flag & 3) == 2;
}

void hmmc_file_mgr::write_char(char c){
    if((file_flag & 3) != 2) return;
    char cs[2] = {c,'\0'};
    fprintf(file, cs);
}

int hmmc_file_mgr::read_line(int count, char* row){
    char value = 0;
    int j = 0;
    while(!feof(file)){
        fscanf(file, "%c", &value);
        row[j] = value;
        j++;
        if(j >= count || value == '\n') return j;
    }
    return j;
}
