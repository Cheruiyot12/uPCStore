#ifndef QACRYPT_GLOBAL_H
#define QACRYPT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QACRYPT_LIBRARY)
#  define QACRYPTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QACRYPTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QACRYPT_GLOBAL_H
