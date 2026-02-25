//
//  information.h
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#ifndef INFORMATION_H
#define INFORMATION_H

#include <string>

class Subject {
public:
    std::string name;
    int score;
    Subject* next;
    
    Subject(const std::string& name, int score);
};

class Information {
private:
    int id;
    std::string name;
    std::string className;
    Subject* subjectHead;
public:
    Information();
    Information(int id, const std::string& name, const std::string& className);
    ~Information();
    
    int getId() const;
    std::string getName() const;
    std::string getClassName() const;
    Subject* getSubjectHead() const;
    
    void setName(const std::string& name);
    void setClassName(const std::string& className);
    
    bool addSubject(const std::string& subjectName, int score);
    bool updateSubjectScore(const std::string& subjectName, int score);
    bool removeSubject(const std::string& subjectName);
    int getSubjectScore(const std::string& subjectName) const;
    
    int getTotalScore() const;
    int getScoreCount() const;
    double getAverageScore() const;
    
};
#endif
