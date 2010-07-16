# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

### Install Decapod 0.3

. ./_shared-utils.sh

DECAPOD_MODULES="libtiff-tools imagemagick"
GENPDF_PKG_NAME="decapod-genpdf"
STITCHING_PKG_NAME="decapod-stitching"
DECAPOD_VERSION="0.0-TRUNK"

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
# TODO: Need to optionally switch to a specific tag
if [ "$1" = "remove" ]; then
	uninstall_dpkg $GENPDF_PKG_NAME
else
	checkinstall -D -y --nodoc --pkgname $GENPDF_PKG_NAME --pkgversion $DECAPOD_VERSION scons -j 4 install; python setup.py install
fi
cd ../..

# Install Decapod Stitching script
cd decapod-stitching/
# TODO: Need to optionally switch to a specific tag (eg. "hg update decapod-0.3")
if [ "$1" = "remove" ]; then
	uninstall_dpkg $STITCHING_PKG_NAME
else
	checkinstall -D -y --nodoc --pkgname $STITCHING_PKG_NAME --pkgversion $DECAPOD_VERSION python setup.py install
fi
cd ..
