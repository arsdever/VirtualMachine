#include "interfaces.h"
#include <QUuid>

IUnknown::~IUnknown() {}

const QString ILogger::UUID				= QUuid::createUuid().toString();
const QString IDebugger::UUID			= QUuid::createUuid().toString();
const QString IVMInformation::UUID		= QUuid::createUuid().toString();