#pragma once

#include <QMap>
#include <cpumgr>

class Executer
{
private:
	Executer();

public:
	typedef QMap<CCPUManager::InstructionCode, CCPUManager::FunctionPointer> InstructionMap;

	static void NAND_exec(CCPUState* pState);
	static void NOP_exec(CCPUState* pState);
	static void RET_exec(CCPUState* pState);
	static void BRK_exec(CCPUState* pState);
	static void INT_exec(CCPUState* pState);
	static void ADD_exec(CCPUState* pState);
	static void SUB_exec(CCPUState* pState);
	static void MUL_exec(CCPUState* pState);
	static void DIV_exec(CCPUState* pState);
	static void MOV_exec(CCPUState* pState);
	static void ASG_exec(CCPUState* pState);
	static void LOD_exec(CCPUState* pState);
	static void STR_exec(CCPUState* pState);
	static void JMP_exec(CCPUState* pState);
	static void SWP_exec(CCPUState* pState);
	static void AND_exec(CCPUState* pState);
	static void XOR_exec(CCPUState* pState);
	static void NOT_exec(CCPUState* pState);
	static void NOR_exec(CCPUState* pState);
	static void INC_exec(CCPUState* pState);
	static void DEC_exec(CCPUState* pState);
	static void JNZ_exec(CCPUState* pState);
	static void JNG_exec(CCPUState* pState);
	static void JNL_exec(CCPUState* pState);
	static void HLT_exec(CCPUState* pState);
	static void JG_exec(CCPUState* pState);
	static void JL_exec(CCPUState* pState);
	static void JZ_exec(CCPUState* pState);
	static void OR_exec(CCPUState* pState);

	static InstructionMap InitMap();
	static InstructionMap s_mapInstructions;
};