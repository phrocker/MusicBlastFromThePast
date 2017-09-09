#ifndef ENVIRONMENTAL_FUNCTIONS_HPP 
#define ENVIRONMENTAL_FUNCTIONS_HPP 
#include <cstdlib>
char 
convertHexToChar( const char* inString ) 
{ 
    //0x4f == 01001111 mask 
    register char digit; 
    digit = (inString[0] >= 'A' ? ((inString[0] & 0x4f) - 'A')+10 : (inString[0] - '0')); 
    digit *= 16; 
    digit += (inString[1] >='A' ? ((inString[1] & 0x4f) - 'A')+10 : (inString[1] - '0')); 
    return digit; 
} 

/* 
iE = inEnvironment 
*/ 
char 
*getEnv(const char *iE) 
{ 
//    cout << "printing environment " << iE << " " << getenv(iE) << endl;
    return (char*)(getenv(iE)!=NULL ? getenv(iE) : ""); 
} 

int url_decode(char *in, char *out, int n) 		/* routine to decode URL into plain text. parameter n indicates the maximum length of the result string. */
{
  static char *pos;	    				/* current position in scanning the input string */
  static char *outpos;	    				/* current position is writing the output string */
  static char build;	    				/* buffer for reconstructing escaped values */
  int length = 1;					/* current length of output string (we will always have a null terminator at the end */

  for(pos=in, outpos=out; (*pos!='\0')&&(length<n); pos++)
    {
      if(*pos=='+') { *outpos=' '; outpos++; }		/* decode + characters into a single space */
      else if(*pos=='&') { *outpos='\n'; outpos++; }	/* decode & characters into a newline character */
      else if(*pos=='%')				/* decode escape sequences... */
	{
	  pos++;					/* look at the first character of the escape sequence... */
	  if(isdigit(*pos)) build=*pos-'0';		/* if the first digit is a decimal number, then set build to the value 0-9 appropriately */
	  else if((*pos>='A')&&(*pos<='F')) build=*pos-'A'+10; /* if the first digit is a letter A-F, then set build to the value 10-15 appropriately */
	  else if((*pos>='a')&&(*pos<='f')) build=*pos-'a'+10; /* same for lowercase a-f */
	  else continue;				/* if not a valid hex digit, then skip this character */
	  pos++; build<<=4;				/* look at the second character of the escape sequence (and update build to indicate that the first digit was in the 16's place)... */
	  if(isdigit(*pos)) build+=*pos-'0';		/* if the first digit is a decimal number, then set build to the value 0-9 appropriately */
	  else if((*pos>='A')&&(*pos<='F')) build+=*pos-'A'+10; /* if the first digit is a letter A-F, then set build to the value 10-15 appropriately */
	  else if((*pos>='a')&&(*pos<='f')) build+=*pos-'a'+10; /* same for lowercase a-f */
	  else { build>>=4; pos--; }			/* if not a valid hex digit, then not to spec, but we will just use the one hex digit as the value */
	  *outpos=build; outpos++;			/* update the output stream to hold the unescaped character */
	}
      else { *outpos=*pos; outpos++; }			/* otherwise, just copy the value to the output string */
    }

  *outpos='\0';						/* terminate the output string */
  return(length);						/* indicate success */
}

int url_encode(char *in, char *out, int n)		/* routine to encode string so that it is safe to be used as a URL. parameter n indicates the maximum length of the result string. */
/* URL encodes the input string and writes it out as the output string. A-Za-z0-9.-_ are outputted without
   modification. Spaces are compacted and converted to a single + character. End-of-line markers are converted
   to a single & character. All other characters are converted to a % character followed by the hexadecimal
   representation of their ASCII value.
*/
{
  const char hexdigits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};        /* lookup table for output hexadecimal text */

  static char *pos;					/* current position in scanning the input string */
  static char *outpos;					/* current position is writing the output string */
  int length = 1;					/* current length of output string (we will always have a null terminator at the end) */
  int totalLength=0;
  for(pos=in, outpos=out; (*pos!='\0')&&(length<n); pos++)
    {
      if((isalnum(*pos))||(*pos=='.')||(*pos=='-')||(*pos=='_')||(*pos<0)) { *outpos=*pos; outpos++; totalLength++; } /* if character is A-Z, a-z, 0-9, ., -, or _, then copy it to the output string unchanged */
      else if(*pos==' ')				/* if is a space, compact any consecutive spaces and output them collectively as a single + character */
	{

	  while(*pos==' ')
      {
          	  *outpos='+'; outpos++; 			/* output the + to represent the space(s) */
           pos++;
		  totalLength++;
          }			/* skip over any consecutive space characters */
	  pos--;
	}
      else if((*pos=='\r')||(*pos=='\n'))		/* if is an end-of-line marker */
	{
	  *outpos='&'; outpos++;      			/* output the & to represent the end-of-line marker */
		totalLength++;
	  while((*pos=='\n')||(*pos=='\r')) pos++;	/* skip over any consecutive newline markers */
	}
      else if(length+3<n)				/* otherwise, we need to url encode the character (if we have enough room remaining in the output string to) */
	{
	  *outpos='%'; outpos++;			/* output the escape character */
		totalLength++;
	  *outpos=hexdigits[*pos>>4]; outpos++;         /* output the hexadecimal representation of the high 4 bits of the ASCII representation of the input character */
	  *outpos=hexdigits[*pos&0x0F]; outpos++;	/* output the hexadecimal representation of the low 4 bits of the ASCII representation of the input character */
	}
    }
  *outpos='\0';						/* terminate the output string */
  return(totalLength);						/* indicate success */
}
string
unescapeString(char *hexstr ) 
{ 
  char *getValue = new char[strlen(hexstr)*2];
  url_decode(hexstr,getValue,strlen(hexstr)+1);
  string returnString = getValue;
  delete [] getValue;
  return returnString;
} 

     
#endif
