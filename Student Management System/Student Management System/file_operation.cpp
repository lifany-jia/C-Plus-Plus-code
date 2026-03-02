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

bool FileOperation::saveBinary(const StudentManage& manager) {
    cout << "===========二进制文件保存============" << endl;
    cout << "请输入文件名：" << endl;
    string filename;
    cin >> filename;
    cout << endl;
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "无法打开文件" + filename;
        return false;
    }
    FileHeader header;
    memcpy(header.magic, BINARY_MAGIC, 4);
    header.version = 1;
    header.studentCount = manager.getSize();
    
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!file) {
        cout << "写入文件头失败" + filename;
        file.close();
        return false;
    }
    StudentNode* curr = manager.getHead();
    while (curr) {
        const Information& info = curr->data;
        string id = info.getId();
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
            cout << "写入学生数据出错" + filename;
            file.close();
            return false;
        }
        curr = curr->next;
    }
    file.close();
    cout << "保存成功" << endl;
    return true;
}

bool FileOperation::saveText(const StudentManage &manager) {
    cout << "===========文本文件保存============" << endl;
    cout << "请输入文件名：" << endl;
    string filename;
    cin >> filename;
    cout << endl;
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文本文件进行保存:" + filename;
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
    cout << "保存成功" << endl;
    return true;
}

bool FileOperation::loadBinary(StudentManage &manager) {
    cout << "===========二进制文件加载============" << endl;
    cout << "请输入文件名：" << endl;
    string filename;
    cin >> filename;
    cout << endl;
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "无法打开文件进行读取：" + filename;
        return false;
    }
    FileHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (memcmp(header.magic, BINARY_MAGIC, 4) != 0) {
        cout << "文件格式错误：不是有效二进制文件";
        file.close();
        return false;
    }
    for (int i = 0; i < header.studentCount; i++) {
        string id;
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        size_t nameLen;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, ' ');
        file.read(&name[0], nameLen);
        size_t classLen;
        file.read(reinterpret_cast<char*>(&classLen), sizeof(classLen));
        string className(classLen, ' ');
        file.read(&className[0], classLen);
        Information* info = manager.getStudentById(id);
        if (info) {
            int subjectCount;
            file.read(reinterpret_cast<char*>(&subjectCount), sizeof(subjectCount));
            for (int j = 0; j < subjectCount; j++) {
                size_t subNameLen;
                file.read(reinterpret_cast<char*>(&subNameLen), sizeof(subNameLen));
                string subjectName(subNameLen, ' ');
                file.read(&subjectName[0], subNameLen);
                int score;
                file.read(reinterpret_cast<char*>(&score), sizeof(score));
                if (!info->addSubject(subjectName, score)) {
                    cout << "添加科目失败：" + subjectName;
                    file.close();
                    return false;
                }
            }
            if (!manager.addStudent(*info)) {
                cout << "添加学生失败：" + name;
                file.close();
                return false;
            }
        } else cout << name << " 不存在该学生！请联系管理员添加学生账户" << endl;
    }
    file.close();
    cout << "导入成功" << endl;
    return true;
}
    
bool FileOperation::loadText(StudentManage &manager) {
    cout << "===========文本文件加载============" << endl;
    cout << "支持文件格式：学生ID,姓名,班级,科目:成绩,科目:成绩...." << endl;
    cout << "请输入文件名：" << endl;
    string filename;
    cin >> filename;
    cout << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文本文件：" + filename;
        return false;
    }
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
            cout << "第" + to_string(lineNum) + "行格式错误：数据字段不足";
            file.close();
            return false;
        }
        try {
            string id = tokens[0];
            Information* info = manager.getStudentById(id);
            if (info) {
                for (size_t i = 3; i < tokens.size(); i++) {
                    string token = tokens[i];
                    token.erase(0, token.find_first_not_of(" \t"));
                    token.erase(token.find_last_not_of(" \t") + 1);
                    size_t colonPos = token.find(':');
                    if (colonPos != string::npos) {
                        string subjectName = token.substr(0, colonPos);
                        string scoreStr = token.substr(colonPos + 1);
                        int score = stoi(scoreStr);
                        if (!info->addSubject(subjectName, score)) {
                            cout << "第" + to_string(lineNum) + "行：添加科目失败-" + subjectName;
                            file.close();
                            return false;
                        }
                    }
                }
            } else cout << tokens[1] << "学生不存在！请联系管理员添加账户"  << endl;
        } catch (const exception& e) {
            cout << "第" + to_string(lineNum) + "行解析错误：" + string(e.what());
            file.close();
            return false;
        }
    }
    file.close();
    cout << "导入成功" << endl;
    return true;
}

