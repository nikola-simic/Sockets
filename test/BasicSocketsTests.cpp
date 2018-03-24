#include <gtest\gtest.h>
#include "Sockets.h"

class BasicSocketsTest: public ::testing::Test {
public:
    static void SetUpTestCase() {

    }
    
    static void TearDownTestCase() {

    }

    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(BasicSocketsTest, InitializeSockets) {

    EXPECT_EQ(0, 0);
}

TEST_F(BasicSocketsTest, UninitializeSockets) {

    EXPECT_EQ(0, 0);
}