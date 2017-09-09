#ifndef TIME_FUNCTIONS_H
#define TIME_FUNCTIONS_H

#include <iostream>
#include <time.h>
using namespace std;
class time_functions
{
	struct tm *myTime;
	time_t long_time;
	public:
  		int current_hour(long iS =0){ return requested_time(1,iS); }
  		int current_minute(long iS =0){ return requested_time(2,iS); }
		int current_second(long iS =0){ return requested_time(3,iS); }
		int current_date(long iS =0){ return requested_time(4,iS); }
  		int current_month(long iS =0){ return requested_time(5,iS); }
  		int current_year(long iS =0){ return requested_time(6,iS); }
  		
		char *output_time(long iS);
		char *output_time(const char *iS){ return output_time(atol(iS)); }
		long time_stamp();
          const char *gm_time(string);
	private:
		int requested_time(int iP,long iS);
};


// a long integer of the seconds since 1970 
long time_functions::time_stamp()
{
	return (long)time(NULL);
}

char *time_functions::output_time(long iS=0)
{
	if (iS != 0)
	{
		struct tm *returntime;
		returntime = localtime(&iS);
		return asctime(returntime);
	}
	else
	{
		return asctime(myTime);
	}
}

/*
  iP = inParameter, iS = inSeconds, rC = returnConvert
  (1 = hour,2 = min, 3 = sec, 4 = day of month, 5 = mon,
  6 = year,)
*/
int
time_functions::requested_time(int iP,long iS)
{
	struct tm *rC;
	if (iS != 0)
		rC = localtime(&iS);
	switch(iP)
	{
		case 1:
			return (iS != 0) ? rC->tm_hour : myTime->tm_hour;
		case 2:
			return (iS != 0) ? rC->tm_min : myTime->tm_min;
		case 3:
			return (iS != 0) ? rC->tm_sec : myTime->tm_sec;
		case 4:
			return (iS != 0) ? rC->tm_mday : myTime->tm_mday;
		case 5:
			return (iS != 0) ? rC->tm_mon : myTime->tm_mon;
		case 6:
			return (iS != 0) ? rC->tm_year+1900 : myTime->tm_year+1900;
		default:
			return iS; // return it, no matter the value
	}
}

/*
  cS = currentSeconds, aS = addSeconds,
  cT = changeTime, iS = inString,fN=findNum,
  FN = FoundNumber
  (h=hours,m=minutes,s=seconds,d=day
*/
const char
*time_functions::gm_time(string s)
//*gm_time(const char *iS)
{
    const char *iS=s.c_str();
	struct tm *cT;
	time_t long_time;
	long_time = time(NULL);
	cT = gmtime ( &long_time );
	long aS;
	time_t cS = 0;
	char *FN;
	const char *fN;
	fN = iS;
	while ((*fN > 47 && *fN < 58) || *fN==43 || *fN==45) fN++;
	FN = new char[ strlen(iS)-strlen(fN) +1 ];
	strncpy(FN,iS,strlen(iS)-strlen(fN));
	FN[strlen(iS)-strlen(fN)]='\0';
	aS = atol(FN);
	delete [] FN;
	while(*iS)
	{
		if ((*iS >64 && *iS <91) || (*iS> 97 && *iS<122))
		{
			switch(*iS)
			{
				case (109):
					cS += (aS*60);
					break;
				case (83):
				case (115):
					cS += (aS);
					break;
				case (68):
				case (100):
					cS += (aS*86400);
					break;
				case (72):
				case (104):
					cS += (aS*3600);
					break;
				case (77):
					cS += (aS*2678400);
					break;
				case (121):
					cS += (aS*31536000); // close enough. excludes leap year
				default:
					//cS += 0;
					break;
			}
			iS++;
			if (strlen(iS) > 0)
			{
				fN = iS;
				while ((*fN > 47 && *fN < 58) || *fN==43 || *fN==45) fN++;	
				FN = new char[ strlen(iS)-strlen(fN) +1 ];
				strncpy(FN,iS,strlen(iS)-strlen(fN));
				FN[strlen(iS)-strlen(fN)]='\0';
				aS = atol(FN);
				delete [] FN;
				continue;
			}
			else
			{
				break;
			}
		}
		iS++;
	}
	
	long_time=long_time+cS;
	cT = gmtime ( &long_time );
//	return asctime(cT);
	char *dg = asctime(cT);
	dg[strlen(dg)-1]='\0';
	return dg;
}



#endif

