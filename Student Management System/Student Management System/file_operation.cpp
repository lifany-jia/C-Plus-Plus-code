//
//  file_operation.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#include "file_operation.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;

const char FileOperation::BINARY_MAGIC[4] = {'S', 'M', 'S', '1'};
string FileOperation::lastError = "";

void FileOperation::setError(const std::string& error) {
    lastError = error;
}
string FileOperation::getError() {
    return lastError;
}

bool FileOperation::saveBinary(const StudentManage& manager, const std::string& filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        setError("无法打开文件" + filename);
        return false;
    }
    FileHeader header;
    memcpy(header.magic, BINARY_MAGIC, 4);
    header.version = 1;
    header.studentCount = manager.getSize();
    
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!file) {
        setError("写入文件头失败" + filename);
        file.close();
        return false;
    }
    StudentNode* curr = manager.getHead();
    while (curr) {
        const Information& info = curr->data;
        // 创建别名，引用，同一个地址
        int id = info.getId();
        string name = info.getName();
        string className = info.getClassName();
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        size_t nameLen = name.length();
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        file.write(name.c_str(), nameLen);
        
        size_t classLen = className.length();
        file.write(reinterpret_cast<const char*>(&classLen), sizeof(classLen));
        file.write(className.c_str(), classLen);
        
        int subjectCount = info.getScoreCount();
        file.write(reinterpret_cast<const char*>(&subjectCount), sizeof(subjectCount));
        const Subject* sub = info.getSubjectHead();
        while (sub) {
            int score = sub->score;
            string subjectName = sub->name;
            size_t subjectNameLen = subjectName.length();
            file.write(reinterpret_cast<const char*>(&subjectNameLen), sizeof(subjectNameLen));
            file.write(subjectName.c_str(), subjectNameLen);
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
            sub = sub->next;
        }
        if (!file) {
            setError("写入学生数据出错" + filename);
            file.close();
            return false;
        }
        curr = curr->next;
    }
    file.close();
    return true;
}

bool FileOperation::saveText(const StudentManage &manager, const std::string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        setError("无法打开文本文件进行保存:" + filename);
        return false;
    }
    file << "# Student Management System - Text " << endl;
    file << "# Version: 1.0" << endl;
    file << "# Total Student: " << manager.getSize() << endl;
    file << "==============================================================" << endl;
    
    StudentNode* curr = manager.getHead();
    while (curr) {
        const Information& info = curr->data;
        file << info.getId() << "," << info.getName() << "," << info.getClassName();
        const Subject* sub = info.getSubjectHead();
        while (sub) {
            file << "," << sub->name << ":" << sub->score;
            sub = sub->next;
        }
        file << endl;
        curr = curr->next;
    }
    file.close();
    return true;
}
