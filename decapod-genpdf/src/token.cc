// -*- C++ -*-

// Copyright 2010-2011 Image Understanding and Token Recognition, University of Kaiserslautern, Germany
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
// File: token.cc
// Purpose: Token is the class which holds the bytearray representing the image and information about it
// Responsible: Michael Patrick Cutter
// Reviewer: Joost van Beusekom
// Primary Repository: iupr1.cs.uni-kl.de/~shared/decapod/tiff2pdf
// Web Sites: www.iupr.org, www.ocropus.org

#include "token.h"

int Token::staticID=0; //initilize counter

Token::Token() {
    count = 0;
}


Token::Token(bytearray &con_img, int cls) {
    
    tokenID = next_user_id();
    img.resize(con_img.dim(0), con_img.dim(1)) ;
    for (int i = 0; i<con_img.dim(0); i++) for (int j = 0; j<con_img.dim(1); j++)
            img(i,j) = con_img(i,j);
    count = 1;
    classes.push(cls);
}

//copy constructor
Token::Token(const Token &tok) {
    tokenID = tok.get_tokenID();
    for(int i=0;i<tok.getClassesLength();i++)
        classes.push(tok.getClassAt(i));
    count = tok.getCount();
    img.resize(tok.sizeX(), tok.sizeY()) ;
    for (int i = 0; i < tok.sizeX(); i++)
        for (int j = 0; j < tok.sizeY(); j++)
            img(i,j) = tok.getVal(i, j) ;

}
//assingment
Token& Token::operator=(const Token &tok) {
    tokenID = tok.get_tokenID();
    count = tok.getCount();
    for(int i=0;i<tok.getClassesLength();i++)
        classes.push(tok.getClassAt(i));
    img.resize(tok.sizeX(), tok.sizeY()) ;
    for (int i = 0; i < tok.sizeX(); i++)
        for (int j = 0; j < tok.sizeY(); j++)
            img(i,j) = tok.getVal(i, j) ;
}

int Token::next_user_id()
    {
    return staticID++;
    }

int Token::get_tokenID() const{
    return tokenID;
}

int Token::sizeX() const { return img.dim(0);  }
int Token::sizeY() const { return img.dim(1);  }

int Token::getVal(int i, int j) const {
    CHECK(i < sizeX() and j < sizeY());
    return img(i,j);
}

int Token::getCount() const {
    return count;
}

int Token::getMostFrequent() {
    for(int i=0;i < getClassesLength(); i++) { throw Unimplemented(); }
}

bytearray& Token::get_img() {
    return img;
}

int Token::getClassesLength() const {
    int length = classes.dim(0);
    return length;
}

int Token::getClassAt(int index) const{
    int length = classes.dim(0);
    if(length > index and index >= 0){
        int at =  classes(index);
        return at;
    }
    return -1;
}



void Token::add(bytearray &con_img,int cls) {
     //printf("HALLO IN ADD %d\n",count);
     //assert self.count==sum(self.classes.values())
     classes.push(cls);
     if(count < 1) {
         img.resize(con_img.dim(0), con_img.dim(1)) ;
         for (int i = 0; i<con_img.dim(0); i++) for (int j = 0; j<con_img.dim(1); j++)
             img(i,j) = con_img(i,j);
         count = 1;
     }else {
         //printf("merging.call....\n");
         mergeImage(con_img);
         count++;
     }
}

//old token "tok" will be deleted, extract weighted information and increase self.count by tok.count
void Token::mergeToken(Token &tok) {
    floatarray imgCurr;
    floatarray imgPat;
    copy(imgCurr,img);
    copy(imgPat,tok.get_img());
//self.a = (float(self.count)*self.a + float(tok.count)*tok.a)/(float(self.count)+float(tok.count))
    mul(imgCurr,count);
    mul(imgPat,tok.getCount());
    imgCurr+=imgPat;
    imgCurr/= int(count+tok.getCount());
    count += tok.getCount();
    img.clear();
    img.copy(imgCurr);

}

void Token::dshown() {
    iulib::dinit(512,512);
    iulib::dsection("token");
    
    iulib::dshown(img);
    iulib::dwait();    
}

void Token::increaseGreaterThanThreshold(bytearray &image,float thresh) {
    for (int i = 0; i<image.dim(0); i++) for (int j = 0; j<image.dim(1); j++) 
            if(image(i,j) > thresh)  image(i,j) = 255;    
}

void Token::nullLessThanThreshold(bytearray &img,float thresh) {
    for (int i = 0; i<img.dim(0); i++) for (int j = 0; j<img.dim(1); j++) 
            if(img(i,j) <= thresh)  img(i,j) = 0;    
}


void Token::mergeImage(bytearray &addedImg)
{
    float thresh = 200;
    debugf("dgraphics","showing images that are being merged\n");
    dshown();
    iulib::dinit(512,512);
    iulib::dshown(addedImg);
    iulib::dwait();       
    CHECK(img.dim(0) == addedImg.dim(0));
    CHECK(img.dim(1) == addedImg.dim(1));
    //printf("merging.in....%d/%d %d/%d\n",img.dim(0),addedImg.dim(0),img.dim(1),addedImg.dim(1));	
    for (int x = 0; x < img.dim(0); x++) for (int y = 0; y < img.dim(1); y++) {
            img(x,y) = (((count*img(x,y))+(addedImg(x,y))) * (1.0/(count+1))) ;
         }
    dshown();
    nullLessThanThreshold(img,(thresh));
    increaseGreaterThanThreshold(img,(thresh));
    debugf("dgraphics","here comes the image\n");
    dshown();
    debugf("dgraphics","image was just shown\n");
}

