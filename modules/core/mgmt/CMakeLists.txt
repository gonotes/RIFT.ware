# 
# (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
#
# Author(s): Anil Gunturu
# Creation Date: 03/26/2014
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

##
# Include the subdirs
##
set(subdirs
  rwcli
  rwdebug-scripts
  rwrestconf
  rwuagent
  )

rift_add_subdirs(SUBDIR_LIST ${subdirs})


##
# This macro adds targets for documentaion, unittests, code coverage and packaging
##
rift_add_submodule_targets(SUBMODULE_PACKAGE_NAME "rw.core.mgmt")

# Workaround whatever mess rw.package is doing as it can't seem
# to figure out that it should make a directory -before- making
# symlinks..
set(dir usr/lib64/python${RIFT_PYTHON3}/site-packages/gi/overrides)
install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/.cpack-workaround
  DESTINATION ${dir})

if (RIFT_SUPPORT_PYTHON2)
  set(dir usr/lib64/python${RIFT_PYTHON2}/site-packages/gi/overrides)

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/.cpack-workaround
    DESTINATION ${dir})
endif()
