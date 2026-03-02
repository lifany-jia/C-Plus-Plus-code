//
//  student.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef STUDENT_H
#define STUDENT_H
#include "student_manage.h"
#include "auth.h"
#include <string>
#include <vector>

class StudentUI {
private:
    StudentManage* manager;
    Auth* auth;
    std::string currentStudentId;
    std::string currentClass;
    std::string currentStudentName;
    bool isLoggedIn;
    
    bool login();
    void showMainMenu(); //菜单
    void queryMyScores();
    void queryClassScores();  // 查询
    void showClassRanking();   // 排名
    void addAppeal();
    void showScoreBarChart(const std::vector<int>& scores, const std::string& title);  // 条状图
    Information* getCurrentStudent();
    void waitForEnter();
    void clearScreen();
public:
    StudentUI(StudentManage*mgr, Auth* auth);
    void run();
};
#endif
