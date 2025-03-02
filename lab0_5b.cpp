#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>

//a node in the list
struct Node {
    char data;
    Node* next;
};

//append a node to the list
void append(Node** head_ref, const char new_data){
    Node* new_node = new Node();
    Node* last = *head_ref;

    //new node construction
    new_node->data = new_data;
    new_node->next = nullptr;

    //if list is empty, make the new node as head
    if (*head_ref == nullptr) {
        *head_ref = new_node;
        return;
    }
    //else append the new node to the list
    while (last->next != nullptr)
        last = last->next;
    
    last->next = new_node;
}
void createList(const char* str){
    Node* head = nullptr;
}