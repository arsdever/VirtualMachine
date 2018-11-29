#pragma once

#include "assets_global.h"
#include <QDialog>

class QGroupBox;
class QRadioButton;
class QLineEdit;
class QLabel;

class ASSETS_EXPORT CMemoryChangeDialog : public QDialog
{

	Q_OBJECT

private:
	CMemoryChangeDialog();
	~CMemoryChangeDialog();
	
	qint8 GetData(quint32& address, quint64& data);

	QLineEdit* m_pAddressEdit;
	QLineEdit* m_pValueEdit;
	QGroupBox* m_pAddressType;
	QRadioButton* m_pTypeByte;
	QRadioButton* m_pTypeWord;
	QRadioButton* m_pTypeDWord;
	QRadioButton* m_pTypeQWord;

	qint8 m_bExitCode;
	qint32 m_nAddress;
	qint64 m_nData;

private slots:
	void OnOk();
	void OnCancel();

public:
	static qint8 GetDataToChange(quint32& rAddress, quint64& rData);
};

