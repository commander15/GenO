#ifndef GENO_GLOBAL_H
#define GENO_GLOBAL_H

#include <GenO/config.h>

#ifdef GENO_SHARED
#ifdef GENO_BUILD
#define GENO_EXPORT Q_DECL_EXPORT
#else
#define GENO_EXPORT Q_DECL_IMPORT
#endif
#else
#define GENO_EXPORT
#endif

#define GENO_Q(ClassName) ClassName *q = static_cast<ClassName *>(q_ptr)
#define GENO_D(ClassName) \
    ClassName##Private *d = static_cast<ClassName##Private *>(qGetPtrHelper(d_ptr))

#endif // GENO_GLOBAL_H
