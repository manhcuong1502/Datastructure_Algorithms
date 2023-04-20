#include "SymbolTable.h"

Instruction* AddInstruction(string ins){
    regex DECSTR("^INSERT [a-z]\\w* string$");
    regex DECINT("^INSERT [a-z]\\w* number$");
    regex ASGSTR("^ASSIGN [a-z]\\w* '[0-9a-zA-Z\\s]+'$");
    regex ASGINT("^ASSIGN [a-z]\\w* [0-9]+?$");
    regex ASGIDF("^ASSIGN [a-z]\\w* [a-z]\\w*?$");
    regex OPENBRAC("^BEGIN$");
    regex CLOSEBRAC("^END$");
    regex FIND("^LOOKUP [a-z]\\w*?$");
    regex PRT("^PRINT$");
    regex RPRT("^RPRINT$");

    TypeCode type = Invalid;
    string identifier_name = "";

    if (regex_match(ins, DECINT)){
        type = insert_number;
        identifier_name = ins.substr(ins.find(' ') + 1, ins.find_last_of(' ') - ins.find(' ') - 1);
    }
    else if (regex_match(ins, DECSTR)){
        type = insert_string;
        identifier_name = ins.substr(ins.find(' ') + 1, ins.find_last_of(' ') - ins.find(' ') - 1);
    }
    else if (regex_match(ins, ASGSTR)){
        type = assign_string;
        identifier_name = ins.substr(ins.find(' ') + 1, ins.find('\'') - ins.find(' ') - 2);
    }
    else if (regex_match(ins, ASGINT)){
        type = assign_number;
        identifier_name = ins.substr(ins.find(' ') + 1, ins.find_last_of(' ') - ins.find(' ') - 1);
    }
    else if (regex_match(ins, ASGIDF)){
        type = assign_identifier;
        identifier_name = ins.substr(ins.find(' ') + 1, ins.find_last_of(' ') - ins.find(' ') - 1);
    }
    else if (regex_match(ins, OPENBRAC)){
        type = BEGIN;
    }
    else if (regex_match(ins, CLOSEBRAC)){
        type = END;
    }
    else if (regex_match(ins, FIND)){
        type = LOOKUP;
        identifier_name = ins.substr(ins.find(' ') + 1);
    }
    else if(regex_match(ins, PRT)){
        type = PRINT;
    }
    else if(regex_match(ins, RPRT)){
        type = RPRINT;
    }
    return new Instruction(type, identifier_name, ins);
}

void ReleaseInstruction(){
    while(lsIns != nullptr){
        Instruction* tmp = lsIns;
        lsIns = lsIns->next;
        delete tmp;
    }
}

void SymbolTable::readFile(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    if (!ifs.good()) return;
    lsIns = new Instruction();
    Instruction* tmp = lsIns;
    while (!ifs.eof()) {
        string ins;
        getline(ifs, ins);
        tmp->next = AddInstruction(ins);
        tmp = tmp->next;
    }
    ifs.close();
}

void SymbolTable::run(string filename)
{
    readFile(filename);
    Instruction* tmp = lsIns;
    while (tmp->next != nullptr){

        switch(tmp->next->type){
            case insert_string:
            {
                if (this->insert(tmp->next->identifier_name, str, this->scopeCurr))
                    cout << "success\n";
                else
                    throw Redeclared(tmp->next->ins);
                break;
            }
            case insert_number:
            {
                if (this->insert(tmp->next->identifier_name, num, this->scopeCurr))
                    cout << "success\n";
                else
                    throw Redeclared(tmp->next->ins);
                break;
            }
            case assign_number:
            {
                Identifier* Obj = lookup(tmp->next->identifier_name);
                if (Obj == nullptr)
                    throw Undeclared(tmp->next->ins);
                else if (Obj->type != num)
                    throw TypeMismatch(tmp->next->ins);
                cout << "success\n";
                break;
            }
            case assign_string:
            {
                Identifier* Obj = lookup(tmp->next->identifier_name);
                if (Obj == nullptr)
                    throw Undeclared(tmp->next->ins);
                else if (Obj->type != str)
                    throw TypeMismatch(tmp->next->ins);
                cout << "success\n";
                break;
            }
            case assign_identifier:
            {
                Identifier* ObjL = lookup(tmp->next->identifier_name);
                Identifier* ObjR = lookup((tmp->next->ins).substr((tmp->next->ins).find_last_of(' ') + 1));
                if (ObjL == nullptr || ObjR == nullptr)
                    throw Undeclared(tmp->next->ins);
                else if (ObjL->type != ObjR->type)
                    throw TypeMismatch(tmp->next->ins);
                cout << "success\n";
                break;
            }
            case BEGIN:
            {
                this->scopeCurr++;
                break;
            }
            case END:
            {
                if(this->scopeCurr - 1 == -1)
                    throw UnknownBlock();
                this->remove(this->scopeCurr--);
                break;
            }
            case LOOKUP:
            {
                Identifier* find = lookup(tmp->next->identifier_name);
                if (find == nullptr)
                    throw Undeclared(tmp->next->ins);
                else
                    cout << find->scope << endl;
                break;
            }
            case PRINT:
            {
                this->Print();
                break;
            }
            case RPRINT:
            {
                this->rPrint();
                break;
            }
            case Invalid:
            {
                throw InvalidInstruction(tmp->next->ins);
            }
        }

        tmp = tmp->next;
    }

    if(this->scopeCurr != 0)
        throw UnclosedBlock(this->scopeCurr);
    ReleaseInstruction();
    this->clean();
}

SymbolTable::SymbolTable(){
    this->scopeCurr = 0;
    this->headList = new Identifier();
    this->tailList = new Identifier();
    this->headList->next = this->tailList;
    this->tailList->prev = this->headList;
}
void SymbolTable::clean(){
    while(this->scopeCurr > -1){
        this->remove(this->scopeCurr);
        this->scopeCurr --;
    }
    delete this->headList;
    delete this->tailList;
}

bool SymbolTable::insert(const string name, DataType type, int scope){

    Identifier* prevIdentifier = nullptr;
    prevIdentifier = lookup(name);
    if (prevIdentifier != nullptr) {
        if (prevIdentifier->scope == scope)
            return false;
        prevIdentifier->print = false;
    }

    this->tailList->prev->next = new Identifier(name, type, scope, prevIdentifier, this->tailList, this->tailList->prev);
    this->tailList->prev = this->tailList->prev->next;
    return true;
}

void SymbolTable::remove(int scope) {
    Identifier* tmp = this->tailList;
    while (tmp->prev->scope == scope){
        if (tmp->prev->prevIdentifier != nullptr)
            tmp->prev->prevIdentifier->print = true;
        tmp->prev = tmp->prev->prev;
        delete tmp->prev->next;
        tmp->prev->next = tmp;
    }
}

SymbolTable::Identifier* SymbolTable::lookup(string name){
    int scope = -1;
    Identifier* Obj = nullptr;

    Identifier* tmp = this->headList;
    while(tmp != this->tailList){
        if (tmp->next->name == name && tmp->next->scope > scope){
            scope = tmp->next->scope;
            Obj = tmp->next;
        }
        tmp = tmp->next;
    }
    return Obj;
}

void SymbolTable::Print() {
    string out = "";
    Identifier* tmp = this->headList;
    while(tmp->next != this->tailList){
        if (tmp->next->print == true)
            out += (tmp->next->name + "//" + to_string(tmp->next->scope) + " ");
        tmp = tmp->next;
    }
    if (out != "") {
        out.pop_back();
        cout << out << endl;
    }
}

void SymbolTable::rPrint(){
    string out = "";
    Identifier* tmp = this->tailList;
    while(tmp->prev != this->headList){
        if (tmp->prev->print == true)
            out += (tmp->prev->name + "//" + to_string(tmp->prev->scope) + " ");
        tmp = tmp->prev;
    }
    if(out != "") {
        out.pop_back();
        cout << out << endl;
    }
}
