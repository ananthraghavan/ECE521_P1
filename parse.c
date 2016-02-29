/*
 * parse.c
 *
 *  Created on: 28 Sep 2015
 *      Author: Ananth
 */

#include "parse.h"
#include "block.h"

#include <stdlib.h>
#include <stdio.h>


args_st args;
extern cache l[2];

void parse_args (int argc, char *argv[]){

	unsigned int i=0;
	char *ptr;

	args.block_size   =strtol(argv[1],&ptr,10);
	args.l1_size      =strtol(argv[2],&ptr,10);
	args.l1_assoc     =strtol(argv[3],&ptr,10);
	args.vc_nblks     =strtol(argv[4],&ptr,10);
	args.l2_size      =strtol(argv[5],&ptr,10);
	args.l2_assoc     =strtol(argv[6],&ptr,10);
	args.fname        =argv[7];

#ifdef ADEBUG
	if (argc!=8)
	{
		printf("\n\n!!Error!!, need 7 arguments\n\n");
	}

#endif

	//for (i=0;i<argc;i++)
	{
		printf("===== Simulator configuration ====="
				"\n  BLOCKSIZE:              %-d"
				"\n  L1_SIZE:                %-d"
				"\n  L1_ASSOC:               %-d"
				"\n  VC_NUM_BLOCKS:          %-d"
				"\n  L2_SIZE:                %-d"
				"\n  L2_ASSOC:               %-d"
				"\n  trace_file:             %-s"
				,args.block_size,
				args.l1_size,
				args.l1_assoc,
				args.vc_nblks,
				args.l2_size,
				args.l2_assoc,
				args.fname);
	}





}

void file_parse()
{
	FILE *fp;
	char rw;
	unsigned int i=0;

	unsigned int address;


	fp = fopen(args.fname,"r");
#ifdef ADEBUG
	if (fp==NULL)
	{
		printf("\n\n!!Error!!, cannot open file\n\n");
		return;
	}

#endif


	//printf("\nContents of file:\n");

	do {
		fscanf(fp,"%c %8x\n",&rw,&address);
		//printf ("%c    %8x\n",rw,address);
		i++;
#ifdef RDEBUG
		printf("\n----------------------------------------");
		printf("\n# %d: %s %8x",i,((rw=='r')?"read":"write"),address);
#endif

		//if (rw =='r')
		cache_req(1,address,rw);
		//else if(rw == 'w')
			//cache_req(1,address,'w');
			//cache_write(1,address);
#ifdef BDEBUG
		display_cache(1);
#endif

	}while (!feof(fp) ); //( i<31 );


	fclose(fp);

	return;



}

addr parse_address (unsigned int level, unsigned int address)
{

	addr ans;
	unsigned int x;   //array index
    x=level-1;

    ans.blk_offset= address & (l[x].bo_mask);
    ans.index= (address >>  l[x].bo_bits) & (l[x].index_mask);
    ans.tag= (address >>  (l[x].bo_bits +l[x].index_bits)); //& (l[x].tag_mask);
#ifdef XDEBUG
    printf ("\naddress: %x",address);
    printf ("\nblk_offset: %x",ans.blk_offset);
    printf ("\nindex: %x",ans.index);
    printf ("\ntag: %x",ans.tag);
    printf ("\nblk_mask: %x",l[x].bo_mask);
    printf ("\nindex_mask %x",l[x].index_mask);
    printf ("\ntag_mask %x\n",l[x].tag_mask);
#endif



	return ans;

}

unsigned int log2 (unsigned int i)
{
	unsigned int l=0;

	while (i= i>>1)
	{
		++l;
	}
	return l;
}

unsigned int unparse_address (unsigned int level, unsigned int index, unsigned int tag)
{

	unsigned int x;  //cache array index
	x=level-1;

	unsigned int address;

	address = (tag << (l[x].bo_bits +l[x].index_bits)) + (index  << (l[x].bo_bits));

	return address;


}

