//  main.cpp
//  Schell
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
GLfloat gAngle = 0.0;

using namespace std;
unsigned char* image;
unsigned char* p;
int nure, nubl, ix, iy, wrongred, wrongblue, ig;
//time variables
std::chrono::time_point<std::chrono::system_clock> start, fend, finnumbhum, finbiasArray, looptimen0, looptime1;
std::chrono::time_point<std::chrono::system_clock> swaptime0, swaptime1, swaptime2;
long long looptimedif, swaptimedif1, swaptimedif2, swaptimedif3, swaptimedif4;

//CONSTANTS AND VARIABLES
int numbhum, segvar, i, j, k, w, x, y, testmode, roundvar, roundrep, iseg, swapmax, standpar, roundret;
float tau;
int  biasvar, biasvarX, biasvarY, rednu, bluenu, torvar;
int targvarnAr, targvarnAb, varBiasAr, varBiasAb, artargvarnAr0, artargvarnAr1, artargvarnAb0, artargvarnAb1;
int biasRtargOld, biasBtargOld;//transitional memory bias variables for updating unhappy lists
int swapx0, swapx1, swapy0, swapy1, biasvarR0, biasvarR1, biasvarB0, biasvarB1;
int neipar =0, ineg, jneg, biasRtargNew, biasBtargNew, convar;
//TUPLE VARIABLES
tuple <int, int> swapx, swapy;
//VECTOR ARRAYS
vector<vector<int> > numbhumArray;
vector<vector<int> > numbhumArray0;
vector<vector<int> >  biasArray;
std::vector<tuple <int, int> > unhapRed;
std::vector<tuple <int, int> > unhapBlue;
std::vector<tuple <int, tuple <int, tuple <int, int>>> > changedhum;
int widtha, heighta;
float xg, yg, zg;
//static int wb[2] = {0, 255};//color array
float zoomvar = 1.0f, zoomvaro = 0.0f, zoomvary = 0.0f, zoomvarz = 0.0f, translvarx = -0.0f, translvary = -0.0f, translvarz=-0.0f,
rotvarx=0.0, rotvary=0.0, rotvarz=0.0,
translvarx0 = -0.0f, translvary0 = -0.0f, translvarz0=-0.0f,
translvarx1 = -0.0f, translvary1 = -0.0f, translvarz1=-0.0f,
pervarz0=0.0, pervarz1=0.0;
bool fullScreen = false;

//REPORT ROUTINES
void detailedreport(void);
void countnumbers(void);
////GRAPHICS ROUTINES
void display();
void reshape(int width, int height);
void init();
void keyb(unsigned char key, int x, int y);
void keybspec(unsigned char ket, int x, int y);

void coroutr(void){
    glColor3ub((GLubyte)255,(GLubyte)80,(GLubyte)60);/*red*/
    //glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)0);/*black*/
    
}
void coroutb(void){
    //      glColor3ub((GLubyte)80,(GLubyte)80,(GLubyte)255);/*blue*/
    //glColor3ub((GLubyte)135,(GLubyte)135,(GLubyte)135);/*grey*/
    glColor3ub((GLubyte)173,(GLubyte)255,(GLubyte)173);/*green*/
}


void reshape(int width, int height)
{    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
    widtha=width;
    heighta=height;
    glMatrixMode(GL_PROJECTION);
    
	glLoadIdentity();
    glScalef(zoomvar, zoomvar, zoomvar);
    //    glViewport(0, 0, width, height);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //    gluLookAt(0.0f, 0.0f, -0.2,
    //              0.0f, 0.0f, 0.0f,
    //              0, 10, 0);
    
    //    glOrtho(-100, 100, -100*aspectRatio, 100*aspectRatio,  -90, 90);
    gluPerspective(-200+zoomvaro,aspectRatio, 1+zoomvary, 50+zoomvarz);
    glTranslatef(-20, -20, 0);
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

void keyb(unsigned char key, int x, int y){
    switch (key){
        case 'f': fullScreen = !fullScreen;
            if (fullScreen) {glutFullScreen(); }
            else {glutPositionWindow(0,0); glutReshapeWindow(900, 900);} break;
        case 'q':  exit (0);  break;         // quit
        case 'p': zoomvar -= 0.02;      break;
        case 'P': zoomvar += 0.02;       break;
        case 'k': translvarz1 += 1;      break;
        case 'K': translvarz1-= 1;       break;
        case 'j': translvarx1 += 1;      break;
        case 'J': translvarx1-= 1;       break;
        case 'h': zoomvaro += 1;      break;
        case 'H': zoomvaro-= 1;       break;
        case 'n': zoomvary += 1;      break;
        case 'N': zoomvary-= 1;       break;
        case 'm': zoomvarz += 1;      break;
        case 'M': zoomvarz-= 1;       break;
            
            
        case 'u': translvarx0 += 1;     break;
        case 'U': translvarx0-= 1;      break;
        case 'i': translvary0 += 1;     break;
        case 'I': translvary0-= 1;      break;
        case 'o': translvarz0 += 1;      break;
        case 'O': translvarz0-= 1;       break;
            
        case 'l': gAngle += 10.;      break;
        case 'L': gAngle-= 10.;       break;
    }
    glutPostRedisplay();
    reshape(widtha, heighta);
}


void keybspec(int ket, int x, int y){
    switch (ket)
    {   case GLUT_KEY_RIGHT: /*if (translvarx <0.9) {*/translvarx += 1.;/*}*/ break;
        case GLUT_KEY_LEFT: /*if (translvarx > -0.99) {*/translvarx -= 1.;/*}*/  break;
        case GLUT_KEY_UP: /*if (translvary <0.9) {*/translvary += 1.;/*}*/    break;
        case GLUT_KEY_DOWN: /*if (translvary > -0.99) {*/translvary -= 1.;/*}*/  break;
    }
    glutPostRedisplay();
    reshape(widtha, heighta);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    //    glRasterPos3i(-0, 0,1);//bottomDown
    //glPixelZoom(3.0f, 30.0f);
    //glColor3f(10.0f,10.0f,-10.0f);	// Color Blue
    glPointSize(2.0);
    //glutWireSphere(1.5,10,20);//draw a wireframe sphere.
    //    glutWireCube(1.0f);
    //    glutSolidCube (1.6);
    //    glutSolidSphere (1.6, 100, 100);
    glPushMatrix();
    glTranslatef(translvarx,translvary,-10);
    glRotatef(gAngle,1.+translvarx0,1.+translvary0,1.+pervarz0);
    //glutWireCube(20.); //cube of size 20x20x20
    
    
    
    glBegin(GL_POINTS);
    for (i=0; i<numbhum; i++){
        for (j=0; j<numbhum; j++){
            if (numbhumArray0[i][j]==1){coroutr();} else {coroutb();}
            glVertex3f(((40-translvarx1)*(float)i)/(float)numbhum, ((40-translvarx1)*(float)j)/(float)numbhum, 0);
            if (numbhumArray[i][j]==1){coroutr();} else {coroutb();}
            glVertex3f(((40-translvarx1)*(float)i)/(float)numbhum,
                       ((40-translvarx1)*(float)j)/(float)numbhum, ((-0.9+translvarz1)*(float)roundvar)/(float)numbhum);
        }}
    glEnd();
    
    for (ig=0; ig< changedhum.size(); ig++){
        if(get<0>(get<1>(changedhum[ig]))==1){coroutr();} else {coroutb();}
        xg = ((40-translvarx1)*(float)(get<0>(get<1>(get<1>(changedhum[ig])))))/(float)numbhum;
        yg = ((40-translvarx1)*(float)(get<1>(get<1>(get<1>(changedhum[ig])))))/(float)numbhum;
        zg= ((-0.9+translvarz1)*(float)(get<0>(changedhum[ig])))/(float)numbhum;
        glBegin(GL_POINTS);
        glVertex3f(xg, yg, zg);
        glEnd();}
    glPopMatrix();
    glFlush();
}


void drawtheoutput(int argc, char **argv){
    
    glutInitWindowPosition(0,0);
    glutInitWindowSize(900,900);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    
    glutCreateWindow("Schelling 2D");
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glutDisplayFunc(display);
    init();
    
    glutKeyboardFunc(keyb);
    glutSpecialFunc(keybspec);
    glutMainLoop();
}

////MY FUNCTIONS
//
int signfu(int argt, int argn){//this is for torus version
    //outputs the canonical index of a human or -1, depending on value of torvar,
    //given one outside [0,numhum] (range in [-numhum, 2numhum]): argn is numbhum
    if (argt<0 && torvar==1) {return argn + argt;}
    else if (argt>= argn && torvar==1){return argt-argn;}
    else if (argt<0 && torvar==0){return -1;}
    else if (argt>= argn && torvar==0){return -1;}
    else {return argt;}
}

int neighsize(int inei, int jnei){//gives the size of the neigbourhood of a point, given torus or not
    neipar=0;
    if (torvar == 1){return (2*w+1)*(2*w+1);}
    else {for (ineg=0; ineg< 2*w+1; ineg++){
        for (jneg=0; jneg< 2*w+1; jneg++){
            if ( (ineg-w + inei >=0) && (jneg-w + jnei >=0) &&
                (ineg-w + inei < numbhum) && (jneg-w + jnei <numbhum) ){neipar++;} }} return neipar;}
}


int biasArMod(int argp, int argq){//returns the element in numbhumArray if
    //arguments are correct (depending on torvar) and 0 otherwise
    if (signfu(argp, numbhum)>=0 && signfu(argq, numbhum)>=0) {
        return numbhumArray[signfu(argp, numbhum)][signfu(argq, numbhum)];
    }
    else {return 0;}
}


int signpos(int argt){//returns the input if >=0 and 0 otherwise
    if (argt<0) {return 0;}
    else {return argt;}
}

//time routines

void timestart(void){
    start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << "\n Computation started at " << std::ctime(&start_time);
}

void timeendroutine(void){
    fend = std::chrono::system_clock::now();
    long long elapsed_minutes = std::chrono::duration_cast<std::chrono::minutes> (fend-start).count();
    long long elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds> (fend-start).count();
    long long elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> (fend-start).count();
    std::time_t end_time = std::chrono::system_clock::to_time_t(fend);
    std::cout << " Total elapsed time: ";
    if (!(elapsed_minutes ==0)){std::cout << elapsed_minutes<< "min ";}
    if (!(elapsed_seconds ==0)){std::cout << elapsed_seconds % 60 << "s ";}
    std::cout << elapsed_milliseconds % 1000 << "ms\n";
    std::cout << " Finished overall computation at " << std::ctime(&end_time)<<"\n";}


// main SUBROUTINES

void initialcall(void){
    std::cout << " Welcome to Schelling 2D, a program by Barmpalias, Elwes and Lewis-Pye.\n\n";//Initial Input Calls
    
    std::cout << " Yes/No answers are given by 1/0\n";
    std::cout << " Time is measured in seconds (s), milliseconds (ms) or microseconds (mus)\n";
    std::cout << " Default values are: convention on, no torus, max swaps n*500, report every 5*n, and no test mode\n\n";
    
    std::cout << " Graphical output controls:\n\n";//Initial Input Calls
    std::cout << "      - Press q for quitting\n";
    std::cout << "      - Press f for fullscreen\n";
    std::cout << "      - Press p,l for enlarging or shrinking the square\n";
    std::cout << "      - Press the keyboard arrows for moving the square around\n\n";
    
    numbhum=80; w=4.2; tau=0.4; standpar=1;
    //    std::cout << " User input:\n\n";
    //    std::cout << "      - Give me the number of humans on one edge of the square: ";//Initial Input Calls
    //    std::cin >> numbhum;
    //    std::cout << "      - Give me the neighbourhood radius w: ";
    //    std::cin >> w;
    //    std::cout << "      - Give me the tolerance parameter tau: ";
    //    std::cin >> tau;
    //    std::cout << "      - The rest default? ";
    //    std::cin >> standpar;
    
    if (standpar==0){
        if (tau>0.5) {std::cout << "      - Convention for tau>0.5? (0 or 1) ";
            std::cin >> convar;}
        std::cout << "      - Torus or not ? (answer with 1,0 respectively): ";
        std::cin >> torvar;
        std::cout << "      - Give me the max number of swaps allowed to do: ";
        std::cin >> swapmax;
        std::cout << "      - Every how many swaps should I report completion? ";
        std::cin >> roundrep;
        std::cout << "      - Test mode? (0 or 1) ";
        std::cin >> testmode;}
    else {convar=1; torvar=0, swapmax=500*numbhum; roundrep=5*numbhum; testmode=0;}
    
    srand ((unsigned int)time (NULL) );//random seed
}


void buildhumans(void){
    for (i=0; i< numbhum; i++){//Build human array and anounce
        for (j=0; j< numbhum; j++){
            if (rand () % 2 == 0 ) {numbhumArray[i][j]= -1; numbhumArray0[i][j]= -1; bluenu++;}
            else {numbhumArray[i][j]= 1; numbhumArray0[i][j]= 1; rednu++;}
        }}
    
    finnumbhum = std::chrono::system_clock::now();
    long long finnumbhms = std::chrono::duration_cast<std::chrono::milliseconds> (finnumbhum-start).count();
    
    std::cout << "\n"<<" Produced randomly coloured humans: "<<rednu<< " red and "<<bluenu<<" blue"<<
    " (in "<<finnumbhms<< " ms)\n";
}

void countnumbers(){
    nure=0;
    nubl=0;
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

void buildinitbiasarr(void){
    
    biasvar = 0;    //calculate the bias of numbhumArray [0][0]
    for (i=0; i < 2*w + 1; i++){
        for (j=0; j<2*w+1; j++){biasvar = biasvar + biasArMod(i-w, j-w);}
    }
    biasArray[0][0] = biasvar;
    
    if ((numbhumArray[0][0]==1) && (biasArray[0][0] < ( neighsize(0,0) * (2*tau -1))))
    {unhapRed.push_back (make_tuple(0, 0));}
    else if ((numbhumArray[0][0]==-1) && ((biasArray[0][0] > neighsize(0,0) * (1-2*tau))))
    {unhapBlue.push_back (make_tuple(0, 0));}
    
    
    //calculate the bias of column numbhumArray [][0]
    for (i=0; i < numbhum-1; i++){
        biasvarX = 0; biasvarY = 0;
        for (j=0; j<2*w+1; j++){biasvarX = biasvarX + biasArMod(i+1+w, j-w);}
        for (j=0; j<2*w+1; j++){biasvarY = biasvarY + biasArMod(i-w, j-w);}
        biasArray[i+1][0] = biasArray[i][0]-biasvarY + biasvarX;
        
        if ((numbhumArray[i+1][0]==1) && (biasArray[i+1][0]< neighsize(i+1,0) * (2*tau -1)))
        {unhapRed.push_back (make_tuple(i+1, 0));}
        else if ((numbhumArray[i+1][0]==-1) && (biasArray[i+1][0]> neighsize(i+1,0) * (1-2*tau)))
        {unhapBlue.push_back (make_tuple(i+1, 0));}
    }
    
    for (i=0; i < numbhum; i++){//calculate recursively the rest using the first column
        for (k=0; k < numbhum-1; k++){
            biasvarX = 0; biasvarY = 0;
            for (j=0; j<2*w+1; j++){biasvarX = biasvarX + biasArMod(i-w+j, k+1+w);}
            for (j=0; j<2*w+1; j++){biasvarY = biasvarY + biasArMod(i-w+j, k-w);}
            biasArray[i][k+1] = biasArray[i][k]-biasvarY + biasvarX;
            
            if ((numbhumArray[i][k+1]==1) && (biasArray[i][k+1]< neighsize(i, k+1) * (2*tau -1)))
            {unhapRed.push_back (make_tuple(i, k+1));}
            else if ((numbhumArray[i][k+1]==-1) && (biasArray[i][k+1]> neighsize(i, k+1) * (1-2*tau)))
            {unhapBlue.push_back (make_tuple(i, k+1));}
        }}
}


void doswapfollowconv(void){
    if (roundvar % roundrep ==0) {swaptime0 = std::chrono::system_clock::now();}
    
    x= rand () % unhapRed.size();//pick the unhappy to swap
    y= rand () % unhapBlue.size();
    swapx = unhapRed[x];// Record last swap
    swapy = unhapBlue[y];
    
    swapx0=std::get<0>(unhapRed[x]);
    swapx1=std::get<1>(unhapRed[x]);
    swapy0=std::get<0>(unhapBlue[y]);
    swapy1=std::get<1>(unhapBlue[y]);
    
    while (roundret< swapmax && tau>0.5 && convar==1 && (
                                                         ( ( (abs(swapx0-swapy0)<=w) || (torvar * abs(swapx0-swapy0) >= numbhum - w))//if not 0-far from swapy
                                                          && ((abs(swapx1-swapy1)<=w)  || (torvar * abs(swapx1-swapy1) >= numbhum - w))) ||
                                                         biasArray[swapx0][swapx1] >= biasArray[swapy0][swapy1] + 2) )//or bias won't increase after swap
    {//keep the convension for >0.5 - re-choose the swap targets
        
        x= rand () % unhapRed.size();//pick the unhappy to swap
        y= rand () % unhapBlue.size();
        swapx = unhapRed[x];// Record last swap
        swapy = unhapBlue[y];
        if (roundvar % roundrep ==0){cout<< " Trying swap: "<< roundvar+1<< ". I re-chose to swap ("<< swapx0 << ", " <<
            swapx1 << ") with ("<<  swapy0 << ", " << swapy1 << ")\n";}
        swapx0=std::get<0>(unhapRed[x]);//need to do this here to refer to the coordinates in the while condition
        swapx1=std::get<1>(unhapRed[x]);
        swapy0=std::get<0>(unhapBlue[y]);
        swapy1=std::get<1>(unhapBlue[y]);
        roundret++;
    }
    //now we finally found a suitable swap pair
    
    changedhum.push_back (make_tuple(roundvar, make_tuple(-1, make_tuple(swapx0, swapx1))));
    changedhum.push_back (make_tuple(roundvar, make_tuple(1, make_tuple(swapy0, swapy1))));
    
    numbhumArray[swapx0][swapx1] = -1;// Do the swap;
    numbhumArray[swapy0][swapy1] = 1;
    if (testmode==1){cout<< " I swap ("<< swapx0 << ", " << swapx1 << ") with ("<<  swapy0 << ", " << swapy1 << ")\n";}
    unhapRed.erase (unhapRed.begin()+x);//Erase member with index x from unhapRed
    unhapBlue.erase (unhapBlue.begin()+y);//Erase member with index y from unhapBlue
}

void updatebiaslists(void){//update bias and unhappy arrays
    if (roundvar % roundrep ==0) {swaptime1 = std::chrono::system_clock::now();}
    
    for (i=0; i < 2*w + 1; i++){//Update the bias around humans unhapRed[x], unhapBlue[y]
        for (j=0; j < 2*w + 1; j++){//Update the bias around humans unhapRed[x], unhapBlue[y]
            
            if (signfu(swapx0-w+i, numbhum)>=0 && signfu(swapx1-w+j, numbhum)>=0){//if coordinate legitimate for swapx
                biasvarR0=signfu(swapx0-w+i, numbhum);//set biasvarR tuple to that coordinate for swapx
                biasvarR1=signfu(swapx1-w+j, numbhum);
                
                biasRtargOld=biasArray[biasvarR0][biasvarR1]; biasRtargNew =biasRtargOld; //save the old value for use below
                
                if ( ( ( (abs(biasvarR0-swapy0)>w) && (torvar * abs(biasvarR0-swapy0) < numbhum - w))//if 0-far from swapy
                      || ((abs(biasvarR1-swapy1)>w) && (torvar * abs(biasvarR1-swapy1) < numbhum - w))) )//or if 1-far from swapy
                    
                {biasRtargNew = biasArray[biasvarR0][biasvarR1]-2; biasArray[biasvarR0][biasvarR1]=biasRtargNew;//update bias
                    
                    //now update unhappy lists for this point  (biasvarR0,biasvarR1)
                    if ((numbhumArray[biasvarR0][biasvarR1]==1)//if red
                        && (biasRtargNew < neighsize(biasvarR0, biasvarR1) * (2*tau -1))//and unhappy
                        &&//and was happy before, i.e. not currently in the list
                        (biasRtargOld >= neighsize(biasvarR0, biasvarR1) * (2*tau -1)) ){
                        unhapRed.push_back (make_tuple(biasvarR0, biasvarR1));}//then put it in
                    
                    else if ((numbhumArray[biasvarR0][biasvarR1]==-1)//if blue
                             &&//and was unhappy before, i.e. currently in the list
                             (biasRtargOld > neighsize(biasvarR0, biasvarR1) * (1-2*tau)) &&// and its not swapx
                             (!(biasvarR0==swapx0) || !(biasvarR1==swapx1))
                             &&//but is happy now
                             (biasRtargNew <= neighsize(biasvarR0, biasvarR1) * (1-2*tau)))//then take it out of the list
                    {unhapBlue.erase(std::remove(unhapBlue.begin(), unhapBlue.end(),
                                                 make_tuple(biasvarR0, biasvarR1)), unhapBlue.end());}//END of 3rd if
                }//END OF second IF
            }//END OF first IF, end of make the change in bias and unhaplist part R
            
            if (signfu(swapy0-w+i, numbhum)>=0 && signfu(swapy1-w+j, numbhum)>=0){//if coordinate legitimate for swapy
                biasvarB0=signfu(swapy0-w+i, numbhum);//set biasvarB tuple to that coordinate for swapy
                biasvarB1=signfu(swapy1-w+j, numbhum);
                
                biasBtargOld=biasArray[biasvarB0][biasvarB1];//save the old value for use below
                biasBtargNew=biasBtargOld;
                
                if ( ( ( (abs(biasvarB0-swapx0)>w) && (torvar * abs(biasvarB0-swapx0) < numbhum - w))//if 0-far from swapx
                      || ((abs(biasvarB1-swapx1)>w) && (torvar * abs(biasvarB1-swapx1) < numbhum - w))) )//or if 1-far from swapx
                {biasBtargNew = biasArray[biasvarB0][biasvarB1]+2; biasArray[biasvarB0][biasvarB1] = biasBtargNew;
                    
                    //now update unhappy lists for this point  (biasvarB0,biasvarB1)
                    if ((numbhumArray[biasvarB0][biasvarB1]==1)//if red
                        && (biasBtargNew >= neighsize(biasvarB0, biasvarB1) * (2*tau -1) )
                        //and happy (equiv. was not happy before by more than 2 margin)
                        &&//and was unhappy before and is not swapy, i.e. not currently in the list
                        (biasBtargOld < neighsize(biasvarB0, biasvarB1) * (2*tau -1)) &&
                        ( !(biasvarB0== swapy0) || !(biasvarB1== swapy1))){
                        unhapRed.erase(std::remove(unhapRed.begin(), unhapRed.end(),
                                                   make_tuple(biasvarB0, biasvarB1)), unhapRed.end());}
                    
                    else if ((numbhumArray[biasvarB0][biasvarB1]==-1)//if blue
                             &&//and was happy before
                             (biasBtargOld <= neighsize(biasvarB0, biasvarB1) * (1-2*tau))
                             &&//but unhappy now
                             (biasBtargOld +2 > neighsize(biasvarB0, biasvarB1) * (1-2*tau)))//then put it in the list
                    {unhapBlue.push_back (make_tuple(biasvarB0, biasvarB1));}//then remove it from Red unhappy list
                }//END OF INNER IF FOR biasvarB
            }//END OF outer IF, end of make the change in bias and unhaplist
        }}   //end of the two for
    
    if ( //if it is swap x and unhappy then put it in
        biasArray[swapx0][swapx1] > neighsize(swapx0, swapx1) * (1 - 2*tau) )
    {unhapBlue.push_back (make_tuple(swapx0, swapx1));}
    
    
    if ( //if it is swap y and unhappy then put it in
        biasArray[swapy0][swapy1] < neighsize(swapy0, swapy1) * (2*tau -1) )
    {unhapRed.push_back (make_tuple(swapy0, swapy1));}
    
    if (roundvar % roundrep ==0) {swaptime2 = std::chrono::system_clock::now();}
    
    
}//end of routine

int loopstopcondition(void){
    if (unhapRed.empty()) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " There are no more unhappy red humans (reason for stopping swapping).\n"; return 0;}
    else if (unhapBlue.empty()) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " There are no more unhappy blue humans (reason for stopping swapping).\n"; return 0;}
    else if (roundvar>=swapmax) {std::cout <<
        "\n Finished swapping after "<< roundvar << " swaps.\n"<<
        " We hit the max number of swaps (reason for stopping swapping).\n"; return 0;}
    else if (roundret >= swapmax) {std::cout <<
        "\nFinished swapping after "<< roundvar << " swaps and " << roundret <<
        " unsuccessful swap attempts under convention.\n"; return 0;}
    else {return 1;} }

///REPORT SUBROUTINES (PART OF MAIN PROGRAM)

void reportunhapnumb(void){
    finbiasArray = std::chrono::system_clock::now();
    long long finbiasArrayms = std::chrono::duration_cast<std::chrono::milliseconds> (finbiasArray-start).count();
    
    std::cout << " Compiled initial bias and unhappy arrays (in " << finbiasArrayms << " ms)\n";
    std::cout << " We have "<< unhapRed.size() << " unhappy red and " << unhapBlue.size() << " unhappy blue\n";
    std::cout << "\n"<< " Started swapping ...\n\n";}


void reportnumbswapscur(void){
    
    if (roundvar % roundrep ==0) {
        looptime1 = std::chrono::system_clock::now();
        looptimedif = std::chrono::duration_cast<std::chrono::milliseconds> (looptime1-looptimen0).count();
        std::cout << "\n\n " << roundvar +1 << " swaps completed and going ...";
        std::cout << "(Unhappy: "<< unhapRed.size()<< " red and "<< unhapBlue.size()<<" blue)\n\n";
        looptimen0 = looptime1;
        swaptimedif1 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime1-swaptime0).count();
        swaptimedif2 = std::chrono::duration_cast<std::chrono::microseconds> (swaptime2-swaptime1).count();
        //spaptimes: 0 when swap begins, 1 when the choice is done and earased from unhaplists
        //2 when the bias is updated, 3 when unhap lists updated, 4 when unhappy lists are normalized
        
        std::cout << "      Time report on swap "<<roundvar +1<<":\n\n";
        std::cout <<"           - Last "<< roundrep << " swaps in "<< looptimedif << " ms\n";
        std::cout <<"           - "<< swaptimedif1<<" mus for swap and erase from unhappy lists\n";
        std::cout <<"           - "<< swaptimedif2<<" mus for updating the bias array and unhappy lists\n\n\n";
        
        if (testmode==1){detailedreport();}
    }}

void outputfinalnumbers (void){//Output number of unhappy and runs
    std::cout << "\n The number of the finally unhappy is "
    << unhapRed.size() << " red and "<< unhapBlue.size() << " blue\n";
}
//
/////REPORT SUBROUTINES (FOR TESTING)
//
void detailedreport(void){//FOR TESTING
    std::cout << "\n"; //output the human/bias array
    std::cout << " The human/bias array is:\n";
    std::cout << "\n";
    for (i=0; i< numbhum; i++){
        for (k=0; k<numbhum; k++){
            if (biasArray[i][k]<0 && numbhumArray[i][k]==1) {std::cout << " "<< biasArray[i][k]<< "x ";}
            else if (biasArray[i][k]<0 && numbhumArray[i][k]==-1) {std::cout << " "<< biasArray[i][k]<< "o ";}
            else if (biasArray[i][k]>=0 && numbhumArray[i][k]==1){std::cout << " "; std::cout << " "<< biasArray[i][k]<< "x ";}
            else if (biasArray[i][k]>=0 && numbhumArray[i][k]==-1){std::cout << " "; std::cout << " "<< biasArray[i][k]<< "o ";}
            std::cout << "\t";}
        std::cout << "\n\n";}
    std::cout << "\n";
    
    std::cout << " The unhappy red are:\n";
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


//MAIN PROGRAM
int main(int argc, char **argv) {
    roundret=0;
    initialcall();                                              //ask for imput to the program
    
    start = std::chrono::system_clock::now();//new timer
    timestart();             //set the time parameters for start and announce
    //
    //
    numbhumArray.resize (numbhum, vector<int> ( numbhum ));         //resize arrays
    numbhumArray0.resize (numbhum, vector<int> ( numbhum ));         //resize arrays
    biasArray.resize (numbhum, vector<int> ( numbhum ));
    buildhumans();                                              //build humans and announce
    buildinitbiasarr();                                     //build initial bias array
    if (testmode==1) {detailedreport();}
    
    reportunhapnumb();
    
    looptimen0 = std::chrono::system_clock::now();
    
    for (roundvar=0; roundvar<=swapmax; roundvar++) {// start swapping loop, first checking conditions
        if (loopstopcondition()==0){break;}
        doswapfollowconv();
        updatebiaslists();
        reportnumbswapscur();
    }// End of Swapping loop
    
    if (testmode==1){detailedreport();}
    outputfinalnumbers();
    timeendroutine();
    
    drawtheoutput(argc, argv);
    return 0;
}
