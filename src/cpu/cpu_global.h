#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CPU_LIB)
#  define CPU_EXPORT Q_DECL_EXPORT
# else
#  define CPU_EXPORT Q_DECL_IMPORT
# endif
#else
# define CPU_EXPORT
#endif
