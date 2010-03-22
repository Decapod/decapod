// -*- C++ -*-

// Copyright 2010-2011 Image Understanding and Pattern Recognition, University of Kaiserslautern, Germany
// or its licensors, as applicable.
//
// You may not use this file except under the terms of the accompanying license.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may
// obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Project: Decapod
// File: binnednn.h
// Purpose: header file for abstract data type
// Responsible: Michael Patrick Cutter
// Reviewer: Joost van Beusekom
// Primary Repository: iupr1.cs.uni-kl.de/~shared/decapod/tiff2pdf
// Web Sites: www.iupr.org, www.ocropus.org

#ifndef h_binnednn_
#define h_binnednn_

#include "token.h"
#include "ocropus/ocr-utils.h"
#include "ocropus/glinerec.h"
#include "ocropus/glutils.h"
#include "ocropus/glclass.h"
#include "ocropus/bookstore.h"
#include <colib/colib.h>
#include <iulib/iulib.h>
#include <iulib/imgio.h>
#include "iulib/imgbits.h"

using namespace std;
using namespace ocropus;
using namespace colib;
using namespace iulib;
using namespace narray_io;
using namespace narray_ops;
using namespace glinerec;

class binnednn
{
    private:
        objlist<narray<Token> > bins; // 2D structure of clustered patterns; clustered tokens
        narray<int> keyList;            // maps token characteristics to bins
                                        // keylist to get around not using a hash
                                        // w * 1000 + l will become key and keylist will map key to index in byte array
        float eps;      // error tolerance for clustering
        float reps;     // error tolerance for clustering (remerging)
        // FIXME: make a bool out of this?
        bool enforceCSEGmatching;
        int hole_bins;  // used to define what type of features are used to determine bins (hole feature of chars)
        // FIXME: make a bool out of this?
        int skel_bins;  // not implemented yet
        int count;      // counts the number of instances
        int origMerge;  // used for debugging (number of times a token is appended to cluster)
        
        //these functions are called by add, they should be called
        int makeKey(int width, int length,int nholes);
        int findKey(int width,int length, int nholes);
        void find(bytearray& best_transformed,narray<float>& resultData,bytearray& image,int nholes,int cls);
        int addKeyed(bytearray& image,int nholes, int cls);
        
    public: 
        ~binnednn();
        //this function creates the binnednn
        binnednn(float epsN, float repsN, bool enforceCSEGmatching);
        //the function add takes an image places it in a bin, the int returned is the ID of the token the image now belongs to
        int add(bytearray& Bchar, int cls);
        //if you are using binnednn with the hope of token-based-compression and you call remerge,
        //make sure to use the version with reconstruct book
        void remerge();
        void remerge(intarray& reconstructBook);
        //call output_tokens after all classification for a book has been complete. used for token-based-compression
        void output_tokens(const char* book_dir);
        //accessor methods
        float getReps() const;
        float getEps() const;
        //debugging and statistical information
        //total number of bins
        int numBins();
        //number of times an image was added to a preexisting token
        int numOrigMerge() const;
        //total number of classes (tokenIDs) 
        int countTotalClasses();
        //outputs the token with the most members and 
        void maxToken();
        int totalCount() const;
        void sortImage(const char* sortedImg); // use for debugging only; use carefully :-) 







};

#endif