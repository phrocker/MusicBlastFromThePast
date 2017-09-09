#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#include "gd.h"
#define USE_MYSQL 1
#include "dbase.h"
#define DATFILE "/home/phrocker/data_files/general.dat"
#include "cgiparser.h"
int main()
{
	gdImagePtr im;
	int black;
	int white;
	int brect[8];
	int x, y;
	char *err;
	char *s="NOT AUTHORIZED";
	cgiparser cgi_utilities;
 	retreive_values variables;
	bool isGood=false;
    variables.parse_file(DATFILE);	
	if (cgi_utilities.cgivalue("rep")!="")
		{
			
			
			DB_Handler database;
			database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
			
			Result_Set results;
			Query query;
			
			query << "select sid from `keys` where `kid`='" << cgi_utilities.cgivalue("rep") << "'";			
			
			if (database.execute_query(query,results))
			{
				if (results.number_rows() > 0)
				{
					
					int i=0;
					string result = results[0];
					s = new char[ (result.length()*2)+1 ];
					for (int j=0; i < result.length()*2; i++)
					{
						if (i%2 == 0)
							s[i] = ' ';
						else
						{
							s[i] = results[0].at(j);
							j++;
						}
					}
					s[i] = '\0';
					isGood = true;
				}
			}
			//strcpy(s,cgi_utilities.cgivalue("rep").c_str());
		}
	//char *s = "abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890"; /* String to draw. */
	double sz = 20.;
	char *f = "/home/marc/Bring_tha_noize.ttf"; /* fontconfig pattern */
	/* obtain brect so that we can size the image */
	err = gdImageStringFT(NULL,&brect[0],0,f,sz,0.,0,0,s);
	if (err) {
		ifstream error("/home/phrocker/public_html/images/na.png", ios::binary );
		cout << "Content-type: image/png\n\n";
		while(error)
			cout << error;
		error.close();
		return 0;
}
	
/* create an image big enough for the string plus a little whitespace */
	x = brect[2]-brect[6] + 6;
	y = brect[3]-brect[7] + 6;
	im = gdImageCreate(x,y);

/* Background color (first allocated) */
	white = gdImageColorResolve(im, 97, 101,247);
	black = gdImageColorResolve(im, 255, 255, 255);

	/* render the string, offset origin to center string*/
	/* note that we use top-left coordinate for adjustment
 	* since gd origin is in top-left with y increasing downwards. */
	x = 3 - brect[6];
	y = 3 - brect[7];
	err = gdImageStringFT(im,&brect[0],black,f,sz,0.0,x,y,s);
	if (err) {
		ifstream error("/home/phrocker/public_html/images/na.png", ios::binary );
		cout << "Content-type: image/png\n\n";
		while(error)
			cout << error;
		error.close();
//fprintf(stderr,err); return 1;

	}else
{
	/* Write img to stdout */
	//gdImageJpeg(im, jpegout,-1);
	//cout << "Content-type: text/html\n\n";
	//cout << "Hello <br />";
	cout << "Content-type: image/png\n\n";
	//gdImageJpeg(im,stdout);
	gdImagePng(im, stdout);
	/* Destroy it */
	
	gdImageDestroy(im);
	if (isGood==true)
		delete [] s;
}
	return 0;
}
