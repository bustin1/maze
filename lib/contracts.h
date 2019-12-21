
#include <assert.h>

#undef ASSERT
#undef REQUIRES
#undef ENSURES

#ifdef DEBUG
#define ASSERT(COND) assert(COND)
#define REQUIRES(COND) assert(COND)
#define ENSURES(COND) assert(COND)

#else
#define ASSERT(COND) assert((void)0)
#define REQUIRES(COND) assert((void)0)
#define ENSURES(COND) assert((void)0)

#endif

