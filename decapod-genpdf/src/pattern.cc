#include "pattern.h"

int Pattern::staticID=0; //initilize counter

Pattern::Pattern() {
    count = 0;
}


Pattern::Pattern(bytearray& con_img, int cls) {
    
    patternID = next_user_id();
    img.resize(con_img.dim(0), con_img.dim(1)) ;
    for (int i = 0; i<con_img.dim(0); i++) for (int j = 0; j<con_img.dim(1); j++)
            img(i,j) = con_img(i,j);
    count = 1;
    classes.push(cls);
}

//copy constructor
Pattern::Pattern(const Pattern& pat) {
    patternID = pat.get_patternID();
    for(int i=0;i<pat.getClassesLength();i++)
        classes.push(pat.getClassAt(i));
    count = pat.getCount();
    img.resize(pat.sizeX(), pat.sizeY()) ;
    for (int i = 0; i < pat.sizeX(); i++)
        for (int j = 0; j < pat.sizeY(); j++)
            img(i,j) = pat.getVal(i, j) ;

}
//assingment
Pattern& Pattern::operator=(const Pattern& pat) {
    patternID = pat.get_patternID();
    count = pat.getCount();
    for(int i=0;i<pat.getClassesLength();i++)
        classes.push(pat.getClassAt(i));
    img.resize(pat.sizeX(), pat.sizeY()) ;
    for (int i = 0; i < pat.sizeX(); i++)
        for (int j = 0; j < pat.sizeY(); j++)
            img(i,j) = pat.getVal(i, j) ;
}

int Pattern::next_user_id()
    {
    return staticID++;
    }

int Pattern::get_patternID() const{
    return patternID;
}

int Pattern::sizeX() const { return img.dim(0);  }
int Pattern::sizeY() const { return img.dim(1);  }

int Pattern::getVal(int i, int j) const {
    CHECK(i < sizeX() and j < sizeY());
    return img(i,j);
}

int Pattern::getCount() const {
    return count;
}

int Pattern::getMostFrequent() {
    for(int i=0;i < getClassesLength(); i++) { throw Unimplemented(); }
}

bytearray& Pattern::get_img() {
    return img;
}

int Pattern::getClassesLength() const {
    int length = classes.dim(0);
    return length;
}

int Pattern::getClassAt(int index) const{
    int length = classes.dim(0);
    if(length > index and index >= 0){
        int at =  classes(index);
        return at;
    }
    return -1;
}



void Pattern::add(bytearray& con_img,int cls) {
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

//old pattern "pat" will be deleted, extract weighted information and increase self.count by pat.count
void Pattern::merge_Pattern(Pattern& pat) {
    floatarray imgCurr;
    floatarray imgPat;
    copy(imgCurr,img);
    copy(imgPat,pat.get_img());
//self.a = (float(self.count)*self.a + float(pat.count)*pat.a)/(float(self.count)+float(pat.count))
    mul(imgCurr,count);
    mul(imgPat,pat.getCount());
    imgCurr+=imgPat;
    imgCurr/= int(count+pat.getCount());
    count += pat.getCount();
    img.clear();
    img.copy(imgCurr);

}

void Pattern::dshown() {
    iulib::dinit(512,512);
    iulib::dsection("pattern");
    
    iulib::dshown(img);
    iulib::dwait();    
}

void Pattern::increaseGreaterThanThreshold(bytearray& image,float thresh) {
    for (int i = 0; i<image.dim(0); i++) for (int j = 0; j<image.dim(1); j++) 
            if(image(i,j) > thresh)  image(i,j) = 255;    
}

void Pattern::nullLessThanThreshold(bytearray& img,float thresh) {
    for (int i = 0; i<img.dim(0); i++) for (int j = 0; j<img.dim(1); j++) 
            if(img(i,j) <= thresh)  img(i,j) = 0;    
}


void Pattern::mergeImage(bytearray& addedImg)
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

