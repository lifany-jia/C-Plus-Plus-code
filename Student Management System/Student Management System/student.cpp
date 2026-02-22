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

StudentUI::StudentUI(StudentManage* mgr, Auth* auth)
    :manager(mgr), auth(auth), isLoggedIn(auth->isLoggedInStatus()){
}

void StudentUI::clearScreen() {
    system("clear");
}

void StudentUI::waitForEnter() {
    cout << "\n按回车键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

Information* StudentUI::getCurrentStudent() {
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
    while (!isLoggedIn) {
        clearScreen();
        cout << "============================" << endl;
        cout << "         学生登录系统         " << endl;
        cout << "============================" << endl;
        cout << "1. 注册" << endl;
        cout << "2. 登录" << endl;
        cout << "3. 退出" << endl;
        cout << "============================" << endl;
        cout << "请输入您的指令：" ;
        int num;
        cin >> num;
        cout << endl;
        if (num == 1) {
            if (auth->registerUser()) {
                cout << "✅ 用户注册成功！请登录" << endl;
                waitForEnter();
            }
        } else if (num == 2) {
            if (auth->login()) {
                if (auth->getCurrentRole() == "student") {
                    string userId = auth->getCurrentUserId();
                    StudentNode* curr = manager->getHead();
                    bool found = false;
                    while (curr) {
                        if (to_string(curr->data.getId()) == userId) {
                            currentClass = curr->data.getClassName();
                            currentStudentId = curr->data.getId();
                            currentStudentName = curr->data.getName();
                            found = true;
                            isLoggedIn = true;
                            cout << "🎉欢迎" << currentStudentName << "同学！" << endl;
                            cout << "班级：" << currentClass << endl;
                            waitForEnter();
                            return true;
                        }
                        curr = curr->next;
                    }
                    if (found == false) {
                        cout << "❌ 错误：学号存在但学生信息不存在" << endl;
                        cout << "请联系统管理员" << endl;
                        auth->logout();
                    }
                } else {
                    cout << "❌ 错误：这不是学生账户" << endl;
                    cout << "请联系系统管理员" << endl;
                    auth->logout();
                }
            }
        } else if (num == 3) {
            cout << "已退出登录" << endl;
            waitForEnter();
            return false;
        } else {
            cout << "❌ 请输入正确指令！" << endl;
            waitForEnter();
        }
    }
    return false;
}

void StudentUI::printStudentInfo(const Information &Student) {
    cout << "============================" << endl;
    cout << "学号：" << Student.getId() << endl;
    cout << "姓名：" << Student.getName() << endl;
    cout << "班级：" << Student.getClassName() << endl;
    
    const Subject* sub = Student.getSubjectHead();
    int totalScore = 0;
    int subjectCount = 0;
    while (sub) {
        cout << left << setw(15) << sub->name << "：" << sub->score << "分" << endl;
        totalScore += sub->score;
        subjectCount++;
        sub = sub->next;
    }
    if (subjectCount > 0) {
        cout << "-----------------------------" << endl;
        cout << "总分：" << totalScore << "分" << endl;
        cout << "平均分" << fixed << setprecision(2) << (double)totalScore / subjectCount << "分" << endl;
    }
}

//显示条状图
void StudentUI::showScoreBarChart(const std::vector<int> &scores, const std::string &title) {
    if (scores.empty()) {
        return;
    }
    map<string, int> distribution;
    distribution["90 - 100"] = 0;
    distribution["80 - 89"] = 0;
    distribution["70 - 79"] = 0;
    distribution["60 - 69"] = 0;
    distribution["0 - 59"] = 0;
    
    int maxScore = *max_element(scores.begin(), scores.end());
    int minScore = *min_element(scores.begin(), scores.end());
    
    double avgScore = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
    for (int score : scores) {
        if (score >= 90) distribution["90 - 100"]++;
        else if (score >= 80) distribution["80 - 89"]++;
        else if (score >= 70) distribution["70 - 79"]++;
        else if (score >= 60) distribution["60 - 69"]++;
        else distribution["0 - 59"]++;
    }
    cout << "\n" << title << endl;
    cout << "============================" << endl;
    cout << "最高分：" << maxScore << "分" << endl;
    cout << "最低分：" << minScore << "分" << endl;
    cout << "平均分：" << fixed << setprecision(2) << avgScore << "分" << endl;
    cout << "============================" << endl;
    
    int maxCount = 0;
    for (const auto& pair : distribution) {
        if (pair.second > maxCount) maxCount = pair.second;
    }
    if (maxCount == 0) {
        cout << "暂无成绩数据📊" << endl;
        return;
    }
    cout << "\n      成绩分布图📊" << endl;
    cout << "============================" << endl;
    double scale = (maxCount > 20) ? 20.0 / maxCount : 1.0;
    for (const auto& pair : distribution) {
        cout << left << setw(8) << pair.first << "|";
        int starCount = static_cast<int>(pair.second * scale);
        cout << left << setw(20) << string(starCount, '*');
        cout << right << setw(4) << pair.second << "人" << endl;
    }
}

void StudentUI::queryMyScores() {
    clearScreen();
    cout << "============================" << endl;
    cout << "          我的成绩单          " << endl;
    cout << "============================" << endl;
    Information* curr = getCurrentStudent();
    if (!curr) {
        cout << "错误：无法获取您的信息📑" << endl;
        waitForEnter();
        return;
    }
    cout << "学号：" << curr->getId() << endl;
    cout << "姓名：" << curr->getName() << endl;
    cout << "班级：" << curr->getClassName() << endl;
    cout << "============================" << endl;
    const Subject* sub = curr->getSubjectHead();
    if (!sub) {
        cout << "暂无成绩数据" << endl;
        waitForEnter();
        return;
    }
    vector<Information> classStudents = getClassStudents();
    int totalScore = 0;
    int subjectCount = 0;
    int maxScore = 0;
    int minScore = 100;
    string bestSubject, worstSubject;
    cout << left << setw(20) << "科目" << setw(10) << "成绩" << setw(10) << "评级" << setw(10) << "班级排名" << endl;
    cout << "-----------------------------" << endl;
    while (sub) {
        totalScore += sub->score;
        subjectCount++;
        if (sub->score > maxScore) {
            maxScore = sub->score;
            bestSubject = sub->name;
        }
        if (sub->score < minScore) {
            minScore = sub->score;
            worstSubject = sub->name;
        }
        string grade;
        int rank = 0;
        string strRank;
        if (manager->getStudentRank(currentStudentId, sub->name, rank)) {
            strRank = "第" + to_string(rank) + "名";
        } else {
            strRank = "无数据";
        }
        if (sub->score >= 90) grade = "优秀";
        else if (sub->score >= 80) grade = "良好";
        else if (sub->score >= 70) grade = "中等";
        else if (sub->score >= 60) grade = "及格";
        else  grade = "不及格";
        
        cout << left << setw(20) << sub->name << setw(10) << sub->score << setw(10) << grade  << setw(10) << strRank << endl;
        sub = sub->next;
    }
    cout << "-----------------------------" << endl;
    double avgScore = curr->getAverageScore();
    int classRank = 0;
    string strClassRank;
    if (manager->getStudentRank(currentStudentId, classRank)) {
        strClassRank = "第" + to_string(classRank) + "名";
    } else {
        strClassRank = "无数据";
    }
    cout << "总分：" << totalScore << "分" << endl;
    cout << "平均分：" << fixed << setprecision(2) << avgScore << "分" << endl;
    cout << "最高分科目：" << bestSubject << "（" << maxScore << "分）"<< endl;
    cout << "最低分科目：" << worstSubject << "（" << minScore << "分）"<< endl;
    cout << "班级排名：" << strClassRank << "（共" << manager->getSize() << "人）"<< endl;
    waitForEnter();
}

void StudentUI::queryClassScores() {
    clearScreen();
    cout << "============================" << endl;
    cout << "     " << currentClass << "成绩单"  << endl;
    cout << "============================" << endl;
    vector<Information> classStudents = getClassStudents();
    if (classStudents.empty()) {
        cout << "本班暂无学生数据" << endl;
        waitForEnter();
        return ;
    }
    sort(classStudents.begin(), classStudents.end(),
         [](const Information& a, const Information& b) {
        return a.getId() < b.getId();
    });
    cout << left << setw(10) << "学号" << setw(10) << "姓名" << setw(10) << "总分" << setw(10) << "平均分" << endl;
    cout << "-----------------------------" << endl;
    for (const auto& student : classStudents) {
        cout << left << setw(10) << student.getId() << setw(10) << student.getName() << student.getTotalScore() << setw(10) << student.getAverageScore() << endl;
    }
}

void StudentUI::showClassRanking() {
    while (true) {
        clearScreen();
        cout << "============================" << endl;
        cout << "         班级排名系统" << endl;
        cout << "============================" << endl;
        cout << "1. 总分排名" << endl;
        cout << "2. 单科排名" << endl;
        cout << "3. 返回🔙" << endl;
        cout << "============================" << endl;
        cout << "请输入指令：";
        int choice;
        cin >> choice;
        cout << endl;
        StudentNode* stu = manager->getHead();
        if (!stu) {
            cout << "本班暂无学生数据" << endl;
            waitForEnter();
            return;
        }
        if (choice == 1) {
            vector<pair<int, int>> students;
            while (stu) {
                int id = stu->data.getId();
                int rank = 0;
                if (manager->getStudentRank(id, rank)) {
                    students.push_back({id, rank});
                }
                stu = stu->next;
            }
            sort(students.begin(), students.end(),
                 [](const auto& a, const auto& b) {
                return a.second < b.second;
            });
            clearScreen();
            cout << "============================" << endl;
            cout << "           总分排名" << endl;
            cout << "============================" << endl;
            cout << left << setw(6) << "名次" << setw(10) << "学号" << setw(10) << "姓名" << setw(10) << "总分" << endl;
            cout << "----------------------------" << endl;
            
            for (const auto& item : students) {
                const Information* curr = manager->getStudentById(item.first);
                cout << left << setw(6) << item.second << setw(10) << item.first << setw(10) << curr->getName() << setw(10) << curr->getTotalScore() << endl;
            }
            waitForEnter();
            cout << "是否以条状图呈现（y/n）：" ;
            char ch;
            cin >> ch;
            cout << endl;
            if (ch == 'y' || ch == 'Y') {
                vector<int> scores;
                stu = manager->getHead();
                while (stu) {
                    scores.push_back(stu->data.getTotalScore());
                    stu = stu->next;
                }
                showScoreBarChart(scores, "总分分布图");
            }
            waitForEnter();
        } else if (choice == 2) {
            map<string, bool> allSubjects;
            while (stu) {
                Subject* curr = stu->data.getSubjectHead();
                while (curr) {
                    allSubjects[curr->name] = true;
                    curr = curr->next;
                }
                stu = stu->next;
            }
            if (allSubjects.empty()) {
                cout << "暂无科目数据" << endl;
                waitForEnter();
                return;
            }
            while (true) {
                clearScreen();
                vector<string> subjectList;
                int index = 1;
                cout << "0.退出" << endl;
                for (const auto& item : allSubjects) {
                    cout << index++ << "." << item.first << endl;
                    subjectList.push_back(item.first);
                }
                cout << "请选择（0-" << subjectList.size() << "）" ;
                int subChoice;
                cin >> subChoice;
                cout << endl;
                if (subChoice >= 1 && subChoice <= subjectList.size()) {
                    string selectSubject = subjectList[subChoice - 1];
                    vector<pair<int, int>> subjectScores;
                    stu = manager->getHead();
                    while (stu) {
                        int subRank = 0;
                        int id = stu->data.getId();
                        if (manager->getStudentRank(id, selectSubject, subRank)) {
                            subjectScores.push_back({id, subRank});
                        }
                        stu = stu->next;
                    }
                    sort(subjectScores.begin(), subjectScores.end(),
                         [](const auto& a, const auto& b) {
                        return a.second < b.second;
                    });
                    clearScreen();
                    cout << "============================" << endl;
                    cout << "       " << selectSubject << "排名" << endl;
                    cout << "============================" << endl;
                    cout << left << setw(6) << "名次" << setw(10) << "学号" << setw(10) << "姓名" << setw(10) << "成绩" << endl;
                    cout << "----------------------------" << endl;
                    for (auto& item : subjectScores) {
                        int score;
                        const Information* curr = manager->getStudentById(item.first);
                        curr->getSubjectScore(selectSubject, score);
                        cout << left << setw(6) << item.second << setw(10) << item.first << setw(10) << curr->getName() << setw(10) << score << endl;
                        
                    }
                    waitForEnter();
                    cout << "是否以条状图呈现（y/n）：" ;
                    char ch;
                    cin >> ch;
                    cout << endl;
                    if (ch == 'y' || ch == 'Y') {
                        vector<int> scores;
                        int subScore;
                        stu = manager->getHead();
                        while (stu) {
                            stu->data.getSubjectScore(selectSubject, subScore);
                            scores.push_back(subScore);
                            stu = stu->next;
                        }
                        showScoreBarChart(scores, selectSubject + "分布图");
                    }
                    waitForEnter();
                } else if (subChoice == 0) {
                    break;
                } else {
                    cout << "❌ 输入错误" << endl;
                }
            }
        } else {
            return;
        }
        waitForEnter();
    }
}

void StudentUI::queryMenu() {
    while (true) {
        clearScreen();
        cout << "============================" << endl;
        cout << "          📖成绩查询菜单" << endl;
        cout << "============================" << endl;
        cout << "1. 查询我的成绩" << endl;
        cout << "2. 查询本班成绩" << endl;
        cout << "3. 返回上一级" << endl;
        cout << "============================" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                queryMyScores();
                break;
            case 2:
                queryClassScores();
            case 3:
                return;
            default:
                cout << "无效选择，请重新输入！" << endl;
                waitForEnter();
        }
    }
}

void StudentUI::analysisMenu() {
    while (true) {
        clearScreen();
        cout << "============================" << endl;
        cout << "         🧐成绩分析菜单" << endl;
        cout << "============================" << endl;
        cout << "1. 班级排名" << endl;
        cout << "2. 返回上一级" << endl;
        cout << "============================" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                showClassRanking();
                break;
            case 2:
                return;
            default:
                cout << "无效选择，请重新输入！" << endl;
                waitForEnter();
        }
    }
}

void StudentUI::showMainMenu() {
    while (isLoggedIn) {
        clearScreen();
        cout << "============================" << endl;
        cout << "       🧑‍🎓学生成绩查询系统" << endl;
        cout << "============================" << endl;
        cout << "当前用户：" << currentStudentName << endl;
        cout << "班   级：" << currentClass << endl;
        cout << "============================" << endl;
        cout << "1. 成绩查询" << endl;
        cout << "2. 成绩分析" << endl;
        cout << "3. 修改密码" << endl;
        cout << "4. 退出登录" << endl;
        cout << "请选择（1-4）：";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                queryMenu();
                break;
            case 2:
                analysisMenu();
                break;
            case 3:
                auth->changePassword();
                break;
            case 4:
                isLoggedIn = false;
                auth->logout();
                cout << "已退出登录！" << endl;
                waitForEnter();
            default:
                cout << "无效选择，请重新输入！" << endl;
                waitForEnter();
        }
    }
}

void StudentUI::run() {
    while (true) {
        if (!isLoggedIn) {
            bool loginSuccess = login();
            if (!loginSuccess) {
                cout << "返回主菜单..." << endl;
                waitForEnter();
                break;
            }
        } else {
            showMainMenu();
        }
    }
}
