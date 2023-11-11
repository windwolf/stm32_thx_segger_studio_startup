#include "unit_test.h"
#include "communication_protocol/inc/message_parser_test.h"
#include "communication_protocol/inc/nmea_test.h"
#include "communication_protocol/inc/tree_accessor_test.h"
#include "algorithm/inc/kf_test.h"
#include "algorithm/inc/pid_test.h"
#include "stdint.h"
#include "stdio.h"
#define LOG_MODULE "TEST"
#include "log.h"
#include "arm_math.h"

#ifdef UNITTEST

void test_before_hal_init()
{
#ifdef DEBUG_SIM
	kf_test();
	message_parser_test();
	tree_accessor_test();
#endif //DEBUG_SIM
}

void test_after_hal_init()
{
#ifndef DEBUG_SIM
	kf_test();
	LOG_I("kf_test end");
	
	message_parser_test();
	LOG_I("message_parser_test end");
	
	tree_accessor_test();
	LOG_I("tree_accessor_test end");
#endif //DEBUG_SIM
}

#endif //UNITTEST