AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_CXXFLAGS = -Wall -Werror
AM_LDFLAGS = $(BOOST_LDFLAGS)

bin_PROGRAMS = primal
primal_SOURCES = primal.cxx client.cxx server.cxx
primal_CPPFLAGS = -I../include
primal_LDADD = \
   $(BOOST_SYSTEM_LIB) \
   $(BOOST_PROGRAM_OPTIONS_LIB)