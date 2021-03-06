# 
# (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
#
# Author(s): Tim Mortsolf / Anil Gunturu
# Creation Date: 8/29/2013
# 

set(VALA_NAME basic_functor)
set(VALA_VERSION 1.0)
set(VALA_RELEASE 1)
set(VALA_LONG_NAME ${VALA_NAME}-${VALA_VERSION})

rift_pkg_check_modules(PEAS REQUIRED libpeas-1.0>=1.12.2)
include_directories(BEFORE
  ${PEAS_INCLUDE_DIRS})

##
# Specify libraries or flags to use when linking a given target
# The target must have been created in the current directory by a 
# command such as add_executable or add_library
##
set(basic_functor_libs
  ${VALA_NAME}-c
  ${VALA_LONG_NAME}
  ${PEAS_LIBRARIES})

add_definitions(
  -DINSTALLDIR="${CMAKE_INSTALL_PREFIX}"
  -DPLUGINDIR="${CMAKE_CURRENT_BINARY_DIR}/../vala"
  )

##
# Add the basic plugin google test
##
rift_gtest(basic_functor_gtest
  TEST_SRCS basic_functor_gtest.cc
  TEST_LIBS ${basic_functor_libs}
  DEPENDS ${VALA_NAME}-c ${VALA_LONG_NAME} ${VALA_NAME}-py
  )

##
# Add the basic plugin python test
##
rift_unittest(basic_functor_pytest
  TEST_ARGS python3 basic_functor_pytest.py
  TEST_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
