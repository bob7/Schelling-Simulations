//Schel1Danimv2
//this version has smoother graphics
//they are done via overlays of triangle fans as opposed to points which was the way
//that was used in v.1. I have also added movement controls.

//LIBRARIES
#include <GLUT/glut.h>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <vector>
#include <tuple>
#include <fstream>
#include <unistd.h>
using namespace std;
typedef unsigned char byte;
char filenamet [ FILENAME_MAX ];
bool flagpic=true;
char curdirout[]=".";
const char basedata[] = "Schel1Danim_";
char foldername [ FILENAME_MAX ];
const char extbmp[] = ".bmp";
char picname [ FILENAME_MAX ];
char picdir [ FILENAME_MAX ];
char schenodir [ FILENAME_MAX ];
ofstream myfilef;
ofstream myfilet;
int iwrite, number;

int ysa, ringpar;
//VECTOR ARRAYS
std::vector<vector<int> > numbhumArray;
//std::vector<int> numbhumArray;
//std::vector<vector<int> > changedhum;
std::vector<tuple <int, tuple <int, int>> > changedhum;
std::vector<int> biasArray;
std::vector<int> segragArray;
std::vector<int> unhapRed;
std::vector<int> unhapBlue;
unsigned int seedvar;
float revar=1;
int redper;
//time variables
std::chrono::time_point<std::chrono::system_clock> start, fend, finnumbhum, finbiasArray, looptimen0, looptime1;
std::chrono::time_point<std::chrono::system_clock> swaptime0, swaptime1, swaptime2;
long long looptimedif, swaptimedif1, swaptimedif2, swaptimedif3, swaptimedif4;
bool flagra=false, redshow=true, greenshow=true, revealun=false;
char spstring0 [ FILENAME_MAX ], spstring1 [ FILENAME_MAX ], spstring2 [ FILENAME_MAX ],
spstring3 [ FILENAME_MAX ], spstring4 [ FILENAME_MAX ],
spstring5 [FILENAME_MAX], tstri0 [ FILENAME_MAX ],
tstri1 [ FILENAME_MAX ], tstri2 [ FILENAME_MAX ];
//CONSTANTS AND VARIABLES
#define GL_PI  3.141592653f   // Define a constant for the value of PI

int numbhum, segvar, i, ig, jg, j, k, x, y, roundvar, roundrep, iseg, swapmax;
int w, swapx, swapy, iround=0, extrarad=0;
float tau;
int  biasvar, biasvarR, biasvarB, rednu, bluenu, biasRtargOld, biasRtargNew,biasBtargNew, biasBtargOld;
int targvarnAr, targvarnAb, varBiasAr, varBiasAb, artargvarnAr, artargvarnAb;
int standpar, convar, torvar, testmode, unsucswap=0;
int widtha, heighta;
float zoomvar = 50.0, translvarx = -0.0f, translvary = -0.0f, pointsiz=1.0;
bool fullScreen = false, flagend=false, flagstart=false;
int initoffset=10;
float radiousvar = 0;//radious of circle
GLfloat xg,yg, xg1, yg1, xg2, yg2, xg3, yg3, angle; // Storage for coordinates and angles
void detailedreport(void);
//GRAPHICS ROUTINES

void idle();

void idle(void){}

void snapshot(int ws, int hs, char* filename)
{    byte* bmpBuffer = (byte*)malloc(ws*hs*3);
    if (!bmpBuffer)
        return;
    
    glReadPixels((GLint)0, (GLint)0,
                 (GLint)ws-1, (GLint)hs-1,
                 GL_RGB, GL_UNSIGNED_BYTE, bmpBuffer);
    
    FILE *filePtr = fopen(filename, "wb");
    if (!filePtr)
        return;
    
    unsigned char bfh[54] = {0x42, 0x4d,
        /* bfSize [2]*/ 54, 0, 0, 0, /**/
        /* reserved [6]*/ 0, 0, 0, 0, /**/
        /* biOffBits [10]*/ 54, 0, 0, 0, /**/
        /* biSize [14]*/ 40, 0, 0, 0, /**/
        /* width [18]*/ 0, 0, 0, 0, /**/
        /* height [22]*/ 0, 0, 0, 0, /**/
        /* planes [26]*/ 1, 0, /**/
        /* bitcount [28]*/ 24, 0,/**/
        /* compression [30]*/ 0, 0, 0, 0, /**/
        /* size image [34]*/ 0, 0, 0, 0, /**/
        /* xpermeter [38]*/ 0, 0, 0, 0, /**/
        /* ypermeter [42]*/ 0, 0, 0, 0, /**/
        /* clrused [46]*/ 0, 0, 0, 0, /**/
        /* clrimportant [50]*/ 0, 0, 0, 0 /**/};
    int realw = ws * 3, rem = ws % 4, isz = (realw + rem) * hs, fsz = isz + 54;
    //bfh.bfSize = fsz;
    bfh[2] = (fsz & 0xFF); bfh[3] = (fsz >> 8) & 0xFF;
    bfh[4] = (fsz >> 16) & 0xFF; bfh[5] = (fsz >> 24) & 0xFF;
    //bfh.biSize = isz
    bfh[34] = (isz & 0xFF); bfh[35] = (isz >> 8) & 0xFF;
    bfh[36] = (isz >> 16) & 0xFF; bfh[37] = (isz >> 24) & 0xFF;
    //bfh.biWidth = w;
    bfh[18] = (ws & 0xFF); bfh[19] = (ws >> 8) & 0xFF;
    bfh[20] = (ws >> 16) & 0xFF; bfh[21] = (ws >> 24) & 0xFF;
    //bfh.biHeight = h;
    bfh[22] = (hs & 0xFF); bfh[23] = (hs >> 8) & 0xFF;
    bfh[24] = (hs >> 16) & 0xFF; bfh[25] = (hs >> 24) & 0xFF;
    // xpels/ypels
    // bfh[38] = 19; bfh[39] = 11;
    // bfh[42] = 19; bfh[43] = 11;
    fwrite((void*)bfh, 54, 1, filePtr);
    
    unsigned char* bstr = new unsigned char[realw], *remstr = 0;
    if(rem != 0) { remstr = new unsigned char[rem]; memset(remstr,0,rem); }
    for(int js = 0 ; js < hs; js++){
        for(int is = 0 ; is < ws ; is++){
            for(int ks = 0 ; ks < 3 ; ks++) { bstr[is*3+ks] = bmpBuffer[(js*realw+is*3)+(2-ks)]; }}
        fwrite(bstr,realw,1,filePtr);
        if (rem != 0) { fwrite(remstr,rem,1,filePtr); }}
    delete [] bstr; if(remstr) delete [] remstr;
    
    fwrite(bmpBuffer, ws*hs*3, 1, filePtr);
    fclose(filePtr);
    free(bmpBuffer);}

void keyboard(string key, float x, float y)
{glColor3ub((GLubyte)200,(GLubyte)205,(GLubyte)203);
    glRasterPos2f(x, y);
    for (i=0; i< key.length(); i++){
        glutBitmapCharacter(
                            //GLUT_BITMAP_HELVETICA_18
                            //GLUT_BITMAP_TIMES_ROMAN_10
                            GLUT_BITMAP_9_BY_15
                            // GLUT_BITMAP_8_BY_13
                            , key[i]);}}


void coroutr(void){
    glColor3ub((GLubyte)255,(GLubyte)80,(GLubyte)60);/*red*/
    //glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)0);/*black*/
    
}
void coroutb(void){
    //        glColor3ub((GLubyte)80,(GLubyte)80,(GLubyte)255);/*blue*/
    //glColor3ub((GLubyte)135,(GLubyte)135,(GLubyte)135);/*grey*/
    glColor3ub((GLubyte)173,(GLubyte)255,(GLubyte)173);/*green*/
}


//void ProcessMenu(int value)
//{switch(value)
//    {case 1:
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            glEnable(GL_BLEND);
//            glEnable(GL_POINT_SMOOTH);
//            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//            glEnable(GL_LINE_SMOOTH);
//            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//            glEnable(GL_POLYGON_SMOOTH);
//            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);break;
//        case 2:
//            // Turn off blending and all smoothing
//            glDisable(GL_BLEND);
//            glDisable(GL_LINE_SMOOTH);
//            glDisable(GL_POINT_SMOOTH);
//            glDisable(GL_POLYGON_SMOOTH); break;
//        case 3: glShadeModel(GL_FLAT); break;
//        case 4: glShadeModel(GL_SMOOTH); break;
//        default: break;}
//    glutPostRedisplay();}


void reshape(int width, int height)
{   widtha=width;
    heighta=height;
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, width, height);
    glTranslatef(translvarx, translvary, 0.0f);
    
	gluPerspective(zoomvar,(GLfloat) width/(GLfloat) height, 0.1, 5);
	gluLookAt(0.0f, 0.0f, 5.0f,
              0.0f, 0.0f, 0.0f,
              0, 1, 0); }

//void keybspec(int ket, int x, int y){
//    switch (ket){
//        case GLUT_KEY_RIGHT: if (translvarx <1.5) {translvarx += 0.05;} break;
//        case GLUT_KEY_LEFT: if (translvarx > -1.5) {translvarx -= 0.05;}  break;
//        case GLUT_KEY_UP: if (translvary <1.5) {translvary += 0.05;}    break;
//        case GLUT_KEY_DOWN: if (translvary > -1.5) {translvary -= 0.05;}  break;}
//    glutPostRedisplay();
//    reshape(widtha, heighta);}

//MY FUNCTIONS

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
    std::cout << " Welcome to Schelling 1D full-history animation"<<
    " version 2.0, a program by Barmpalias, Elwes and Lewis-Pye.\n\n";//Initial Input Calls
    
    //    std::cout << " Yes/No answers are given by 1/0\n";
    std::cout << " Time is measured in seconds (s), milliseconds (ms) or microseconds (mus)\n";
    //    std::cout << " Default values are: convention on, max swaps n*1000, report every 5*n, and no test mode\n\n";
    
    std::cout << " Graphical output controls:\n\n";//Initial Input Calls
    std::cout << "      - Press q for quitting\n";
    std::cout << "      - Press f for fullscreen\n\n";
    //    std::cout << "      - Press p,l for enlarging or shrinking the circle\n";
    //    std::cout << "      - Press o,k for enlarging or shrinking the initial circle\n";
    //    std::cout << "      - Press i,j for enlarging or shrinking the size of the points\n";
    //    std::cout << "      - Press u,h for hiding/revealing the red/green part\n";
    //    std::cout << "      - Press y,t for revealing the initial unhappy (extending the corresponding lines)\n";
    //    std::cout << "      - Press the keyboard arrows for moving the circle around\n\n";
    
    std::cout << " User input:\n\n";
    std::cout << "      - Give me the number of humans: ";//Initial Input Calls
    std::cin >> numbhum;
    std::cout << "      - Give me the neighbourhood radius w: ";
    std::cin >> w;
    std::cout << "      - Give me the tolerance parameter tau: ";
    std::cin >> tau;
    std::cout << "      - Give me the percentage of green (1/p): ";
    std::cin >> redper;
//    std::cout << "      - Every how many swaps should I update the frame? ";
//    std::cin >> ringpar;
    ringpar=100;
    standpar=1;
    //    std::cout << "      - The rest default? ";
    //    std::cin >> standpar;
    
    if (standpar==0){
        if (tau>0.5) {std::cout << "      - Convention for tau>0.5? (0 or 1) ";
            std::cin >> convar;}
        //        std::cout << "      - Torus or not ? (answer with 1,0 respectively): ";
        //        std::cin >> torvar;
        std::cout << "      - Give me the max number of swaps allowed to do: ";
        std::cin >> swapmax;
        std::cout << "      - Every how many swaps should I report completion? ";
        std::cin >> roundrep;
        std::cout << "      - Test mode? (0 or 1) ";
        std::cin >> testmode;}
    else {convar=1; roundrep=6000; testmode=0;}
    seedvar = (unsigned int)time (NULL);
    srand (seedvar);
    std::cout<< "\n Random seed is "<< seedvar;}

void buildhumans(void){
    for (i=0; i< numbhum; i++){//Build human array and announce
        if (rand () % redper == 0 ) {numbhumArray[0][i] = -1; numbhumArray[1][i] = -1; bluenu++;}
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
    std::cout << " Total elapsed time: " ;
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
            numbhum - abs(swapx - swapy)<=w) ) {//keep the convension for >0.5
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
    //    if (testmode==1){std::cout<< "\n I swap "<< swapx << " with "<<  swapy << "\n";}
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
                                         biasvarR), unhapBlue.end());}//END of 2nd if
        }//end of first IF
        
        if ( (abs (biasvarB - swapx) > w) && (numbhum - abs (biasvarB - swapx) > w) ) {//if far from swapx
            
            biasArray[biasvarB] = biasArray[biasvarB]+2; biasBtargNew = biasBtargNew+2;
            //now update unhappy lists for this point  biasvarB
            
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
            {unhapBlue.push_back (biasvarB);} }//END OF OUTER IF FOR biasvarB
    }//End of FOR
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
    else if (SegregFun()==2) {std::cout <<
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
    std::cout << "\n"<< " Started swapping ...\n\n";}

void reportnumbswapscur(void){
    if ((roundvar % roundrep ==0) && !(roundvar==0) ) {
        looptime1 = std::chrono::system_clock::now();
        looptimedif = std::chrono::duration_cast<std::chrono::milliseconds> (looptime1-looptimen0).count();
        std::cout << "\n\n " << roundvar +1 << " swaps completed and going ...";
        if (convar==1 && tau>0.5) {std::cout << "\n\n " << unsucswap <<" unsuccessful swap attempts\n";}
        std::cout << "(Unhappy: "<< unhapRed.size()<< " red and "<< unhapBlue.size()<<" blue)\n\n";
        looptimen0 = looptime1;
        swaptimedif1 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime1-swaptime0).count();
        swaptimedif2 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime2-swaptime1).count();
        
        std::cout << "      Time report on swap "<<roundvar +1<<":\n\n";
        std::cout <<"           - Last "<< roundrep << " swaps in "<< looptimedif << " ms\n";
        std::cout <<"           - "<< swaptimedif1<<" mus for swap and erase from unhappy lists\n";
        std::cout <<"           - "<< swaptimedif2<<" mus for updating the bias array and unhappy lists\n\n\n";
        /*if (testmode==1){detailedreport();}*/ } }

void outputfinalnumbers (void){//Output number of unhappy and runs
    std::cout << "\n The number of the finally unhappy is "<< unhapRed.size()<< " red and "<< unhapBlue.size()<<" blue\n";
    sprintf(spstring4, "%s%d%s%d%s", "Unhappy: ",  (int)unhapRed.size(), " red, ", (int)unhapBlue.size(), " blue");
    std::cout << "\n There are " << SegregFun() << " monochromatic runs\n";
    sprintf(spstring3, "%s%d", "Blocks: ", SegregFun());
    std::cout << " There were "<< roundvar<<" swaps\n";
    if (convar==1 && tau>0.5) {std::cout << " There were "<< unsucswap<<" unsuccessful swap attempts\n";} }

///REPORT SUBROUTINES (FOR TESTING)

void detailedreport(void){//FOR TESTING
    std::cout << "\n"; //output the human/bias array
    std::cout << " The human/bias array is:\n";
    std::cout << "\n";
    for (i=0; i< numbhum; i++){
        if ((i % 10 ==0) && !(i==0)){std::cout<<"\n";}
        if (biasArray[i]<0 && numbhumArray[1][i]==1) {std::cout << " "<< biasArray[i]<< "x ";}
        else if (biasArray[i]<0 && numbhumArray[1][i]==-1) {std::cout << " "<< biasArray[i]<< "o ";}
        else if (biasArray[i]>=0 && numbhumArray[1][i]==1){std::cout << " "; std::cout << " "<< biasArray[i]<< "x ";}
        else if (biasArray[i]>=0 && numbhumArray[1][i]==-1){std::cout << " "; std::cout << " "<< biasArray[i]<< "o ";}}
    std::cout << "\n";
    
    std::cout << "\n"; std::cout << "\n"; std::cout << " The unhappy red are:\n"; std::cout << "\t";
    for (i=0; i < unhapRed.size(); i++){std::cout << unhapRed[i]<<" "; if ((i % 10 ==0) && !(i==0)){std::cout<<"\n";
        std::cout << "\t";}}
    std::cout << "\n"; std::cout << "\n The unhappy blue are:\n"; std::cout << "\t";
    for (i=0; i < unhapBlue.size(); i++){std::cout << unhapBlue[i]<<" "; if ((i % 10 ==0) && !(i==0)){std::cout<<"\n";
        std::cout << "\t";}}
    rednu=0;
    bluenu=0;
    for (i=0; i< numbhum; i++){if (numbhumArray[1][i]==1){rednu++;} else if (numbhumArray[1][i]==-1){bluenu++;} }
    std::cout<< "\n\n There are "<< rednu<< " red and " << bluenu<< " blue";
    std::cout << "\n";
    rednu=0;
    bluenu=0;
    for (i=0; i< unhapRed.size(); i++){if (numbhumArray[1][unhapRed[i]]==-1){rednu++;}}
    for (i=0; i< unhapBlue.size(); i++){if (numbhumArray[1][unhapBlue[i]]==1){bluenu++;}}
    std::cout<< "\n There are "<< rednu<< " blue in unhapRed and " << bluenu<< " red in unhap Blue";
    std::cout << "\n";}

void loopmainfeed(void){
    if (loopstopcondition()==1){
        doswapfollowconv();
        updatebiaslists();
        reportnumbswapscur();
        roundvar++;}// End of Swapping loop
    else {if(!flagend){outputfinalnumbers(); timeendroutine(); flagend=true;}} }

void keyb(unsigned char key, int x, int y){
    switch (key){
        case 'f': fullScreen = !fullScreen; if (fullScreen) {glutFullScreen();}
        else {glutPositionWindow(0,0); glutReshapeWindow(900, 900);} break;
        case 'q':  exit (0);  break;
            //        case 'l': zoomvar += 2.0; break;
            //        case 'p': zoomvar-= 2.0; break;
        case 'g': flagstart=!flagstart; break;
            //        case 'i': pointsiz += 1.0;  glPointSize(pointsiz); break;
            //        case 'j': pointsiz -= 1.0;  glPointSize(pointsiz); break;
            //        case 'u': redshow=!redshow;   break;
            //        case 'h': greenshow=!greenshow;  break;
    }
    
    glutPostRedisplay();
    reshape(widtha, heighta);}

int signpos(int xsignpos){if (xsignpos>=0){return xsignpos;} else {return 0;}}

void display(void) {
    float INCRPAR = 2*GL_PI/numbhum;//increase angle parameter
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    
    for(ig=0; ig< numbhum; ig++){
        angle=INCRPAR*ig;
        xg =  (0.1)*sin(angle);
        yg =  (0.1)*cos(angle);
        xg1 = (0.2)*sin(angle);
        yg1 = (0.2)*cos(angle);
        xg2 = (0.1)*sin(angle+INCRPAR);
        yg2 = (0.1)*cos(angle+INCRPAR);
        xg3 = (0.2)*sin(angle+INCRPAR);
        yg3 = (0.2)*cos(angle+INCRPAR);
        
        if (numbhumArray[0][ig]>0){glColor3ub((GLubyte)255,(GLubyte)99,(GLubyte)71);}
        else if (numbhumArray[0][ig]<0) {glColor3ub((GLubyte)173,(GLubyte)255,(GLubyte)173);}
        
        glBegin(GL_QUADS);
        glVertex2f(xg, yg); glVertex2f(xg1, yg1);
        glVertex2f(xg3, yg3); glVertex2f(xg2, yg2);
        glEnd();
        
        if (abs(numbhumArray[0][ig])>1){
            glBegin(GL_LINES);
            glVertex2f(0.25*sin(angle), 0.25*cos(angle));
            glVertex2f(0.3*sin(angle), 0.3*cos(angle));
            glEnd();}}
    
    
    glBegin(GL_QUADS);
    for(ig=0, angle = 0.0f*GL_PI; ig< numbhum && angle <= 2.0f*GL_PI; angle += INCRPAR, ig++){
        xg = (2)*sin(angle);
        yg = (2)*cos(angle);
        xg1 = (2.2)*sin(angle);
        yg1 = (2.2)*cos(angle);
        xg2 = (2.2)*sin(angle+INCRPAR);
        yg2 = (2.2)*cos(angle+INCRPAR);
        xg3 = (2)*sin(angle+INCRPAR);
        yg3 = (2)*cos(angle+INCRPAR);
        
        if (numbhumArray[1][ig]==1 ){glColor3ub((GLubyte)255,(GLubyte)99,(GLubyte)71);}
        else if (numbhumArray[1][ig]==-1 ) {glColor3ub((GLubyte)173,(GLubyte)255,(GLubyte)173);}
        else {glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)0);}
        glVertex2f(xg, yg); glVertex2f(xg1, yg1);
        if (numbhumArray[1][signfu(ig+1, numbhum)]==1){glColor3ub((GLubyte)255,(GLubyte)99,(GLubyte)71);}
        else if (numbhumArray[1][signfu(ig+1, numbhum)]==-1 && greenshow) {glColor3ub((GLubyte)173,(GLubyte)255,(GLubyte)173);}
        else {glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)0);}
        glVertex2f(xg2, yg2); glVertex2f(xg3, yg3); }
    glEnd();
    
    
    for (ig=0; ig< changedhum.size(); ig++){
        radiousvar=0.33+1.6*( (float)(get<0>(changedhum[ig]) )/(float)(iround+1));
        angle= INCRPAR*(get<0>(get<1>(changedhum[ig])));
        if(get<1>(get<1>(changedhum[ig]))==1){coroutr();}
        else {coroutb();}
        xg = radiousvar*sin(angle); yg = radiousvar*cos(angle);
        glBegin(GL_POINTS);
        glVertex2f(xg, yg);
        glEnd();}
    
    keyboard("Schelling 1D animation",-2.4,2.2);
    keyboard(spstring5,-2.4,2.1);
    keyboard(spstring0,-2.4,-2.27);
    if (flagend){keyboard(spstring2, 1.4,2.2); keyboard(spstring1,1.4,2.1);
        keyboard(spstring3, 1.4,2); keyboard(spstring4, 1,-2.27);}
    
    //    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    
    if (flagend && flagpic ){
        snapshot(900, 900, picdir);
        flagpic=false;
        myfilet.close();}
    
    while(flagstart && !flagend){
        if (roundvar % ringpar ==0){iround++; loopmainfeed(); break;}
        else {iround++; loopmainfeed();} }
    
}


void drawtheoutput(int argc, char **argv){
    glutInitWindowPosition(0,0);
    glutInitWindowSize(900,900);
    glutInit(&argc, argv);
    //    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Schelling 1D animation");
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //    glutCreateMenu(ProcessMenu);
    //	glutAddMenuEntry("Antialiased Rendering",1);
    //	glutAddMenuEntry("Normal Rendering",2);
    //	glutAddMenuEntry("Flat shade",3);
    //	glutAddMenuEntry("Smooth shade",4);
    //	glutAttachMenu(GLUT_LEFT_BUTTON);
    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glutInitDisplayMode(GLUT_DOUBLE);
    
    glutDisplayFunc(display);
    //    init();
    glutKeyboardFunc(keyb);
    //    glutSpecialFunc(keybspec);
    glutIdleFunc(idle);
    glutMainLoop();}


//MAIN PROGRAM
int main(int argc, char **argv) {
    system("mkdir Schel1Danimruns");
    sprintf(schenodir, "%s%s%s", curdirout, "/Schel1Danimruns/", "SchelNo.txt");
    //  Check if there is protocole file, if not make one with data 1
    ifstream myfilew(schenodir);
    if (!myfilew) {
        ofstream myfilew;
        myfilew.open (schenodir);
        myfilew << 0;
        myfilew.close();}
    //get the protocol number and put it into iwrite
    ifstream myfile;
    myfile.open (schenodir);
    myfile >> iwrite;
    myfile.close();
    iwrite++;
    // create the NAME OF the corresponding file and put in filename charvar
    number = iwrite;
    sprintf(picname, "%s%d%s", basedata, number, extbmp);
    sprintf(picdir, "%s%s%s", curdirout, "/Schel1Danimruns/", picname);
    
    initialcall();
    start = std::chrono::system_clock::now();//new timer
    timestart();
    numbhumArray.resize (2, vector<int> ( numbhum ));
    biasArray.resize (numbhum);
    buildhumans();
    buildinitbiasarr();
    reportunhapnumb();
    sprintf(spstring0, "%s%d%s%d%s%.2f", "n=", numbhum, ", w=", w, ", tau=", tau);
    //increase protocole number for writing data
    myfilef.open (schenodir);
    myfilef << iwrite;
    myfilef.close();
    myfilet.open (picdir);
    
    drawtheoutput(argc, argv);
    return 0;}
