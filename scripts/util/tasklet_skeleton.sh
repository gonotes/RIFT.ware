#! /bin/bash

# 
 * (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
 *
# Author(s): Anil Gunturu
# Creation Date: 07/24/2014
# 

# This script is used for installing openstack
# using devstack on a grunt machine.
# This script should be run as regular user.
# At high level this script does the following:
# - Clone the OpenStack repository
# - Run the devstack install scripts as root using the ssh_root script

set -e

function print_usage() {
    echo "Usage: $0 [-n NAME] [-o OUTDIR] "
    echo "       Mandatory arguments"
    echo "       -n|--name Name of the tasklet"
    echo "       Optional arguments"
    echo "       -o|--out-dir Output directory"
    exit 1
}

if [ -z "$RIFT_ROOT" ]; then
    echo "Error: You must be in a RIFT_SHELL to run this script."
    exit 1
fi

##
# Parse options
##
while [[ $# > 0 ]]
do
key="$1"
shift
case $key in
    -n|--name)
    NAME=$1
    shift
    ;;
    -o|--out-dir)
    OUTDIR=$1
    shift
    ;;
    *)
    print_usage
    ;;
esac
done

if [[ "$NAME" == "" ]]; then
    echo "Must specify the name of the tasklet"
    print_usage
fi

if [[ "$OUTDIR" == "" ]]; then
    OUTDIR="."
fi

if ! [[ "$NAME" =~ ^[a-z][a-z0-9_]*$ ]]; then
    echo "
Error: Tasklet name should be specified as lower case 
       alphanumeric string. The string may be separated by '_'."

    exit 1
fi

UPPERCASE_NAME=`echo ${NAME^^}`
CAMELCASE_NAME=`echo "$NAME" | sed -r 's/_([a-z0-9])/\u\1/g' | sed -r 's/^([a-z0-9])/\u\1/g'`
DATE=`date +"%m/%d/%Y"`


TASKLETDIR=$OUTDIR/plugins/$NAME-c
mkdir -p $TASKLETDIR
mkdir -p $OUTDIR/include/riftware
mkdir -p $OUTDIR/src

echo "Tasklet Name  : ${NAME}"
echo "Uppercase Name: ${UPPERCASE_NAME}"
echo "CamelCase Name: ${CAMELCASE_NAME}"
echo "Date          : ${DATE}"
echo "Tasklet Dir   : ${TASKLETDIR}"
echo ""

###################################################### 
echo "Generating ${TASKLETDIR}/Makefile"
echo "
# 
 * (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
 *
# Author(s): Autogenerated
# Creation Date: $DATE
# 

##
# Define a Makefile function: find_upwards(filename)
#
# Searches for a file of the given name in the directory ., .., ../.., ../../.., etc.,
# until the file is found or the root directory is reached
##
find_upward = \$(word 1, \$(shell while [ `pwd` != / ] ; do find `pwd` -maxdepth 1 -name \$1 ; cd .. ; done))

##
# Call find_upward("Makefile.top") to find the nearest upwards adjacent Makefile.top
##
makefile.top := \$(call find_upward, "Makefile.top")

##
# If Makefile.top was found, then include it
##
include \$(makefile.top)
" > $TASKLETDIR/Makefile

###################################################### 
echo "Generating ${TASKLETDIR}/CMakeLists.txt"

ABSOLUTE_OUTDIR=$(realpath ${OUTDIR})
if [[ "$ABSOLUTE_OUTDIR" =~ "modules/core/rwvx" ]]; then
    # Inside the modules/core/rwvx submodule.
    VAPIDIR="\${RIFT_SUBMODULE_BINARY_ROOT}/rwvcs/plugins/vala/"
else
    VAPIDIR="\${CMAKE_INSTALL_PREFIX}/usr/share/rift/vapi/"
fi

echo "
# 
 * (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
 *
# Author(s): Autogenerated
# Creation Date: $DATE
# 

##
# Set some RIFT specific variables for this cmake project
##
set(PLUGIN_NAME ${NAME}-c)

##
# Set the name of the Vala source for this cmake project; for use later in this file
##
set(C_SOURCES ${NAME}-c.c)

##
# Adds a library target to be built from the source files listed in the command invocation
# STATIC, SHARED, or MODULE may be given to specify the type of library to be created
##
add_library(\${PLUGIN_NAME} SHARED \${C_SOURCES})
target_link_libraries(\${PLUGIN_NAME} PRIVATE
  rw_tasklet_plugin-1.0
  rwtrace
  ${NAME}
  CoreFoundation
  glib-2.0
  peas-1.0)

##
# This function creates a vapi2c target to generate Vala source/header files from .in files
##
rift_add_plugin_vapi2c(
  \${PLUGIN_NAME}.vapi2c
  VAPI_FILE \${RIFT_SUBMODULE_BINARY_ROOT}/rwvcs/plugins/vala/rw_tasklet_plugin-1.0.vapi
  PLUGIN_SOURCE_PREFIX \${PLUGIN_NAME}
  PLUGIN_PREFIX ${CAMELCASE_NAME}
  DEPENDS rw_tasklet_plugin-1.0
  )

##
# This function builds the plugin schema files
##
rift_add_c_plugin_schema(
  \${PLUGIN_NAME}
  GSCHEMA \${PLUGIN_NAME}.gschema.xml
  PLUGIN \${PLUGIN_NAME}.plugin
  )

##
# This function creates an install target for the plugin artifacts
##
rift_install_plugin_artifacts(
  \${PLUGIN_NAME}
  SOTARGETS \${PLUGIN_NAME}
  GSCHEMAFILES \${CMAKE_CURRENT_BINARY_DIR}/\${PLUGIN_NAME}.gschema.xml
  PLUGINFILES \${CMAKE_CURRENT_BINARY_DIR}/\${PLUGIN_NAME}.plugin
  COMPONENT \${PKG_LONG_NAME}
  )

" > $TASKLETDIR/CMakeLists.txt

###################################################### 
# Output the .gschema.xml
echo "Generating ${TASKLETDIR}/${NAME}-c.gschema.xml"
echo "
<schemalist>
  <schema id=\"${NAME}-c\" path=\"/org/riftio/${NAME}-c\">
    <key name=\"a-setting\" type=\"s\">
      <default>'Blah'</default>
      <summary>Just a setting.</summary>
      <description>Just a setting.</description>
    </key>
  </schema>
</schemalist>
" > $TASKLETDIR/$NAME-c.gschema.xml

###################################################### 
# Output the .plugin
echo "Generating ${TASKLETDIR}/${NAME}-c.plugin"
echo "
[Plugin]
Module=${NAME}-c
Name=${NAME}
Description=This plugin 'c' plugin for ${NAME} tasklet
Authors=RIFT.io
Copyright=Copyright © 2014 RIFT.io
Website=http://www.riftio.com
" > $TASKLETDIR/$NAME-c.plugin

###################################################### 
# Output the .in.h
echo "Generating ${TASKLETDIR}/${NAME}-c.in.h"
echo "
/*
 * $NAME-c.h
 */

#ifndef __${UPPERCASE_NAME}_C_H__
#define __${UPPERCASE_NAME}_C_H__

#include <libpeas/peas.h>
#include \"${NAME}.h\"

#define ${UPPERCASE_NAME}_TYPE_COMPONENT RW_TASKLET_PLUGIN_TYPE_COMPONENT
typedef RwTaskletPluginComponentIface ${CAMELCASE_NAME}ComponentIface;

#define VAPI_TO_C_AUTOGEN
#ifdef VAPI_TO_C_AUTOGEN

#endif //VAPI_TO_C_AUTOGEN

#endif /* __${UPPERCASE_NAME}_C_H__ */

" > $TASKLETDIR/$NAME-c.in.h

###################################################### 
# Output the .in.c
echo "Generating ${TASKLETDIR}/${NAME}-c.in.c"
echo "

/*
 * 
 * (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
 *
 */



/**
 * @file $NAME-c.in.c
 * @author Autogenerated
 * @date $DATE
 * @brief Add description here
 */

#include \"$NAME-c.h\"

RW_CF_TYPE_DEFINE(\"${CAMELCASE_NAME} tasklet component type\", ${NAME}_component_ptr_t);

RW_CF_TYPE_DEFINE(\"${CAMELCASE_NAME} tasklet instance type\", ${NAME}_instance_ptr_t);

RwTaskletPluginComponentHandle *${NAME}__component__component_init(RwTaskletPluginComponent *self)
{
  ${NAME}_component_ptr_t component;
  RwTaskletPluginComponentHandle *h_component;
  gpointer handle;

  // Register the RW.Init types
  RW_CF_TYPE_REGISTER(${NAME}_component_ptr_t);
  RW_CF_TYPE_REGISTER(${NAME}_instance_ptr_t);

  // Allocate a new ${NAME}_component structure
  component = ${NAME}_component_init();
  RW_CF_TYPE_VALIDATE(component, ${NAME}_component_ptr_t);

  // Allocate a gobject for the handle
  handle = g_object_new(RW_TASKLET_PLUGIN_TYPE_COMPONENT_HANDLE, 0);
  h_component = RW_TASKLET_PLUGIN_COMPONENT_HANDLE(handle);
  h_component->priv = (gpointer) component;

  // Return the handle to the rwtasklet component
  return h_component;
}

void ${NAME}__component__component_deinit(
        RwTaskletPluginComponent *self,
        RwTaskletPluginComponentHandle *h_component)
{
}

static RwTaskletPluginInstanceHandle *${NAME}__component__instance_alloc(
        RwTaskletPluginComponent *self, 
        RwTaskletPluginComponentHandle *h_component, 
        RwTaskletPlugin_RWTaskletInfo *tasklet_info, 
        RwTaskletPlugin_RWExecURL *instance_url)
{
  ${NAME}_component_ptr_t component;
  ${NAME}_instance_ptr_t  instance;
  RwTaskletPluginInstanceHandle *h_instance;
  gpointer handle;

  // Validate input parameters
  component = (${NAME}_component_ptr_t) h_component->priv;
  RW_CF_TYPE_VALIDATE(component, ${NAME}_component_ptr_t);

  // Allocate a new ${NAME}_instance structure
  instance = ${NAME}_instance_alloc(component, tasklet_info, instance_url);
  RW_CF_TYPE_VALIDATE(instance, ${NAME}_instance_ptr_t);
  instance->component = component;

  // Allocate a gobject for the handle
  handle = g_object_new(RW_TASKLET_PLUGIN_TYPE_INSTANCE_HANDLE, 0);
  h_instance = RW_TASKLET_PLUGIN_INSTANCE_HANDLE(handle);
  h_instance->priv = (gpointer) instance;

  // Return the handle to the rwtasklet instance
  return h_instance;
}

void ${NAME}__component__instance_free(
        RwTaskletPluginComponent *self,
        RwTaskletPluginComponentHandle *h_component,
        RwTaskletPluginInstanceHandle *h_instance)
{
}

static void ${NAME}__component__instance_start(
        RwTaskletPluginComponent *self, 
        RwTaskletPluginComponentHandle *h_component, 
        RwTaskletPluginInstanceHandle *h_instance)
{
  ${NAME}_component_ptr_t component;
  ${NAME}_instance_ptr_t  instance;

  // Validate input parameters
  component = (${NAME}_component_ptr_t) h_component->priv;
  RW_CF_TYPE_VALIDATE(component, ${NAME}_component_ptr_t);

  instance = (${NAME}_instance_ptr_t) h_instance->priv;
  RW_CF_TYPE_VALIDATE(instance, ${NAME}_instance_ptr_t);

  // Call the tasklet instance start routine
  ${NAME}_instance_start(component, instance);

  RWTRACE_INFO(instance->rwtasklet_info->rwtrace_instance,
               RWTRACE_CATEGORY_RWTASKLET,
               \"${NAME} -- tasklet [%d] is started!\",
               instance->rwtasklet_info->identity.rwtasklet_instance_id);

}

static void ${NAME}__component__instance_stop(
        RwTaskletPluginComponent *self, 
        RwTaskletPluginComponentHandle *h_component, 
        RwTaskletPluginInstanceHandle *h_instance)
{
  ${NAME}_component_ptr_t component;
  ${NAME}_instance_ptr_t  instance;

  // Validate input parameters
  component = (${NAME}_component_ptr_t) h_component->priv;
  RW_CF_TYPE_VALIDATE(component, ${NAME}_component_ptr_t);

  instance = (${NAME}_instance_ptr_t) h_instance->priv;
  RW_CF_TYPE_VALIDATE(instance, ${NAME}_instance_ptr_t);
}


static void ${NAME}_c_extension_init(${CAMELCASE_NAME}CExtension *plugin)
{
}

static void ${NAME}_c_extension_class_init(${CAMELCASE_NAME}CExtensionClass *klass)
{
}

static void ${NAME}_c_extension_class_finalize(${CAMELCASE_NAME}CExtensionClass *klass)
{
}

#define VAPI_TO_C_AUTOGEN
#ifdef VAPI_TO_C_AUTOGEN

#endif //VAPI_TO_C_AUTOGEN
" > $TASKLETDIR/$NAME-c.in.c

###################################################### 
echo "Generating ${OUTDIR}/src/${NAME}.c"
echo "

#include <$NAME.h>

/**
 * @file $NAME.c
 * @author Atogenerated
 * @date $DATE
 * @brief Add description here
 */

${NAME}_component_ptr_t ${NAME}_component_init(void) 
{
  ${NAME}_component_ptr_t component = NULL;

  /* ADD CODE HERE */

  return component;
}

void ${NAME}_component_deinit(${NAME}_component_ptr_t component)
{
  /* ADD CODE HERE */
  return;
}

${NAME}_instance_ptr_t ${NAME}_instance_alloc(
        ${NAME}_component_ptr_t component, 
        RwTaskletPlugin_RWTaskletInfo *rwtasklet_info, 
        RwTaskletPlugin_RWExecURL *instance_url)
{
  ${NAME}_instance_ptr_t instance = NULL;

  /* ADD CODE HERE */

  return instance;
}

void ${NAME}_instance_free(
        ${NAME}_component_ptr_t component, 
        ${NAME}_instance_ptr_t instance)
{
  /* ADD CODE HERE */
  return;
}

void ${NAME}_instance_start(
        ${NAME}_component_ptr_t component, 
        ${NAME}_instance_ptr_t instance)
{
  /* ADD CODE HERE */
  return;
}
" > ${OUTDIR}/src/${NAME}.c

###################################################### 
echo "Generating ${OUTDIR}/include/riftware/${NAME}.h"
echo "
#ifndef __${NAME}_H__
#define __${NAME}_H__

#include \"rwtasklet.h\"

struct ${NAME}_component_s {
  CFRuntimeBase _base;
  /* ADD ADDITIONAL FIELDS HERE */
};

RW_TYPE_DECL(${NAME}_component);
RW_CF_TYPE_EXTERN(${NAME}_component_ptr_t);


struct ${NAME}_instance_s {
  CFRuntimeBase _base;
  rwtasklet_info_ptr_t rwtasklet_info;
  ${NAME}_component_ptr_t component;

  /* ADD ADDITIONAL FIELDS HERE */
};

RW_TYPE_DECL(${NAME}_instance);
RW_CF_TYPE_EXTERN(${NAME}_instance_ptr_t);

${NAME}_component_ptr_t ${NAME}_component_init(void);

void ${NAME}_component_deinit(${NAME}_component_ptr_t component);

${NAME}_instance_ptr_t ${NAME}_instance_alloc(
        ${NAME}_component_ptr_t component, 
        RwTaskletPlugin_RWTaskletInfo *rwtasklet_info, 
        RwTaskletPlugin_RWExecURL *instance_url);

void ${NAME}_instance_free(
        ${NAME}_component_ptr_t component, 
        ${NAME}_instance_ptr_t instance);

void ${NAME}_instance_start(
        ${NAME}_component_ptr_t component, 
        ${NAME}_instance_ptr_t instance);

#endif //__${NAME}_H__
" > ${OUTDIR}/include/riftware/${NAME}.h
