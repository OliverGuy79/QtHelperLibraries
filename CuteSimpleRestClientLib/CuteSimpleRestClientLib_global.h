#ifndef CUTESIMPLERESTCLIENTLIB_GLOBAL_H
#define CUTESIMPLERESTCLIENTLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CUTESIMPLERESTCLIENTLIB_LIBRARY)
#  define CUTESIMPLERESTCLIENTLIB_EXPORT Q_DECL_EXPORT
#else
#  define CUTESIMPLERESTCLIENTLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // CUTESIMPLERESTCLIENTLIB_GLOBAL_H
