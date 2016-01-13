/*
 * deeplearning.h
 *
 *  Created on: Jan 7, 2016
 *      Author: nachi
 */

#ifndef CALTECH_CPU_DEEPLEARNING_H_
#define CALTECH_CPU_DEEPLEARNING_H_

#include "struct_defs.h"

/** \brief API to rectify the feature maps
*
* @param[in] pFeatureMap	Address of the feature map container
* @return	status flag
*/
APP_STATUS_E scalar_fix_rectify(MAP_CONTAINER_T *pFeatureMap);

/** \brief Max pooling of feature maps. Output map size will be reduced by factor 2 in both dimensions.
*
* @param[in]  pImap		Address of input feature map container
* @param[out] pOmap		Address of the ouput map container
* @return	  			status flag
*/
APP_STATUS_E scalar_fix_max_pool(MAP_CONTAINER_T *pImap, MAP_CONTAINER_T *pOmap);

APP_STATUS_E scalar_fix_subsample(MAP_CONTAINER_T *pImap, MAP_CONTAINER_T *pOmap);


APP_STATUS_E scalar_fix_conv2D(FP_IMG_PIXEL *pImg, FP_KERNEL *pKer, int H, int W, int kRows, int kCols, int scale, FP_MAP_PIXEL *pMap);

APP_STATUS_E scalar_fix_L2_maps(MAP_CONTAINER_T *pInMaps, KERNEL_INFO_T *pKer,
    int *pMapConn, int *pKerConn, int nConn, MAP_CONTAINER_T *pOutMaps);

APP_STATUS_E scalar_fix_L2_maps_v2(MAP_CONTAINER_T *pInMaps, KERNEL_INFO_T *pKer,
    int *pMapConn, int nConn, MAP_CONTAINER_T *pOutMaps);

APP_STATUS_E scalar_fix_L2_maps_v2_all2all(MAP_CONTAINER_T *pInMaps, KERNEL_INFO_T *pKer,
    MAP_CONTAINER_T *pOutMaps);

APP_STATUS_E scalar_fix_L1_maps(IN_IMG_T *pImg, KERNEL_INFO_T *pKer, MAP_CONTAINER_T *pOutMap);

#endif /* CALTECH_CPU_DEEPLEARNING_H_ */
