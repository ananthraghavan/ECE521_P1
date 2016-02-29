/*
 * block.h
 *
 *  Created on: 28 Sep 2015
 *      Author: Ananth
 */

#ifndef BLOCK_H_
#define BLOCK_H_

//#define BDEBUG
//#define RDEBUG
//#define WBDEBUG
//#define GRAPH

typedef struct {
	unsigned int tag;
	unsigned int v;
	unsigned int d;

	unsigned int counter;

}blk;


typedef struct {
	unsigned int block_size;
	unsigned int l1_size;
	unsigned int l1_assoc;
	unsigned int vc_nblks;
	unsigned int l2_size;
	unsigned int l2_assoc;

	char *fname;

}args_st;

typedef struct {
	unsigned int rows;
	unsigned int cols;  //also assoc
	unsigned int size;

	unsigned int bo_bits;
	unsigned int index_bits;
	unsigned int tag_bits;

	unsigned int bo_mask;
	unsigned int index_mask;
	unsigned int tag_mask;

	unsigned int vc_nblks;

	blk *contents;

	blk *vc;
}cache;

typedef struct {
	unsigned int tag;
	unsigned int blk_offset;
	unsigned int index;

}addr;

typedef struct 
{
	unsigned int reads[2];
	unsigned int writes[2];

	unsigned int read_misses[2];
	unsigned int write_misses[2];

	unsigned int sr;
	float srr;

	unsigned int swaps;

	float miss_rate[2];

	unsigned int writebacks[2];

	unsigned int mem_traffic;

}mments;


typedef struct {
	unsigned int success;
	unsigned int dirty;

}vcresult;


#endif /* BLOCK_H_ */
