#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEVICES_LIB)
#  define DEVICES_EXPORT Q_DECL_EXPORT
# else
#  define DEVICES_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEVICES_EXPORT
#endif
