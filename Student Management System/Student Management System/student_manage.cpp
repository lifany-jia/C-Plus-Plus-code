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
            int a, b;
            bool hasa = curr->data.getSubjectScore(subjectName, a);
            bool hasb = curr->next->data.getSubjectScore(subjectName, b);
            if (!hasa) a = -1;
            if (!hasb) b = -1;
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

int StudentManage::getTotalPages(int pageSize) const {
    if (pageSize <= 0) return 0;
    return (size + pageSize - 1) / pageSize;
    // 向上取整
}
StudentNode* StudentManage::getPageStart(int pageIndex, int pageSize) const {
    if (pageIndex < 1 || pageSize <= 0) {
        return nullptr;
    }
    int startPos = (pageIndex - 1) * pageSize;
    // 例子：找第2页，每页3个
    // startPos = (2-1) * 3 = 3
    // 意思：跳过前3个学生，从第4个开始（因为从0开始数）
    if (startPos >= size) {
        return nullptr;
    }
    StudentNode* curr = head;
    for (int i = 0; i < startPos && curr; i++) {
        curr = curr->next;
    }
    return curr;
}

StudentNode* StudentManage::getHead() const {
    return head;
}
StudentNode* StudentManage::getTail() const {
    return tail;
}
