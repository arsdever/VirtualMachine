#include "executer.h"
#include "cpu_state.h"

Executer::InstructionMap Executer::InitMap()
{
	InstructionMap map;
	map.insert(CCPUManager::NAND, &Executer::NAND_exec);
	map.insert(CCPUManager::NOP, &Executer::NOP_exec);
	map.insert(CCPUManager::RET, &Executer::RET_exec);
	map.insert(CCPUManager::BRK, &Executer::BRK_exec);
	map.insert(CCPUManager::INT, &Executer::INT_exec);
	map.insert(CCPUManager::ADD, &Executer::ADD_exec);
	map.insert(CCPUManager::SUB, &Executer::SUB_exec);
	map.insert(CCPUManager::MUL, &Executer::MUL_exec);
	map.insert(CCPUManager::DIV, &Executer::DIV_exec);
	map.insert(CCPUManager::MOV, &Executer::MOV_exec);
	map.insert(CCPUManager::ASG, &Executer::ASG_exec);
	map.insert(CCPUManager::LOD, &Executer::LOD_exec);
	map.insert(CCPUManager::STR, &Executer::STR_exec);
	map.insert(CCPUManager::JMP, &Executer::JMP_exec);
	map.insert(CCPUManager::SWP, &Executer::SWP_exec);
	map.insert(CCPUManager::AND, &Executer::AND_exec);
	map.insert(CCPUManager::XOR, &Executer::XOR_exec);
	map.insert(CCPUManager::NOT, &Executer::NOT_exec);
	map.insert(CCPUManager::NOR, &Executer::NOR_exec);
	map.insert(CCPUManager::INC, &Executer::INC_exec);
	map.insert(CCPUManager::DEC, &Executer::DEC_exec);
	map.insert(CCPUManager::JNZ, &Executer::JNZ_exec);
	map.insert(CCPUManager::JNG, &Executer::JNG_exec);
	map.insert(CCPUManager::JNL, &Executer::JNL_exec);
	map.insert(CCPUManager::HLT, &Executer::HLT_exec);
	map.insert(CCPUManager::JG, &Executer::JG_exec);
	map.insert(CCPUManager::JL, &Executer::JL_exec);
	map.insert(CCPUManager::JZ, &Executer::JZ_exec);
	map.insert(CCPUManager::OR, &Executer::OR_exec);
	return map;
}

Executer::InstructionMap Executer::s_mapInstructions = Executer::InitMap();

void Executer::NAND_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::NOP_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::RET_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::BRK_exec(CCPUState* pState) {
	pState->m_bHalt = true;
	pState->RegIP() += 1;
}
void Executer::INT_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::ADD_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint8 OPERAND2 = ((qint8*)pState->RegIR())[2];
	qint32 OP1Value = pState->GetRegisters().GetValue<qint32>(OPERAND1);
	qint32 OP2Value = pState->GetRegisters().GetValue<qint32>(OPERAND2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, OP1Value + OP2Value);
	pState->RegIP() += 3;
}
void Executer::SUB_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint8 OPERAND2 = ((qint8*)pState->RegIR())[2];
	qint32 OP1Value = pState->GetRegisters().GetValue<qint32>(OPERAND1);
	qint32 OP2Value = pState->GetRegisters().GetValue<qint32>(OPERAND2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, OP1Value - OP2Value);
	pState->RegIP() += 3;
}
void Executer::MUL_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint8 OPERAND2 = ((qint8*)pState->RegIR())[2];
	qint32 OP1Value = pState->GetRegisters().GetValue<qint32>(OPERAND1);
	qint32 OP2Value = pState->GetRegisters().GetValue<qint32>(OPERAND2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, qint32(OP1Value * OP2Value));
	pState->RegIP() += 3;
}
void Executer::DIV_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint8 OPERAND2 = ((qint8*)pState->RegIR())[2];
	qint32 OP1Value = pState->GetRegisters().GetValue<qint32>(OPERAND1);
	qint32 OP2Value = pState->GetRegisters().GetValue<qint32>(OPERAND2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, qint32(OP1Value / OP2Value));
	pState->RegIP() += 3;
}
void Executer::MOV_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint32 OPERAND2 = *(qint32*)((qint8*)pState->RegIR() + 2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, OPERAND2);
	pState->RegIP() += 6;
}
void Executer::ASG_exec(CCPUState* pState) {
	qint8 OPERAND1 = ((qint8*)pState->RegIR())[1];
	qint32 OPERAND2 = *(qint32*)((qint8*)pState->RegIR() + 2);
	pState->GetRegisters().SetValue<qint32>(OPERAND1, OPERAND2);
	pState->RegIP() += 6;
}
void Executer::LOD_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::STR_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JMP_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::SWP_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::AND_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::XOR_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::NOT_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::NOR_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::INC_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::DEC_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JNZ_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JNG_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JNL_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::HLT_exec(CCPUState* pState) {
	pState->m_bHalt = true;
	pState->RegIP() += 1;
}
void Executer::JG_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JL_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::JZ_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}
void Executer::OR_exec(CCPUState* pState) {
	pState->RegIP() += 1;
}