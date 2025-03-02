#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>

class Node{
    char data;
    class Node* next;
    friend class List;
};

class List{
    Node *head;
    Node* Prev(Node *);
public:
    List(){
        head = nullptr;
    }
    bool isEmpty() {return head == nullptr;}
    char getData() {return head->data;}
    Node* getFirst() {return head;}
    Node* getLast();
    void Clear();
    Node* Next(Node *);
    Node* Append(char, Node *);

};