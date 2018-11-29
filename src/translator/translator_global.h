#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TRANSLATOR_LIB)
#  define TRANSLATOR_EXPORT Q_DECL_EXPORT
# else
#  define TRANSLATOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define TRANSLATOR_EXPORT
#endif
