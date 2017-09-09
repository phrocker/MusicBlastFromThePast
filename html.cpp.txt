 #ifndef HTML_CPP
#define HTML_CPP
// lR = lastRead
// cS = compare_string
// tV = traverse
html::html()
{
	universal = true;
	destroy = 1;
}

string
html::parse_layer(const char *inpath, bool output) {

	ifstream filestream;
//	filestream.open(inpath, ios::binary );
	filestream.open(inpath);

//	stringstream returnstring;
	string returnstring;
	if (filestream && filestream.good())
	{
		char *tV, *cS,*insert_string, *findtext="$(";
		 // get length of file:
		int filesize=0,lR=0,originalfilesize,lastplace = 0,i;
  		filestream.seekg (0, ios::end);
  		filesize = filestream.tellg();
		originalfilesize = filesize;

		while (filestream.good())
		{
			filestream.seekg (lastplace, ios::beg);
			if (filesize > 300)
			{
				lR = 300;
				lastplace = lastplace+300;
				filesize = filesize-300;

			}
			else
			{

				lR = originalfilesize-lastplace;
				lastplace = originalfilesize;
				filesize=0;
			}
				if (lR <= 0) break;
				tV = new char[lR+1];
				filestream.read(tV,lR);
				tV[lR]='\0';


  				i = find(tV,findtext);
				if (i==-1)
				{
					if (output == false)
						returnstring += tV;
						//returnstring << tV;
					else
						cout << tV;
					continue;
				}
				cout.write(tV,i);
				filestream.seekg ((lastplace+2)-(lR-i), ios::beg);
				delete [] tV;

				if (originalfilesize - ((lastplace+2)-(lR-i)) > 30)
				{
					tV = new char[31];
					filestream.read(tV,30);
					tV[30]='\0';
				}
				else
				{
					tV = new char[ (originalfilesize-( (lastplace+2)-(lR-i))) +1];
					filestream.read(tV, originalfilesize-( (lastplace+2)-(lR-i)) );
					tV[ originalfilesize-( (lastplace+2)-(lR-i)) ]='\0';
				}

				cS = tV;
				while (*cS)
				{
					if (*cS == 41)
					{
						insert_string = new char[ (strlen(tV)-strlen(cS))+1];
						strncpy(insert_string,tV,strlen(tV)-strlen(cS));
						insert_string[(strlen(tV)-strlen(cS))] ='\0';
						// if the value of insert_string is quite large, then it can slow us
						// down some, so just let mvalueOut decide the increments
						if (output == false)
							returnstring += html_layer->return_value(insert_string);
//							returnstring << return_value(insert_string);
						else
							print_value(insert_string);
//						cout << mvalueOf(insert_string);
						delete [] insert_string;
						break;

					}
					cS++;
				}
				lastplace=((lastplace+2)-(lR-i)) + ((strlen(tV)-strlen(cS))+1);
				filesize=((filesize+2)-(lR-i))+ ( (strlen(tV)-strlen(cS))+1);
			delete [] tV;
		}

	}else{
		cout << "<p align=center>" << errorcode[FILE_ERROR] << "on path \"" << inpath << "\"</p>";
	}
	filestream.close();
	if (output == false) return returnstring;
	else return "";
}

int
html::parsePage(const char *path)
{
	return parsePage(path,1);
}

int
html::parsePage(const char *path,int choose_destroy)
{
	destroy=choose_destroy;
	dump_core(path,0);
	return 1; 
}

void
html::resetUniversal()
{
	if (universal)
		universal = false;
	else
		universal = true;
}

const char
*html::htmlhead()
{
	char *ssfullpath;


	ssfullpath = new char[ strlen(program_setup->installation_directory) + strlen(program_setup->html_directory) + strlen(program_setup->universal_html) + strlen(program_setup->domain) + 3];


	sprintf(ssfullpath,"%s%s%s.%s",program_setup->installation_directory,program_setup->html_directory,program_setup->universal_html,program_setup->domain);

	ifstream	mysfilestream(ssfullpath);

	delete [] ssfullpath;

	string temp_string;

	temp_string = "<html><head>
<SCRIPT LANGUAGE=\"JavaScript\">

function ClearMyself() {
opener.parentReload();
}
</SCRIPT>
</head>
<body ";
	if (mysfilestream && mysfilestream.good()) {
		stringstream *my_universal_buffer = new stringstream();
		*my_universal_buffer << mysfilestream.rdbuf() << ends;
		string *findstring = new string();
		*findstring = my_universal_buffer->str();
		delete my_universal_buffer;
		mysfilestream.close();
		int addlen;
		string::size_type location;
		string::size_type second_location;
		int search = 1;
		location = findstring->find("<body",0);
		addlen = strlen("<body");
		if (location == string::npos)
		{
			location = findstring->find("<BODY",0);
			addlen = strlen("<body");
			if (location == string::npos)
			{
				location = findstring->find("< body",0);
				addlen = strlen("< body");
				if (location == string::npos)
				{
					location = findstring->find("< BODY",0);
					addlen = strlen("< body");
					if (location == string::npos)
					{
						search = 0;
					}
				}
			}
		}

		if (search > 0)
		{
			second_location = findstring->find(">",(int)location);
			if (second_location-location <= 255)
			{
				temp_string = temp_string + (string)findstring->substr(location+addlen+1,second_location-(location+addlen));
				delete findstring;
			}
			else
			{
				delete findstring;
				temp_string = temp_string +  "bgcolor=\"#000000\">";
			}
		}
		else
		{
			delete findstring;
			temp_string = temp_string +  "bgcolor=\"#000000\">";
		}

	}else{
		temp_string = (string)temp_string + "bgcolor=\"#000000\">";
	}

	return temp_string.c_str();

}
// 0 means a filepath
// 1 means formatted data
void
html::dump_core(const char *indata, int data_type)
{
	// most likely being called from the command line 
	if (program_setup->template_data->use_template == 0)
	{
		parse_layer(indata);
		return;    
	}  
	if (program_setup->template_data->content_printed == 0){ cout << "Content-type: text/html\n\n"; program_setup->content_printed =1; }
if (universal)
	{
		string sfullpath = program_setup->template_data->installation_directory;
		sfullpath += program_setup->template_data->html_directory;
		sfullpath += program_setup->template_data->universal_html;
		sfullpath += ".";
		sfullpath += program_setup->site;  


	ifstream filestream;
	filestream.open(sfullpath.c_str());

		if (filestream && filestream.good())
		{
			char *tV, *cS,*insert_string, *findtext="$(";
			int filesize=0,lR=0,originalfilesize,lastplace = 0,i;
  			filestream.seekg (0, ios::end);
  			filesize = filestream.tellg();
			originalfilesize = filesize;
			while (filestream.good())
			{
				filestream.seekg (lastplace, ios::beg);
				if (filesize > 300)
				{
					lR = 300;
					lastplace = lastplace+300;
					filesize = filesize-300;

				}
				else
				{

					lR = originalfilesize-lastplace;
					lastplace = originalfilesize;
					filesize=0;
				}
					if (lR <= 0) break;
					tV = new char[lR+1];
					filestream.read(tV,lR);
					tV[lR]='\0';
					i = find(tV,findtext);
					if (i == -1)
					{
						cout << tV;
						continue;
					}
					cout.write(tV,i);
					filestream.seekg ((lastplace+2)-(lR-i), ios::beg);
					delete [] tV;

					if (originalfilesize - ((lastplace+2)-(lR-i)) > 30)
					{
						tV = new char[31];
						filestream.read(tV,30);
						tV[30]='\0';
					}
					else
					{
						tV = new char[ (originalfilesize-( (lastplace+2)-(lR-i))) +1];
						filestream.read(tV, originalfilesize-( (lastplace+2)-(lR-i)) );
						tV[ originalfilesize-( (lastplace+2)-(lR-i)) ]='\0';
					}

					cS = tV;
					while (*cS)
					{
						if (*cS == 41)
						{
							insert_string = new char[ (strlen(tV)-strlen(cS))+1];
							strncpy(insert_string,tV,strlen(tV)-strlen(cS));
							insert_string[(strlen(tV)-strlen(cS))] ='\0';
							if (!strcmp(insert_string,"paste"))
							{
								if (data_type == 1)
									cout << indata;
								else
									parse_layer(indata);
							}
							delete [] insert_string;
							break;

						}
						cS++;
					}
					lastplace=((lastplace+2)-(lR-i)) + ((strlen(tV)-strlen(cS))+1);
					filesize=((filesize+2)-(lR-i))+ ( (strlen(tV)-strlen(cS))+1);
				delete [] tV;
			}
	}else{
			cout << htmlhead();

			if (data_type == 1)
				cout << indata;
			else
				parse_layer(indata);

			cout << "</body></html>";
		}


	}else{
		cout << htmlhead();

		if (data_type == 1)
			cout << indata;
		else
			parse_layer(indata);

		cout << "</body></html>";
	}
}

void
html::htmlerror(char *reportError, int errornum)
{
	if (program_setup->content_printed == 0){ cout << "Content-type: text/html\n\n"; program_setup->content_printed =1; }

	char *filelocation;
	filelocation = new char[strlen(program_setup->installation_directory) + strlen(program_setup->html_directory) + strlen("error.dat.") + strlen(program_setup->default_language) + 3];
	sprintf(filelocation,"%s%serror.dat",program_setup->installation_directory,program_setup->html_directory,program_setup->default_language);
	ifstream	filestream(filelocation);
	string::size_type location;
	// if the file is good...
	if (filestream && filestream.good()) {
			stringstream *universal_html = new stringstream();
			*universal_html << filestream.rdbuf();
			string *universal_html_string = new string();
			*universal_html_string = universal_html->str();
			delete universal_html;
			location = universal_html_string->find("$(errorno)",0);
			if (location != string::npos)
			{
				char *dog = new char[sizeof(errornum) + 1];
				sprintf (dog,"%i",errornum);
				universal_html_string->replace(location,10,dog);
				delete [] dog;
			}
			location = universal_html_string->find("$(error)",location+1);
			if (location != string::npos)
			{
				universal_html_string->replace(location,8,reportError);
			}
			dump_core(universal_html_string->c_str(),1);
	}else{ cout << "<html><body><p align=center>" << errorcode[FILE_ERROR] << "on path \"" << filelocation << "\"</p></body></html>" << endl; }
	delete [] filelocation;
	filestream.close();
}

void
html::create_layer(const char *inpath)
{
	layer.layer_path = inpath;
	layer.layer_array.clear_array();
}

void
html::finalize_layer(const char *inname)
{
	html_value(inname,parse_layer(layer.layer_path) );
}
#endif
