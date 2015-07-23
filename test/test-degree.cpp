#include "Math3D/Degree.hpp"
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES

#include <gtest/gtest.h>
using namespace Math3D;

TEST(DegreeTest, construct)
{
    Degree degree_a;
    Degree degree_b(2.0);
    EXPECT_DOUBLE_EQ(0.0, degree_a.val);
    EXPECT_DOUBLE_EQ(2.0, degree_b.val);
}

TEST(DegreeTest, memberfunc)
{
    Degree degree_a(180.0);
    EXPECT_DOUBLE_EQ(M_PI, degree_a.toRadian());
    degree_a.fromRadian(M_PI / 2.0);
    EXPECT_DOUBLE_EQ(90.0, degree_a.val);
    EXPECT_DOUBLE_EQ(90.0, static_cast<double>(degree_a));
}

TEST(DegreeTest, assignment_operator)
{
    Degree degree_a;
    Degree degree_b(2.0);
    degree_a += degree_b;
    degree_b -= degree_a;
    EXPECT_DOUBLE_EQ(2.0, degree_a.val);
    EXPECT_DOUBLE_EQ(0.0, degree_b.val);
}

TEST(DegreeTest, add_sub_operator)
{
    Degree degree_a;
    Degree degree_b(2.0);
    degree_a = degree_a + degree_b;
    EXPECT_DOUBLE_EQ(2.0, degree_a.val);
    degree_a = degree_a - degree_b;
    EXPECT_DOUBLE_EQ(0.0, degree_a.val);
}

TEST(DegreeTest, comp_operator)
{
    Degree degree_a(1.0);
    Degree degree_b(2.0);
    Degree degree_c(3.0);

    EXPECT_EQ(true, degree_a == degree_a);
    EXPECT_EQ(false, degree_a == degree_b);

    EXPECT_EQ(true, degree_a != degree_b);
    EXPECT_EQ(false, degree_a != degree_a);

    EXPECT_EQ(true, degree_b > degree_a);
    EXPECT_EQ(false, degree_a > degree_a);

    EXPECT_EQ(true, degree_a < degree_b);
    EXPECT_EQ(false, degree_b < degree_a);

    EXPECT_EQ(true, degree_a >= degree_a);
    EXPECT_EQ(true, degree_b >= degree_a);

    EXPECT_EQ(true, degree_a <= degree_a);
    EXPECT_EQ(true, degree_a <= degree_b);
}

TEST(DegreeTest, cos)
{
    Degree a(0.0);
    EXPECT_DOUBLE_EQ(1.0, cos(a));
}

TEST(DegreeTest, sin)
{
    Degree a(90.0);
    EXPECT_DOUBLE_EQ(1.0, sin(a));
}

TEST(DegreeTest, tan)
{
    Degree a(45.0);
    EXPECT_DOUBLE_EQ(1.0, tan(a));
}

TEST(DegreeTest, scale)
{
    Degree a(90.0), b(180.0);
    EXPECT_EQ(b, scale(2.0, a));
}
