#include "cpu.h"
#include <core>
#include "cpu_mgr.h"

CCPU::CCPU()
{
}

void CCPU::Register()
{
	CCPUManager* pManager = GetCore()->GetManager<CCPUManager>();

	if (pManager == nullptr)
		return;

	pManager->RegisterInterface((IUnknown*)this);
}

void CCPU::XCPU::Fetch()
{
	
}

void CCPU::XCPU::Decode()
{

}

void CCPU::XCPU::Execute()
{

}
//
//void CCPU::XCPU::UpdateRegisters(CRegisterSet const& newState)
//{
//	for (int i = 0; i < 64; ++i)
//		m_pThis- GetRegisters().SetValue<char>(i, newState.GetValue<char>(i));
//}
//
//CRegisterSet CCPU::XCPU::GetRegisters() const
//{
//	return m_pThis->GetRegisters();
//}