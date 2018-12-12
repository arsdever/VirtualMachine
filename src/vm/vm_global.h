#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(VM_LIB)
#  define VM_EXPORT Q_DECL_EXPORT
# else
#  define VM_EXPORT Q_DECL_IMPORT
# endif
#else
# define VM_EXPORT
#endif
