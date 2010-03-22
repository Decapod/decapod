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
// File:
// Purpose:
// Responsible: Michael Patrick Cutter
// Reviewer: Joost van Beusekom
// Primary Repository: iupr1.cs.uni-kl.de/~shared/decapod/tiff2pdf
// Web Sites: www.iupr.org, www.ocropus.org

#include "binnednn.h"




//destructor
binnednn::~binnednn()
{}



//constructor
binnednn::binnednn(float epsN, float repsN, bool enforceCSEG) { //epsN=7 repsN=0.07

    hole_bins=1;
    skel_bins=0;
    enforceCSEGmatching = enforceCSEG;
    reps = repsN;
    eps = epsN;
    count = 0;
    origMerge=0;

}

/*

Helper functions that do not modify data

*/


float binnednn::getReps() const{
    return reps;
}
float binnednn::getEps() const{
    return eps;
}

// function centerOfMass
// called by distance
// find the centroid of an image used to align images
void centerOfMass(narray<float>& result,bytearray& img) {
    int Mx=0,My=0,mass=0;    
    for (int i = 0; i<img.dim(0); i++) {
        for (int j = 0; j<img.dim(1); j++) {

            if (img(i,j) > 0) {
                mass++;
                Mx += j;
                My += i;
            }
         }
    }
    float COMmx = float(Mx)/mass;
    float COMmy = float(My)/mass;
    result.push(COMmx);
    result.push(COMmy);
}


// hashing function
int binnednn::makeKey(int width, int length,int nholes) {
    int key = nholes*100000+width*1000+length;
    //printf("key = %d\n",key);
    return key;
}


// returns index to bin from keylist
int binnednn::findKey(int width, int length,int nholes) {
    int key = makeKey(width,length,nholes);
    for(int i=0;i<keyList.length();++i) if(keyList[i] == key) return i;
    //printf("not found returning dummy value\n");
    return -1;
} 

/*

Data changing helper functions

*/


// Python code describing what's being done
//    skeleton = thin(image)
//    mask = ~(binary_dilation(image,iterations=r) - binary_erosion(image,iterations=r))
//    mask |= skeleton # binary_dilation(skeleton,iterations=1)
// rep = radius of structuring element for morphological operations
void makeMask(bytearray& mask, bytearray& image,int rep=1) {
    //iulib::dinit(512,512);
    //iulib::dshown(image);
    //iulib::dwait();
    iulib::dinit(512,512);

    bytearray part1,part2,mask2;
    copy(mask,image);
    copy(mask2,image);
    copy(part1,image);
    copy(part2,image);
    // FIXME: check what rep does and update accordingly
    binary_dilate_circle(part1,rep);
    binary_erode_circle(part2,rep);
    part1-=part2;
    invert(part1);
    //printf("sum of first part = %f\n",sum(part1));

    iulib::dshown(part1);

    iulib::dwait();
    //binary_dilate_circle(mask,rep);
    //binary_erode_circle(mask2,rep);
    //mask-=mask2;     
    thin(mask);
    //invert(mask);

    iulib::dshown(mask);
    iulib::dwait();
    binary_or(mask,part1);
    
    debugf("dgraphics","final mask\n");
    iulib::dshown(mask);
    iulib::dwait();

}
void nullLessThenThreshold(bytearray& img,float thresh) {
    //#pragma omp parallel for
    for (int i = 0; i<img.dim(0); i++) for (int j = 0; j<img.dim(1); j++) 
            if(img(i,j) < thresh)  img(i,j) = 0;    
}


/*

Core functions of binnednn

*/

// function dist
// called by find
// computes the distance between an image and the candidate token
void dist(narray<double>& return_results,bytearray& shifted,bytearray& image, bytearray& item) {
    CHECK_ARG(samedims(image,item));

    narray<float> result;
    centerOfMass(result,image); 
    float ix = result[0];
    float iy = result[1];
    result.clear();
    centerOfMass(result,item);  
    float x = result[0];
    float y = result[1];
    //printf("center of mass x=%f,y=%f | ix=%f,iy=%f\n",x,y,ix,iy);
    int dx = int(0.5+x-ix);
    int dy = int(0.5+y-iy);
    //printf("delta for this shit is:%d/%d\n",dx,dy);
    
    if (abs(dx)>3 or abs(dy)>3) {
        return_results.push(9999);
        return_results.push(9999);
        return;
    }
    copy(shifted,image);
    byte blah = 0; //change with polarity
    shift_by(shifted,dx,dy,blah);;
    bytearray mask;
    bytearray shifted2mask;
    copy(shifted2mask,shifted);
    //nullLessThenThreshold(shifted2mask,float(255/2));
    makeMask(mask,shifted2mask,1);
    debugf("dgraphics","shifted img looks like this\n");
    dshown(shifted);
    dwait();

    mask /= 255;

//    err = sum(mask*abs(item-image))
    intarray item_temp;
    intarray mask_temp;
    copy(item_temp,item);
    copy(mask_temp,mask);

    sub(item_temp,shifted);
    abs(item_temp);
    mul(item_temp,mask);
    double err = sum(item_temp);
//    total = min(sum(mask*item),susm(mask*image))
    mask_temp.clear();
    intarray mask_temp2;
    copy(mask_temp,mask);
    copy(mask_temp2,mask);
    mul(mask_temp,shifted);
    mul(mask_temp2,shifted);

    //printf("the 1 to 255 check item(5,5) %d vs mask_temp(5,5) %d\n",image(10,10),mask_temp(10,10));
    //printf("CPP says the sumItem=%f, and sumImage=%f sumofImage=%f sumofmask=%f\n",sum(mask_temp2),sum(mask_temp),sum(image),sum(mask));
    double total = min(sum(mask_temp2),sum(mask_temp));

//    rerr = err/max(1.0,total)
    double rerr = err/max(1.0,total);
    //printf("rerr %f, total %f, err %f\n",rerr,total,err); 
//    return err,rerr,image    
    return_results.push(err);
    return_results.push(rerr);    
}

// function symdist
// called by remerge function; checks if two bins fit together
// calls distance
void symdist(narray<double>& return_results,bytearray& image, bytearray& item) {
    narray<double> result;
    bytearray image_t1;
    bytearray item_t2;
    copy(image_t1,image);
    copy(item_t2,item);

    bytearray shifted;
    dist(result,shifted,image_t1,item);
    double err = result[0];
    double rerr = result[1];
    
    result.clear();
    shifted.clear();   

    dist(result,shifted,item_t2,image);
    double err2 = result[0];
    double rerr2 = result[1];

    if(rerr<rerr2) {
        return_results.push(err);
        return_results.push(rerr);
        //image.move(image_t1);
    }else {
        return_results.push(err2);
        return_results.push(rerr2);
        //image.move(item_t2);  
    }
}

// function searchAndReplace
// called by remerge
// side effects: entries in intarray, reconstructBook, are changed. size of reconstructBook is unchanged
// updates tokenIDs after remerging
void searchAndReplace(intarray& reconstructBook, int old_patID, int new_patID, int maxSwapCount){
    int swapCount=0;
    for(int i=0;i<reconstructBook.length();i++) {
        if(reconstructBook[i] == old_patID) {
            reconstructBook[i] = new_patID;
            swapCount++;
        }
        //if(swapCount>=maxSwapCount) break; //total number of occurances of orig pattern 
    } 
}

// function remerge
// called optionally after all images are added to classifier
// side effects, number of tokens are decreased, entries in reconstructBook are altered accordingly
// remerges patterns: compares all patterns ina  bin and sees if they can be joined.
// FIXME: merge both remerge methods; add flag for updating book structure
void binnednn::remerge(intarray& reconstructBook) {
//outer loop is to be parallized due to the independence of each bin during all operations in the remerge functions
    #pragma omp parallel for schedule(dynamic, 1)
    for(int iOut=0;iOut<bins.length();iOut++) { 
        for (int iIn=0;iIn< bins.ref(iOut).length();iIn++) {
            int i=0;
            while (i<bins[iOut].length()) {
                int j = i+1;
                while (j<bins[iOut].length()) {
                    narray<double> result;
                    if(bins[iOut][j].getClassAt(0)!=bins[iOut][i].getClassAt(0)) continue;
                    symdist(result,bins[iOut][j].get_img(),bins[iOut][i].get_img());
                    double err = result[0];
                    double rerr = result[1];
                    if(err<eps or rerr<reps) {
                        printf("merging %d %d err %f rerr %f\n",bins[iOut][j].get_tokenID() ,
                                        bins[iOut][i].get_tokenID(),err,rerr);
                        //searchAndReplace is here to keep the reconstructBook concurrent with remerging
                        searchAndReplace(reconstructBook,bins[iOut][j].get_tokenID() ,
                                        bins[iOut][i].get_tokenID(),bins[iOut][j].getCount());
                        bins[iOut][i].mergeToken(bins[iOut][j]);
                        remove_element(bins[iOut],j);
                        //bins[iOut][j].get_tokenID();
                     }j++;
                }i++;
          }        
        }
    }
}


  
// FIXME: see previous remerge function
void binnednn::remerge() {
#pragma omp parallel for
    for(int iOut=0;iOut<bins.length();iOut++){ 
        for (int iIn=0;iIn< bins.ref(iOut).length();iIn++) {
            int i=0;
            while (i<bins[iOut].length()) {
                //if bins[iOut][i] is None: continue need to replace this with something FIXME
                //IS NULL? if(bins[iOut][i]

                int j = i+1;
                while (j<bins[iOut].length()) {
                    //if bins[iOut][j] is None: continue need to replace this with something FIXME
                    narray<double> result;
                    symdist(result,bins[iOut][j].get_img(),bins[iOut][i].get_img());
                    double err = result[0];
                    double rerr = result[1];
                    if(err<eps or rerr<reps) {
                        //printf "merging",key,i,j,"err",err,"rerr",rerr
                        //# fixme really need tzo use image here
                        bins[iOut][i].mergeToken(bins[iOut][j]);
                        remove_element(bins[iOut],j);
                        //del bins[iOut][j] remove_element(narray<T> &,int )
                    }j += 1;
              }i += 1;
            }        
        }
    }
}

// function find
// find best matching token for a given new image
// called by addKeyed
// calls distance
void binnednn::find(bytearray& best_transformed,floatarray& resultData,bytearray& image,int nholes, int cls) {
    //maybePermute();
    int key = findKey(image.dim(0),image.dim(1),nholes);
    if( key == -1) return; // key does not exist 
    //printf("key exisits\n");
    narray<Token> items = bins[key];
    int best_i = -1;
    double best_err = 1e38;
    double best_rerr = 1e38;
    bytearray shifted;
    for(int i=0; i<items.length();i++) {
         narray<double> result;
         //printf("%d/%d %d/%d holes=%d\n",items[i].sizeX(),image.dim(0),items[i].sizeY(),image.dim(1),nholes);
         shifted.clear();
         if(enforceCSEGmatching) {
             if(items[i].getClassAt(0) != cls) continue;
         }
         dist(result,shifted,image,items[i].get_img());
         double err = result[0];
         double rerr = result[1];
         //do result unpacking here
         if(err>=best_err) continue;
         best_err = err;
         best_rerr = rerr;
         move(best_transformed,shifted); 
         best_i = i;
         //if(best_err > eps) break;
     }
     if(best_i>=0) {
        resultData.push(best_err);
        resultData.push(best_rerr);
        resultData.push(best_i);
     }else //printf("could not find anything in find\n");
     return;
}

// function addKeyed
// checks if a new image is added to some existing token or if a new bin or a new token is created
// called by add
// calls find, makekey
// side effects: adds tokens to bins
int binnednn::addKeyed(bytearray& image,int nholes, int cls=0) {
    bytearray best_transformed;
    floatarray resultData;
    //best_transformed.makelike(image);
    //printf("finding....\n");
    find(best_transformed,resultData,image,nholes,cls);
    //printf("founded\n");
    if (resultData.length() < 1) { //no bin exsists for this size and #hole set
        narray<Token> new_stack;
        new_stack.push(Token(image,cls));
        copy(bins.push(),new_stack);
        keyList.push(makeKey(image.dim(0),image.dim(1),nholes));
        count++;
        return new_stack[0].get_tokenID();
        //printf("finished simple new ... \n");
        }
    else {
        float err = resultData[0];
        float rerr = resultData[1];
        int best_i = resultData[2];
        int key = findKey(image.dim(0),image.dim(1),nholes);
        if( err>eps and rerr>reps) {
            bins(key).push(Token(image,cls));
            count++;
            return bins(key)[bins(key).length()-1].get_tokenID();
        }else {
            bins(key)[best_i].add(best_transformed,cls);
            origMerge++;
            count++;
            return bins(key)[best_i].get_tokenID();
        }
    }
}

// function pad_to
// adds white border around the image (used for shifting)
// side effects: byte array is larger and has a white boarder
void pad_to(bytearray& image,int w,int h) {
    int iw = image.dim(0);
    int ih = image.dim(1);
    int wd = int(w-iw);
    CHECK(wd>=0);
    int w0 = wd/2;
    int w1 = wd-w0;
    int hd = h-ih;
    CHECK(hd>=0);
    int h0 = hd/2;
    int h1 = hd-h0;
    bytearray result;
    result.resize(w,h);
    result.fill(0); //change with polarity
    dinit(512,512);
    dshown(image);
    dwait();
    //result[w0:w0+iw,h0:h0+ih] = image
    for(int i=0;i<iw;i++) for(int j=0;j<ih;j++)
        result(w0+i,h0+j) = image(i,j);
    image.clear();
    image.makelike(result);
    image.move(result);
    //printf("came in as %d/%d came out as %d/%d \n",iw,ih,w0+iw,ih+h0);
}

// idem as above
void pad_bin(bytearray& image,int r=5){
    int w = image.dim(0);
    int h = image.dim(1);
    w = r*int((w+r-1)/r);
    h = r*int((h+r-1)/r);
    //printf("PAD_BIN %d/%d \n",w,h);
    pad_to(image,w,h);
}

//convenience function for pad_to
void pad_by(bytearray& image, int w=3, int h=3) {
    int iw = image.dim(0);
    int ih = image.dim(1);
    pad_to(image,iw+2*w,ih+2*h);
}

// function add
// calls addKeyed
// determines fast characteristics of image to search in appropriate bin
// calls padding functions so that images can be shifted
// wraper for all the detailed "add" stuff
int binnednn::add(bytearray& Bchar, int cls) {
    int nholes=0;
    int r=5;
    pad_bin(Bchar);
    if(hole_bins) {
        //_,nholes = measurements.label(1-binary_closing(pad_by(char,2,2)))
        bytearray Bchar_temp;
        intarray img;
        copy(Bchar_temp,Bchar);
        pad_by(Bchar_temp);
        binary_close_circle(Bchar_temp,1);
        invert(Bchar_temp);
        copy(img,Bchar_temp);
        label_components(img,false);
        narray<rectangle> bboxes;
        bounding_boxes(bboxes,img);
        nholes = bboxes.length()-1;
        //printf("InCpp#h=%d\n",nholes);
        //dinit(512,512);
        //dshown(Bchar_temp);
        //dwait();
    }
    return addKeyed(Bchar,nholes,cls);

}

/*
void calcFreq(&o,&bin
void maybePermuteByFreq(intarray &bin) {
  srand(1);
  float r = rand();
  printf("the value is %f\n",r);
  exit(2);
  intarray p1,o2,ids1;
  quicksort(p1,o2);
  permute(ids1,p1);
}
*/


/*

OUTPUTING FUNCTIONS

*/


// function maybeMakeDirectory
// called by output_tokens
// copy & paste from ocropus
void maybeMakeDirectory(const char* book_dir) {
    strg s;
    sprintf(s,"%s/%s",(const char *)book_dir,"tokens/");
    mkdir(s,0777);
}

// function output_tokens
// outputs the image for each token and names the file by the tokens ID
// changing the format of this will break the interface to tokens2pdf.py
// given book dir outputs tokens and ordered token ID list for each cseg
void binnednn::output_tokens(const char* book_dir)
{
    float thresh = 200;
    maybeMakeDirectory(book_dir);
    #pragma omp parallel for
     for(int iOut=0;iOut<bins.length();iOut++) for (int iIn=0;iIn< bins.ref(iOut).length();iIn++){
        strg path="";
        sprintf_append(path,"%s",book_dir);
        sprintf_append(path,"tokens/%08d.png",bins.ref(iOut).at(iIn).get_tokenID());
        invert(bins.ref(iOut).at(iIn).get_img());
        //bins.ref(iOut).at(iIn).increaseGreaterThanThreshold(bins.ref(iOut).at(iIn).get_img(),thresh);
        //bins.ref(iOut).at(iIn).nullLessThanThreshold(bins.ref(iOut).at(iIn).get_img(),thresh);
        write_image_gray(path.c_str(),bins.ref(iOut).at(iIn).get_img());
    }
}


/*

DEBUGGING FUNCTIONS

*/


// function sortImage
// WARNING do not call on a large book structure, will result in segfault
// prints out each bin, does not use class information
// For controlling purposes: outputs an image with all the Tokens sorted to there respective class
void binnednn::sortImage(const char* sortedImg)
{
    narray<unsigned char> outimg ;
    int tmpx = 0 ;
    int tmpy = 0 ;
    int sizex = 0 ;
    int sizey = 0 ;
    for (int i = 0; i < bins.length(); i++) {
        tmpx = 0 ;
        sizey = sizey + 10 + bins.ref(i).at(0).sizeY() ;
        for (int j = 0; j < bins.ref(i).length(); j++)	{
            tmpx = tmpx + 10 + bins.ref(i).at(j).sizeX() ;
        }
        if (tmpx > sizex)
            sizex = tmpx ;
    }
    outimg.resize(sizex, sizey) ;
    for (int x = 0; x < sizex; x++)
        for (int y = 0; y < sizey; y++)
            outimg(x,y) = 128 ;
	
    for (int i = 0; i < bins.length(); i++) {
        tmpx = 0 ;
		
// 		cout<<" before"<<endl ;
        for (int j = 0; j < bins.ref(i).length(); j++)	{
            for (int x = 0; x < bins.ref(i).at(j).sizeX(); x++)
                for (int y = 0; y < bins.ref(i).at(j).sizeY(); y++)
                    outimg(tmpx+x, tmpy+y) = bins.ref(i).at(j).getVal(x,y) ;
            tmpx = tmpx + 10 + bins.ref(i).at(j).sizeX() ;
        }
        tmpy = tmpy + 10 + bins.ref(i).at(0).sizeY() ;
    }
    FILE* outstream ;
    if ((outstream = fopen(sortedImg, "w")) == NULL) {
        fprintf(stderr, "could not write control image to control.pgm");
    }
    //write_pgm(outstream,outimg);
    write_image_gray(outstream,outimg,"png");
    fclose(outstream) ;
}

// counts the number of tokens in the bins
int binnednn::countTotalClasses() {
    int total=0;
    for(int iOut=0;iOut<bins.length();iOut++) total+=bins.ref(iOut).length();
    return total;
}

// outputs debug info
void binnednn::maxToken() {
    int max = 0;
    int totalPat = 0;
    int classLabel = -1;
    int tokenID = -1;
    for(int iOut=0;iOut<bins.length();iOut++) for (int iIn=0;iIn< bins.ref(iOut).length();iIn++){
        if(bins[iOut][iIn].getClassesLength() > max) {
            max = bins[iOut][iIn].getClassesLength(); 
            classLabel = bins[iOut][iIn].getClassAt(0);
            tokenID = bins[iOut][iIn].get_tokenID();
        }
    }
    printf("the largest single cluster contains %d tokens and its class label is %d tokenID is %d\n", max, classLabel,tokenID);
}


//outputs the number of images that where added to tokens 
int binnednn::numOrigMerge() const{
    return origMerge;
}
//outputs the number of total bins in document
int binnednn::numBins() {
    return keyList.length();
}
//outputs the total count images added
int binnednn::totalCount() const{
    return count;
}
