#include "lib.h"

Node* createLinkedList(int n, int* arr){
    //CODE HERE
    Node* ls = new Node();
    ls->next = nullptr;
    ls->value = arr[0];
    for(Node* p = ls, int i = 1; i < n; i++){
        p->next = new Node();
        p = p->next;
        p->value = arr[i];
        p->next = nullptr;
    }
    return ls;
}

int removeAt(Node* &list, int pos) {
    //CODE HERE
    Node* p = list;
    int count = 0;
    while (p != nullptr){
        count ++;
        p = p->next;
    }
    if (pos < 0 || pos >= count)
    return -1;
    if (pos == 0) {
        count = list->value;
        delete list;
        list = NULL;
    }
    else{
        
    }
}

int remove(Node* &list, int value){
    //CODE HERE
    return 0;
}

void sort(Node* list){
    //CODE HERE
}

void printList(Node* list){

}

int main(int argv, char** argc){
    try{
        ifstream ifs;
        ifs.open(argc[1]);
        int n, pos, value;
        ifs >> n;
        int* arr = new int[n];
        for (int i = 0; i < n; i++){
            ifs >> arr[i];
        }
        ifs >> pos;
        ifs >> value;
        ifs.close();
        Node* ls = createLinkedList(n, arr);
        printList(ls);
        
        cout << removeAt(ls,pos) << endl;
        cout << remove(ls, value) << endl;
        sort(ls);
        printList(ls);
    }
    catch (exception e){
        cout << e.what() << endl;
    }
    return EXIT_SUCCESS;
}