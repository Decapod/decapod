# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

. ./_shared-utils.sh

# Third-party packages to get via apt
SCM='cvs mercurial subversion'
BUILDS="scons autoconf"
SYSTEM="hardinfo hwinfo checkinstall valgrind"
C_LIBS="libtool gettext libusb-dev libpopt-dev"
PYTHON_LIBS="swig python-tk python-cherrypy3 python-numpy python-imaging python-scipy python-matplotlib python-reportlab python-opencv python-gnome2 python-simplejson"

if [ "$1" = "remove" ]; then
	OPERATION=$1
else
	OPERATION="install"
fi


# Install all third-party packages via apt-get
install_packages $SCM $BUILDS $SYSTEM $C_LIBS $PYTHON_LIBS
