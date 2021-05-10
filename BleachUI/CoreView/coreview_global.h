#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COREVIEW_LIB)
#  define COREVIEW_EXPORT Q_DECL_EXPORT
# else
#  define COREVIEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define COREVIEW_EXPORT
#endif
