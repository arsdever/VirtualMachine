#include "cpu.h"
#include <register>
#include <QString>
#include <ram>
#include "../debugger/debugger.h"

#define REGISTER_EXECUTER(__instruction, __sz1, __sz2, __sz3, __sz4)\
map.insert(CCPU::##__instruction## | (CCPU::BYTE << 8), QPair<quint8, CCPU::FunctionPointer>(##__sz1##, &CCPU::##__instruction##_exec<qint8>)); \
map.insert(CCPU::##__instruction## | (CCPU::WORD << 8), QPair<quint8, CCPU::FunctionPointer>(##__sz2##, &CCPU::##__instruction##_exec<qint16>)); \
map.insert(CCPU::##__instruction## | (CCPU::DOUBLE_WORD << 8), QPair<quint8, CCPU::FunctionPointer>(##__sz3##, &CCPU::##__instruction##_exec<qint32>)); \
//map.insert(CCPU::##__instruction | (CCPU::QUAD_WORD << 8), QPair<quint8, CCPU::FunctionPointer>(##__sz4##, &CCPU::##__instruction##_exec<qint64>)); 
#define REGISTER_EXECUTER1(__instruction, __sz1) REGISTER_EXECUTER(__instruction, __sz1, __sz1, __sz1, __sz1)

CCPU::InstructionMap CCPU::InitMap()
{
	InstructionMap map;
	map.insert(CCPU::NOP, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::NOP_exec));
	map.insert(CCPU::RET, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::RET_exec));
	map.insert(CCPU::BRK, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::BRK_exec));
	map.insert(CCPU::HLT, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::HLT_exec));
	map.insert(CCPU::CALL, QPair<quint8, CCPU::FunctionPointer>(5, &CCPU::CALL_exec));
	map.insert(CCPU::INT, QPair<quint8, CCPU::FunctionPointer>(2, &CCPU::CALL_exec));
	map.insert(CCPU::PUSH, QPair<quint8, CCPU::FunctionPointer>(2, &CCPU::PUSH_exec));
	map.insert(CCPU::POP, QPair<quint8, CCPU::FunctionPointer>(2, &CCPU::POP_exec));
	map.insert(CCPU::PUSHF, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::PUSHF_exec));
	map.insert(CCPU::POPF, QPair<quint8, CCPU::FunctionPointer>(1, &CCPU::POPF_exec));
	map.insert(CCPU::ASG, QPair<quint8, CCPU::FunctionPointer>(6, &CCPU::ASG_exec));
	map.insert(CCPU::LOD, QPair<quint8, CCPU::FunctionPointer>(3, &CCPU::LOD_exec));
	map.insert(CCPU::STR, QPair<quint8, CCPU::FunctionPointer>(3, &CCPU::STR_exec));
	map.insert(CCPU::MOV, QPair<quint8, CCPU::FunctionPointer>(3, &CCPU::MOV_exec));
	map.insert(CCPU::CMP, QPair<quint8, CCPU::FunctionPointer>(3, &CCPU::CMP_exec));
	map.insert(CCPU::JMP, QPair<quint8, CCPU::FunctionPointer>(5, &CCPU::JMP_exec));
	map.insert(CCPU::JMPC, QPair<quint8, CCPU::FunctionPointer>(6, &CCPU::JMPC_exec));
	map.insert(CCPU::INC, QPair<quint8, CCPU::FunctionPointer>(2, &CCPU::INC_exec));
	map.insert(CCPU::DEC, QPair<quint8, CCPU::FunctionPointer>(2, &CCPU::DEC_exec));
	REGISTER_EXECUTER1(NAND, 4)
	REGISTER_EXECUTER1(ADD, 4)
	REGISTER_EXECUTER1(SUB, 4)
	REGISTER_EXECUTER1(MUL, 4)
	REGISTER_EXECUTER1(DIV, 4)
	REGISTER_EXECUTER1(ADDS, 4)
	REGISTER_EXECUTER1(SUBS, 4)
	REGISTER_EXECUTER1(MULS, 4)
	REGISTER_EXECUTER1(DIVS, 4)
	REGISTER_EXECUTER1(SWP, 4)
	REGISTER_EXECUTER1(AND, 4)
	REGISTER_EXECUTER1(OR, 4)
	REGISTER_EXECUTER1(XOR, 4)
	REGISTER_EXECUTER1(NOR, 4)
	REGISTER_EXECUTER1(NOT, 3)
	return map;
}

CCPU::InstructionMap CCPU::s_mapInstructions = CCPU::InitMap();

CCPU::CCPU()
	: m_sState()
	, m_fptrExecuter(nullptr)
{
}

void CCPU::Fetch()
{
	CRAM* pRam = CRAM::instance();

	if (pRam == nullptr)
		throw ram_not_found_exception();

	*((quint64*)m_sState.IR) = pRam->operator[]<quint64>(m_sState.PC);
}

quint8 CCPU::Decode()
{
	qint32 opcode = ((quint8*)m_sState.IR)[0];
	switch(opcode)
	{
	case CCPU::NOP:
	case CCPU::RET:
	case CCPU::BRK:
	case CCPU::HLT:
	case CCPU::CALL:
	case CCPU::INT:
	case CCPU::PUSH:
	case CCPU::POP:
	case CCPU::PUSHF:
	case CCPU::POPF:
	case CCPU::ASG:
	case CCPU::LOD:
	case CCPU::STR:
	case CCPU::MOV:
	case CCPU::JMP:
	case CCPU::JMPC:
	case CCPU::INC:
	case CCPU::DEC:
		m_fptrExecuter = CCPU::s_mapInstructions[opcode].second;
		return CCPU::s_mapInstructions[opcode].first;
	}

	qint32 components = ((((quint8*)m_sState.IR)[1] & SIZE_MASK) << 8);
	m_fptrExecuter = CCPU::s_mapInstructions[components | opcode].second;
	return CCPU::s_mapInstructions[components | opcode].first;
}

void CCPU::Execute()
{
	std::invoke(m_fptrExecuter, this);
}
//
//void CCPU::UpdateRegisters(CRegisterSet const& newState)
//{
//	for (int i = 0; i < 64; ++i)
//		GetRegisters().SetValue<char>(i, newState.GetValue<char>(i));
//}
//
CCPU::SState CCPU::GetState() const
{
	return m_sState;
}

void CCPU::Run()
{
	while (m_sState.RUN)
	{
		Step();
	}
}

void CCPU::Step()
{
	if (m_sState.FLAGS & SState::IFlag)
		HandleInterrupt();

	Fetch();
	m_sState.PC += Decode();
	Execute();
}

void CCPU::HandleInterrupt()
{
	m_sState.STACK.push(m_sState.PC);
	m_sState.STACK.push(m_sState.FLAGS);
	m_sState.PC = m_sState.IVT[m_sState.INT];
	m_sState.SF = m_sState.SP;
	m_sState.FLAGS &= ~SState::IFlag;
}

// instruction implementation

template <typename INT_TYPE>
void CCPU::NAND_exec() {
}

void CCPU::NOP_exec() {
}

void CCPU::CALL_exec() {
	m_sState.SP -= 4;

	CRAM::instance()->operator[]<quint32>(m_sState.SP) = m_sState.PC;
	m_sState.PC = *(quint32*)(m_sState.IR + 1);
}

void CCPU::RET_exec() {
	m_sState.PC = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	m_sState.SP += 4;
}

void CCPU::INT_exec() {
	m_sState.FLAGS |= SState::IFlag;
	m_sState.INT = m_sState.IR[1];
}

void CCPU::BRK_exec() {
	throw breakpoint_exception();
}

void CCPU::HLT_exec() {
	m_sState.RUN = false;
}

void CCPU::PUSH_exec() {
	m_sState.SP -= 4;

	if (m_sState.IR[1] & 0x80)
	{
		if ((m_sState.IR[1] & 0x07) > 5)
			throw invalid_register_exception();

		CRAM::instance()->operator[]<quint32>(m_sState.SP) = m_sState.AR[m_sState.IR[1] & 0x07];
	}
	else
	{
		if ((m_sState.IR[2] & 0x3F) + 4 > 64)
			throw invalid_register_exception();

		CRAM::instance()->operator[]<quint32>(m_sState.SP) = *(quint32*)(m_sState.GR + m_sState.IR[1]);
	}
}

void CCPU::POP_exec() {
	if (m_sState.IR[1] & 0x80)
	{
		if ((m_sState.IR[1] & 0x07) > 5)
			throw invalid_register_exception();

		m_sState.AR[m_sState.IR[1] & 0x07] = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	}
	else
	{
		if ((m_sState.IR[2] & 0x3F) + 4 > 64)
			throw invalid_register_exception();

		(quint32&)m_sState.GR[m_sState.IR[1]] = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	}
	m_sState.SP += 4;
}

void CCPU::PUSHF_exec() {
	m_sState.SP -= 4;
	CRAM::instance()->operator[]<quint32>(m_sState.SP) = m_sState.FLAGS;
}

void CCPU::POPF_exec() {
	m_sState.FLAGS = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	m_sState.SP += 4;
}

template <typename INT_TYPE>
void CCPU::ADD_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 + o2;

	if (result < o1 || result < o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)(m_sState.GR[m_sState.IR[2]]) = result;
}

template <typename INT_TYPE>
void CCPU::SUB_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 - o2;

	if (result > o1 || result > o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::MUL_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 * o2;

	if (result > o1 || result > o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::DIV_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 / o2;

	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::ADDS_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 + o2;

	if ((o1 > 0 && o2 > 0) && (result < o1 || result < o2) ||
		(o1 < 0 && o2 < 0) && (result > o1 || result > o2))
		m_sState.FLAGS |= SState::OFlag;
	if (result < 0)
		m_sState.FLAGS |= SState::SFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::SUBS_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 - o2;

	if ((o1 > 0 && o2 > 0) && (result < o1 || result < o2) ||
		(o1 < 0 && o2 < 0) && (result > o1 || result > o2))
		m_sState.FLAGS |= SState::OFlag;
	if (result < 0)
		m_sState.FLAGS |= SState::SFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::MULS_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 * o2;

	if ((o1 > 0 && o2 > 0) && (result < o1 || result < o2) ||
		(o1 < 0 && o2 < 0) && (result > o1 || result > o2))
		m_sState.FLAGS |= SState::OFlag;
	if (result < 0)
		m_sState.FLAGS |= SState::SFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::DIVS_exec() {
	INT_TYPE o1 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[2]);
	INT_TYPE o2 = *(INT_TYPE*)(m_sState.GR + m_sState.IR[3]);
	INT_TYPE result = o1 / o2;

	if ((o1 > 0 && o2 > 0) && (result < o1 || result < o2) ||
		(o1 < 0 && o2 < 0) && (result > o1 || result > o2))
		m_sState.FLAGS |= SState::OFlag;
	if (result < 0)
		m_sState.FLAGS |= SState::SFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

void CCPU::MOV_exec() {
	if ((m_sState.IR[1] & 0x80)
		&& (m_sState.IR[2] & 0x80))
	{
		if ((m_sState.IR[1] & 0x07) > 5 || (m_sState.IR[2] & 0x07) > 5)
			throw invalid_register_exception();
		m_sState.AR[(m_sState.IR[1] & 0x07)] = m_sState.AR[(m_sState.IR[2] & 0x07)];
	}
	else if ((m_sState.IR[1] & 0x80)
		&& !(m_sState.IR[2] & 0x80))
	{
		if ((m_sState.IR[1] & 0x07) > 5 || ((m_sState.IR[2] & 0x3F) + 4) > 64)
			throw invalid_register_exception();
		m_sState.AR[m_sState.IR[1] & 0x07] = *(quint32*)(m_sState.GR + (m_sState.IR[2] & 0x3F));
	}
	else if (!(m_sState.IR[1] & 0x80)
		&& !(m_sState.IR[2] & 0x80))
	{
		if (((m_sState.IR[1] & 0x3F) + 4) > 64 || ((m_sState.IR[2] & 0x3F) + 4) > 64)
			throw invalid_register_exception();
		(quint32&)m_sState.GR[m_sState.IR[1] & 0x3F] = *(quint32*)(m_sState.GR + (m_sState.IR[2] & 0x3F));
	}
	else if (!(m_sState.IR[1] & 0x80)
		&& (m_sState.IR[2] & 0x80))
	{
		if (((m_sState.IR[1] & 0x3F) + 4) > 64 || (m_sState.IR[2] & 0x07) > 5)
			throw invalid_register_exception();
		(quint32&)m_sState.GR[m_sState.IR[1] & 0x3F] = m_sState.AR[m_sState.IR[2] & 0x07];
	}
}

void CCPU::ASG_exec() {
	if (m_sState.IR[1] & 0x80)
		m_sState.AR[m_sState.IR[1] & 0x07] = (quint32)m_sState.IR[2];
	else
		m_sState.GR[m_sState.IR[1] & 0x3F] = (quint32)m_sState.IR[2];
}

void CCPU::LOD_exec() {
	if(m_sState.IR[1] & 0x80)
		(quint32&)m_sState.AR[m_sState.IR[1] & 0x07] = (quint32)CRAM::instance()->operator[]<quint32>(m_sState.AR[m_sState.IR[2] & 0x07]);
	else
		(quint32&)m_sState.GR[m_sState.IR[1] & 0x3F] = (quint32)CRAM::instance()->operator[]<quint32>(m_sState.AR[m_sState.IR[2] & 0x07]);
}

void CCPU::STR_exec() {
	if (m_sState.IR[1] & 0x80)
		(quint32&)CRAM::instance()->operator[]<quint32>(m_sState.AR[m_sState.IR[2] & 0x07]) = *(quint32*)(m_sState.AR + (m_sState.IR[1] & 0x07));
	else
		(quint32&)CRAM::instance()->operator[]<quint32>(m_sState.AR[m_sState.IR[2] & 0x07]) = *(quint32*)(m_sState.GR + (m_sState.IR[1] & 0x3F));
}

void CCPU::CMP_exec() {
	switch (m_sState.IR[1] & 0xC0)
	{
	case BYTE:
		if ((quint8)m_sState.GR[m_sState.IR[1] & 0x3C] == (quint8)m_sState.GR[m_sState.IR[2] & 0x3C])
			m_sState.FLAGS = SState::ZFlag;
		else
			m_sState.FLAGS = 0;
		break;
	case WORD:
		if ((quint16)m_sState.GR[m_sState.IR[1] & 0x3C] == (quint16)m_sState.GR[m_sState.IR[2] & 0x3C])
			m_sState.FLAGS = SState::ZFlag;
		else
			m_sState.FLAGS = 0;
		break;
	case DOUBLE_WORD:
		if ((quint32)m_sState.GR[m_sState.IR[1] & 0x3C] == (quint32)m_sState.GR[m_sState.IR[2] & 0x3C])
			m_sState.FLAGS = SState::ZFlag;
		else
			m_sState.FLAGS = 0;
		break;
	case QUAD_WORD:
		break;/*
		if ((quint64)m_sState.GR[m_sState.IR[1] & 0x3C] == (quint64)m_sState.GR[m_sState.IR[2] & 0x3C])
			m_sState.FLAGS = SState::ZFlag;
		else
			m_sState.FLAGS = 0;*/
	}
}

void CCPU::JMPC_exec() {
	quint32 mask = (m_sState.IR[1] & InstructionComponents::CONDITIONAL_MASK);
	if ((mask == EQUAL) && (m_sState.FLAGS & m_sState.ZFlag) != 0)	
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == NOT_EQUAL) && (m_sState.FLAGS & m_sState.ZFlag) == 0)
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == ABOVE) &&
		((m_sState.FLAGS & m_sState.CFlag) == 0) && ((m_sState.FLAGS & m_sState.ZFlag) == 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == ABOVE_OR_EQUAL) && (m_sState.FLAGS & m_sState.CFlag) == 0)
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == BELOW) && (m_sState.FLAGS & m_sState.CFlag) != 0)
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == BELOW_OR_EQUAL) &&
		((m_sState.FLAGS & m_sState.CFlag) != 0) || ((m_sState.FLAGS & m_sState.ZFlag) != 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == GREATER) &&
		((m_sState.FLAGS & m_sState.ZFlag) == 0) && ((m_sState.FLAGS & m_sState.SFlag) == (m_sState.FLAGS & m_sState.OFlag)))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == GREATER_OR_EQUAL) &&
		((m_sState.FLAGS & m_sState.SFlag) == (m_sState.FLAGS & m_sState.OFlag)))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == LESS) &&
		((m_sState.FLAGS & m_sState.SFlag) != (m_sState.FLAGS & m_sState.OFlag)))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == LESS_OR_EQUAL) &&
		((m_sState.FLAGS & m_sState.ZFlag) == 0) && ((m_sState.FLAGS & m_sState.SFlag) != (m_sState.FLAGS & m_sState.OFlag)))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == OVERFLOW_SET) &&
		((m_sState.FLAGS & m_sState.OFlag) != 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == NOT_OVERFLOW) &&
		((m_sState.FLAGS & m_sState.OFlag) == 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == SIGN_SET) &&
		((m_sState.FLAGS & m_sState.SFlag) != 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == NOT_SIGN) &&
		((m_sState.FLAGS & m_sState.SFlag) == 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == PARITY_SET) &&
		((m_sState.FLAGS & m_sState.PFlag) != 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
	else if ((mask == NOT_PARITY) &&
		((m_sState.FLAGS & m_sState.PFlag) == 0))
		m_sState.PC = *(quint32*)(m_sState.IR + 2);
}

void CCPU::JMP_exec() {
	m_sState.PC = *(quint32*)(m_sState.IR + 1);
}

void CCPU::INC_exec() {
	if (m_sState.IR[1] & 0x80)
		++*(quint32*)(m_sState.AR[m_sState.IR[1] & 0x07]);
	else
		++*(quint32*)(m_sState.GR + (m_sState.IR[1] & 0x3F));
}

void CCPU::DEC_exec() {
	if (m_sState.IR[1] & 0x80)
		--*(quint32*)(m_sState.AR[m_sState.IR[1] & 0x07]);
	else
		--*(quint32*)(m_sState.GR + (m_sState.IR[1] & 0x3F));
}

template <typename INT_TYPE>
void CCPU::SWP_exec() {
}

template <typename INT_TYPE>
void CCPU::AND_exec() {
}

template <typename INT_TYPE>
void CCPU::XOR_exec() {
}

template <typename INT_TYPE>
void CCPU::NOT_exec() {
}

template <typename INT_TYPE>
void CCPU::NOR_exec() {
}

template <typename INT_TYPE>
void CCPU::OR_exec() {
}