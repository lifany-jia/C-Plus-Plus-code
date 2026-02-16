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
};
class Auth {
private:
    std::map<std::string, User> users;
    std::string currentUserId;
    bool isLoggedIn;
public:
    Auth();
    
    //用户管理
    bool registerUser();
    bool login();
    void logout();
    bool changePassword();
    bool deleteUser(); //管理员删除
    
    //查询
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
    
    //文件
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    //看文件是否存在
    static bool isFileExist(const std::string & filename);
};
#endif
