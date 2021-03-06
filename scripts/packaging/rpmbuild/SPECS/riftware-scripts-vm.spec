# 
# (c) Copyright RIFT.io, 2013-2016, All Rights Reserved
#

%define _topdir           %{_rift_root}/.install/rpmbuild/
%define name              %{_rpmname}
%define release           1%{?dist}
#%%define release           %{_buildnum}%{?dist}
%define version           %{_version}
%define buildroot         %{_topdir}/%{name}-%{version}-root
%define DST_RIFT_ROOT     %{_dst_rift_root}
%define RIFT_ROOT         %{_rift_root}

%define _binaries_in_noarch_packages_terminate_build   0 	# http://winzter143.blogspot.com/2011/11/linux-arch-dependent-binaries-in-noarch.html


Name:     	%{name}
Version:  	%{version}
Release:  	%{release}
Summary:        RIFT.ware

License:        Apache
URL:            http://www.riftio.com
Source0:        %{name}-%{version}.tar.gz


#BuildArch: noarch
BuildRequires: yum

# turn this off when we have proper deps
AutoReqProv: no

Requires: riftware-base >= %{_version}
#Requires(post): info
#Requires(preun): info

Obsoletes: rift_scripts

%description
The %{name} program is a RIFT.ware package.


# This reads the sources and patches in the source directory %_sourcedir. It unpackages the sources to a subdirectory underneath the build directory %_builddir and applies the patches.
%prep

# This compiles the files underneath the build directory %_builddir. This is often implemented by running some variation of "./configure && make".
%build
echo "BuildDir: %_builddir"

#%setup


# This reads the files underneath the build directory %_builddir and writes to a directory underneath the build root directory %_buildrootdir. The files that are written are the files that are supposed to be installed when the binary package is installed by an end-user.
# Beware of the weird terminology: The build root directory is not the same as the build directory. This is often implemented by running "make install".
#
# this is a little wonky now but it works
#
%install
echo "buildroot: %{buildroot}"
echo "pwd: `pwd`";

# copy all copied source files
echo "running CMD: cp -Rp %{RIFT_ROOT}/.install/rpmbuild/SOURCES/%{name}-%{version}/* %{buildroot}/"
cp -Rp %{RIFT_ROOT}/.install/rpmbuild/SOURCES/%{name}-%{version}/* %{buildroot}/


%pre

# remove /home/rift/scripts if it is a symlink, comes from 4.0 code
SDIR="/home/rift/scripts"

if [ -d "$SDIR" ]; then
  if [ -L "$SDIR" ]; then
    # it is a symlink
    rm "$SDIR"
  fi
fi
# remove /home/rift/scripts if it is a symlink, comes from 4.0 code

# redundant NOW with posttrans work ???
# remove /usr/rift if it is a symlink, so we can create it later
#RDIR="/usr/rift"
#if [ -d "$RDIR" ]; then
#  if [ -L "$RDIR" ]; then
#    # it is a symlink
#    rm "$RDIR"
#  fi
#fi
# remove /usr/rift if it is a symlink, so we can create it later


#%post
# we actually need to do this AFTER the transaction runs to handle the case when rift_scripts is replaced in a single transaction
%posttrans

# setup symlinks
#ln -s /home/rift/ /usr/rift
# some ordering around install of this package and uninstall of rift_scripts is borking this link, let's try a force

RDIR="/usr/rift"

if [ -d "$RDIR" ]; then
  if [ -L "$RDIR" ]; then
    # it is a symlink
    rm "$RDIR"
  else
    # real dir, we probably don't want to rm it
    mv /usr/rift /usr/rift.rpmold-$$
  fi
fi

# recreate symlink
ln -sTf /home/rift/ /usr/rift

%postun
# cleanup symlinks on uninstall

# remove /usr/rift if it is a symlink
RDIR="/usr/rift"

if [ -d "$RDIR" ]; then
  if [ -L "$RDIR" ]; then
    # it is a symlink
    rm "$RDIR"
  fi
fi
# remove /usr/rift if it is a symlink



%files
/%{DST_RIFT_ROOT}/


%clean
rm -rf ${buildroot}

#rm -rf $RPM_BUILD_ROOT
#rm -rvf $RPM_BUILD_ROOT

# we may need some of this stuff down the road ?!?
# doc AUTHORS ChangeLog NEWS README THANKS TODO
# license COPYING

%changelog
* Thu Nov 12 2015 <Nate.Hudson@riftio.com> 4.0
- Initial version of the package

