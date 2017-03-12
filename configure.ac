AC_PREREQ([2.69])
AC_INIT([primal], [1.0], [aschult5@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

# Checks programs
AC_PROG_CXX
AC_LANG_PUSH(C++)
AX_CXX_COMPILE_STDCXX_14()
AC_LANG_POP()

# Check libraries
AX_BOOST_BASE([1.63],, [AC_MSG_ERROR([primal needs Boost, but it was not found in your system])])
AX_BOOST_SYSTEM
AX_BOOST_ASIO
AX_BOOST_PROGRAM_OPTIONS

AC_OUTPUT