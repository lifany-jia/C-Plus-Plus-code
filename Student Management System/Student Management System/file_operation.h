//
//  file_operation.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "student_manage.h"
#include <string>

class FileOperation {
private:
    FileOperation() = delete;
    static const char BINARY_MAGIC[4];
    struct FileHeader {
        char magic[4];
        int version;
        int studentCount;
    };
public:
    static bool saveBinary(const StudentManage& manager);
    static bool loadBinary(StudentManage& manager);
    
    static bool saveText(const StudentManage& manager);
    static bool loadText(StudentManage& manager);
    
    static std::string lastError;
    static void setError(const std::string& error);
    static std::string getError();
};

#endif
