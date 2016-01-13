/*
 * user_config.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Nachiappan
 */

#ifndef DSP_DEEPLEARN_SWEEP_USER_CONFIG_H_
#define DSP_DEEPLEARN_SWEEP_USER_CONFIG_H_

#define DSP_FREQ_IN_MHZ			1000
#define NUM_CORES 				8
#define MSMC_REG_BASE			0x0C000000
#define MSMC_SRAM_SIZE 			4*1024*1024 //in bytes 4MB
#define L2_HEAP_BASE			0x0081C000
#define L2_HEAP_SIZE			400*1024    //400KB scratch space in L2

#define MASTER_CORE_ID			0
#define L1_DOWN_SIZE_FACTOR 	2
#define L2_DOWN_SIZE_FACTOR 	2
#define L1_L2_CONNECTIONS 		10

#define MAX_WIDTH				1024

#define HW_SEM_SYS 				5

#define INIT_DONE	 *(volatile int*)0x80001000
#define L1_SYNC		 *(volatile int*)0x80001004
#define L2_SYNC		 *(volatile int*)0x80001008

#ifdef OPERATE_LAYER_3
#define L1_MAPS 16
#define L2_MAPS 64
#define L3_MAPS 128

#define L1_KERNEL_SIZE 9
#define L2_KERNEL_SIZE 7
#define L3_KERNEL_SIZE 5

#endif

#ifdef CLASSIFIER
#define CLASSIFIER_INDEX 104
#endif

#endif /*DSP_DEEPLEARN_SWEEP_USER_CONFIG_H_ */
