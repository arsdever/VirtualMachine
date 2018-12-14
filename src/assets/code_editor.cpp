#include "code_editor.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QLibrary>

CCodeEditor::CCodeEditor()
	: m_pNumberArea(new CEditorNumberArea(this))
	, m_pAddressArea(new CEditorAddressArea(this))
	, m_pBPArea(new CEditorBPArea(this))
{
	appendHtml("<font style=\"color:#fff\">");
	//setAutoFillBackground(true);
	m_pNumberArea->SetDrawerWidget(this);
	m_pAddressArea->SetDrawerWidget(this);
	m_pBPArea->SetDrawerWidget(this);
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateAreas(QRect, int)));
}

CCodeEditor::~CCodeEditor()
{
}

void CCodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	m_pNumberArea->setGeometry(QRect(cr.left(), cr.top(), m_pNumberArea->sizeHint().width(), cr.height()));
	m_pAddressArea->setGeometry(QRect(cr.left() + m_pNumberArea->sizeHint().width(), cr.top(), m_pAddressArea->sizeHint().width(), cr.height()));
	m_pBPArea->setGeometry(QRect(cr.left() + m_pNumberArea->sizeHint().width()+ m_pAddressArea->sizeHint().width(), cr.top(), m_pBPArea->sizeHint().width(), cr.height()));
}

void CCodeEditor::updateAreaWidth(int w)
{
	setViewportMargins(/*m_pThis->viewportMargins() +*/ QMargins(GetDecorationsSize().width(), 0, 0, 0));
}

void CCodeEditor::XDebugger::SetBreakpoint(quint32 address)
{
	if (m_pThis->m_lstBreakpoints.indexOf(address) >= 0)
		return;

	m_pThis->m_lstBreakpoints.push_back(address);
}

void CCodeEditor::XDebugger::UnsetBreakpoint(quint32 address)
{
	if (m_pThis->m_lstBreakpoints.indexOf(address) < 0)
		return;

	m_pThis->m_lstBreakpoints.removeOne(address);
}

void CCodeEditor::XDebugger::ClearBreakpoints()
{
	m_pThis->m_lstBreakpoints = QList<quint32>();
}

void CCodeEditor::XDebugger::ToggleBreakpoint(quint32 address)
{
	if (m_pThis->m_lstBreakpoints.indexOf(address) >= 0)
		UnsetBreakpoint(address);
	else
		SetBreakpoint(address);

	m_pThis->update();
}

void CCodeEditor::XDebugger::SetRunningAddress(quint32 address)
{
	m_pThis->m_nCurrentAddress = address;
	m_pThis->update();
}

QSize CCodeEditor::XEditorNumberAreaImplementer::GetWidgetSizeHint() const
{
	int digits = 1;
	int max = qMax(1, m_pThis->blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}
	if (digits < 3)
		digits = 3;

	int space = 3 + m_pThis->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

	return QSize(space + 10, 0);
}

void CCodeEditor::XEditorNumberAreaImplementer::DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const
{
	QPainter painter(pWidget);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.fillRect(pEvent->rect(), QColor(16, 16, 16));
	QTextBlock block = m_pThis->firstVisibleBlock();
	painter.setPen(QColor(55, 120, 192));
	QFont numberFont = m_pThis->font();
	numberFont.setBold(true);
	painter.setFont(numberFont);
	int blockNumber = block.blockNumber();
	int top = (int)m_pThis->blockBoundingGeometry(block).translated(m_pThis->contentOffset()).top();
	int bottom = top + (int)m_pThis->blockBoundingRect(block).height();
	while (block.isValid() && top <= pEvent->rect().bottom()) {
		if (block.isVisible() && bottom >= pEvent->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.drawText(5, top, pWidget->width() - 10, m_pThis->fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)m_pThis->blockBoundingRect(block).height();
		++blockNumber;
	}
}

void CCodeEditor::updateAreas(QRect const& r, int d)
{
	if (d)
	{
		m_pNumberArea->scroll(0, d);
		m_pAddressArea->scroll(0, d);
		m_pBPArea->scroll(0, d);
	}
	else
	{
		m_pNumberArea->update(0, r.y(), m_pNumberArea->width(), r.height());
		m_pAddressArea->update(m_pNumberArea->width(), r.y(), m_pAddressArea->width(), r.height());
		m_pBPArea->update(m_pNumberArea->width() + m_pAddressArea->width(), r.y(), m_pBPArea->width(), r.height());
	}

	if (r.contains(viewport()->rect()))
		updateAreaWidth(0);
}

QSize CCodeEditor::XEditorAddressAreaImplementer::GetWidgetSizeHint() const
{
	int digits = 10;

	int space = 3 + m_pThis->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

	return QSize(space + 10, 0);
}

void CCodeEditor::XEditorAddressAreaImplementer::DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const
{
	QPainter painter(pWidget);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.fillRect(pEvent->rect(), QColor(24, 24, 24));

	typedef bool(*Validator)(quint32, quint32*);
	Validator validate = (Validator)QLibrary::resolve(QString("debugger")
#ifdef _DEBUG
		+ "d"
#endif
	, "LineToAddress");

	if (validate == nullptr)
		return;

	QTextBlock block = m_pThis->firstVisibleBlock();
	painter.setPen(QColor(96,96,96));
	int blockNumber = block.blockNumber();
	int top = (int)m_pThis->blockBoundingGeometry(block).translated(m_pThis->contentOffset()).top();
	int bottom = top + (int)m_pThis->blockBoundingRect(block).height();
	while (block.isValid() && top <= pEvent->rect().bottom()) {
		if (block.isVisible() && bottom >= pEvent->rect().top()) {
			int lineNumber = blockNumber;
			quint32 address = 0;

			if (validate(lineNumber, &address))
			{
				painter.drawText(5, top, pWidget->width() - 10, m_pThis->fontMetrics().height(),
					Qt::AlignRight, QString("0x%1").arg(address, 8, 16, QChar('0')));

			}
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)m_pThis->blockBoundingRect(block).height();
		++blockNumber;
	}
}

QSize CCodeEditor::XEditorBPAreaImplementer::GetWidgetSizeHint() const
{
	return QSize(m_pThis->fontMetrics().height() + 4, 0);
}

void CCodeEditor::XEditorBPAreaImplementer::DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const
{
	QPainter painter(pWidget);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.fillRect(pEvent->rect(), QColor(28,28,28));

	typedef bool(*Validator)(quint32, quint32*);
	Validator validate = (Validator)QLibrary::resolve(QString("debugger")
#ifdef _DEBUG
		+ "d"
#endif
	, "LineToAddress");

	if (validate == nullptr)
		return;

	QTextBlock block = m_pThis->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)m_pThis->blockBoundingGeometry(block).translated(m_pThis->contentOffset()).top();
	int bottom = top + (int)m_pThis->blockBoundingRect(block).height();
	while (block.isValid() && top <= pEvent->rect().bottom()) {
		if (block.isVisible() && bottom >= pEvent->rect().top()) {
			int lineNumber = blockNumber;
			quint32 address = 0;

			if (validate(lineNumber, &address))
			{

				if (m_pThis->m_lstBreakpoints.indexOf(address) >= 0)
				{
					painter.setPen(QPen(Qt::white, 2));
					painter.setBrush(Qt::red);
					painter.drawEllipse(2, top + 2, m_pThis->fontMetrics().height() - 4, m_pThis->fontMetrics().height() - 4);
				}

				if (address == m_pThis->m_nCurrentAddress)
				{
					painter.setPen(QPen(QColor(14,65,14), 1));
					painter.setBrush(Qt::green);
					QPainterPath path;
					path.moveTo(3, top + 3);
					path.lineTo(m_pThis->fontMetrics().height() - 3, top + m_pThis->fontMetrics().height()/2);
					path.lineTo(3, top + m_pThis->fontMetrics().height() - 3);
					path.closeSubpath();
					painter.drawPath(path);
				}
			}
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)m_pThis->blockBoundingRect(block).height();
		++blockNumber;
	}
}

void CCodeEditor::XEditorBPAreaImplementer::PressedAt(QMouseEvent* pEvent) const
{
	typedef bool(*Validator)(quint32, quint32*);
	Validator validate = (Validator)QLibrary::resolve(QString("debugger")
#ifdef _DEBUG
		+ "d"
#endif
	, "LineToAddress");

	if (validate == nullptr)
		return;

	QTextBlock block = m_pThis->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)m_pThis->blockBoundingGeometry(block).translated(m_pThis->contentOffset()).top();
	int bottom = top + (int)m_pThis->blockBoundingRect(block).height();
	while (block.isValid() && top <= m_pThis->visibleRegion().boundingRect().bottom()) {
		if (block.isVisible() && bottom >= m_pThis->visibleRegion().boundingRect().top()) {
			qint32 value = pEvent->pos().y();
			quint32 address = 0;

			if (value >= top && value <= bottom && validate(blockNumber, &address))
			{
				try
				{
					CallFunction<IDebugger>(IDebugger::ToggleBreakpointFunctor(address));
					m_pThis->update();
				}
				catch (IDebugger::debugger_exception)
				{}
				return;
			}

			block = block.next();
			top = bottom;
			bottom = top + (int)m_pThis->blockBoundingRect(block).height();
			++blockNumber;
		}
	}
}
void CCodeEditor::keyPressEvent(QKeyEvent* event)
{}
