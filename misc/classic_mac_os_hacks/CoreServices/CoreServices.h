// HACK: Mac OS X's CoreServices.h is used in ode for functionality that is also avalible in Classic Mac OS's Timer.h.
#ifdef CLASSIC_MAC_OS
#include <Timer.h>
#else
// todo: re-export the real CoreServices.h here, somehow.
#endif