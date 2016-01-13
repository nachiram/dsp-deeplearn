#ifndef _STRUCT_DEFS_H_
#define _STRUCT_DEFS_H_

#include "app_config.h"
#include "stdint.h"

#ifndef MAX
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#endif

//=============================================================
// Various pixel representations
typedef unsigned char PIXEL_U8;
typedef unsigned short PIXEL_U16;
typedef signed char PIXEL_S8;
typedef signed short PIXEL_S16;
typedef float FL_IMG_PIXEL;
typedef float FL_MAP_PIXEL;
typedef float FL_KERNEL;
typedef float FL_FC_WEIGHT;
typedef float FL_FC_BIAS;
typedef PIXEL_S16	FP_IMG_PIXEL;	       // Fixed point input image pixel representation
typedef PIXEL_S16	FP_MAP_PIXEL;	       // Fixed point feature map pixel
typedef signed short 	FP_KERNEL;
typedef signed short 	FP_FC_WEIGHT;
typedef signed short 	FP_FC_BIAS;

//=============================================================
//=============================================================
// Application status
typedef enum {
	FAILED,
	SUCCESS,
	INIT_DONE,
	MALLOC_FAIL,
	SP_MALLOC_FAIL,
	// Add extra status here
	STATUS_INVALID
} APP_STATUS_E;
//=============================================================
//=============================================================
// Convolution mode
typedef enum {
	VS_MUL,				// vector-scalar immediate MUL operation during convolution
	VV_CONCAT_MAPS,		// vector-vector MUL, concatenated map mode
	// Add extra modes here

	INVALID_CONV_MODE
} DNN_CONV_MODE_E;
//=============================================================
//=============================================================
// Map arrangement mode
typedef enum {
	MAP_ISOLATED,	// rows of a map are in continuous memory
	MAP_CONCAT		// Rth row of all maps are in continuous memory.
} MAP_ARRANGE_MODE;
//===============	==============================================
//=============================================================
// Input image / frame buffer information structure.
typedef struct {
	// Pointer to fixed point image buffer
	FP_IMG_PIXEL *pImgBuff;
#ifdef DNN_FLOAT_ARITH
	// Pointer to image buffer
	FL_IMG_PIXEL *pFImgBuff;
#endif // DNN_FLOAT_ARITH
	// Image width
	int width;
	// Image height
	int height;
} IN_IMG_T;
//=============================================================
//=============================================================
// Deep neural net architecture parameter structure
typedef struct {
	// Number of convolution layers
	int nLayers;
	// Array containing no of feature maps in all layers
	int nMaps[DNN_MAX_LAYERS];
	// Array containing kernel size for each layer
	int kSize[DNN_MAX_LAYERS];
	// Array containing no of connections between layer Ln and Ln+1
	int nConn[DNN_MAX_LAYERS - 1];
	// Pointer to kernel connection loopup table
	int *pKconBuff[DNN_MAX_LAYERS - 1];
	// Pointer to map connection loopup table
	int *pMconBuff[DNN_MAX_LAYERS - 1];
	// Width of maps at all layers
	int mapWidth[DNN_MAX_LAYERS];
	// Height of maps at all layers.
	int mapHeight[DNN_MAX_LAYERS];
	// No of layers in FC layer including input layer
	int nFcLayers;
	// No of neurons in each FC layer
	int nFcNeurons[MAX_FC_LAYERS];
} DNN_ARCH_PARAM_T;
//=============================================================
//=============================================================
// Feature map information strucure
typedef struct {
	// Pointer to feature map in fixed point format
	FP_MAP_PIXEL *pMap;
#ifdef DNN_FLOAT_ARITH
	// Pointer to feature map in fixed point format
	FL_MAP_PIXEL *pFMap;
#endif // DNN_FLOAT_ARITH
	// Map width
	int width;
	// Map height
	int height;
	// Number of maps in the container. If > 1 then, maps are in row concatenated format.
	int nMaps;
	// Map arrangement mode
	MAP_ARRANGE_MODE mMode;
} MAP_CONTAINER_T;
//=============================================================
//=============================================================
// Structure for performance counters
typedef struct {
	// Start timestamp in clock cycles
	uint64_t startTime;
	// End timestamp in clock cycles
	uint64_t endTime;

} PERF_CNTR_T;
//=============================================================
//=============================================================
// Convolution kernel containers
typedef struct {
	// Fixed point kernel buffer address
	FP_KERNEL *pKerBuff;
#ifdef DNN_FLOAT_ARITH
	// Floating point kernel buffer address
	FL_KERNEL *pFKerBuff;
#endif // DNN_FLOAT_ARITH
	// Kernel size ; assuming kernel width = height
	int kerSize;
	// Original Kernel buffer size in number of elements
	size_t kerBuffSize;
	// Extended fixed point kernel buffer
	FP_KERNEL **ppExtKerBuff;
	// Size of extended one kernel buffer(corresponding to 1 coeff of each kernel) size in no of elements
	int extKbuffSize;
	// No of such kernel buffers.
	int nKerBuff;
} KERNEL_INFO_T;
//=============================================================
//=============================================================
// Block control structure for split image mode for large images
typedef struct {
	// Buffer for input feature map block.
	FP_MAP_PIXEL *pInBlock[DNN_MAX_LAYERS];
	// buffer for output feature map block.
	FP_MAP_PIXEL *pOutBlock[DNN_MAX_LAYERS];
	// Width of image block with overlap.
	int blkWidth[DNN_MAX_LAYERS];
	// Height of image block with overlap
	int blkHeight[DNN_MAX_LAYERS];
	// Horizontal overlap in no of pixels
	//int hOverlap[DNN_MAX_LAYERS];
	// Vertical overlap in no of pixels
	//int vOverlap[DNN_MAX_LAYERS];
	// max vector length for rectify function.
	int maxVlenRectify;
	// max vector length for max pool operation
	int maxVlenMpool;
} BLK_CTLR_T;
//=============================================================
//=============================================================
// Scratchpad buffer info structure
typedef struct {
	// Pointer to part of allocated scratchpad memory
	void *pSpad;
	// Size of allocated SP in bytes
	size_t spadSize;
} SP_BUFF_T;
//=============================================================
//=============================================================
// Fully connected layer weight structure
typedef struct {
	// Pointer to buffer containing weights.
	FP_FC_WEIGHT *pFcWeight;
//#ifdef DNN_FLOAT_ARITH
	// Pointer to buffer containing floating point weights.
	FL_FC_WEIGHT *pFFcWeight;
//#endif // DNN_FLOAT_ARITH
	// No of input weights per neuron = no of neurons in previous layer
	int weightsPerNeuron;
	// No of neurons in the current layer
	int nNeurons;
	// Pointer to buffer containing bias of network layer
	FP_FC_BIAS *pFcBias;
//#ifdef DNN_FLOAT_ARITH
	FL_FC_BIAS *pFFcBias;
//#endif // DNN_FLOAT_ARITH
} FC_WEIGHTS_T;
//=============================================================
//=============================================================
// Structure containing DNN application context
typedef struct {
	// DNN layer architecture parameters
	DNN_ARCH_PARAM_T dnnArch;
	// Info about input image
	IN_IMG_T inImg;
	// Feature map info
	MAP_CONTAINER_T mapInfo[DNN_MAX_LAYERS];
	// Kernel info
	KERNEL_INFO_T kInfo[DNN_MAX_LAYERS];
	// Image block information for split image mode
	BLK_CTLR_T blkCtlr;
	// FC layer weights and bias
	FC_WEIGHTS_T fcWeightInfo[MAX_FC_LAYERS -1];
	// Perf counters for all layers
	PERF_CNTR_T perfCntr[DNN_MAX_LAYERS];
	// Convolution mode
	DNN_CONV_MODE_E convMode;
} DNN_APP_CTX_T;
#endif // _STRUCT_DEFS_H_
