#include "iostream"
#include "fstream"

using namespace std;

struct cacheline {
	
	
	char state;
	unsigned int tag;
	

}; 
cacheline line[4][256];

int error = 0;
int m0 = 0, o0 = 0, e0 = 0, s0 = 0, i0 = 0;
int m1 = 0, o1 = 0, e1 = 0, s1 = 0, i1 = 0;
int m2 = 0, o2 = 0, e2 = 0, s2 = 0, i2 = 0;
int m3 = 0, o3 = 0, e3 = 0, s3 = 0, i3 = 0;

int mcount [4];
int ocount [4];
int ecount [4];
int scount [4];
int icount [4];

long wb[4] = {0, 0, 0, 0};


// This Function is used to check if any other cache with same index has the same tag
int initialstatechecker (int proc_id, int index_temp, unsigned int tag_temp)
{
	//cacheline temp[4][10000];
	for (int i=0; i<4; i++)
	{
		if (proc_id != i)
		{
			if ((line[i][index_temp].tag == tag_temp) && (line[i][index_temp].state != 'i')) 
			return (1);
			
			
			
		}
		
		
		
	}
	return (0);
	
}

//This function changes the state of the line whose processor is issuing the read.write request 
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

//This function sets the snoop signal for current state and hit/miss of the line
char setsnoop (char x, int rw, int istate, int proc_id, int index_temp)
{
	int counter=0;
	for (int i=0; i<4; i++)
	{
		//if (proc_id != i)
		//{
			if(line[i][index_temp].state == 's')
			counter++;
			
			if (counter >= 3)
			{
				std::cout << "YYYYY";				
				return ('R');
			}
		//}
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

//This function dictates what a line should do during a snoop, based on its own state and snoop signal
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

//This function changes the state of the line based on the snoop signal
char changesnoop (char x, char snoop)
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
			else if (snoop == 'U')
			return ('i');
			else
			return ('e');
			break;
		case 'm':
			if (snoop == 'R')
			return ('o');
			else if (snoop == 'X')
			return ('i');
			else if (snoop == 'U')
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

	
//Invalidations counter function
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
		
	long timestamp =0;	//Used to store the timestamp from each file
        long count = 0;	//the global clock  				
	short pro_id=0; //stores the processor id
	short run; // the while loop condition variable
	int snoopsignal[4] = {0, 0, 0, 0};	
	int rw; //read/write status storage
	int index[4]; // index storage
	int action; // store the snoopacction
	int p[4][4];	//cache to cache transfers
	int istate;  //initial state checking variable


			
	unsigned int tag[4]; //tag storage
	char ownstate; 
	char otherstate;
	char newstate;	
	char snoop[4];
	int address;//char address[10];
	ifstream myFile[4];
	myFile[0].open("p0.tr", ios::in);
	myFile[1].open("p1.tr", ios::in);
	myFile[2].open("p2.tr", ios::in);	
	myFile[3].open("p3.tr", ios::in);		


//Initializing all lines to 'i' state and storing tag = 0xFFFFFFF just as an indicator in them
	for (int i = 0; i<4; i++)
	{
		for (unsigned int j=0; j<256; j++)
		{
			line[i][j].state = 'i';
			line[i][j].tag = 0xFFFFFFFF;
	
		}
	}
//Initializing counter to 0
	for (int i = 0; i<4; i++)
	{
		for (unsigned int j=0; j<4; j++)
		{
			p[i][j] = 0;
			
		}
	}
//Main while loop with global counter
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
			index[i] = (address >> 6) & 255;
			
			pro_id = i;
			if (timestamp != count)					//3 different conditions for different lenght lines of the trace files
			{
				if (timestamp <1000)
				{
			
					myFile[i].seekg (-16, ios_base::cur);  	
				        	
				}
				else if ((timestamp >=1000) && (timestamp < 10000))
				{
					myFile[i].seekg (-17, ios_base::cur);
				  
				}
				else if (timestamp >=10000)
				{
					myFile[i].seekg (-18, ios_base::cur);
					  
				}
			}			
			else
			{
					/**********Processor Read/Write Logic**************/
				
				if((line[i][index[i]].state == 'm') || (line[i][index[i]].state == 'o')) 
				{
					if (line[i][index[i]].tag != tag[i])
					wb[i]++;                                //writeback counter due to tag conflicts
				}
					
				ownstate = line[i][index[i]].state; // store ownstate in a variable
				istate = initialstatechecker (pro_id, index[i], tag[i]); //check if any other line with same index has the same tag
				if (line[i][index[i]].tag != tag[i]) 
				ownstate = 'i';                    // invalidate the line if there is a tag conflict
				newstate = ownstatechanger (ownstate, rw, istate); // store new state of the line in a variable
				snoop[i] = setsnoop (ownstate, rw, istate, pro_id, index[i]); //set the snoop signal based on ownstate and rw signal from processor
				line[i][index[i]].state = newstate;
				line[i][index[i]].tag = tag[i];

					
						/*************snoop logic*************/
		
					
			
			//Line 1

			for (int j = 0; j<4; j++)
			{
				
				if (j != pro_id)
				{		
										
									
					otherstate = line[j][index[i]].state;
					action = snoopaction (snoop[i], otherstate);//determines what action needs to be taken for different snoop signals
		
				
			
					if ((action == 1)) //Flush
					{
						if((otherstate == 'm') || (otherstate == 'o')) 
						{
							if (line[j][index[i]].tag != tag[i])
							wb[j]++;
						}

					}
					if (action == 2) //Flush*
					{
					
						if ((line[j][index[i]].tag == tag[i]))
						p[j][i]++;
					

					}		
					
					
					if ((line[j][index[i]].tag == tag[i]))
					line[j][index[i]].state = changesnoop(otherstate, snoop[i]); //change the state of the line based on the snoop signal
										
				
						
				
					if (line[j][index[i]].state == 'i')
					invalidationcounter (otherstate, j);	//invalidation counter
					
					
					if (line[j][index[i]].state == 'i')
					line[j][index[i]].tag = 0xFFFFFFFF;
					
					
					
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


for (int i=0; i < 256; i++)
{
	for (int j=0; j<4; j++)
	std::cout << "Line "<<j<<" "<<i<<" = " <<line[j][i].state<<" ";

	std::cout << std::endl;
}
	

	for (int i=0; i < 4; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			//std::cout << "State of line" << i<<" " <<j<< " = " << line[i][j].state<<std::endl;		
			if (line[i][j].state == 'm')
			{	
			mcount[i]++;
			wb[i]++;
			}
			else if (line[i][j].state == 'o')
			{
			ocount[i]++;
			wb[i]++;
			}	
			else if (line[i][j].state == 'e')
			ecount[i]++;
			else if (line[i][j].state == 's')
			scount[i]++;
			else if (line[i][j].state == 'i')
			icount[i]++;				
		} 	
	}



	std::cout << "Hi" << std::endl;	
	std::cout << "Error = " <<error << std::endl;	
	std::cout << "p0p1 p0p2 p0p3 = " << dec << p[0][1] << " " << dec << p[0][2] << " " << dec << p[0][3] << std::endl;
	std::cout << "p1p0 p1p2 p1p3 = " << dec << p[1][0] << " " << dec << p[1][2] << " " << dec << p[1][3] << std::endl;	
	std::cout << "p2p0 p2p1 p2p3 = " << dec << p[2][0] << " " << dec << p[2][1] << " " << dec << p[2][3] << std::endl;
	std::cout << "p3p0 p3p1 p3p2 = " << dec << p[3][0] << " " << dec << p[3][1] << " " << dec << p[3][2] << std::endl;
	std::cout << std::endl;
	std::cout << "P0: m0 o0 e0 s0 i0 = " << dec << m0 <<" "<<o0<<" "<<e0<<" "<<s0<<" "<< 0 << std::endl;	
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
