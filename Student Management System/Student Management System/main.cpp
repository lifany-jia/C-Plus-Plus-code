//
//  main.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/17.
//
#include <iostream>
#include "auth.h"
#include "student.h"
#include "teacher.h"
#include "manager.h"
#include "student_manage.h"
using namespace std;

int main() {
    Auth auth;
    StudentManage manager;
    while (true) {
        TeacherUI::clearScreen();
        cout << "=========================" << endl;
        cout << "       学生管理系统" << endl;
        cout << "=========================" << endl;
        cout << "1. 学生" << endl;
        cout << "2. 教师" << endl;
        cout << "3. 管理员" << endl;
        cout << "4. 退出系统" << endl;
        cout << "---------------------------" << endl;
        cout << "请选择：" << endl;
        string choice;
        cin >> choice;
        cout << endl;
        if (choice == "1") {
            StudentUI studentUI(&manager, &auth);
            studentUI.run();
        } else if (choice == "2") {
            TeacherUI teacherUI(&manager, &auth);
            teacherUI.run();
        } else if (choice == "3") {
            ManagerUI managerUI(&manager, &auth);
            managerUI.run();
        } else if (choice == "4") {
            cout << "感谢使用学生管理系统！再见👋" << endl;
            TeacherUI::waitForEnter();
            return 0;
        } else {
            cout << "❌无效输入" << endl;
            TeacherUI::waitForEnter();
        }
    }
    return 0;
}
