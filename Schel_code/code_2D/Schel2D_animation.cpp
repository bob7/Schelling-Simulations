
//  Schell2Danim version 2.1
//This is based on image renewal after a certain number of swaps
//As opposed to a certain number of ms passing, by the use of timer function.

//  Created by gb on 10/21/12.
//  Copyright (c) 2012 Barmpalias. All rights reserved.

//LIBRARIES
#include <GLUT/glut.h>
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
int nure, nubl, ix, iy, wrongred, wrongblue;
//time variables
std::chrono::time_point<std::chrono::system_clock> fstart, fend, finnumbhum, finbiasArray, looptimen0, looptime1;
std::chrono::time_point<std::chrono::system_clock> swaptime0, swaptime1, swaptime2;
long long looptimedif, swaptimedif1, swaptimedif2, swaptimedif3, swaptimedif4;
int freezevar=0, abortvar;

//CONSTANTS AND VARIABLES
int numbhum, segvar, i, j, k, w, x, y, testmode, roundvar, roundrep, iseg, swapmax, standpar, roundret=0;
float tau, tau0, tau1;
int  biasvar, biasvarX, biasvarY, rednu, bluenu, utilr, utilg, negsize;
int seedvar, exhausttest, swaphappened, haltvar, exhaustswap;
int targvarnAr, targvarnAb, varBiasAr, varBiasAb, artargvarnAr0, artargvarnAr1, artargvarnAb0, artargvarnAb1;
int biasRtargOld, biasBtargOld;//transitional memory bias variables for updating unhappy lists
int swapx0, swapx1, swapy0, swapy1, biasvarR0, biasvarR1, biasvarB0, biasvarB1;
int neipar =0, ineg, jneg, biasRtargNew, biasBtargNew;
//TUPLE VARIABLES
//tuple <int, int> swapx, swapy;
int ijnegvecsize;
//VECTOR ARRAYS
vector<vector<int> > numbhumArray;
vector<vector<int> >  biasArray;
std::vector<tuple <int, int> > unhapRed;
std::vector<tuple <int, int> > unhapBlue;
std::vector<tuple <int, int> > unhapRedspec;
std::vector<tuple <int, int> > unhapBluespec;
std::vector<tuple <int, int> > ijnegvec;
int widtha, heighta;
float zoomvar = 1.0, translvarx = -0.9f, translvary = -0.9f;
bool fullScreen = false, flagend=false, flagstart=false;

//REPORT ROUTINES (DECLARATION)
void detailedreport(void);
void    countnumbers(void);

////GRAPHICS ROUTINES (DECLARATION)
void display();

void reshape(int width, int height);
void init();
void reportnumbswapscur();
void keyb(unsigned char key, int x, int y);
void keybspec(unsigned char ket, int x, int y);
static void Timer(int value);

//MAINROUTINES (DECLARATION)
void doswapfollowconv(void);
int loopstopcondition(void);
void outputfinalnumbers (void);
void timeendroutine(void);
void updatebiaslists(void);
void loopmainfeed(void);

////GRAPHICS ROUTINES
void reshape(int width, int height)
{   widtha=width;
    heighta=height;
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translvarx, translvary, 0.0f);
	gluLookAt(0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f,
              0, 1, 0); }

int utilitycount(void){
    int i, j;
    utilr=0;
    utilg=0;
    for (i=0; i<numbhum; i++){
        for (j=0; j<numbhum; j++){
            if (numbhumArray[i][j]==1){utilr=utilr+(((2*w+1)+biasArray[i][j])/2);}
            else {utilg=utilg+(((2*w+1)-biasArray[i][j])/2);}
        }
    }
    return utilr+utilg;
}


void reportnumbswapscur(void){
    
    if (roundvar % roundrep ==0) {
        looptime1 = std::chrono::system_clock::now();
        looptimedif = std::chrono::duration_cast<std::chrono::milliseconds> (looptime1-looptimen0).count();
        std::cout << "\n " << roundvar +1 << " swaps completed and going ...\n\n";
        std::cout << "      Utility: "<<utilitycount()<<"\n";
        std::cout << "      Unhappy: "<< unhapRed.size() << " red and "<< unhapBlue.size()<<" blue\n\n";
        looptimen0 = looptime1;
        swaptimedif1 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime1-swaptime0).count();
        swaptimedif2 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime2-swaptime1).count();
        //spaptimes: 0 when swap begins, 1 when the choice is done and earased from unhaplists
        //2 when the bias is updated, 3 when unhap lists updated, 4 when unhappy lists are normalized
        
        std::cout << "      Time report on swap "<<roundvar +1<<":\n\n";
        std::cout <<"           - Last "<< roundrep << " swaps in "<< looptimedif << " ms\n";
        std::cout <<"           - "<< swaptimedif1<<" mus for swap and erase from unhappy lists\n";
        std::cout <<"           - "<< swaptimedif2<<" mus for updating the bias array and unhappy lists\n\n\n";
        
       // if (testmode==1){detailedreport();}
    }}

void detailedreport(void){//FOR TESTING
    int i, k;
    std::cout << "\n"; //output the human/bias array
    std::cout << " The human/bias array is:\n";
    std::cout << "\n";
    for (i=0; i< numbhum; i++){
        std::cout<< "\n";
        for (k=0; k<numbhum; k++){
            if (biasArray[i][k]<0 && numbhumArray[i][k]==1) {std::cout << " "<< biasArray[i][k]<< "x";}
            else if (biasArray[i][k]<0 && numbhumArray[i][k]==-1) {std::cout << " "<< biasArray[i][k]<< "o";}
            else if (biasArray[i][k]>=0 && numbhumArray[i][k]==1){std::cout << " "; std::cout << " "<< biasArray[i][k]<< "x";}
            else if (biasArray[i][k]>=0 && numbhumArray[i][k]==-1){std::cout << " "; std::cout << " "<< biasArray[i][k]<< "o";}
        }}
    std::cout << "\n";
    
    std::cout << "\n The unhappy red are:\n";
    for (i=0; i < unhapRed.size(); i++){
        if ( i % 10 ==0){std::cout << "\n";}
        std::cout << " ("<<std::get<0>(unhapRed[i])<<","<<std::get<1>(unhapRed[i])<<")"<<" ";}
    std::cout << "\n";
    std::cout << "\n";
    std::cout << " The unhappy Blue are:\n";
    for (i=0; i < unhapBlue.size(); i++){
        if (i % 10 ==0){std::cout << "\n";}
        std::cout << " ("<<std::get<0>(unhapBlue[i])<<","<<std::get<1>(unhapBlue[i])<<")"<<" ";}
    std::cout << "\n\n";
    
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glColor3f(1.0f, 0.0f, 0.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}


////MY FUNCTIONS



int signfu(int argt, int argn){//this is for torus version
    //outputs the canonical index of a human or -1
    //given one outside [0,numhum] (range in [-numhum, 2numhum]): argn is numbhum
    if (argt<0) {return argn + argt;}
    else if (argt>= argn){return argt-argn;}
    else {return argt;} }

int signfs(int argt){//this is the simple version of signfu
    //outputs the canonical index of a human or -1
    //given one outside [0,numhum] (range in [-numhum, 2numhum]): argn is numbhum
    if (argt<0) {return numbhum + argt;}
    else if (argt>= numbhum){return argt-numbhum;}
    else {return argt;} }


int biasArMod(int argp, int argq){//returns the element in numbhumArray if
    //arguments are correct and 0 otherwise
    if (signfs(argp)>=0 && signfs(argq)>=0) {
        return numbhumArray[signfs(argp)][signfs(argq)]; }
    else {return 0;} }

int signpos(int argt){//returns the input if >=0 and 0 otherwise
    if (argt<0) {return 0;}
    else {return argt;} }

//TIME ROUTINES
void timestart(void){
    fstart = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(fstart);
    std::cout << "\n Computation started at " << std::ctime(&start_time);}

void timeendroutine(void){
    fend = std::chrono::system_clock::now();
    long long elapsed_minutes = std::chrono::duration_cast<std::chrono::minutes> (fend-fstart).count();
    long long elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds> (fend-fstart).count();
    long long elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds> (fend-fstart).count();
    std::time_t end_time = std::chrono::system_clock::to_time_t(fend);
    std::cout << " Total elapsed time: ";
    if (!(elapsed_minutes ==0)){std::cout << elapsed_minutes<< "min ";}
    if (!(elapsed_seconds ==0)){std::cout << elapsed_seconds % 60 << "s ";}
    std::cout << elapsed_microseconds % 1000 << "mus\n";
    std::cout << " Finished overall computation at " << std::ctime(&end_time)<<"\n";}


// MAIN SUBROUTINES
void initialcall(void){
    std::cout << " Welcome to Schelling 2D Animation version 1.1\n"<< " A program by Barmpalias, Elwes and Lewis-Pye.\n\n";
    std::cout << " Yes/No answers are given by 1/0\n";
    std::cout << " Time is measured in seconds (s), milliseconds (ms) or microseconds (mus)\n";
    std::cout << " Default values are: convention on;\n"<<
    "                     torus;\n" <<
    //"                     max swaps n*1000;\n" <<
    "                     report every 5*n;\n" ;//<<
    //"                     no test mode;\n\n";
    std::cout << " Graphical output controls:\n\n";//Initial Input Calls
    std::cout << "      - Press g to start\n";
    std::cout << "      - Press q to quit\n";
    std::cout << "      - Press f for fullscreen\n";
    std::cout << "      - Press p,l for enlarging or shrinking the square\n";
    std::cout << "      - Press the keyboard arrows for moving the square around\n\n";
    
    std::cout << " User input:\n\n";/*Initial Input Calls*/
    std::cout << "      - Give me the number of humans on one edge of the square: "; std::cin >> numbhum;
    std::cout << "      - Give me the neighbourhood radius w: ";
    std::cin >> w; negsize=(2*w+1)*(2*w+1);
    std::cout << "      - Give me the tolerance parameter tau: ";
    std::cin >> tau; tau1=negsize * (1-2*tau); tau0=negsize * (2*tau-1);

    swapmax=20*numbhum*numbhum; roundrep=100*numbhum; testmode=0;
    seedvar=(unsigned int)time (NULL);
    srand (seedvar);//random seed
    std::cout<< "\n  Seed is: "<<seedvar<<"\n";
}

void buildhumans(void){
    int i, j;
    for (i=0; i< numbhum; i++){//Build human array and anounce
        for (j=0; j< numbhum; j++){
            if (rand () % 2 == 0 ) {numbhumArray[i][j]= -1; bluenu++;}
            else {numbhumArray[i][j]= 1;rednu++;} }}
    finnumbhum = std::chrono::system_clock::now();
    long long finnumbhms = std::chrono::duration_cast<std::chrono::milliseconds> (finnumbhum-fstart).count();
    std::cout << "\n"<<" Produced randomly coloured humans: "<<rednu<< " red and "<<bluenu<<" blue"<<
    " (in "<<finnumbhms<< " ms)\n"; }

void buildinitbiasarr(void){
    int i, j;
    biasvar = 0;    //calculate the bias of numbhumArray [0][0]
    for (i=0; i < 2*w + 1; i++){
        for (j=0; j<2*w+1; j++){biasvar = biasvar + biasArMod(i-w, j-w);}}
    biasArray[0][0] = biasvar;
    if ((numbhumArray[0][0]==1) && (biasArray[0][0] < ( tau0)))
    {unhapRed.push_back (make_tuple(0, 0));}
    else if ((numbhumArray[0][0]==-1) && ((biasArray[0][0] > tau1)))
    {unhapBlue.push_back (make_tuple(0, 0));}
    //calculate the bias of column numbhumArray [][0]
    for (i=0; i < numbhum-1; i++){
        biasvarX = 0; biasvarY = 0;
        for (j=0; j<2*w+1; j++){biasvarX = biasvarX + biasArMod(i+1+w, j-w);}
        for (j=0; j<2*w+1; j++){biasvarY = biasvarY + biasArMod(i-w, j-w);}
        biasArray[i+1][0] = biasArray[i][0]-biasvarY + biasvarX;
        
        if ((numbhumArray[i+1][0]==1) && (biasArray[i+1][0]< tau0))
        {unhapRed.push_back (make_tuple(i+1, 0));}
        else if ((numbhumArray[i+1][0]==-1) && (biasArray[i+1][0]> tau1))
        {unhapBlue.push_back (make_tuple(i+1, 0));}}
    for (i=0; i < numbhum; i++){//calculate recursively the rest using the first column
        for (k=0; k < numbhum-1; k++){
            biasvarX = 0; biasvarY = 0;
            for (j=0; j<2*w+1; j++){biasvarX = biasvarX + biasArMod(i-w+j, k+1+w);}
            for (j=0; j<2*w+1; j++){biasvarY = biasvarY + biasArMod(i-w+j, k-w);}
            biasArray[i][k+1] = biasArray[i][k]-biasvarY + biasvarX;
            
            if ((numbhumArray[i][k+1]==1) && (biasArray[i][k+1]< tau0))
            {unhapRed.push_back (make_tuple(i, k+1));}
            else if ((numbhumArray[i][k+1]==-1) && (biasArray[i][k+1]> tau1))
            {unhapBlue.push_back (make_tuple(i, k+1));}}}}

int swaptest(void){
    if (biasArray[swapx0][swapx1] > biasArray[swapy0][swapy1] + 2){return 0;}
    else if (
             ( ( abs(swapx0-swapy0)<=w) && (abs(swapx1-swapy1)<=w)  ) &&
             (biasArray[swapx0][swapx1] >= biasArray[swapy0][swapy1] + 2)
             ){return 0;}
    else {return 1;}
}

void exhaustrout(void){
    int i, j;
    exhausttest=1; exhaustswap=0;
    unhapRedspec.resize(0);
    unhapBluespec.resize(0);
    std::cout<< " STARTING EXAUSTIVE SEARCH...";
    for (i=0; i< unhapRed.size(); i++){
        for (j=0; j< unhapBlue.size(); j++){
//            swapx = unhapRed[i];// Record last swap
//            swapy = unhapBlue[j];
            swapx0=std::get<0>(unhapRed[i]);
            swapx1=std::get<1>(unhapRed[i]);
            swapy0=std::get<0>(unhapBlue[j]);
            swapy1=std::get<1>(unhapBlue[j]);
            if (swaptest()==1){exhaustswap++;
                unhapRedspec.push_back (make_tuple(swapx0, swapx1));
                unhapBluespec.push_back (make_tuple(swapy0, swapy1));
            }
        }}
    if (exhaustswap==0){haltvar=1; std::cout<< "...NO LEGIT SWAPS REMAINING\n";}
    else {haltvar=0; std::cout<< "FOUND "<< exhaustswap<<" POSSIBILITIES\n";
        x= rand () % unhapRedspec.size();//pick the unhappy pair
//        swapx = unhapRedspec[x];// Record last swap
//        swapy = unhapBluespec[x];
        swapx0=std::get<0>(unhapRedspec[x]);
        swapx1=std::get<1>(unhapRedspec[x]);
        swapy0=std::get<0>(unhapBluespec[x]);
        swapy1=std::get<1>(unhapBluespec[x]);
    }
}


void doswapfollowconv(void){
    swaphappened=0;
    if (roundvar % roundrep ==0) {swaptime0 = std::chrono::system_clock::now();}
    x= rand () % unhapRed.size();//pick the unhappy to swap
    y= rand () % unhapBlue.size();
//    swapx = unhapRed[x];// Record last swap
//    swapy = unhapBlue[y];
    
    swapx0=std::get<0>(unhapRed[x]);
    swapx1=std::get<1>(unhapRed[x]);
    swapy0=std::get<0>(unhapBlue[y]);
    swapy1=std::get<1>(unhapBlue[y]);
    roundret=0; exhausttest=0;
    while (roundret< numbhum*numbhum && tau>0.5 && (swaptest()==0) )
    {//keep the convension for >0.5 - re-choose the swap targets
        x= rand () % unhapRed.size();//pick the unhappy to swap
        y= rand () % unhapBlue.size();
//        swapx = unhapRed[x];// Record last swap
//        swapy = unhapBlue[y];
        // if (roundvar % roundrep ==0){cout<< " Trying swap: "<< roundvar+1; //
        //     //<< ". I re-chose to swap ("<< swapx0 << ", " << swapx1 << ") with ("<<  swapy0 << ", " << swapy1 << ")\n";
        //}
        swapx0=std::get<0>(unhapRed[x]);//need to do this here to refer to the coordinates in the while condition
        swapx1=std::get<1>(unhapRed[x]);
        swapy0=std::get<0>(unhapBlue[y]);
        swapy1=std::get<1>(unhapBlue[y]);
        roundret++;
    }
    if (roundret < numbhum*numbhum) {
        numbhumArray[swapx0][swapx1] = -1;// Do the swap;
        numbhumArray[swapy0][swapy1] = 1;
        //if (testmode==1){cout<< " I swap ("<< swapx0 << ", " << swapx1 << ") with ("<<  swapy0 << ", " << swapy1 << ")\n";}
        unhapRed.erase (unhapRed.begin()+x);//Erase member with index x from unhapRed
        unhapBlue.erase (unhapBlue.begin()+y);//Erase member with index y from unhapBlue
        swaphappened=1;
    }
    else if (tau>0.5){
        exhaustrout();
        if(exhaustswap>0){
        numbhumArray[swapx0][swapx1] = -1;// Do the swap;
        numbhumArray[swapy0][swapy1] = 1;
        unhapRed.erase (unhapRed.begin()+x);//Erase member with index x from unhapRed
        unhapBlue.erase (unhapBlue.begin()+y);//Erase member with index y from unhapBlue
            swaphappened=1;}
    }
}

void updatebiaslists(void){//update bias and unhappy arrays
    int i;
    if (roundvar % roundrep ==0) {swaptime1 = std::chrono::system_clock::now();}
    
    for (i=0; i < ijnegvecsize; i++){//Update the bias around humans unhapRed[x], unhapBlue[y]
        biasvarR0=signfs(swapx0+std::get<0>(ijnegvec[i]));//set biasvarR tuple to that coordinate for swapx
        biasvarR1=signfs(swapx1+std::get<1>(ijnegvec[i]));
        biasRtargOld=biasArray[biasvarR0][biasvarR1]; biasRtargNew =biasRtargOld; //save the old value for use below

        if ( ( ( (abs(biasvarR0-swapy0)>w) && ( abs(biasvarR0-swapy0) < numbhum - w ) )//if 0-far from swapy
                      || ((abs(biasvarR1-swapy1)>w) && (abs(biasvarR1-swapy1) < numbhum - w ))) )//or if 1-far from swapy
                {//biasRtargNew = biasArray[biasvarR0][biasvarR1]-2;
                    biasArray[biasvarR0][biasvarR1]=biasRtargOld -2;//update bias
                    //now update unhappy lists for this point  (biasvarR0,biasvarR1)
                    if ((numbhumArray[biasvarR0][biasvarR1]==1)//if red
                        && (biasRtargOld -2 < tau0)//and unhappy
                        &&//and was happy before, i.e. not currently in the list
                        (biasRtargOld >= tau0) ){
                        unhapRed.push_back (make_tuple(biasvarR0, biasvarR1));}//then put it in
                    else if ((numbhumArray[biasvarR0][biasvarR1]==-1)//if blue
                             &&//and was unhappy before, i.e. currently in the list
                             (biasRtargOld > tau1) &&// and its not swapx
                             (!(biasvarR0==swapx0) || !(biasvarR1==swapx1))
                             &&//but is happy now
                             (biasRtargOld -2 <= tau1))//then take it out of the list
                    {unhapBlue.erase(std::remove(unhapBlue.begin(), unhapBlue.end(),
                                                 make_tuple(biasvarR0, biasvarR1)), unhapBlue.end());}//END of 3rd if
                }//END OF second IF
            //}//END OF first IF, end of make the change in bias and unhaplist part R
  //torus//if (signfu(swapy0-w+i, numbhum)>=0 && signfu(swapy1-w+j, numbhum)>=0){//if coordinate legitimate for swapy
        biasvarB0=signfs(swapy0+std::get<0>(ijnegvec[i]));//set biasvarB tuple to that coordinate for swapy
        biasvarB1=signfs(swapy1+std::get<1>(ijnegvec[i]));
        biasBtargOld=biasArray[biasvarB0][biasvarB1]; biasBtargNew=biasBtargOld;//save old value 4use below

        if ( ( ( (abs(biasvarB0-swapx0)>w) && (abs(biasvarB0-swapx0) < numbhum - w))//if 0-far from swapx
                      || ((abs(biasvarB1-swapx1)>w) && (abs(biasvarB1-swapx1) < numbhum - w))))//or if 1-far from swapx
                {biasBtargNew = biasBtargOld+2; biasArray[biasvarB0][biasvarB1] = biasBtargNew;
                    
                    //now update unhappy lists for this point  (biasvarB0,biasvarB1)
                    if ((numbhumArray[biasvarB0][biasvarB1]==1)//if red
                        && (biasBtargOld +2 >= tau0 )
                        //and happy (equiv. was not happy before by more than 2 margin)
                        &&//and was unhappy before and is not swapy, i.e. not currently in the list
                        (biasBtargOld < tau0) &&
                        ( !(biasvarB0== swapy0) || !(biasvarB1== swapy1))){
                        unhapRed.erase(std::remove(unhapRed.begin(), unhapRed.end(),
                                                   make_tuple(biasvarB0, biasvarB1)), unhapRed.end());}
                    else if ((numbhumArray[biasvarB0][biasvarB1]==-1)//if blue
                             &&//and was happy before
                             (biasBtargOld <= tau1)
                             &&//but unhappy now
                             (biasBtargOld +2 > tau1))//then put it in the list
                    {unhapBlue.push_back (make_tuple(biasvarB0, biasvarB1));}//then remove it from Red unhappy list
                }//END OF INNER IF FOR biasvarB
            //}//END OF outer IF, end of make the change in bias and unhaplist
        }   //end of the two for
    if ( //if it is swap x and unhappy then put it in
        biasArray[swapx0][swapx1] > tau1 )
    {unhapBlue.push_back (make_tuple(swapx0, swapx1));}
    if ( //if it is swap y and unhappy then put it in
        biasArray[swapy0][swapy1] < tau0 )
    {unhapRed.push_back (make_tuple(swapy0, swapy1));}
    if (roundvar % roundrep ==0) {swaptime2 = std::chrono::system_clock::now();}}//end for routine

int loopstopcondition(void){
    if (unhapRed.empty()) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " There are no more unhappy red humans (reason for stopping swapping).\n"; return 0;}
    else if (unhapBlue.empty()) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " There are no more unhappy blue humans (reason for stopping swapping).\n"; return 0;}
    else if (haltvar==1) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " No legitimate swaps remaining, after exhaustive search.\n"; return 0;}
    else {return 1;} }

///REPORT SUBROUTINES (PART OF MAIN PROGRAM)

void reportunhapnumb(void){
    finbiasArray = std::chrono::system_clock::now();
    long long finbiasArrayms = std::chrono::duration_cast<std::chrono::milliseconds> (finbiasArray-fstart).count();
    
    std::cout << " Compiled initial bias and unhappy arrays (in " << finbiasArrayms << " ms)\n";
    std::cout << " We have "<< unhapRed.size() << " unhappy red and " << unhapBlue.size() << " unhappy blue\n";
}

void outputfinalnumbers (void){//Output number of unhappy and runs
    std::cout << "\n The number of the finally unhappy is "
    << unhapRed.size() << " red and "<< unhapBlue.size() << " blue\n";}

void loopmainfeed(void){
    if (loopstopcondition()==1){
        doswapfollowconv();
        if (swaphappened==1){updatebiaslists(); reportnumbswapscur(); roundvar++;}
           }// End of Swapping loop
    else {if(!flagend){outputfinalnumbers(); timeendroutine(); flagend=true;}} }


void keyb(unsigned char key, int x, int y){
    switch (key){
        case 'f': fullScreen = !fullScreen; if (fullScreen) {glutFullScreen();}
        else {glutPositionWindow(0,0); glutReshapeWindow(900, 900);} break;
        case 'q':  exit (0);  break;
        case 'p': zoomvar += 0.2; break;
        case 'l': zoomvar-= 0.1; break;
        case 'g': flagstart=!flagstart; break;}
    glutPostRedisplay();
    //    reshape(widtha, heighta);
}


void keybspec(int ket, int x, int y){
    switch (ket){
        case GLUT_KEY_RIGHT: if (translvarx <0.9) {translvarx += 0.05;} break;
        case GLUT_KEY_LEFT: if (translvarx > -0.99) {translvarx -= 0.05;}  break;
        case GLUT_KEY_UP: if (translvary <0.9) {translvary += 0.05;}    break;
        case GLUT_KEY_DOWN: if (translvary > -0.99) {translvary -= 0.05;}  break;}
    glutPostRedisplay();
    reshape(widtha, heighta);}

void display() {
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(zoomvar,zoomvar);
    glRasterPos2i(-21, -20);//bottomDown
    glDrawPixels(numbhum, numbhum, GL_RGB, GL_UNSIGNED_BYTE, image);
    glFlush();
    
    image = (unsigned char*) malloc(numbhum * numbhum * 4);
    p = image;
    for (i = 0; i < numbhum; ++i){
        for (j = 0; j < numbhum; ++j) {
            *p++ = (numbhumArray[i][j]==1)? 255: 0;
            *p++ = (numbhumArray[i][j]==-1)? 255: 0;
            *p++ = 0;}}
    glutPostRedisplay();
    while(flagstart && !flagend){
        loopmainfeed();
        if (roundvar % 2000 ==0){break;}
}
}

void countnumbers(){
    nure=0;
    nubl=0;
    int ix, iy;
    for(ix=0; ix< numbhum; ix++){
        for (iy=0; iy< numbhum; iy++) {if (numbhumArray[ix][iy]==1){nure++;}
            else if (numbhumArray[ix][iy]==-1){nubl++;}}}
    cout<< "\n Number of red: "<<nure<<"\n";
    cout<< "\n Number of blue: "<<nubl<<"\n";
    
    for (ix=0; ix< unhapRed.size(); ix++){
        if (numbhumArray[get<0>(unhapRed[ix])][get<1>(unhapRed[ix])]==-1){wrongred++;}}
    for (iy=0; iy< unhapBlue.size(); iy++){
        if (numbhumArray[get<0>(unhapBlue[iy])][get<1>(unhapBlue[iy])]==1){wrongblue++;}}
    cout<< "\n Number of red in unhapBlue: "<<wrongblue<<"\n";
    cout<< "\n Number of blue in unhapRed: "<<wrongred<<"\n\n";
}


void drawtheoutput(int argc, char **argv){
    glutInitWindowPosition(0,0);
    glutInitWindowSize(900,900);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Schelling 2D animation");
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutDisplayFunc(display);
    init();
    glutKeyboardFunc(keyb);
    glutSpecialFunc(keybspec);
    glutMainLoop();}

//MAIN PROGRAM
int main(int argc, char **argv) {
    int i, j;
    initialcall();//ask for imput to the program
    timestart();
    numbhumArray.resize (numbhum, vector<int> ( numbhum ));         //resize arrays
    biasArray.resize (numbhum, vector<int> ( numbhum ));
    buildhumans();                                              //build humans and announce
    buildinitbiasarr();//build initial bias array
    
    /* Make legit coordinate arrays*/
    for (i=0; i < 2*w + 1; i++){
        for (j=0; j < 2*w + 1; j++){ijnegvec.push_back (make_tuple(i-w, j-w));}}
    ijnegvecsize= (int)ijnegvec.size();

    
    reportunhapnumb();
//    std::cout<< " Should I proceed? (0/1): ";
//    std::cin >> abortvar;
//    if(abortvar==1){
        looptimen0 = std::chrono::system_clock::now();
        std::cout << "\n"<< " Started swapping ...\n\n";
        image = (unsigned char*) malloc(numbhum * numbhum * 4);
        p = image;
        for (i = 0; i < numbhum; ++i){
            for (j = 0; j < numbhum; ++j) {
                *p++ = (numbhumArray[i][j]==1)? 255: 0;
                *p++ = (numbhumArray[i][j]==-1)? 255: 0;
                *p++ = 0;}}
        drawtheoutput(argc, argv);//}
//    else {std::cout<< " The program has terminated.";}
    return 0; }
