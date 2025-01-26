#ifndef GENO_DEBUG_P_H
#define GENO_DEBUG_P_H

#include <QtCore/qloggingcategory.h>

#define genoDebug()    qCDebug(geno).noquote().nospace()
#define genoInfo()     qCInfo(geno).noquote().nospace()
#define genoWarning()  qCWarning(geno).noquote().nospace()
#define genoCritical() qCCritical(geno).noquote().nospace()

Q_DECLARE_LOGGING_CATEGORY(geno)

#endif // GENO_DEBUG_P_H
