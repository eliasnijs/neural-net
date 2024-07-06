// DEPENDENCY: "<stdio.h>"
// DEPENDENCY: "./types.h"

////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Debug Utilities

#if ENABLE_ASSERT
#define AssertBreak() (*((I32 *)0x0) = 0)
#define Assert(c) Stmnt(\
			if (!(c)){\
                          fprintf(stderr, "assertion error: %s %d, ", __FILE__, __LINE__);\
                          fprintf(stderr, "`"#c"`\n");\
                          AssertBreak();\
			})
#else
#define Assert(c)
#endif

#if ENABLE_PRINT
#define print(...)		printf(__VA_ARGS__);\
				printf("\n");\
				fflush(stdout);

#define print_error(...)	fprintf(stderr, ANSI_ESC_RGB_BG(180, 60, 60));\
				fprintf(stderr, "[error]# ");\
				fprintf(stderr, __VA_ARGS__);\
				fprintf(stderr, ANSI_ESC_RGB_FG(180, 180, 180));\
				fprintf(stderr, " (%s %d)", __FILE__, __LINE__);\
				fprintf(stderr, ANSI_ESC_RESET_ALL);\
				fprintf(stderr, "\n");\
				fflush(stderr);
#define print_info(...)		fprintf(stderr, ANSI_ESC_RGB_BG(60, 60, 180));\
				fprintf(stderr, "[info]# ");\
				fprintf(stderr, __VA_ARGS__);\
				fprintf(stderr, ANSI_ESC_RGB_FG(180, 180, 180));\
				fprintf(stderr, " (%s %d)", __FILE__, __LINE__);\
				fprintf(stderr, ANSI_ESC_RESET_ALL);\
				fprintf(stderr, "\n");\
				fflush(stderr);
#else
#define print(...)
#define print_error(...)
#define print_info(...)
#endif

#if ENABLE_DEBUGLOG
#define PrintInt64(a)		printf(#a" = %ld\n", (Int64)a)
#define PrintUint64(a)		printf(#a" = %lu\n", (Uint64)a)
#define PrintFloat64(a)		printf(#a" = %f\n", (Float64)a)
#define PrintChar(s)		printf(#s" = %c\n", (char)s)
#define PrintString(s)		printf(#s" = %s\n", (char *)s)
#define PrintBool(b)		printf(#b" = %s\n", (b)?"true":"false")
#define PrintPtr(p)		printf(#p" = %p\n", p)
#define PrintLine(p)		printf("──────────────────────────────"\
					"────────────────────────────\n")
#else
#define PrintInt64(a)
#define PrintUint64(a)
#define PrintFloat64(a)
#define PrintChar(s)
#define PrintString(s)
#define PrintBool(b)
#define PrintPtr(p)
#define PrintLine(p)
#endif
