# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

. ./_shared-utils.sh

LIBGPHOTO_VERSION="2.4.9"
GPHOTO_VERSION="2.4.9"

if [ "$1" = "remove" ]; then
	OPERATION=$1
else
	OPERATION="install"
fi

# Build and install libgphoto2 from source
download_and_install http://sourceforge.net/projects/gphoto/files/libgphoto/$LIBGPHOTO_VERSION/libgphoto2-$LIBGPHOTO_VERSION.tar.gz/download libgphoto2-$LIBGPHOTO_VERSION.tar.gz libgphoto2-$LIBGPHOTO_VERSION libgphoto2 $LIBGPHOTO_VERSION 

# Build and install gphoto2 from source
download_and_install http://sourceforge.net/projects/gphoto/files/gphoto/$GPHOTO_VERSION/gphoto2-$GPHOTO_VERSION.tar.gz/download gphoto2-$GPHOTO_VERSION.tar.gz gphoto2-$GPHOTO_VERSION gphoto2 $GPHOTO_VERSION
