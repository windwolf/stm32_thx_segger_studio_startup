#ifndef ___UNIT_TEST_H__
#define ___UNIT_TEST_H__

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef UNITTEST
    void test_before_hal_init();
    void test_after_hal_init();

#endif //UNITTEST
#ifdef __cplusplus
}
#endif

#endif // ___UNIT_TEST_H__