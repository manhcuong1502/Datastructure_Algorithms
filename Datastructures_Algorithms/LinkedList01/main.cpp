#include "lib.h"

Node* createLinkedList(int n, int* arr){
    //CODE HERE
    return NULL;
}

int removeAt(Node* &list, int pos) {
    //CODE HERE
    return -1;
}

int remove(Node* &list, int value){
    //CODE HERE
    return -1;
}

void sort(Node* list){
    //CODE HERE
}

void printList(Node* list){
    //CODE HERE
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
        cout << remove(ls, value) << endl;
        cout << removeAt(ls,pos) << endl;        
        sort(ls);
        printList(ls);
    }
    catch (exception e){
        cout << e.what() << endl;
    }
    return EXIT_SUCCESS;
}