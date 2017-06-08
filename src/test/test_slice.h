#ifndef TEST_SLICE_INC
#define TEST_SLICE_INC
#include "dotest.h"
#include "../util/slice.h"

namespace test {

void test_slice() {
    lightswing::Slice slice("hello world");
    lightswing::Slice slice2(" hello morning");
    lightswing::Slice slice3 = slice + slice2;
    DEBUG_INFO<<slice3;
    EXCEPT_EQ(slice3, "hello world hello morning");
}

}//namespace
#endif // TEST_SLICE_INC

