#ifndef _BITS_SYSCALLS_H
#define _BITS_SYSCALLS_H
#ifndef _SYSCALL_H
# error "Never use <bits/syscalls.h> directly; include <sys/syscall.h> instead."
#endif

/*
   Some of the sneaky macros in the code were taken from
   glibc-2.2.5/sysdeps/unix/sysv/linux/i386/sysdep.h
*/

#ifndef __ASSEMBLER__

#include <errno.h>

#define SYS_ify(syscall_name)  (__NR_##syscall_name)

#define INTERNAL_SYSCALL_DECL(err) do { } while (0)

#define INTERNAL_SYSCALL_ERROR_P(val, err) \
  ((unsigned int) (val) >= 0xfffff001u)

#define INTERNAL_SYSCALL_ERRNO(val, err)        (-(val))

/* We need some help from the assembler to generate optimal code.  We
   define some macros here which later will be used.  */

#if defined  __SUPPORT_LD_DEBUG__ && defined __DOMULTI__
#error LD debugging and DOMULTI are incompatible
#endif

#ifdef __DOMULTI__
__asm__ (".L__X'%ebx = 1\n\t"
     ".L__X'%ecx = 2\n\t"
     ".L__X'%edx = 2\n\t"
     ".L__X'%eax = 3\n\t"
     ".L__X'%esi = 3\n\t"
     ".L__X'%edi = 3\n\t"
     ".L__X'%ebp = 3\n\t"
     ".L__X'%esp = 3\n\t"
     ".ifndef _BITS_SYSCALLS_ASM\n\t"
     ".set _BITS_SYSCALLS_ASM,1\n\t"
     ".macro bpushl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "pushl %ebx\n\t"
     ".else\n\t"
     "xchgl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t"
     ".macro bpopl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "popl %ebx\n\t"
     ".else\n\t"
     "xchgl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t"
     ".macro bmovl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "movl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t"
     ".endif\n\t");
#else
__asm__ (".L__X'%ebx = 1\n\t"
     ".L__X'%ecx = 2\n\t"
     ".L__X'%edx = 2\n\t"
     ".L__X'%eax = 3\n\t"
     ".L__X'%esi = 3\n\t"
     ".L__X'%edi = 3\n\t"
     ".L__X'%ebp = 3\n\t"
     ".L__X'%esp = 3\n\t"
     ".macro bpushl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "pushl %ebx\n\t"
     ".else\n\t"
     "xchgl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t"
     ".macro bpopl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "popl %ebx\n\t"
     ".else\n\t"
     "xchgl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t"
     ".macro bmovl name reg\n\t"
     ".if 1 - \\name\n\t"
     ".if 2 - \\name\n\t"
     "movl \\reg, %ebx\n\t"
     ".endif\n\t"
     ".endif\n\t"
     ".endm\n\t");
#endif

#undef _syscall0
#define _syscall0(type,name) \
type name(void) \
{ \
return (type) (INLINE_SYSCALL(name, 0)); \
}

#undef _syscall1
#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
return (type) (INLINE_SYSCALL(name, 1, arg1)); \
}

#undef _syscall2
#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
return (type) (INLINE_SYSCALL(name, 2, arg1, arg2)); \
}

#undef _syscall3
#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
return (type) (INLINE_SYSCALL(name, 3, arg1, arg2, arg3)); \
}

#undef _syscall4
#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
return (type) (INLINE_SYSCALL(name, 4, arg1, arg2, arg3, arg4)); \
}

#undef _syscall5
#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
return (type) (INLINE_SYSCALL(name, 5, arg1, arg2, arg3, arg4, arg5)); \
}

#undef _syscall6
#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5, type6 arg6) \
{ \
return (type) (INLINE_SYSCALL(name, 6, arg1, arg2, arg3, arg4, arg5, arg6)); \
}
 #define INLINE_SYSCALL(name, nr, args...) \
  ({                                                                          \
    unsigned int _resultvar = INTERNAL_SYSCALL (name, , nr, args);            \
    if (__builtin_expect (INTERNAL_SYSCALL_ERROR_P (_resultvar, ), 0))        \
      {                                                                       \
        __set_errno (INTERNAL_SYSCALL_ERRNO (_resultvar, ));                  \
        _resultvar = 0xffffffff;                                              \
      }                                                                       \
    (int) _resultvar; })

#define INTERNAL_SYSCALL(name, err, nr, args...) \
  ({                                                                          \
    size_t strlen(const char*);						\
    int write(int, const __ptr_t, size_t);				\
    write(1, "unimp "#name "\n", 7 + strlen(#name));			\
    0;									\
  })

#define LOADARGS_0
#define LOADARGS_1 \
    "bpushl .L__X'%k2, %k2\n\t"						      \
    "bmovl .L__X'%k2, %k2\n\t"
#define LOADARGS_2	LOADARGS_1
#define LOADARGS_3	LOADARGS_1
#define LOADARGS_4	LOADARGS_1
#define LOADARGS_5	LOADARGS_1
#define LOADARGS_6	LOADARGS_1 "push %%ebp ; movl %7, %%ebp\n\t"

#define RESTOREARGS_0
#define RESTOREARGS_1 \
    "bpopl .L__X'%k2, %k2\n\t"
#define RESTOREARGS_2	RESTOREARGS_1
#define RESTOREARGS_3	RESTOREARGS_1
#define RESTOREARGS_4	RESTOREARGS_1
#define RESTOREARGS_5	RESTOREARGS_1
#define RESTOREARGS_6	"pop %%ebp\n\t" RESTOREARGS_1

#define ASMFMT_0()
#define ASMFMT_1(arg1) \
	, "acdSD" (arg1)
#define ASMFMT_2(arg1, arg2) \
	, "adCD" (arg1), "c" (arg2)
#define ASMFMT_3(arg1, arg2, arg3) \
	, "aCD" (arg1), "c" (arg2), "d" (arg3)
#define ASMFMT_4(arg1, arg2, arg3, arg4) \
	, "aD" (arg1), "c" (arg2), "d" (arg3), "S" (arg4)
#define ASMFMT_5(arg1, arg2, arg3, arg4, arg5) \
	, "a" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5)
#define ASMFMT_6(arg1, arg2, arg3, arg4, arg5, arg6) \
	, "a" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5), "m" (arg6)

#endif /* __ASSEMBLER__ */
#endif /* _BITS_SYSCALLS_H */
