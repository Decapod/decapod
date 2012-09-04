Decapod 0.6
============

Main Project Site:  	http://sites.google.com/site/decapodproject/
Documentation:      	http://wiki.fluidproject.org/display/fluid/Decapod+0.6+Release
Source Code:  		    http://wiki.fluidproject.org/display/fluid/Decapod+Source+Code

Highlights of the 0.6 Release
==============================

Decapod 0.6 adds the following features and enhancements:

1. Support for new PDF format: font-matched PDF.
2. Now export images to TIFF and PNG image formats.
3. Ability to specify custom dimensions and resolution for PDF formats.
4. Decapod now supports Ubuntu 12.04.
5. Improved configurability of the server.

Installing Decapod 0.6
======================

For full installation and configuration instructions, visit the Decapod User Guide:

http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.6


Tested Platform
===============

Decapod 0.6 is tested and supported in the following configurations:
 
  * Operating System: Ubuntu 12.04
  * Web Browser: Firefox 15
  
Other configurations may work, but haven't been tested. The Decapod community welcomes 
contributions, including testing various combinations of operating systems and browsers.


What's in this Release?
=======================

Interface:
 * A simple import interface for uploading images to Decapod.
 * Status messages indicating how many files are queued and how many have errors are provided.
 * Four PDF, and two image export formats to select from.

Importing images:
 * Image formats supported in this release are: JPEG, TIFF, and PNG.
 * Default maximum image file size is 100MB, but configurable to any custom file size.

Exporting:
 * Export the collection of images as a greyscale PDF. Four PDF formats include:
    1. Image PDF
    2. A PDF with electable OCR'd text underlaid.
    3. A PDF with contents traced by computer.
    4. A PDF with detected Latin text converted to a True-Type font.
    
 * Export the collection of images as an archive of PNG or TIFF files.



What Kind of Material Can I Capture with Decapod 0.6?
=====================================================

For this release, it is strongly recommended that Decapod be used with books employing a "Manhattan" style layout.
These layouts should be in rectangular columns, with content fitting and staying within those column boundaries.

OCR and tracing performs best on Latin characters with each character reasonably distinguishable from one another.

Books that are not this style may produce PDFs with unexpected anomalies. Future releases may support more complex layouts.


License
=======

Decapod is licensed under the Apache 2 license. The full license can be found here:

    http://wiki.fluidproject.org/display/fluid/Decapod+License

Decapod also depends upon some third party open source modules, which must be installed separately.


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

See: http://wiki.fluidproject.org/display/fluid/Decapod+Dependencies

**Note: GPL licensed code is not used directly, but only interfaced through calls to the shell**


Known Issues
============

Bugs and other known issues in Decapod 0.6 are documented in the Release Page:

http://wiki.fluidproject.org/display/fluid/Decapod+0.6+Release

Decapod is actively being developed and improved. We welcome your feedback, bug reports, and
feature requests. Decapod uses JIRA to track bugs and issues: http://issues.fluidproject.org/browse/DECA

