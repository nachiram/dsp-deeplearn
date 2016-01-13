#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// Deep learning application configuration parameters.
#define DNN_MAX_LAYERS		(2)	// max feature extraction convolution layers
#define IN_IMG_WIDTH		(150)	// input image width
#define IN_IMG_HEIGHT		(150)	// input image height
#define NO_L1_MAPS		(32)	// no of layer 1 feature maps
#define NO_L2_MAPS		(256)	// no of layer 2 feature maps
#define L1_L2_CONN		(10)	// no of connections from L1 to L2
#define K1_SIZE			(9)	// layer 1 kernel size
#define K2_SIZE			(9)	// layer 2 kernel size
#define IMG_FRAC_BITS		(15)	// no of fraction bits in the input fixed point image
#define KERNEL_FRAC_BITS	(13)	// no of fraction bits in the fixed point kernel coefficients
#define MAX_FC_LAYERS		(3)	// no of fully connected layers in the last stage (classifier) including input layer.
#define NO_INPUT_NEURONS	(1152)
#define NO_HIDDEN_NEURONS	(8500)
#define NO_OUTPUT_NEURONS	(10)

#endif // _APP_CONFIG_H_
