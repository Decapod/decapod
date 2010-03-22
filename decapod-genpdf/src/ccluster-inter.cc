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
    // File: ccluster-inter.cc
    // Purpose: method responsible for intergrating OCRopus components to interface with binnednn.cc
    // Responsible: Michael Patrick Cutter
    // Reviewer: Joost van Beusekom
    // Primary Repository: iupr1.cs.uni-kl.de/~shared/decapod/tiff2pdf
    // Web Sites: www.iupr.org, www.ocropus.org

    #include "binnednn.h"
    #include <unistd.h>

    param_string method("method","BinarizeBySauvola","binarization method");
    param_string cbookstore("bookstore","SmartBookStore","storage abstraction for book");

    
    /* Function: seg2bbox
    outputs bounding box information needed for pdf generation
    inputs: bookstore
    outputs: updated book_dir
    */
    void seg2bbox(const char* book_dir,IBookStore &bookstore, int verbose) {
        try {

        for(int pageno=1;pageno<bookstore.numberOfPages();pageno++) {
            char fn[128];  
            sprintf(fn,"%s%04d.pseg",book_dir,pageno);
            char fnIn[128];
            sprintf(fnIn,"%s.png",fn);

            intarray pageSeg;
            read_image_packed(pageSeg,fnIn);
            //printf("hello3\n");
            rectarray bboxes;
            //write_image_packed("bla.png",pageSeg);
            renumber_labels(pageSeg,0);
            bounding_boxes(bboxes, pageSeg);
            //printf("hello4\n");
            sprintf(fn,"%s.txt",fn);
            FILE *file = fopen(fn,"w");
            for(int i=0;i<bboxes.length();i++){
                if(bboxes(i).x1 >= 0 && bboxes(i).y1 >= 0)
                    fprintf(file,"%d %d %d %d\n",bboxes(i).x0,
                    bboxes(i).y0,
                    bboxes(i).x1,
                    bboxes(i).y1);
            }
            fclose(file);
            if( verbose >= 1)printf("outputing seg2bbox info for page %d\n",pageno);
            #pragma omp parallel for
            for(int lineno=0;lineno<bookstore.linesOnPage(pageno);lineno++) {
            intarray imgSeg;
            int line = bookstore.getLineId(pageno,lineno);  //hex format 
            if (!bookstore.getLine(imgSeg,pageno,line,"cseg")){ //read_image_packed(in,file);
                if( verbose >= 1) debugf("info","%04d %x: no such cseg\n",pageno,line);
                continue;
            }   
            rectarray bboxes;
            renumber_labels(imgSeg,0);
            bounding_boxes(bboxes, imgSeg);
            
            ustrg ustring;
            for(int i=0;i<bboxes.length();i++){
                if(bboxes(i).x1 >= 0 && bboxes(i).y1 >= 0)
                    sprintf_append(ustring,"%d %d %d %d\n",bboxes(i).x0,
                    bboxes(i).y0,
                    bboxes(i).x1,
                    bboxes(i).y1);
            }
            bookstore.putLine(ustring,pageno,line,"cseg");
    }
        }

        }
        catch(const char *oops) {
            fprintf(stderr,"oops: %s\n",oops);
        }
        
    }    

    /* Function: output_patInforInBookFormat 
    outputs the "tokenID" corresponding to cseg file with same ID number
    Inputs: @reconstructBook (intarray), each ID is the tokenID found in order when scanned
            it's very important that the same iterator logic is used to output the tokens as they were read in
            @book_dir (const char*) this is the dir of the OCRopus book
    */
    void output_patInfoInBookFormat(intarray& reconstructBook,IBookStore &bookstore, int verbose){
        int placeID=0;
        for(int pageno=0;pageno<bookstore.numberOfPages();pageno++) 
            for(int lineno=0;lineno<bookstore.linesOnPage(pageno);lineno++) {
                intarray in;
                int line = bookstore.getLineId(pageno,lineno);   
                if (!bookstore.getLine(in,pageno,line,"cseg")){ //read_image_packed(in,file);
                    if(verbose >= 1) debugf("info","%04d %x: no such cseg\n",pageno,line);
                    continue;
                }
                autodel<IGrouper> grouper;
                grouper=make_SimpleGrouper();
                grouper->setCSegmentation(in);
                ustrg ustring;
                if (verbose >= 3) printf("outputing tokenID info %d:%d \n",pageno,lineno);
                for(int i=0;i<grouper->length();i++){
                    sprintf_append(ustring,"%d\n",reconstructBook[placeID]);
                    placeID++;//reconstructBook entries and one to one with for each char in book
                }
                bookstore.putLine(ustring,pageno,line,"tokID");
        }
    }

    /* Function: output_stats
    outputs statistics from binned classifying process
    */
    void output_stats(binnednn &binned, int verbose) {
        if( verbose >= 1)printf("the total num bins=%d, and the total count=%d numOrigMerge=%d\n",binned.numBins(),binned.totalCount(),binned.numOrigMerge());
        if( verbose >= 2)printf("total classes....%d\n",binned.countTotalClasses());
        if( verbose >= 1)binned.maxToken();
    //  if( verbose >= 3)binned.sortImage("resorted.png"); NOT ADVISABLE ON LARGE FILES (all tokens stored in ram)
    }

    /* Function: read_token_from_cseg 
    outputs: @reconstructBook (intarray), each ID is the tokenID found in order when scanned
                @binned filled with tokens
    */
    void read_token_from_cseg(intarray& reconstructBook,IBookStore &bookstore,binnednn &binned,const char* data_dir,int verbose,int epsN=7, float repsN=0.07) {  
        autodel<IBinarize> binarizer;
        make_component(method,binarizer);    
        
        for(int pageno=0;pageno<bookstore.numberOfPages();pageno++){
            if(verbose >= 1) debugf("info","%d page\n",pageno);
            for(int lineno=0;lineno<bookstore.linesOnPage(pageno);lineno++) {
                char transcript[10000];
                intarray in,output;
                nustring csegText;
                bytearray image;
                int line = bookstore.getLineId(pageno,lineno);   
                if (!bookstore.getLine(in,pageno,line,"cseg")){ //read_image_packed(in,file);
                    if( verbose >= 1)debugf("info","%04d %x: no such cseg\n",pageno,line);
                    continue;
                }
                make_line_segmentation_black(in);
                CHECK(in.length()>100);
                image.makelike(in);
                for(int i=0;i<image.length1d();i++) image.at1d(i) = 255*!in.at1d(i);
                
                if( bookstore.getLine(csegText,pageno,line) ) //read in class information plan means just use cseg      
                for(int i=0;i<csegText.length();i++) transcript[i] = csegText[i].ord();
                transcript[csegText.length()] = 0;
                chomp(transcript);
                remove_spaces(transcript);
                if( verbose >= 2 ) {
                    debugf("info","%d/%x the transcript has this many entires:%d \n%s:\n",pageno,line,csegText.length(),transcript);
                }            
                
                autodel<IGrouper> grouper;
                grouper=make_SimpleGrouper();
                grouper->setCSegmentation(in);
                if( verbose >= 3 ) debugf("info","the number components is=%d\n",grouper->length());
                
                for(int i=0;i<grouper->length();i++) {
                    intarray segments;
                    bytearray mask;
                    mask.clear();
                    grouper->getSegments(segments,i);
                    bytearray char_image;
                    char_image.clear();
                    grouper->extractWithMask(char_image,mask,image,i,1);              
                    bytearray output;
                    binarizer->binarize(output,mask);
                    int pattern_id = binned.add(output,transcript[i]);
                    reconstructBook.push(pattern_id);
                    if( verbose >= 3 ) printf("added....%d\n",pattern_id);
                }
            }
        }
    }

    /*  @Function: main
        @invokes ocropus components
        handels operhands
        makes binnednn
    */
    int main(int argc,char *argv[]){
        try{
            int epsN = NULL;
            float repsN = NULL;
            int verbose = NULL;
            int c;
            const char* book_dir = NULL;
            bool remergeOpt = false;
            bool repsFlag = false;
            bool epsFlag = false;
            bool seg2bboxFlag = true;
            bool enforceCSEGmatching = true;
            //parse options
            while ((c = getopt (argc, argv, "he:r:v:b:RSC")) != -1) {
                switch (c) {           
                    case 'C':   
                    enforceCSEGmatching = false;
                    break;                    
                    case 'R':   
                    remergeOpt = true;
                    break;
                    case 'e':   
                    epsFlag = true;
                    epsN = atoi(optarg);
                    break;
                    case 'h':   
                    printf ("Usage:\n-b arg (char* book_dir)\n-v arg (verbose;0=silient,3=loud)\n-r arg (float reps)\n-e arg (int eps)\n");
                    exit(0);                   
                    case 'r':
                    repsFlag = true;
                    repsN = atof(optarg);
                    break;
                    case 'v':
                    verbose = atoi(optarg);
                    break;
                    case 'b':
                    book_dir = optarg;
                    break;   
                    case 'S':
                    seg2bboxFlag = false;
                    break;                     
                    case '?':
                    if (optopt == 'b')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 'r')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 'e')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 'v')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (isprint (optopt))
                        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                    else
                        fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
                    return 1;
                    default:
                    abort ();
                    } 
            }//if no book dir exit
            if( book_dir == NULL) {
                printf("must set book_dir\n");
                exit(2);
            }//setdefault values if not set
            if( epsFlag == false) epsN = 7;
            if( repsFlag == false) repsN = .07;
            if( verbose >= 1)printf("options selected \nverbose= %d eps %d reps %f bookdir %s remergeOpt=%d\n",verbose,epsN,repsN,book_dir,remergeOpt);
            
            init_ocropus_components(); // used for ocropus to work
            init_glfmaps();

            autodel<IBookStore> bookstore;
            make_component(bookstore,cbookstore);
            bookstore->setPrefix(book_dir);
            
            if( verbose >= 1)printf("running on %s\n",book_dir);
            if( verbose >= 1)printf("prefix set number of pages=%d\n",bookstore->numberOfPages());
            
            narray<int> reconstructBook; 
            autodel<binnednn> binned;
            binned = new binnednn(epsN,repsN,enforceCSEGmatching);
            if( verbose >= 1)printf("class binnednn made\n");

            read_token_from_cseg(reconstructBook,*bookstore,*binned,book_dir,verbose,epsN,repsN);

            if(remergeOpt == true) {
                if( verbose >= 1)printf("remerging....\n");
                binned->remerge(reconstructBook);//binned->remerge();//
            }
            if( verbose >= 1)printf("outputing tokens\n");
            binned->output_tokens(book_dir);
            if( verbose >= 1)printf("outputing reconstruct info\n");
            output_patInfoInBookFormat(reconstructBook,*bookstore,verbose);
            output_stats(*binned,verbose);
            if( verbose >= 1)printf("outputing seg2bbox info\n");
            if (seg2bboxFlag )seg2bbox(book_dir,*bookstore,verbose);
            
        }catch(const char *e) {
            fprintf(stderr,"error: %s\n",e);
        } 
    }
