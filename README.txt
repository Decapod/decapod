Decapod 0.5
============

Main Project Site:  	http://sites.google.com/site/decapodproject/
Documentation:      	http://wiki.fluidproject.org/display/fluid/Decapod+0.5+Release
Source Code:  		http://wiki.fluidproject.org/display/fluid/Decapod+Source+Code

Highlights of the 0.5 Release
==============================

Decapod 0.5 adds the following features and enhancements:

1. New UI focused on image importing, and exporting to PDF formats (Image, OCR, and Traced)
2. Upgraded to a Pre-release version of Fluid Infusion 1.5
3. Improved server side implementation 
	a. Re-implemented based on the proposed REST architecture ( http://wiki.fluidproject.org/pages/viewpage.action?pageId=28344822 )
	b. Added server side file validation
	c. Specific response codes returned
	d. Upgraded to Cherrypy 3.2.2

*Note: Decapod 0.5 removes support for camera capture and organizing images. Some of this will be reintroduced in a future release.

Installing Decapod 0.5
======================

For full installation and configuration instructions, visit the Decapod User Guide:

http://wiki.fluidproject.org/display/fluid/Download+and+Install+Decapod+0.5


Supported Platforms
===================

Decapod 0.5 is tested and supported in the following configurations:
 
  * Operating System: Ubuntu 10.04
  * Web Browser: Firefox 12
  
Other configurations may work, but haven't been tested. The Decapod community welcomes 
contributions, including testing various combinations of operating systems, browsers, and cameras.


What's in this Release?
=======================

Interface:
 * A simple import interface for uploading images to Decapod.
 * Status messages indicating how many files are queued and how many have errors are provided.
 * A choice of three PDF export formats to select.

Importing images:
* Image formats supported in this release are: JPEG, TIFF, and PNG.
* Default maximum image file size is 100MB, but configurable to any custom file size.

Exporting:
 * Export the collection of images as a greyscale image PDF.
 * Export the collection of images as a greyscale image PDF with selectable OCR'd text underlaid.
 * Export the collection of images as a traced PDF.


What Kind of Material Can I Capture with Decapod 0.5?
=====================================================

For this release, it is strongly recommended that Decapod be used with books employing a "Manhattan" style layout.
These layouts should be in rectangular columns, with content fitting and staying within those column boundaries.

OCR and tracing performs best on Latin characters with each character reasonably distinguishable from one another.

Books that are not this style may produce PDFs with unexpected anomalies. Future releases of OCRopus (which Decapod
uses for page analysis and OCR operations) may support more complex layouts.


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
 * OCRopus: http://code.google.com/p/ocropus/ (Apache 2 License)
 * ImageMagick: http://www.imagemagick.org (ImageMagick License)
 * CherryPy: http://www.cherrypy.org (BSD License)


Known Issues
============

Bugs and other known issues in Decapod 0.5 are documented in the Release Page:

http://wiki.fluidproject.org/display/fluid/Decapod+0.5+Release#Decapod0.5Release-KnownIssues

Decapod is actively being developed and improved. We welcome your feedback, bug reports, and
feature requests. Decapod uses JIRA to track bugs and issues: http://issues.fluidproject.org/browse/DECA

