//
//  teacher.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//

#include "teacher.h"
#include "file_operation.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

TeacherUI::TeacherUI(StudentManage* mgr, Auth* auth)
    :manager(mgr), auth(auth), isLoggedIn(auth->isLoggedInStatus()) {
}

void TeacherUI::clearScreen() {
    system("clear");
}
void TeacherUI::waitForEnter() {
    cout << "\n按回车键继续..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool TeacherUI::isStudentInMyClass(int studentId) const {
    const Information* stu = manager->getStudentById(studentId);
    return stu && stu->getClassName() == currentClass;
}

Information* TeacherUI::findStudentById(int studentId) {
    if (!isStudentInMyClass(studentId)) return nullptr;
    return manager->getStudentById(studentId);
}

vector<Information> TeacherUI::getMyClassStudents() const {
    vector<Information> students;
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getClassName() == currentClass) {
            students.push_back(curr->data);
        }
        curr = curr->next;
    }
    return students;
}

string TeacherUI::getRankString(int studentId) const {
    int rank = 0;
    if (manager->getStudentRank(studentId, rank)) {
        return "第" + to_string(rank) + "名";
    }
    return "无数据";
}

string TeacherUI::getRankString(int studentId, const string& subject) const {
    int rank = 0;
    if (manager->getStudentRank(studentId, subject, rank)) {
        return "第" + to_string(rank) + "名";
    }
    return "无数据";
}

void TeacherUI::printStudentSimple(const Information &stu) {
    int id = stu.getId();
    string strRank = getRankString(id);
    cout << left << setw(10) << stu.getId() << setw(15) << stu.getClassName() << setw(15) << stu.getName() << setw(10) << stu.getTotalScore() << "分" << fixed << setprecision(2) << setw(10) << stu.getAverageScore() << "分" << setw(10) << strRank << endl;
}

void TeacherUI::printStudentDetail(const Information &stu) {
    int id = stu.getId();
    string strRank = getRankString(id);
    cout << "==============================" << endl;
    cout << "学号：" << stu.getId() << endl;
    cout << "姓名：" << stu.getName() << endl;
    cout << "班级：" << stu.getClassName() << endl;
    cout << "名次：" << strRank << endl;
    cout << "------------------------------" << endl;
    const Subject* sub = stu.getSubjectHead();
    if (!sub) {
        cout << "暂无学生数据" << endl;
        return;
    }
    cout << left << setw(15) << "科目" << setw(10) << "成绩" << setw(10) << "排名" << endl;
    cout << "------------------------------" << endl;
    while (sub) {
        string strSubRank = getRankString(id, sub->name);
        cout << left << setw(15) << sub->name << setw(10) << sub->score << setw(10) << strSubRank << endl;
        sub = sub->next;
    }
    cout << "-------------------------------" << endl;
    cout << "总分：" << stu.getTotalScore() << "分" << endl;
    cout << "平均分：" << fixed << setprecision(2) << stu.getAverageScore() << "分" << endl;
}

void TeacherUI::printPageStudents(StudentNode *start, int pageSize, int currentPage, int totalPage) {
    cout << "\n==============================" << endl;
    cout << "  第" << currentPage << "页/共" << totalPage << "页" << endl;
    cout << "==============================" << endl;
    cout << left << setw(10) << "学号" << setw(15) << "班级" << setw(15) << "姓名" << setw(10) << "总分" << setw(10) << "平均分" << endl;
    cout << "------------------------------" << endl;
    
    StudentNode* curr = start;
    for (int i = 0; i < pageSize && curr; i++) {
        printStudentSimple(curr->data);
        curr = curr->next;
    }
    cout << "------------------------------" << endl;
    cout << "n：下一页  p：上一页 q：退出翻页 输入页码直接跳转⏭️：";
}

bool TeacherUI::login() {
    while (!isLoggedIn) {
        clearScreen();
        cout << "==============================" << endl;
        cout << "         🧑‍🏫教师登录系统" << endl;
        cout << "==============================" << endl;
        cout << "1. 注册" << endl;
        cout << "2. 登录" << endl;
        cout << "3. 退出" << endl;
        cout << "==============================" << endl;
        cout << "请输入您的指令：";
        int choice;
        cin >> choice;
        cout << endl;
        if (choice == 1) {
            if (auth->registerUser()) {
                cout << "✅用户注册成功！请登录" << endl;
                waitForEnter();
            } else if (choice == 2) {
                if (auth->login()) {
                    if (auth->getCurrentRole() == "teacher") {
                        currentTeacherId = auth->getCurrentUserId();
                        cout << "请输入您管理的班级名称：";
                        cin >> currentClass;
                        cout << endl;
                        currentTeacherName = "教师";
                        isLoggedIn = true;
                        cout << "🎉欢迎" << currentTeacherId << "老师！" << endl;
                        waitForEnter();
                        return true;
                    } else {
                        cout << "❌ 错误：这不是教师账户" << endl;
                        auth->logout();
                        waitForEnter();
                    }
                }
            }
        } else if (choice == 3) {
            return false;
        } else {
            cout << "❌ 请输入正确指令" << endl;
            waitForEnter();
        }
    }
    return false;
}
