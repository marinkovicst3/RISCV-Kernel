#ifndef DEQUE_H
#define DEQUE_H

#include "memoryAllocator.h"

template <typename T>
class Deque {
public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void pop_front() {
        if (empty()) return;
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
    }

    void pop_back() {
        if (empty()) return;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
    }

    T& front() {
        return head->data;
    }

    const T& front() const {
        return head->data;
    }

    T& back() {
        return tail->data;
    }

    const T& back() const {
        return tail->data;
    }

private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value, Node* n = nullptr, Node* p = nullptr)
            : data(value), next(n), prev(p) {}

        void* operator new(size_t size) {
            return MemoryAllocator::getInstance().mem_alloc(size);
        }

        void operator delete(void* ptr) noexcept {
            MemoryAllocator::getInstance().mem_free(ptr);
        }
    };

    Node* head;
    Node* tail;
    size_t count;
};

#endif