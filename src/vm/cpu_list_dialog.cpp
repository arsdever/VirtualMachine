#include "cpu_list_dialog.h"

#include <QListWidget>
#include <QPushButton>
#include <QLayout>
#include "virtualmachine.h"

CCPUListDialog::CCPUListDialog(CVirtualMachine* pVM, QWidget* pParent)
	: QDialog(pParent)
{
	QListWidget* pCPUList = new QListWidget();
	pCPUList->setSelectionMode(QAbstractItemView::SingleSelection);
	QPushButton* pOk = new QPushButton("OK");
	QPushButton* pCancel = new QPushButton("Cancel");
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	QHBoxLayout* pButtonLayout = new QHBoxLayout();
	pButtonLayout->addWidget(pOk);
	pButtonLayout->addWidget(pCancel);
	pCPUList->addItems(pVM->GetCPUIDList());
	pMainLayout->addWidget(pCPUList);
	pMainLayout->addLayout(pButtonLayout);
	setLayout(pMainLayout);

	connect(pOk, SIGNAL(clicked()), this, SLOT(Accept()));
	connect(pCancel, SIGNAL(clicked()), this, SLOT(Cancel()));
	connect(pCPUList, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
}

CCPUListDialog::~CCPUListDialog()
{
}

void CCPUListDialog::Accept()
{
	close();
}

void CCPUListDialog::Cancel()
{
	m_strSelection = "";
	close();
}

void CCPUListDialog::SelectionChanged()
{
	QListWidget* pWidget = dynamic_cast<QListWidget*>(sender());
	if (pWidget == nullptr)
		return;

	m_strSelection = pWidget->selectedItems().front()->text();
}

QString CCPUListDialog::GetSelection() const
{
	return m_strSelection;
}