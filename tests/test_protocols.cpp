#include <gtest/gtest.h>

#include "AST/ProtocolDeclarationNode.hpp"

using namespace o2l;

class ProtocolTest : public ::testing::Test {
   protected:
    void SetUp() override {}
};

TEST_F(ProtocolTest, BasicTest) {
    EXPECT_TRUE(true);
}