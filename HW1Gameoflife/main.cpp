#include "console.h"
#include "simpio.h"
#include <fstream>
#include "filelib.h"
#include "strlib.h"
#include "grid.h"
//#include "lifegui.h"
using namespace std;

void InfoDisplay(void)
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
void getsize(ifstream &file,string &path,int &row , int &col)
{
    string s;
    openFile(file,path);
        getline(file,s);
        row = stringToInteger(s);
        getline(file,s);
        col = stringToInteger(s);
}
Grid<char> getmatrix(ifstream &file,int&row , int &col)
{
    Grid<char>bound(row,col);
    string s;
    int i =0;
    while(getline(file,s)&&i<row)
    {
        for(int j = 0;j!=col;j++)
            bound[i][j] = s[j];
        i++;
    }
    file.close();
    return bound;
}


void showGrid(Grid<char>&bound)
{
    for(int i =0 ; i<bound.numRows();i++)
    {
        for(int j=0 ; j<bound.numCols();j++)
            cout<<bound[i][j];
        cout<<endl;
    }
};
void getRealPos(int&x,int&y,Grid<char> &bound,int &real_pos_x, int &real_pos_y)
{

    real_pos_x = (x+bound.numCols())%(bound.numCols());
    real_pos_y = (y+bound.numRows())%(bound.numRows());
}
int cellCanLive(int &x ,int &y,Grid<char> &bound)
/*
 * wrapping mode
 * x col 1
 * y row 0
*/
{
    int sum = 0;
    int pos_x, pos_y =0;
    int y_temp,x_temp;
    //-1 row
    y_temp = y-1;
    for(int i = -1;i<2;i++)
    {
        int x_temp = x+i;
        getRealPos(x_temp,y_temp,bound,pos_x,pos_y);
        if(bound[pos_y][pos_x]=='X')
            sum++;
    }
    //0 row
    y_temp = y;
    x_temp = x-1;
    getRealPos(x_temp,y_temp,bound,pos_x,pos_y);
    if(bound[pos_y][pos_x]=='X')
    {
        sum++;
    }
    x_temp = x+1;
    getRealPos(x_temp,y_temp,bound,pos_x,pos_y);
    if(bound[pos_y][pos_x]=='X')
    {
        sum++;
    }
    //+1 row
    y_temp = y+1;
    for(int i = -1;i<2;i++)
    {
        int x_temp = x+i;
        getRealPos(x_temp,y_temp,bound,pos_x,pos_y);
        if(bound[pos_y][pos_x]=='X')
            sum++;
    }

    return sum;
}
void countCell(Grid<char> &bound, int &x ,int &y)
/*
 * no-wrapp mode
 * x - col
 * y - row
 */
{
    //corner
    // edge
    //inner
    int row = bound.numRows();
    int col = bound.numCols();
    if((x==0||x==(col-1))&&(y==0||y==(row-1)))
    {
        //corner
        if(x==0&&y==0)
        {
//            bound(x,y+1)
//            bound(x+1,y)
//            bound(x+1,y+1)
        }

    }
    if(x==0&&y!=0&&y!=(row-1))
    {
        //left
    }
    if(x==(col-1)&&y!=0&&y!=(row-1))
    {
        //right
    }
    if(y==0&&x!=0&&x!=(col-1))
    {
        //top
    }
    if(y==(row-1)&&x!=0&&x!=(col-1))
    {
        //bottom
    }
}


void GetNextGeneration(Grid<char> &bound,Grid<char> &next)
{
    for(int i = 0;i<bound.numRows();i++)
    {
        for(int j = 0 ; j<bound.numCols();j++)
        {
            int sum = cellCanLive(j,i,bound);
            if(bound[i][j]=='-'&&sum==3)
            {
                   next[i][j] = 'X';
                   continue;
            }
            if(bound[i][j]=='X'&&(sum==0||sum==1))
            {
                next[i][j] = '-';
                continue;
            }
            if(bound[i][j]=='X'&&(sum==4||sum==5||sum==6||sum==7||sum==8))
            {
                next[i][j] = '-';
                continue;
            }
        }
    }
}


int main()
{
    string path = "./res/snowflake.txt";
    ifstream file;
    string command;
    Grid<char> bound;
    Grid<char> next;
    int row,col;

//    next = bound;
//    showGrid(next);
//    cout<<"            "<<endl;
//    cout<<"            "<<endl;
//    GetNextGeneration(bound,next);
//    showGrid(next);


    //user interface
    InfoDisplay();
    cout<<"Grid input file name?";
    while(!fileExists(command = getLine()))
    {

        cout<<"Unable to open that file.  Try again."<<endl;
        cout<<"Grid input file name?";
     }
        getsize(file,command,row,col);
        bound =  getmatrix(file,row,col);
        next = bound;
     cout<<"Should the simulation wrap around the grid (y/n)?";
     command = getLine();//assume 'yes'
     if(command == "y") showGrid(bound);//select wrapp or no- wrapp
     cout<<"a)nimate, t)ick, q)uit? ";
     while((command=getLine())!="q")
     {
         if(command == "a")
         {
             cout<<"will come!"<<endl;
         }
         if(command == "t")
         {
             GetNextGeneration(bound,next);
             bound = next;
             showGrid(next);
         }
             cout<<"a)nimate, t)ick, q)uit? ";
     }
     cout<<"Have a nice life!"<<endl;







    return 0;
}
