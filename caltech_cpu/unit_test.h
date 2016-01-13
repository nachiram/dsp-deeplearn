#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include "struct_defs.h"
#include "img_utility.h"
#include "dnn_scalar_fix_api.h"
#include "dnn_scalar_float_api.h"
#include "dnn_vector_fix_api.h"
#include "block_controller.h"
//=============================================================
// Unit test names
typedef enum {
        START_OF_TEST,
//		UT_GENERIC,
/*		UT_RECTIFY,
		UT_MAX_POOL,
		UT_CONV_2D,
		UT_CONV_2D_VVMUL,
		UT_CONV_2D_INLAYER,
		UT_CONV_2D_HIDDENLAYER,
		UT_CONV_2D_HIDDENLAYER_ALL2ALL,*/
		UT_FCCM_TB,
		//UT_FCCM_API,
		//UT_FCCM_DMA,
		//UT_OPT_PARAM_ANALYSIS,
        // Add extra test cases here
        END_OF_TEST
} UNIT_TEST_E;
//=============================================================
//=============================================================
// Function pointer to test case callback
typedef APP_STATUS_E (* const pTestCallback)(int, ...);
//=============================================================
// Structure for test framework table
typedef struct {
    // Test name
    const char * testName;
    // Test number
    UNIT_TEST_E testNo;
    // Test callback function
    //APP_STATUS_E (* const test_callback)(int, ...);
    pTestCallback testCallback;
    // Timestamp counters for the test case.
    PERF_CNTR_T ts;
} TEST_SET_T;

extern TEST_SET_T dnnTestLookup[];

APP_STATUS_E run_unit_tests();

APP_STATUS_E test_optimalBlockSize_inLayer(void);
#endif // _UNIT_TEST_H_
