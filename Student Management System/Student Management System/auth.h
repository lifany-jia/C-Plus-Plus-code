//
//  auth.h
//  Student Management System
//
//  Created by lifany on 2026/2/15.
//
#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <map>

struct User {
    std::string userId; // 用户Id
    std::string password;
    std::string role;  //角色
    std::string className;
    std::string realName;
    
    User() {}
    User(std::string userId, std::string password, std::string role, std::string className, std::string realName)
        :userId(userId), password(password), role(role), className(className), realName(realName){}
};
class Auth {
private:
    std::map<std::string, User> users;
    std::string currentUserId;
    bool isLoggedIn;
public:
    Auth();
    //用户管理
    bool login();
    void logout();
    bool changePassword();
    bool deleteUser(); //管理员删除
    bool addUser(const User& user);
    bool isUserExist(const std::string& userId) const; //用户是否存在
    std::string getCurrentRole() const; //用户的角色
    std::string getCurrentUserId() const; //当前用户ID
    bool isLoggedInStatus() const {  //用户登陆状态
        return isLoggedIn;
    }
    size_t getUserCount() const {
        return users.size();   //用户数量
    }
    const std::map<std::string, User>& getAllUsers() const {
        return users;
    }
};
#endif
