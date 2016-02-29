#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "parse.h"


extern cache l[2];
extern mments m;



void cache_req(unsigned int level, unsigned int address, char rw) {

	unsigned int x;  //cache array index
	x=level-1;

	if (level==3)
	{
		m.mem_traffic++;
		return;

	}

	unsigned int index;   //1D array index

	unsigned int j=0;
	unsigned int sets=0;   //local vars for speed and convenience
	unsigned int assoc=0;

	unsigned int hit=0;
	unsigned int evict=1;
	unsigned int evict_address;

	vcresult v;

	v.success=0;

	sets = l[x].rows;
	assoc= l[x].cols;

	if (sets==0)
	{
		// go down the hierarchy if no cache present
		cache_req(level+1,address,'r');
		return;
	}

	addr p;
	p=parse_address(level,address);


	index = (p.index) * (assoc);

	unsigned int address_m;
	address_m =address >> l[x].bo_bits;
	address_m =address_m << l[x].bo_bits;
#ifdef RDEBUG
	 printf("\nL%d %s: %x (tag %x, index %d)",level,((rw=='r')?"read":"write"),address_m,p.tag,p.index);
#endif		


// checking for address in cache
	for (j=0;j<assoc;j++){

		//check for valid bit
		if (l[x].contents[index+j].v){


			if (p.tag == l[x].contents[index+j].tag){

				// this is a hit

				update_measurements(level,rw,'h');
#ifdef RDEBUG
				printf("\nL%d hit",level);
				printf("\nL%d update LRU",level);
#endif	
				update_lru(level,index,j);
				l[x].contents[index+j].d= (rw=='w')?1:l[x].contents[index+j].d;				
#ifdef RDEBUG
				if (rw=='w')
				printf("\nL%d set dirty",level);
#endif
				hit=1;
				return;

			}
		}
	}


	//flow will reach here if there is a miss


	update_measurements(level,rw,'m');
#ifdef RDEBUG
	printf("\nL%d miss",level);
#endif


	//check if there is an invalid block

	for (j=0;j<assoc;j++){

		//check for valid bit
		if (!l[x].contents[index+j].v){

			//found space
			evict=0;
#ifdef RDEBUG
			printf("\nL%d victim: none",level);
#endif				
			break;
		}
	}	


	if (evict==1){
		
		

		//must evict lru
		for (j=0;j<assoc;j++) {
			
			if (l[x].contents[index+j].counter==assoc){
				

				evict_address = unparse_address (level,p.index,l[x].contents[index+j].tag);


#ifdef RDEBUG
				printf("\nL%d victim: %x (tag %x, index %d, %s)",level
					,evict_address,l[x].contents[index+j].tag,
					p.index,((l[x].contents[index+j].d==1)?"dirty":"clean"));
#endif			

				// VC CODE COMES HERE
				if (l[x].vc_nblks)
					v = swap_req(level, address, evict_address, l[x].contents[index+j].d);


				if ( (l[x].contents[index+j].d) && (!l[x].vc_nblks) )
				{
					//THIS IS A WRITEBACK

					//printf("\nERROR THIS SHOULD NOT PRINT IF THERE IS A VICTIM CACHE\n");

					m.writebacks[x]++;
					//evict_address = unparse_address (level,p.index,l[x].contents[index+j].tag);

					cache_req(level+1,evict_address,'w');
				}

				break;
			}
		}


	}

	if(!v.success)
		cache_req(level+1,address,'r');
		


	l[x].contents[index+j].v = 1;	
	update_lru(level,index,j);

#ifdef RDEBUG
	printf("\nL%d update LRU",level);
#endif


	if(v.success)
		l[x].contents[index+j].d= v.dirty;
	else
		l[x].contents[index+j].d= (rw=='w')?1:0;

	l[x].contents[index+j].d= (rw=='w')?1:l[x].contents[index+j].d;

#ifdef RDEBUG
	if (rw == 'w')
		printf("\nL%d set dirty",level);
#endif

	l[x].contents[index+j].tag=	p.tag;
	update_measurements(level,rw,'h');

	return;


}