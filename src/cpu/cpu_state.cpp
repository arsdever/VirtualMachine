#include "cpu_state.h"
#include "register.h"

CCPUState::CCPUState()
	: m_baRegister(64)
	, m_nInstructionPointer(0)
	, m_bHalt(false)
{
}

CCPUState::~CCPUState()
{
}