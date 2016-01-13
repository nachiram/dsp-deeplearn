/*******************************************************
* File Name -- deep_learn.c
*
* Authors:  Nachiappan
*
* Created on : 27-September-2015
*
* Contents: Deeplearn algorithm implementation
*           for TI TMS320C6678 - DSP
*
********************************************************/
#include <DSP_maxval.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "user_config.h"
#include "c6x.h"

#include "DSP_add16.h"

#include "inc/kernel_5x5.h"
#include "inc/kernel_7x7.h"
#include "IMG_conv_7x7_i16s_c16s_c.h"
#include "IMG_conv_5x5_i16s_c16s_c.h"

#define FULLY_CONNECTED

#define MIN_DSPMAX_ELEMENTS 8

extern unsigned int core_id;

#pragma DATA_ALIGN(dilate,8)
short dilate[MIN_DSPMAX_ELEMENTS] = {0};

static short* temp_img_ptr;
static short* layer1_ptr;
static short* temp1_ptr;
static int l1_size;
static int l2_size;

short *pInputNeurons, *pInputWt, *pHiddenBias, *pOutputWt;
short *pHiddenNeuron, *pOutputNeuron;

typedef struct maps_info
{
	 uint16_t l1_maps;
	 uint16_t l2_maps;
     short* img_ptr;
	 short* l1_maps_ptr;
     short* l2_maps_ptr;
     uint16_t iN,hN,oN;
}maps_info;

maps_info maps_info_1;

void dummy_classifier(short *pInputNeurons, int nInputNeurons, int nHiddenNeurons, int nOutputNeurons, short *pInputWt, short *pHiddenBias, short *pOutputWt);

void MemMgr_HeapInit(uint32_t img_width, uint32_t img_height)
{
	int l1_width,l1_height;
	int l2_width,l2_height;

	l1_width = img_width/L1_DOWN_SIZE_FACTOR;
	l1_height = img_height/L1_DOWN_SIZE_FACTOR;
	l2_width = l1_width/L2_DOWN_SIZE_FACTOR;
	l2_height = l1_height/L2_DOWN_SIZE_FACTOR;

	l1_size = l1_width*l1_height;
	l2_size = l2_width*l2_height;

	maps_info_1.l1_maps = L1_MAPS;
	maps_info_1.l2_maps = L2_MAPS;

	maps_info_1.img_ptr = malloc(img_width*img_height*sizeof(short));
	while(maps_info_1.img_ptr==NULL);

#ifndef FULLY_CONNECTED
	maps_info_1.l1_maps_ptr = malloc(l1_width*l1_height*maps_info_1.l1_maps*sizeof(short)); ///*TODO*/data allignment
	while(maps_info_1.l1_maps_ptr==NULL);
#else
	maps_info_1.l1_maps_ptr = malloc(l1_width*l1_height*sizeof(short)); ///*TODO*/data allignment
	while(maps_info_1.l1_maps_ptr==NULL);
#endif

	maps_info_1.l2_maps_ptr = malloc(l2_width*l2_height*maps_info_1.l2_maps*sizeof(short));
	while(maps_info_1.l2_maps_ptr==NULL);

	temp_img_ptr 	= malloc(img_width*img_height*sizeof(short));
	while(temp_img_ptr==NULL);
	layer1_ptr  	= malloc(l1_width*l1_height*sizeof(short));
	while(layer1_ptr==NULL);
	temp1_ptr  		= malloc(l1_width*l1_height*sizeof(short));
	while(temp1_ptr==NULL);

	maps_info_1.iN = L2_MAPS * ((((IMG_WIDTH-KERNELL1_SIZE+1)/L1_DOWN_SIZE_FACTOR)-KERNEL2_SIZE+1)/L2_DOWN_SIZE_FACTOR) * ((((IMG_HEIGHT-KERNELL1_SIZE+1)/L1_DOWN_SIZE_FACTOR)-KERNEL2_SIZE+1)/L2_DOWN_SIZE_FACTOR);
	maps_info_1.hN = HIDDEN_NEURONS;
	maps_info_1.oN = OUTPUT_NEURONS;

	pInputNeurons 	= malloc(maps_info_1.iN*sizeof(float));
	while(pInputNeurons==NULL);
	pHiddenNeuron 	= malloc(maps_info_1.hN*sizeof(float));
	while(pHiddenNeuron==NULL);
	pOutputNeuron 	= malloc(maps_info_1.oN*sizeof(float));
	while(pOutputNeuron==NULL);
	pInputWt 		= malloc(maps_info_1.hN*maps_info_1.iN*sizeof(float));
	while(pInputWt==NULL);
	pHiddenBias 	= malloc(maps_info_1.hN*sizeof(float));
	while(pHiddenBias==NULL);
	pOutputWt 		= malloc(maps_info_1.hN*maps_info_1.oN*sizeof(float));
	while(pOutputWt==NULL);
}

static void maxpool(short *image,short *dest_image, int M, int N)
{
	int krows =2,kcols =2,stride_len=2;
    int row, col,kernel_row, kernel_col;
    int dest_col = N/2,drows=0,dcols=0;

    for (row = 0; row < M; row=row+stride_len,drows++)
    {
        for (col = 0; col < N; col=col+stride_len,dcols++)
        {
            for (kernel_row = 0; kernel_row < krows; kernel_row++)
            {
                for (kernel_col = 0; kernel_col < kcols; kernel_col++)
                {
                    (*(dilate + kcols*kernel_row + kernel_col)) = *(image + N*row + col + N*kernel_row + kernel_col);
                }
            }
        	*(dest_image + dest_col*drows + dcols) = DSP_maxval(&dilate[0],MIN_DSPMAX_ELEMENTS);
        }
    }

}

Conv5x5(short* src_ptr,short* out_ptr,uint32_t w,uint32_t h,const short*mask_ptr)
{
    short shift = 0; /*TODO*/
    int col;

    for(col=0;col<h-4;col++)
    {
    	IMG_conv_5x5_i16s_c16s_cn ((src_ptr+col*w), (out_ptr+col*w), w, w, mask_ptr,shift);
    }
}
operateLayer1( uint32_t w, uint32_t h)
{
    uint32_t i;
    uint32_t j;
    short pixels = w*h;

    for (i = 0; i<L1_MAPS; i++)
    {
    	Conv5x5(maps_info_1.img_ptr,temp_img_ptr,w,h,&kernel5x5[i][0]);

    	for (j=0; j<pixels; j++)
        {
    		temp_img_ptr[j] = ((temp_img_ptr[j] < 0) ? 0 : temp_img_ptr[j]);
        }
#ifndef FULLY_CONNECTED
    	maxpool(temp_img_ptr,(maps_info_1.l1_maps_ptr+(i*l1_size)), w, h);
#else
    	maxpool(temp_img_ptr,temp1_ptr, w, h);
    	DSP_add16_shift(maps_info_1.l1_maps_ptr,temp1_ptr,maps_info_1.l1_maps_ptr,(w*h/4),1);
#endif
    }
}

operateLayer2(uint32_t w, uint32_t h)
{
    uint8_t i;
    uint32_t j;
    short pixels = w*h;

    for (i = 0; i<L2_MAPS; i++)
    {
#ifndef FULLY_CONNECTED
    	uint8_t selection;
        short* l1_maps_ptr = (short*)maps_info_1.l1_maps_ptr[0];
        short* ptr2 = NULL;

    	selection = rand() % L1_MAPS;
        ptr1 = (l1_maps_ptr+(selection*l1_size));
        for(k=0;k<L1_L2_CONNECTIONS/2;k++)
        {
        	selection = rand() % L1_MAPS;
        	ptr2 = (l1_maps_ptr+(selection*l1_size));
        	DSP_add16_shift(ptr1,ptr2,temp1_ptr,pixels,1);
        	selection = rand() % L1_MAPS;
        	ptr2 = (l1_maps_ptr+(selection*l1_size));
        	DSP_add16_shift(temp1_ptr,ptr2,ptr1,pixels,1);
        }
#endif

        Conv5x5(maps_info_1.l1_maps_ptr,layer1_ptr, w, h, &kernel5x5[i][0]);

        for (j=0; j<pixels; j++)
        {
        	layer1_ptr[j] = ((layer1_ptr[j] < 0) ? 0 : layer1_ptr[j]);
        }

        maxpool(layer1_ptr,(maps_info_1.l2_maps_ptr+(i*l2_size)), w, h);
    }
}
void MemMgr_HeapDeInit()
{
	free(maps_info_1.img_ptr);
	free(maps_info_1.l1_maps_ptr);
	free(maps_info_1.l2_maps_ptr);
	free(temp_img_ptr);
	free(temp1_ptr);
	free(layer1_ptr);
	free(pInputNeurons);
	free(pInputWt);
	free(pHiddenBias);
	free(pOutputWt);
	free(pHiddenNeuron);
	free(pOutputNeuron);
}
void deeplearn( uint32_t w, uint32_t h)
{

	MemMgr_HeapInit(w,h);

    operateLayer1(w, h);

    operateLayer2(w / 2, h / 2);
	// dummy classifier
	dummy_classifier(pInputNeurons, maps_info_1.iN, maps_info_1.hN, maps_info_1.oN, pInputWt, pHiddenBias, pOutputWt);

	//MemMgr_HeapDeInit();
}
