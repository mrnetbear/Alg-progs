#include <iostream>
#include <sstream>
#include <string>
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

//transform the string into the list
Node* to_list(const std::string& str){
    Node* head = nullptr;
    for (auto it : str){
        append(&head, it);
    }
    return head;
}

void print_list(Node* head){
    while (head != nullptr) {
        std::cout << head->data;
        head = head->next;
    }
    std::cout << std::endl;
}

int main(){
    std::string line;

    while (std::getline(std::cin, line)) {    
        Node* crr_line = to_list(line);
        print_list(crr_line);
    }
    return 0;
}