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
    int currentTeacherId;
    std::string currentTeacherName;
    std::string currentClass;
    bool isLoggedIn;
    bool isAdminMode;
    
    bool login();
    void showMainMenu();// 菜单
    void listClassStudents();  // 学生操作
    void addAppeal();
    std::vector<Information> getMyClassStudents() const;
public:
    static void studentManageMenu(StudentManage* manager);
    static void addStudent(StudentManage* manager);
    static void modifyStudent(StudentManage* manager);
    static void printStudentDetail(StudentManage* manager, const Information& stu);
    static std::string getRankString(int studentId, StudentManage* manager) ;
    static std::string getRankString(int studentId, StudentManage* manager, const std::string& subject);
    static void waitForEnter();
    static void clearScreen();
    TeacherUI(StudentManage* mgr, Auth* auth);
    TeacherUI(StudentManage* mgr, Auth* auth, std::string teacherId);  // 管理员模式
    void run();
};

#endif


