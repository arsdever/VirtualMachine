#pragma once

#include "cpu_global.h"
#include <pluginmgr>
#include <QMap>

class CCPUState;
class CRegisterSet;
class Executer;

class CPU_EXPORT CCPUManager : public CPluginManager
{

	Q_OBJECT

public:
	typedef void (*FunctionPointer)(CCPUState*);

	enum InstructionCode
	{
		NOP, BRK, RET, INT,
		ADD, SUB, MUL, DIV,
		MOV, ASG, LOD, STR,
		JMP, SWP, AND, OR,
		XOR, NOT, NAND, NOR,
		INC, DEC, JZ, JNZ,
		JG, JNG, JL, JNL,
		HLT
	};

public:
	CCPUManager();
	~CCPUManager();

	void Fetch();
	void Decode();
	void Execute();
	void Run();		
	void UpdateRegisters(CRegisterSet const& newState);
	CRegisterSet GetRegisters() const;

signals:
	void RegistersUpdated();

private:
	CCPUState* m_pCPUState;
	FunctionPointer m_fptrExecuter;

public:
	friend class Executor;
};
