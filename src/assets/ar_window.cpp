#include "ar_window.h"

#include <cpu>

#include <QLayout>
#include <QLabel>
#include <QLineEdit>

CARegisterWindow::CARegisterWindow(QWidget* pParent)
	: QWidget(pParent)
	, m_arrRegisterValues(7)
{
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("Address registers");
	QGridLayout* _layout = new QGridLayout();

	for (int i = 0; i < m_arrRegisterValues.size(); ++i)
	{
		m_arrRegisterValues[i] = new QLineEdit();
		if(i > 1 && i < 6)
			_layout->addWidget(new QLabel(QString("A%1").arg(i)));
		else if(i == 0)
			_layout->addWidget(new QLabel(QString("SF")));
		else if(i == 1)
			_layout->addWidget(new QLabel(QString("SP")));
		else if(i == 6)
			_layout->addWidget(new QLabel(QString("PC")));
		_layout->addWidget(m_arrRegisterValues[i], i, 1);
	}

	setLayout(_layout);
}

CARegisterWindow::~CARegisterWindow()
{
}

void CARegisterWindow::ToggleView()
{
	if (isHidden())
		show();
	else
		hide();
}

void CARegisterWindow::XVMInformation::UpdateARegistersInformation(CCPU* pCPU)
{
	if (pCPU == nullptr)
		return;

	quint32* registers = pCPU->GetState().AR;
	quint32 ip = pCPU->GetState().PC;
	m_pThis->m_arrRegisterValues[6]->setText(QString("0x%1").arg(ip, 8, 16, QChar('0')));
	for (int i = 0; i < m_pThis->m_arrRegisterValues.size() - 1; ++i)
		m_pThis->m_arrRegisterValues[i]->setText(QString("0x%1").arg(registers[i], 8, 16, QChar('0')));

	CallFunction<ILogger>(ILogger::InfoFunctor("Address registers updated."));
}