#include "ram.h"
#include <QByteArray>

CRAM::CRAM(quint32 nSize)
	: m_nSize(nSize)
	, m_pMemory(new quint8[m_nSize]{0})
{
}

CRAM::~CRAM()
{
}

CRAM* CRAM::pinstance = nullptr;
CRAM* CRAM::instance()
{
	if (pinstance == nullptr)
		return pinstance = new CRAM();

	return pinstance;
}