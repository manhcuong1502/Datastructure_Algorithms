#include "VM.h"

int VM::readFile(string filename) {
	ifstream ifs;
	ifs.open(filename);
	int n = 0;
	if (ifs.fail()) return 0;
	while (!ifs.eof()) {
		char tmp[256];
		ifs.getline(tmp, 256, '\n');
		insMem[n].data = tmp;
		insMem[n].type = checkLoadingError(n);
		n++;
	}
	ifs.close();
	return n;
}

void VM::run(string filename)
{
	this->nIns = readFile(filename);
	
	while (IP < this->nIns) {
		IP++;
		Instruction* virMachine = nullptr;
		switch (insMem[IP-1].type) {
		case Arithmetic:
		{
			virMachine = new ArithProc(IP - 1, this);
			break;
		}
		case Comparision:
		{
			virMachine = new CmpProc(IP - 1, this);
			break;
		}
		case Logical:
		{
			virMachine = new LogicProc(IP - 1, this);
			break;
		}
		case LoadStore:
		{
			virMachine = new LdSProc(IP - 1, this);
			break;
		}
		case SeqControl:
		{
			virMachine = new CtrlProc(IP - 1, this);
			break;
		}
		case InOut:
		{
			virMachine = new InOutProc(IP - 1, this);
			break;
		}
		case Invalid:
			return;
		}
		if (virMachine != nullptr) {
			virMachine->execute();
			delete virMachine;
		}
	}
}

int VM::checkRegistor(int IP, string ins) {
	int i = ins.find('R');
	if (i == -1 || ins == "Return") return 0;
	ins.erase(0,i+1);
	int idx = stoi(ins);
	if (idx < 1 || idx > 15) throw InvalidOperand(IP);
	return i + 2;
}

VM::TypeIns VM::checkLoadingError(int IP) {
	string Arith = "^(Add|Minus|Mul|Div) ([a-zA-Z0-9]+?), ([a-zA-Z0-9\\.]+?)$";
	string Cmp = "^(CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE) ([a-zA-Z0-9]+?), ([a-zA-Z0-9\\.]+?)$";
	string Lgc = "^Not ([a-zA-Z0-9]+?)|(And|Or) ([a-zA-Z0-9]+?), ([a-zA-Z0-9\\.]+?)$";
	string LdS = "^(Move|Load|Store) ([a-zA-Z0-9]+?), ([a-zA-Z0-9\\.]+?)$";
	string Seq = "^Return|Halt|(Jump|Call) ([a-zA-Z0-9\\.]+?)|JumpIf ([a-zA-Z0-9]+?), ([a-zA-Z0-9\\.]+?)$";
	string IO = "^(Input|Output) ([a-zA-Z0-9\\.]+?)$";

	regex synArith(Arith);
	regex synCmp(Cmp);
	regex synLgc(Lgc);
	regex synLdS(LdS);
	regex synSeq(Seq);
	regex synIO(IO);

	TypeIns type = Invalid;

	if (regex_match(insMem[IP].data, synArith)) {
		regex syntax("^(Add|Minus|Mul|Div) R[0-9]{1,2}, (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = Arithmetic;
		else throw InvalidOperand(IP);//
	}
	else if (regex_match(insMem[IP].data, synCmp)) {
		regex syntax("^(CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE) R[0-9]{1,2}, (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = Comparision;
		else throw InvalidOperand(IP);
	}
	else if (regex_match(insMem[IP].data, synLgc)) {
		regex syntax("^Not R[0-9]{1,2}|(And|Or) R[0-9]{1,2}, (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = Logical;
		else throw InvalidOperand(IP);
	}
	else if (regex_match(insMem[IP].data, synLdS)) {
		regex syntax("^(Move|Load|Store) R[0-9]{1,2}, (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = LoadStore;
		else throw InvalidOperand(IP);
	}
	else if (regex_match(insMem[IP].data, synSeq)) {
		regex syntax("(Jump|Call) (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)|Return|Halt|JumpIf R[0-9]{1,2}, (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = SeqControl;//
		else throw InvalidOperand(IP);
	}
	else if (regex_match(insMem[IP].data, synIO)) {
		regex syntax("^Input R[0-9]{1,2}|Output (R[0-9]{1,2}|[0-9]+?A?|[0-9]+?\\.[0-9]+?|true|false)$");
		if (regex_match(insMem[IP].data, syntax)) type = InOut;
		else throw InvalidOperand(IP);
	}
	if (!type) throw InvalidInstruction(IP);
	int r1 = checkRegistor(IP, insMem[IP].data);
	if (r1 != 0) checkRegistor(IP, insMem[IP].data.substr(r1, insMem[IP].data.length() - r1));
	return type;
}

VM::typeData VM::Instruction::CheckOperand(string op) {
	regex _Int("^[0-9]+$");
	regex _Float("^[0-9]+?\\.[0-9]+?$");
	regex _bool("^true|false$");
	regex _Address("^[0-9]+?A$");
	regex _Register("^R[0-9]{1,2}$");
	if (regex_match(op, _Int)) return Int;
	else if (regex_match(op, _Float)) return Float;
	else if (regex_match(op, _bool)) return Boolean;
	else if (regex_match(op, _Address)) return Address;
	else if (regex_match(op, _Register)) return Registor;
	else return InvalidData;
}

int VM::Instruction::getOper(regex* ls, int nO) {
	int operator_ = -1;
	for (int i = 0; i < nO; i++) {
		if (regex_match(ins, ls[i])) operator_ = i;
	}
	int i = ins.find(' ');
	ins.erase(0, i + 2);
	i = ins.find(' ');
	dest = &(vm->reg[stoi(ins)]);
	op1 = vm->reg[stoi(ins)];
	if (i != -1) {
		ins.erase(0, i + 1);
		op2 = ins;
		if (CheckOperand(op2) == Registor) {
			op2 = vm->reg[stoi(op2.substr(1, op2.length() - 1))];
		}
	}
	if (op1 == "") op1 = "0";


	if (!CheckOperand(op1) || !CheckOperand(op2)) throw TypeMismatch(IP);
	return operator_;
}

VM::Instruction::~Instruction() {
	dest = NULL;
	vm = NULL;
}
