# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

### Install Decapod 0.5a

. ./_shared-utils.sh

DECAPOD_MODULES="libtiff-tools imagemagick"
CHERRYPY_PKG_NAME="cherrypy-3.2.2"
GENPDF_PKG_NAME="decapod-genpdf"
DECAPOD_NAME="decapod-0.5a"
DECAPOD_VERSION=$(echo $DECAPOD_NAME | sed -e 's/.*-//')

if [ "$1" = "remove" ]; then
    OPERATION=$1
else
    OPERATION="install"
fi

# Get some Decapod-specific dependencies
install_packages $DECAPOD_MODULES

# Back out to the main Decapod directories.
cd ../..

# Install Decapod PDF Generation Script
cd decapod-genpdf/src/
hg update $DECAPOD_NAME

if [ "$1" = "remove" ]; then
    uninstall_dpkg $GENPDF_PKG_NAME
else
    checkinstall -D -y --nodoc --pkgname $GENPDF_PKG_NAME --pkgversion $DECAPOD_VERSION scons -j 4 install; python setup.py install
fi
cd ../..

#install cherrypy
if [ "$1" = "remove"]; then
    uninstall_dpkg $CHERRYPY_PKG_NAME
else
    wget http://download.cherrypy.org/CherryPy/3.2.2/CherryPy-3.2.2.tar.gz
    tar -zxvf CherryPy-3.2.2.tar.gz
    rm CherryPy-3.2.2.tar.gz
    cd CherryPy-3.2.2
    checkinstall -D -y --nodoc --pkgname $CHERRYPY_PKG_NAME --pkgversion $DECAPOD_VERSION python setup.py install
    cd ..
    rm -rf CherryPy-3.2.2
fi
