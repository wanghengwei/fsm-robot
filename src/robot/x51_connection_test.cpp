#include <gtest/gtest.h>
#include "x51_connection.h"
#include <QtCore/QPointer>

TEST(x51_connection_test, test_qpointer) {
    x51::Connection* p = new x51::Connection;
    QPointer<x51::Connection> qp = p;
    ASSERT_TRUE(qp);

    delete p;
    ASSERT_TRUE(!qp);
}