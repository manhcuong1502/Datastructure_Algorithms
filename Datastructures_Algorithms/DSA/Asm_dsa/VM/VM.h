#ifndef VM_H
#define VM_H

#include "main.h"

class VM
{
public:
	class Instruction;
	class ArithProc;
	class CmpProc;
	class LogicProc;
	class LdSProc;
	class CtrlProc;
	class InOutProc;

	enum TypeIns {
		Arithmetic = 1,
		Comparision = 2,
		Logical = 3,
		LoadStore = 4,
		SeqControl = 5,
		InOut = 6,
		Invalid = 0
	};

	enum typeData {
		InvalidData,
		Int = 1,
		Float = 2,
		Boolean = 3,
		Address = 4,
		Registor = 5
	};
	
	struct Memory{
		TypeIns type = Invalid;
		string data = "";
		friend class VM;
	};
	
private:
	Memory insMem[65536];    // Declare instruction memory
	string staMem[65536]; // Declare static memory
	string reg[16]; // Declare 16 registers
	int IP;
	int nIns;
	vector<int> insAdd; // Declare stack storage address of instructions;
	
public:
	VM () : IP(0), nIns(0){};
	int readFile(string);
	void run(string);
	TypeIns checkLoadingError(int);
	int checkRegistor(int, string);

	class Instruction {
	protected:
		string op1;
		string op2;
		string* dest;
		string ins;
		int IP;
		VM* vm;
		friend class VM;
	public:
		Instruction(): op1(""), op2(""), dest(NULL), ins(""), IP(0), vm(NULL){};
		virtual typeData CheckOperand(string);
		virtual void Calculate() = 0;
		virtual int getOper(regex*, int);
		virtual void execute() {Calculate();}
		virtual ~Instruction();
	};

	class ArithProc: public Instruction {
	public:
		ArithProc(int IP, VM* vm) {
			this->op1 = "0";
			this->op2 = "0";
			this->dest = NULL;
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
		}
		void Calculate() {
			regex ls[4];
			ls[0].assign("^Add .+?$");
			ls[1].assign("^Minus .+?$");
			ls[2].assign("^Mul .+?$");
			ls[3].assign("^Div .+?$");
			int operator_ = getOper(ls, 4);
			typeData dataOp1 = CheckOperand(op1);
			typeData dataOp2 = CheckOperand(op2);
			if ((dataOp1 != Int && dataOp1 != Float) ||
				(dataOp2 != Int && dataOp2 != Float)) throw TypeMismatch(IP);
			typeData typeResult;
			if (dataOp1 * dataOp2 == 1) typeResult = Int;
			else typeResult = Float;
			double result = 0;
			double a = stod(op1);
			double b = stod(op2);
			switch (operator_) {
			case 0: 
			{
				result = a + b;
				break;
			}
			case 1:
			{
				result = a - b;
				break;
			}
			case 2:
			{
				result = a * b;
				break;
			}
			case 3:
			{
				if (b == 0) throw DivideByZero(IP);
				result = a / b;
				break;
			}
			}
			if (typeResult == Int) *dest = to_string(static_cast <int> (result));
			else *dest = to_string(static_cast <float>(result));
		}
	};

	class CmpProc: public Instruction {
	public: 
		CmpProc(int IP, VM* vm) {
			this->op1 = "0";
			this->op2 = "0";
			this->dest = NULL;
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
		}
		void Calculate() {
			regex ls[6];
			ls[0].assign("^CmpEQ .+?$");
			ls[1].assign("^CmpNE .+?$");
			ls[2].assign("^CmpLT .+?$");
			ls[3].assign("^CmpLE .+?$");
			ls[4].assign("^CmpGT .+?$");
			ls[5].assign("^CmpGE .+?$");
			int operator_ = getOper(ls, 6);
			typeData dataOp1 = CheckOperand(op1);
			typeData dataOp2 = CheckOperand(op2);
			if (dataOp1 == Boolean && dataOp2 == Boolean) {
				op1 = (op1 == "true") ? "1" : "0";
				op2 = (op2 == "true") ? "1" : "0";
			}
			else if ((dataOp1 != Int && dataOp1 != Float) ||
				(dataOp2 != Int && dataOp2 != Float)) throw TypeMismatch(IP);

			double a = stod(op1);
			double b = stod(op2);
			switch (operator_) {
			case 0:
			{
				if (a == b) *dest = "true";
				else *dest = "false";
				break;
			}
			case 1:
			{
				if (a != b) *dest = "true";
				else *dest = "false";
				break;
			}
			case 2:
			{
				if (a < b) *dest = "true";
				else *dest = "false";
				break;
			}
			case 3:
			{
				if (a <= b) *dest = "true";
				else *dest = "false";
				break;
			}
			case 4:
			{
				if (a > b) *dest = "true";
				else *dest = "false";
				break;
			}
			case 5:
			{
				if (a >= b) *dest = "true";
				else *dest = "false";
				break;
			}
			}
		}
	};

	class LogicProc : public Instruction {
	public:
		LogicProc(int IP, VM* vm) {
			this->op1 = "true";
			this->op2 = "true";
			this->dest = NULL;
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
		}
		void Calculate() {
			regex ls[3];
			ls[0].assign("^Not .+?$");
			ls[1].assign("^Or .+?$");
			ls[2].assign("^And .+?$");
			int operator_ = getOper(ls, 3);
			typeData dataOp1 = CheckOperand(op1);
			typeData dataOp2 = CheckOperand(op2);
			if (dataOp1 != Boolean || dataOp2 != Boolean) throw TypeMismatch(IP);
			op1 = (op1 == "true") ? "1" : "0";
			op2 = (op2 == "true") ? "1" : "0";
			switch (operator_) {
			case 0:
			{
				*dest = (op1 == "1") ? "false" : "true";
				break;
			}
			case 1:
			{
				*dest = (stoi(op1) || stoi(op2)) ? "true" : "false";
				break;
			}
			case 2:
			{
				*dest = (stoi(op1) && stoi(op2)) ? "true" : "false";
				break;
			}
			}
			//cout << *dest;
		}
	};

	class LdSProc :public Instruction {
	public:
		LdSProc(int IP, VM* vm) {
			this->op1 = "0";
			this->op2 = "0";
			this->dest = NULL;
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
		}
		void Calculate() {
			regex ls[3];
			ls[0].assign("^Move .+?$");
			ls[1].assign("^Load .+?$");
			ls[2].assign("^Store .+?$");
			int operator_ = getOper(ls, 3);
			typeData dataOp1 = CheckOperand(op1);
			typeData dataOp2 = CheckOperand(op2);
			
			switch (operator_) {
			case 0:
			{
				*dest = op2;
				break;
			}
			case 1:
			{
				if (dataOp2 != Address) throw TypeMismatch(IP);
				*dest = vm->staMem[stoi(op2)];
				break;
			}
			case 2:
			{
				if (dataOp1 != Address) throw TypeMismatch(IP);
				dest = &vm->staMem[stoi(op1)];
				*dest = op2;
				break;
			}
			}
		}
	};

	class CtrlProc : public Instruction {
	public:
		CtrlProc(int IP, VM* vm) {
			this->op1 = "true";
			this->op2 = "true";
			this->dest = NULL;
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
		}
		void Calculate() { return; }
		void execute() {
			int op = -1;
			regex ls[5];
			ls[0].assign("^Jump ([0-9]+?A|R[0-9]{1,2})$");
			ls[1].assign("^JumpIf .+?, [0-9]+?A$");
			ls[2].assign("^Call ([0-9]+?A|R[0-9]{1,2})$");
			ls[3].assign("^Return$");
			ls[4].assign("^Halt$");
			for (int i = 0; i < 5; i++) {
				if (regex_match(ins, ls[i])) op = i;
			}
			switch (op) {
			case 0:
			{
				int i = ins.find(' ');
				ins.erase(0, i + 1);
				if (CheckOperand(ins) != Address) throw TypeMismatch(IP);
				int _IP = stoi(ins);
				vm->IP = (_IP > -1 && _IP < (vm->nIns)) ? _IP : throw InvalidDestination(IP);
				break;
			}
			case 1:
			{
				int i = ins.find(' ');
				ins.erase(0, i + 2);
				i = ins.find(' ');
				op1 = vm->reg[stoi(ins)];
				ins.erase(0, i + 1);
				op2 = ins;
				if (CheckOperand(op2) == Registor) {
					op2 = vm->reg[stoi(op2.substr(1, op2.length() - 1))];
				}
				typeData dataOp1 = CheckOperand(op1);
				typeData dataOp2 = CheckOperand(op2);
				if (dataOp1 != Boolean || dataOp2 != Address) throw TypeMismatch(IP);
				int addr = stoi(op2);
				vm->IP = (op1 == "true") ? (addr > -1 && addr < vm->nIns)? addr: throw InvalidDestination(IP) : vm->IP;
				break;
			}
			case 2:
			{
				int i = ins.find(' ');
				ins.erase(0, i + 1);
				if (CheckOperand(ins) == Registor) {
					ins = vm->reg[stoi(ins.substr(1, ins.length() - 1))];
				}
				
				if (CheckOperand(ins) != Address) throw TypeMismatch(IP);
				if (vm->insAdd.size() > 1000) throw StackFull(IP);
				vm->insAdd.push_back(vm->IP);
				vm->IP = stoi(ins);
				break;
			}
			case 3:
			{
				
				vm->IP = vm->insAdd.back();
				vm->insAdd.pop_back();
				break;
			}
			case 4:
			{
				vm->IP = vm->nIns + 1;
				break;
			}
			}
		}
	};

	class InOutProc : public Instruction {
	public:
		InOutProc(int IP, VM* vm) {
			this->vm = vm;
			this->IP = IP;
			this->ins = vm->insMem[IP].data;
			dest = NULL;
		}
		void Calculate() { return; }
		void execute() {
			int i = ins.find(' ');
			if (ins.substr(0, i) == "Input") {
				dest = &(vm->reg[stoi(ins.substr(i + 2, ins.length() - i - 2))]);
				cin >> *dest;
				typeData inData = CheckOperand(*dest);
				if (inData != Int && inData != Float && inData != Boolean) throw InvalidInput(IP);
			}
			else {
				ins = ins.substr(i + 1, ins.length() - i - 1);
				if (CheckOperand(ins) == Registor) {
					ins = vm->reg[stoi(ins.substr(1, ins.length() - 1))];
				}
				cout << ins;
			}
		}
	};
};
#endif