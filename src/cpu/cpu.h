#pragma once

#include "cpu_global.h"
#include <plugin>
#include <macros>
#include <interfaces>

class CPU_EXPORT CCPU : public CPlugin
{

	REGISTER_INTERFACE(CCPU, CPU)
		void Fetch() override;
		void Decode() override;
		void Execute() override;
		//void UpdateRegisters(CRegisterSet const& newState) override;
		//CRegisterSet GetRegisters() const override;
	REGISTER_INTERFACE_END(CPU)

public:
	CCPU();

private:
	void Register();
};
