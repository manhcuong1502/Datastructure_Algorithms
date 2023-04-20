#include <iostream>
using namespace std;

class IntDLinkedList
{
private:
    class Node;
private:
    Node *head;
    Node *tail;
    int count;
public:
    IntDLinkedList() : head(NULL), tail(NULL), count(0) {}
    ~IntDLinkedList()
    {
        this->clear();
    }
    
    void add(int element)
    {
        Node* pNew = new Node(element);
        if (count == 0)
        {
            head = pNew;
            tail = pNew;
        }
        else
        {
            tail->next = pNew;
            pNew->prev = tail;
            tail = pNew;
        }
        count++;
    }

    void add(int index, int element)
    {
        if (count == 0 || index == count) add(element);
        else
        {
            Node* pNew = new Node(element);
            if (index == 0)
            {
                pNew->next = head;
                head->prev = pNew;
                head = pNew;
            }
            else
            {
                if(count-index-1 > index)
                {
                    int i = 0;
                    Node* temp = head;
                    while (i != index-1)
                    {
                        temp = temp->next;
                        i++;
                    }
                    temp->next->prev = pNew;
                    pNew->next = temp->next;
                    temp->next = pNew;
                    pNew->prev = temp;
                }
                else
                {
                    int i = 0;
                    Node* temp = tail;
                    while (i != count-index-1)
                    {
                        temp = temp->prev;
                        i++;
                    }
                    temp->prev->next = pNew;
                    pNew->prev = temp->prev;
                    pNew->next = temp;
                }
            }
        }
    }

    int removeAt(int index)
    {
        if (index == 0)
        {
            head = head->next;
            head->prev = nullptr;
            count--;
        }
        else if (index == )
        else if (index == count-1)
        {
            tail->prev->next = nullptr;
            tail = tail->prev;
            count--;
        }
        else 
        {

        }
        return -1;
    }

    bool removeItem(int element)
    {
        if (index == 0 || count == 0) remove(element);
        else 
        {
            Node* pNew = new Node(element);
            if (index == 0)
            {
                pNew->next = head;
                head->prev = pNew;
                head = pNew;
            }
            else
            {

            }

        }
        return false;
    }

    int get(int index)
    {
        Node* temp;
        if (index < count/2) temp = head;
        for (int i=0; i < index; i++)
        {
            temp = temp->next;
        }
        else
        {
            temp = tail;
            for (int i=0; i < count - index -1; i++)
            {
                temp = temp->prev;
            }
            return temp;
        }
        return -1;
    }

    void set(int index, int element)
    {
        
    }

    int indexOf(int element)
    {
        return -1;
    }

    bool contains(int element)
    {
        return false;
    }

    int size()
    {
        return 0;
    }

    bool empty()
    {
        return false;
    }

    void clear()
    {
        
    }

    void dump()
    {
        if (this->count == 0)
        {
            assert(this->head == NULL);
            assert(this->tail == NULL);
            cout << "List: []" << endl;
            cout << "Reverse list: []" << endl;
            return;
        }
        
        cout << "List: [";
        Node *temp = this->head;
        while (temp != this->tail)
        {
            cout << temp->value << ",";
            temp = temp->next;
        }
        cout << temp->value << "]" << endl;
        cout << "Reverse list: [";
        temp = this->tail;
        while (temp != this->head)
        {
            cout << temp->value << ",";
            temp = temp->prev;
        }
        cout << temp->value << "]" << endl;
    }
    
private:
    class Node
    {
    public:
        int value;
        Node *prev;
        Node *next;
        Node(int value = 0, Node *prev = NULL, Node *next = NULL) : value(value), prev(prev), next(next) {}
    };
};

int main ()
{

}