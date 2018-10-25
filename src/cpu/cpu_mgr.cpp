#include "cpu_mgr.h"
#include <consolemgr>
#include "cpu_state.h"
#include "register.h"
#include "executer.h"
#include <ram>

QString GetOperationName(CCPUManager::InstructionCode operation)
{
	switch (operation)
	{
	case CCPUManager::InstructionCode::NOP: return "NOP";
	case CCPUManager::InstructionCode::RET: return "RET";
	case CCPUManager::InstructionCode::INT: return "INT";
	case CCPUManager::InstructionCode::BRK: return "BRK";
	case CCPUManager::InstructionCode::ADD: return "ADD";
	case CCPUManager::InstructionCode::SUB: return "SUB";
	case CCPUManager::InstructionCode::MUL: return "MUL";
	case CCPUManager::InstructionCode::DIV: return "DIV";
	}

	return "Unknown instruction";
}

CCPUManager::CCPUManager()
	: m_pCPUState(new CCPUState())
{
}

CCPUManager::~CCPUManager()
{
}

void CCPUManager::Fetch()
{
	CConsoleManager* pConsole = GetCore()->GetManager<CConsoleManager>();
	if (pConsole != nullptr)
		pConsole->PrintInfoMessage(tr("Fetching [0x%0]").arg(int(m_pCPUState->RegIP()), 8, 16, QChar('0')));

	CRAMManager* pRam = GetCore()->GetManager<CRAMManager>();

	if (pRam == nullptr)
	{
		if (pConsole != nullptr)
			pConsole->PrintErrorMessage("RAM not found.");
		return;
	}

	pRam->ReadMemory(m_pCPUState->RegIP(), (char*)m_pCPUState->RegIR(), 10);
}

void CCPUManager::Decode()
{
	CConsoleManager* pConsole = GetCore()->GetManager<CConsoleManager>();
	if (pConsole != nullptr)
		pConsole->PrintInfoMessage(tr("Decoding [%0]").arg(GetOperationName((InstructionCode)((char*)m_pCPUState->RegIR())[0])));

	m_fptrExecuter = Executer::s_mapInstructions[(InstructionCode)((char*)m_pCPUState->RegIR())[0]];
}

void CCPUManager::Execute()
{
	CConsoleManager* pConsole = GetCore()->GetManager<CConsoleManager>();

	if (pConsole != nullptr)
		pConsole->PrintInfoMessage(tr("Executing [%0]").arg(GetOperationName((InstructionCode)((char*)m_pCPUState->RegIR())[0])));

	m_fptrExecuter(m_pCPUState);
	emit RegistersUpdated();
}

void CCPUManager::Run()
{
	m_pCPUState->m_bHalt = false;
	while (!m_pCPUState->m_bHalt)
	{
		Fetch();
		Decode();
		Execute();
	}
}

void CCPUManager::UpdateRegisters(CRegisterSet const& newState)
{
	for (int i = 0; i < 64; ++i)
		GetRegisters().SetValue<char>(i, newState.GetValue<char>(i));
}

CRegisterSet CCPUManager::GetRegisters() const
{
	return m_pCPUState->GetRegisters();
}