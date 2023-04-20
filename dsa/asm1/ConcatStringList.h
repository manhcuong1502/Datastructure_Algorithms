#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

int stringlen(const char*);
char* strcpy(const char*, int);

class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration
    class CharALNode;

public:
    static ReferencesList refList;
    static DeleteStringList delStrList;
    // TODO: may provide some attributes

    CharALNode* head;
    CharALNode* tail;
    int len;

public:
    ConcatStringList(const char *);
    ConcatStringList(CharALNode* _head, CharALNode* _tail, int _len): head(_head), tail(_tail), len(_len) {};
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    std::string toString() const;
    ConcatStringList concat(const ConcatStringList & otherS) const;
    ConcatStringList subString(int from, int to) const;
    ConcatStringList reverse() const;
    ~ConcatStringList();

public:
    class ReferencesList {
    public:
        class ReferenceNode;
        // TODO: may provide some attributes
        ReferenceNode* head;
        ReferenceNode* tail;
        int n_node;
        int zero_node;

    public:
        ReferencesList();
        ReferenceNode* add(CharALNode*, int);
        void reSorted(ReferenceNode*);
        void clear();
        int size() const;
        int refCountAt(int index) const;
        std::string refCountsString() const;
        ~ReferencesList();

        class ReferenceNode {
        private:
            int refNum;
            bool del;
            CharALNode* refNode;
            ReferenceNode* next;
            ReferenceNode* prev;

            friend class CharALNode;
            friend class ReferencesList;
            friend class DeleteStringList;

        public:
            ReferenceNode(CharALNode* _refNode, int _refNum, ReferenceNode* _prev = nullptr,
                        ReferenceNode* _next = nullptr) : refNode(_refNode), prev(_prev), next(_next), refNum(_refNum), del(false) {};
            void increase();
            void decrease();
            int getRefNum();
            void moveToTail();
            void moveToHead();
        }; 
    };

    class DeleteStringList {
        // TODO: may provide some attributes
    public:
        class DeleteStringNode;
    private:
        DeleteStringNode* head;
        DeleteStringNode* tail;
        int n_node;

    public:
        DeleteStringList();
        void add(ReferencesList::ReferenceNode*, ReferencesList::ReferenceNode*);
        void updated();
        int size() const;
        std::string totalRefCountsString() const;
        ~DeleteStringList();

    public:
        class DeleteStringNode{
            friend class ConcatStringList::DeleteStringList;
            ReferencesList::ReferenceNode* refHeadNode;
            ReferencesList::ReferenceNode* refTailNode;
            DeleteStringNode* next;
            DeleteStringNode* prev;
        public:
            DeleteStringNode(ReferencesList::ReferenceNode* head = nullptr, ReferencesList::ReferenceNode* tail = nullptr): 
                                                                            refHeadNode(head), refTailNode(tail), next(nullptr), prev(nullptr){};
        };
    };

    class CharALNode {
    private:
        char* CharArrayList;
        int length;
        CharALNode* next;
        CharALNode* prev;
        ReferencesList::ReferenceNode* refNode;

        friend class ConcatStringList;
        friend class ReferencesList::ReferenceNode;

    public:
        CharALNode(): CharArrayList(nullptr), next(nullptr), refNode(nullptr) {};
        CharALNode(const char*);
        void reverse();
        ~CharALNode();
    };
};

#endif // __CONCAT_STRING_LIST_H__