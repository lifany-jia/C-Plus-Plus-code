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
    int currentStudentId;
    std::string currentClass;
    std::string currentStudentName;
    bool isLoggedIn;
    
    bool login();
    void showMainMenu();//菜单
    void queryMenu();
    void analysisMenu();
    
    void queryMyScores(); //功能
    void queryClassScores();
    void showClassRanking();
    void showScoreBarChart(const std::vector<int>& scores, const std::string& title);
    
    Information* getCurrentStudent(); // 辅助
    std::vector<Information> getClassStudents();
    void printStudentInfo(const Information& Student);
    void waitForEnter();
    void clearScreen();
    
public:
    StudentUI(StudentManage*mgr);
    void run();
};
#endif
