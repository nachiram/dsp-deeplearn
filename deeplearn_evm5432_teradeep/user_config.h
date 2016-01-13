/*
 * user_config.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Nachiappan
 */

#ifndef DSP_DEEPLEARN_MNIST_USER_CONFIG_H_11
#define DSP_DEEPLEARN_MNIST_USER_CONFIG_H_

#define SHORT_SIZE				2

#define L1_MAPS					16
#define L2_MAPS					64
#define L1_DOWN_SIZE_FACTOR 	4
#define L2_DOWN_SIZE_FACTOR 	4

#define DDR_REG_BASE 			(0x81000000+0x40000)
#define CACHE_LINESIZE			128
#define DRAM_SIZE				0x01000000


#endif /*DSP_DEEPLEARN_MNIST_USER_CONFIG_H_ */
