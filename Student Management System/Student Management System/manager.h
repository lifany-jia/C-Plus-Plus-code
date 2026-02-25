//
//  manager.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef MANAGER_H
#define MANAGER_H

#include "student_manage.h"
#include "auth.h"
#include <string>
#include <vector>

class ManagerUI {
private:
    StudentManage* mgr;
    Auth* auth;
    std::string currentAdminId;
    bool isLoggedIn;

    bool login();
    void mainMenu();
    void viewTasks();
    void fileMenu();  // 菜单
    void exportUsers();
    void importStudents();      
    void importTeachers();   //文件操作
    void loginAsTeacher();  // 登录教师端
    void authManager();   // 账户管理
public:
    ManagerUI(StudentManage* mgr, Auth* auth);
    void run();
};

#endif
