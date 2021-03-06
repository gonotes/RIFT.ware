# 
# (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
#
# Author(s): Kiran Kashalkar
# Creation Date: 08/18/2015
# 

##
# DEPENDENCY ALERT
# The submodule dependencies must be specified in the
# .gitmodules.dep file at the top level (supermodule) directory
# If this submodule depends other submodules remember to update
# the .gitmodules.dep
##

cmake_minimum_required(VERSION 2.8)

##
# Set the path to the top level cmake modules directory
##
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/modules")

##
# DO NOT add any code before this and DO NOT
# include this file anywhere else
##
include(rift_submodule)

##
# Submodule specific includes will go here,
# These are specified here, since these variables are accessed
# from multiple sub directories. If the variable is subdirectory
# specific it must be declared in the subdirectory.
##

rift_externalproject_add(
  api
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/api
  CONFIGURE_COMMAND echo
  BUILD_COMMAND
    npm install
  INSTALL_COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/install_api.sh
    ${CMAKE_CURRENT_BINARY_DIR}/api/api-build
    ${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/api
    ${RIFT_SUBMODULE_INSTALL_PREFIX}/api/${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/api

  BCACHE_COMMAND echo
)

#rift_externalproject_add(
#  webapp
#  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/webapp
#  CONFIGURE_COMMAND echo
#  BUILD_COMMAND
#    ${CMAKE_CURRENT_BINARY_DIR}/webapp/webapp-build/scripts/build.sh
#  INSTALL_COMMAND
#    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/install_ui.sh
#    ${CMAKE_CURRENT_BINARY_DIR}/webapp/webapp-build
#    ${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/webapp
#    ${RIFT_SUBMODULE_INSTALL_PREFIX}/webapp/${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/webapp
#
#  BCACHE_COMMAND echo
#)

rift_externalproject_add(
  skyquake
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/skyquake
  CONFIGURE_COMMAND echo
  BUILD_COMMAND
    ${CMAKE_CURRENT_BINARY_DIR}/skyquake/skyquake-build/scripts/build.sh
  INSTALL_COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/install_ui.sh
    ${CMAKE_CURRENT_BINARY_DIR}/skyquake/skyquake-build
    ${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/skyquake
    ${RIFT_SUBMODULE_INSTALL_PREFIX}/skyquake/${CMAKE_INSTALL_PREFIX}/usr/share/rw.ui/skyquake

  BCACHE_COMMAND echo
)

##
# Include the subdirs
##
#set(subdirs
#  api
#  )
#rift_add_subdirs(SUBDIR_LIST ${subdirs})

##
# This macro adds targets for documentaion, unittests, code coverage and packaging
##
rift_add_submodule_targets(SUBMODULE_PACKAGE_NAME "rw.ui")
