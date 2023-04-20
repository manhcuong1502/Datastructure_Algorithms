#include "ConcatStringList.h"


/////////////// HELPER_FUNCTION ///////////////

int stringlen (const char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    
    return i;
}

char* strcpy (const char* str, int length)
{
    char* cpystr = new char[length + 1];
    for (int i=0; i < length; i++)
    {
        cpystr[i] = str[i];
    }
    cpystr[length] = '\0';

    return cpystr;
}

ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();

////////////// CONCAT_STRING_LIST /////////////

ConcatStringList::ConcatStringList(const char* str)
{
    this->head = this->tail = new ConcatStringList::CharALNode(str);
    this->len = this->head->length;

    this->head->refNode = refList.add(this->head, 2);
}

int ConcatStringList::length() const
{
    return this->len;
}

char ConcatStringList::get(int index) const
{
    if (index < 0 || index >= this->len) 
        throw std::out_of_range("Index of string is invalid!");

    CharALNode* tmp = this->head;

    while(index >= tmp->length)
    {
        index -= tmp->length;
        tmp = tmp->next;
    }

    return tmp->CharArrayList[index];
}

int ConcatStringList::indexOf(char c) const
{
    CharALNode* tmp = this->head;
    int idx = 0;

    while(tmp != tail->next)
    {
        int i = 0;
        while(i < tmp->length)
        {
            if(c == tmp->CharArrayList[i])
            {
                return idx;
            }
            else 
            {
                idx++;
                i++;
            }
        }
        tmp = tmp->next;
    }

    return -1;
}

string ConcatStringList::toString() const
{
    string s; 
    CharALNode* tmp = this->head;

    while(tmp != tail->next)
    {
        s.append(tmp->CharArrayList);
        tmp = tmp->next;
    }

    return s;
}

ConcatStringList ConcatStringList::concat(const ConcatStringList & otherS) const
{
    this->tail->next = otherS.head;
    otherS.head->prev = this->tail;

    this->head->refNode->increase();
    refList.reSorted(this->head->refNode);

    otherS.tail->refNode->increase();
    refList.reSorted(otherS.tail->refNode);

    return ConcatStringList(this->head, otherS.tail, this->len + otherS.len);
}

ConcatStringList ConcatStringList::subString(int from, int to) const
{
    if(from < 0 || to > this->len)
        throw std::out_of_range("Index of string is invalid");
    else if (from >= to)
        throw std::logic_error("Invalid range");

    int length = to - from;

    CharALNode* tmpH = this->head;
    CharALNode* tmpT = this->head;
    CharALNode* ls = new CharALNode();
    CharALNode* tmp = ls;

    while(from > tmpH->length)
    {
        from -= tmpH->length;
        tmpH = tmpH->next;
    }
    
    while(to > tmpT->length)
    {
        to -= tmpT->length;
        tmpT = tmpT->next;
    }

    while(tmpH != tmpT)
    {
        tmp->next = new CharALNode(tmpH->CharArrayList + from);
        tmp->next->prev = tmp;

        tmp = tmp->next;
        tmpH = tmpH->next;
        from = 0;
    }

    char* str = strcpy(tmpH->CharArrayList + from, to - from);
    tmp->next = new CharALNode(str);
    tmp->next->prev = tmp;
    delete str;

    if (ls->next != tmp->next)
    {
        ls->next->refNode = refList.add(ls->next, 1);
        tmp->next->refNode = refList.add(tmp->next, 1);
    }
    else 
    {
        ls->next->refNode = refList.add(ls->next, 2);
    }

    ls->next->prev = nullptr;
    tmp->next->next = nullptr;
    ConcatStringList* substr = new ConcatStringList(ls->next, tmp->next, length);
    delete ls;

    return *substr;
}

ConcatStringList ConcatStringList::reverse() const
{
    CharALNode* tmp = this->head;
    CharALNode* prev = nullptr;
    CharALNode* curr = new CharALNode(tmp->CharArrayList);
    curr->next = prev;
    prev = curr;
    curr->reverse();
    tmp = tmp->next;

    CharALNode* _tail = curr; 

    while (tmp != this->tail->next)
    {
        curr = new CharALNode(tmp->CharArrayList);
        curr->next = prev;
        prev = curr;
        curr->next->prev = curr;
        curr->reverse();
        tmp = tmp->next;
    }

    if (curr != _tail)
    {
        curr->refNode = refList.add(curr, 1);
        _tail->refNode = refList.add(_tail, 1);
    }
    else 
    {
        curr->refNode = refList.add(curr, 2);
    }

    return ConcatStringList(curr, _tail, this->len);
}

ConcatStringList::~ConcatStringList()
{
    this->head->refNode->decrease();
    if (this->head->refNode->getRefNum() != 0)
    {
        this->head->refNode->moveToHead();
    }
    else
    {
        this->head->refNode->moveToTail();
        refList.zero_node +=1;
    }

    this->tail->refNode->decrease();
    if(this->tail->refNode->getRefNum() != 0)
    {
        this->tail->refNode->moveToHead();
    }
    else
    {
        this->tail->refNode->moveToTail();
        refList.zero_node +=1;
    }
    

    delStrList.add(this->head->refNode, this->tail->refNode);
    delStrList.updated();
    if (refList.n_node == refList.zero_node)
    {
        refList.clear();
    }
}

//////////////// CHAR_AL_NODE ////////////////

ConcatStringList::CharALNode::CharALNode(const char* arr)
{
    this->length = stringlen(arr);
    this->CharArrayList = strcpy(arr, this->length);
    this->next = nullptr;
    this->prev = nullptr;
    refNode = nullptr;
}

void ConcatStringList::CharALNode::reverse()
{
    for (int i=0; i < this->length/2; i++)
    {
        char tmp = this->CharArrayList[i];
        this->CharArrayList[i] = this->CharArrayList[this->length-i-1];
        this->CharArrayList[this->length-i-1] = tmp;
    }
}

ConcatStringList::CharALNode::~CharALNode()
{
    delete this->CharArrayList;
    this->CharArrayList = nullptr;
    this->next = nullptr;
    this->prev = nullptr;
}

/////////////// REFERENCE LIST /////////////////

ConcatStringList::ReferencesList::ReferencesList()
{
    this->head = new ReferenceNode(nullptr, 0);
    this->tail = new ReferenceNode(nullptr, 0);
    this->head->next = tail;
    this->tail->prev = head;
    this->n_node = 0;
    this->zero_node = 0;
}

ConcatStringList::ReferencesList::ReferenceNode* ConcatStringList::ReferencesList::add(ConcatStringList::CharALNode* node, int refNum)
{
    ReferenceNode* new_Node = new ReferenceNode(node, refNum);
    ReferenceNode* tmp = this->head;
    while(tmp->next != this->tail && tmp->next->refNum < refNum)
    {
        tmp = tmp->next;
    }
    new_Node->next = tmp->next;
    new_Node->prev = tmp;
    tmp->next->prev = new_Node;
    tmp->next = new_Node;
    this->n_node++;
    return new_Node;
}


void ConcatStringList::ReferencesList::reSorted(ReferenceNode* updatedNode)
{
    ReferenceNode* curr = updatedNode->next;
    while (updatedNode->refNum > curr->refNum && curr->refNum != 0)
    {
        curr = curr->next;
    }
    if (curr != updatedNode->next)
    {
        ReferenceNode* tmp = updatedNode->next;
        updatedNode->next = curr->next;
        curr->next = tmp;

        updatedNode->next->prev = updatedNode;
        curr->next->prev = curr;

        tmp = updatedNode->prev;
        updatedNode->prev = curr->prev;
        curr->prev = tmp;

        updatedNode->prev->next = updatedNode;
        curr->prev->next = curr;
    }
}

void ConcatStringList::ReferencesList::clear()
{
    ReferenceNode* curr = this->head->next;
    while (curr != this->tail)
    {
        curr = curr->next;
        delete curr->prev;
    }
    this->head->next = tail;
    this->tail->prev = head;
    this->n_node = 0;
    this->zero_node = 0;
}

int ConcatStringList::ReferencesList::size() const
{
    return this->n_node;
}

int ConcatStringList::ReferencesList::refCountAt(int index) const
{
    if (index < 0  || index >= refList.n_node)
    {
        throw std::out_of_range("Index of references list is invalid!");
    }
    ReferenceNode* tmp = refList.head->next;
    while (index != 0)
    {
        tmp = tmp->next;
        index--;
    }
    return tmp->refNum;
}

string ConcatStringList::ReferencesList::refCountsString() const
{
    string s = "RefCounts[]";
    ReferenceNode* tmp = refList.head->next;
    //int i = 0;
    string res = "";
    while (tmp != refList.tail)
    {
        res.append(to_string(tmp->getRefNum()));
        res.append(",");
        
        tmp = tmp->next;
    }
    if (res.length() != 0)
    {
        s.pop_back();
        res.pop_back();
        s += res;
        s += "]";
    }
    return s;
}

ConcatStringList::ReferencesList::~ReferencesList()
{
    delete this->head;
    delete this->tail;
}

////////////// REFERENCE_NODE /////////////////

void ConcatStringList::ReferencesList::ReferenceNode::increase()
{
    refNum++;
}

void ConcatStringList::ReferencesList::ReferenceNode::decrease()
{
    refNum--;
}

int ConcatStringList::ReferencesList::ReferenceNode::getRefNum()
{
    return this->refNum;
}

void ConcatStringList::ReferencesList::ReferenceNode::moveToTail()
{
    ReferenceNode* curr = this->next;
    while(curr->refNum != 0)
    {
        curr = curr->next;
    }
    if (curr != this->next)
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;

        this->next = curr;
        this->prev = curr->prev;
        this->prev->next = this;
        this->next->prev = this;
    }
}

void ConcatStringList::ReferencesList::ReferenceNode::moveToHead()
{
    ReferenceNode* curr = this->prev;
    while (curr->refNum > this->refNum)
    {
        curr = curr->prev;
    }
    if (curr != this->prev)
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;

        this->prev = curr;
        this->next = curr->next;
        this->prev->next = this;
        this->next->prev = this;
    }
}

/////////////////// DELETE STRING LIST ////////////////////////

ConcatStringList::DeleteStringList::DeleteStringList()
{
    this->n_node = 0;
    this->head = new DeleteStringNode();
    this->tail = new DeleteStringNode();
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

void ConcatStringList::DeleteStringList::add(ReferencesList::ReferenceNode* head, ReferencesList::ReferenceNode* tail)
{
    DeleteStringNode* new_Node = new DeleteStringNode(head, tail);

    new_Node->prev = this->tail->prev;
    new_Node->next = this->tail;
    this->tail->prev->next = new_Node;
    this->tail->prev = new_Node;
    this->n_node++;

}

void ConcatStringList::DeleteStringList::updated()
{
    DeleteStringNode* curr = this->head->next;
    while(curr != this->tail)
    {
        if (curr->refHeadNode->getRefNum() == 0 && curr->refTailNode->getRefNum() == 0)
        {
            CharALNode* tmp = nullptr;
            if (curr->refHeadNode->del == false)
            {
                tmp = curr->refHeadNode->refNode;
                if (tmp->prev != nullptr)
                {
                    tmp->prev->next = nullptr;
                }

                while (tmp != curr->refTailNode->refNode && tmp != nullptr)
                {
                    CharALNode* del = tmp;
                    tmp = tmp->next;
                    delete del;
                }

                if (tmp != nullptr)
                {
                    if (tmp->next != nullptr)
                    {
                        tmp->next->prev = nullptr;
                    }
                    delete tmp;
                    curr->refTailNode->del = true;
                }

                curr->refHeadNode->del = true;
            }
            else if (curr->refTailNode->del == false)
            {
                tmp = curr->refTailNode->refNode;
                if (tmp->next != nullptr)
                {
                    tmp->next->prev = nullptr;
                }

                while (tmp->prev != nullptr)
                {
                    tmp = tmp->prev;
                    delete tmp->next;
                }
                delete tmp;
                curr->refTailNode->del = true;
            }
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            DeleteStringNode* delNode = curr;
            curr = curr->next;
            delete delNode;
            this->n_node--;
        }
        else 
        {
            curr = curr->next;
        }
    }
}

int ConcatStringList::DeleteStringList::size() const
{
    return this->n_node;
}

string ConcatStringList::DeleteStringList::totalRefCountsString() const
{
    string s = "TotalRefCounts[]";
    DeleteStringNode* tmp = this->head->next;
    string res;
    while (tmp != this->tail)
    {
        
        if (tmp->refHeadNode != tmp->refTailNode)
        {
            res.append(to_string(tmp->refHeadNode->getRefNum() + tmp->refTailNode->getRefNum()));
        }
        else
        {
            res.append(to_string(tmp->refHeadNode->getRefNum()));
        }
        res.append(",");
        tmp = tmp->next;
    }
    if (res.length() != 0)
    {
        s.pop_back();
        res.pop_back();
        s += res;
        s += "]";
    }
    return s;
}

ConcatStringList::DeleteStringList::~DeleteStringList()
{
    delete this->head;
    delete this->tail;
}
