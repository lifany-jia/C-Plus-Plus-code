//
//  student_manage.cpp
//  Student Management System
//
//  Created by lifany on 2026/1/18.
//
#include "student_manage.h"

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
    //只交换数据
}

//基础操作
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

//学生信息
bool StudentManage::addStudent(const Information &info) {
    if (findNodeById(info.getId())) {
        return false;
    }
    // 检查ID唯一性（利用封装：Information提供getId()）
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

//成绩信息修改
bool StudentManage::addSubjectToStudent(int id, const std::string &subjectName, int score) {
    StudentNode* node = findNodeById(id);
    if (node) {
        return node->data.addSubject(subjectName, score);
    }
    return false;
}
bool StudentManage::updateStudentSubjectScore(int id, const std::string &subjectName, int score) {
    StudentNode* student = findNodeById(id);
    return student ? student->data.updateSubjectScore(subjectName, score) : false;
}
bool StudentManage::removeStudentSubjectScore(int id, const std::string &subjectName) {
    StudentNode* node = findNodeById(id);
    return node ? node->data.removeSubject(subjectName) : false;
}

// 排序

