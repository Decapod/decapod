Decapod 0.4
============

Main Project Site:  	http://sites.google.com/site/decapodproject/
Documentation:      	http://wiki.fluidproject.org/display/fluid/Decapod+0.4+Release
                        http://wiki.fluidproject.org/display/fluid/Decapod+0.4+User+Guide
                        http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.4
Source Code:        	http://wiki.fluidproject.org/display/fluid/Decapod+Source+Code


Highlights of the 0.4 Release
==============================

Decapod 0.4 adds the following features and enhancements:

1. A new camera calibration wizard, enabling automatic adjustment of image rotation and page order
2. Better feedback for long-running tasks such as export and image capture
3. Improved installation process--Decapod now includes scripts to make it easier to install and setup.  Look for more install improvements in future releases.
4. Simplified the Decapod server code, providing architectural and performance improvements


Installing Decapod 0.4
======================

For full installation and configuration instructions, visit the Decapod User Guide:

http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.4


Supported Platforms
===================

Decapod 0.4 is tested and supported in the following configurations:
 
  * Operating System: Ubuntu 10.0.4
  * Web Browser: Firefox 3.6
  * Cameras: Canon G10
  
Other configurations may work, but haven't been tested. The Decapod community welcomes 
contributions, including testing various combinations of operating systems, browsers, and cameras.


What's in this Release?
=======================

Camera Setup:
 * Cameras detected, and any issues or unsupported
 * Fix image rotation and page order

Interface:
 * A vertical thumbnail navigator listing images captured by the cameras.
 * A larger preview area displaying the currently selected thumbnail in higher resolution.
 * Keyboard navigation of image thumbnails.
 * Reordering of image order by keyboard or mouse.
 * Delete an image by using keyboard or mouse (appropriate warning dialog displayed).
 * Display a status message confirming a successful deletion of image.

Capture:
 * Capture images using two cameras that support remote capture.
 * Activate capture process using mouse.
 * Detect supported and unsupported cameras, and give an appropriate diagnosis to the user (Partially implemented).
 * Display a status message confirming a successful capture.

Post-Capture Processing:
 * Automatically rotate and adjust the page order as specified during Camera Setup.
 * Join a pair of captured images to create a page spread equivalent.
 * Generate a thumbnail of the page spread.

Exporting:
 * Export the collection of images as a greyscale image PDF.


What Kind of Material Can I Capture with Decapod 0.4?
=====================================================

<Jonathan, can you fill this in?>


License
=======

Decapod is licensed under the Apache 2 license. The full license can be found here:

    http://wiki.fluidproject.org/display/fluid/Decapod+License

Decapod also depends upon some third party open source modules, which must be installed separately.


Third Party Software Used By Decapod
====================================

Decapod is a Web application front-end for a collection image processing and capture tools, 
aggregated together using command line utilities. Third-party dependencies include:

 * libtifftools: http://www.libtiff.org (MIT License)
 * gphoto2: http://www.gphoto.org/ (GNU Public License)
 * libgphoto2: http://www.gphoto.org/ (Lesser GNU Public License)
 * OCRopus: http://code.google.com/p/ocropus/ (Apache 2 License)
 * ImageMagick: http://www.imagemagick.org (ImageMagick License)
 * CherryPy: http://www.cherrypy.org (BSD License)


Known Issues
============

Bugs and other known issues in Decapod 0.4 are documented in the User Guide:

http://wiki.fluidproject.org/display/fluid/Decapod+0.4+User+Guide#Decapod0.4UserGuide-LimitationsandKnownIssues

Decapod is actively being developed and improved. We welcome your feedback, bug reports, and
feature requests. Decapod uses JIRA to track bugs and issues: http://issues.fluidproject.org/browse/DECA
