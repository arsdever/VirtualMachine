#include "call_stack.h"
#include <cpu>
#include <ram>
#include "disassembler.h"

CCallStackView::CCallStackView(QWidget* pParent)
	: QListWidget(pParent)
{
}

CCallStackView::~CCallStackView()
{
}

void CCallStackView::XVMInformation::UpdateCallStack(quint32 sf, CRAM* pRAM)
{
	QStringList result;
	while (sf < pRAM->GetSize())
	{
		quint32 ip = pRAM->operator[]<quint32>(sf + 4) - CCPU::s_mapInstructions[CCPU::CALL].first;
		quint32 address = pRAM->operator[]<quint32>(ip + 1);

		if (s_mapRelocaionTable.contains(address))
			result.push_back(s_mapRelocaionTable[address]);
		else
			result.push_back(QString("0x%1").arg(address, 8, 16, QChar('0')));

		sf = pRAM->operator[]<quint32>(sf);
	}
	m_pThis->clear();
	m_pThis->addItems(result);
}