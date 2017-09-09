#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

enum ALIGNMENT {CENTER=0,LEFT,RIGHT};
class table
{
    public:
      //table(rows,columns,width,height,border,cellspacing,cellborder,alignment)  
        table(unsigned int col,
              unsigned int row, 
              unsigned int wid=100,
              unsigned int hei=100,
              unsigned int bor=0,
              unsigned int ces=0,
              unsigned int ceb=0,
              unsigned int alignment=CENTER) : rows(row), columns(col), width(wid), height(hei), border(bor),cellspacing(ces),cellborder(ceb), align(alignment)
        {
            colrow.resize(col);
            for (int i =0; i < col; i++)
                colrow[i].resize(row);
        }
        unsigned int &numRows(){ return rows; }
        unsigned int &numColumns(){ return columns; }
        unsigned int &Height(){ return height; }
        unsigned int &Width(){ return width; }
        unsigned int &Border(){ return border;}
        unsigned int &CellSpacing(){ return cellspacing; }
        unsigned int &CellBorder(){ return cellborder; }
        
        vector<string> &operator[](unsigned int index)
        {
            if (index >= columns)
                index=columns;
            return colrow[index];                
        }

        string &completeTable()
        {
            mytable  << "<p align=";
            mytable  << (align==CENTER ? "center" : (align==LEFT ? "left" : "right"));
            mytable << ">";
            mytable << "<table height=" << height << " width=" << width << " border=" << border << " cellspacing=" << cellspacing << " cellpadding=" << cellborder << ">";
            for ( int i = 0; i < columns; i++)
            {
                mytable << "<tr>";
                for (int j = 0; j< rows; j++)
                {
                    mytable << "<td valign=top>";
                    mytable << colrow[i][j];
                    mytable << "</td>";
                }
                mytable << "</tr>";
            }
            mytable << "</table>";
            tt=mytable.str();
            return tt;
        }
       
    private:
        stringstream mytable;
        string tt;
        unsigned int rows;
        unsigned int columns;
        unsigned int height;
        unsigned int width;
        unsigned int align;
        unsigned int border;
        unsigned int cellspacing;
        unsigned int cellborder;
        vector< vector<string> > colrow;
};



int main() 
{
    ofstream fwrite("html2.html");

    int rows=0,columns=0;
    char text[255];
    cout << "Enter number of columns ";
    cin.getline(text,15);
    columns=atoi(text);
    cout <<  "Now enter the number of rows ";
    cin.getline(text,15);
    rows=atoi(text);
    table myTable(columns,rows);
    cout << "Now, enter the data: " << endl;
            for ( int i = 0; i < columns; i++)
            {
    //            cout << "for column " << i;
                for (int j = 0; j< rows; j++)
                {
  //                  cout << " row " << j << ": ";
//                    cin.getline(text,255);
                    sprintf(text,"%i",j);
                    myTable[i][j]=text;
//                    cout << endl << "we have text, it is " << text;
                }
            }

    fwrite << myTable.completeTable();
    fwrite.close();
    return 0;
}
