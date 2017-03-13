AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_CXXFLAGS = --pedantic -Wall -Werror
AM_LDFLAGS = $(BOOST_LDFLAGS)

check_PROGRAMS = gtest
gtest_SOURCES = gtest.cpp ../src/server.cxx ../src/client.cxx
gtest_CPPFLAGS = -I../include
gtest_LDADD = \
   $(BOOST_SYSTEM_LIB) \
   -lgtest \
   -lpthread

TESTS = gtest
