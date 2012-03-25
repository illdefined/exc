#include <setjmp.h>
#include <stdlib.h>

#include <defy/expect>
#include <defy/thread_local>

extern thread_local void *__exc_head;

#define exc_try { \
		jmp_buf  __exc_node; \
		void *__exc_save = __exc_head; \
		__exc_head = __exc_node; \
		int __exc_type = setjmp(__exc_node); \
		switch (__exc_type) { \
		case 0:

#define exc_catch(type) \
			break; \
		case (type):

#define exc_throw(type) \
	do { \
		if (likely(__exc_head)) \
			longjmp(__exc_head, (type)); \
		else \
			abort(); \
	} while (0)

#define exc_end \
			break; \
		default: \
			__exc_head = __exc_save; \
			exc_throw(__exc_type); \
		} \
		__exc_head = __exc_save; \
	} while (0)
