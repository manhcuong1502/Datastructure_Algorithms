#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

enum TypeCode {
    Invalid,
    insert_string,
    insert_number,
    assign_string,
    assign_number,
    assign_identifier,
    BEGIN,
    END,
    LOOKUP,
    PRINT,
    RPRINT
};

struct Instruction{
    TypeCode type;
    string identifier_name;
    string ins;
    Instruction *next;

    Instruction(): type(Invalid), identifier_name(""), ins(""), next(nullptr){ };
    Instruction(TypeCode type, string identifier_name, string ins){
        this->type = type;
        this->identifier_name = identifier_name;
        this->ins = ins;
        this->next = nullptr;
    }
};

Instruction* lsIns = nullptr;
Instruction* AddInstruction(string);

enum DataType{
    str,
    num
};

class SymbolTable
{
    class Identifier{
        string name;
        DataType type;
        bool print;
        int scope;

        Identifier* prevIdentifier;

        Identifier* next;
        Identifier* prev;

        friend class SymbolTable;
    public:
        Identifier(): name(""), type(str), print(true), scope(-1), prevIdentifier(nullptr), next(nullptr), prev(nullptr) {};
        Identifier(const string _name, DataType _type, int _scope, Identifier* _prevIdentifier, Identifier* _next = nullptr, Identifier* _prev = nullptr) :
                name (_name), type(_type), print(true), scope(_scope), prevIdentifier(_prevIdentifier), next(_next), prev(_prev) {};
    };

    Identifier* headList;
    Identifier* tailList;
    int scopeCurr;

public:
    SymbolTable() ;
    void clean();
    void run(string);
    void readFile(string);

    bool insert(const string, DataType, int);
    void remove(int);
    void Print();
    void rPrint();
    Identifier* lookup(string);
};

#endif