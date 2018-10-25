#include "register.h"

CRegisterSet::CRegisterSet(int bytes)
	: m_aBuffer(bytes, 0)
{
}

CRegisterSet::~CRegisterSet()
{
}
