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
    static bool saveBinary(const StudentManage& manager, const std::string& filename);
    static bool loadBinary(StudentManage& manager, const std::string& filename);
    
    static bool saveText(const StudentManage& manager, const std::string& filename);
    static bool loadText(StudentManage& manager, const std::string& filename);
    
    static std::string lastError;
    static void setError(const std::string& error);
public:
    enum Format {
        BINARY, TEXT, UNKNOWN
    };
    static bool save(const StudentManage& manager, const std::string& filename, Format format = BINARY);
    static bool load(StudentManage& manager, const std::string& filename);
    static std::string getError();
    static Format detectFormat(const std::string& filename);
};

#endif
