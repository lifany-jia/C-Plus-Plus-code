//
//  file_operation.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#include "file_operation.h"
#include <fstream>
#include <iostream>
#include <cstring>

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

bool FileOperation::loadBinary(StudentManage &manager, const std::string &filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        setError("无法打开文件进行读取：" + filename);
        return false;
    }
    
    FileHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!file) {
        setError("读取文件头出错");
        file.close();
        return false;
    }
    if (memcmp(header.magic, BINARY_MAGIC, 4) != 0) {
        setError("文件格式错误：不是有效二进制文件");
        file.close();
        return false;
    }
    if (header.version != 1) {
        setError("不支持的文件版本：" + to_string(header.version));
        file.close();
        return false;
    }
    manager.clear();
    for (int i = 0; i < header.studentCount; i++) {
        int id;
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        if (!file) {
            setError("读取学生ID出错");
            file.close();
            return false;
        }
        size_t nameLen;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        if (!file) {
            setError("读取学生姓名长度出错");
            file.close();
            return false;
        }
        string name(nameLen, ' ');
        file.read(&name[0], nameLen);
        if (!file) {
            setError("读取学生姓名出错");
            file.close();
            return false;
        }
        size_t classLen;
        file.read(reinterpret_cast<char*>(&classLen), sizeof(classLen));
        if (!file) {
            setError("读取学生班级名称长度出错");
            file.close();
            return false;
        }
        string className(classLen, ' ');
        file.read(&className[0], classLen);
        if (!file) {
            setError("读取学生班级名称出错");
            file.close();
            return false;
        }
        Information info(id, name, className);
        int subjectCount;
        file.read(reinterpret_cast<char*>(&subjectCount), sizeof(subjectCount));
        if (!file) {
            setError("读取学生科目数目出错");
            file.close();
            return false;
        }
        for (int j = 0; j < subjectCount; j++) {
            size_t subNameLen;
            file.read(reinterpret_cast<char*>(&subNameLen), sizeof(subNameLen));
            if (!file) {
                setError("读取学生科目名称长度出错");
                file.close();
                return false;
            }
            string subjectName(subNameLen, ' ');
            file.read(&subjectName[0], subNameLen);
            if (!file) {
                setError("读取学生科目名称出错");
                file.close();
                return false;
            }
            int score;
            file.read(reinterpret_cast<char*>(&score), sizeof(score));
            if (!file) {
                setError("读取学生成绩出错");
                file.close();
                return false;
            }
            if (!info.addSubject(subjectName, score)) {
                setError("添加科目失败：" + subjectName);
                file.close();
                return false;
            }
        }
        
        if (!manager.addStudent(info)) {
            setError("添加学生失败：" + name);
            file.close();
            return false;
        }
        
    }
    file.close();
    return true;
}


    
    
bool FileOperation::loadText(StudentManage &manager, const std::string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        setError("无法打开文本文件：" + filename);
        return false;
    }
    manager.clear();
        
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        vector<string> tokens;
        size_t start = 0;
        size_t end = line.find(',');
        while (end != string::npos) {
            tokens.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(',', start);
        }
        tokens.push_back(line.substr(start));
        
        if (tokens.size() < 3) {
            setError("第" + to_string(lineNum) + "行格式错误：数据字段不足");
            file.close();
            return false;
        }
        try {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            string className = tokens[2];
            
            Information info(id, name, className);
            
            for (size_t i = 3; i < tokens.size(); i++) {
                string token = tokens[i];
                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                size_t colonPos = token.find(':');
                if (colonPos != string::npos) {
                    string subjectName = token.substr(0, colonPos);
                    string scoreStr = token.substr(colonPos + 1);
                    int score = stoi(scoreStr);
                    
                    if (!info.addSubject(subjectName, score)) {
                        setError("第" + to_string(lineNum) + "行：添加科目失败-" + subjectName);
                        file.close();
                        return false;
                    }
                }
            }
            if (!manager.addStudent(info)) {
                setError("第" + to_string(lineNum) + "行：学生ID重复或添加失败");
                file.close();
                return false;
            }
        } catch (const exception& e) {
            setError("第" + to_string(lineNum) + "行解析错误：" + string(e.what()));
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool FileOperation::save(const StudentManage &manager, const std::string &filename, Format format) {
    if (filename.empty()) {
        setError("文件名不能为空");
        return false;
    }
    if (manager.getSize() == 0) {
        setError("没有学生数据可以保存");
        return false;
    }
    switch (format) {
        case BINARY:
            return saveBinary(manager, filename);
        case TEXT:
            return saveText(manager, filename);
        default:
            setError("不支持的格式类型");
            return false;
    }
}

bool FileOperation::load(StudentManage &manager, const std::string &filename) {
    if (filename.empty()) {
        setError("文件名不能为空");
        return false;
    }
    Format detectedFormat = detectFormat(filename);
    
    switch (detectedFormat) {
        case BINARY:
            return loadBinary(manager, filename);
        case TEXT:
            return loadText(manager, filename);
        default:
            setError("无法识别文件格式：" + filename);
            return false;
    }
}

FileOperation::Format FileOperation::detectFormat(const std::string &filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return UNKNOWN;
    }
    
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);
    if (fileSize < 4) {
        file.close();
        return TEXT;
    }
    
    char magic[4];
    file.read(magic, 4);
    file.close();
    
    if (file.gcount() == 4 && memcmp(magic, BINARY_MAGIC, 4) == 0) {
        return BINARY;
    }
    return TEXT;
}
