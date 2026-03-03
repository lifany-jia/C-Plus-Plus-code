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
    :manager(mgr), auth(auth), isLoggedIn(auth->isLoggedInStatus()), isAdminMode(false) {
}
TeacherUI::TeacherUI(StudentManage* mgr, Auth* auth, std::string teacherId)
:manager(mgr), auth(auth), isLoggedIn(true), isAdminMode(true){
    currentTeacherId = stoi(teacherId);
    const auto& allUsers = auth->getAllUsers();
    auto it = allUsers.find(teacherId);
    if (it != allUsers.end()) {
        currentClass = it->second.className;
        currentTeacherName = it->second.realName;
    }
}
void TeacherUI::clearScreen() {
    system("clear");
}
void TeacherUI::waitForEnter() {
    cout << "\n按回车键继续..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string TeacherUI::getRankString(string studentId, StudentManage* manager) {
    int rank = 0;
    if (manager->getStudentRank(studentId, rank)) {
        return "第" + to_string(rank) + "名";
    }
    return "无数据";
}

string TeacherUI::getRankString(string studentId, StudentManage* manager, const string& subject) {
    int rank = 0;
    if (manager->getStudentRank(studentId, subject, rank)) {
        return "第" + to_string(rank) + "名";
    }
    return "无数据";
}

void TeacherUI::showMainMenu() {
    while (isLoggedIn) {
        clearScreen();
        cout << "===========================" << endl;
        cout << "        🧑‍🏫教师工作台" << endl;
        cout << "===========================" << endl;
        if (isAdminMode) cout << "[管理员模式]" << endl;
        cout << "教师：" << currentTeacherName << endl;
        cout << "班级：" << currentClass << endl;
        cout << "===========================" << endl;
        cout << "1. 学生信息管理" << endl;
        cout << "2. 成绩查看" << endl;
        cout << "3. 成绩分析" << endl;
        cout << "4. 学生信息下载至文件" << endl;
        cout << "5. 文件导入" << endl;
        cout << "6. 联系管理员" << endl;
        cout << "7. 返回上一级" << endl;
        cout << "===========================" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                TeacherUI::studentManageMenu(manager);
                break;
            case 2:
                listClassStudents();
                break;
            case 3:
                showClassAverage();
                break;
            case 4:
                cout << "1. 保存为文本文件" << endl;
                cout << "2. 保存为二进制文件" << endl;
                int num;
                cin >> num;
                cout << endl;
                switch (num) {
                    case 1:
                        FileOperation::saveText(*manager);
                        waitForEnter();
                        break;
                    case 2:
                        FileOperation::saveBinary(*manager);
                        waitForEnter();
                        break;
                    default:
                        break;
                }
                break;
            case 5:
                cout << "1. 文本文件导入" << endl;
                cout << "2. 二进制文件导入" << endl;
                int numm;
                cin >> numm;
                cout << endl;
                switch (numm) {
                    case 1:
                        FileOperation::loadText(*manager);
                        waitForEnter();
                        break;
                    case 2:
                        FileOperation::loadBinary(*manager);
                        waitForEnter();
                        break;
                    default:
                        break;
                }
                break;
            case 6:
                addAppeal();
                break;
            case 7:
                isLoggedIn = false;
                isAdminMode = false;
                auth->logout();
                cout << "已退出登录！" << endl;
                waitForEnter();
                break;;
            default:
                cout << "❌ 无效选择" << endl;
                waitForEnter();
        }
    }
}

void TeacherUI::studentManageMenu(StudentManage* manager) {
    while (true) {
        clearScreen();
        cout << "============学生信息管理===========" << endl;
        cout << "1. 增添学生成绩信息" << endl;
        cout << "2. 删除学生信息" << endl;
        cout << "3. 修改学生信息" << endl;
        cout << "4. 查看学生信息" << endl;
        cout << "5. 返回上一级" << endl;
        cout << "---------------------------------" << endl;
        cout << "请选择：" ;
        int choice;
        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                TeacherUI::addStudentSubject(manager);
                break;
            case 2:
                while (true) {
                    string id;
                    cout << "请输入删除学生的ID(输入0退出）：" ;
                    cin >> id;
                    cout << endl;
                    if (id == "0") break;
                    if (manager->removeStudentById(id)) {
                        cout << id << "删除成功！" << endl;
                    } else cout << id << "删除失败！" << endl;
                }
                break;
            case 3:
                TeacherUI::modifyStudent(manager);
                break;
            case 4:
                while (true) {
                    clearScreen();
                    string stu;
                    cout << "请输入学生的ID(输入0退出）：" ;
                    cin >> stu;
                    if (stu == "0") break;
                    Information* stuInfo = manager->getStudentById(stu);
                    if (stuInfo) {
                        TeacherUI::printStudentDetail(manager,*stuInfo);
                    } else {
                        cout << "未找到改学生" << endl;
                    }
                    waitForEnter();
                }
                break;
            case 5: return;
            default:
                cout << "❌ 无效选择" << endl;
                waitForEnter();
        }
    }
}

void TeacherUI::addStudentSubject(StudentManage* manager) {
    while (true) {
        clearScreen();
        cout << "==============学生成绩添加============" << endl;
        cout << "请输入学生ID(输入0退出）：" ;
        string id;
        cin >> id;
        if (id == "0") break;
        Information* curr = manager->getStudentById(id);
        if (!curr) {
            cout << "\n当前学生不存在！" << endl;
            waitForEnter();
            continue;
        }
        string subName;
        int score;
        while (true) {
            cout << "\n请输入科目名称（输入0退出）：" ;
            cin >> subName;
            if (subName == "0") break;
            cout << "\n请输入学生成绩：" ;
            cin >> score;
            cout << endl;
            curr->addSubject(subName, score);
            cout << subName << "成绩添加成功！" << endl;
        }
        return ;
    }
}

void TeacherUI::modifyStudent(StudentManage* manager) {
    while (true) {
        clearScreen();
        cout << "=============修改信息============" << endl;
        cout << "1. 添加科目成绩" << endl;
        cout << "2. 修改科目成绩" << endl;
        cout << "3. 删除科目成绩" << endl;
        cout << "4. 修改学生姓名" << endl;
        cout << "5. 修改学生班级" << endl;
        cout << "6. 返回上一级" << endl;
        cout << "--------------------------------" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        if (choice == 6) return;
        cout << "\n请输入修改的学生的ID：";
        string id;
        cin >> id;
        Information* stu = manager->getStudentById(id);
        if (!stu) {
            cout << "该学生不存在！" << endl;
            waitForEnter();
            continue;
        }
        string subName, className, name;
        int score;
        switch (choice) {
            case 1:
                cout << "请输入添加科目名称：";
                cin >> subName;
                cout << "\n请输入科目成绩：";
                cin >> score;
                cout << endl;
                if (!stu->addSubject(subName, score)) cout << "添加失败！" << endl;
                break;
            case 2:
                cout << "请输入修改科目名称：";
                cin >> subName;
                cout << "\n请输入科目成绩：";
                cin >> score;
                cout << endl;
                if (!stu->updateSubjectScore(subName, score)) cout << "修改失败！" << endl;
                break;
            case 3:
                cout << "请输入删除科目名称：";
                cin >> subName;
                cout << endl;
                if (!stu->removeSubject(subName)) cout << "删除失败！" << endl;
                break;
            case 4:
                cout << "请输入修改后的学生姓名：";
                cin >> name;
                cout << endl;
                stu->setName(name);
                cout << "修改成功！" << endl;
                waitForEnter();
                break;
            case 5:
                cout << "请输入学生班级：";
                cin >> className;
                cout << endl;
                stu->setClassName(className);
                break;
            case 6 : return;
            default:
                cout << "❌无效指令" << endl;
                waitForEnter();
        }
    }
}

void TeacherUI::printStudentDetail(StudentManage* manager, const Information &stu) {
    string id = stu.getId();
    string strRank = TeacherUI::getRankString(id, manager);
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
    cout << left << setw(15) << "科目" << setw(10) << "成绩" << setw(15) << "排名" << endl;
    cout << "------------------------------" << endl;
    while (sub) {
        string strSubRank = TeacherUI::getRankString(id, manager, sub->name);
        cout << left << setw(15) << sub->name << setw(10) << sub->score << setw(10) << strSubRank << endl;
        sub = sub->next;
    }
    cout << "------------------------------" << endl;
    cout << "总分：" << stu.getTotalScore() << "分" << endl;
    cout << "平均分：" << fixed << setprecision(2) << stu.getAverageScore() << "分" << endl;
}

void TeacherUI::listClassStudents() {
    vector<const Information*> students;
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getClassName() == currentClass) {
            students.push_back(&curr->data);
        }
        curr = curr->next;
    }
    if (students.empty()) {
        cout << "本班暂无学生数据" << endl;
        waitForEnter();
        return;
    }
    sort(students.begin(), students.end(),
         [](const Information* a, const Information* b){
        return a->getTotalScore() > b->getTotalScore();
    });
    const int PAGE_SIZE = 5;
    size_t totalStudent = students.size();
    int totalPages = ((int)totalStudent + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentPage = 1;
    while (true) {
        clearScreen();
        cout << "===============" << currentClass << "成绩单===============" << endl;
        cout << "第" << currentPage << "页/共" << totalPages << "页" << endl;
        cout << "总人数：" << totalStudent << "人" << endl;
        cout << "----------------------------------------" << endl;
        int startIdx = (currentPage - 1) * PAGE_SIZE;
        int endIdx = min(startIdx + PAGE_SIZE, (int)totalStudent);
        cout << left << setw(10) << "名次" << setw(10) << "学号" << setw(15) << "姓名" << setw(10) << "总分" << setw(12) << "平均分" << endl;
        cout << "----------------------------------------" << endl;
        for (int i = startIdx; i < endIdx; i++) {
            const Information* stu = students[i];
            int rank = i + 1;
            cout << left << setw(6) << rank << setw(10) << stu->getId() << setw(15) << stu->getName() << setw(10) << stu->getTotalScore() << fixed << setprecision(2) << setw(10) << stu->getAverageScore() << endl;
        }
        cout << "----------------------------------------" << endl;
        cout << "n : 下一页    p : 上一页   q : 退出";
        char ch;
        cin >> ch;
        cout << endl;
        if (ch == 'n' || ch == 'N') {
            if (currentPage < totalPages) currentPage++;
        } else if (ch == 'p' || ch == 'P') {
            if (currentPage > 1)  currentPage--;
        } else if (ch == 'q' || ch == 'Q') break;
        else {
            cout << "❌ 无效输入" << endl;
            waitForEnter();
        }
    }
}

void TeacherUI::addAppeal() {
    while (isLoggedIn) {
        clearScreen();
        cout << "==========投递箱==========" << endl;
        cout << "1. 账户密码修改" << endl;
        cout << "2. 学生账户注册申请" << endl;
        cout << "3. 查看回复" << endl;
        cout << "4. 返回上一级" << endl;
        cout << "--------------------------" << endl;
        cout << "请选择：";
        int choice;
        cin >> choice;
        cout << endl;
        if (choice == 1) {
            Appeal appeal(currentTeacherId, currentTeacherName, "账号密码修改");
            cout << "请写出你的诉求，账户ID，新密码:" << endl;
            cin >> appeal.appeal;
            manager->appeals.push_back(appeal);
            waitForEnter();
        } else if (choice == 2) {
            Appeal appeal(currentTeacherId, currentTeacherName, "学生账号注册");
            cout << "请写出注册理由，账号ID，姓名，班级:" << endl;
            cin >> appeal.appeal;
            manager->appeals.push_back(appeal);
            waitForEnter();
        } else if (choice == 3) {
            for (auto& item : manager->appeals) {
                if (item.id == currentTeacherId) {
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
        else if (choice == 4) return;
        else {
            cout << "❌无效指令" << endl;
            waitForEnter();
        }
    }
}

void TeacherUI::showClassAverage() {
    clearScreen();
    cout << "============成绩分析============" << endl;
    vector<Information*> myClass;
    StudentNode* curr = manager->getHead();
    while (curr) {
        if (curr->data.getClassName() == currentClass) {
            myClass.push_back(&curr->data);
        }
        curr = curr->next;
    }
    if (myClass.empty()) {
        cout << "本班暂无学生数据" << endl;
        waitForEnter();
        return;
    }
    cout << "班级：" << currentClass << endl;
    cout << "总人数：" << myClass.size() << "人" << endl;
    int total = 0;
    for (const auto& stu : myClass) {
        total += stu->getTotalScore();
    }
    cout << "班级总平均分：" << fixed << setprecision(2) << (double)total / myClass.size() << endl;
    map<string, pair<int, int>> subjects;
    for (const auto& stu : myClass) {
        Subject* sub = stu->getSubjectHead();
        while (sub) {
            subjects[sub->name].first += sub->score;
            subjects[sub->name].second++;
            sub = sub->next;
        }
    }
    if (subjects.empty()) {
        cout << "暂无科目成绩数据" << endl;
    } else {
        for (const auto& item : subjects) {
            double avg = (double)item.second.first / item.second.second;
            cout << left << setw(15) << item.first << setw(10) << "平均分"  << setw(10) << fixed << setprecision(2) << avg << endl;
        }
    }
    waitForEnter();
}

bool TeacherUI::login() {
    if (isAdminMode) {
        return true;
    }
    while (!isLoggedIn) {
        clearScreen();
        cout << "==============================" << endl;
        cout << "         🧑‍🏫教师登录系统" << endl;
        cout << "==============================" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 退出" << endl;
        cout << "==============================" << endl;
        cout << "请输入您的指令：";
        int choice;
        cin >> choice;
        cout << endl;
        if (choice == 1) {
            if (auth->login()) {
                if (auth->getCurrentRole() == "teacher") {
                    currentTeacherId = auth->getCurrentUserId();
                    currentClass = auth->users[auth->getCurrentUserId()].className;
                    currentTeacherName = auth->users[auth->getCurrentUserId()].realName;
                    isLoggedIn = true;
                    cout << "🎉欢迎" << currentTeacherName << "老师！" << endl;
                    waitForEnter();
                    return true;
                } else {
                    cout << "❌ 错误：这不是教师账户" << endl;
                    auth->logout();
                    waitForEnter();
                }
            }
        } else if (choice == 2) {
            return false;
        } else {
            cout << "❌ 请输入正确指令" << endl;
            waitForEnter();
        }
    }
    return false;
}

void TeacherUI::run() {
    while (true) {
        if (!isAdminMode && !isLoggedIn) {
            if (!login()) {
                break;
            }
        }
        showMainMenu();
    }
}


