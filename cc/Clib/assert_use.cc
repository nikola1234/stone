#include <assert.h>
#include <iostream>
#include <string>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS   // c++代码中引用C标准库代码
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function) 
    __THROW __attribute__ ((__noreturn__));// Likewise, but prints the error text for ERRNUM.
__END_DECLS
#endif

//__typeof__(var) 是gcc对C语言的一个扩展保留字，用于声明变量类型,var可以是数据类型（int， char*..),也可以是变量表达式
#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

#endif

void assert_num (int a)
{
    assert(a == 0); // NDEBUG禁用assert
}
int main()
{
    MCHECK(1);
    //assert_num(1);
    return 0;
}