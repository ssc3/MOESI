

/*This has the exact same implementation as the DM cache except the fact that the struct object is now a 3D object and the tag and index are different. An lru member is added to the structure. lru = 1 => it is MRU. lru = 0 => it is LRU. Also, initially both ways of the cache are checked for the possibility of tag match  */
#include "iostream"
#include "fstream"

using namespace std;

class  cacheline {
	
	public:
	//cacheline ();
	char state;
	unsigned int tag;
	int lru;

}; 
cacheline line[2][4][128];

int m0 = 0, o0 = 0, e0 = 0, s0 = 0, i0 = 0;
int m1 = 0, o1 = 0, e1 = 0, s1 = 0, i1 = 0;
int m2 = 0, o2 = 0, e2 = 0, s2 = 0, i2 = 0;
int m3 = 0, o3 = 0, e3 = 0, s3 = 0, i3 = 0;

int mcount [4];
int ocount [4];
int ecount [4];
int scount [4];
int icount [4];
int error = 0;
int wb[4] = {0, 0, 0, 0};

int initialstatechecker (int proc_id, int index_temp, unsigned int tag_temp)
{
	//cacheline temp[4][10000];
	for (int k=0; k<2; k++)
	{	
	for (int i=0; i<4; i++)
	{
		if (proc_id != i)
		{
			if ((line[k][i][index_temp].tag == tag_temp) && (line[k][i][index_temp].state != 'i')) 
			return (1);
			
			
			
		}
		
	}	
		
	}
	return (0);
	
}
char ownstatechanger (char x, int rw, int istate)
{
	switch (x)
	{
		case 'i':
			if (rw == 0)
			{
				if (istate == 0)
				return ('e');
				else 
				return ('s');
			}
			else
			return ('m');
			break;
		case 'e':
			if (rw == 0)
			return ('e');
			else
			return ('m');
			break;
		case 'm':
			if (rw == 0)
			return ('m');
			else
			return ('m');
			break;
		case 'o':
			if (rw == 0)
			return ('o');
			else
			return ('m');
			break;
		case 's':
			if (rw == 0)
			return ('s');
			else
			return ('m');
			break;
		default:
			error++;
	}
}

char setsnoop (char x, int rw, int istate, int proc_id, int index_temp)
{
	int counter=0;
for (int k=0; k<2; k++)
{
	for (int i=0; i<4; i++)
	{
		if (proc_id != i)
		{
			if(line[k][i][index_temp].state == 's')
			counter++;
			
			if (counter >= 7)
			{
				std::cout << "YYYYY";				
				return ('R');
			}
		}
	}		
}

	switch (x)
	{
		case 'i':
			if (rw == 0)
			return ('R');
			else
			{
			//	if (istate == 1)	
				return ('X');
				
			//	else
			//	return ('O');
			}
			break;
		case 'e':
			if (rw == 0)
			return ('O');
			else
			return ('O');
			break;
		case 'm':
			if (rw == 0)
			return ('O');
			else
			return ('O');
			break;
		case 'o':
			if (rw == 0)
			return ('O');
			else
			return ('U');
			break;
		case 's':
			if (rw == 0)
			return ('O');
			else
			return ('U');
			break;
		default:
			error++;
	}
}

int snoopaction (char snoop, char state)
{
	switch (state)
	{
		case 'i': 
			if (snoop == 'R')
			return (0);			//return (1) = Flush, return (2) = Flush*
			else if (snoop == 'X')
			return (0); 			
			else if (snoop == 'U')
			return (0);
			else if (snoop == 'O')
			return (0);
			break;
		case 'e': 
			if (snoop == 'R')
			return (2);			//return (1) = Flush, return (2) = Flush*
			else if (snoop == 'X')
			return (0); 			
			else if (snoop == 'U')
			return (0);
			else if (snoop == 'O')
			return (0);	
			break;
		case 'm': 
			if (snoop == 'R')
			return (2);			//return (1) = Flush, return (2) = Flush*
			else if (snoop == 'X')
			return (1); 			
			else if (snoop == 'U')
			return (0);
			else if (snoop == 'O')
			return (0);
			break;
		case 'o': 
			if (snoop == 'R')
			return (2);			//return (1) = Flush, return (2) = Flush*
			else if (snoop == 'X')
			return (1); 			
			else if (snoop == 'U')
			return (0);
			else if (snoop == 'O')
			return (0);
			break;
		case 's': 
			if (snoop == 'R')
			return (0);			//return (1) = Flush, return (2) = Flush*
			else if (snoop == 'X')
			return (0); 			
			else if (snoop == 'U')
			return (0);
			else if (snoop == 'O')
			return (0);
			break;
		default:
			error++;
			return (0);

			break;
	}
}				
char changesnoop (char x, char snoop, int comparison)
{
	switch (x)
	{
		case 'i':
			return ('i');
			break;
		case 'e':
			if (snoop == 'R')
			return ('s');
			else if (snoop == 'X')
			return ('i');
			else
			return ('e');
			break;
		case 'm':
			if (snoop == 'R')
			return ('o');
			else if (snoop == 'X')
			return ('i');
			else 
			return ('m');
			break;
		case 'o':
					
			if (snoop == 'R')
			return ('o');
			else if (snoop == 'X')
			return ('i');
			else if (snoop == 'U')
			return ('i');
			else
			return ('o');	
			break;
			
		case 's':
			if (snoop == 'R')
			return ('s');
			else if (snoop == 'X')
			return ('i');
			else if (snoop == 'U')
			return ('i');
			else 
			return ('s');
			break;
		default:
			error++;
		
	}
}

	

void invalidationcounter (char oldstate, int proc_id)
{
	switch (oldstate)
	{
		case 'i':
			if (proc_id == 0)
			i0++;
			else if (proc_id == 1)
			i1++;
			else if (proc_id == 2)
			i2++;
			else
			i3++;
			break;
		case 'e':
			if (proc_id == 0)
			e0++;
			else if (proc_id == 1)
			e1++;
			else if (proc_id == 2)
			e2++;
			else
			e3++;
			break;
		case 'm':
			if (proc_id == 0)
			m0++;
			else if (proc_id == 1)
			m1++;
			else if (proc_id == 2)
			m2++;
			else
			m3++;
			break;

		case 'o':
			if (proc_id == 0)
			o0++;
			else if (proc_id == 1)
			o1++;
			else if (proc_id == 2)
			o2++;
			else
			o3++;
			break;
		case 's':
			if (proc_id == 0)
			s0++;
			else if (proc_id == 1)
			s1++;
			else if (proc_id == 2)
			s2++;
			else
			s3++;
			break;
		default:
			error++;
			break;

	}
}
int main()
{
		
	long timestamp =0;
        long count = 0;
	unsigned int oldtag;
	short pro_id=0;
	short run;
	short way;	
	int snoopsignal[4] = {0, 0, 0, 0};	
	int rw;
	int index[4];
	int action;
	int p[4][4];	
	int istate;
	int comparison=0;

			
	unsigned int tag[4];
	char ownstate;
	char otherstate;
	char newstate;	
	char snoop[4];
	int address;
	int transfer = 0;	//char address[10];
	ifstream myFile[4];
	myFile[0].open("p0.tr", ios::in);
	myFile[1].open("p1.tr", ios::in);
	myFile[2].open("p2.tr", ios::in);	
	myFile[3].open("p3.tr", ios::in);		

for (int k=0; k<2; k++)
{	
	for (int i = 0; i<4; i++)
	{
		for (unsigned int j=0; j<128; j++)
		{
			line[k][i][j].state = 'i';
			line[k][i][j].tag = 0xFFFFFFFF;
			

		}
	}
}
	for (int i = 0; i<4; i++)
	{
		for (unsigned int j=0; j<4; j++)
		{
			p[i][j] = 0;
			
		}
	}

	while (run)
	{
		
		for (int i = 0; i<4; i++)
		{ 
		if (!myFile[i].eof())	
		
		{	
			myFile[i] >> dec>> timestamp;
			myFile[i] >> dec >> rw;
			myFile[i] >> hex >> address;
			tag[i] = address >> 14; 
			index[i] = (address >> 7) & 127;		
			
			pro_id = i;
			if (timestamp != count)
			{
				if (timestamp <1000)
				{
			
					myFile[i].seekg (-16, ios_base::cur);  	
					snoopsignal[i] = 0;        	
				}
				else if ((timestamp >=1000) && (timestamp < 10000))
				{
					myFile[i].seekg (-17, ios_base::cur);
					snoopsignal[i] = 0;    
				}
				else if (timestamp >=10000)
				{
					myFile[i].seekg (-18, ios_base::cur);
					snoopsignal[i] = 0;    
				}
			}			
			else
			{
					/**********Processor Read/Write Logic**************/
				
					
					if (line[0][i][index[i]].tag == tag[i])
					{
						ownstate = line[0][i][index[i]].state;
						way = 0;
						line[0][i][index[i]].lru = 1;
						line[1][i][index[i]].lru = 0;		
					}
					else if (line[1][i][index[i]].tag == tag[i])
					{
						ownstate = line[1][i][index[i]].state;
						way = 1;
						line[1][i][index[i]].lru = 1;
						line[0][i][index[i]].lru = 0;		
					}


					else 
					{
						if (line[0][i][index[i]].lru == 0)
						{
							ownstate = line[0][i][index[i]].state;
							way = 0;
							line[0][i][index[i]].lru = 1;
							line[1][i][index[i]].lru = 0;		
							ownstate = 'i';
						}
						else
						{
							ownstate = line[1][i][index[i]].state;
							way = 1;
							line[1][i][index[i]].lru = 1;
							line[0][i][index[i]].lru = 0;		
							ownstate = 'i';
						}
					}
					if (rw == 1)
					{
											
						if((line[way][i][index[i]].state == 'm') || (line[way][i][index[i]].state == 'o')) 
						{
							if (line[way][i][index[i]].tag != tag[i])
							wb[i]++;
						}
					}
					
					istate = initialstatechecker (pro_id, index[i], tag[i]);
					char ownstate1=ownstate;
					if (line[way][i][index[i]].tag != tag[i]) 
					ownstate = 'i';
					
					newstate = ownstatechanger (ownstate, rw, istate);
					line[way][i][index[i]].state = newstate;		//change state of the line accessed by processor
					line[way][i][index[i]].tag = tag[i];
					snoop[i] = setsnoop (ownstate, rw, istate, pro_id, index[i]);
					transfer = 0;
				
										
					
			
			
					/*************snoop logic*************/
		

			for (int j = 0; j<4; j++)
			{
				for (int k = 0; k < 2; k++)
				{
				
					if ((j != pro_id) || ((j == pro_id) && (k != way)))
					{			
											
						otherstate = line[k][j][index[i]].state;
						action = snoopaction (snoop[i], otherstate);	

						if (action == 1) //Flush
						{
						if((otherstate == 'm') || (otherstate == 'o')) 
						{
							if (line[k][j][index[i]].tag != tag[i])
							wb[j]++;
						}
						}
						else if (action == 2) //Flush*
						{
							line[way][i][index[i]].tag = tag[i];
							if (line[k][j][index[i]].tag == tag[i])
							{
								p[j][i]++;
								
							}
						}
						action = 0;
				 		
						if (line[k][j][index[i]].tag == tag[i])
						line[k][j][index[i]].state = changesnoop(otherstate, snoop[i], comparison);	
						
						if (line[k][j][index[i]].state == 'i')
						invalidationcounter (otherstate, j);

						if ((line[k][j][index[i]].state == 'i') && (snoop[i] == 'X'))
						{
							line[k][j][index[i]].tag = 0xFFFFFFFF;	
						
							if ((k == 0) && (line[1][j][index[i]].lru == 0))		
							{
								line[0][j][index[i]].lru = 0;
								line[1][j][index[i]].lru = 1;
							}
							else if ((k == 1) && (line[0][j][index[i]].lru == 0))
							{
								line[0][j][index[i]].lru = 1;
								line[1][j][index[i]].lru = 0;
							}
						}
	
					
					}


				}
			}						
}
}
}
		
		if ((!myFile[0].eof()) || (!myFile[1].eof()) || (!myFile[2].eof()) || (!myFile[3].eof()))
		run = 1;
		else
		run = 0;
	
		count ++;
		
	}

	
for (int i=0; i < 128; i++)
{
	for (int k=0; k <2; k++)
	{
		for (int j=0; j<4; j++)
		{	
			std::cout << "Line "<< k <<" "<<i<<" "<<j<<" = " <<line[k][j][i].state<<" ";
			std::cout << "Tag "<< k <<" "<<i<<" "<<j<<" = " <<line[k][j][i].tag<<" ";				
		}

}
std::cout << std::endl;
}
for (int k=0; k < 2; k++)
{
	
	for (int i=0; i < 4; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			//std::cout << "State of line" << i<<" " <<j<< " = " << line[i][j].state<<std::endl;		
			if (line[k][i][j].state == 'm')
			{
			mcount[i]++;
			wb[i]++;
			}
			if (line[k][i][j].state == 'o')
			{
			ocount[i]++;
			wb[i]++;
			}
			if (line[k][i][j].state == 'e')
			ecount[i]++;
			if (line[k][i][j].state == 's')
			scount[i]++;
			if (line[k][i][j].state == 'i')
			icount[i]++;				
		} 	
	}
}
	std::cout << "Hi" << std::endl;	
	std::cout << "Error" << error << std::endl;	
	std::cout << "p0p1 p0p2 p0p3 = " << dec << p[0][1] << " " << dec << p[0][2] << " " << dec << p[0][3] << std::endl;
	std::cout << "p1p0 p1p2 p1p3 = " << dec << p[1][0] << " " << dec << p[1][2] << " " << dec << p[1][3] << std::endl;	
	std::cout << "p2p0 p2p1 p2p3 = " << dec << p[2][0] << " " << dec << p[2][1] << " " << dec << p[2][3] << std::endl;
	std::cout << "p3p0 p3p1 p3p2 = " << dec << p[3][0] << " " << dec << p[3][1] << " " << dec << p[3][2] << std::endl;
	std::cout << std::endl;
	std::cout << "P0: m0 o0 e0 s0 i0 = " << dec << m0 <<" "<<o0<<" "<<e0<<" "<<s0<<" "<< 0  << std::endl;	
	std::cout << "P1: m1 o1 e1 s1 i1 = " << dec << m1 <<" "<<o1<<" "<<e1<<" "<<s1<<" "<< 0  << std::endl;
	std::cout << "P2: m2 o2 e2 s2 i2 = " << dec << m2 <<" "<<o2<<" "<<e2<<" "<<s2<<" "<< 0  << std::endl;
	std::cout << "P3: m3 o3 e3 s3 i3 = " << dec << m3 <<" "<<o3<<" "<<e3<<" "<<s3<<" "<< 0  << std::endl;
	std::cout << std::endl;
	std::cout << "P0 WB = " << dec << wb[0]  << std::endl;	
	std::cout << "P1:WB = " << dec << wb[1]  << std::endl;
	std::cout << "P2:WB = " << dec << wb[2]  << std::endl;
	std::cout << "P3:WB = " << dec << wb[3]  << std::endl;	
	std::cout << std::endl;
	std::cout << "P0 Counts: m o e s i =" << dec << mcount[0] <<" "<<ocount[0]<<" "<<ecount [0]<<" "<<scount[0]<<" "<<icount [0]  << std::endl;	
	std::cout << "P1 Counts: m o e s i =" << dec << mcount[1] <<" "<<ocount[1]<<" "<<ecount [1]<<" "<<scount[1]<<" "<<icount [1]  << std::endl;	
	std::cout << "P2 Counts: m o e s i =" << dec << mcount[2] <<" "<<ocount[2]<<" "<<ecount [2]<<" "<<scount[2]<<" "<<icount [2]  << std::endl;		
	std::cout << "P3 Counts: m o e s i =" << dec << mcount[3] <<" "<<ocount[3]<<" "<<ecount [3]<<" "<<scount[3]<<" "<<icount [3]  << std::endl;	
	return 0;
}
