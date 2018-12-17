//------------------------------------------------------------------------------------------------------------------
// ╔════════════════════════════╗
// ║ Register operand structure ║
// ╚════════════════════════════╝
// 
//            ┌─────────┬──── general purpose register number
//			  │   		│
// binary 0 1 0 0 0 1 0 1     extands for "general purpose register / uint8 type / number 5"
//		  │	│ │   │ │   │
//		  │ │ │   │ └───┴──── address register number
//		  │ │ └───┴────────── not used for address types
//		  │ └──────────────── operand type  (1-uint8 / 0-uint32) unused for address types
//        └────────────────── register type (1-ar / 0-gpr)
//------------------------------------------------------------------------------------------------------------------

#pragma once

#include "cpu_global.h"

#include <exception>

#include <QObject>
#include <QMap>
#include <QPair>

class CRAM;
class CIOController;

class CPU_EXPORT CCPU
{
public:
	struct SState
	{
		enum EFlags
		{
			CFlag = 1,
			ZFlag = 2,
			SFlag = 4,
			OFlag = 8,
			PFlag = 16,
			IFlag = 32
		};

		quint32 PC;
		quint8 IR[8];
		quint32 AR[8];
		quint8 GR[64];
		bool RUN;
		qint32 FLAGS;
		quint32& SF;
		quint32& SP;/*
		quint8 INT;
		QVector<qint32> IVT;*/

		SState()
			: PC(0)
			, IR{ 0 }
			, AR{ 0 }
			, GR{ 0 }
			, RUN(false)
			, FLAGS((EFlags)0)
			, SF(AR[0])
			, SP(AR[1])
		{
		}

		SState(SState const& c)
			: SF(AR[0])
			, SP(AR[1])
		{
			operator=(c);
		}

		SState& operator=(SState const& c)
		{
			PC = c.PC;
			(quint64&)IR = (quint64&)c.IR;

			for(int i = 0; i < 6; ++i)
				AR[i] = c.AR[i];

			for(int i = 0; i < 64; ++i)
				GR[i] = c.GR[i];

			RUN = c.RUN;
			FLAGS = c.FLAGS;
			return *this;
		}

	} m_sState;


public:
	CCPU(CRAM* pRam, CIOController* pIOController);
	CRAM* Ram() const { return m_pRam; }
	SState GetState() const;
	QString GetUUID() const;
	void Run();
	void Step();

	void Restart();

private:
	void Fetch();
	quint8 Decode();
	void Execute();
	void HandleInterrupt();

public:
	typedef void (CCPU::*FunctionPointer)();

	enum InstructionCode
	{
		NOP,//
		RET,//
		BRK,//
		INT,//
		HLT,//
		PUSH,//
		POP,//
		PUSHF,//
		POPF,//
		CALL,//
		ASG,//
		LOD,//
		STR,//
		CMP,//
		JMP,//
		JMPC,//
		INC,//
		DEC,//
		ADD,
		SUB,
		MUL,
		DIV,
		MOV,//
		SWP,
		AND,
		OR,
		XOR,
		NOT,
		NAND,
		NOR,
		ADDS,
		SUBS,
		MULS,
		DIVS,
		IN,
		OUT,
		OUT1,

		CONDOTIONAL = 0x80
	};

	enum InstructionComponents
	{
		BYTE = 0x01,
		WORD = 0x02,
		DOUBLE_WORD = 0x00,
		QUAD_WORD = 0x03,
		SIZE_MASK = 0x03,

		//CFlag = 1
		//ZFlag = 2
		//SFlag = 4
		//OFlag = 8
		//PFlag = 16
		//IFlag = 32

		EQUAL = 0x00,
		NOT_ZERO = EQUAL,
		NOT_EQUAL = 0x02,
		ZERO = NOT_EQUAL,
		ABOVE = 0x04,
		NOT_BELOW_OR_EQUAL = ABOVE,
		ABOVE_OR_EQUAL = 0x08,
		NOT_BELOW = ABOVE_OR_EQUAL,
		BELOW = 0x10,
		NOT_ABOVE_OR_EQUAL = BELOW,
		BELOW_OR_EQUAL = 0x14,
		NOT_ABOVE = BELOW_OR_EQUAL,
		GREATER = 0x18,
		NOT_LESS_OR_EQUAL = GREATER,
		GREATER_OR_EQUAL = 0x1C,
		NOT_LESS = GREATER_OR_EQUAL,
		LESS = 0x20,
		NOT_GREATER_OR_EQUAL = LESS,
		LESS_OR_EQUAL = 0x24,
		NOT_GREATER = LESS_OR_EQUAL,
		OVERFLOW_SET = 0x28,
		NOT_OVERFLOW = 0x2C,
		SIGN_SET = 0x30,
		NOT_SIGN = 0x34,
		PARITY_SET = 0x38,
		NOT_PARITY = 0x3C,
		CONDITIONAL_MASK = 0x3C,

		REGISTER_MASK1 = 0x40,
		REGISTER_MASK2 = 0x80
	};

	typedef QMap<int, QPair<quint8, CCPU::FunctionPointer>> InstructionMap;

	// Instruction executers declaration

	void NOP_exec();
	void RET_exec();
	void BRK_exec();
	void HLT_exec();
	void CALL_exec();
	void INT_exec();
	void PUSH_exec();
	void POP_exec();
	void PUSHF_exec();
	void POPF_exec();
	void ASG_exec();
	void LOD_exec();
	void STR_exec();
	void MOV_exec();
	void CMP_exec();
	void JMP_exec();
	void JMPC_exec();
	void INC_exec();
	void DEC_exec();
	void IN_exec();
	void OUT_exec();
	void OUT1_exec();
	template <typename INT_TYPE>
	void NAND_exec();
	template <typename INT_TYPE>
	void ADD_exec();
	template <typename INT_TYPE>
	void SUB_exec();
	template <typename INT_TYPE>
	void MUL_exec();
	template <typename INT_TYPE>
	void DIV_exec();
	template <typename INT_TYPE>
	void ADDS_exec();
	template <typename INT_TYPE>
	void SUBS_exec();
	template <typename INT_TYPE>
	void MULS_exec();
	template <typename INT_TYPE>
	void DIVS_exec();
	template <typename INT_TYPE>
	void SWP_exec();
	template <typename INT_TYPE>
	void AND_exec();
	template <typename INT_TYPE>
	void XOR_exec();
	template <typename INT_TYPE>
	void NOT_exec();
	template <typename INT_TYPE>
	void NOR_exec();
	template <typename INT_TYPE>
	void OR_exec();

	static InstructionMap InitMap();
	static InstructionMap s_mapInstructions;

private:
	FunctionPointer m_fptrExecuter;
	CRAM* m_pRam;
	CIOController* m_pController;
	QString m_strUUID;

public:
	struct cpu_exception : public std::exception {};
	struct breakpoint_exception : cpu_exception {};
	struct ram_not_found_exception : cpu_exception {};
	struct invalid_register_exception : cpu_exception {};
	struct invalid_function_call : cpu_exception {};
};