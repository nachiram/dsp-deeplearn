/*
 * user_config.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Nachiappan
 */

#ifndef DSP_DEEPLEARN_MNIST_USER_CONFIG_H_11
#define DSP_DEEPLEARN_MNIST_USER_CONFIG_H_

#define SHORT_SIZE				2
#define ARCH1

#ifdef ARCH1
#define IMG_WIDTH 				96
#define IMG_HEIGHT				96
#define L1_MAPS					32
#define L2_MAPS					32
#define KERNELL1_SIZE			5
#define KERNEL2_SIZE			5
#define L1_DOWN_SIZE_FACTOR 	2
#define L2_DOWN_SIZE_FACTOR 	2
#define L1_L2_CONNECTIONS 		10
#define HIDDEN_NEURONS			64
#define OUTPUT_NEURONS			10
#endif

#ifdef ARCH2
#define IMG_WIDTH 				96
#define IMG_HEIGHT				96
#define L1_MAPS					128
#define L2_MAPS					64
#define KERNELL1_SIZE			5
#define KERNEL2_SIZE			5
#define L1_DOWN_SIZE_FACTOR 	2
#define L2_DOWN_SIZE_FACTOR 	2
#define L1_L2_CONNECTIONS 		10
#define HIDDEN_NEURONS			128
#define OUTPUT_NEURONS			10
#endif

#define DDR_REG_BASE 			(0x81000000+0x40000)
#define CACHE_LINESIZE			128
#define DRAM_SIZE				0x01000000


#endif /*DSP_DEEPLEARN_MNIST_USER_CONFIG_H_ */
