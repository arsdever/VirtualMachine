#include "gpr_window.h"

#include <cpu>

#include <QLayout>
#include <QLineEdit>
#include <QLabel>

CGRegisterWindow::CGRegisterWindow(QWidget* pParent)
	: QWidget(pParent)
	, m_arrRegisterValues(16)
{
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("General purpose registers");
	QGridLayout* _layout = new QGridLayout();

	for (int i = 0; i < m_arrRegisterValues.size(); ++i)
	{
		m_arrRegisterValues[i] = new QLineEdit();
		_layout->addWidget(new QLabel(QString("REG%1").arg(i*4)));
		_layout->addWidget(m_arrRegisterValues[i], i, 1);
	}

	setLayout(_layout);
	//connect(form.m_pSubmit, SIGNAL(clicked()), this, SLOT(ChangeRegisters()));
}

CGRegisterWindow::~CGRegisterWindow()
{
}

void CGRegisterWindow::ChangeHandler()
{
	bool ok = false;
	quint32 value = static_cast<QLineEdit*>(sender())->text().toInt(&ok, 16);
	if (!ok)
		return;

	quint8 index = (quint8)findChildren<QLineEdit*>().indexOf((QLineEdit*)sender());
	emit ChangeRegister(index, value);
}

void CGRegisterWindow::ToggleView()
{
	if (isHidden())
		show();
	else
		hide();
}

void CGRegisterWindow::XVMInformation::UpdateGRegistersInformation(CCPU* pCPU)
{
	if (pCPU == nullptr)
		return;

	quint32* registers = &((quint32&)pCPU->GetState().GR);
	for(int i = 0; i < m_pThis->m_arrRegisterValues.size(); ++i)
		m_pThis->m_arrRegisterValues[i]->setText(QString("0x%1").arg(registers[i], 8, 16, QChar('0')));

	CallFunction<ILogger>(ILogger::InfoFunctor("General purpose registers updated."));
}