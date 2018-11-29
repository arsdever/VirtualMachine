#include <cpu>
#include <ram>

template <typename INT_TYPE>
void CCPU::NAND_exec() {
}

template <typename INT_TYPE>
void CCPU::NOP_exec() {
}

template <typename INT_TYPE>
void CCPU::RET_exec() {
}

template <typename INT_TYPE>
void CCPU::BRK_exec() {
	//m_pBreakpoint->HandleBreakpoint();
}

template <typename INT_TYPE>
void CCPU::INT_exec() {
	m_sState.FLAGS |= SState::IFlag;
	m_sState.INT = m_sState.IR[1];
}

template <typename INT_TYPE>
void CCPU::ADD_exec() {
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
	INT_TYPE result = o1 + o2;

	if (result < o1 || result < o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)(m_sState.GR[m_sState.IR[2]]) = result;
}

template <typename INT_TYPE>
void CCPU::SUB_exec() {
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
	INT_TYPE result = o1 - o2;

	if (result > o1 || result > o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::MUL_exec() {
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
	INT_TYPE result = o1 * o2;

	if (result > o1 || result > o2)
		m_sState.FLAGS |= SState::CFlag;
	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::DIV_exec() {
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
	INT_TYPE result = o1 / o2;

	if (result == 0)
		m_sState.FLAGS |= SState::ZFlag;

	(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = result;
}

template <typename INT_TYPE>
void CCPU::ADDS_exec() {
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
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
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
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
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
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
	INT_TYPE o1 = (INT_TYPE)((m_sState.GR)[m_sState.IR[2]]);
	INT_TYPE o2 = (INT_TYPE)((m_sState.GR)[m_sState.IR[3]]);
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

template <typename INT_TYPE>
void CCPU::MOV_exec() {
	if ((m_sState.IR[1] & CCPU::REGISTER_MASK1)
		&& (m_sState.IR[1] & CCPU::REGISTER_MASK2))
	{
		if (m_sState.IR[2] > 5 || m_sState.IR[3] > 5)
			throw invalid_register_exception();
		m_sState.AR[m_sState.IR[2]] = m_sState.AR[m_sState.IR[3]];
	}
	else if ((m_sState.IR[1] & CCPU::REGISTER_MASK1)
		&& !(m_sState.IR[1] & CCPU::REGISTER_MASK2))
	{
		if (m_sState.IR[2] > 5 || (m_sState.IR[3] + sizeof(INT_TYPE)) > 64)
			throw invalid_register_exception();
		m_sState.AR[m_sState.IR[2]] = (INT_TYPE)m_sState.GR[m_sState.IR[3]];
	}
	else if (!(m_sState.IR[1] & CCPU::REGISTER_MASK1)
		&& !(m_sState.IR[1] & CCPU::REGISTER_MASK2))
	{
		if ((m_sState.IR[2] + sizeof(INT_TYPE)) > 64 || (m_sState.IR[3] + sizeof(INT_TYPE)) > 64)
			throw invalid_register_exception();
		(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = (INT_TYPE)m_sState.GR[m_sState.IR[3]];
	}
	else if (!(m_sState.IR[1] & CCPU::REGISTER_MASK1)
		&& (m_sState.IR[1] & CCPU::REGISTER_MASK2))
	{
		if ((m_sState.IR[2] + sizeof(INT_TYPE)) > 64 || m_sState.IR[3] > 5)
			throw invalid_register_exception();
		(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = m_sState.AR[m_sState.IR[3]];
	}
}

template <typename INT_TYPE>
void CCPU::ASG_exec() {
	if (m_sState.IR[1] & CCPU::REGISTER_MASK1)
	{
		m_sState.AR[m_sState.IR[2]] = (INT_TYPE)m_sState.IR[3];
	}
	else
	{
		(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = (INT_TYPE)m_sState.IR[3];
	}

	if ((INT_TYPE)m_sState.IR[3])
		m_sState.FLAGS |= SState::ZFlag;
}

template <typename INT_TYPE>
void CCPU::LOD_exec() {
}

template <typename INT_TYPE>
void CCPU::STR_exec() {
}

template <typename INT_TYPE>
void CCPU::JMP_exec() {
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
void CCPU::INC_exec() {
}

template <typename INT_TYPE>
void CCPU::DEC_exec() {
}

template <typename INT_TYPE>
void CCPU::JNZ_exec() {
}

template <typename INT_TYPE>
void CCPU::JNG_exec() {
}

template <typename INT_TYPE>
void CCPU::JNL_exec() {
}

template <typename INT_TYPE>
void CCPU::HLT_exec() {
	m_sState.RUN = false;
}

template <typename INT_TYPE>
void CCPU::JG_exec() {
}

template <typename INT_TYPE>
void CCPU::JL_exec() {
}

template <typename INT_TYPE>
void CCPU::JZ_exec() {
}

template <typename INT_TYPE>
void CCPU::OR_exec() {
}

template <typename INT_TYPE>
void CCPU::PUSH_exec() {
	m_sState.SP -= 4;

	if (m_sState.IR[1] & CCPU::REGISTER_MASK1)
	{
		if (m_sState.IR[2] > 5)
			throw invalid_register_exception();

		CRAM::instance()->operator[]<quint32>(m_sState.SP) = m_sState.AR[m_sState.IR[2]];
	}
	else
	{
		if (m_sState.IR[2] + sizeof(INT_TYPE) > 64)
			throw invalid_register_exception();

		CRAM::instance()->operator[]<INT_TYPE>(m_sState.SP) = (INT_TYPE)m_sState.GR[m_sState.IR[2]];
	}
}

template <typename INT_TYPE>
void CCPU::POP_exec() {
	if (m_sState.IR[1] & CCPU::REGISTER_MASK1)
	{
		if (m_sState.IR[2] > 5)
			throw invalid_register_exception();

		m_sState.AR[m_sState.IR[2]] = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	}
	else
	{
		if (m_sState.IR[2] + sizeof(INT_TYPE) > 64)
		throw invalid_register_exception();

		(INT_TYPE&)m_sState.GR[m_sState.IR[2]] = CRAM::instance()->operator[]<INT_TYPE>(m_sState.SP);
	}
	m_sState.SP += 4;
}

template <typename INT_TYPE>
void CCPU::PUSHF_exec() {
	m_sState.SP -= 4;
	CRAM::instance()->operator[]<quint32>(m_sState.SP) = m_sState.FLAGS;
}

template <typename INT_TYPE>
void CCPU::POPF_exec() {
	m_sState.FLAGS = CRAM::instance()->operator[]<quint32>(m_sState.SP);
	m_sState.SP += 4;
}