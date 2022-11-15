#ifndef QDOTENV_GLOBAL_H
#define QDOTENV_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QDOTENV_LIBRARY)
#  define QDOTENV_EXPORT Q_DECL_EXPORT
#else
#  define QDOTENV_EXPORT Q_DECL_IMPORT
#endif

#endif // QDOTENV_GLOBAL_H
