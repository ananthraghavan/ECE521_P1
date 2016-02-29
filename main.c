#include <stdio.h>


#include "block.h"
#include "parse.h"

extern args_st args;
extern cache l[2];

int main(int argc, char *argv[]) {

	init_measurements();

	parse_args(argc,argv);

	init_cache(1,args.vc_nblks);

	//if (args.l2_size)
		init_cache(2,0);

	init_measurements();
	

	// addr x;
	// x=parse_address(1,0x400341a1);

	// printf ("\n Parse Address\n"
	// 		"Tag:     %8x\n"
	// 		"Index:   %8x\n"
	// 		"BO:      %8x\n"
	// 		,x.tag,x.index,x.blk_offset);

	// unsigned int unp;

	// unp= unparse_address(1,x.index,x.tag);
	// printf("\n\n Unparsed Address:     %8x\n",unp);
	
#ifdef ADEBUG
			printf ("\n\n index 240  %x %d",l[0].contents[240].tag,l[0].contents[240].v);	
#endif

	file_parse();
#ifndef GRAPH
	display_cache(1,args.vc_nblks);

	if (args.l2_size)
		display_cache(2,0);
#endif
	update_measurements_global();

	display_mments();

	return 0;
}
