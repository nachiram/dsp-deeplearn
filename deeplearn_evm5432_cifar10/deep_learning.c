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

#include "inc/kernel_5x5.h"
#include "IMG_conv_5x5_i16s_c16s_c.h"

#define FULLY_CONNECTED

#define MIN_DSPMAX_ELEMENTS 8

extern unsigned int core_id;

#pragma DATA_ALIGN(dilate,8)
short dilate[MIN_DSPMAX_ELEMENTS] = {0};


static short* pad_img_ptr;
static short* pad_l2_ptr;
static short* pad_l3_ptr;
static short* temp_img_ptr;
static short* layer1_ptr;
static short* layer2_ptr;
static short* accum1_ptr;
static short* accum2_ptr;
static int l1_size;
static int l2_size;
static int l3_size;

short *pInputNeurons, *pInputWt, *pHiddenBias, *pOutputWt;
short *pHiddenNeuron, *pOutputNeuron;

#ifdef FUNCTION_PROFILE
extern uint64_t startVal1,endVal1,startVal2,endVal2;
extern double layer1,layer2,layer3,conv1,conv2,conv3,pool1,pool2,pool3,rect1,rect2,rect3,add1,add2,pad1,pad2,pad3;
#endif
typedef struct maps_info
{
	 uint8_t l1_maps;
	 uint8_t l2_maps;
	 uint8_t l3_maps;
	 short* img_ptr;
	 short* l1_maps_ptr;
     short* l2_maps_ptr;
     short* l3_maps_ptr;
	 uint16_t iN,hN,oN;
}maps_info;

maps_info maps_info_1;

void dummy_classifier(short *pInputNeurons, int nInputNeurons, int nHiddenNeurons, int nOutputNeurons, short *pInputWt, short *pHiddenBias, short *pOutputWt);

void MemMgr_HeapInit(uint32_t img_width, uint32_t img_height)
{
	int l1_width,l1_height;
	int l2_width,l2_height,l3_width,l3_height;

	l1_width 	= (img_width/L1_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);
	l1_height 	= (img_height/L1_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);
	l2_width 	= (l1_width/L2_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);
	l2_height 	= (l1_height/L2_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);
	l3_width 	= (l2_width/L3_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);
	l3_height 	= (l2_height/L3_DOWN_SIZE_FACTOR)+(2*PAD_SIZE);

	l1_size = l1_width*l1_height;
	l2_size = l2_width*l2_height;
	l3_size = l3_width*l3_height;

	maps_info_1.l1_maps = L1_MAPS;
	maps_info_1.l2_maps = L2_MAPS;
	maps_info_1.l3_maps = L3_MAPS;

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

	maps_info_1.l3_maps_ptr = malloc(l3_width*l3_height*maps_info_1.l3_maps*sizeof(short));
	while(maps_info_1.l3_maps_ptr==NULL);

	pad_img_ptr		= malloc((img_width+2*PAD_SIZE)*(img_height+2*PAD_SIZE)*SHORT_SIZE);
	while(pad_img_ptr==NULL);
	temp_img_ptr 	= malloc(img_width*img_height*SHORT_SIZE);
	while(temp_img_ptr==NULL);
	accum1_ptr  	= malloc(l1_width*l1_height*SHORT_SIZE);
	while(accum1_ptr==NULL);


	layer1_ptr  	= malloc(l1_width*l1_height*SHORT_SIZE);
	while(layer1_ptr==NULL);
	pad_l2_ptr 		= malloc((l1_width+2*PAD_SIZE)*(l1_height+2*PAD_SIZE)*SHORT_SIZE);
	while(pad_l2_ptr==NULL);
	accum2_ptr  	= malloc(l2_width*l2_height*SHORT_SIZE);
	while(accum2_ptr==NULL);


	layer2_ptr  	= malloc(l2_width*l2_height*SHORT_SIZE);
	while(layer2_ptr==NULL);
	pad_l3_ptr 		= malloc((l2_width+2*PAD_SIZE)*(l2_height+2*PAD_SIZE)*SHORT_SIZE);
	while(pad_l3_ptr==NULL);

	maps_info_1.iN = L3_MAPS * ((12 - 5 + 1)/2) * ((12 - 5 + 1)/2);
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
    	IMG_conv_5x5_i16s_c16s_cn ((src_ptr+col*w), (out_ptr+col*w), w, w+2*PAD_SIZE, mask_ptr,shift);
    }
}
do_padding(short*image,short*dest_image, uint32_t M, uint32_t N, uint8_t col_pad_width, uint8_t row_pad_width)
{
	int row, col;
	short* temp = dest_image;
	temp = temp + (col_pad_width+N+col_pad_width)*row_pad_width;
    for (row = 0; row < M; row=row++)
    {
    	temp = temp + col_pad_width;
        for (col = 0; col < N; col=col++)
        {
        	*(temp+N*row+col) = *(image + N*row + col);
        }
        temp = temp + col_pad_width;
    }
}
operateLayer1( uint32_t w, uint32_t h)
{
    uint32_t i;
    uint32_t j;
    short pixels = w*h;

    for (i = 0; i<L1_MAPS; i++)
    {
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
    	do_padding(maps_info_1.img_ptr,pad_img_ptr,w,h,PAD_SIZE,PAD_SIZE);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pad1 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
    	Conv5x5(pad_img_ptr,temp_img_ptr,w,h,&kernel5x5[i][0]);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	conv1 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
    	for (j=0; j<pixels; j++)
        {
    		temp_img_ptr[j] = ((temp_img_ptr[j] < 0) ? 0 : temp_img_ptr[j]);
        }
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	rect1 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        maxpool(temp_img_ptr,accum1_ptr, w, h);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pool1 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        DSP_add16_shift(maps_info_1.l1_maps_ptr,accum1_ptr,maps_info_1.l1_maps_ptr,(w*h/4),1);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	add1 += ((endVal2-startVal2)/DSP_FREQ);
#endif
    }
}

operateLayer2(uint32_t w, uint32_t h)
{
    uint8_t i;
    uint32_t j;
    short pixels = w*h;
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
    do_padding(maps_info_1.l1_maps_ptr,pad_l2_ptr,w,h,PAD_SIZE,PAD_SIZE);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pad2 += ((endVal2-startVal2)/DSP_FREQ);
#endif
    for (i = 0; i<L2_MAPS; i++)
    {
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        Conv5x5(pad_l2_ptr,layer1_ptr, w, h, &kernel5x5[i][0]);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	conv2 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        for (j=0; j<pixels; j++)
        {
        	layer1_ptr[j] = ((layer1_ptr[j] < 0) ? 0 : layer1_ptr[j]);
        }
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	rect2 += ((endVal2-startVal2));
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        maxpool(layer1_ptr,accum2_ptr, w, h);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pool2 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
        DSP_add16_shift(maps_info_1.l2_maps_ptr,accum2_ptr,maps_info_1.l2_maps_ptr,(w*h/4),1);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	add2 += ((endVal2-startVal2)/DSP_FREQ);
#endif
    }
}

operateLayer3(uint32_t w, uint32_t h)
{
	uint8_t i;
	uint32_t j;
	short pixels = w*h;

#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
	do_padding(maps_info_1.l2_maps_ptr,pad_l3_ptr,w,h,PAD_SIZE,PAD_SIZE);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pad3 += ((endVal2-startVal2));
#endif
	for (i = 0; i<maps_info_1.l3_maps; i++)
	{
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
		Conv5x5(pad_l3_ptr,layer2_ptr, w, h, &kernel5x5[i][0]);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	conv3 += ((endVal2-startVal2)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
		for (j=0; j<pixels; j++)
		{
			layer2_ptr[j] = ((layer2_ptr[j] < 0) ? 0 : layer2_ptr[j]);
		}
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	rect3 += ((endVal2-startVal2));
#endif
#ifdef FUNCTION_PROFILE
	startVal2 = _itoll(TSCH,TSCL);
#endif
		maxpool(layer2_ptr,maps_info_1.l3_maps_ptr+i*l3_size, w, h);
#ifdef FUNCTION_PROFILE
	endVal2 = _itoll(TSCH,TSCL);
	pool3 += ((endVal2-startVal2));
#endif
	}
}
void MemMgr_HeapDeInit()
{
	free(maps_info_1.img_ptr);
	free(maps_info_1.l1_maps_ptr);
	free(maps_info_1.l2_maps_ptr);
	free(maps_info_1.l3_maps_ptr);
	free(temp_img_ptr);
	free(pad_img_ptr);
	free(pad_l2_ptr);
	free(pad_l3_ptr);
	free(layer1_ptr);
	free(layer2_ptr);
	free(accum1_ptr);
	free(accum2_ptr);
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
#ifdef FUNCTION_PROFILE
	startVal1 = _itoll(TSCH,TSCL);
#endif
    operateLayer1(w, h);
#ifdef FUNCTION_PROFILE
	endVal1 = _itoll(TSCH,TSCL);
	layer1 += ((endVal1-startVal1)/DSP_FREQ);
#endif
#ifdef FUNCTION_PROFILE
	startVal1 = _itoll(TSCH,TSCL);
#endif
    operateLayer2(w / 2, h / 2);
#ifdef FUNCTION_PROFILE
	endVal1 = _itoll(TSCH,TSCL);
	layer2 += ((endVal1-startVal1)/DSP_FREQ);
#endif

#ifdef FUNCTION_PROFILE
	startVal1 = _itoll(TSCH,TSCL);
#endif
    operateLayer3(w/4, h/4);
#ifdef FUNCTION_PROFILE
	endVal1 = _itoll(TSCH,TSCL);
	layer3 += ((endVal1-startVal1)/DSP_FREQ);
#endif
	// dummy classifier
	dummy_classifier(pInputNeurons, maps_info_1.iN, maps_info_1.hN, maps_info_1.oN, pInputWt, pHiddenBias, pOutputWt);

	//MemMgr_HeapDeInit();
}
