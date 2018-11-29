#include "cpu_mgr.h"
#include <consolemgr>
#include "register.h"
#include <cpu_state.h>
#include <cpu>

CCPUManager::CCPUManager()
{
}

CCPUManager::~CCPUManager()
{
}
//
//CCPUState CCPUManager::GetState() const
//{
//	return m_pCurrentCPU->GetState();
//}
//
//CRegisterSet CCPUManager::GetRegisters() const
//{
//	return m_pCurrentCPU->GetRegisters();
//}
//
//void CCPUManager::UpdateRegisters(CRegisterSet const& newState)
//{
//	m_pCurrentCPU->UpdateRegisters(newState);
//}

void CCPUManager::Run()
{
	m_pCurrentCPU->Start();

}

ICPU* CCPUManager::CurrentCPU() const
{
	return m_pCurrentCPU;
}

ICPU* CCPUManager::Deploy(int nCPUCount)
{
	//RegisterInterface((IUnknown*)(new CCPU()));
	m_pCurrentCPU = (ICPU*)new CCPU();
	if (m_pCurrentCPU == nullptr)
		throw 0;

	return m_pCurrentCPU;
}
