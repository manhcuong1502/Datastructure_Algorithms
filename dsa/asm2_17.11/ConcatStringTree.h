#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree {
public:
    class StringNode;

private:
    StringNode* root;
    StringNode* head;
    StringNode* tail;

public:
    ConcatStringTree(const char * s);
    ConcatStringTree(StringNode* node): root(node), head(node), tail(node) {};
    int length() const;
    char get(int index);
    int indexOf(char c);
    string toStringPreOrder() const;
    string GetPreOrderTraversal(StringNode*) const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree & otherS) const;
    void NLR_Traversal_GetSubStr(StringNode* orgNode, StringNode* &cpyNode, int idx, int role) const ;
    ConcatStringTree subString(int from, int to) const;
    //ConcatStringTree reverse() const;

    //int getParTreeSize(const string & query) const;
    //string getParTreeStringPreOrder(const string & query) const;

public:
    class StringNode {
    private:
        char* data;
        int len;
        int leftlen;
        StringNode* left;
        StringNode* right;
        StringNode* next;
        
        friend class ConcatStringTree;

    public:
        StringNode(char*, int, int, StringNode*, StringNode*);
        //~StringNode();
    };

public:
    static int stringlen(const char* s);
    static char* strcopy(const char* s, int len);
};

/*
class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
    friend class LitStringHash;
};

class LitStringHash {
public:
    LitStringHash(const HashConfig & hashConfig);
    int getLastInsertedIndex() const;
    string toString() const;
};

class ReducedConcatStringTree  {
public:
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash);
    LitStringHash * litStringHash;
};
*/
#endif // __CONCAT_STRING_TREE_H__