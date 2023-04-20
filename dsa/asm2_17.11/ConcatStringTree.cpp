#include "ConcatStringTree.h"

//////////////////////////////////////////////////
//////////   HELPER_FUNCTION /////////////////////
//////////////////////////////////////////////////

int ConcatStringTree::stringlen(const char* s){
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

char* ConcatStringTree::strcopy(const char* s, int len) {
    char* str = new char[len + 1];
    for (int i = 0; i < len; i++) {
        str[i] = s[i];
    }
    str[len] = '\0';
    return str;
}

//////////////////////////////////////////////////
//////////   CONCAT_STRING_TREE //////////////////
//////////////////////////////////////////////////

ConcatStringTree::ConcatStringTree(const char* s) {
    if (s != nullptr) {
        int len = ConcatStringTree::stringlen(s);
        this->root = new StringNode(ConcatStringTree::strcopy(s, len), len, 0, nullptr, nullptr);
    }
    else {
        this->root = new StringNode(nullptr, 0, 0, nullptr, nullptr);
    }
    this->head = this->tail = this->root;
}

int ConcatStringTree::length() const {
    return this->root->len;
}

char ConcatStringTree::get(int index) {
    if (index < 0 || index > this->root->len-1)
        throw std::out_of_range("Index of string is invalid!");
    else{
        StringNode* tmp = this->root;
        while (tmp->data == nullptr) {
            if (index > tmp->leftlen - 1) {
                index = index-tmp->leftlen;
                tmp = tmp->right;
            }
            else {
                tmp = tmp->left;
            }
        }
        return tmp->data[index];
    }
}

int ConcatStringTree::indexOf(char c) {
    int i = 0;
    if (this->root->data != nullptr) {
        while (this->root->data[i] != '\0') {
            if (this->root->data[i] == c)
                return i;
            i++;
        }
    }
    else {
        StringNode* tmp = this->head;
        int len_substr = 0;
        for (; i < this->root->len; i++) {
            if (tmp->data[i-len_substr] == c)
                return i;
            else if (tmp->data[i] == '\0') {
                len_substr = i;
                i--;
                tmp = tmp->next; 
            }
        }
    }
    return -1;
}

string ConcatStringTree::GetPreOrderTraversal(StringNode* node) const {
    if (node != nullptr) {
        string result = "(LL=" + to_string(node->leftlen) + 
                        ",L=" + to_string(node->len) +
                        ",";
        if (node->data != nullptr) {
            result += string(node->data);
        }
        else {
            result += "<NULL>";
        }
        result += ");";

        return result + GetPreOrderTraversal(node->left) + GetPreOrderTraversal(node->right);
    }
    else return "";
}

string ConcatStringTree::toStringPreOrder () const {
    string res = this->GetPreOrderTraversal(this->root);
    res.pop_back();
    return res;
}

string ConcatStringTree::toString() const {
    StringNode* tmp = this->head;
    string result;
    int i = 0;
    while (i < this->root->len) {
        result += string(tmp->data);
        i+= tmp->len;
        tmp = tmp->next;
    }
    return result;
}

ConcatStringTree ConcatStringTree::concat(const ConcatStringTree & otherS) const {
    ConcatStringTree result((char*)(nullptr));
    result.root->left = this->root;
    result.root->right = otherS.root;
    result.root->leftlen = this->root->len;
    result.root->len = this->root->len + otherS.root->len;
    result.head = this->head;
    result.tail = otherS.tail;
    this->tail->next = otherS.head;

    return result;
}

void ConcatStringTree::NLR_Traversal_GetSubStr(StringNode* orgNode, StringNode* &cpyNode, int idx, int role) const{
    if (orgNode->data != nullptr){
        if (role == -1){
            cpyNode->data = ConcatStringTree::strcopy(orgNode->data + idx, orgNode->len - idx);
            cpyNode->len = orgNode->len - idx;
        }
        else if (role == 0){
            cpyNode->data = ConcatStringTree::strcopy(orgNode->data, orgNode->len);
            cpyNode->len = orgNode->len;
        }
        else{
            cpyNode->data = ConcatStringTree::strcopy(orgNode->data, idx);
            cpyNode->len = idx;
        }
        cpyNode->leftlen = 0;
    }
    else{
        if(role == -1){
            cpyNode = new StringNode(nullptr, orgNode->len, orgNode->len - idx, nullptr, nullptr);
            if (orgNode->leftlen > idx){
                NLR_Traversal_GetSubStr(orgNode->left, cpyNode->left, idx, -1);
                NLR_Traversal_GetSubStr(orgNode->right, cpyNode->right, idx, 0);
            }
            else {
                NLR_Traversal_GetSubStr(orgNode->right, cpyNode->right, idx - orgNode->leftlen, -1);
            }
        }
        else if (role == 0){
            cpyNode = new StringNode(nullptr, orgNode->len, orgNode->len, nullptr, nullptr);
            NLR_Traversal_GetSubStr(orgNode->left, cpyNode->left, idx, 0);
            NLR_Traversal_GetSubStr(orgNode->right, cpyNode->right, idx, 0);
        }
        else {
            cpyNode = new StringNode(nullptr, orgNode->len, idx - orgNode->leftlen, nullptr, nullptr);
            if (orgNode->leftlen < idx){
                NLR_Traversal_GetSubStr(orgNode->left, cpyNode->left, idx, 0);
                NLR_Traversal_GetSubStr(orgNode->right, cpyNode->right, idx-orgNode->leftlen, 1);
            }
            else {
                NLR_Traversal_GetSubStr(orgNode->left, cpyNode->left, idx, 1);
            }
        } 
    }
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
    // Check exception
    if (from < 0 || to > this->root->len)
        throw std::out_of_range("Index of string is invalid!");
    else if (from >= to)
        throw std::logic_error("Invalid range!");

    // Do
    StringNode* subTree = new StringNode(nullptr, 0, 0, nullptr, nullptr);
    StringNode* tmpSubTree = subTree;
    StringNode* tmpTree = this->root;
    
    // CASE FROM AND TO BELONG TO THE SAME LEAF
    while (1){
        if (tmpTree->data)
            break;

        if (from < tmpTree->leftlen && to <= tmpTree->leftlen){
            tmpSubTree->left = new StringNode(nullptr, to-from, to-from, nullptr, nullptr);
            tmpSubTree = tmpSubTree->left;
            tmpTree = tmpTree->left;
        }
        else if (from >= tmpTree->leftlen && to > tmpTree->leftlen){
            tmpSubTree->right = new StringNode(nullptr, 0, to-from, nullptr, nullptr);
            
            from -= tmpTree->leftlen;
            to -= tmpTree->leftlen;
            
            tmpSubTree = tmpSubTree->right;
            tmpTree = tmpTree->right;
        }
        else
            break;
    }
    
    // Only one node
    if (tmpTree->data != nullptr){
        char* substr = new char[to - from + 1];
        for (int i = 0 ; i < (to - from); i++){
            substr[i] = tmpTree->data[from + i];
        }
        substr[to - from] = '\0';
        tmpSubTree->data = substr;
        tmpSubTree->leftlen = 0;

        if (subTree->left)
            tmpSubTree = subTree->left;
        else if (subTree->right)
            tmpSubTree = subTree->right;
        
        ConcatStringTree res((char*)(nullptr));
        delete res.root;
        res.root = tmpSubTree;
        return res;
        
        if (tmpSubTree != subTree)
            delete subTree;
        
        return ConcatStringTree(tmpSubTree);
        cout << "here"<< endl;
    }

    // CASE FROM AND TO IN DIFFERENCE LEAF
    if (tmpSubTree == subTree){
        subTree->leftlen = tmpTree->leftlen-from;
        subTree->len = to - from;
    }
    NLR_Traversal_GetSubStr(tmpTree->left, tmpSubTree->left, from, -1);
    NLR_Traversal_GetSubStr(tmpTree->right, tmpSubTree->right, to, 1);

    if (subTree->left == nullptr){
        tmpSubTree = subTree->right;
        delete subTree;
        subTree = tmpSubTree;
        
    }
    else if (subTree->right == nullptr){
        tmpSubTree = subTree->left;
        delete subTree;
        subTree = tmpSubTree;
    }

    return ConcatStringTree(subTree);
}
/*
ConcatStringTree ConcatStringTree::reverse() const {

}*/

//////////////////////////////////////////////////
//////////   STRING_NODE   ///////////////////////
//////////////////////////////////////////////////

ConcatStringTree::StringNode::StringNode(char* data, int len, int leftlen, StringNode* left, StringNode* right) {
    this->len = len;
    this->data = data;
    this->leftlen = leftlen;
    this->left = left;
    this->right = right;
    this->next = nullptr;
}