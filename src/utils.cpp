typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;
typedef unsigned long	U64;
typedef signed char	I8;
typedef signed short	I16;
typedef signed int	I32;
typedef signed long	I64;
typedef float		F32;
typedef double		F64;
typedef signed char	B8;
typedef signed short	B16;
typedef signed int	B32;
typedef signed long	B64;

#define internal	static
#define global_variable static
#define local_persist	static

#define true 1
#define false 0

#define MAX_F32 3.402823466e+38f

#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

