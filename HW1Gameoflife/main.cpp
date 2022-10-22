#include "console.h"
#include "simpio.h"
#include <fstream>
#include "filelib.h"
#include "strlib.h"
#include "grid.h"
#include "lifegui.h"
using namespace std;
void WelcomeInfoDisplay(void);
string Getchoice(void);
void GetMatrix(ifstream &file,Grid<char> &bound);
void LoadFile(Grid<char>&bound);
int CountCell(Grid<char> &bound, int &row, int &col, bool &mode);
void showGrid(Grid<char>&bound);
//Grid<char> GenNextBound(Grid<char>&bound, bool mode);
void GenNext(Grid<char>&bound,bool mode,LifeGUI &life);
int getFrames();
void GuiDisplay(LifeGUI &life,Grid<char> &bound);
Grid<char> GenNextBound(Grid<char>&bound, bool mode,LifeGUI &life);






int main()
{
    Grid<char>bound(2,2);//default
    LifeGUI life;
    WelcomeInfoDisplay();
    LoadFile(bound);
    life.resize(bound.numRows(),bound.numCols());
    bool mode = getYesOrNo("Should the simulation wrap around the grid (y/n)");
    showGrid(bound);
    GuiDisplay(life,bound);
    GenNext(bound,mode,life);
   return 0;
}

void WelcomeInfoDisplay(void)
{
    cout<< "Welcome to the CS 106B Game of life, "<<endl;
    cout<< "a simulation of the lifecycle of a bacteria colony. "<<endl;
    cout<< "Cell(X) live and die by the following rules: "<<endl;
    cout<< "- A cell with 1 or fewer neighbors dies.  "<<endl;
    cout<< "- Locations with 2 neighbors remain stable.  "<<endl;
    cout<< "- Locations with 3 neighbors will create life.  "<<endl;
    cout<< "- A cell with 4 or more neighbors dies.  "<<endl;
    cout<<endl;
    cout<<endl;
}
void GetMatrix(ifstream &file,Grid<char> &bound)
{
    string s;
    int r =0;
    while(getline(file,s)&&r<bound.numRows())
    {
        for(int c = 0;c!=bound.numCols();c++)
            bound[r][c] = s[c];
        r++;
    }
}
string Getchoice()
{
   string prompt = "a)nimate, t)tick, q)uit?";
   string choice = getLine(prompt);
   while(!(choice=="a"||choice=="t"||choice=="q")){
       cout<<"Invalid choice; please try again."<<endl;
       choice = getLine(prompt);
   }
   return choice;
}
void GenNext(Grid<char>&bound,bool mode,LifeGUI &life){
    string choice = Getchoice();
    while(choice!="q"){
        //run funcion about generate'
        if(choice == "a"){
            int frames = getFrames();
            int f=1;
            while(f<=frames)
            {
               //text version
                clearConsole();
                Grid<char> Next = GenNextBound(bound,mode,life);
                bound = Next;
                f++;
                pause(50);
            }
        }
        if(choice == "t"){
            Grid<char> Next = GenNextBound(bound,mode,life);
            //showGrid(Next);
            bound = Next;
        }
        choice = Getchoice();
    }
    cout<<"Have a nice life!"<<endl;
}
void LoadFile(Grid<char>&bound)
{
    ifstream fileStream;
    string numsStr;
    string LineSymbol;
     int rows;
     int cols;
    string filename = getLine("Grid input file name?");
    while(!fileExists(filename)){
       cout<<"Unable to open that file.  Try again."<<endl;
       filename = getLine("Grid input file name?");
    }
        openFile(fileStream,filename);
        getline(fileStream,numsStr);
        rows = stringToInteger(numsStr);
        getline(fileStream,numsStr);
        cols = stringToInteger(numsStr);
        bound.resize(rows,cols);
        GetMatrix(fileStream,bound);
        fileStream.close();
}
int CountCell(Grid<char> &bound, int &row, int &col, bool &mode){
    int sum = 0;
    int rows = bound.numRows();
    int cols = bound.numCols();
    if(mode==true){
        for(int i=-1;i<2;i++){
           if(bound[(row+i+rows)%rows][(col-1+cols)%cols] == 'X')
               sum++;
        }
        for(int i=-1;i<2;i++){
            if(bound[(row+i+rows)%rows][(col+1+cols)%cols] == 'X')
                sum++;
        }
        if(bound[(row-1+rows)%rows][(col+cols)%cols] == 'X')
            sum++;
         if(bound[(row+1+rows)%rows][(col+cols)%cols] == 'X')
            sum++;
        return sum;
    }
    if(mode==false){
        for(int i=-1;i<2;i++){
            if(bound.inBounds(row+i,col-1)&&bound[row+i][col-1]=='X')
                sum++;
        }
        for(int i=-1;i<2;i++){
            if(bound.inBounds(row+i,col+1)&&bound[row+i][col+1]=='X')
                sum++;
        }
        if(bound.inBounds(row-1,col)&&bound[row-1][col]=='X')
            sum++;
        if(bound.inBounds(row+1,col)&&bound[row+1][col]=='X')
            sum++;
        return sum;
       }
    return -1;
    }
void showGrid(Grid<char>&bound)
/*
 * text show cell function
*/
{
    for(int i =0 ; i<bound.numRows();i++)
    {
        for(int j=0 ; j<bound.numCols();j++)
            cout<<bound[i][j];
        cout<<endl;
    }
}
Grid<char> GenNextBound(Grid<char>&bound, bool mode,LifeGUI &life){
/*
 * Calulate the next generation
 * text show the next
 * anmiate the change
*/
    Grid<char> next = bound;
    int cols = bound.numCols();
    int rows = bound.numRows();
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            int NeighborCells = CountCell(bound,r,c,mode);
            if(NeighborCells<2&&bound[r][c]=='X'){
                next[r][c] = '-';
                life.drawCell(r,c,false);
            }
            if(NeighborCells==3&&bound[r][c]=='-'){
                next[r][c] = 'X';
                life.drawCell(r,c,true);
            }

            if(NeighborCells>3&&bound[r][c]=='X'){
                next[r][c] = '-';
                life.drawCell(r,c,false);
            }

        }
    }
    showGrid(next);
    return next;
}
int getFrames(){
    string prompt = "How many frames?";
    string frames = getLine(prompt);
    while(!stringIsInteger(frames)){
        cout<<"Illegal integer format. Try again."<<endl;
        frames = getLine(prompt);
    }
    return stringToInteger(frames);
}
void GuiDisplay(LifeGUI &life,Grid<char> &bound){
    bool state=false;
    for(int r=0;r<bound.numRows();r++){
        for(int c=0;c<bound.numCols();c++){
            if(bound[r][c]=='X'){
                state = true;
            }
            else if(bound[r][c]=='-'){
                state = false;
            }
            life.drawCell(r,c,state);
        }
    }
}
