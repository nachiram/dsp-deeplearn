/*
 * classifier.c
 *
 *  Created on: Dec 24, 2015
 *      Author: nachi
 */

#include <DSP_maxidx.h>
#include "DSP_vec.h"
#include "DSP_dotprod.h"


extern short* pHiddenNeuron;
extern short* pOutputNeuron;

void dummy_classifier(short *pInputNeurons, int nInputNeurons, int nHiddenNeurons, int nOutputNeurons, short *pInputWt, short *pHiddenBias, short *pOutputWt)
{

	int n, label;

	// Input layer computations
	for (n = 0; n < nHiddenNeurons; n++)
	{
		pHiddenNeuron[n] = DSP_dotprod(pInputNeurons,	pInputWt + n * nInputNeurons, nInputNeurons);//dot_prod(pInputNeurons,	pInputWt + n * nInputNeurons, nInputNeurons);
	}
	DSP_vec(pHiddenNeuron, pHiddenBias,pHiddenNeuron, nHiddenNeurons);

	// Nonlinear activation
	for ( n = 0; n < nHiddenNeurons; n++)
	{
		if(pHiddenNeuron[n] < 0)
			pHiddenNeuron[n] = 0;
	}

	// Output layer
	for ( n = 0; n < nOutputNeurons; n++)
	{
		pOutputNeuron[n] = DSP_dotprod(pHiddenNeuron, pOutputWt + n * nHiddenNeurons, nHiddenNeurons); //dot_prod(pHiddenNeuron, pOutputWt + n * nHiddenNeurons, nHiddenNeurons);
	}
	label = DSP_maxidx(pOutputNeuron,(nOutputNeurons%64 == 0 ? nOutputNeurons : nOutputNeurons+(nOutputNeurons%64)));
}


