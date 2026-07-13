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
vector<const Information*> StudentUI::getMyClass() {
    vector<const Information*> myClass;
    StudentNode* curr = manager->getHead();
    if (!curr) {
        cout << "manager是空指针！" <<endl;
        return myClass;
    }
    while (curr) {
        if (curr->data.getClassName() == currentClass) {
            myClass.push_back(&curr->data);
        }
        curr = curr->next;
    }
    return myClass;
}

void StudentUI::showMainMenu() {
    while (isLoggedIn) {
        clearScreen();
        cout << "============================" << endl;
        cout << "     🧑‍🎓学生成绩查询系统" << endl;
        cout << "============================" << endl;
        cout << "当前用户：" << currentStudentName << endl;
        cout << "班   级：" << currentClass << endl;
        cout << "============================" << endl;
        cout << "1. 成绩查询" << endl;
        cout << "2. 查询本班成绩" << endl;
        cout << "3. 成绩分析" << endl;
        cout << "4. 修改密码" << endl;
        cout << "5. 联系管理员" << endl;
        cout << "6. 返回上一级" << endl;
        cout << "============================" << endl;
        cout << "请选择：";
        string choice;
        cin >> choice;
        if (choice == "1") {
            queryMyScores();
        } else if (choice == "2") {
            queryClassScores();
        } else if (choice == "3") {
            showClassRanking();
        } else if (choice == "4") {
            auth->changePassword();
        } else if (choice == "5") {
            addAppeal();
        } else if (choice == "6") {
            isLoggedIn = false;
            auth->logout();
            cout << "已退出登录！" << endl;
            waitForEnter();
            return;
        } else {
            cout << "❌无效选择" << endl;
            waitForEnter();
        }
    }
}
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
    
    int maxCount = 0;
    for (const auto& pair : distribution) {
        if (pair.second > maxCount) maxCount = pair.second;
    }
    if (maxCount == 0) {
        cout << "暂无成绩数据📊" << endl;
        return;
    }
    cout << "\n" << title << endl;
    cout << "============================" << endl;
    cout << "最高分：" << maxScore << "分" << endl;
    cout << "最低分：" << minScore << "分" << endl;
    cout << "平均分：" << fixed << setprecision(2) << avgScore << "分" << endl;
    cout << "============================" << endl;
    double scale = (maxCount > 20) ? 20.0 / maxCount : 1.0;
    for (const auto& pair : distribution) {
        cout << left << setw(8) << pair.first << "|";
        int starCount = static_cast<int>(pair.second * scale);
        cout << left << setw(20) << string(starCount, '*');
        cout << right << setw(4) << pair.second << "人" << endl;
    }
    waitForEnter();
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
    int totalScore = curr->getTotalScore();
    int subjectCount = curr->getScoreCount();
    string bestSubject, worstSubject;
    cout << left << setw(25) << "科目" << setw(12) << "成绩" << setw(12) << "评级" << setw(15) << "班级排名" << endl;
    cout << "-----------------------------" << endl;
    while (sub) {
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
        
        cout << left << setw(25) << sub->name << setw(12) << sub->score << setw(12) << grade  << setw(15) << strRank << endl;
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
    cout << "班级排名：" << strClassRank << "（共" << manager->getSize() << "人）"<< endl;
    waitForEnter();
}

void StudentUI::queryClassScores() {
    clearScreen();
    cout << "===================================" << endl;
    cout << "         " << currentClass << "成绩单"  << endl;
    cout << "===================================" << endl;
    vector<const Information*> classStudents = getMyClass();
    if (classStudents.empty()) {
        cout << "本班暂无学生数据" << endl;
        waitForEnter();
        return ;
    }
    sort(classStudents.begin(), classStudents.end(),
         [](const Information* a, const Information* b) {
        return a->getId() < b->getId();
    });
    cout << left << setw(20) << "学号" << setw(10) << "姓名" << setw(10) << "总分" << setw(10) << "平均分" << endl;
    cout << "-----------------------------------" << endl;
    for (const auto& student : classStudents) {
        cout << left << setw(15) << student->getId() << setw(10) << student->getName() << setw(10) << student->getTotalScore() << setw(10) << student->getAverageScore() << endl;
    }
    waitForEnter();
}

void StudentUI::showClassRanking() {
    while (true) {
        clearScreen();
        cout << "===============排名系统=============" << endl;
        cout << "1. 总分排名" << endl;
        cout << "2. 单科排名" << endl;
        cout << "3. 总分分布图 " << endl;
        cout << "4. 返回🔙" << endl;
        cout << "===================================" << endl;
        cout << "请输入指令：";
        string choice;
        cin >> choice;
        cout << endl;
        vector<const Information*> students = getMyClass();
        if (students.empty()) {
            cout << "本班暂无学生数据" << endl;
            waitForEnter();
            return;
        }
        if (choice == "1") {
            sort(students.begin(), students.end(),
                 [](const Information* a, const Information* b) {
                return a->getTotalScore() > b->getTotalScore();
            });
            clearScreen();
            cout << "==================================" << endl;
            cout << "              总分排名" << endl;
            cout << "==================================" << endl;
            cout << left << setw(10) << "名次" << setw(15) << "学号" << setw(10) << "姓名" << setw(10) << "总分" << endl;
            cout << "----------------------------------" << endl;
            
            for (int i = 0; i < students.size(); i++) {
                cout << left << setw(6) << i + 1 << setw(15) << students[i]->getId() << setw(10) <<
                students[i]->getName() << setw(10) << students[i]->getTotalScore() << endl;
            }
            waitForEnter();
        } else if (choice == "2") {
            map<string, bool> allSubjects;
            StudentNode* stu = manager->getHead();
            while (stu) {
                if (stu->data.getClassName() == currentClass) {
                    Subject* curr = stu->data.getSubjectHead();
                    while (curr) {
                        allSubjects[curr->name] = true;
                        curr = curr->next;
                    }
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
                    sort(students.begin(), students.end(),
                         [selectSubject](const Information* a, const Information* b) {
                        return a->getSubjectScore(selectSubject) > b->getSubjectScore(selectSubject);
                    });
                    clearScreen();
                    cout << "============================" << endl;
                    cout << "       " << selectSubject << "排名" << endl;
                    cout << "============================" << endl;
                    cout << left << setw(6) << "名次" << setw(10) << "学号" << setw(10) << "姓名" << setw(10) << "成绩" << endl;
                    cout << "----------------------------" << endl;
                    for (int i = 0; i < students.size(); i++) {
                        int score;
                        cout << left << setw(6) << i + 1 << setw(10) << students[i]->getId() << setw(10) << students[i]->getName() << setw(10) << students[i]->getSubjectScore(selectSubject) << endl;
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
                            if (stu->data.getClassName() == currentClass) {
                                int subScore = stu->data.getSubjectScore(selectSubject);
                                scores.push_back(subScore);
                            }
                            stu = stu->next;
                        }
                        showScoreBarChart(scores, selectSubject + "分布图");
                    }
                } else if (subChoice == 0) {
                    break;
                } else {
                    cout << "❌ 输入错误" << endl;
                }
            }
        } else if (choice == "3") {
            vector<int> scores;
            StudentNode* stu = manager->getHead();
            while (stu) {
                if (stu->data.getClassName() == currentClass) {
                    scores.push_back(stu->data.getTotalScore());
                }
                stu = stu->next;
            }
            map<string, int> distribution;
            distribution["600+"] = 0;
            distribution["500 - 600"] = 0;
            distribution["400 - 500"] = 0;
            distribution["300 - 400"] = 0;
            distribution["0 - 300"] = 0;
            
            int maxScore = *max_element(scores.begin(), scores.end());
            int minScore = *min_element(scores.begin(), scores.end());
            
            double avgScore = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
            for (int score : scores) {
                if (score >= 600) distribution["600+"]++;
                else if (score >= 500) distribution["500 - 600"]++;
                else if (score >= 400) distribution["400 - 500"]++;
                else if (score >= 300) distribution["300 - 400"]++;
                else distribution["0 - 300"]++;
            }
            int maxCount = 0;
            for (const auto& pair : distribution) {
                if (pair.second > maxCount) maxCount = pair.second;
            }
            if (maxCount == 0) {
                cout << "暂无成绩数据📊" << endl;
                return;
            }
            cout << "\n" << "总分条形图📊" << endl;
            cout << "============================" << endl;
            cout << "最高分：" << maxScore << "分" << endl;
            cout << "最低分：" << minScore << "分" << endl;
            cout << "平均分：" << fixed << setprecision(2) << avgScore << "分" << endl;
            cout << "============================" << endl;
            double scale = (maxCount > 20) ? 20.0 / maxCount : 1.0;
            for (const auto& pair : distribution) {
                cout << left << setw(8) << pair.first << "|";
                int starCount = static_cast<int>(pair.second * scale);
                cout << left << setw(20) << string(starCount, '*');
                cout << right << setw(4) << pair.second << "人" << endl;
            }
            waitForEnter();
        } else {
            return;
        }
    }
}

void StudentUI::addAppeal() {
    while (isLoggedIn) {
        clearScreen();
        cout << "==========投递箱==========" << endl;
        cout << "1. 账户密码修改" << endl;
        cout << "2. 成绩申诉" << endl;
        cout << "3. 查看回复" << endl;
        cout << "4. 返回上一级" << endl;
        cout << "--------------------------" << endl;
        cout << "请选择：";
        string choice;
        cin >> choice;
        cout << endl;
        if (choice == "1") {
            Appeal appeal(currentStudentId, currentStudentName, "账户密码修改");
            cout << "请写出你的诉求，以及新密码:" << endl;
            cin >> appeal.appeal;
            manager->appeals.push_back(appeal);
            waitForEnter();
        } else if (choice == "2") {
            Appeal appeal(currentStudentId, currentStudentName, "成绩申诉");
            cout << "请写出你的申请理由，以及修改科目和成绩:" << endl;
            cin >> appeal.appeal;
            manager->appeals.push_back(appeal);
            waitForEnter();
        } else if (choice == "3") {
            for (auto& item : manager->appeals) {
                if (item.id == currentStudentId) {
                    if (item.reply.empty()) {
                        cout << "暂无回复" << endl;
                        waitForEnter();
                    } else {
                        cout << item.reply << endl;
                        waitForEnter();
                        return;
                    }
                }
            }
        }
        else if (choice == "4") return;
        else {
            cout << "❌无效指令" << endl;
            waitForEnter();
        }
    }
}

bool StudentUI::login() {
    while (!isLoggedIn) {
        clearScreen();
        cout << "============================" << endl;
        cout << "         学生登录系统         " << endl;
        cout << "============================" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 退出" << endl;
        cout << "============================" << endl;
        cout << "请输入您的指令：" ;
        string num;
        cin >> num;
        cout << endl;
        if (num == "1") {
            if (auth->login()) {
                if (auth->getCurrentRole() == "student") {
                    string userId = auth->getCurrentUserId();
                    StudentNode* curr = manager->getHead();
                    bool found = false;
                    while (curr) {
                        if (curr->data.getId() == userId) {
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
                    auth->logout();
                }
            }
        } else if (num == "2") {
            cout << "已退出登录!" << endl;
            return false;
        } else {
            cout << "❌ 请输入正确指令！" << endl;
            waitForEnter();
        }
    }
    return false;
}
void StudentUI::run() {
    while (true) {
        if (!isLoggedIn) {
            if (!login()) {
                break;
            }
        }
        showMainMenu();
    }
}
