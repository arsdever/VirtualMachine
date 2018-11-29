#pragma once

#include "cpu_global.h"
#include <QObject>
#include <QMap>
#include <QPair>
#include <QStack>
#include <QVector>
#include <exception>

class CPU_EXPORT CCPU : public QObject
{

	Q_OBJECT

public:
	struct SState
	{
		enum EFlags
		{
			CFlag = 0,
			ZFlag = 1,
			SFlag = 2,
			OFlag = 4,
			PFlag = 8,
			IFlag = 16
		};

		quint32 PC;
		quint8 IR[8];
		quint32 AR[8];
		quint8 GR[64];
		bool RUN;
		qint32 FLAGS;
		quint32& SP;
		quint32& SF;
		quint8 INT;
		QStack<qint32> STACK;
		QVector<qint32> IVT;

		SState()
			: PC(0)
			, IR{ 0 }
			, AR{ 0 }
			, GR{ 0 }
			, RUN(true)
			, FLAGS((EFlags)0)
			, SP(AR[0])
			, SF(AR[1])
		{}

		SState(SState const& c)
			: SP(AR[0])
			, SF(AR[1])
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
	CCPU();
	void Run();
	void Fetch();
	quint8 Decode();
	void Execute();
	SState GetState() const;
	void HandleInterrupt();
	//void UpdateRegisters(CRegisterSet const& newState);

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
		ADD,
		SUB,
		MUL,
		DIV,
		MOV,
		JMP,
		SWP,
		AND,
		OR,
		XOR,
		NOT,
		NAND,
		NOR,
		INC,
		DEC,
		ADDS,
		SUBS,
		MULS,
		DIVS,

		CONDOTIONAL = 0x80
	};

	enum InstructionComponents
	{
		BYTE = 0x01,
		WORD = 0x02,
		DOUBLE_WORD = 0x00,
		QUAD_WORD = 0x03,
		SIZE_MASK = 0x03,

		EQUAL = 0x00,
		ZERO = EQUAL,
		NOTEQUAL = 0x04,
		NOTZERO = NOTEQUAL,
		ABOVE = 0x08,
		NOT_BELOW_OR_EQUAL = ABOVE,
		ABOVE_OR_EQUAL = 0x0C,
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

	void NOP_exec();
	void RET_exec();
	void BRK_exec();
	void HLT_exec();
	void CALL_exec();
	void INT_exec();

	//------------------------------------------------------------------------------------------------------------------
	// For instructions PUSH/POP operand structure is
	//            ┌─────────┬──── general purpose register number
	//			  │   		│
	// binary 0 1 0 0 0 1 0 1     extands for "general purpose register / uint8 type / number 5"
	//		  │	│ │   │ │   │
	//		  │ │ │   │ └───┴──── address register number
	//		  │ │ └───┴────────── not used for address types
	//		  │ └──────────────── operand type  (1-uint8 / 0-uint32) unused for address types
	//        └────────────────── register type (1-ar / 0-gpr)
	//------------------------------------------------------------------------------------------------------------------

	void PUSH_exec();
	void POP_exec();
	void PUSHF_exec();
	void POPF_exec();
	void ASG_exec();
	void LOD_exec();
	void STR_exec();
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
	void MOV_exec();
	template <typename INT_TYPE>
	void JMP_exec();
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
	void INC_exec();
	template <typename INT_TYPE>
	void DEC_exec();
	template <typename INT_TYPE>
	void JNZ_exec();
	template <typename INT_TYPE>
	void JNG_exec();
	template <typename INT_TYPE>
	void JNL_exec();
	template <typename INT_TYPE>
	void JG_exec();
	template <typename INT_TYPE>
	void JL_exec();
	template <typename INT_TYPE>
	void JZ_exec();
	template <typename INT_TYPE>
	void OR_exec();

	static InstructionMap InitMap();
	static InstructionMap s_mapInstructions;

private:
	FunctionPointer m_fptrExecuter;

public:
	struct cpu_exception : std::exception {};
	struct breakpoint_exception : cpu_exception {};
	struct ram_not_found_exception : cpu_exception {};
	struct invalid_register_exception : cpu_exception {};
};