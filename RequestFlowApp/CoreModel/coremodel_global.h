#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COREMODEL_LIB)
#  define COREMODEL_EXPORT Q_DECL_EXPORT
# else
#  define COREMODEL_EXPORT Q_DECL_IMPORT
# endif
#else
# define COREMODEL_EXPORT
#endif
