#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "parse.h"


extern cache l[2];
extern mments m;



vcresult swap_req(unsigned int level, unsigned int address, unsigned int vic_address, unsigned int vic_dirty) {


	unsigned int x;  //cache array index
	x=level-1;
	
	vcresult v;
	v.success = 0;
	v.dirty = 0;
	
	unsigned int i;
	unsigned int nblks=l[x].vc_nblks;
	//local vars for speed and convenience


	unsigned int evict=1;
	unsigned int evict_address;

	unsigned int xaddr;
	xaddr= address >> l[x].bo_bits; 


	unsigned int vaddr;
	vaddr= vic_address >> l[x].bo_bits;

	m.sr++;

	address= address >> l[x].bo_bits;
	address= address << l[x].bo_bits;


#ifdef RDEBUG
	 printf("\n VC swap req: [%x, %x]",address,vic_address);
#endif	

	 for (i=0;i<nblks;i++){

	 	//check for valid bit
	 	if (l[x].vc[i].v) {

	 		if (xaddr == l[x].vc[i].tag){

	 			//this is a hit or a swap
	 			m.swaps++;
#ifdef RDEBUG
				printf("\nVC hit: %x (%s)",address, 
					((l[x].vc[i].d==1)?"dirty":"clean"));
				printf("\nVC update LRU");
#endif	
				update_vlru(level,i);

				v.success = 1;
				v.dirty   = l[x].vc[i].d;

				l[x].vc[i].tag =vaddr;
				l[x].vc[i].d   =vic_dirty;

				return v;

	 		}
	 	}
	 }

	 //flow will reach here if there is a miss

#ifdef RDEBUG
	printf("\nVC miss");
#endif

	//check if there is an invalid block

	for (i=0;i<nblks;i++) {

		//check for valid bit
		if (!l[x].vc[i].v) {

			// found space
			evict =0;

#ifdef RDEBUG
			printf("\nVC victim: none");
#endif				
			break;

		}
	}

	if (evict==1) {


		//must evict lru

		for (i=0;i<nblks;i++) {

			if (l[x].vc[i].counter==nblks){

				evict_address = l[x].vc[i].tag << l[x].bo_bits;

#ifdef RDEBUG
				printf("\nVC victim: %x (tag %x, index %d, %s)"
					,evict_address,l[x].vc[i].tag,
					0,((l[x].vc[i].d==1)?"dirty":"clean"));
#endif

				if (l[x].vc[i].d) {
					//THIS IS A WRITEBACK

					m.writebacks[x]++;

					cache_req(level+1,evict_address,'w');

				}

				break;

			}

		}

	}


	l[x].vc[i].v=1;
	update_vlru(level,i);

#ifdef RDEBUG
	printf("\nVC update LRU");
#endif

	l[x].vc[i].d=vic_dirty;

	l[x].vc[i].tag=vaddr;

	return v;

}


