#include "Math3D/Quaternion.hpp"
#include <cmath>

#include <gtest/gtest.h>
using namespace Math3D;

TEST(QuaternionTest, construct)
{
    Quaternion a;
    Quaternion b(1.0, 2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(1.0, a.w);
    EXPECT_DOUBLE_EQ(0.0, a.x);
    EXPECT_DOUBLE_EQ(0.0, a.y);
    EXPECT_DOUBLE_EQ(0.0, a.z);

    EXPECT_DOUBLE_EQ(1.0, b.w);
    EXPECT_DOUBLE_EQ(2.0, b.x);
    EXPECT_DOUBLE_EQ(3.0, b.y);
    EXPECT_DOUBLE_EQ(4.0, b.z);
}

TEST(QuaternionTest, getNorm)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);
    EXPECT_DOUBLE_EQ(2.0, a.getNorm());
}

TEST(QuaternionTest, from_angle_axis)
{
    Degree angle(120.0);
    Vector3 axis(1.0, 1.0, 1.0);
    Quaternion a(angle, axis);

    EXPECT_DOUBLE_EQ(0.5, a.w);
    EXPECT_DOUBLE_EQ(0.5, a.x);
    EXPECT_DOUBLE_EQ(0.5, a.y);
    EXPECT_DOUBLE_EQ(0.5, a.z);
}

TEST(QuaternionTest, to_angle_axis)
{
    Degree angle;
    Vector3 axis;
    Quaternion a(0.5, 0.5, 0.5, 0.5);
    a.toAngleAxis(angle, axis);
    axis = normalize(axis);
    Vector3 axis_ans(1.0, 1.0, 1.0);
    axis_ans = normalize(axis_ans);
    EXPECT_DOUBLE_EQ(120.0, angle.val);
    EXPECT_EQ(true, axis == axis_ans);
}

TEST(QuaternionTest, from_two_vectors)
{
    Degree angle;
    Vector3 axis;
    Vector3 x(1.0, 0.0, 0.0);
    Vector3 y(0.0, 1.0, 0.0);
    Vector3 z(0.0, 0.0, 1.0);
    Degree ninety(90.0);
    Quaternion a(x, y);
    a.toAngleAxis(angle, axis);
    EXPECT_DOUBLE_EQ(90.0, angle);
    EXPECT_EQ(true, z == axis);
}

TEST(QuaternionTest, from_RPY)
{
    Degree roll(30.0), pitch(15.0), yaw(20.0);
    Quaternion a;
    a.fromRPY(roll, pitch, yaw);
    EXPECT_NEAR(0.937, a.w, 0.001);
    EXPECT_NEAR(0.275, a.x, 0.001);
    EXPECT_NEAR(0.0796, a.y, 0.001);
    EXPECT_NEAR(0.200, a.z, 0.001);
}

TEST(QuaternionTest, to_RPY)
{
    Degree roll, pitch, yaw;
    Quaternion a(0.772, 0.514, 0.210, 0.310);
    a.toRPY(roll, pitch, yaw);
    EXPECT_NEAR(60.0, roll.val, 0.1);
    EXPECT_NEAR(40.0, pitch.val, 0.1);
    EXPECT_NEAR(20.0, yaw.val, 0.1);
}

TEST(QuaternionTest, assignment_operator)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);

    Quaternion b(0.5, 0.5, 0.5, 0.5);

    a += b;
    EXPECT_DOUBLE_EQ(1.5, a.w);
    EXPECT_DOUBLE_EQ(1.5, a.x);
    EXPECT_DOUBLE_EQ(1.5, a.y);
    EXPECT_DOUBLE_EQ(1.5, a.z);

    a -= b;
    EXPECT_DOUBLE_EQ(1.0, a.w);
    EXPECT_DOUBLE_EQ(1.0, a.x);
    EXPECT_DOUBLE_EQ(1.0, a.y);
    EXPECT_DOUBLE_EQ(1.0, a.z);

    a *= b;
    EXPECT_DOUBLE_EQ(-1.0, a.w);
    EXPECT_DOUBLE_EQ(1.0, a.x);
    EXPECT_DOUBLE_EQ(1.0, a.y);
    EXPECT_DOUBLE_EQ(1.0, a.z);
}

TEST(QuaternionTest, add_sub_mult_operator)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);
    Quaternion b(0.5, 0.5, 0.5, 0.5);
    Quaternion buff;

    buff = a + b;
    EXPECT_DOUBLE_EQ(1.5, buff.w);
    EXPECT_DOUBLE_EQ(1.5, buff.x);
    EXPECT_DOUBLE_EQ(1.5, buff.y);
    EXPECT_DOUBLE_EQ(1.5, buff.z);

    buff = a - b;
    EXPECT_DOUBLE_EQ(0.5, buff.w);
    EXPECT_DOUBLE_EQ(0.5, buff.x);
    EXPECT_DOUBLE_EQ(0.5, buff.y);
    EXPECT_DOUBLE_EQ(0.5, buff.z);

    a.set(1.0, 1.0, 1.0, 1.0);
    b.set(0.5, 0.5, 0.5, 0.5);
    buff = a * b;
    EXPECT_DOUBLE_EQ(-1.0, buff.w);
    EXPECT_DOUBLE_EQ(1.0, buff.x);
    EXPECT_DOUBLE_EQ(1.0, buff.y);
    EXPECT_DOUBLE_EQ(1.0, buff.z);
}

TEST(QuaternionTest, comp_operator)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);
    Quaternion b(1.0, 1.0, 1.0, 1.0);
    Quaternion c(0.5, 0.5, 0.5, 0.5);

    EXPECT_EQ(true, a == b);
    EXPECT_EQ(false, a == c);

    EXPECT_EQ(true, a != c);
    EXPECT_EQ(false, a != b);
}

TEST(QuaternionTest, scale)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);
    Quaternion b(2.0, 2.0, 2.0, 2.0);
    EXPECT_EQ(true, b == scale(a, 2));
}

TEST(QuaternionTest, normalize)
{
    Quaternion a(1.0, 1.0, 1.0, 1.0);
    Quaternion b(2.0, 2.0, 2.0, 2.0);

    EXPECT_EQ(true, normalize(a) == normalize(b));
}
