/*
 * cache.c
 *
 *  Created on: 29 Sep 2015
 *      Author: Ananth
 */

#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "parse.h"

extern args_st args;
cache l[2];


void init_cache(int level, int vc){


	unsigned int x;  //array index
	x=level-1;
	unsigned int i=0;
	unsigned int j=0;


	if (level == 1)
	{
		l[x].rows=        (args.l1_size==0)?0:((args.l1_size) / (args.l1_assoc * args.block_size));
		l[x].cols=        (args.l1_assoc);
		l[x].size=        (args.l1_size);
		l[x].vc_nblks=    (args.vc_nblks);

	}
	if (level == 2)
	{
		l[x].rows=        (args.l2_size==0)?0:((args.l2_size) / (args.l2_assoc * args.block_size));
		l[x].cols=        (args.l2_assoc);
		l[x].size=        (args.l2_size);
		l[x].vc_nblks=    0;

	}

	l[x].bo_bits=log2(args.block_size);
	l[x].index_bits=log2(l[x].rows);
	l[x].tag_bits=32- (l[x].bo_bits+ l[x].index_bits);

	l[x].bo_mask= (1<<l[x].bo_bits)-1;
	l[x].index_mask= (1<<l[x].index_bits)-1;
	l[x].tag_mask= (1<<l[x].tag_bits)-1;

#ifdef XDEBUG
	printf("\nL%d Config\n"
			"BO Bits:       %d\n"
			"Index Bits:    %d\n"
			"Tag Bits:      %d\n"
			"Index Mask     %d\n",
			level,l[x].bo_bits,l[x].index_bits,l[x].tag_bits,l[x].index_mask);
#endif


	l[x].contents= malloc (l[x].rows * l[x].cols * sizeof(blk));
	l[x].vc=       malloc (l[x].vc_nblks * sizeof(blk));  

#ifdef ADEBUG
	if (l[x].contents == NULL)
		printf("\n\n !!ERROR!!\nMalloc failed!\n");
	if (l[x].vc == NULL)
		printf("\n\n !!ERROR!!\nMalloc failed!\n");
#endif

	// Initializing contents of LX Cache//

	for (i=0; i<l[x].rows; i++){
		for (j=0; j<l[x].cols; j++){

			l[x].contents[j+i*l[x].cols].d=0;
			l[x].contents[j+i*l[x].cols].v=0;
			l[x].contents[j+i*l[x].cols].counter=j+1;
			l[x].contents[j+i*l[x].cols].tag=0;


		}
	}

	//Initializing contents of LX Cache victim cache
	if (vc) {
		for (i=0; i<l[x].vc_nblks; i++){
			l[x].vc[i].d=0;
			l[x].vc[i].v=0;
			l[x].vc[i].counter=i+1;
			l[x].vc[i].tag=0;
		}

	}
	

}

void display_cache(int level, int vc){

	unsigned int x;   //array index
	x=level-1;
	unsigned int i=0;
	unsigned int j=0;
	unsigned int k=0;
	char dirty;

	printf ("\n\n===== L%d contents =====",level);

	// blk *order;
	// order = malloc (l[x].rows.cols * sizeof(blk));

	// for (j=0; j<l[x].cols; j++){
	// 	order.d=0;
	// 	order.v=0;
	// 	order.counter=j+1;
	// 	order.tag=0;
	// }

	for (i=0; i<l[x].rows; i++){
		printf ("\n  set   %d: ",i);
		for (k=1;k<=l[x].cols;k++){
			for (j=0; j<l[x].cols; j++){
				if (l[x].contents[j+i*l[x].cols].counter==k){
					dirty = l[x].contents[j+i*l[x].cols].d==0 ? ' ':'D';
					printf ("   %x %c",l[x].contents[j+i*l[x].cols].tag,dirty);
#ifdef CDEBUG
					printf(" %d",l[x].contents[j+i*l[x].cols].counter);
#endif
					break;
				}
				
			}
		}
	}
	printf("\n");


	if (vc){


		printf ("\n\n===== VC contents =====");
		printf ("\n  set   0: ");
			for (k=1;k<=l[x].vc_nblks;k++){
				for (i=0;i<l[x].vc_nblks;i++){
					if (l[x].vc[i].counter==k){
						dirty = l[x].vc[i].d==0 ? ' ':'D';
						printf ("   %x %c",l[x].vc[i].tag,dirty);
#ifdef CDEBUG
						printf(" %d",l[x].vc[i].counter);
#endif
						break;

						}

					

				}

			}
	

		printf("\n");
	}
	// free(order);

}



void update_lru(unsigned int level,unsigned int index, unsigned int col){

	unsigned int x;
	x=level-1;

	unsigned int j=0;
	unsigned int sets=0;   //local vars for speed and convenience
	unsigned int assoc=0;

	unsigned int mru;

	sets = l[x].rows;
	assoc= l[x].cols;

	mru = l[x].contents[index+col].counter;
	

	for (j=0;j<assoc;j++){
		if (l[x].contents[index+j].counter<mru)
			l[x].contents[index+j].counter++;
	}


	l[x].contents[index+col].counter=1;
}

void update_vlru(unsigned int level,unsigned int col){

	unsigned int x;
	x=level-1;

	unsigned int i;
	unsigned int nblks=l[x].vc_nblks;
	//local vars for speed and convenience

	unsigned int mru;

	mru = l[x].vc[col].counter;
	

	for (i=0;i<nblks;i++){
		if (l[x].vc[i].counter<mru)
			l[x].vc[i].counter++;
	}


	l[x].vc[col].counter=1;
}