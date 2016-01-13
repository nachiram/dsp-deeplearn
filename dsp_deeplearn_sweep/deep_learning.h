/*******************************************************
* File Name -- main.c
*
* Authors:  Nachiappan, Gourav, Mahesh
*
* Created on : 27-September-2015
*
********************************************************/

#ifndef DSP_DEEPLEARN_DEEP_LEARNING_H_
#define DSP_DEEPLEARN_DEEP_LEARNING_H_

extern uint32_t initializeData(uint32_t w, uint32_t h);
extern uint32_t deinitializeData();
extern uint32_t operateLayer1(uint16_t** src, uint32_t w, uint32_t h);
extern uint32_t operateLayer2(uint32_t w, uint32_t h);
extern uint32_t operateLayer3(uint32_t w, uint32_t h);
extern uint32_t classifier(uint16_t** src_initial, int w, int h);
extern uint32_t deeplearn(uint16_t* data, uint32_t w, uint32_t h);


#endif /* DSP_DEEPLEARN_DEEP_LEARNING_H_ */
