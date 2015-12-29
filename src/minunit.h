//Adapted from minunit test framework.
#define TEST_ASSERT(test, fail_message) do { if (!(test)) return fail_message; } while (0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s at line %d:  Expected %d, but got %d", \
                __FILE__, __LINE__, expected, actual); \
            tests_failed++; \
            return; \
        } \
    } while (0)

// #define TEST_ASSERT_EQUAL(expected, actual) \
//     do { if ((expected) != (actual)) return "FAIL"; } while (0)

#define RUN_TEST(test) do { setUp(); test(); tearDown(); tests_run++; } while (0)

extern int tests_run;
extern int tests_failed;
