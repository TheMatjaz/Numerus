#parse("Doxygen file header.h")

#if (${HEADER_FILENAME})
#[[#include]]# "${HEADER_FILENAME}"
#end
