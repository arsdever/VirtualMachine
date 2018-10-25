#include "assets.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <register>
#include <core>
#include <cpumgr>

CRegisterWindow::CRegisterWindow(QWidget* pParent)
	: QWidget(pParent)
{
	form.setupUi(this);
	form.m_pReg0->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg1->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg2->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg3->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg4->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg5->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg6->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));
	form.m_pReg7->setValidator(new QIntValidator(0, INT_FAST64_MAX, this));

	connect(form.m_pSubmit, SIGNAL(clicked), this, SLOT(ChangeRegisters()));
	CCPUManager* pCPUMgr = GetCore()->GetManager<CCPUManager>();
	if (pCPUMgr == nullptr)
		throw int();
	connect(pCPUMgr, SIGNAL(RegistersUpdated()), this, SLOT(UpdateValues()));
}

CRegisterWindow::~CRegisterWindow()
{
}

void CRegisterWindow::ChangeRegisters()
{
	CRegisterSet registers(64);
	registers.SetValue<qint32>(0, form.m_pReg0->text().toInt());
	registers.SetValue<qint32>(4, form.m_pReg1->text().toInt());
	registers.SetValue<qint32>(8, form.m_pReg2->text().toInt());
	registers.SetValue<qint32>(12, form.m_pReg3->text().toInt());
	registers.SetValue<qint32>(16, form.m_pReg4->text().toInt());
	registers.SetValue<qint32>(20, form.m_pReg5->text().toInt());
	registers.SetValue<qint32>(24, form.m_pReg6->text().toInt());
	registers.SetValue<qint32>(28, form.m_pReg7->text().toInt());
	registers.SetValue<qint32>(32, form.m_pReg8->text().toInt());
	registers.SetValue<qint32>(36, form.m_pReg9->text().toInt());
	registers.SetValue<qint32>(40, form.m_pReg10->text().toInt());
	registers.SetValue<qint32>(44, form.m_pReg11->text().toInt());
	registers.SetValue<qint32>(48, form.m_pReg12->text().toInt());
	registers.SetValue<qint32>(52, form.m_pReg13->text().toInt());
	registers.SetValue<qint32>(56, form.m_pReg14->text().toInt());
	registers.SetValue<qint32>(60, form.m_pReg15->text().toInt());

	CCPUManager* pCPUMgr = GetCore()->GetManager<CCPUManager>();
	if (pCPUMgr == nullptr)
		return;
	pCPUMgr->UpdateRegisters(registers);
}

void CRegisterWindow::UpdateValues()
{
	CCPUManager* pCPUMgr = GetCore()->GetManager<CCPUManager>();
	if (pCPUMgr == nullptr)
		return;
	CRegisterSet registers = pCPUMgr->GetRegisters();
	form.m_pReg0->setText(QString::number(registers.GetValue<qint32>(0), 16));
	form.m_pReg1->setText(QString::number(registers.GetValue<qint32>(4), 16));
	form.m_pReg2->setText(QString::number(registers.GetValue<qint32>(8), 16));
	form.m_pReg3->setText(QString::number(registers.GetValue<qint32>(12), 16));
	form.m_pReg4->setText(QString::number(registers.GetValue<qint32>(16), 16));
	form.m_pReg5->setText(QString::number(registers.GetValue<qint32>(20), 16));
	form.m_pReg6->setText(QString::number(registers.GetValue<qint32>(24), 16));
	form.m_pReg7->setText(QString::number(registers.GetValue<qint32>(28), 16));
	form.m_pReg8->setText(QString::number(registers.GetValue<qint32>(32), 16));
	form.m_pReg9->setText(QString::number(registers.GetValue<qint32>(36), 16));
	form.m_pReg10->setText(QString::number(registers.GetValue<qint32>(40), 16));
	form.m_pReg11->setText(QString::number(registers.GetValue<qint32>(44), 16));
	form.m_pReg12->setText(QString::number(registers.GetValue<qint32>(48), 16));
	form.m_pReg13->setText(QString::number(registers.GetValue<qint32>(52), 16));
	form.m_pReg14->setText(QString::number(registers.GetValue<qint32>(56), 16));
	form.m_pReg15->setText(QString::number(registers.GetValue<qint32>(60), 16));
}