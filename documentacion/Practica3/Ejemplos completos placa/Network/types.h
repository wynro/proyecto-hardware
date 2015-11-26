#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef HANDLE
typedef void*			HANDLE;
#endif
#ifndef UCHAR
typedef unsigned char 	UCHAR;
#endif
#ifndef USHORT
typedef unsigned short 	USHORT;
#endif
#ifndef ULONG
typedef unsigned long 	ULONG;
#endif

#ifndef NULL
#define NULL 0
#endif

extern inline USHORT __byte_swap2(USHORT val);
extern inline ULONG  __byte_swap4(ULONG val);

/* brief Convert short value from host to network byte order. */
#define htons(x) __byte_swap2(x)

/* brief Convert long value from host to network byte order. */
#define htonl(x) __byte_swap4(x)

/* brief Convert short value from network to host byte order. */
#define ntohs(x) __byte_swap2(x)

/* brief Convert long value from network to host byte order. */
#define ntohl(x) __byte_swap4(x)

#endif /* _TYPES_H_ */
