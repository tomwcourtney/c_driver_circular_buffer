#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../../inc/circular_buffer.h"
}

#define TEST_CAPACITY 5

TEST_GROUP(CircBufTest) 
{
    circular_buffer_t cb;
    uint8_t buffer[TEST_CAPACITY];
    uint32_t capacity = TEST_CAPACITY;
    void setup()
    {
        circular_buffer_init(&cb, buffer, capacity);
    }

    void teardown()
    {
    }

};

/********/
/* ZERO */
/********/

TEST(CircBufTest, init_buffer_buffer_empty)
{
    CHECK_TRUE(circular_buffer_empty(&cb));
}

TEST(CircBufTest, init_buffer_not_full)
{
    CHECK_FALSE(circular_buffer_full(&cb));
}

TEST(CircBufTest, get_from_empty_buffer_returns_error_and_value_unchanged)
{
    uint8_t val = 32;
    LONGS_EQUAL(CB_BUFFER_EMPTY, circular_buffer_get(&cb, &val));
    LONGS_EQUAL(32, val);
}

TEST(CircBufTest, get_from_empty_buffer_still_empty)
{
    uint8_t val;
    circular_buffer_get(&cb, &val);
    CHECK_TRUE(circular_buffer_empty(&cb));
}

TEST(CircBufTest, empty_buffer_has_size_zero)
{
    LONGS_EQUAL(0, circular_buffer_size(&cb));
}

TEST(CircBufTest, get_from_empty_buffer_size_still_zero)
{
    uint8_t val;
    circular_buffer_get(&cb, &val);
    LONGS_EQUAL(0, circular_buffer_size(&cb));
}

TEST(CircBufTest, initialise_buffer_with_capacity_zero_returns_error)
{
    LONGS_EQUAL(CB_ZERO_CAPACITY, circular_buffer_init(&cb, buffer, 0));
}

/********/
/* ONE  */
/********/

TEST(CircBufTest, put_to_empty_buffer_buffer_not_empty)
{
    circular_buffer_put(&cb, 3);
    CHECK_FALSE(circular_buffer_empty(&cb));
}

TEST(CircBufTest, put_to_buffer_get_from_buffer_put_equals_get)
{
    uint8_t put = 22;
    uint8_t get;

    LONGS_EQUAL(CB_OK, circular_buffer_put(&cb, put));
    LONGS_EQUAL(CB_OK, circular_buffer_get(&cb, &get));

    LONGS_EQUAL(put, get);
}

TEST(CircBufTest, put_then_get_buffer_is_still_empty)
{
    uint8_t put = 22;
    uint8_t get;

    circular_buffer_put(&cb, put);
    circular_buffer_get(&cb, &get);

    CHECK_TRUE(circular_buffer_empty(&cb));
}

/********/
/* MANY */
/********/

TEST(CircBufTest, put_then_get_twice_equals)
{
    uint8_t put0 = 22;
    uint8_t put1 = 45;
    uint8_t get0;
    uint8_t get1;

    circular_buffer_put(&cb, put0);
    circular_buffer_put(&cb, put1);

    circular_buffer_get(&cb, &get0);
    circular_buffer_get(&cb, &get1);

    // circular buffer is FIFO, so we want the first put to equal the first get.
    LONGS_EQUAL(put0, get0);
    LONGS_EQUAL(put1, get1);
}

TEST(CircBufTest, fill_buffer_check_full)
{
    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb, 2);
    }

    CHECK_TRUE(circular_buffer_full(&cb));
}

TEST(CircBufTest, full_buffer_cant_put)
{
    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb, 2);
    }

    LONGS_EQUAL(CB_BUFFER_FULL, circular_buffer_put(&cb, 2));
}

TEST(CircBufTest, fill_buffer_then_get_one_then_put)
{
    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb, 2);
    }

    uint8_t val;
    circular_buffer_get(&cb, &val);

    LONGS_EQUAL(CB_OK, circular_buffer_put(&cb, 3));
}

TEST(CircBufTest, fill_buffer_empty_buffer_compare_in_vs_out)
{
    uint8_t test_puts[] = {1, 3, 5, 7, 9};
    uint8_t test_gets[TEST_CAPACITY] = {0};

    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb, test_puts[i]);
    }

    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_get(&cb, &(test_gets[i]));
    }

    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        LONGS_EQUAL(test_puts[i], test_gets[i]);
    }
}

TEST(CircBufTest, fill_different_buffers_make_sure_they_dont_interfere)
{
    circular_buffer_t cb0;
    uint8_t buffer0[TEST_CAPACITY] = {0};
    circular_buffer_init(&cb0, buffer0, TEST_CAPACITY);
    circular_buffer_t cb1;
    uint8_t buffer1[TEST_CAPACITY] = {0};
    circular_buffer_init(&cb1, buffer1, TEST_CAPACITY);

    uint8_t test_puts0[] = {1, 3, 5, 7, 9};
    uint8_t test_puts1[] = {11, 13, 15, 17, 19};

    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb0, test_puts0[i]);
        circular_buffer_put(&cb1, test_puts1[i]);
    }

    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        uint8_t test_get0;
        uint8_t test_get1;
        circular_buffer_get(&cb0, &test_get0);
        circular_buffer_get(&cb1, &test_get1);
        LONGS_EQUAL(test_puts0[i], test_get0);
        LONGS_EQUAL(test_puts1[i], test_get1);
    }
}

TEST(CircBufTest, two_buffers_one_empty_one_not)
{
    circular_buffer_t cb0;
    uint8_t buffer0[TEST_CAPACITY] = {0};
    circular_buffer_init(&cb0, buffer0, TEST_CAPACITY);
    circular_buffer_t cb1;
    uint8_t buffer1[TEST_CAPACITY] = {0};
    circular_buffer_init(&cb1, buffer1, TEST_CAPACITY);

    circular_buffer_put(&cb0, 2);

    CHECK_FALSE(circular_buffer_empty(&cb0));
    CHECK_TRUE(circular_buffer_empty(&cb1));
}

TEST(CircBufTest, head_wraps_around)
{
    // provide a buffer that is two bigger than the capacity so we can peek the edge case
    circular_buffer_t cb0;
    uint8_t buffer0[TEST_CAPACITY + 2] = {0};
    circular_buffer_init(&cb0, buffer0, TEST_CAPACITY);

    // add two elements to buffer
    circular_buffer_put(&cb0, 1);
    circular_buffer_put(&cb0, 1);

    // remove trhem
    uint8_t val;
    circular_buffer_get(&cb0, &val);
    circular_buffer_get(&cb0, &val);
    
    // add elements to capacity
    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb0, 2);
    }

    // last two elements of raw buffer should still be zero, but first two elements of raw buffer should be 2
    LONGS_EQUAL(0, buffer0[TEST_CAPACITY]);
    LONGS_EQUAL(0, buffer0[TEST_CAPACITY + 1]);
    LONGS_EQUAL(2, buffer0[0]);
    LONGS_EQUAL(2, buffer0[1]);
}

TEST(CircBufTest, tail_wraps_around)
{
    // from previous test we know head is wrapping around. so we will get the head/tail situation like so (where X means head and tail pointer in same position (empty buffer))
    //       X
    // 0 1 2 3 _
    //
    // then add 3 elements
    //   H   T
    // 9 1 2 7 8
    // then get 3 elements and make sure they're 7, 8, 9. if tail doesn't wrap it will begin reading outside of the capacity area, which will be zero'd by using an oversized array

    circular_buffer_t cb0;
    uint8_t buffer0[TEST_CAPACITY + 1] = {0};
    circular_buffer_init(&cb0, buffer0, TEST_CAPACITY);

    // add 4 elements to buffer
    for (int i = 0; i < 4; i++)
    {
        circular_buffer_put(&cb0, 1);
    }

    // remove 4 elements
    for (int i = 0; i < 4; i++)
    {
        uint8_t val;
        circular_buffer_get(&cb0, &val);
    }

    // add another 3
    for (int i = 0; i < 3; i++)
    {
        circular_buffer_put(&cb0, 2);
    }

    // remove another 3, ensure they're all == 2
    for (int i = 0; i < 3; i++)
    {
        uint8_t val;
        circular_buffer_get(&cb0, &val);
        LONGS_EQUAL(2, val);
    }
}

TEST(CircBufTest, fill_buffer_empty_it_fill_again_wraparound)
{
    for (int i = 0; i < TEST_CAPACITY * 20; i++)
    {
        circular_buffer_put(&cb, i % 255);
        uint8_t val;
        circular_buffer_get(&cb, &val);
        LONGS_EQUAL(i % 255, val);
    }
}

TEST(CircBufTest, fill_and_empty_buffer_size_is_correct)
{
    for (int i = 0; i < TEST_CAPACITY; i++)
    {
        circular_buffer_put(&cb, 2);
        LONGS_EQUAL(i + 1, circular_buffer_size(&cb));
    }

    for (int i = TEST_CAPACITY; i > 0; i--)
    {
        uint8_t val;
        circular_buffer_get(&cb, &val);
        LONGS_EQUAL(i - 1, circular_buffer_size(&cb));
    }
}