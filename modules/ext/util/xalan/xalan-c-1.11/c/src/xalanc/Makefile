#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the  "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###################################################################
#                    IMPORTANT NOTE                               #
###################################################################
# Before you try to run the Makefile, make sure you have two      #
# environment variables set.                                      #
#                                                                 #
# export XALANCROOT=<the directory where you installed XALAN-C>   #
# export XERCESCROOT=<the directory where you installed XERCES-C> #
#                                                                 #
#                                                                 #
# If you are going to do the OS390BATCH build, make sure you have #
# the OS390BATCH environment variable set.                        #
#                                                                 #
#   export OS390BATCH=1                                           #
#                                                                 #
# Also,                                                           #
#                                                                 #
#   export LOADMOD=USERID.LOAD                                    #
#   export LOADEXP=USERID.EXP                                     #
#   export LOADSAMP=USERID.SAMP                                   #
#                                                                 #
# to specify your LOADMOD, the definition sidedeck dataset, and   #
# the testXSLT/Xalan executable destination.                      #
#                                                                 #
###################################################################

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}

DEPFILE=depends

# Output directory
OUTDIR := ../..


ifndef XALANCROOT 
noxalanroot:
	@echo XALANCROOT must be defined
endif

ifndef XERCESCROOT 
noxercesroot:
	@echo XERCESCROOT must be defined
endif

XALANDOM_DIR = $(XALANCROOT)/src/xalanc/XalanDOM
DOMSUPPORT_DIR = $(XALANCROOT)/src/xalanc/DOMSupport
INCLUDE_DIR = $(XALANCROOT)/src/xalanc/Include
ICUBRIDGE_DIR = $(XALANCROOT)/src/xalanc/ICUBridge
PLATFORMSUPPORT_DIR = $(XALANCROOT)/src/xalanc/PlatformSupport
XALANSOURCETREE_DIR = $(XALANCROOT)/src/xalanc/XalanSourceTree
TESTXPATH_DIR = $(XALANCROOT)/src/xalanc/TestXPath
XALANEXE_DIR = $(XALANCROOT)/src/xalanc/XalanExe
TESTXSLT_DIR = $(XALANCROOT)/src/xalanc/TestXSLT
XMLSUPPORT_DIR = $(XALANCROOT)/src/xalanc/XMLSupport
XPATH_DIR = $(XALANCROOT)/src/xalanc/XPath
XPATHCAPI_DIR = $(XALANCROOT)/src/xalanc/XPathCAPI
XSLT_DIR = $(XALANCROOT)/src/xalanc/XSLT
XALANEXTENSIONS_DIR = $(XALANCROOT)/src/xalanc/XalanExtensions
XERCESPARSERLIAISON_DIR = $(XALANCROOT)/src/xalanc/XercesParserLiaison
DEPRECATED_XERCESPARSERLIAISON_DIR = $(XALANCROOT)/src/xalanc/XercesParserLiaison/Deprecated
XALANEXSLT_DIR = $(XALANCROOT)/src/xalanc/XalanEXSLT
XALANTRANSFORMER_DIR = $(XALANCROOT)/src/xalanc/XalanTransformer
HARNESS_DIR = $(XALANCROOT)/src/xalanc/Harness
SAMPLES_DIR = ${XALANCROOT}/samples
TESTS_DIR = ${XALANCROOT}/Tests

ALL_VPATH_CPP = \
$(XALANDOM_DIR):$(PLATFORMSUPPORT_DIR):$(DOMSUPPORT_DIR):$(XMLSUPPORT_DIR): \
$(XPATH_DIR):$(XERCESPARSERLIAISON_DIR):$(DEPRECATED_XERCESPARSERLIAISON_DIR):$(XALANSOURCETREE_DIR):$(XPATHCAPI_DIR): \
$(XSLT_DIR):$(XALANEXTENSIONS_DIR):$(XALANEXSLT_DIR):$(XALANTRANSFORMER_DIR):$(HARNESS_DIR): \
$(TESTXPATH_DIR):$(TESTXSLT_DIR) \

ALLSOURCE = \
	$(wildcard $(XALANDOM_DIR)/*.cpp) \
	$(wildcard $(PLATFORMSUPPORT_DIR)/*.cpp) \
	$(wildcard $(DOMSUPPORT_DIR)/*.cpp) \
	$(wildcard $(XMLSUPPORT_DIR)/*.cpp) \
	$(wildcard $(XPATH_DIR)/*.cpp) \
	$(wildcard $(XERCESPARSERLIAISON_DIR)/*.cpp) \
	$(wildcard $(DEPRECATED_XERCESPARSERLIAISON_DIR)/*.cpp) \
	$(wildcard $(XALANSOURCETREE_DIR)/*.cpp) \
	$(wildcard $(XPATHCAPI_DIR)/*.cpp) \
	$(wildcard $(XSLT_DIR)/*.cpp) \
	$(wildcard $(XALANEXTENSIONS_DIR)/*.cpp) \
	$(wildcard $(XALANTRANSFORMER_DIR)/*.cpp) \
	$(wildcard $(XALANEXSLT_DIR)/*.cpp) \
	$(wildcard $(HARNESS_DIR)/*.cpp) \

ALL_HEADERS_DIRS = \
	DOMSupport \
	Include \
	PlatformSupport \
	XMLSupport \
	XPath \
	XSLT \
	XalanDOM \
	XalanEXSLT \
	XalanExtensions \
	XalanSourceTree \
	XercesParserLiaison \
	XercesParserLiaison/Deprecated \
	XalanTransformer \
    Harness \
	XPathCAPI

ALL_BINARIES = Xalan$(EXT)

include $(XALANCROOT)/version.incl
include ../../Makefile.incl

# Using ICUBridge
ifdef XALAN_USE_ICU
  ALL_VPATH_CPP+=:$(ICUBRIDGE_DIR)
  ALLSOURCE += $(wildcard $(ICUBRIDGE_DIR)/*.cpp) 
  ALL_HEADERS_DIRS += ICUBridge

  XSL_INCL += -I${ICUROOT}/include
  XSL_BUILD_OPTIONS += -DXALAN_USE_ICU
  ICU_LIBI18N_LIB=$(shell cd $(ICUROOT)/lib; ls libicui18n*[0-9][0-9].[0-9]*)
  ICU_LIBI18N_SO_NAME=$(shell cd $(ICUROOT)/lib; ls libicui18n*[0-9][0-9] libicui18n[0-9][0-9].? 2>/dev/null)
  ICU_LIBI18N_LINK_NAME=libicui18n$(SHLIBSUFFIX)
endif

ALL_OBJECTS = $(addprefix $(XSL_OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(ALLSOURCE)))))

vpath %.cpp \
$(ALL_VPATH_CPP)
${XSL_OBJ_DIR}/%.o:%.cpp
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

${XSL_OBJ_DIR}/%.o:%.hpp
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $(patsubst .hpp,.cpp,$<)

all:	 lib  testXSLT testXPath XalanExe

prepare:
	@echo Preparing the directory structure for a build ...
	-${MKDIR} -p ${XSL_OBJ_DIR}
	-${MKDIR} -p ${XSL_LIB_DIR}
	-${MKDIR} -p $(XSL_BIN_DIR)
	$(MAKE) -C Utils $@

# Create header dependencies file -- this must be run before any objects are
# built
depend:
	$(MAKE) -f makedepends DEPFILE=$(DEPFILE)

-include $(DEPFILE)	

compile:	 $(ALL_OBJECTS) 

ifeq (${OS390BATCH},1)
XalanExe: lib  "//'$(LOADSAMP)(${BATCH_XALAN_UTIL_NAME})'"

"//'$(LOADSAMP)(${BATCH_XALAN_UTIL_NAME})'": ${XSL_OBJ_DIR}/XalanExe.o
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS}  \
	${EXTRA_LINK_OPTIONS} $^ -o $@ $(XALAN_LIB) $(ALLLIBS)
${XSL_OBJ_DIR}/XalanExe.o: $(XALANEXE_DIR)/XalanExe.cpp                       
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

testXSLT: lib  "//'$(LOADSAMP)(${BATCH_TEST_UTIL_NAME})'"

"//'$(LOADSAMP)(${BATCH_TEST_UTIL_NAME})'": ${XSL_OBJ_DIR}/process.o
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS}  \
	${EXTRA_LINK_OPTIONS} $^ -o $@ $(XALAN_LIB) $(ALLLIBS)
${XSL_OBJ_DIR}/process.o: $(TESTXSLT_DIR)/process.cpp                       
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

else # Not OS390BATCH

XalanExe: lib  $(XSL_BIN_DIR)/Xalan

$(XSL_BIN_DIR)/Xalan: ${XSL_OBJ_DIR}/XalanExe.o
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS}  \
	${EXTRA_LINK_OPTIONS} $^ -o $@ $(XALAN_LIB) $(ALLLIBS)	$(OTHER_LINK_PARAMETERS)
${XSL_OBJ_DIR}/XalanExe.o: $(XALANEXE_DIR)/XalanExe.cpp
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<
endif

testXSLT: lib  $(XSL_BIN_DIR)/testXSLT

$(XSL_BIN_DIR)/testXSLT: ${XSL_OBJ_DIR}/process.o
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS}  \
	${EXTRA_LINK_OPTIONS} $^ -o $@ $(XALAN_LIB) $(ALLLIBS)	$(OTHER_LINK_PARAMETERS)
${XSL_OBJ_DIR}/process.o: $(TESTXSLT_DIR)/process.cpp
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

testXPath: lib  $(XSL_BIN_DIR)/testXPath

$(XSL_BIN_DIR)/testXPath: ${XSL_OBJ_DIR}/TestXPath.o ${XSL_OBJ_DIR}/NodeNameTreeWalker.o
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS}  \
	${EXTRA_LINK_OPTIONS} $^ -o $@ $(XALAN_LIB) $(ALLLIBS) $(CXXFLAGS)	$(OTHER_LINK_PARAMETERS)
${XSL_OBJ_DIR}/%.o:$(TESTXPATH_DIR)/%.cpp
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

## OS390BATCH
ifeq (${OS390BATCH},1)
lib:	prepare locale compile ${BATCH_TARGET}

locale:        
	$(MAKE) -C Utils $@ 

${BATCH_TARGET}: $(ALL_OBJECTS)
	@echo Building ${BATCH_TARGET}
	$(MAKE_SHARED) $^ -o $@ $(ALLLIBS)

else

lib:	prepare locale compile $(XSL_LIB_DIR)/$(LIB)

locale:	
	$(MAKE) -C Utils $@

$(XSL_LIB_DIR)/$(LIB): $(ALL_OBJECTS)
	$(MAKE_SHARED) $(PLATFORM_LIBRARIES) $(EXTRA_LINK_OPTIONS) $(LINK_SONAME) $^ -o $@ $(ALLLIBS) $(OTHER_LINK_LIB_PARAMERES)
ifeq ($(PLATFORM), OS390)
	mv *.x  ${XSL_LIB_DIR}        
else
	rm -f $(XSL_LIB_DIR)/$(LINK_NAME) && \
		$(LN) -fs $(LIB) $(XSL_LIB_DIR)/$(LINK_NAME)
	rm -f $(XSL_LIB_DIR)/$(SO_NAME) && \
		$(LN) -fs $(LIB) $(XSL_LIB_DIR)/$(SO_NAME)
endif ## OS390                                
endif ## OS390BATCH

install: lib XalanExe
	$(MAKE) -C Utils $@
	$(MKINSTALLDIRS) $(DESTDIR)/$(bindir)
	for bin in $(ALL_BINARIES); do \
	  $(INSTALL_PROGRAM) $(XSL_BIN_DIR)/$$bin \
                             $(DESTDIR)/$(bindir); \
	done
	$(MKINSTALLDIRS) $(DESTDIR)/$(libdir)
	$(INSTALL_PROGRAM) $(XSL_LIB_DIR)/$(LIB) $(DESTDIR)/$(libdir)
ifneq ($(PLATFORM), OS390)
	rm -f $(DESTDIR)/$(libdir)/$(LINK_NAME) && \
		$(LN) -fs $(LIB) $(DESTDIR)/$(libdir)/$(LINK_NAME)
	rm -f $(DESTDIR)/$(libdir)/$(SO_NAME) && \
		$(LN) -fs $(LIB) $(DESTDIR)/$(libdir)/$(SO_NAME)
endif ## OS390
	$(MKINSTALLDIRS) $(DESTDIR)/$(includedir)/xalanc
	for hdir in $(ALL_HEADERS_DIRS); do \
	  $(MKINSTALLDIRS) $(DESTDIR)/$(includedir)/xalanc/$$hdir; \
	  cp $(XALANCROOT)/src/xalanc/$$hdir/*.h* \
             $(DESTDIR)/$(includedir)/xalanc/$$hdir \
	     2> /dev/null; \
	done
ifdef XALAN_USE_ICU
	$(INSTALL_PROGRAM) $(ICUROOT)/lib/$(ICU_LIBI18N_LIB) \
                           $(DESTDIR)/$(libdir)
  ifneq ($PLATFORM), OS390)
	rm -f $(DESTDIR)/$(libdir)/$(ICU_LIBI18N_LINK_NAME) && \
		$(LN) -fs $(ICU_LIBI18N_LIB) \
			$(DESTDIR)/$(libdir)/$(ICU_LIBI18N_LINK_NAME)
	rm -f $(DESTDIR)/$(libdir)/$(ICU_LIBI18N_SO_NAME) && \
		$(LN) -fs $(ICU_LIBI18N_LIB) \
			$(DESTDIR)/$(libdir)/$(ICU_LIBI18N_SO_NAME)
  endif ## OS390
endif

clean:	prepare doClean

doClean:
	$(MAKE) -C Utils $@
	(cd $(XSL_OBJ_DIR); rm -rf *)
	rm -rf $(OUTDIR)/so_locations
	rm -rf $(XSL_BIN_DIR)/SunWS_cache
	rm -rf $(XSL_LIB_DIR)/SunWS_cache
	rm -f $(XSL_LIB_DIR)/$(LIB)
	rm -f $(XSL_LIB_DIR)/$(LINK_NAME)
	rm -f $(XSL_LIB_DIR)/$(SO_NAME)
	rm -f $(XSL_LIB_DIR)/$(LIB).map1
	rm -f $(XSL_BIN_DIR)/testXSLT$(EXT)
	rm -f $(XSL_BIN_DIR)/testXPath$(EXT)
	rm -f $(XSL_BIN_DIR)/Xalan$(EXT)
ifeq ($(PLATFORM), OS390)
	rm -f $(LIBRARY_LINK_NAME)
endif ## OS390

install-package: install
