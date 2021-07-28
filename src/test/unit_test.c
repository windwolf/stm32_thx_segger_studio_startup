#include "unit_test.h"
#include "communication_protocol/inc/message_parser_test.h"
#include "communication_protocol/inc/nmea_test.h"
#include "communication_protocol/inc/tree_accessor_test.h"
#include "stdint.h"
#include "stdio.h"
#define LOG_MODULE test
#include "log.h"
#include "arm_math.h"

typedef struct
{
	uint8_t a;
	uint32_t b;
} __attribute__((packed)) A;
//typedef struct
//{
//	uint8_t a;
//	uint32_t b;
//} B __attribute__((packed));

void test_before_hal_init()
{
	printf("sizeof(A):%d", sizeof(A));
	//printf("sizeof(B):%d", sizeof(B));
	message_parser_test();
	tree_accessor_test();
}

void test_after_hal_init()
{
}