#ifndef MYSQL_RESULT_HH
#define MYSQL_RESULT_HH
#include <mysql/mysql.h>
    void
    Result_Set::freeData()
    {
		if (status==RACTIVE)
		{
          	mysql_free_result(sqlData);
		}
        status=FREE;
    }

    bool
    Result_Set::captureSize()
    {
        if (status==RACTIVE)
        {

            numRows=mysql_num_rows(sqlData);
            numColumns=mysql_num_fields(sqlData);
            if (numRows==0) return true;
	    currentRow=0;
	    rows.resize(1);
	    rows[currentRow].resize(numColumns);
	    row = mysql_fetch_row(sqlData);
	    for (int i =0; i< numColumns; i++)
		rows[currentRow][i]=row[i];
            return true;
        }
            return false;
    }
    
    bool
    Result_Set::operator++(int nd)
    {
    	if (this->numRows==0) return false;
    	if (++this->currentRow > this->numRows-1)
    		return false; // we have reached the end
//    	if (currentRow > rows.size()-1) return true; // no need in getting all onery
    
	    rows.resize(rows.size()+1);
    	rows[currentRow].resize(numColumns);
	    row = mysql_fetch_row(sqlData);
        if (!row) return false;
    	for (int i =0; i< numColumns; i++)
	   	rows[currentRow][i]=row[i];
    	return true;
    
    }
    
    bool
    Result_Set::operator--(int nd)
    {
    	if (this->numRows==0) return false;
    	if (--this->currentRow < this->numRows-1)
		return false;
	return true;
    }
    
    
    string
    &Result_Set::operator[](unsigned int currentCol)
    {
//        if (numRows==0) return null;
    	if (currentRow > this->numRows)
            return null;
    	if (currentCol >  this->numColumns-1)
    		return null;
    	return rows[currentRow][currentCol]; // simple return
	
    }
    
    string
   &Result_Set::operator[](string currentCol)
    {
    	// we're really better off using the direct
		// index operator as this involves creating,
		// setting up, and ultimately searching a hash table
    	if (hashActive==false)
		{
			for (int i =0; i< my_query.length(); i++)
			if (my_query[i]>=65 && my_query[i]<=90)
          	      	my_query[i]=char((int)my_query[i]+32);
			columns.Set(&stoi,numColumns,-1);
			string::size_type select=    my_query.find("select ",0);
	        	string::size_type from = my_query.find("from",0);
			string breakaway = my_query.substr(select+6,from-(select+6));
			select=0;
			string temp="";
			string::size_type comma   =    breakaway.find(",",0);
			int currentLoc=0;
			if (numColumns >= columns.AllowableSize())
				columns.Set(&stoi,numColumns,-1);
			while(comma != string::npos)
			{
				temp=breakaway.substr(select,comma-select);
            		select=comma+1;
				columns[ columnSanctify(temp) ] = currentLoc;
				currentLoc++;
          	  	comma   =    breakaway.find(",",select);
        		}
        		temp=breakaway.substr(select,breakaway.length()-1-select);        
        		columns[ columnSanctify(temp) ] = currentLoc;
	   		hashActive=true;

		}
    	if (currentRow > this->numRows-1)
            return null;
    		if (columns[currentCol].GetData() > numColumns - 1)
			 return null;

	    for (int i =0; i< currentCol.length(); i++)
			if (currentCol[i]>=65 && currentCol[i]<=90)
          	      	currentCol[i]=char((int)currentCol[i]+32);
	
		int col=columns[currentCol].GetData();
    		if (col ==-1)
			return null;
		return rows[currentRow][col]; // simple return
	
    }
#endif 
