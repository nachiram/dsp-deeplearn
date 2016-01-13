/*******************************************************
* File Name -- main.c
*
* Authors:  Nachiappan
*
* Created on : 27-September-2015
*
* Contents: Deeplearn algorithm implementation
*
********************************************************/

#include <stdio.h>
#include <c6x.h>
#include "user_config.h"

#define ITERATIONS 				5

#ifdef FUNCTION_PROFILE
uint64_t startVal1,endVal1,startVal2,endVal2;
double layer1,layer2,layer3,conv1,conv2,conv3,pool1,pool2,pool3,rect1,rect2,rect3,add1,add2,pad1,pad2,pad3;
#endif

/* INTC_DSP registers */
#define SYS_INTC_EVTFLAG0		0x01800000
#define SYS_INTC_EVTFLAG1		0x01800004
#define SYS_INTC_EVTFLAG2		0x01800008
#define SYS_INTC_EVTFLAG3		0x0180000C
#define SYS_INTC_EVTCLR0		0x01800040
#define SYS_INTC_EVTCLR1		0x01800044
#define SYS_INTC_EVTCLR2		0x01800048
#define SYS_INTC_EVTCLR3		0x0180004C
#define SYS_INTC_EVTMASK0		0x01800080
#define SYS_INTC_EVTMASK1		0x01800084
#define SYS_INTC_EVTMASK2		0x01800088
#define SYS_INTC_EVTMASK3		0x0180008C
#define SYS_INTC_INTMUX1		0x01800104
#define SYS_INTC_INTMUX2		0x01800108
#define SYS_INTC_INTMUX3		0x0180010C
#define SYS_INTC_INTXCLR		0x01800184
#define SYS_INTC_INTDMASK		0x01800188

/* Cache control registers */
#define	L1_SCACHE_CONFIG		0x01C30004
#define	L1_SCACHE_INT			0x01C30008
#define	L1_SCACHE_OCP			0x01C3000C
#define L1_SCACHE_MAINT			0x01C30010
#define L1_SCACHE_MTSTART		0x01C30014
#define L1_SCACHE_MTEND			0x01C30018

#define	L2_SCACHE_CONFIG		0x01C30204
#define	L2_SCACHE_INT			0x01C30208
#define	L2_SCACHE_OCP			0x01C3020C
#define L2_SCACHE_MAINT			0x01C30210
#define L2_SCACHE_MTSTART		0x01C30214
#define L2_SCACHE_MTEND			0x01C30218

#define SCACHE_LOCK				(1<<0)
#define BYPASS					(1<<1)
#define	LOCK_INT				(1<<2)
#define LOCK_PORT				(1<<3)
#define	LOCK_MAIN				(1<<4)

#define	CLEANBUF				(1<<5)

#define	CLEAN					(1<<3)
#define INVALIDATE				(1<<4)
#define INTERRUPT				(1<<5)

/* SCACHE_MMU registers */
#define	SCACHE_MMU_LARGE_ADDR	0x01C30800
#define	SCACHE_MMU_LARGE_POLICY	0x01C30840
#define	SCACHE_MMU_MED_ADDR		0x01C30860
#define	SCACHE_MMU_MED_POLICY	0x01C308E0
#define	SCACHE_MMU_SMALL_ADDR	0x01C30920
#define	SCACHE_MMU_SMALL_POLICY	0x01C30A20

#define	ENABLE					(1<<0)
#define	SIZE					(1<<1)
#define VOLATILE				(1<<3)
#define L1_CACHEABLE			(1<<16)
#define L1_POSTED				(1<<17)
#define L1_ALLOCATE				(1<<18)
#define L1_WR_POLICY			(1<<19)
#define L2_CACHEABLE			(1<<20)
#define L2_POSTED				(1<<21)
#define L2_ALLOCATE				(1<<22)
#define L2_WR_POLICY			(1<<23)

/* Counter/Timer registers */
#define SCACHE_SCTM_CTCNTL		0x01C30400
#define SCACHE_SCTM_TINTVLR		0x01C30440
#define SCACHE_SCTM_CTCR_WT		0x01C30500

#define ENBL					(1<<0)
#define RESET					(1<<1)
#define	CHAIN					(1<<2)
#define DURMODE					(1<<3)
#define FREE					(1<<4)
#define IDLE					(1<<5)
#define INT						(1<<8)
#define	DBG						(1<<9)
#define	RESTART					(1<<10)

/* macro definitions */
#define WR_MEM_32(addr, data)   *(volatile unsigned int*)(addr) = (unsigned int)(data)
#define RD_MEM_32(addr)         *(volatile unsigned int*)(addr)

/* function definitions */
void config_AMMU(void)
{
	/* allow un-cached accesses into address range 0x40000000..0x5FFFFFFF */
	WR_MEM_32(SCACHE_MMU_LARGE_ADDR, 0x40000000);
	WR_MEM_32(SCACHE_MMU_LARGE_POLICY, ENABLE | SIZE);
	/* allow cached accesses into address range 0x80000000..0x9FFFFFFF */
	WR_MEM_32(SCACHE_MMU_LARGE_ADDR+0x04, 0x80000000);
	WR_MEM_32(SCACHE_MMU_LARGE_POLICY+0x04, ENABLE | SIZE |
			  L1_CACHEABLE | L1_POSTED | L1_WR_POLICY |
			  L2_CACHEABLE | L2_POSTED | L2_WR_POLICY);
	/* allow cached accesses into address range 0xA0000000..0xBFFFFFFF */
	WR_MEM_32(SCACHE_MMU_LARGE_ADDR+0x08, 0xA0000000);
	WR_MEM_32(SCACHE_MMU_LARGE_POLICY+0x08, ENABLE | SIZE |
			  L1_CACHEABLE | L1_POSTED | L1_WR_POLICY |
			  L2_CACHEABLE | L2_POSTED | L2_WR_POLICY);
	/* allow cached accesses into address range 0xC0000000..0xDFFFFFFF */
	WR_MEM_32(SCACHE_MMU_LARGE_ADDR+0x0C, 0xC0000000);
	WR_MEM_32(SCACHE_MMU_LARGE_POLICY+0x0C, ENABLE | SIZE |
			  L1_CACHEABLE | L1_POSTED | L1_WR_POLICY |
			  L2_CACHEABLE | L2_POSTED | L2_WR_POLICY);
	/* allow cached accesses into address range 0xE0000000..0xFFFFFFFF */
	WR_MEM_32(SCACHE_MMU_LARGE_ADDR+0x10, 0xE0000000);
	WR_MEM_32(SCACHE_MMU_LARGE_POLICY+0x10, ENABLE | SIZE |
			  L1_CACHEABLE | L1_POSTED | L1_WR_POLICY |
			  L2_CACHEABLE | L2_POSTED | L2_WR_POLICY);

	/* used first small page for accesses to INTC_DSP registers */
	WR_MEM_32(SCACHE_MMU_SMALL_POLICY, 0);
	WR_MEM_32(SCACHE_MMU_SMALL_ADDR, 0x01800000);
	WR_MEM_32(SCACHE_MMU_SMALL_POLICY, ENABLE | VOLATILE);
}


void enable_L1Cache(void)
{
	WR_MEM_32(L1_SCACHE_CONFIG, BYPASS | LOCK_INT | LOCK_PORT | LOCK_MAIN);
}


void enable_L2Cache(void)
{
	WR_MEM_32(L2_SCACHE_CONFIG, BYPASS | LOCK_INT | LOCK_PORT | LOCK_MAIN);
}


void invalidate(unsigned int address, unsigned int size)
{
	WR_MEM_32(L2_SCACHE_MTSTART, address);
	WR_MEM_32(L2_SCACHE_MTEND, address+size);
	WR_MEM_32(L2_SCACHE_MAINT, INVALIDATE);
	while(RD_MEM_32(L2_SCACHE_MTSTART) != 0);		// wait until L2 maintenance operation is finished
	WR_MEM_32(L1_SCACHE_MTSTART, address);
	WR_MEM_32(L1_SCACHE_MTEND, address+size);
	WR_MEM_32(L1_SCACHE_MAINT, INVALIDATE);
	while(RD_MEM_32(L1_SCACHE_MTSTART) != 0);		// wait until L1 maintenance operation is finished
}


void clean(unsigned int address, unsigned int size)
{
	WR_MEM_32(L1_SCACHE_OCP, CLEANBUF);
	WR_MEM_32(L1_SCACHE_MTSTART, address);
	WR_MEM_32(L1_SCACHE_MTEND, address+size);
	WR_MEM_32(L1_SCACHE_MAINT, CLEAN);
	while(RD_MEM_32(L1_SCACHE_MTSTART) != 0);		// wait until L1 maintenance operation is finished
	WR_MEM_32(L2_SCACHE_OCP, CLEANBUF);
	WR_MEM_32(L2_SCACHE_MTSTART, address);
	WR_MEM_32(L2_SCACHE_MTEND, address+size);
	WR_MEM_32(L2_SCACHE_MAINT, CLEAN);
	while(RD_MEM_32(L2_SCACHE_MTSTART) != 0);		// wait until L2 maintenance operation is finished
}

void deeplearn(uint32_t img_width, uint32_t img_height);
int main(void)
{
	int i,img_width=IMG_WIDTH,img_height=IMG_HEIGHT;
	uint64_t startVal,endVal;
	double cumulative=0;

	TSCL = 0;
	config_AMMU();
	enable_L1Cache();
	enable_L2Cache();

	for(i=0;i<ITERATIONS;i++)
	{
		startVal = _itoll(TSCH,TSCL);
		deeplearn(img_width, img_height);
		endVal = _itoll(TSCH,TSCL);
		cumulative += ((endVal-startVal)/DSP_FREQ);
	}
#ifdef FUNCTION_PROFILE
		printf("%lf %lf %lf \n",(layer1/ITERATIONS),(layer2/ITERATIONS),(layer3/ITERATIONS));
		printf("%lf %lf %lf %lf \n",(pad1/ITERATIONS),(conv1/ITERATIONS),(rect1/ITERATIONS),(pool1/ITERATIONS));
		printf("%lf %lf %lf %lf %lf \n",(add1/ITERATIONS),(pad2/ITERATIONS),(conv2/ITERATIONS),(rect2/ITERATIONS),(pool2/ITERATIONS));
		printf("%lf %lf %lf %lf %lf \n",(add2/ITERATIONS),(pad3/ITERATIONS),(conv3/ITERATIONS),(rect3/ITERATIONS),(pool3/ITERATIONS));
#else
		printf("%lf us",(cumulative/ITERATIONS));
#endif
}

#if 0
#define SCTM_TIMEVT1_INTR		125			// Timer interrupt 1
/* global variables */
volatile int intrCnt = 0;
void start_Timer(void)
{
	// enable timer interrupt
	WR_MEM_32(SYS_INTC_EVTMASK0, 0xFFFFFFFF);		// disable event combiner
	WR_MEM_32(SYS_INTC_EVTMASK1, 0xFFFFFFFF);
	WR_MEM_32(SYS_INTC_EVTMASK2, 0xFFFFFFFF);
	WR_MEM_32(SYS_INTC_EVTMASK3, 0xFFFFFFFF);

	WR_MEM_32(SYS_INTC_INTXCLR, 1);					// clear interrupt exception status register
	WR_MEM_32(SYS_INTC_INTDMASK, 0x0000FFF0);		// disable dropped event detection for all interrupts

													// DSP INT[4] is used to handle timer interrupt
	WR_MEM_32(SYS_INTC_INTMUX1, (RD_MEM_32(SYS_INTC_INTMUX1) & 0xFFFFFF00) | SCTM_TIMEVT1_INTR);
	ICR = (1<<4);									// clear any pending INT[4] interrupt
	IER |= (1<<4) | (1<<1);							// enable INT[4] and NMI
	CSR |= 1;										// GIE = 1, global interrupt enable

	// configure and start timer
	WR_MEM_32(SCACHE_SCTM_CTCNTL, RD_MEM_32(SCACHE_SCTM_CTCNTL) | ENBL);	// enable SCTM module
	WR_MEM_32(SCACHE_SCTM_CTCR_WT, RESET);									// reset timer
	WR_MEM_32(SCACHE_SCTM_TINTVLR, DSP_FREQ/1000);							// set interrupt period to 1ms
	WR_MEM_32(SCACHE_SCTM_CTCR_WT, ENBL | INT | RESTART);					// start timer in repeat mode
}
/* Timer Interrupt Service Routine */
interrupt void timerISR(void)
{
	intrCnt++;
}
#endif
