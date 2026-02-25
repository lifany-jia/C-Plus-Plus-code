//
//  student_manage.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#include "student_manage.h"
#include <utility>
#include <vector>
#include <algorithm>

StudentNode::StudentNode(const Information& info)
    :data(info), prev(nullptr), next(nullptr) {
}

StudentManage::StudentManage()
    :head(nullptr), tail(nullptr), size(0){
}
StudentManage::~StudentManage() {
    clear();
}

// 内部函数工具
StudentNode* StudentManage::findNodeById(int id) const{
    StudentNode* curr = head;
    while (curr) {
        if (curr->data.getId() == id) {
            //information封装，只能使用它提供的接口，不能直接访问具体private数据
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}
void StudentManage::swapNode(StudentNode *a, StudentNode *b) {
    if (!a || !b || a == b) {
        return;
    }
    Information temp = a->data;
    // 默认构造函数起作用
    a->data = b->data;
    b->data = temp;
}
int StudentManage::getSize() const {
    return size;
}
bool StudentManage::isEmpty() const {
    return size == 0;
}
void StudentManage::clear() {
    StudentNode* curr = head;
    while (curr) {
        StudentNode* next = curr->next;
        delete curr;
        curr = next;
    }
    head = tail = nullptr;
    size = 0;
}
bool StudentManage::addStudent(const Information &info) {
    if (findNodeById(info.getId())) {
        return false;
    }
    StudentNode* newnode = new StudentNode(info);
    //StudentNode没有默认构造函数，因为没有意义，只是一个节点，如果没内容反而麻烦
    // 参数info是const引用，避免拷贝传入
    if (isEmpty()) {
        head = tail = newnode;
    } else {
        tail->next = newnode;
        newnode->prev = tail;
        tail = newnode;
    }
    size++;
    return true;
}
bool StudentManage::removeStudentById(int id) {
    StudentNode* node = findNodeById(id);
    if (!node) {
        return false;
    }
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    delete node;
    size--;
    return true;
}
Information* StudentManage::getStudentById(int id) {
    StudentNode* node = findNodeById(id);
    return node ? &(node->data) : nullptr;
}
const Information* StudentManage::getStudentById(int id) const
{
    const StudentNode* node = findNodeById(id);
    return node ? &(node->data) : nullptr;
}

// 排序
void StudentManage::sortByTotalScore(bool ascending) {
    if (size < 2) {
        return;
    }
    bool swapped;
    do {
        swapped = false;
        StudentNode* curr = head;
        while (curr && curr->next) {
            int a = curr->data.getTotalScore();
            int b = curr->next->data.getTotalScore();
            bool shouldSwap = ascending ? (a > b) : (a < b);
            if (shouldSwap) {
                swapNode(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);
}

void StudentManage::sortByAverageScore(bool ascending) {
    if (size < 2) {
        return ;
    }
    bool swapped;
    do {
        swapped = false;
        StudentNode* curr = head;
        while (curr && curr->next) {
            double a = curr->data.getAverageScore();
            double b = curr->next->data.getAverageScore();
            bool shouldSwap = ascending ? (a > b) : (a < b);
            if (shouldSwap) {
                swapNode(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);
}

void StudentManage::sortBySubjectScore(const std::string &subjectName, bool ascending){
    if (size < 2) return;
    bool swapped;
    do {
        swapped = false;
        StudentNode* curr = head;
        while (curr && curr->next) {
            int a = curr->data.getSubjectScore(subjectName);
            int b = curr->next->data.getSubjectScore(subjectName);
            bool shouldSwap = false;
            if (ascending) {
                // 升序，无科目排最后
                shouldSwap = (a > b) || (a == -1 && b != -1);
            } else {
                shouldSwap = (a < b) || (a == -1 && b != -1);
            }
            if (shouldSwap) {
                swapNode(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    }while (swapped);
}

StudentNode* StudentManage::getHead() const {
    return head;
}
StudentNode* StudentManage::getTail() const {
    return tail;
}

bool StudentManage::getStudentRank(int studentId, int& myRank) const {
    const Information* currStudent = getStudentById(studentId);
    if (!currStudent) return false;
    std::vector<std::pair<int, int>> classTotalScore;
    StudentNode* curr = getHead();
    while (curr) {
        classTotalScore.push_back({curr->data.getId(), curr->data.getTotalScore()});
        curr = curr->next;
    }
    std::sort(classTotalScore.begin(), classTotalScore.end(),
              [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    bool found = false;
    for (int i = 0; i < classTotalScore.size(); i++) {
        if (classTotalScore[i].first == studentId) {
            myRank = i + 1;
            return true;
        }
    }
    return false;
}

bool StudentManage::getStudentRank(int studentId, const std::string &subjectName, int& myRank) const {
    const Information* currStudent = getStudentById(studentId);
    if (!currStudent) return false;
    std::vector<std::pair<int, int>> classScores;
    StudentNode* curr = getHead();
    while (curr) {
        int score = curr->data.getSubjectScore(subjectName);
        if (score != -1) {
            classScores.push_back({curr->data.getId(), score});
        }
        curr = curr->next;
    }
    std::sort(classScores.begin(), classScores.end(),
              [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    bool found = false;
    for (int i = 0; i < classScores.size(); i++) {
        if (classScores[i].first == studentId) {
            myRank = i + 1;
            return true;
        }
    }
    return false;
}


