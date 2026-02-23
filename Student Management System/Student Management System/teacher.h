//
//  teacher.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef TEACHER_H
#define TEACHER_H

#include "student_manage.h"
#include "auth.h"
#include <string>
#include <vector>

class TeacherUI {
private:
    StudentManage* manager;
    Auth* auth;
    std::string currentTeacherId;
    std::string currentTeacherName;
    std::string currentClass;
    bool isLoggedIn;
    
    bool login();
    
    void showMainMenu();
    void studentManageMenu();
    void scoreViewMenu();
    void fileOperationMenu();
    
    void addStudent();
    void deleteStudent();
    void modifyStudent();
    void queryStudent();
    void listClassStudents();
    
    void addStudentScore();
    void deleteStudentScore();
    void modifyStudentScore();
    void viewStudentScore();
    void viewClassScore();
    
    void downloadStudentInfo();
    
    std::string getRankString(int studentId) const;
    std::string getRankString(int studentId, const std::string& subject) const;
    
    bool isStudentInMyClass(int studentId) const;
    Information* findStudentById(int studentId);
    std::vector<Information> getMyClassStudents() const;
    void printStudentSimple(const Information& stu);
    void printStudentDetail(const Information& stu);
    void printPageStudents(StudentNode* start, int pageSize, int currentPage, int totalPage);
    
    void waitForEnter();
    void clearScreen();
    
public:
    TeacherUI(StudentManage* mgr, Auth* auth);
    void run();
};

#endif


