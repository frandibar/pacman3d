#pragma once

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class SocketsTest : public CppUnit::TestFixture 
{
    CPPUNIT_TEST_SUITE(SocketsTest);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

    void testConnect(); 
};

