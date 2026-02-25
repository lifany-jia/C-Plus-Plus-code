//
//  information.cpp
//  Student Management System
//
//  Created by lifany on 2026/2/7.
//
#include "information.h"

// Subject
Subject::Subject(const std::string& name, int score)
    :name(name), score(score), next(nullptr) {
}

// Information
Information::Information()
    :id(0), name(""), className(""), subjectHead(nullptr) {
}
Information::Information(int id, const std::string& name, const std::string& className)
    :id(id), name(name), className(className), subjectHead(nullptr) {
}
Information::~Information() {
    Subject* curr = subjectHead;
    while (curr) {
        Subject* temp = curr->next;
        delete curr;
        curr = temp;
    }
    subjectHead = nullptr;
}

// Getter
int Information::getId() const {
    return id;
}
std::string Information::getName() const {
    return name;
}
std::string Information::getClassName() const {
    return className;
}
Subject* Information::getSubjectHead() const {
    return subjectHead;
}

// Setter
void Information::setName(const std::string& name) {
    this->name = name;
}
void Information::setClassName(const std::string& className) {
    this->className = className;
}

// Subject
bool Information::addSubject(const std::string& subjectName, int score) {
    Subject* curr = subjectHead;
    while (curr) {
        if (curr->name == subjectName) {
            return false;
        }
        curr = curr->next;
    }
    Subject* newscore = new Subject(subjectName, score);
    newscore->next = subjectHead;
    subjectHead = newscore;
    return true;
}

bool Information::updateSubjectScore(const std::string& subjectName, int score) {
    Subject* curr = subjectHead;
    while (curr) {
        if (curr->name == subjectName) {
            curr->score = score;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool Information::removeSubject(const std::string &subjectName) {
    Subject* curr = subjectHead;
    Subject* prev = nullptr;
    while (curr) {
        if (curr->name == subjectName) {
            if (prev == nullptr) {
                subjectHead = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

int Information::getSubjectScore(const std::string &subjectName) const {
    Subject* curr = subjectHead;
    while (curr) {
        if (curr->name == subjectName) {
            return curr->score;
        }
        curr = curr->next;
    }
    return -1;
}

int Information::getTotalScore() const {
    Subject* curr = subjectHead;
    int sum = 0;
    while (curr) {
        sum += curr->score;
        curr = curr->next;
    }
    return sum;
}

int Information::getScoreCount() const {
    Subject* curr = subjectHead;
    int count = 0;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

double Information::getAverageScore() const {
    int count = getScoreCount();
    if (count == 0) {
        return 0.0;
    }
    int sum = getTotalScore();
    return static_cast<double>(sum) / count;
}
