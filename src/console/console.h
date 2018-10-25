#pragma once

#include <QWidget>
#include "console_global.h"
#include <interfaces>
#include <macros>
#include <QTextEdit>
#include <plugin>

class CConsoleManager;
class QKeyEvent;

class CONSOLE_EXPORT CConsole : public QTextEdit//, public CPlugin
{

	Q_OBJECT

	REGISTER_INTERFACE(CConsole, Console)
		void PrintMessage(QString const&);
		void PrintError(QString const&);
		void PrintWarning(QString const&);
		void GetInput(QString const&);
	REGISTER_INTERFACE_END(Console)

public:
	CConsole(QWidget* pParent = nullptr);
	~CConsole() {}

protected:
	void keyPressEvent(QKeyEvent* pEvent) override;
	//void keyReleaseEvent(QKeyEvent* pEvent) override;
	void ParseInputString();
	void Register(); // override;

private:
	CConsoleManager* m_pManager;
	QString m_strInputString;
};
