#pragma once

#include "register.h"
#include "cpu_global.h"
#include <interfaces>
#include <macros>

class CPU_EXPORT CCPUState
{
	REGISTER_INTERFACE(CCPUState, Register)
	REGISTER_INTERFACE_END(Register)

public:
	enum Flags
	{
		CF = 0,
		ZF = 1,
		SF = 2,
		OF = 3
	};
	
public:
	CCPUState();
	~CCPUState();

	CRegisterSet& GetRegisters() { return m_baRegister; }
	int& RegIP() { return m_nInstructionPointer; }
	void* RegIR() { return m_pInstructionRegister; }
	Flags& RegFlags() { return m_eFlags; }

private:
	CRegisterSet m_baRegister;
	int m_nInstructionPointer;
	char m_pInstructionRegister[10];
	Flags m_eFlags;
public:
	bool m_bHalt;
};

