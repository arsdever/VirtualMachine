#include "assets.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <register>
#include <cpu>
#include <ram>

CRegisterWindow::CRegisterWindow(QWidget* pParent)
	: QWidget(pParent)
{
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("General purpose registers");
	setFixedWidth(500);
	form.setupUi(this);
	connect(form.m_pReg0 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg1 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg2 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg3 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg4 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg5 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg6 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg7 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg8 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg9 , SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg10, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg11, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg12, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg13, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg14, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));
	connect(form.m_pReg15, SIGNAL(editingFinished()), this, SLOT(ChangeHandler()));

	connect(form.m_pSubmit, SIGNAL(clicked), this, SLOT(ChangeRegisters()));
}

CRegisterWindow::~CRegisterWindow()
{
}

void CRegisterWindow::ChangeHandler()
{
	bool ok = false;
	quint32 value = static_cast<QLineEdit*>(sender())->text().toInt(&ok, 16);
	if (!ok)
		return;

	quint8 index = (quint8)findChildren<QLineEdit*>().indexOf((QLineEdit*)sender());
	emit ChangeRegister(index, value);
}

void CRegisterWindow::UpdateValues(CCPU* pCPU)
{
	quint32* registers = &((quint32&)pCPU->GetState().GR);
	form.m_pReg0 ->setText(QString("0x%1").arg(registers[0],  8, 16, QChar('0')));
	form.m_pReg1 ->setText(QString("0x%1").arg(registers[1],  8, 16, QChar('0')));
	form.m_pReg2 ->setText(QString("0x%1").arg(registers[2],  8, 16, QChar('0')));
	form.m_pReg3 ->setText(QString("0x%1").arg(registers[3],  8, 16, QChar('0')));
	form.m_pReg4 ->setText(QString("0x%1").arg(registers[4],  8, 16, QChar('0')));
	form.m_pReg5 ->setText(QString("0x%1").arg(registers[5],  8, 16, QChar('0')));
	form.m_pReg6 ->setText(QString("0x%1").arg(registers[6],  8, 16, QChar('0')));
	form.m_pReg7 ->setText(QString("0x%1").arg(registers[7],  8, 16, QChar('0')));
	form.m_pReg8 ->setText(QString("0x%1").arg(registers[8],  8, 16, QChar('0')));
	form.m_pReg9 ->setText(QString("0x%1").arg(registers[9],  8, 16, QChar('0')));
	form.m_pReg10->setText(QString("0x%1").arg(registers[10], 8, 16, QChar('0')));
	form.m_pReg11->setText(QString("0x%1").arg(registers[11], 8, 16, QChar('0')));
	form.m_pReg12->setText(QString("0x%1").arg(registers[12], 8, 16, QChar('0')));
	form.m_pReg13->setText(QString("0x%1").arg(registers[13], 8, 16, QChar('0')));
	form.m_pReg14->setText(QString("0x%1").arg(registers[14], 8, 16, QChar('0')));
	form.m_pReg15->setText(QString("0x%1").arg(registers[15], 8, 16, QChar('0')));
	form.m_pFlagRegister->setText(QString::number(pCPU->GetState().FLAGS, 16));
}

CARegisterWindow::CARegisterWindow(QWidget* pParent)
	: QWidget(pParent)
{
	setFixedWidth(200);
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("Address registers");
	form.setupUi(this);
}

CARegisterWindow::~CARegisterWindow()
{
}

void CARegisterWindow::UpdateValues(CCPU* pCPU)
{
	quint32* registers = pCPU->GetState().AR;
	quint32 ip = pCPU->GetState().PC;
	form.m_pAReg0->setText(QString("0x%1").arg(registers[0], 8, 16, QChar('0')));
	form.m_pAReg1->setText(QString("0x%1").arg(registers[1], 8, 16, QChar('0')));
	form.m_pAReg2->setText(QString("0x%1").arg(registers[2], 8, 16, QChar('0')));
	form.m_pAReg3->setText(QString("0x%1").arg(registers[3], 8, 16, QChar('0')));
	form.m_pAReg4->setText(QString("0x%1").arg(registers[4], 8, 16, QChar('0')));
	form.m_pAReg5->setText(QString("0x%1").arg(registers[5], 8, 16, QChar('0')));
	form.m_pARegIP->setText(QString("0x%1").arg(ip, 8, 16, QChar('0')));
}