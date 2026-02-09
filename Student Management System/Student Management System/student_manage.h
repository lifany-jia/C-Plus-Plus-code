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

class StudentNode {
public:
    Information data;
    StudentNode* prev;
    StudentNode* next;
    
    StudentNode(const Information& info);
};
class StudentManage {
private:
    StudentNode* head;
    StudentNode* tail;
    int size;
    
    StudentNode* findNodeById(int id) const;
    void swapNode(StudentNode* a, StudentNode* b);
public:
    StudentManage();
    ~StudentManage();
    
    int getSize()const;
    bool isEmpty()const;
    void clear();
    
    bool addStudent(const Information& info);
    bool removeStudentById(int id);
    
    Information* getStudentById(int id);
    const Information* getStudentById(int id) const;
    
    bool addSubjectToStudent(int id, const std::string& subjectName, int score);
    bool updateStudentSubjectScore(int id, const std::string& subjectName, int score);
    bool removeStudentSubjectScore(int id, const std::string& subjectName);
    
    void sortByTotalScore(bool ascending = true);
    void sortByAverageScore(bool ascending = true);
    void sortBySubjectScore(const std::string& subjectName, bool ascending = true);
    
    int getTotalPages(int pageSize) const;
    StudentNode* getPageStart(int pageIndex, int pageSize) const;
    
    StudentNode* getHead() const;
    StudentNode* getTail() const;
};
#endif
