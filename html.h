#ifndef HTML_H
#define HTML_H
// implement layers
// lastRead = lastRead
// cS = compare_string
// traverse = traverse
// higher is faster

#define RATIO 300
#include <sstream>
#include <string>
#include "retreive_values.h"
#include "string_functions.h"
#include "HashTable.h"
#include "hash.h"
//#include "cookies.h"
using namespace std;
enum ALIGNMENT{ CENTER=0, LLEFT, RRIGHT };
//enum ALIGNMENT{CENTER,LEFT,RIGHT};
class html : protected string_functions
{
        struct lyr
		{
			string layer_name;
            string layer_path;
            HashTable<string,string> layer_hash; // be sure to setHash(&stoi,4,0) before using
			
		};
        retreive_values html_variables;
        
        HashTable<string,string> html_hash;

    public:
//	html(bool pr = true){ ::html("",pr); }
    	html(bool  = true);
	   ~html(){}
        bool parsePage(const char *path);

        void dump_core(const char *inData,int data_type);

        void html_value(string aName, string aValue ){ html_hash[aName]=aValue; }

        void html_value(string aName, int aValue )
	{ 
		string tmp; 
		char d[255];
		sprintf(d,"%i",aValue);
		tmp=d; 
		html_hash[aName]=tmp; 
	}

    	void htmlerror(string, int);

        bool create_layer(string,string);

//    	void layer_html_value(const char* aName, char* aValue){ layer.layer_array.add_value(aName,aValue); }

        void finalize_layer(const char *inname);

        HashTable<string,string> &layer(string);

        vector < lyr > layers;

        bool find_layer(string,lyr&);
        bool find_layer(string name)
        {
            for (int i = 0; i < layers.size(); i++)
            if (layers[i].layer_name==name)
                    true;
             return false; 
        }
     //   HashTable<string,string> &layer(string name);


   //     cookies cookieFunctions;

        
    protected:
        string html_string;
//    	string parse_layer(const char *inpath);
    	string &parse_layer(const char *inpath,HashTable<string,string> &);
    	string &parse_layer(const char *inpath){ return parse_layer(inpath,html_hash); }
      
    private:

       
        class table
        {
            public:
            table(unsigned int col=0,
                  unsigned int row=0, 
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

            void SetTable(unsigned int col,
                  unsigned int row, 
                  unsigned int wid=100,
                  unsigned int hei=100,
                  unsigned int bor=0,
                  unsigned int ces=0,
                  unsigned int ceb=0,
                  unsigned int alignment=CENTER)
            {
	    	rows=row;
		columns=col;
		width=wid;
		height=hei;
		border=bor;
		cellspacing=ces;
		cellborder=ceb;
		align=alignment;
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
                mytable  << (align==CENTER ? "center" : (align==LLEFT ? "left" : "right"));
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
    protected:
		table myTable;

};
html::html( bool pr)
{
	if (pr)
    {
    		cout << "Cache-Control: no-store\n";
		cout << "Pragma: no-cache\n";
		cout << "Content-type: text/html\n\n";
//    cout << "<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">";

    }
    //cout << "here";
    html_hash.Set(&stoi,10,"");
  //  cout << "here";
    html_variables.parse_file("/home/marc/data_files/html.dat");
//	html_variables["CORE_HTML"]=val;
//    cout << "here";
    
}

string
&html::parse_layer(const char *inpath,HashTable<string,string> &myHash) {

   // cout << "yo";

	ifstream filestream;
//	filestream.open(inpath, ios::binary );
	filestream.open(inpath);
    html_string="";
//	stringstream returnstring;
//	string html_string="";
    //cout << inpath << " " << endl;
   // cout << "yo";
    string_functions st;
    int n=0;
	if (filestream && filestream.good())
	{
		char *traverse, *compare_string,*insert_string, *findtext="$(";
		 // get length of file:
		int filesize=0,lastRead=0,originalfilesize,lastplace = 0,i;
  		filestream.seekg (0, ios::end);
  		filesize = filestream.tellg();
		originalfilesize = filesize;

  //  cout << "yo1";

		while (filestream.good())
		{
			filestream.seekg (lastplace, ios::beg);
   // cout << "yo2";
			if (filesize > RATIO)
			{
				lastRead = RATIO;
				lastplace = lastplace+RATIO;
				filesize = filesize-RATIO;

			}
			else
			{

				lastRead = originalfilesize-lastplace;
				lastplace = originalfilesize;
				filesize=0;
			}
  //  cout << "yo3";

				if (lastRead <= 0) break;
				traverse = new char[lastRead+2];
				filestream.read(traverse,lastRead);
				traverse[lastRead]='\0';

   /// cout << "yo4";

  				i = st.find(traverse,findtext);
				if (i==-1)
				{
//					html_string += traverse;
                    cout << traverse;
						//html_string << traverse;
					continue;
				}
                for (int j=0; j<i; j++)
                    cout << traverse[j];
//                    html_string+= traverse[j];
    
				filestream.seekg ((lastplace+2)-(lastRead-i), ios::beg);
				delete [] traverse;

				if (originalfilesize - ((lastplace+2)-(lastRead-i)) > 30)
				{
					traverse = new char[32];
					filestream.read(traverse,30);
					traverse[30]='\0';
				}
				else
				{
					traverse = new char[ (originalfilesize-( (lastplace+2)-(lastRead-i))) +2];
					filestream.read(traverse, originalfilesize-( (lastplace+2)-(lastRead-i)) );
					traverse[ originalfilesize-( (lastplace+2)-(lastRead-i))]='\0';
				}
   // cout << "yo5";

				compare_string = traverse;
				while (*compare_string)
				{
                  //  cout << "yo5.5";
					if (*compare_string == 41)
					{
                     //   cout << "yo6";
						insert_string = new char[ (strlen(traverse)-strlen(compare_string))+2];
						strncpy(insert_string,traverse,strlen(traverse)-strlen(compare_string));
						insert_string[(strlen(traverse)-strlen(compare_string))] ='\0';
						// if the value of insert_string is quite large, then it can slow us
						// down some, so just let mvalueOut decide the increments
//                        cout << insert_string << "--";
//                        cout << myHash[insert_string].GetData() << " " << insert_string << " " << inpath << endl;
                         cout << myHash[insert_string].GetData();
//							html_string += myHash[insert_string].GetData();
						delete [] insert_string;
                  //      cout << "yo7";
						break;


					}
					compare_string++;
				}   

     //        html_string+=compare_string;
                      //  cout << "yo8";
				lastplace=((lastplace+2)-(lastRead-i)) + ((strlen(traverse)-strlen(compare_string))+1);
				filesize=((filesize+2)-(lastRead-i))+ ( (strlen(traverse)-strlen(compare_string)));
			delete [] traverse;

		}
               //     return "";
	}else{
        htmlerror(html_variables.value("HTML_ERROR"),-1);
	}
	filestream.close();
	return html_string;
}


bool
html::parsePage(const char *path)
{
	dump_core(path,0);
	return true; 
}


// 0 means a filepath
// 1 means formatted data
void
html::dump_core(const char *indata, int data_type)
{
	// most likely being called from the command line 
	if (html_variables.value("USE_TEMPLATE") == "0" || data_type==1)
	{
		parse_layer(indata);
		return;    
	}  

		string sfullpath;
        sfullpath = html_variables.value("HTML_DIRECTORY");
        if (html_variables.value("HTML_DIRECTORY").length() > 0)
    		sfullpath += "/";
        sfullpath += html_variables.value("CORE_HTML");
        /* can be used for multisite application
           sfullpath += ".";
	       sfullpath += program_setup->site;  
        */

	ifstream filestream;
    //cout << sfullpath.c_str() << " " << html_variables.value("CORE_HTML")<<endl;
	filestream.open(sfullpath.c_str());

		if (filestream && filestream.good())
		{
			char *traverse, *compare_string,*insert_string, *findtext="$(";
			int filesize=0,lastRead=0,originalfilesize,lastplace = 0,i;
  			filestream.seekg (0, ios::end);
  			filesize = filestream.tellg();
			originalfilesize = filesize;
			while (filestream.good())
			{
				filestream.seekg (lastplace, ios::beg);
				if (filesize > RATIO)
				{
					lastRead = RATIO;
					lastplace = lastplace+RATIO;
					filesize = filesize-RATIO;

				}
				else
				{

					lastRead = originalfilesize-lastplace;
					lastplace = originalfilesize;
					filesize=0;
				}
					if (lastRead <= 0) break;
					traverse = new  char[lastRead+1];
					filestream.read(traverse,lastRead);
//						cout << traverse;
					traverse[lastRead]='\0';
					i = find(traverse,findtext);
					if (i == -1)
					{  
                        cout << traverse;
//                        cout << traverse << " " << findtext << " " << lastRead<< endl;
  //                      cout << "whoa nelly again" << endl;
						continue;
					}
					cout.write(traverse,i);
					filestream.seekg ((lastplace+2)-(lastRead-i), ios::beg);
					delete [] traverse;

					if (originalfilesize - ((lastplace+2)-(lastRead-i)) > 30)
					{
						traverse = new char[31];
						filestream.read(traverse,30);
						traverse[30]='\0';
					}
					else
					{
						traverse = new char[ (originalfilesize-( (lastplace+2)-(lastRead-i))) +2];
						filestream.read(traverse, originalfilesize-( (lastplace+2)-(lastRead-i)) );
						traverse[ originalfilesize-( (lastplace+2)-(lastRead-i)) ]='\0';
					}

					compare_string = traverse;
					while (*compare_string)
					{
						if (*compare_string == 41)
						{
							insert_string = new char[ (strlen(traverse)-strlen(compare_string))+2];
							strncpy(insert_string,traverse,strlen(traverse)-strlen(compare_string));
							insert_string[(strlen(traverse)-strlen(compare_string))] ='\0';
							if (!strcmp(insert_string,"paste"))
							{
								if (data_type == 1)
                                {
									cout << indata;
                                }
								else
                                {
									parse_layer(indata,html_hash);
									//cout << parse_layer(indata,html_hash);
                                }
							}
							delete [] insert_string;
							break;

						}
						compare_string++;
					}
					lastplace=((lastplace+2)-(lastRead-i)) + ((strlen(traverse)-strlen(compare_string))+1);
					filesize=((filesize+2)-(lastRead-i))+ ( (strlen(traverse)-strlen(compare_string)));
				delete [] traverse;
			}
	}else{
            htmlerror(html_variables.value("HTML_ERROR"),-1);
		}

}

void
html::htmlerror(string reportError, int errornum)
{
    string filelocation = html_variables.value("HTML_DIRECTORY");
    filelocation+="error.dat";
    //cout << filelocation << endl;
	ifstream	filestream(filelocation.c_str());
	string::size_type location;
	// if the file is good...
	if (filestream && filestream.good()) {
			stringstream *universal_html = new stringstream();
			*universal_html << filestream.rdbuf();
			string universal_html_string;
			universal_html_string = universal_html->str();
			delete universal_html;
			location = universal_html_string.find("$(errorno)",0);
			//cout << location << endl;
			if (location != string::npos)
			{
				string tmp; 
				char d[255];
				sprintf(d,"%i",errornum);
				tmp=d; 
		
				universal_html_string.replace(location,10,tmp);
			}
			location = universal_html_string.find("$(error)",0);
			//cout << location << " " << universal_html_string << endl;
			if (location != string::npos)
			{
				universal_html_string.replace(location,8,reportError);
			}
			cout << universal_html_string << endl;
			//dump_core(universal_html_string.c_str(),1);
	filestream.close();
	}else{ 
        cout << "<html><body><p align=center> File Error on path " << filelocation.c_str() << "</p></body></html>" << endl; 
    }
    exit(1);

}

bool
html::create_layer(string inpath,string name)
{   
    
    if (!find_layer(name))
    {
        layers.resize( layers.size()+1 );
        layers[ layers.size() -1 ].layer_path=inpath;
        layers[ layers.size() -1 ].layer_name=name;
        layers[ layers.size() -1 ].layer_hash.Set(&stoi,0,"");
        return true;
    }
        return false;
}


bool html::find_layer(string name,lyr &liar)
{
    for (int i = 0; i < layers.size(); i++)
    {
        if (layers[i].layer_name==name)
            {

                liar=layers[i];
                return true;
            }
    }
    return false;
}


void
html::finalize_layer(const char *inname)
{
    lyr tmplyr;
    if (find_layer(inname,tmplyr))
        {
            html_hash[inname]=parse_layer( tmplyr.layer_path.c_str(),tmplyr.layer_hash); 
        }
}




HashTable<string,string>
&html::layer(string name)
{
    for (int i = 0; i < layers.size(); i++)
        if (layers[i].layer_name==name)
            return layers[i].layer_hash;
    create_layer("",name);
    return layers[ layers.size()-1 ].layer_hash;

}




#endif
