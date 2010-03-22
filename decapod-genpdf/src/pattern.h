#ifndef h_pattern_
#define h_pattern_

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
using namespace colib;
using namespace iulib;
using namespace narray_io;
using namespace narray_ops;
using namespace ocropus;


class Pattern
{
    private:
        static int staticID;
        int patternID;
        int count;
        bytearray img;
        narray<int> classes;

    public:
        Pattern();
        Pattern(bytearray& con_img, int cls);
        Pattern(const Pattern& pat);
        Pattern& operator=(const Pattern& pat);
        static int next_user_id(); //static ID for Pattern
        int get_patternID() const;
        int sizeX() const; 
        int sizeY() const; 
        int getVal(int i, int j) const;
        int getCount() const;
        bytearray& get_img();
        int getClassAt(int i) const;
        int getClassesLength() const;
        int getMostFrequent();
        void add(bytearray& con_img, int cls);
        void merge_Pattern(Pattern&);
        void dshown();
        void mergeImage(bytearray& addedImg);
        void increaseGreaterThanThreshold(bytearray& image,float thresh);
        void nullLessThanThreshold(bytearray& img,float thresh);
};

#endif