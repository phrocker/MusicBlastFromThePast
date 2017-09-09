#ifndef UTILS_H
#define UTILS_H

int getchr( ) {
struct termios oldt,
newt;
int ch;
tcgetattr( STDIN_FILENO, &oldt );
newt = oldt;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );
ch = getchar();
tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
return ch;
}


string getpass()
{
    char chr=getchar();
    chr=32;
    string pass;
    while((chr = getchr())>=32)
        pass+=chr;
    return pass;
}    

#endif
