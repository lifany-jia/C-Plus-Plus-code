//
//  student_manage.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef STUDENT_MANAGE_H
#define STUDENT_MANAGE_H

#include "information.h"
#include <string>
#include <vector>

class StudentNode {
public:
    Information data;
    StudentNode* prev;
    StudentNode* next;
    
    StudentNode(const Information& info);
};
struct Appeal {
    int id;
    std::string name;
    std::string type;
    std::string appeal;
    std::string reply;
    time_t createTime;
    Appeal(int id, std::string name, std::string type)
        :id(id), name(name), type(type) {
    }
};
class StudentManage {
private:
    StudentNode* head;
    StudentNode* tail;
    int size;
    
    StudentNode* findNodeById(int id) const;
    void swapNode(StudentNode* a, StudentNode* b);
public:
    std::vector<Appeal> appeals;
    StudentManage();
    ~StudentManage();
    
    bool isEmpty()const;
    void clear();
    int getSize()const;
    StudentNode* getHead() const;
    StudentNode* getTail() const;
    
    bool addStudent(const Information& info);
    bool removeStudentById(int id);      //  学生操作
    Information* getStudentById(int id);
    const Information* getStudentById(int id) const;  // 获取学生
    void sortByTotalScore(bool ascending = true);
    void sortByAverageScore(bool ascending = true);
    void sortBySubjectScore(const std::string& subjectName, bool ascending = true);   // 排序
    bool getStudentRank(int studentId, int& myRank) const;
    bool getStudentRank(int studentId, const std::string& subjectName, int& myRank) const;     // 排名
};
#endif
