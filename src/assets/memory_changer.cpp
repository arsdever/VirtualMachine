#include "memory_changer.h"

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>

CMemoryChangeDialog::CMemoryChangeDialog()
	: m_pAddressEdit(new QLineEdit())
	, m_pValueEdit(new QLineEdit())
	, m_pAddressType(new QGroupBox("Value type"))
	, m_pTypeByte(new QRadioButton("Byte"))
	, m_pTypeWord(new QRadioButton("Word"))
	, m_pTypeDWord(new QRadioButton("Double word"))
	, m_pTypeQWord(new QRadioButton("Quad word"))
{
	setWindowTitle("Set memory value");
	QVBoxLayout* pLayout = new QVBoxLayout();

	QLabel* pAddressLabel = new QLabel("Address");
	QLabel* pValueLabel = new QLabel("Value");
	QHBoxLayout* pAddressLayout = new QHBoxLayout();
	QHBoxLayout* pValueLayout = new QHBoxLayout();
	QPushButton* pButtonOk = new QPushButton("Ok");
	QPushButton* pButtonCancel = new QPushButton("Cancel");

	pAddressLayout->addWidget(pAddressLabel);
	pAddressLayout->addWidget(m_pAddressEdit);
	pValueLayout->addWidget(pValueLabel);
	pValueLayout->addWidget(m_pValueEdit);

	m_pAddressType->setLayout(new QVBoxLayout());
	m_pTypeByte->setChecked(true);
	m_pAddressType->layout()->addWidget(m_pTypeByte);
	m_pAddressType->layout()->addWidget(m_pTypeWord);
	m_pAddressType->layout()->addWidget(m_pTypeDWord);
	m_pAddressType->layout()->addWidget(m_pTypeQWord);

	pLayout->addLayout(pAddressLayout);
	pLayout->addLayout(pValueLayout);
	pLayout->addWidget(m_pAddressType);
	pLayout->addWidget(pButtonOk);
	pLayout->addWidget(pButtonCancel);

	setLayout(pLayout);
	
	connect(pButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(pButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

void CMemoryChangeDialog::OnOk()
{
	bool isOk = false;
	m_nAddress = m_pAddressEdit->text().toULongLong(&isOk, 0);

	if (!isOk)
	{
		m_bExitCode = -1;
		close();
	}

	m_nData = m_pValueEdit->text().toULongLong(&isOk, 0);

	if (!isOk)
	{
		m_bExitCode = -1;
		close();
	}

	if (m_pTypeByte->isChecked())
		m_bExitCode = 0;
	else if (m_pTypeWord->isChecked())
		m_bExitCode = 1;
	else if (m_pTypeDWord->isChecked())
		m_bExitCode = 2;
	else if (m_pTypeQWord->isChecked())
		m_bExitCode = 3;

	close();
}

void CMemoryChangeDialog::OnCancel()
{
	m_bExitCode = -1;
	close();
}

CMemoryChangeDialog::~CMemoryChangeDialog()
{
}

qint8 CMemoryChangeDialog::GetData(quint32& address, quint64& data)
{
	address = m_nAddress;
	data = m_nData;
	return m_bExitCode;
}

qint8 CMemoryChangeDialog::GetDataToChange(quint32& rAddress, quint64& rData)
{
	CMemoryChangeDialog* pDialog = new CMemoryChangeDialog();
	pDialog->exec();

	qint8 result = pDialog->GetData(rAddress, rData);
	delete pDialog;
	return result;
}