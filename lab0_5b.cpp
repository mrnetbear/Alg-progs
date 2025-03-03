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
        if (isdigit(it) || it == '\t' || it == ' ')
            append(&head, it);
        else throw std::invalid_argument("Invalid digit: " + std::string(1, it)); //exception
    }
    return head;
}

//printing the list
void print_list(Node* head){
    while (head != nullptr) {
        std::cout << head->data;
        head = head->next;
    }
    std::cout << std::endl;
}

void list_transform(Node* head, Node** evenHead, Node** oddHead){
    size_t i = 1;
    bool fIsDigit = false;
    Node* current = head;
    std::cout << "busy... preparing loop..." << std::endl;
    while(current != nullptr){
        if(isdigit(current->data)){
            if (i % 2){
                append(oddHead, current->data);
            }
            else {
                append(evenHead, current->data);
            }
            ++i;
            fIsDigit = true;
            std::cout << "busy... digit found!" << std::endl;

        }
        else if (fIsDigit){
            append(evenHead, ' ');
            append(oddHead, ' ');
            i = 1;
            fIsDigit = false;
            std::cout << "busy... space found!" << std::endl;

        }
        current = current->next; 
    }
    std::cout << "busy... loop is done!" << std::endl;
}

int main(){
    std::string line;

    while (std::getline(std::cin, line)) {   
        Node* crr_line = nullptr;
        Node* evenHead = nullptr;
        Node* oddHead = nullptr;
        try{ 
            crr_line = to_list(line);
            print_list(crr_line);
            list_transform(crr_line, &evenHead, &oddHead);
            std::cout << "Even: ";
            print_list(evenHead);
            std::cout << "Odd: ";
            print_list(oddHead);
            std::cout << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << '\n';
        }
        
    }
    return 0;
}