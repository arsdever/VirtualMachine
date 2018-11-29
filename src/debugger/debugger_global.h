#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEBUGGER_LIB)
#  define DEBUGGER_EXPORT Q_DECL_EXPORT
# else
#  define DEBUGGER_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEBUGGER_EXPORT
#endif
