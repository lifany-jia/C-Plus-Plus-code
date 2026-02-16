//
//  student.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#include "student.h"
#include "file_operation.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <map>
#include <numeric>
#include <cstdlib>

using namespace std;

StudentUI::StudentUI(StudentManage* mgr)
    :manager(mgr), isLoggedIn(false){
}

void StudentUI::clearScreen() {
    system("clear");
}

void StudentUI::waitForEnter() {
    cout << "\n按回车键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

Information* StudentUI::getCurrentStudents() {
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getId() == currentStudentId) {
            return &curr->data;
        }
        curr = curr->next;
    }
    return nullptr;
}

vector<Information> StudentUI::getClassStudents() {
    vector<Information> result;
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getClassName() == currentClass) {
            result.push_back(curr->data);
        }
        curr = curr->next;
    }
    return result;
}

bool StudentUI::login() {
    clearScreen();
    cout << "============================" << endl;
    cout << "         学生登录系统         " << endl;
    cout << "============================" << endl;
    cout << "      请输入您的学号和密码      " << endl;
    cout << "============================" << endl;
    
    int id;
    string password;
    cout << "学号：";
    cin >> id;
    cout << "密码：";
    cin >> password;
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getId() == id)
    }
}
