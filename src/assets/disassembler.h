#pragma once

#include "assets_global.h"
#include <cpu>

class ASSETS_EXPORT CDisassembler
{
public:
	CDisassembler();
	~CDisassembler();

	static QString Disassemble(quint8 instruction[8]);
};

