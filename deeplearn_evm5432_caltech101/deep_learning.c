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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "user_config.h"
#include "c6x.h"

#include "DSP_maxval.h"
#include "DSP_add16.h"

#include "inc/kernel_11x11.h"
#include "IMG_conv_11x11_i16s_c16s_c.h"

#define FULLY_CONNECTED

#define MIN_DSPMAX_ELEMENTS 100

extern unsigned int core_id;

#pragma DATA_ALIGN(dilate,8)
short dilate[MIN_DSPMAX_ELEMENTS] = {0};

static short* local_l1_maps_ptr;

static short* temp_img_ptr;
static short* layer1_ptr;
static short* temp1_ptr;
static int l1_size;
static int l2_size;

float *pInputNeurons, *pInputWt, *pHiddenBias, *pOutputWt;
float *pHiddenNeuron, *pOutputNeuron;

typedef struct maps_info
{
	 uint16_t l1_maps;
	 uint16_t l2_maps;
	 short* l1_maps_ptr;
     short* l2_maps_ptr;
}maps_info;

maps_info maps_info_1;
maps_info* maps_info_ptr;

void dummy_classifier(float *pInputNeurons, int nInputNeurons, int nHiddenNeurons, int nOutputNeurons, float *pInputWt, float *pHiddenBias, float *pOutputWt);

void MemMgr_HeapInit(uint32_t img_width, uint32_t img_height)
{
	int l1_width,l1_height;
	int l2_width,l2_height;
//	int iN,hN,oN;

	l1_width = img_width/L1_DOWN_SIZE_FACTOR;
	l1_height = img_height/L1_DOWN_SIZE_FACTOR;
	l2_width = l1_width/L2_DOWN_SIZE_FACTOR;
	l2_height = l1_height/L2_DOWN_SIZE_FACTOR;

	l1_size = l1_width*l1_height;
	l2_size = l2_width*l2_height;

	maps_info_1.l1_maps = 64;
	maps_info_1.l2_maps = 256;
	maps_info_ptr = &maps_info_1;

#ifndef FULLY_CONNECTED
	maps_info_ptr->l1_maps_ptr = malloc(l1_width*l1_height*maps_info_ptr->l1_maps*sizeof(short)); ///*TODO*/data allignment
	while(maps_info_ptr->l1_maps_ptr==NULL);
#else
	maps_info_ptr->l1_maps_ptr = malloc(l1_width*l1_height*sizeof(short)); ///*TODO*/data allignment
	while(maps_info_ptr->l1_maps_ptr==NULL);
#endif

	maps_info_ptr->l2_maps_ptr = malloc(l2_width*l2_height*maps_info_ptr->l2_maps*sizeof(short));
	while(maps_info_ptr->l2_maps_ptr==NULL);

	temp_img_ptr 	= malloc(img_width*img_height*sizeof(short));
	while(temp_img_ptr==NULL);
	layer1_ptr  	= malloc(l1_width*l1_height*sizeof(short));
	while(layer1_ptr==NULL);
	temp1_ptr  		= malloc(l1_width*l1_height*sizeof(short));
	while(temp1_ptr==NULL);

	//W3 = 12; H3 = 12; K3 = 5; N3 = 64;
	//iN = N3 * ((W3 - K3 + 1)/2) * ((H3 - K3 + 1)/2);
//	iN = 50*9;
//	hN = 64;
//	oN = 10;

//	pInputNeurons 	= malloc(iN*sizeof(float));
//	while(pInputNeurons==NULL);
//	pInputWt 		= malloc(hN*iN*sizeof(float));
//	while(pInputWt==NULL);
//	pHiddenBias 	= malloc(hN*sizeof(float));
//	while(pHiddenBias==NULL);
//	pOutputWt 		= malloc(hN*oN*sizeof(float));
//	while(pOutputWt==NULL);
}


static void maxpool_l1(short *image,short *dest_image, int M, int N)
{
	int krows =L1_AVG_POOL,kcols =L1_AVG_POOL,stride_len=L1_DOWN_SIZE_FACTOR;
    int row, col,kernel_row, kernel_col;
    int dest_col = N/L1_DOWN_SIZE_FACTOR,drows=0,dcols=0;

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
        	*(dest_image + dest_col*drows + dcols) = DSP_maxval(&dilate[0],L1_AVG_POOL*L1_AVG_POOL);
        }
    }

}

static void maxpool_l2(short *image,short *dest_image, int M, int N)
{
	int krows =L2_AVG_POOL,kcols =L2_AVG_POOL,stride_len=L2_DOWN_SIZE_FACTOR;
    int row, col,kernel_row, kernel_col;
    int dest_col = N/L2_DOWN_SIZE_FACTOR,drows=0,dcols=0;

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
        	*(dest_image + dest_col*drows + dcols) = DSP_maxval(&dilate[0],L2_AVG_POOL*L2_AVG_POOL);
        }
    }

}
Conv9x9(short* src_ptr,short* out_ptr,uint32_t w,uint32_t h,const short*mask_ptr)
{
    short shift = 0;
    int col;

    for(col=0;col<h-8;col++)
    {
    	IMG_conv_11x11_i16s_c16s_cn ((src_ptr+col*w), (out_ptr+col*w), w, w, mask_ptr,shift);
    	//IMG_conv_9x9_i16s_c16s_cn ((src_ptr+col*w), (out_ptr+col*w), w, w, mask_ptr,shift);
    }
}

operateLayer1(short* img_ptr, uint32_t w, uint32_t h)
{
    uint32_t i;
    uint32_t j;
    short pixels = w*h;
    uint32_t local_l1_maps = maps_info_ptr->l1_maps;

    for (i = 0; i<local_l1_maps; i++)
    {
    	Conv9x9(img_ptr,temp_img_ptr,w,h,&kernel11x11[i][0]);

    	for (j=0; j<pixels; j++)
        {
    		temp_img_ptr[j] = ((temp_img_ptr[j] < 0) ? 0 : temp_img_ptr[j]);
        }
#ifndef FULLY_CONNECTED
    	maxpool(temp_img_ptr,(local_l1_maps_ptr+(i*l1_size)), w, h);
#else
    	maxpool_l1(temp_img_ptr,temp1_ptr, w, h);
    	DSP_add16_shift(local_l1_maps_ptr,temp1_ptr,local_l1_maps_ptr,(w*h/4),1);
#endif
    }
}

operateLayer2(uint32_t w, uint32_t h)
{
    uint16_t i;
    uint32_t j;
    short pixels = w*h;
    uint32_t local_l2_maps = maps_info_ptr->l2_maps;

    for (i = 0; i<local_l2_maps; i++)
    {
#ifndef FULLY_CONNECTED
    	uint8_t selection;
        short* global_l1_map_start_ptr = (short*)maps_info_ptr->l1_maps_ptr[0];
        short* ptr2 = NULL;

    	selection = rand() % L1_MAPS;
        ptr1 = (global_l1_map_start_ptr+(selection*l1_size));
        for(k=0;k<L1_L2_CONNECTIONS/2;k++)
        {
        	selection = rand() % L1_MAPS;
        	ptr2 = (global_l1_map_start_ptr+(selection*l1_size));
        	DSP_add16_shift(ptr1,ptr2,temp1_ptr,pixels,1);
        	selection = rand() % L1_MAPS;
        	ptr2 = (global_l1_map_start_ptr+(selection*l1_size));
        	DSP_add16_shift(temp1_ptr,ptr2,ptr1,pixels,1);
        }
#endif

        Conv9x9(maps_info_ptr->l1_maps_ptr,layer1_ptr, w, h, &kernel11x11[i][0]);

        for (j=0; j<pixels; j++)
        {
        	layer1_ptr[j] = ((layer1_ptr[j] < 0) ? 0 : layer1_ptr[j]);
        }

        maxpool_l2(layer1_ptr,(maps_info_ptr->l2_maps_ptr+(i*l2_size)), w, h);
    }
}
void deeplearn(short* data, uint32_t w, uint32_t h)
{

	MemMgr_HeapInit(w,h);

    operateLayer1(data, w, h);

    operateLayer2(w / L1_DOWN_SIZE_FACTOR, h / L1_DOWN_SIZE_FACTOR);

	// dummy classifier
	//dummy_classifier(pInputNeurons, 50*9, 64, 10, pInputWt, pHiddenBias, pOutputWt);
}
