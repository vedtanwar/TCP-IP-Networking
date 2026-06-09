#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <gtest/gtest.h>
#include "log.h"

struct LoggingFixture : testing::Test
{
    static void SetUpTestSuite()
    {
        Logger::init();
    }

    static void TearDownTestSuite()
    {
        Logger::shutdown();
    }
};

#endif // TEST_COMMON_H
