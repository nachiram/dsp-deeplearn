/*
 * user_config.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Nachiappan
 */

#ifndef DSP_DEEPLEARN_MNIST_USER_CONFIG_H_11
#define DSP_DEEPLEARN_MNIST_USER_CONFIG_H_

#define DSP_FREQ				466			// DSP clock frequency in MHz

#define SHORT_SIZE				2
//#define FUNCTION_PROFILE

#define IMG_WIDTH 				28
#define IMG_HEIGHT				28
#define L1_MAPS					5
#define L2_MAPS					50
#define KERNEL1_SIZE			5
#define KERNEL2_SIZE			5
#define L1_DOWN_SIZE_FACTOR 	2
#define L2_DOWN_SIZE_FACTOR 	2
#define HIDDEN_NEURONS			100
#define OUTPUT_NEURONS			10

#define DDR_REG_BASE 			(0x81000000+0x40000)
#define CACHE_LINESIZE			128
#define DRAM_SIZE				0x01000000


#endif /*DSP_DEEPLEARN_MNIST_USER_CONFIG_H_ */
