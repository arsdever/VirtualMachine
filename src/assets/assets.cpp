#include "assets.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <register>
#include <cpu>

CRegisterWindow::CRegisterWindow(QWidget* pParent)
	: QWidget(pParent)
{
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("General purpose registers");
	form.setupUi(this);
	form.m_pReg0->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg1->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg2->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg3->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg4->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg5->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg6->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg7->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg8->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg9->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg10->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg11->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg12->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg13->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg14->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pReg15->setValidator(new QIntValidator(0, INT32_MAX, this));

	connect(form.m_pSubmit, SIGNAL(clicked), this, SLOT(ChangeRegisters()));
}

CRegisterWindow::~CRegisterWindow()
{
}
//
//void CRegisterWindow::ChangeRegisters()
//{
//	quint32* registers;
//	registers[0]  = form.m_pReg0 ->text().toInt();
//	registers[4]  = form.m_pReg1 ->text().toInt();
//	registers[8]  = form.m_pReg2 ->text().toInt();
//	registers[12] = form.m_pReg3 ->text().toInt();
//	registers[16] = form.m_pReg4 ->text().toInt();
//	registers[20] = form.m_pReg5 ->text().toInt();
//	registers[24] = form.m_pReg6 ->text().toInt();
//	registers[28] = form.m_pReg7 ->text().toInt();
//	registers[32] = form.m_pReg8 ->text().toInt();
//	registers[36] = form.m_pReg9 ->text().toInt();
//	registers[40] = form.m_pReg10->text().toInt();
//	registers[44] = form.m_pReg11->text().toInt();
//	registers[48] = form.m_pReg12->text().toInt();
//	registers[52] = form.m_pReg13->text().toInt();
//	registers[56] = form.m_pReg14->text().toInt();
//	registers[60] = form.m_pReg15->text().toInt();
//
//	//pCPU->UpdateRegisters(registers);
//}

void CRegisterWindow::UpdateValues(CCPU* pCPU)
{
	quint32* registers = &((quint32&)pCPU->GetState().GR);
	form.m_pReg0->setText(QString::number (registers[0], 16));
	form.m_pReg1->setText(QString::number (registers[1], 16));
	form.m_pReg2->setText(QString::number (registers[2], 16));
	form.m_pReg3->setText(QString::number (registers[3], 16));
	form.m_pReg4->setText(QString::number (registers[4], 16));
	form.m_pReg5->setText(QString::number (registers[5], 16));
	form.m_pReg6->setText(QString::number (registers[6], 16));
	form.m_pReg7->setText(QString::number (registers[7], 16));
	form.m_pReg8->setText(QString::number (registers[8], 16));
	form.m_pReg9->setText(QString::number (registers[9], 16));
	form.m_pReg10->setText(QString::number(registers[10], 16));
	form.m_pReg11->setText(QString::number(registers[11], 16));
	form.m_pReg12->setText(QString::number(registers[12], 16));
	form.m_pReg13->setText(QString::number(registers[13], 16));
	form.m_pReg14->setText(QString::number(registers[14], 16));
	form.m_pReg15->setText(QString::number(registers[15], 16));
	form.m_pFlagRegister->setText(QString::number(pCPU->GetState().FLAGS, 16));
}

CARegisterWindow::CARegisterWindow(QWidget* pParent)
	: QWidget(pParent)
{
	setWindowFlags(Qt::SubWindow);
	setWindowTitle("Address registers");
	form.setupUi(this);
	form.m_pAReg0->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pAReg1->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pAReg2->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pAReg3->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pAReg4->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pAReg5->setValidator(new QIntValidator(0, INT32_MAX, this));
	form.m_pARegIP->setValidator(new QIntValidator(0, INT32_MAX, this));
}

CARegisterWindow::~CARegisterWindow()
{
}

void CARegisterWindow::UpdateValues(CCPU* pCPU)
{
	quint32* registers = pCPU->GetState().AR;
	quint32 ip = pCPU->GetState().PC;
	form.m_pAReg0->setText(QString::number(registers[0], 16));
	form.m_pAReg1->setText(QString::number(registers[1], 16));
	form.m_pAReg2->setText(QString::number(registers[2], 16));
	form.m_pAReg3->setText(QString::number(registers[3], 16));
	form.m_pAReg4->setText(QString::number(registers[4], 16));
	form.m_pAReg5->setText(QString::number(registers[5], 16));
	form.m_pARegIP->setText(QString::number(ip, 16));
}