//
//  auth.cpp
//  Student Management System
//
//  Created by lifany on 2026/2/15.
//
#include "auth.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Auth::Auth()
    :isLoggedIn(false) {
}

bool Auth::registerUser() {
    cout << "==========用户注册==========" << endl;
    cout << "(输入0退出注册)" <<endl;
    string userId;
    while (true) {
        cout << "请输入您的ID：";
        cin >> userId;
        cout << endl;
        if (userId == "0") {
            cout << "已退出注册页面！" << endl;
            return false;
        }
        if (users.find(userId) != users.end()) {
            cout << "❌ 该用户已存在" << endl;
            continue;
        }
        if (userId.empty()) {
            cout << "❌ ID不能为空！" << endl;
            continue;
        }
        break;
    }
    string role;
    while (true) {
        cout << "请输入您的角色：(student/teacher/admin)";
        cin >> role;
        cout << endl;
        if (role == "0") {
            cout << "已退出注册页面！" << endl;
            return false;
        }
        if (role != "student" && role != "teacher" && role != "admin") {
            cout << "❌ 角色必须是 student/teacher/admin" << endl;
            continue;
        }
        break;
    }
    string password;
    while (true) {
        cout << "请输入您的密码：(不少于六位)";
        cin >> password;
        cout << endl;
        if (password == "0") {
            cout << "已退出注册页面！" << endl;
            return false;
        }
        if (password.length() < 6) {
            cout << "❌ 密码至少为六位!" << endl;
            continue;
        }
        break;
    }
    User newUser;
    newUser.userId = userId;
    newUser.password = password;
    newUser.role = role;
    users[userId] = newUser;
    cout << "✅ 用户注册成功！" << endl;
    return true;
}

bool Auth::login() {
    cout << "==========用户登录==========" << endl;
    cout << "(输入0退出登录)" << endl;
    string userId;
    auto it = users.end();
    while (true) {
        cout << "请输入您的ID：" ;
        cin >> userId;
        cout << endl;
        if (userId == "0") {
            cout << "已退出登录！" << endl;
            return false;
        }
        it = users.find(userId);
        if (it == users.end()) {
            cout << "❌该用户不存在" << endl;
            continue;
        }
        break;
    }
    string password;
    int curr = 0;
    const int MAX_ATTEMPTS = 3;
    while (true) {
        if (curr >= MAX_ATTEMPTS) {
            cout << "尝试密码次数已达上限！" << endl;
            cout << "(忘记密码可联系管理员进行修改)" << endl;
            cout << "已退出登录！" << endl;
            return false;
        }
        cout << "请输入您的密码(剩余尝试次数：" + to_string(MAX_ATTEMPTS - curr) + "次）：";
        cin >> password;
        cout << endl;
        curr++;
        
        if (password == "0") {
            cout << "已退出登录页面！" << endl;
            return false;
        }
        if (it->second.password != password) {
            cout << "❌密码错误" << endl;
            continue;
        }
        break;
    }
    currentUserId = userId;
    isLoggedIn = true;
    cout << "用户登录成功！欢迎👏" + userId << "!" << endl;
    cout << "当前角色🧍🏻‍♀️：" << it->second.role << endl;
    return true;
}

void Auth::logout() {
    currentUserId = "";
    isLoggedIn = false;
}

bool Auth::changePassword() {
    if (!isLoggedIn) {
        cout << "❌ 请先登录！" << endl;
        return false;
    }
    cout << "==========修改密码==========" << endl;
    cout << "(输入0退出修改密码📃)" << endl;
    auto it = users.end();
    bool isAdmin = (users[currentUserId].role == "admin");
    
    if (isAdmin) {
        cout << "进入管理员模式" << endl;
        string id;
        while (true) {
            cout << "请输入要修改密码的用户ID：";
            cin >> id;
            cout << endl;
            if (id == "0") {
                cout << "已退出修改密码📃！" << endl;
                return false;
            }
            it = users.find(id);
            if (it == users.end()) {
                cout << "❌该用户不存在" << endl;
                continue;
            }
            break;
        }
        string gnewPwd, gconfirmPwd;
        while (true) {
            cout << "请输入您的新密码：(不少于六位)";
            cin >> gnewPwd;
            cout << endl;
            if (gnewPwd == "0") {
                cout << "已退出修改密码📃！" << endl;
                return false;
            }
            if (gnewPwd.length() < 6) {
                cout << "❌ 密码至少为六位!" << endl;
                continue;
            }
            cout << "请再次确认新密码：" << endl;
            cin >> gconfirmPwd;
            cout << endl;
            if (gnewPwd != gconfirmPwd) {
                cout << "❌ 两次密码不一致！" << endl;
                continue;
            }
            break;
        }
        it->second.password = gnewPwd;
        cout << "✅修改用户" + id + "密码成功！" << endl;
        if (id == currentUserId) {
            cout << "⚠️ 您修改了自己的密码，请重新登录" << endl;
            isLoggedIn = false;
            currentUserId = "";
            return true;
        }
    } else {
        cout << "进入用户模式" << endl;
        it = users.find(currentUserId);
        string oldPassword;
        int curr = 0;
        const int MAX_ATTEMPTS = 3;
        while (true) {
            if (curr >= MAX_ATTEMPTS) {
                cout << "尝试密码次数已达上限！" << endl;
                cout << "已退出修改密码📃！" << endl;
                return false;
            }
            cout << "请输入您的旧密码(剩余尝试次数：" + to_string(MAX_ATTEMPTS - curr) + "次）：";
            cin >> oldPassword;
            cout << endl;
            curr++;
            
            if (oldPassword == "0") {
                cout << "已退出修改密码📃！" << endl;
                return false;
            }
            if (it->second.password != oldPassword) {
                cout << "❌密码错误" << endl;
                continue;
            }
            break;
        }
        string newPwd, confirmPwd;
        while (true) {
            cout << "请输入您的新密码：(不少于六位)";
            cin >> newPwd;
            cout << endl;
            if (newPwd == "0") {
                cout << "已退出修改密码📃！" << endl;
                return false;
            }
            if (newPwd.length() < 6) {
                cout << "❌ 密码至少为六位!" << endl;
                continue;
            }
            cout << "请再次确认新密码：" << endl;
            cin >> confirmPwd;
            cout << endl;
            if (newPwd != confirmPwd) {
                cout << "❌ 两次密码不一致！" << endl;
                continue;
            }
            break;
        }
        it->second.password = newPwd;
        cout << "✅修改密码成功！请重新登录" << endl;
        isLoggedIn = false;
        currentUserId = "";
    }
    return true;
}

bool Auth::deleteUser() {
    cout << "==========管理员删除用户==========" << endl;
    cout << "(输入0可退出)" << endl;
    string id;
    while (true) {
        cout << "请输入您要删除的用户ID：";
        cin >> id;
        cout << endl;
        if (id == "0") {
            cout << "已退出删除页面！" << endl;
            return false;
        }
        if (users.find(id) == users.end()) {
            cout << "❌当前用户不存在" << endl;
            continue;
        }
        if (id == currentUserId) {
            cout << "⚠️该用户为管理员自己，不能删除当前登录账号！" << endl;
            cout << "请重新输入其他用户ID。" << endl;
            continue;
        }
        break;
    }
    int num;
    cout << "输入0退出/输入其他数字继续删除" << endl;
    cin >> num;
    cout << endl;
    if (num == 0) {
        cout << "已退出删除页面！" << endl;
        return false;
    }
    users.erase(id);
    cout << "✅成功删除用户" + id + "!" << endl;
    cout << "已退出删除页面！" << endl;
    return true;
}

bool Auth::isUserExist(const std::string &userId) const {
    return users.find(userId) != users.end();
}

string Auth::getCurrentRole() const {
    if (!isLoggedIn) return "";
    auto it = users.find(currentUserId);
    if (it == users.end()) return "";
    return it->second.role;
}

string Auth::getCurrentUserId() const {
    if (!isLoggedIn) {
        return "";
    }
    return currentUserId;
}

bool Auth::isFileExist(const std::string &filename) {
    ifstream file(filename);
    return file.is_open();
}

bool Auth::saveToFile(const std::string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 " << filename << " 进行写入" << endl;
        return false;
    }
    file << "# User Data File" << endl;
    file << "# Format:userId,password,role" << endl;
    file << "# Created: " << __DATE__ << " " << __TIME__ << endl;
    file << "# Total users: " << users.size() << endl;
    file << "# ===================================" << endl;
    
    for (const auto& pair : users) {
        const User& user = pair.second;
        file << user.userId << "," << user.password << "," << user.role << endl;
    }
    return true;
}

bool Auth::loadFromFile(const std::string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 " << filename << " 进行读取" << endl;
        return false;
    }
    users.clear();
    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') {
            continue;
        }
        stringstream ss(line);
        User user;
        if (getline(ss, user.userId, ',') && getline(ss, user.password, ',') && getline(ss, user.role, ',')) {
            if (!user.userId.empty() && !user.password.empty() && (user.role == "admin" || user.role == "student" || user.role == "teacher")) {
                users[user.userId] = user;
            } else {
                cerr << "⚠️第" << lineNum << "行数据格式错误，已跳过" << endl;
            }
        } else {
            cerr << "⚠️第" << lineNum << "行格式错误，已跳过" << endl;
        }
    }
    return true;
}
