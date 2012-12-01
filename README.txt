Decapod 0.7
============

Main Project Site: http://sites.google.com/site/decapodproject/
Documentation: http://wiki.fluidproject.org/display/fluid/Decapod+0.7+Release
Source Code: http://wiki.fluidproject.org/display/fluid/Decapod+Source+Code

Highlights of the 0.7 Release
==============================

Decapod 0.7 adds the following features and enhancements:

* Ability to capture using cameras connected via USB
* Create stereo 3D calibration data and dewarp stereo images based on that calibration data
* Ubuntu 12.04 64-bit (32-bit no longer supported)
* Fully internationalizable UI strings
* In-application Help documentation

Installing Decapod 0.7
======================

For full installation and configuration instructions, visit:
http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.7


Tested Platform
===============

Decapod 0.7 is tested and supported in the following configurations:
 
  * Operating System: Ubuntu 12.04 64-bit
  * Web Browser: Firefox 17
  
Other configurations may work, but haven't been tested. The Decapod community welcomes 
contributions, including testing various combinations of operating systems and browsers.


What's in this Release?
=======================

In General:

 * Image formats supported in this release are: JPEG, TIFF, and PNG.

Calibrate UI & Server:

The calibrate interface allows users to upload a zip file with all the calibration images 
into decapod calibrate server. The server generates the calibration data that can be exported
as a zip file.

* Uploading images:
 ** The zip file for calibration images must contain no less than 23 stereo images.
 ** The maximum image file size is unlimited, but configurable to any custom file size.

Capture UI & Server:

The capture interface allows users to control cameras to take pictures for calibration,
dewarping or other purposes. The pictures can be exported as a zip file.

* Capturing images:
 ** The list of supported cameras: http://gphoto.org/proj/libgphoto2/support.php
 ** No more than 2 cameras can be connected to the computer.
 ** If any connected camera is disconnected, the capture server needs to be restarted to
    work with cameras that are still connected or re-connected.

Dewarp UI & Server:

* Steps to use:
 ** Upload the zip file that contains all the images to be dewarped.
 ** Upload the calibration zip file exported from the "Calibrate UI & Server".
 ** Launch the color picker to pick the colors of the page separator string and background.
 ** Start dewarping.
 ** Export the dewarped images as a zip file.

Export UI & Server:

* Interface:
 ** A simple import interface for uploading images to Decapod.
 ** Status messages indicating how many files are queued and how many have errors are provided.
 ** Four PDF, and two image export formats to select from.

* Importing images:
 ** Image formats supported in this release are: JPEG, TIFF, and PNG.
 ** Default maximum image file size is 100MB, but configurable to any custom file size.

* Exporting:
 ** Export the collection of images as a greyscale PDF. Four PDF formats include:
    1. Image PDF.
    2. A PDF with selectable OCR'd text underlaid.
    3. A PDF with contents traced by computer. 
    4. A PDF with matched TrueType font. 
               
                                                      
Third Party Software Used By Decapod
====================================

Decapod is a Web application front-end for a collection image processing and capture tools, 
aggregated together using command line utilities. Third-party dependencies include:                               

 * checkinstall: http://asic-linux.com.mx/~izto/checkinstall/ (GPL License)
 * cherrypy 3.2.2: http://www.cherrypy.org/ (BSD License)
 * Fluid Infusion 1.5-SNAPSHOT: http://fluidproject.org/ (BSD License)
 * fontforge: http://fontforge.sourceforge.net/ (BSD License)
 * fontforge-extras: http://fontforge.sourceforge.net/ (BSD License)
 ** autotrace: http://autotrace.sourceforge.net/ (GPL License)
 ** potrace: http://potrace.sourceforge.net/ (GPL License)
 * imagemagick: http://www.imagemagick.org/script/index.php (ImageMagick License)
 * jQuery UI Accordion 1.8.19: http://jqueryui.com/ (MIT License)
 * mercurial: http://mercurial.selenic.com/ (GPL License)
 * OCRopus: http://code.google.com/p/ocropus/ (Apache 2.0 License)
 ** libjpeg62-dev: http://packages.ubuntu.com/lucid/libjpeg62-dev (libjpeg62 License)
 ** libtiff-tools: http://www.libtiff.org/ (libtiff License)
 * python-fontforge: http://fontforge.sourceforge.net/ (BSDa License)
 * python-imaging: http://www.pythonware.com/products/pil/ (MIT License)
 * python-matplotlib: http://matplotlib.sourceforge.net/ (PSF License)
 * python-nose: http://readthedocs.org/docs/nose/en/latest/ (LGPL License)
 * python-numpy: http://numpy.scipy.org/ (BSD License)
 * python-reportlab: http://www.reportlab.com/ (BSD License)
 * python-scipy: http://www.scipy.org/SciPy (BSD License)
 * python-scipy-dbg: http://www.scipy.org/SciPy (BSD License)
 * python-simplejson: http://simplejson.readthedocs.org/en/latest/ (MIT License)
 * Qunit: http://qunitjs.com/ (MIT License)
 * scons: http://www.scons.org/ (MIT License)
 * gphoto2: http://gphoto.org/ (GPL License)
 * cmake: http://www.cmake.org/cmake/project/project.html (CMake License)
 * git: http://git-scm.com/ (GNU General Public License)
 * python-wxgtk2.8: http://packages.ubuntu.com/search?keywords=python-wxgtk2.8 (Ubuntu License)
 * OpenCV: http://opencv.org/ (BSD license)
 * pyflann: https://github.com/mariusmuja/flann (BSD license)
 * vlfeat: http://www.vlfeat.org/ (BSD license)

See: http://wiki.fluidproject.org/display/fluid/Decapod+Dependencies

**Note: GPL licensed code is not used directly, but only interfaced through calls to the shell**


Known Issues
============

Bugs and other known issues in Decapod 0.7 are documented in the Release Page:

http://wiki.fluidproject.org/display/fluid/Decapod+0.7+Release

Decapod is actively being developed and improved. We welcome your feedback, bug reports, and
feature requests. Decapod uses JIRA to track bugs and issues: http://issues.fluidproject.org/browse/DECA
