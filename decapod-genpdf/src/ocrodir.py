#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright 2006-2008 Deutsches Forschungszentrum fuer Kuenstliche Intelligenz 
# or its licensors, as applicable.
# 
# You may not use this file except under the terms of the accompanying license.
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you
# may not use this file except in compliance with the License. You may
# obtain a copy of the License at http:#www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
# Project: Decapod
# File: ocrodir
# Purpose: "Parse" OCRopus output directory
# Responsible: Joost van Beusekom (joost@iupr.org)
# Reviewer: 
# Primary Repository: 
# Web Sites: www.iupr.org, www.dfki.de

import os, glob, string, sys, time, getopt, commands, math, numpy, datetime, platform, tempfile
import codecs
from numpy import *

# needed to read unicode text files
def readFile(fn):
    try:
        f = codecs.open(fn, encoding="utf-8")
        tmp=[]
        for line in f:
            #print line
            for i in range(len(line)):
                tmp.append(line[i])
        f.close()
        return tmp;
    except UnicodeDecodeError:
        print "Error reading file ", fn
        return []
    except IOError:
	print "Warning: file could not be opened: ",fn
	return []

#reads locations of where each image belongs on page (bounding box)
def readBoxFile(fn):
    if os.path.exists(fn)== False:
	print "Warning: file could not be opened: ",fn
	return []
    f    = file(fn, "r")
    data = [line.split() for line in f]
    f.close()
    coords = zeros([len(data),4],int)
    for i in range(len(data)):
        coords[i,0] = int(data[i][0]) # x0
        coords[i,1] = int(data[i][1]) # y0
        coords[i,2] = int(data[i][2]) # x1
        coords[i,3] = int(data[i][3]) # y1
    return coords

#reads the output from binned clustering
def readTokenIDFile(fn):
    if os.path.exists(fn) == False:
	print "Warning: file could not be opened: ",fn
	return []
    f    = file(fn, "r")
    data = [line.split() for line in f]
    f.close()
    tID = zeros([len(data)],int)
    for i in range(len(data)):
        tID[i] = int(data[i][0]) # x0
    return tID


# ============ Object that represents the book structure ==============
class Book:
    
    # default constructor
    def __init__(self):
        self.bookDir  = "" # path of the book directory
        self.pageSize = [21.0,29.7] # paper size in cm FIXME joost, paramatize
        self.tokenDir = "" # token directory
        self.tokens   = {} # token [ID, file name]

    # ocrodir path is given to constructor; directory tree is parsed automatically
    def __init__(self, path):
        self.bookDir  = path
        self.pageSize = [21.0,29.7] # paper size in cm
        self.tokenDir = path+"/tokens/" 
        self.tokens   = {} # token[ID] = file name
        self.update()
    
    # parse directory tree
    def update(self):
        # get line image list
        self.pages = []
        fileList = os.listdir(self.bookDir) 
        # filelist is unsorted, that's why pages need to be sorted afterwards
        for f in fileList:
            if(len(f)==8 and f[len(f)-4:len(f)] == ".png"): # FIXME joost,
		if(os.path.exists(self.bookDir+"/"+f.split(".")[0])): #whitespace page check
                	p = Page(self.bookDir+"/"+f)
                	self.pages.append(p)
        self.sortPages()
        self.readTokens()
    
    # sort pages
    def sortPages(self):
        oldMinPage = -1
        for i in range(len(self.pages)):
            minLine = 10000
            minIndex = -1
            for j in range(len(self.pages)):
                if (self.pages[j].number < minLine and self.pages[j].number > oldMinPage):
                    minLine = self.pages[j].number
                    minIndex = j
            if minIndex == -1:
                print("Book::sortPages: Error sorting the page!")
                break
            p = self.pages[minIndex]
            self.pages[minIndex]=self.pages[i]
            self.pages[i]=p
            oldMinPage = minLine
    
    # read token dir in book structure to book object
    def readTokens(self):
        fileList = os.listdir(self.tokenDir)
        for f in fileList:
            if(f.split(".")[0]):
                #self.tokens.append([int(f[0:len(f)-4]), self.tokenDir+f])
                self.tokens[int(f[0:len(f)-4])] = self.tokenDir+f
    
    # print the book information to stdout
    def output(self):
        print("bookDir         = %s" %(self.bookDir))
        print("Number of pages = %d" %(len(self.pages)))
        for i in range(len(self.pages)):
            print("Page %d" %(i))
            self.pages[i].output()
    def getTokens(self):
        return tokens
        
        
    def checkTokenable(self):
        for page in self.pages:
            if (page.checkTokenable() == False):
                return False;
        return True;

# child of book object, each book has n pages. Corresponds to OCRopus book structure
class Page:
    def __init__(self):
        self.number   = -1 # page number
        self.image    = "" # original image file name
        self.binImage = "" # binarized image file name
        self.pageDir  = "" # path containing the page information
        self.lineBoxs = "" # file containing the page to line segm. information
        self.linesPos = [] # position of lines
        self.lines    = [] # list of lines
     #   self.tseg    = [] # list of tokID
    # constructor with file name, e.g. bookDIR/0001.png
    def __init__(self, imgFN):
        # filename given
        if ((imgFN[len(imgFN)-4:len(imgFN)]).lower()==".png"): # FIXME joost,
            self.image    = imgFN # original image file name
            self.number   = int(imgFN[len(imgFN)-8:len(imgFN)-4]) # FIXME joost,
            self.binImage = imgFN[0:len(imgFN)-4]+".bin.png" # FIXME joost,
            self.lineBoxs = imgFN[0:len(imgFN)-4]+".pseg.txt" # FIXME joost,
            self.pageDir  = imgFN[0:len(imgFN)-4]+"/" # FIXME joost,
          #  self.tseg    = [] # list of tokID
            self.update()
        # output error message
        else:
            print("Page::fromOcroDir: Error: expected PNG file as input!")
            return
    
    # print page information to std out
    def output(self):
        print("  image      = %s" %(self.image))
        print("  binImage   = %s" %(self.binImage))
        print("  pageNumber = %d" %(self.number))
        print("  num. lines = %d" %(len(self.lines)))
        print("  pageDir    = %s" %(self.pageDir))
        for i in range(len(self.lines)):
            print("    Line %d" %(i))
            self.lines[i].output()

        
        
    # read the page information from the directory structure
    def update(self):
        # get line image list
	if(not os.path.exists(self.pageDir)): return #ocropus does not genereate dir for whitespace page
        self.lines = []
        self.linesPos = readBoxFile(self.lineBoxs)
        fileList = os.listdir(self.pageDir)
        for f in fileList:
            if(len(f) == 10 and f[len(f)-4:len(f)] == ".png"): # FIXME joost,
                l = Line(self.pageDir + f)
                self.lines.append(l)
        self.sortLines()

#        for f in fileList:
#            if(f.find("patID")!=-1):
#                self.tseg.append(f)
#        self.tseg.sort()

    # sort lines
    def sortLines(self):
        oldMinLine = -1
        for i in range(len(self.lines)):
            minLine = 1000000000000
            minIndex = -1
            for j in range(len(self.lines)):
                if (self.lines[j].number < minLine and self.lines[j].number > oldMinLine):
                    minLine = self.lines[j].number
                    minIndex = j
            if minIndex == -1:
                print("Book::sortPages: Error sorting the lines!") # FIXME joost,
                break
            p = self.lines[minIndex]
            self.lines[minIndex]=self.lines[i]
            self.lines[i]=p
            oldMinLine = minLine
    
    def checkTokenable(self):
        for line in self.lines:
            if (line.checkTokenable() == False):
                return False;
        return True;

#child of page, every page contains x lines. Corresponds to OCRopus structure
class Line:
    def __init__(self):
        self.number   = -1 # page number
        self.image    = "" # original image file name
        self.ccs      = [] # connected components
        self.txt      = [] # text in text line
        self.tokenIDs = [] # token IDs in line
        self.tokenFile= "" # tokenID File
        self.bboxFile = "" # list of connected component files
        self.textFile = "" # list containing the line text
    
    # constructor with file name, e.g. bookDIR/0001/0001.png
    def __init__(self, imgFN):
        # filename given
        if ((imgFN[len(imgFN)-4:len(imgFN)]).lower()==".png"):
            self.image    = imgFN # original image file name
            #self.number   = toHex(imgFN[len(imgFN)-10:len(imgFN)-4])
            self.number   = int((imgFN[len(imgFN)-10:len(imgFN)-4]),16) # FIXME joost,
            self.textFile = imgFN[0:len(imgFN)-4]+".txt"
            self.tokenFile= imgFN[0:len(imgFN)-4]+".tokID.txt" # text in text line # FIXME joost, might change to tokID
            self.bboxFile = imgFN[0:len(imgFN)-4]+".cseg.txt"
            self.update()
        # output error message
        else:
            print("Page::fromOcroDir: Error: expected PNG file as input!")
            return
    
    # print page information to std out
    def output(self):
        print("    image      = %s" %(self.image))
        print("    number     = %x" %(self.number))
        print("    text       = %s" %(self.txt))
        print("    num. bboxs = %d" %(len(self.ccs)))
	print("    num. tokIDs= %d" %(len(self.tokenIDs)))
        print("    bboxFile   = %s" %(self.bboxFile))


        
        
    # read the page information from the directory structure
    def update(self):
        # read text from file
        self.txt = readFile(self.textFile)
        self.ccs = readBoxFile(self.bboxFile)
        self.tokenIDs = readTokenIDFile(self.tokenFile)

    # checks if the line information is complete
    def checkTokenable(self):
        return (os.path.exists(self.image) and 
                os.path.exists(self.textFile) and
                os.path.exists(self.tokenFile) and
                os.path.exists(self.bboxFile))
		

