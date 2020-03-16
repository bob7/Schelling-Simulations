//  main.cpp
//  Schell
//  Created by gb on 10/21/12.
//  Copyright (c) 2012 Barmpalias. All rights reserved.

//LIBRARIES
//#include <GLUT/glut.h>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <ctime>

using namespace std;
unsigned char* image;
unsigned char* p;
char picdir [ FILENAME_MAX ];
int biasvarB, biasvarR, iround, unsucswap, quitvar, indexa, dispz;
int nure, nubl, ix, iy, wrongred, wrongblue, seedvar, utilr, utilg, exhausttest, swaphappened, pausevar;
//time variables
std::chrono::time_point<std::chrono::system_clock> start, fend, finnumbhum, finbiasArray, looptimeno0, looptime1;
std::chrono::time_point<std::chrono::system_clock> swaptime0, swaptime1, swaptime2;
long long looptimedif, swaptimedif1, swaptimedif2, swaptimedif3, swaptimedif4;

//CONSTANTS AND VARIABLES
int ijnegvecsize, swapx, swapy;
int numbhum, segvar, i, stage, ig, j, k, w, x, y, testmode, roundvar, roundrep, iseg, swapmax, standpar, roundret;
float tau, tau0, tau1;
int haltvar, exhaustswap, extenvar, negsize, saveimg, convar;
int  biasvar, biasvarX, biasvarY, rednu, bluenu;
int targvarnAr, targvarnAb, varBiasAr, varBiasAb, artargvarnAr0, artargvarnAr1, artargvarnAb0, artargvarnAb1;
int biasRtargOld, biasBtargOld;//transitional memory bias variables for updating unhappy lists
int swapx0, swapx1, swapy0, swapy1, biasvarR0, biasvarR1, biasvarB0, biasvarB1;
int neipar =0, ineg, jneg, biasRtargNew, biasBtargNew, contquest;
float zoomvar = 1.0, translvarx = -0.9f, translvary = -0.9f;
bool fullScreen = false;
std::vector<tuple <int, tuple <int, int>> > changedhum;
#define GL_PI  3.1415926535f // Define a constant for the value of PI
#define GL_PI2 6.28318530718f // Define a constant for the value of PI
vector<vector<int> > pixel_array;
int number_stages;
int pixdim=100; /* pixel dimensions of square*/
int r1=140, r2=110,r3=50,g1=0,g2=150,g3=240, b1=0, b2=0, b3=0;

char spstring [ FILENAME_MAX ];
char spstring0 [ FILENAME_MAX ], spstring1 [ FILENAME_MAX ], spstring2 [ FILENAME_MAX ],
spstring3 [ FILENAME_MAX ], spstring4 [ FILENAME_MAX ],
spstring5 [FILENAME_MAX], tstri0 [ FILENAME_MAX ],
tstri1 [ FILENAME_MAX ], tstri2 [ FILENAME_MAX ];
float ANGLE_INCR, RADIUS_INCR;

std::vector<vector<int> > numbhumArray;
bool chaboo = true;
std::vector<int> biasArray;
std::vector<int> segragArray;
std::vector<int> unhapRed;
std::vector<int> unhapBlue;


//int r1=220, r2=90,r3=160,g1=40,g2=210,g3=190;
//int r1=210, r2=10,r3=80,g1=30,g2=160,g3=230;
//int r1=210, r2=120,r3=10,g1=80,g2=230,g3=100;


void make_pixel_array(void){/*this builds pixel_array from changedhum array*/
    number_stages=iround+1;
    int radius=pixdim/2, radiusa, xc, yc; /*radious of centered circle*/
    radiusa=radius-16;
    pixel_array.resize (pixdim, vector<int> ( pixdim ));
//    ANGLE_INCR = 2*GL_PI/numbhum;/*increase angle parameter*/
//    RADIUS_INCR = radius/number_stages;/*increase angle parameter*/
for (ig=0; ig< changedhum.size(); ig++){
    stage=get<0>(changedhum[ig]);
    indexa=(int)(get<0>(get<1>(changedhum[ig])));
    xc= (int)(radius+stage*radiusa*sin(indexa*GL_PI2/numbhum)/number_stages);
    yc= (int)(radius+stage*radiusa*cos(indexa*GL_PI2/numbhum)/number_stages);
    pixel_array[xc][yc]=get<1>(get<1>(changedhum[ig]));}/*changedhum takes -1 or 1*/}

void write_pixel_array_into_buffer(void){
    image = (unsigned char*) malloc(numbhum * numbhum * 4);
    p = image;
    for (i = 0; i < pixdim; ++i){
        for (j = 0; j < pixdim; ++j) {
            if (pixel_array[i][j]==1){*p++ = r1;}
            if (pixel_array[i][j]==-1){*p++ = g1;}
            if (pixel_array[i][j]==0){*p++ = b1;}
            if (pixel_array[i][j]==1){*p++ = r2;}
            if (pixel_array[i][j]==-1){*p++ = g2;}
            if (pixel_array[i][j]==0){*p++ = b2;}
            if (pixel_array[i][j]==1){*p++ = r3;}
            if (pixel_array[i][j]==-1){*p++ = g3;}
            if (pixel_array[i][j]==0){*p++ = b3;}}}  }


////GRAPHICS ROUTINES

    /*picadd*/ //    WriteBMP(picdir, numbhum, iround+10,  image);
    /*picadd*/void WriteBMP(const char *fname, int ws,int hs,unsigned char *img){
    /*picadd*/    FILE *f = fopen(fname,"wb");
    /*picadd*/    unsigned char bfh[54] = {0x42, 0x4d,
    /*picadd*/        /* bfSize [2]*/ 54, 0, 0, 0, /**/
    /*picadd*/        /* reserved [6]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* biOffBits [10]*/ 54, 0, 0, 0, /**/
    /*picadd*/       /* biSize [14]*/ 40, 0, 0, 0, /**/
    /*picadd*/        /* width [18]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* height [22]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* planes [26]*/ 1, 0, /**/
    /*picadd*/        /* bitcount [28]*/ 24, 0,/**/
    /*picadd*/        /* compression [30]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* size image [34]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* xpermeter [38]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* ypermeter [42]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* clrused [46]*/ 0, 0, 0, 0, /**/
    /*picadd*/        /* clrimportant [50]*/ 0, 0, 0, 0 /**/};
    /*picadd*/    int realw = ws * 3, rem = ws % 4, isz = (realw + rem) * hs, fsz = isz + 54;
    /*picadd*/    //bfh.bfSize = fsz;
    /*picadd*/    bfh[2] = (fsz & 0xFF); bfh[3] = (fsz >> 8) & 0xFF;
    /*picadd*/    bfh[4] = (fsz >> 16) & 0xFF; bfh[5] = (fsz >> 24) & 0xFF;
    /*picadd*/    //bfh.biSize = isz
    /*picadd*/    bfh[34] = (isz & 0xFF); bfh[35] = (isz >> 8) & 0xFF;
    /*picadd*/    bfh[36] = (isz >> 16) & 0xFF; bfh[37] = (isz >> 24) & 0xFF;
    /*picadd*/    //bfh.biWidth = w;
    /*picadd*/    bfh[18] = (ws & 0xFF); bfh[19] = (ws >> 8) & 0xFF;
    /*picadd*/    bfh[20] = (ws >> 16) & 0xFF; bfh[21] = (ws >> 24) & 0xFF;
    /*picadd*/    //bfh.biHeight = h;
    /*picadd*/    bfh[22] = (hs & 0xFF); bfh[23] = (hs >> 8) & 0xFF;
    /*picadd*/    bfh[24] = (hs >> 16) & 0xFF; bfh[25] = (hs >> 24) & 0xFF;
    /*picadd*/   // xpels/ypels
    /*picadd*/    // bfh[38] = 19; bfh[39] = 11;
    /*picadd*/    // bfh[42] = 19; bfh[43] = 11;
    /*picadd*/    fwrite((void*)bfh, 54, 1, f);
    /*picadd*/    unsigned char* bstr = new unsigned char[realw], *remstr = 0;
    /*picadd*/    if(rem != 0) { remstr = new unsigned char[rem]; memset(remstr,0,rem); }
    /*picadd*/    for(int js = hs-1 ; js > -1 ; js--){
    /*picadd*/        for(int is = 0 ; is < ws ; is++)
    /*picadd*/            for(int ks = 0 ; ks < 3 ; ks++) { bstr[is*3+ks] = img[(js*realw+is*3)+(2-ks)]; }
    /*picadd*/        fwrite(bstr,realw,1,f); if (rem != 0) { fwrite(remstr,rem,1,f); }}
    /*picadd*/    delete [] bstr; if(remstr) delete [] remstr;
    /*picadd*/    fclose(f);}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

int signfu(int argt, int unsigned argn){
    //outputs the canonical index of a human given one outside [0,numhum] (range in [-numhum, 2numhum])
    if (argt<0) {return argn + argt;} else if (argt>= argn){return argt-argn;} else {return argt;}}

int SegregFun(void){//outputs the number of boundaries in segregation
    if(numbhumArray[1][0]+ numbhumArray[1][numbhum - 1]==0) {segvar=1;} else {segvar=0;}
    for (iseg=0; iseg<numbhum-1; iseg++) {if(numbhumArray[1][iseg]+ numbhumArray[1][iseg+1]==0) {segvar++;}}
    return segvar;}

// MY SUBROUTINES
void initialcall(void){
    //    tau=0.45; numbhum=4000; ringpar=10; standpar=1; w=10; convar=1; torvar=0, swapmax=500*numbhum; roundrep=10; testmode=0;
    std::cout << " Welcome to Schelling 1D full-history"<<
    " version 2.0, a program by Barmpalias, Elwes and Lewis-Pye.\n\n";//Initial Input Calls
    //    std::cout << " Yes/No answers are given by 1/0\n";
    std::cout << " Time is measured in seconds (s), milliseconds (ms) or microseconds (mus)\n";
    //    std::cout << " Default values are: convention on, max swaps n*1000, report every 5*n, and no test mode\n\n";
    std::cout << " Graphical output controls:\n\n";//Initial Input Calls
    std::cout << "      - Press q for quitting\n";
    std::cout << "      - Press f for fullscreen\n\n";
    
    std::cout << " User input:\n\n";
    std::cout << "      - Give me the number of humans: "; std::cin >> numbhum;
    std::cout << "      - Give me the neighbourhood radius w: "; std::cin >> w;
    std::cout << "      - Give me the tolerance parameter tau: "; std::cin >> tau;
    std::cout << "      - Give me pixel dimension: "; std::cin >> pixdim;
   // std::cout << "      - Display binary?: "; std::cin >> dispz;
    convar=1; swapmax=100*numbhum; roundrep= 10000; testmode=0;
    seedvar = (unsigned int)time (NULL); srand (seedvar);
    std::cout<< "\n\n Random seed is "<< seedvar;}

void buildhumans(void){
    for (i=0; i< numbhum; i++){//Build human array and announce
        if (rand () % 2 == 0 ) {numbhumArray[0][i] = -1; numbhumArray[1][i] = -1; bluenu++;}
        else {numbhumArray[0][i]= 1; numbhumArray[1][i]= 1; rednu++;}}
    
    finnumbhum = std::chrono::system_clock::now();
    long long finnumbhms = std::chrono::duration_cast<std::chrono::milliseconds> (finnumbhum-start).count();
    std::cout << "\n"<<" Produced randomly coloured humans: "<<rednu<< " red and "<<bluenu<<" blue"<<
    " (in "<<finnumbhms<< " ms)\n";}

void timestart(void){
    start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << "\n Computation started at " << std::ctime(&start_time);}

void timeendroutine(void){
    fend = std::chrono::system_clock::now();
    long long elapsed_minutes = std::chrono::duration_cast<std::chrono::minutes> (fend-start).count();
    long long elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds> (fend-start).count();
    long long elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> (fend-start).count();
    std::time_t end_time = std::chrono::system_clock::to_time_t(fend);
    std::cout << " Total elapsed time: ";
    //    if (!(elapsed_minutes==0)) {cout<< elapsed_minutes<< " min ";}
    //    if (!(elapsed_seconds==0)) {cout<< elapsed_seconds % 60 << "s ";}
    //    cout << elapsed_milliseconds % 1000<< "ms\n";
    //    std::cout << " Finished overall computation at " << std::ctime(&end_time)<<"\n";}
    if (!(elapsed_minutes==0)){std::cout << elapsed_minutes<< " min "; sprintf(tstri0, "%d%s", (int)elapsed_minutes, "m ");}
    else {sprintf(tstri0, "%s", "");}
    if (!(elapsed_seconds==0)) {std::cout<< elapsed_seconds % 60 << " s "; sprintf(tstri1, "%d%s", (int)elapsed_seconds % 60, "s ");}
    else {sprintf(tstri1, "%s", "");}
    std::cout<< elapsed_milliseconds % 1000<< " ms\n";
    sprintf(tstri2, "%d%s", (int)elapsed_seconds % 1000, "ms ");
    sprintf(spstring2, "%s%s%s%s", "Duration: ", tstri0, tstri1, tstri2);
    std::cout << " Finished overall computation at " << std::ctime(&end_time)<<"\n";
    sprintf(spstring5, "%.24s", std::ctime(&end_time));}


void buildinitbiasarr(void){
    biasvar = 0;    //calculate the bias of numbhumArray[1] [0]
    for (i=0; i < 2*w + 1; i++){biasvar = biasvar + numbhumArray[1][signfu(i-w,numbhum)];}
    biasArray[0]=biasvar;
    if ((numbhumArray[1][0]==1) && (biasArray[0] < (2*w+1)*(2*tau -1))){unhapRed.push_back (0); numbhumArray[0][0]=2;}
    else if ((numbhumArray[1][0]==-1) && (biasArray[0] > (2*w+1)*(1-2*tau))){unhapBlue.push_back (0); numbhumArray[0][0]=-2;}
    
    for (i=0; i < numbhum-1; i++){//calculate the bias of the rest of biasArray [numbhum]
        biasArray[i+1] =biasArray[i] + numbhumArray[1][signfu(i+1+w, numbhum)]
        - numbhumArray[1][signfu(i-w, numbhum)];
        if ((numbhumArray[1][i+1]==1) && (biasArray[i+1] < (2*w+1)*(2*tau -1))){unhapRed.push_back (i+1); numbhumArray[0][i+1]=2;}
        else if ((numbhumArray[1][i+1]==-1) && (biasArray[i+1] > (2*w+1)*(1-2*tau))){unhapBlue.push_back (i+1);
            numbhumArray[0][i+1]=-2;} } }

void doswapfollowconv(void){
    if (roundvar % roundrep ==0) {swaptime0 = std::chrono::system_clock::now();}
    x= rand () % unhapRed.size();//pick the unhappy to swap
    y= rand () % unhapBlue.size();
    swapx = unhapRed[x];// Record last swap
    swapy = unhapBlue[y];
    
    while (tau > 0.5 && convar==1 &&
           ( (biasArray[swapx] > biasArray[swapy] + 2) || (abs(swapx - swapy) <=w) ||
            (numbhum - abs(swapx - swapy)<=w) )){//keep convension for >0.5
               unsucswap++;
               x= rand () % unhapRed.size();//pick the unhappy to swap
               y= rand () % unhapBlue.size();
               swapx = unhapRed[x];// Record last swap
               swapy = unhapBlue[y];}
    
    numbhumArray[1][swapx] = -1;// Do the swap;
    numbhumArray[1][swapy] = 1;
    changedhum.push_back (make_tuple(iround, make_tuple(swapx, -1)));
    //    changedhum[iround][swapx]=-1;
    changedhum.push_back (make_tuple(iround, make_tuple(swapy, 1)));
    //    changedhum[iround][swapy]=1;
    if (testmode==1){std::cout<< "\n I swap "<< swapx << " with "<<  swapy << "\n";}
    unhapRed.erase (unhapRed.begin()+x);//Erase member with index x from unhapRed
    unhapBlue.erase (unhapBlue.begin()+y);}

void updatebiaslists(void){
    if (roundvar % roundrep ==0) {swaptime1 = std::chrono::system_clock::now();}
    for (i=0; i < 2*w + 1; i++){//Update the bias around humans unhapRed[x], unhapRed[x]
        biasvarR=signfu(swapx-w+i, numbhum);
        biasvarB=signfu(swapy-w+i, numbhum);
        biasRtargOld=biasArray[biasvarR]; biasRtargNew =biasRtargOld;
        biasBtargOld=biasArray[biasvarB]; biasBtargNew =biasBtargOld;
        if ( (abs (biasvarR - swapy) > w) && (numbhum - abs (biasvarR - swapy) > w) ){// if far from swapy
            biasArray[biasvarR] = biasArray[biasvarR]-2; biasRtargNew = biasRtargNew-2;
            if ((numbhumArray[1][biasvarR]==1)//if red
                && (biasRtargNew < (2*tau-1)*(2*w+1))//and unhappy
                &&//and was happy before, i.e. not currently in the list
                (biasRtargOld >= (2*tau-1)*(2*w+1)) ) {
                unhapRed.push_back (biasvarR);}//then put it in
            else if ((numbhumArray[1][biasvarR]==-1)//if blue
                     &&//and was unhappy before, i.e. currently in the list
                     (biasRtargOld > (2*w+1)*(1-2*tau)) &&// and its not swapx
                     (!(biasvarR==swapx))
                     &&//but is happy now
                     (biasRtargNew <= (2*w+1)*(1-2*tau)))//then take it out of the list
            {unhapBlue.erase(std::remove(unhapBlue.begin(), unhapBlue.end(),
                                         biasvarR), unhapBlue.end());}/*END of 2nd if*/ }//end of first IF
        if ( (abs (biasvarB - swapx) > w) && (numbhum - abs (biasvarB - swapx) > w) ) {//if far from swapx
            biasArray[biasvarB] = biasArray[biasvarB]+2; biasBtargNew = biasBtargNew+2;
            if ((numbhumArray[1][biasvarB]==1)//if red
                && (biasBtargNew >= (2*tau-1)*(2*w+1) ) //and happy
                &&//and was unhappy before and is not swapy, i.e. not currently in the list
                (biasBtargOld < (2*tau-1)*(2*w+1)) &&
                ( !(biasvarB==swapy)) ){
                unhapRed.erase(std::remove(unhapRed.begin(), unhapRed.end(),
                                           biasvarB), unhapRed.end());}
            else if ((numbhumArray[1][biasvarB]==-1)//if blue
                     &&//and was happy before
                     (biasBtargOld <= (2*w+1)*(1-2*tau))
                     &&//but unhappy now
                     (biasBtargNew > (2*w+1)*(1-2*tau) ) )//then put it in the list
            {unhapBlue.push_back (biasvarB);} }/*END OF OUTER IF FOR biasvarB*/ }//End of FOR
    if ( biasArray[swapx] > (2*w+1)*(1 - 2*tau) )//if it is swap x and unhappy then put it in
    {unhapBlue.push_back (swapx);}
    if ( biasArray[swapy] < (2*w+1)*(2*tau -1) )//if it is swap y and unhappy then put it in
    {unhapRed.push_back (swapy);}
    if (roundvar % roundrep ==0) {swaptime2 = std::chrono::system_clock::now();} }//END OF ROUTINE

int loopstopcondition(void){
    if (unhapRed.empty()) {std::cout <<
        " Finished swapping after "<< roundvar << " swaps:\n"<<
        "\n There are no more unhappy red humans (reason for stopping swapping)\n";
        sprintf(spstring1, "%s%d", "Swaps: ", roundvar); return 0;}
    else if (unhapBlue.empty()) {std::cout <<
        " Finished swapping after "<< roundvar << " swaps:\n"<<
        " There are no more unhappy blue humans (reason for stopping swapping)\n";
        sprintf(spstring1, "%s%d", "Swaps: ", roundvar); return 0;}
    else if (roundvar % 500 ==0 && SegregFun()==2) {std::cout <<
        " Finished swapping after " << roundvar << " swaps:\n" <<
        "\n Arrived at total segregation (reason for stopping swapping)\n";
        sprintf(spstring1, "%s%d", "Swaps: ", roundvar); return 0;}
    else {return 1;} }

///REPORT SUBROUTINES (PART OF MAIN PROGRAM)
void reportunhapnumb(void){
    finbiasArray = std::chrono::system_clock::now();
    long long finbiasArrayms = std::chrono::duration_cast<std::chrono::milliseconds> (finbiasArray-start).count();
    std::cout << " Compiled initial bias and unhappy arrays (in " << finbiasArrayms << " ms)\n";
    std::cout << " We have "<< unhapRed.size() << " unhappy red and " << unhapBlue.size() << " unhappy blue\n";
    std::cout << "\n"<< " Started swapping ...\n\n";
    looptimeno0=std::chrono::system_clock::now();}

void reportnumbswapscur(void){
    if ((roundvar % roundrep ==0) && !(roundvar==0) ) {
        looptime1 = std::chrono::system_clock::now();
        looptimedif = std::chrono::duration_cast<std::chrono::milliseconds> (looptime1-looptimeno0).count();
        std::cout << "\n\n " << roundvar +1 << " swaps completed and going ...";
        if (convar==1 && tau>0.5) {std::cout << "\n\n " << unsucswap <<" unsuccessful swap attempts\n";}
        std::cout << " Unhappy: "<< unhapRed.size()<< " red and "<< unhapBlue.size()<<" blue\n\n";
        looptimeno0 = looptime1;
        swaptimedif1 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime1-swaptime0).count();
        swaptimedif2 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime2-swaptime1).count();
        
        std::cout << "      Time report on swap "<<roundvar +1<<":\n\n";
        std::cout <<"           - Last "<< roundrep << " swaps in "<< looptimedif << " ms\n";
        std::cout <<"           - "<< swaptimedif1<<" mus for swap and erase from unhappy lists\n";
        std::cout <<"           - "<< swaptimedif2<<" mus for updating the bias array and unhappy lists\n\n\n";} }

void outputfinalnumbers (void){//Output number of unhappy and runs
    std::cout << "\n The number of the finally unhappy is "<< unhapRed.size()<< " red and "<< unhapBlue.size()<<" blue\n";
    sprintf(spstring4, "%s%d%s%d%s", "Unhappy: ",  (int)unhapRed.size(), " red, ", (int)unhapBlue.size(), " blue");
    std::cout << "\n There are " << SegregFun() << " monochromatic runs\n";
    sprintf(spstring3, "%s%d", "Blocks: ", SegregFun());
    std::cout << " There were "<< roundvar<<" swaps\n";
    if (convar==1 && tau>0.5) {std::cout << " There were "<< unsucswap<<" unsuccessful swap attempts\n";} }

void keyb(unsigned char key, int x, int y){
    switch (key){
        case 'q':  exit (0);  break;
            //        case 'h': flagpause=!flagpause; break;
    } }

int signpos(int xsignpos){if (xsignpos>=0){return xsignpos;} else {return 0;}}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    initialcall();
    start = std::chrono::system_clock::now();//new timer
    timestart();
    numbhumArray.resize (2, vector<int> ( numbhum ));
    biasArray.resize (numbhum);
    buildhumans();
    buildinitbiasarr();
    reportunhapnumb();
    std::cout<< " Should I proceed? (0/1) "; std::cin>>quitvar;
    if (quitvar==1){
        while(loopstopcondition()==1){iround++; doswapfollowconv();
            updatebiaslists(); reportnumbswapscur(); roundvar++;}
        outputfinalnumbers(); timeendroutine();
/*picadd*/     make_pixel_array(); write_pixel_array_into_buffer();
/*picadd*/     sprintf(picdir, "%s%d%s", "./", seedvar, ".bmp");
/*picadd*/     WriteBMP(picdir, pixdim, pixdim,  image);
/*picadd*/     std::cout<< " Image saved: "<< picdir<< "\n\n";

//        if (dispz==1){
//        for (i=0; i<pixdim; i++){
//            for (j=0; j<pixdim; j++){
//                cout<<  pixel_array[i][j];}
//            cout<<"\n";}}
        cout<<"\n";
//        cout<<"ANGLE_INCR: "<<ANGLE_INCR<<"\n";
//        cout<<"RADIUS_INCR: "<<RADIUS_INCR<<"\n";
    }
    else {std::cout<< "\n Program terminated.\n\n";}

    
    
    
    return 0;}
