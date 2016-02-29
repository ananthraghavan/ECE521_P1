/*
 * parse.h
 *
 *  Created on: 28 Sep 2015
 *      Author: Ananth
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "block.h"

void parse_args(int argc, char *argv[]);

void init_cache(int level, int vc);
void display_cache(int level, int vc);

void cache_req(unsigned int level, unsigned int address, char rw);
void cache_write(unsigned int level, unsigned int address);
void update_lru(unsigned int level,unsigned int index, unsigned int col);
void update_vlru(unsigned int level,unsigned int col);


vcresult swap_req(unsigned int level, unsigned int address, unsigned int vic_address, unsigned int vic_dirty);

void file_parse();

addr parse_address(unsigned int level, unsigned int address);
unsigned int unparse_address (unsigned int level, unsigned int index, unsigned int tag);

unsigned int log2 (unsigned int i);

void init_measurements();
void update_measurements(unsigned int level,char rw,char mh);
void display_mments();
void update_measurements_global();




#endif /* PARSE_H_ */
