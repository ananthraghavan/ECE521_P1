/*
 * cache.c
 *
 *  Created on: 30 Sep 2015
 *      Author: Ananth
 */

#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "parse.h"

 mments m;

 extern cache l[2];

void init_measurements() {

	unsigned int i;

	for (i=0;i<2;i++){

		m.reads[i] = 0;
		m.read_misses[i] = 0;

		m.writes[i] = 0;
		m.write_misses[i] = 0;

		m.miss_rate[i]=0.0;

		m.writebacks[i]=0;

	}
	
	m.sr=0;
	m.srr=0.0;

	m.swaps=0;

	m.mem_traffic=0;

}

void update_measurements(unsigned int level,char rw,char mh){

	unsigned int x;
	x=level -1;

	switch (rw) {
		case 'r': 
		switch (mh) {
			case 'h': m.reads[x]++;
			break;
			case 'm': m.read_misses[x]++;
			break;
		}
		break;


		case 'w': 
		switch (mh) {
			case 'h': m.writes[x]++;
			break;
			case 'm': m.write_misses[x]++;
			break;
		}
		break;
			
	}

}

void display_mments(){

#ifndef GRAPH	

	printf("\n===== Simulation results ====="
		"\n  a. number of L1 reads:                       %d"
		"\n  b. number of L1 read misses:                 %d"
		"\n  c. number of L1 writes:                      %d"
		"\n  d. number of L1 write misses:                %d"
		"\n  e. number of swap requests:                  %d"
		"\n  f. swap request rate:                        %1.4f"
		"\n  g. number of swaps:                          %d"
		"\n  h. combined L1+VC miss rate:                 %1.4f"
		"\n  i. number writebacks from L1/VC:             %d"
		"\n  j. number of L2 reads:                       %d"
  		"\n  k. number of L2 read misses:                 %d"
		"\n  l. number of L2 writes:                      %d"
  		"\n  m. number of L2 write misses:                %d"
  		"\n  n. L2 miss rate:                             %1.4f"
  		"\n  o. number of writebacks from L2:             %d"
  		"\n  p. total memory traffic:                     %d\n",
		m.reads[0],m.read_misses[0],
		m.writes[0],m.write_misses[0],
		m.sr,m.srr,m.swaps,
		m.miss_rate[0],m.writebacks[0],
		m.reads[1],m.read_misses[1],
		m.writes[1],m.write_misses[1],
		m.miss_rate[1],m.writebacks[1],
		m.mem_traffic);
#endif


#ifdef GRAPH
	printf("\n f. swap request rate:                        %1.4f\n",m.srr);
	printf("\n h. combined L1+VC miss rate:                 %1.4f\n",m.miss_rate[0]);
	printf("\n n.  L2 miss rate:                 %1.4f\n",m.miss_rate[1]);

#endif

}

void update_measurements_global(){

	if ((m.reads[0] + m.writes[0] !=0)){
		m.miss_rate[0]= (float) (m.read_misses[0] + m.write_misses[0] -m.swaps)
					 / (m.reads[0] + m.writes[0]);

		m.srr = (float) (m.sr)
					 / (m.reads[0] + m.writes[0]);
	}
		

		
	if ((m.reads[1] !=0))
		m.miss_rate[1]= (float) (m.read_misses[1])
					 / (m.reads[1]);


	



}

