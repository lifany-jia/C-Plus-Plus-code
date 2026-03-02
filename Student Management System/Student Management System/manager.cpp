//
//  manager.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//

#include "manager.h"
#include "file_operation.h"
#include "teacher.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

ManagerUI::ManagerUI(StudentManage* mgr, Auth* auth)
    :mgr(mgr), auth(auth), isLoggedIn(auth->isLoggedInStatus()){
}

void ManagerUI::mainMenu() {
    while (isLoggedIn) {
        TeacherUI::clearScreen();
        cout << "===============================" << endl;
        cout << "         👩🏻‍💻管理员控制台" << endl;
        cout << "===============================" << endl;
        cout << "当前管理员：" << currentAdminId << endl;
        cout << "- 学生总数：" << mgr->getSize() << "人" << endl;
        cout << "- 用户总数：" << auth->getUserCount() << "人" << endl;
        cout << "===============================" << endl;
        cout << "1. 账户及学生信息管理" << endl;
        cout << "2. 登录教师端" << endl;
        cout << "3. 查看代办" << endl;
        cout << "4. 文件操作" << endl;
        cout << "5. 返回上一级" << endl;
        cout << "===============================" << endl;
        int choice;
        cout << "请选择：" ;
        cin >> choice;
        cout << endl;
        if (choice == 1){
            while (true) {
                TeacherUI::clearScreen();
                cout << "===============学生管理============" << endl;
                cout << "1. 学生信息管理" << endl;
                cout << "2. 用户账户管理" << endl;
                cout << "3. 返回上一级" << endl;
                int num;
                cout << "请选择：" ;
                cin >> num;
                cout << endl;
                if (num == 1) {
                    TeacherUI::studentManageMenu(mgr);
                } else if (num == 2) {
                    authManager();
                } else break;
            }
        } else if (choice == 2) {
            loginAsTeacher();
        } else if (choice == 3) {
            viewTasks();
        } else if (choice == 4) {
            fileMenu();
        } else if (choice == 5) {
            isLoggedIn = false;
            auth->logout();
            cout << "已退出登录" << endl;
            TeacherUI::waitForEnter();
        } else {
            cout << "❌ 无效选择" << endl;
            TeacherUI::waitForEnter();
        }
    }
}

void ManagerUI::viewTasks() {
    while (isLoggedIn) {
        TeacherUI::clearScreen();
        cout << "============待办事项===========" << endl;
        int count = 0;
        for (auto& item : mgr->appeals) {
            if (item.reply.empty()) {
                cout << item.type << ":" << item.id << item.name <<"---"<< item.appeal << endl;
                count++;
            }
        }
        if (count == 0) {
            cout << "🎉暂无待办处理" << endl;
            TeacherUI::waitForEnter();
            return;
        }
        cout << "\n⚠️当前有" << count << "条待办！！" << endl;
        cout << "-------------------------------" << endl;
        cout << "1. 处理结束回复" << endl;
        cout << "2. 返回上一级" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        cout << endl;
        if (choice == 1) {
            string id;
            cout << "请输入你的回复账户ID：";
            cin >> id;
            for (auto& item : mgr->appeals) {
                if (item.id == id) {
                    cout << "请输入你的回复：";
                    cin >> item.reply;
                    cout << endl;
                } else {
                    cout << "账户输入错误" << endl;
                    TeacherUI::waitForEnter();
                    return;
                }
            }
        } else return;
    }
}

void ManagerUI::authManager() {
    while (isLoggedIn) {
        TeacherUI::clearScreen();
        cout << "============账户管理============" << endl;
        cout << "1. 新增账户" << endl;
        cout << "2. 删除账户" << endl;
        cout << "3. 修改账户密码" << endl;
        cout << "4. 返回上一级" << endl;
        cout << "------------------------------" << endl;
        int choice;
        cout << "请选择：" << endl;
        cin >> choice;
        cout << endl;
        string id, className, name, role;
        switch (choice) {
            case 1: {
                while (true) {
                    TeacherUI::clearScreen();
                    cout << "请输入账户ID(输入0退出）：" ;
                    cin >> id;
                    if (id == "0") break;
                    cout << "\n请选择账户角色对应数字（1.student/2.teacher/3.admin):" ;
                    cin >> role;
                    if (role == "1") role = "student";
                    else if (role == "2") role = "teacher";
                    else if (role == "3") role = "admin";
                    else {
                        cout << "❌无效选择" << endl;
                        continue;
                    }
                    cout << "\n请输入账户姓名：" ;
                    cin >> name;
                    cout << "\n请输入账户班级：" ;
                    cin >> className;
                    cout << "\n初始密码默认为123456";
                    User newUser(id, "123456", role, className, name);
                    auth->addUser(newUser);
                    if (role == "student"){
                        Information newStu(id, name, className);
                        if (mgr->addStudent(newStu)) {
                                cout << "✅ 学生信息添加成功" << endl;
                            } else {
                                cout << "❌ 学生信息添加失败（可能ID已存在）" << endl;
                            }
                    }
                    TeacherUI::waitForEnter();
                }
            }
                break;
            case 2:
                auth->deleteUser();
                break;
            case 3:
                auth->changePassword();
                TeacherUI::waitForEnter();
                break;
            case 4: return;
            default:
                cout << "❌无效选择" << endl;
                TeacherUI::waitForEnter();
                break;
        }
    }
}

void ManagerUI::fileMenu() {
    while (true) {
        TeacherUI::clearScreen();
        cout << "=============文件操作=============" << endl;
        cout << "1. 批量导入学生信息" << endl;
        cout << "2. 批量导入教师信息" << endl;
        cout << "3. 导出用户账户信息" << endl;
        cout << "4. 返回上一级" << endl;
        cout << "===============================" << endl;
        cout << "请选择：" << endl;
        int choice;
        cin >> choice;
        cout << endl;
        
        switch (choice) {
            case 1:
                importStudents();
                break;
            case 2:
                importTeachers();
                break;
            case 3:
                exportUsers();
                break;
            case 4: return;
            default:
                cout << "❌ 无效选择" << endl;
                TeacherUI::waitForEnter();
                break;
        }
    }
}

void ManagerUI::importStudents() {
    TeacherUI::clearScreen();
    cout << "=============批量导入学生=============" << endl;
    cout << "文件格式要求：CSV格式，每行：学号，姓名，班级" << endl;
    cout << "例如：2026001，张三，电路1班" << endl;
    cout << "------------------------------------" << endl;
    string filename;
    cout << "请输入文件名：" << endl;
    cin >> filename;
    cout << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "❌ 无法打开文件" << filename << endl;
        TeacherUI::waitForEnter();
        return;
    }
    string line;
    int lineNum = 0, success = 0, failed = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, className;
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, className, ',')) {
            try {
                Information newStu(id, name, className);
                if (mgr->addStudent(newStu)) {
                    User newUser(id, "123456", "student", className, name);
                    auth->addUser(newUser);
                    success++;
                } else failed++;
            } catch (...) {
                failed++;
            }
        } else failed++;
    }
    file.close();
    cout << "===========导入完成===========" << endl;
    cout << "总行数：" << lineNum << endl;
    cout << "成功：" << success << "条" << endl;
    cout << "失败：" << failed << "条" << endl;
    cout << "学生初始密码：123456" << endl;
    TeacherUI::waitForEnter();
}

void ManagerUI::importTeachers() {
    TeacherUI::clearScreen();
    cout << "=============批量导入教师=============" << endl;
    cout << "文件格式要求：CSV格式，每行：工号，姓名，管理班级" << endl;
    cout << "例如：2026001，张三，电路1班" << endl;
    cout << "------------------------------------" << endl;
    string filename;
    cout << "请输入文件名：" << endl;
    cin >> filename;
    cout << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "❌ 无法打开文件" << filename << endl;
        TeacherUI::waitForEnter();
        return;
    }
    string line;
    int lineNum = 0, success = 0, failed = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, name, className;
        if (getline(ss, idStr, ',') && getline(ss, name, ',') && getline(ss, className, ',')) {
            User newUser(idStr, "123456", "teacher", className, name);
            auth->addUser(newUser);
            success++;
        } else failed++;
    }
    file.close();
    cout << "===========导入完成===========" << endl;
    cout << "总行数：" << lineNum << endl;
    cout << "成功：" << success << "条" << endl;
    cout << "失败：" << failed << "条" << endl;
    cout << "教师账户初始密码：123456" << endl;
    TeacherUI::waitForEnter();
}

void ManagerUI::exportUsers() {
    TeacherUI::clearScreen();
    const auto& allUser = auth->getAllUsers();
    if (allUser.empty()) {
        cout << "❌ 没有用户数据可导出" << endl;
        TeacherUI::waitForEnter();
        return ;
    }
    string filename;
    cout << "请输入保存的文件名：" ;
    cin >> filename;
    cout << endl;
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "❌无法创建文件" << filename << endl;
        TeacherUI::waitForEnter();
        return;
    }
    file << "用户ID，姓名，班级，密码，角色" << endl;
    for (const auto& pair : allUser) {
        const User& user = pair.second;
        file << user.userId << "," << user.realName << "," << user.className << "," << user.password << "," << user.role << endl;
    }
    file.close();
    cout << "导出结束" << endl;
    TeacherUI::waitForEnter();
}

void ManagerUI::loginAsTeacher() {
    TeacherUI::clearScreen();
    cout << "==============以教师身份登录==============" << endl;
    const auto& allUsers = auth->getAllUsers();
    vector<pair<string, User>> teachers;
    for (const auto& pair : allUsers) {
        if (pair.second.role == "teacher") {
            teachers.push_back(pair);
        }
    }
    if (teachers.empty()) {
        cout << "❌暂无教师账号" << endl;
        TeacherUI::waitForEnter();
        return;
    }
    cout << "请选择要登录的教师账号：" << endl;
    cout << "0. 返回" << endl;
    for (int i = 0; i < teachers.size(); i++) {
        const User& teacher = teachers[i].second;
        cout << i + 1 << "." << teacher.userId << "-" << teacher.className << endl;
    }
    int choice;
    cin >> choice;
    cout << endl;
    if (choice == 0) {
        return;
    }
    const User& teacher = teachers[choice - 1].second;
    string teacherId = teachers[choice - 1].first;
    cout << "正在以" << teacherId << "老师身份登录..." << endl;
    TeacherUI teacherUI(mgr, auth, teacherId);
    teacherUI.run();
}

bool ManagerUI::login() {
    while (!isLoggedIn) {
        TeacherUI::clearScreen();
        cout << "==========管理员登录=========" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 返回" << endl;
        cout << "----------------------------" << endl;
        cout << "请选择：" << endl;
        int choice;
        cin >> choice;
        cout <<endl;
        if (choice == 1) {
            if (auth->login()) {
                if (auth->getCurrentRole() == "admin") {
                    currentAdminId = auth->getCurrentUserId();
                    isLoggedIn = true;
                    cout << "🎉欢迎管理员" << currentAdminId << "!" << endl;
                    TeacherUI::waitForEnter();
                    return true;
                } else {
                    cout << "❌ 这不是管理员账户" << endl;
                    auth->logout();
                    TeacherUI::waitForEnter();
                }
            }
        } else if (choice == 2) {
            return false;
        }
    }
    return false;
}
void ManagerUI::run() {
    while (true) {
        if (!isLoggedIn) {
            bool loginSuccess = login();
            if (!loginSuccess) {
                break;
            }
        } else {
            mainMenu();
        }
    }
}
