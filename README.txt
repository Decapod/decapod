Decapod 0.3
============

Main Project Site:  http://sites.google.com/site/decapodproject/
Documentation:      http://wiki.fluidproject.org/display/fluid/Decapod+0.3+User+Guide
Source Code:        http://wiki.fluidproject.org/display/fluid/Decapod+Source+Code


Highlights of the 0.3 Release
==============================

Decapod 0.3 provides the ability to:

1. Capture images from a pair of supported cameras. (See Decapod FAQ for supported cameras)
2. Do some basic remastering of the captured images
3. Export the collected images as an image-based PDF document


What's in this Release?
=======================

Decapod 0.3 is a source code-only release, meaning that you will need to download and install the 
source code manually. In the future we plan to have a simpler method of deploying Decapod. For full
installation and configuration instructions, visit the Decapod User Guide:

http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.3

Decapod itself is composed of several modules:

 * Decapod Client: the user interface for Decapod, available as a rich Web application in Firefox 3.6
 * Decapod Server: executes user actions such as supporting the cameras, image manipulation, and generating PDF files
 * Decapod Scripts: Decapod-specific image processing scripts, including PDF generation, stitching, and more
 * Ocropus: provides support for PDF generation, character recognition, layout detection, and font generation


License
=======

Decapod is licensed under the Apache 2 license. The full license can be found here:

    http://wiki.fluidproject.org/display/fluid/Decapod+Licensing

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


Supported Platforms
===================

Decapod 0.3 is tested and supported in the following configurations:
 
  * Operating System: Ubuntu 9.1
  * Web Browser: Firefox 3.6
  * Cameras: Canon G10, Nikon D50, Nikon D200
  
Other configurations may work, but haven't been tested. The Decapod community welcomes 
contributions, including testing various combinations of operating systems, browsers, and cameras.


Known Issues
============

Bugs and other known issues in Decapod 0.3 are documented in the User Guide:

http://wiki.fluidproject.org/display/fluid/Decapod+0.3+User+Guide#Decapod0.3UserGuide-Limitations

Decapod is actively being developed and improved. We welcome your feedback, bug reports, and
feature requests. Decapod uses JIRA to track bugs and issues: http://issues.fluidproject.org/browse/DECA
