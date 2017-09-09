#ifndef IP_ADDY
#define IP_ADDY

		class octet
		{
			public:
			octet(char mi=254,char mj=254, char mk=254,char ml=254){ i=mi; j=mj; k=mk; l=ml; }
			unsigned char i; 
			unsigned char j;
			unsigned char k;
			unsigned char l;
			bool operator==(octet &ip)
			{
				if (ip.i==this->i && ip.j==this->j && ip.k==this->k && ip.l==this->l )
					return true;
				else
					return false;
			}
			
			bool operator<(octet &ip)
			{
				if (ip.i<this->i && ip.j<this->j && ip.k<this->k && ip.l<this->l )
					return false;
				else
					return true;
			}
			
			bool operator>(octet &ip)
			{
				if (ip.i>this->i && ip.j>this->j && ip.k>this->k && ip.l>this->l )
					return false;
				else
					return true;
			}

		};



class ip_address
{
	public:
	ip_address(octet *);
	void operator++(int);
	octet &getData();
	private:
		octet *mine;
};

ip_address::ip_address(octet *d)
{
	mine = d;
}

void
ip_address::operator++(int)
{
	mine->l++;
	if (mine->l==0x00)
		mine->k++;
	if (mine->k==0x00)
		mine->j++;
	if (mine->j==0x00)
		mine->i++;
		
}

octet
&ip_address::getData()
{
	return *mine;
}

#endif

