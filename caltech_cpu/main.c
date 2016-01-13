/*
 * main.c
 *
 *  Created on: Jan 7, 2016
 *      Author: nachi
 */
#include"string.h"
#include"stdio.h"
#include"stdlib.h"
#include"assert.h"
#include"math.h"
#include"struct_defs.h"

APP_STATUS_E scalar_fix_rectify(MAP_CONTAINER_T *pFeatureMap) {
	int map, pix, W, H;

	W = pFeatureMap->width;
	H = pFeatureMap->height;
	for (map = 0; map < pFeatureMap->nMaps; map++) {
		for (pix = 0; pix < W * H; pix++) {
			if(pFeatureMap->pMap[map * W *H + pix] < 0) {
				pFeatureMap->pMap[map * W *H + pix] = 0;
			}
		}
	}
	return SUCCESS;
}

APP_STATUS_E scalar_fix_subsample(MAP_CONTAINER_T *pImap, MAP_CONTAINER_T *pOmap) {

	int map, oW, oH, row, col, iW, iH;
	int32_t sum;

	iW = pImap->width;
	iH = pImap->height;
	oW = pImap->width / 2;
	oH = pImap->height / 2;

	switch(pImap->mMode) {
		case MAP_ISOLATED:		// all rows of a map are in continuous memory
			for (map = 0; map < pImap->nMaps; map++) {
				for(row = 0; row < iH; row += 2) {
					for(col = 0; col < iW; col += 2) {
						// Horizonatlly adjacent comparison of row R
						sum = pImap->pMap[map * iW *iH + row * iW + col] +
							pImap->pMap[map * iW *iH + row * iW + col + 1];
						// Horizonatlly adjacent comparison of row R+1
						sum += pImap->pMap[map * iW *iH + (row + 1) * iW + col];
						sum += pImap->pMap[map * iW *iH + (row + 1) * iW + col + 1];
						// Vertical comparison and store
						pOmap->pMap[map * oW *oH + oW * (row/2) + col/2] = (FP_MAP_PIXEL)(sum / 4);
					}
				}
			}
			break;
		case MAP_CONCAT:		// rows of all maps concatenated to form a bigger row.
			assert(iW % 2 == 0);
			for (row = 0; row < iH; row += 2) {
				for (col = 0; col < iW * pImap->nMaps; col += 2) {
					// FIXME: This is assuming input map width is divisible by 2.
					// If not, then last pixel of a row of map M gets compared with first pixel
					// of the same row of map M+1 which is WRONG.

					// Horizonatlly adjacent comparison of row R
					sum = pImap->pMap[row * iW * pImap->nMaps + col] +
						pImap->pMap[row * iW * pImap->nMaps + col + 1];
					// Horizonatlly adjacent comparison of row R+1
					sum += pImap->pMap[(row + 1) * iW * pImap->nMaps + col];
					sum += pImap->pMap[(row + 1) * iW * pImap->nMaps + col + 1];
					// Vertical comparison and store
					pOmap->pMap[pImap->nMaps * oW * (row / 2) + col / 2] = (FP_MAP_PIXEL)(sum >> 2);
				}
			}
			break;
		default:
			printf("Invalid map arrangement mode\n");
			return FAILED;

	}

	pOmap->nMaps 	= pImap->nMaps;
	pOmap->width 	= oW;
	pOmap->height 	= oH;
	pOmap->mMode	= pImap->mMode;

	return SUCCESS;
}

APP_STATUS_E scalar_fix_max_pool(MAP_CONTAINER_T *pImap, MAP_CONTAINER_T *pOmap) {
	int map, oW, oH, row, col, iW, iH;
	FP_MAP_PIXEL max1, max2;

	iW = pImap->width;
	iH = pImap->height;
	oW = pImap->width / 2;
	oH = pImap->height / 2;

	switch(pImap->mMode) {
		case MAP_ISOLATED:		// all rows of a map are in continuous memory
			for (map = 0; map < pImap->nMaps; map++) {
				for(row = 0; row < iH; row += 2) {
					for(col = 0; col < iW; col += 2) {
						// Horizonatlly adjacent comparison of row R
						max1 = MAX(pImap->pMap[map * iW *iH + row * iW + col],
							pImap->pMap[map * iW *iH + row * iW + col + 1]);
						// Horizonatlly adjacent comparison of row R+1
						max2 = MAX(pImap->pMap[map * iW *iH + (row + 1) * iW + col],
							pImap->pMap[map * iW *iH + (row + 1) * iW + col + 1]);
						// Vertical comparison and store
						pOmap->pMap[map * oW *oH + oW * (row/2) + col/2] = MAX(max1, max2);
					}
				}
			}
			break;
		case MAP_CONCAT:		// rows of all maps concatenated to form a bigger row.
			assert(iW % 2 == 0);
			for (row = 0; row < iH; row += 2) {
				for (col = 0; col < iW * pImap->nMaps; col += 2) {
					// FIXME: This is assuming input map width is divisible by 2.
					// If not, then last pixel of a row of map M gets compared with first pixel
					// of the same row of map M+1 which is WRONG.

					// Horizonatlly adjacent comparison of row R
					max1 = MAX(pImap->pMap[row * iW * pImap->nMaps + col],
						pImap->pMap[row * iW * pImap->nMaps + col + 1]);
					// Horizonatlly adjacent comparison of row R+1
					max2 = MAX(pImap->pMap[(row + 1) * iW * pImap->nMaps + col],
						pImap->pMap[(row + 1) * iW * pImap->nMaps + col + 1]);
					// Vertical comparison and store
					pOmap->pMap[pImap->nMaps * oW * (row / 2) + col / 2] = MAX(max1, max2);
				}
			}
			break;
		default:
			printf("Invalid map arrangement mode\n");
			return FAILED;

	}

	// Update output map dimensions
	pOmap->nMaps 	= pImap->nMaps;
	pOmap->width 	= oW;
	pOmap->height 	= oH;
	pOmap->mMode 	= pImap->mMode;

	return SUCCESS;
}
APP_STATUS_E scalar_fix_conv2D(FP_IMG_PIXEL *pImg, FP_KERNEL *pKer, int H, int W, int kRows, int kCols, int scale, FP_MAP_PIXEL *pMap) {
	int row, col, kr, kc;
	int32_t sop;
	// NOTE: If the output map size to be maintained same as that of input image, then the caller
	// should pad extra rows and columns before calling this API.
	for (row = 0;row < H - kRows + 1; row++)
	{
		for (col = 0; col < W - kCols + 1; col++)
		{
			// Reset accumulator
			sop = 0;
			for ( kr = 0; kr < kRows; kr ++)
			{
				for (kc = 0; kc < kCols; kc++)
				{
					// MAC
					sop += pKer[kCols * kr + kc] * pImg[W * row + col + W * kr + kc];
				}
			}
			sop = (int32_t)(sop / scale);
			sop = sop >> 13; // TODO: THis is very crude way.. !
			pMap[(W - kCols + 1) * row + col] = (FP_MAP_PIXEL)sop;
		}
	}

	return SUCCESS;
}
static void fix_map_scale(FP_MAP_PIXEL *pMap, int M, int N, int scale) {
    int pixel;
    for(pixel = 0; pixel < M * N; pixel++){
        pMap[pixel] = (FP_MAP_PIXEL)(pMap[pixel] / scale);
    }

}
static void fix_map_acc(FP_MAP_PIXEL *map_acc, FP_MAP_PIXEL *map, int M, int N)
{
    int pixel;
    for(pixel = 0; pixel < M * N; pixel++){
        map_acc[pixel] += map[pixel];
    }

}
APP_STATUS_E scalar_fix_L1_maps(IN_IMG_T *pImg, KERNEL_INFO_T *pKer, MAP_CONTAINER_T *pOutMap) {

	int map;
	MAP_CONTAINER_T fMap;
	fMap.height = pImg->height - pKer->kerSize + 1;
	fMap.width	= pImg->width - pKer->kerSize + 1;
	fMap.nMaps 	= pOutMap->nMaps;
	fMap.mMode	= MAP_ISOLATED;
	fMap.pMap	= (FP_MAP_PIXEL *)malloc(fMap.height * fMap.width * fMap.nMaps * sizeof(FP_MAP_PIXEL));
	// Compute all convolutions necessary for the L1 layer
	for (map = 0; map < fMap.nMaps; map++) {
		scalar_fix_conv2D(pImg->pImgBuff,
			pKer->pKerBuff + map * pKer->kerSize * pKer->kerSize,
			pImg->height,
			pImg->width,
			pKer->kerSize,
			pKer->kerSize,
			pKer->kerSize * pKer->kerSize,
			fMap.pMap + map * fMap.height * fMap.width);
//		fix_map_scale(fMap.pMap + map * fMap.height * fMap.width, fMap.height, fMap.width, pKer->kerSize * pKer->kerSize);
	}
	// Rectification
	scalar_fix_rectify(&fMap);
	// Max pooling
//	scalar_fix_max_pool(&fMap, pOutMap);
	scalar_fix_subsample(&fMap, pOutMap);
	scalar_fix_subsample(pOutMap, pOutMap);

	free(fMap.pMap);
	return SUCCESS;
}
APP_STATUS_E scalar_fix_L2_maps_v2_all2all(MAP_CONTAINER_T *pInMaps, KERNEL_INFO_T *pKer,
	MAP_CONTAINER_T *pOutMaps) {

	int map, con;
	MAP_CONTAINER_T fMap;
	int16_t *pAccMap;

	fMap.height = pInMaps->height - pKer->kerSize + 1;
	fMap.width	= pInMaps->width - pKer->kerSize + 1;
	fMap.nMaps 	= pOutMaps->nMaps;
	fMap.mMode	= MAP_ISOLATED;
	fMap.pMap	= (FP_MAP_PIXEL *)malloc(fMap.height * fMap.width * fMap.nMaps * sizeof(FP_MAP_PIXEL));
	// FIXME: accumulator need to  be 32 bit
	pAccMap 	= (FP_MAP_PIXEL *)malloc(pInMaps->height * pInMaps->width * sizeof(FP_MAP_PIXEL));

	for ( map = 0; map < fMap.nMaps; map++) {

		memset(pAccMap, 0, pInMaps->height * pInMaps->width * sizeof(FP_MAP_PIXEL));
		for (con = 0; con < pInMaps->nMaps; con++) {
			//m = pMapConn[map * nConn + con];
			fix_map_acc(pAccMap, pInMaps->pMap + con * pInMaps->height * pInMaps->width, pInMaps->height, pInMaps->width);
		}
		fix_map_scale(pAccMap, pInMaps->height, pInMaps->width, pInMaps->nMaps);
		scalar_fix_conv2D(pAccMap,
			pKer->pKerBuff + map * pKer->kerSize * pKer->kerSize,
			pInMaps->height,
			pInMaps->width,
			pKer->kerSize,
			pKer->kerSize,
			pKer->kerSize * pKer->kerSize,
			fMap.pMap + map * fMap.height * fMap.width);
	}
	// Rectify all maps at once
	scalar_fix_rectify(&fMap);

	// Avg pooling
	scalar_fix_subsample(&fMap, pOutMaps);
	scalar_fix_subsample(pOutMaps, pOutMaps);

	free(pAccMap);
	free(fMap.pMap);
	return SUCCESS;
}
static inline float dot_prod(float *pActivation , float *pWeight, int vLen) {
	int e;
	float sop;

	sop = 0;
	for (e = 0; e < vLen; e++) {
		sop += pActivation[e] * pWeight[e];
	}

	return sop;
}

static inline void vect_add(float *pVect1, float *pVect2, int vLen, float *pSum) {
	int e;
	for ( e = 0; e < vLen; e++) {
		pSum[e] = pVect1[e] + pVect2[e];
	}
}

static int index_of_max(float *pVec, int vLen) {
	float max;
	int maxIndex, e;

	max = -10000000;
	maxIndex = -1;
	for ( e = 0; e < vLen; e++) {
		if (pVec[e] > max) {
			max = pVec[e];
			maxIndex = e;
		}
	}
	if ( maxIndex >= 0) {
	} else {
	//	APP_INFO("No value greater than %f is not found in the array\n", max);
	}

	return maxIndex;
}
APP_STATUS_E dummy_classifier(FL_MAP_PIXEL *pInputNeurons, int nInputNeurons, int nHiddenNeurons, int nOutputNeurons,
	float *pInputWt, float *pHiddenBias, float *pOutputWt) {
	float *pHiddenNeuron, *pOutputNeuron;
	int n, label;



	pHiddenNeuron = (float *)malloc(nHiddenNeurons * sizeof(float));
	pOutputNeuron = (float *)malloc(nOutputNeurons * sizeof(float));

	// Input layer computations
	for (n = 0; n < nHiddenNeurons; n++) {
		pHiddenNeuron[n] = dot_prod(pInputNeurons,	pInputWt + n * nInputNeurons, nInputNeurons);
	}
	vect_add(pHiddenNeuron, pHiddenBias, nHiddenNeurons, pHiddenNeuron);

	// Nonlinear activation
	for ( n = 0; n < nHiddenNeurons; n++) {
		pHiddenNeuron[n] = 1 / ( 1 + exp(-pHiddenNeuron[n]));
	}

	// Output layer
	for ( n = 0; n < nOutputNeurons; n++) {
		pOutputNeuron[n] = dot_prod(pHiddenNeuron, pOutputWt + n * nHiddenNeurons, nHiddenNeurons);
	}
	label = index_of_max(pOutputNeuron, nOutputNeurons);
	printf("%d\n", label);
	free(pHiddenNeuron);
	free(pOutputNeuron);
	return SUCCESS;
}
int enabled = 0;
#if 0
static unsigned int readticks(unsigned int *result)
{
    unsigned int cc;
    if (!enabled) {
        // program the performance-counter control-register:
        asm(" mcr p15, 0, %0, c9, c12, 0" :: "r"(17));
        //enable all counters
        asm ("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
        //Clear overflow.
        asm ("mcr p15, 0, %0, c9, c12, 3" :: "r"(0x8000000f));
        enabled = 1;
    }
    asm ("mrc p15, 0, %0, c9, c13, 0" : "=r"(cc));

    return cc;
}
#endif
void main()
{
	IN_IMG_T img;
	KERNEL_INFO_T ker1, ker2, ker3;
	MAP_CONTAINER_T map1, map2;
	//MAP_CONTAINER_T map3;
	int coeff;
	int W1, W2, W3, H1, H2, H3;
	int N1, N2, N3, K1, K2, K3;
	int iN, hN, oN;
	float *pInputNeurons, *pInputWt, *pHiddenBias, *pOutputWt;

	W1 = 150; H1 = 150; K1 = 9; N1 = 64;
	W2 = 30; H2 = 30; K2 = 9; N2 = 256;
	W3 = 12; H3 = 12; K3 = 5; N3 = 64;
	iN = N3 * ((W3 - K3 + 1)/2) * ((H3 - K3 + 1)/2);
	//iN = 50*9;
	hN = 64;
	oN = 10;


	img.pImgBuff =  (FP_IMG_PIXEL *)malloc(W1*H1*sizeof(FP_IMG_PIXEL));
	map1.pMap = (FP_MAP_PIXEL *)malloc(W1*H1*N1*sizeof(FP_MAP_PIXEL)); // over allocating
	map2.pMap = (FP_MAP_PIXEL *)malloc(W2*H2*N2*sizeof(FP_MAP_PIXEL)); // over allocating
	//map3.pMap = (FP_MAP_PIXEL *)malloc(W3*H3*N3*sizeof(FP_MAP_PIXEL)); // over allocating

	pInputNeurons = malloc(iN * sizeof(float));
	pInputWt = malloc(hN*iN*sizeof(float));
	pHiddenBias = malloc(hN*sizeof(float));
	pOutputWt = malloc(hN*oN*sizeof(float));

	ker1.ppExtKerBuff = (FP_KERNEL **)malloc(K1 * K1 * sizeof(FP_KERNEL *));
	ker2.ppExtKerBuff = (FP_KERNEL **)malloc(K2 * K2 * sizeof(FP_KERNEL *));
	ker3.ppExtKerBuff = (FP_KERNEL **)malloc(K3 * K3 * sizeof(FP_KERNEL *));
	ker1.pKerBuff = (FP_KERNEL *)malloc(K1*K1*N1*sizeof(FP_KERNEL));
	ker2.pKerBuff = (FP_KERNEL *)malloc(K2*K2*N2*sizeof(FP_KERNEL));
	ker3.pKerBuff = (FP_KERNEL *)malloc(K3*K3*N3*sizeof(FP_KERNEL));

    for(coeff = 0; coeff < K1 * K1; coeff++) {
        ker1.ppExtKerBuff[coeff] = (FP_KERNEL *)malloc(N1 * W1 * sizeof(FP_KERNEL));
    }
    for(coeff = 0; coeff < K2 * K2; coeff++) {
        ker2.ppExtKerBuff[coeff] = (FP_KERNEL *)malloc(N2 * W2 * sizeof(FP_KERNEL));
    }
    for(coeff = 0; coeff < K3 * K3; coeff++) {
        ker3.ppExtKerBuff[coeff] = (FP_KERNEL *)malloc(N3 * W3 * sizeof(FP_KERNEL));
    }
	img.width = W1; img.height = H1;
	ker1.kerSize = K1; ker2.kerSize = K2; ker3.kerSize = K3;
	map1.nMaps = N1; map2.nMaps = N2; //map3.nMaps = N3;

#ifdef MXP_PROFILE
        dnnTestLookup[testNo].ts.startTime = vbx_timestamp();
#endif // MXP_PROFILE

#if 1
	// scalar version
	scalar_fix_L1_maps(&img, &ker1, &map1);
	map1.height = H2; map1.width = W2; map1.nMaps = N1; map2.nMaps = N2;
	scalar_fix_L2_maps_v2_all2all(&map1, &ker2, &map2);
	//map2.height = H3; map2.width = W3; map2.nMaps = N2; map3.nMaps = N3;
	//scalar_fix_L2_maps_v2_all2all(&map2, &ker3, &map3);
#endif

	// dummy classifier
	dummy_classifier(pInputNeurons, iN, hN, oN, pInputWt, pHiddenBias, pOutputWt);
#ifdef MXP_PROFILE
        dnnTestLookup[testNo].ts.endTime = vbx_timestamp();
#endif // MXP_PROFILE
}
