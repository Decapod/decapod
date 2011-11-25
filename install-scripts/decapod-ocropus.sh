# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

. ./_shared-utils.sh

OCROPUS_TAG="ocropus-0.4.4"
OCROPUS_VERSION=$(echo $OCROPUS_TAG | sed -e 's/.*-//')
if [ "$1" = "remove" ]; then
	OPERATION=$1
else
	OPERATION="install"
	mkdir $OCROPUS_TAG
	cd $OCROPUS_TAG
fi

# args: 1 download_url, 2 directory_name
clone_and_install_ocropus_lib() {
	if [ "$OPERATION" = "remove" ]; then
		uninstall_dpkg $2
	else
		hg clone -r $OCROPUS_TAG $1 $2
		cd $2
		sh ubuntu-packages # TODO: This OCRopus-specific script requires user intervention. Replace it or fix it.
		checkinstall -D -y --nodoc --pkgname $2 --pkgversion $OCROPUS_VERSION scons -j 4 sdl=1 install
		cd ..
	fi
}

# args: 1 download_url, 2 directory_name
clone_and_install_python_lib() {
	if [ "$OPERATION" = "remove" ]; then
		uninstall_dpkg $2
	else
		hg clone -r $OCROPUS_TAG $1 $2
		cd $2
		checkinstall -D -y --nodoc --pkgname $2 --pkgversion $OCROPUS_VERSION python setup.py install
		cd ..
	fi
}

# args: 1 download_url, 2 directory_name
clone_and_make_ocropus_lib() {
	if [ "$OPERATION" = "remove" ]; then
		uninstall_dpkg $2
	else
		hg clone -r $OCROPUS_TAG $1 $2
		cd $2
		checkinstall -D -y --nodoc --pkgname $2 --pkgversion $OCROPUS_VERSION make
		cd ..
	fi
}

clone_and_install_ocropus_lib https://iulib.googlecode.com/hg/ iulib
clone_and_install_ocropus_lib https://ocropus.googlecode.com/hg/ ocropus
download_and_install https://launchpad.net/debian/+archive/primary/+files/openfst_1.1.orig.tar.gz  openfst-1.1.tar.gz openfst-1.1 openfst 1.1
clone_and_make_ocropus_lib https://pyopenfst.googlecode.com/hg/ pyopenfst
clone_and_make_ocropus_lib https://ocroswig.ocropus.googlecode.com/hg/ ocroswig
clone_and_install_python_lib https://ocropy.ocropus.googlecode.com/hg/ ocropy
