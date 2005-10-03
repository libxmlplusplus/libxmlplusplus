Building xml++ with Borland C++ compiler

1. make sure that you have installed libxml2 and iconv headers. They should be placed like this:
   <base folder>
    |
    |- iconv
    |  |
    |  |--include
    |
    |- libxml2
    |  |
    |  |--include
    |
    |- libxml++

2. switch to win32_bcc folder inside libxml++ and run make. That will build all versions
   of static libraries. 

Here is the list of built static libraries:

libxmlpp_bcc_Debug_rtldll.lib       - Debug library with dynamic RTL singlethreaded
libxmlpp_bcc_Debug_rtldll_mt.lib    - Debug library with dynamic RTL multithreaded
libxmlpp_bcc_Debug_rtlstatic.lib    - Debug library with static RTL singlethreaded
libxmlpp_bcc_Debug_rtlstatic_mt.lib - Debug library with static RTL multithreaded
libxmlpp_bcc_rtldll.lib             - Release library with dynamic RTL singlethreaded
libxmlpp_bcc_rtldll_mt.lib          - Release library with dynamic RTL multithreaded
libxmlpp_bcc_rtlstatic.lib          - Release library with static RTL singlethreaded
libxmlpp_bcc_rtlstatic_mt.lib       - Release library with static RTL multithreaded

To simplify linking to the propper library you can add this peace of code after including libxml++ header(s)

#ifdef __BORLANDC__

//Are we multithreaded here?
#ifdef __MT__
#define MTEXT "_mt"
#else
#define MTEXT ""
#endif

#ifdef _RTLDLL
#define RTLTYPE "_rtldll"MTEXT".lib"
#else
#define RTLTYPE "_rtlstatic"MTEXT".lib"
#endif

#ifdef _DEBUG
#define LIBNAME "libxmlpp_bcc_Debug"RTLTYPE
#else
#define LIBNAME "libxmlpp_bcc"RTLTYPE
#endif

//Which libraries we must link
//#pragma comment(lib, "libxml2.lib")
#pragma comment(lib, LIBNAME     )

#endif //__BORLANDC__

Darko