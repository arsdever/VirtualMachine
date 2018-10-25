#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RAM_LIB)
#  define RAM_EXPORT Q_DECL_EXPORT
# else
#  define RAM_EXPORT Q_DECL_IMPORT
# endif
#else
# define RAM_EXPORT
#endif
