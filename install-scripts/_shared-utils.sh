# Copyright 2010 University of Toronto
#
# Licensed under the Educational Community License (ECL), Version 2.0 or the New BSD license. 
# You may not use this file except in compliance with one these Licenses.
#
# You may obtain a copy of the ECL 2.0 License and BSD License at
# https://source.fluidproject.org/svn/LICENSE.txt

# Generic function for installing modules via apt-get
install_packages() {
	echo
	echo
	for package in $@
	do
		sudo apt-get -y $OPERATION $package
	done
}

uninstall_dpkg() {
	dpkg -r $1
}

install_source() {		
		wget $1 -O $2
		tar -zxvf $2
		cd $3
		if [ $4 = "gphoto2" ]; then
      ./configure --with-libgphoto2=/usr/local
    else
      ./configure
    fi
		make
		checkinstall --fstrans=no -D -y --nodoc --pkgname $4 --pkgversion $5
		ldconfig
		cd ..
}

# Args: 1 url_to_download, 2 filename, 3 directory_name, 4 pkg_name, 5 version
download_and_install() {
	if [ "$OPERATION" = "remove" ]; then
		uninstall_dpkg $4
	else
		install_source $1 $2 $3 $4 $5
	fi
}

# Set up the build directory if necessary
if test ! -e build; then 
	mkdir build
fi

cd build
