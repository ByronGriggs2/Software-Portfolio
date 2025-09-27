#include "pch.h"
#include "PhysicsObjectTestHelper.h"
#include "Capsule.h"
#include "HollowCube.h"
//#include "Cylinder.h"
#include "Plane.h"
#include "Sphere.h"
#include "CollisionManager.h"

using namespace DirectX;
using namespace shapeHelpers;

//constexpr float GRAVITY = -9.81f;
constexpr float TIMESTEP = 0.0001f;
//more generous epsilon for tests involving many simulation steps
constexpr float EPSILONCOMPOUND = (1000.0f / static_cast<float>(TIMESTEP)) * EPSILON;

#define EXPECT_FLOAT_EQL(val1, val2, EPSI) \
do { \
    EXPECT_TRUE(abs((val1) - (val2)) < (EPSI)) << "float " << (val1) << " and float " << (val2) << " are not equal"; \
} while (0);

#define EXPECT_VECTOR3_EQ(vec1, vec2, EPSI) \
    do { \
        EXPECT_FLOAT_EQL(vec1.x, vec2.x, EPSI); \
        EXPECT_FLOAT_EQL(vec1.y, vec2.y, EPSI); \
        EXPECT_FLOAT_EQL(vec1.z, vec2.z, EPSI); \
    } while(0);

#define EXPECT_VECTOR3_NEQ(vec1, vec2, EPSI) \
    do { \
        EXPECT_TRUE(abs(vec1.x-vec2.x) > EPSI || \
abs(vec1.y - vec2.y) > EPSI || \
abs(vec1.z - vec2.z) > EPSI); \
} while (0);

void simulateUntilCollision(PhysicsObject* a, PhysicsObject* b) {
    while (!(a->testIntersect(b)))
    {
        PhysicsObjectTestHelper::kinematics(*a, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(*b, TIMESTEP);
    }
    
    CollisionManager::collisionSimplified(*a, *b, 0);
};
void simulateUntilCollision(PhysicsObject* a, PhysicsObject* b, float timestep) {
    while (!(a->testIntersect(b)))
    {
        PhysicsObjectTestHelper::kinematics(*a, timestep);
        PhysicsObjectTestHelper::kinematics(*b, timestep);
    }
    
    CollisionManager::collisionSimplified(*a, *b, 0);
};
void simulateUntilCollisionAdvanced(PhysicsObject* a, PhysicsObject* b, float timestep) {
    while (!(a->testIntersect(b)))
    {
        PhysicsObjectTestHelper::kinematics(*a, timestep);
        PhysicsObjectTestHelper::kinematics(*b, timestep);
    }
    
    CollisionManager::sphereMassCollision(*a, *b, 0);
}

struct vec3 {
    float x, y, z;
    vec3 operator +=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    vec3 operator *(float other) {
        x *= other;
        y *= other;
        z *= other;
        return *this;
    }
    XMVECTOR xmvect() {
        return XMVectorSet(x, y, z, 0);
    }
    vec3(const XMVECTOR& v) {
        x = XMVectorGetX(v);
        y = XMVectorGetY(v);
        z = XMVectorGetZ(v);
    }
    vec3(float a, float b, float c) {
        x = a;
        y = b;
        z = c;
    }
    vec3() : x(0), y(0), z(0) {}
    vec3 normalize() {
        float temp = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
        x /= temp;
        y /= temp;
        z /= temp;
        return *this;
    }
};
vec3 calculateNewton(const XMVECTOR& acceleration, const XMVECTOR& velocity, const XMVECTOR& position, double time) {
    vec3 newPos(position);
    newPos += vec3(velocity) * static_cast<float>(time);
    newPos += vec3(acceleration) * 0.5 * pow(static_cast<float>(time), 2.0f);
    return newPos;
}

TEST(capsuleMiddleOfFace, capsuleCube) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-1.5, 0, 0, 0);
    Capsule cap(params, 1, 0.01f);

    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(capsuleJuttingIntoFace, capsuleCube) {
    PhysicsObject::constructParams params;
    params._rotation = XMQuaternionRotationRollPitchYaw(0, 0, XM_PIDIV2);
    Capsule cap(params, 3.0001f, 0.001f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(capsuleJuttingIntoFaceNegative, capsuleCube) {
    PhysicsObject::constructParams params;
    params._rotation = XMQuaternionRotationRollPitchYaw(0, 0, -XM_PIDIV2);
    Capsule cap(params, 3.0001f, 0.001f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(capsuleJuttingIntoFaceHighRadius, capsuleCube) {
    PhysicsObject::constructParams params;
    params._rotation = XMQuaternionRotationRollPitchYaw(0, 0, XM_PIDIV2);
    params._position = XMVectorSet(-1.4f, 0, 0, 0);
    Capsule cap(params, 0.0001f, 1.0f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(capsuleJuttingIntoFaceHighRadiusHighHeight, capsuleCube) {
    PhysicsObject::constructParams params;
    params._rotation = XMQuaternionRotationRollPitchYaw(0, 0, XM_PIDIV2);
    params._position = XMVectorSet(-1.4f, 0, 0, 0);
    Capsule cap(params, 3.0001f, 0.5f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(allTheWayThrough, capsuleCube) {
    PhysicsObject::constructParams params;
    params._rotation = XMQuaternionRotationRollPitchYaw(0, 0, XM_PIDIV2);
    Capsule cap(params, 5.0f, 0.01f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(SideRadiusIntoCube, capsuleCube) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 0, -1.4f, 0);
    Capsule cap(params, 1.0f, 0.3f);
    HollowCube cube;

    EXPECT_TRUE(cap.testIntersect(&cube));
    EXPECT_TRUE(cube.testIntersect(&cap));
}

TEST(hardCodeTest, capsuleCube) {
    std::vector<double> raw = { -0.645, -0.405, 0.891,
0.01091,-0.418,-0.391,0.8199,
0.138, 0.1785,

-0.162,-0.666,0.675,
-0.09,-0.964,0.248,-0.023,
0.4,0.393,

-0.132,0.351,0.333,
-0.787581,-0.435,0.337,-0.276,
0.340,0.360,

-0.507,-0.972,-0.513,
-0.616,0.278,-0.236,0.698,
0.267,0.350,

0.816,0.294,1.07,
0.713,0.291,0.555,0.315,
0.114,0.246,

0.357,0.408,-0.753,
-0.830,-0.457,-0.08,0.310,
0.197,0.544,

0.741,-1.143,-0.927,
0.133,-0.699,-0.697,-0.088,
0.277,0.300,

0.090,-0.683,-0.126,
0.363,0.618,-0.621,0.318,
0.250,0.180,

-1.218,0.969,0.0810,
-0.457,0.311,-0.231,0.801,
0.384,0.197 };

    struct testValues {
        float position[3];
        float rotation[4];
        float radius;
        float height;
    };

    std::vector<testValues> testVector;
    for (int i = 0; i < 9; i++) {
        testValues tempTest;
        for (int j = 0; j < 3; j++) {
            tempTest.position[j] = static_cast<float>(raw[9*i+j]);
        }
        for (int j = 0; j < 4; j++) {
            tempTest.rotation[j] = static_cast<float>(raw[9*i+j+3]);
        }
        tempTest.height = static_cast<float>(raw[9*i+7]);
        tempTest.radius = static_cast<float>(raw[9*i+8]);
        testVector.push_back(tempTest);
    }

    std::vector<Capsule> capsuleVector;
    for (auto& test : testVector) {
        PhysicsObject::constructParams tempParams;
        tempParams._position = XMVectorSet(test.position[0], test.position[1], test.position[2], 0);
        tempParams._rotation = XMVectorSet(test.rotation[0], test.rotation[1], test.rotation[2], test.rotation[3]);
        Capsule tempCap(tempParams, test.height, test.radius);
        capsuleVector.push_back(tempCap);
    }

    std::vector<bool> testResults;
    HollowCube cube;
    int i = 0;
    for (auto& item : capsuleVector) {
        if (i == 8) {
            int r = 0;
        }
        testResults.push_back(item.testIntersect(&cube));
        i++;

    }

    for (const auto& result : testResults) {
        EXPECT_FALSE(result);
    }
}

//////////////////////////////////
// Lab 6
//XMVECTOR L6_convertRotationToDirection(const XMVECTOR& rot) {
//    const XMVECTOR axis = XMVector3Normalize(rot);
//    // -1.0f is because torque uses right hand rule and rotation uses left hand rule
//    const float magnitude = -1.0f * XMVectorGetX(XMVector3Dot(rot, axis));
//    const XMVECTOR deltaQuat = XMQuaternionRotationNormal(axis, magnitude);
//    return XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationQuaternion(deltaQuat));
//}
//TEST(basicRoll, Lab6Q3) {
//    Cylinder cyl(XMVectorZero(), XMQuaternionIdentity(), 1, 1);
//    cyl.addTorque(XMVectorSet(XM_PIDIV2, 0, 0, 0));
//    cyl.updateEuler(0.5);
//    cyl.updateEuler(2.0);
//    // expected acceleration = 3pi/2
//    const XMVECTOR expectedVelocity = XMVectorSet(3.0f * XM_PI / 4.0f, 0, 0, 0);
//    const XMVECTOR expectedDisplacement = XMVectorSet(3.0f * XM_PI / 2.0f, 0, 0, 0);
//    const XMVECTOR expectedDirection = L6_convertRotationToDirection(expectedDisplacement);
//
//    EXPECT_VECTOR3_EQ(vec3(expectedDirection), vec3(cyl.getDirection()), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(expectedVelocity), vec3(cyl.getAngularVelocity()), EPSILON);
//}
//TEST(basicPitch, Lab6Q3) {
//    Cylinder cyl(XMVectorZero(), XMQuaternionIdentity(), 1, 1);
//    cyl.addTorque(XMVectorSet(0, XM_PIDIV2, 0, 0));
//    cyl.updateEuler(0.5);
//    cyl.updateEuler(2.0);
//    // expected acceleration = 3pi/2
//    const XMVECTOR expectedVelocity = XMVectorSet(0, 3.0f * XM_PI / 4.0f, 0, 0);
//    const XMVECTOR expectedDisplacement = XMVectorSet(0, 3.0f * XM_PI / 2.0f, 0, 0);
//    const XMVECTOR expectedDirection = XMVectorSet(0, 1, 0, 0);
//
//    EXPECT_VECTOR3_EQ(vec3(expectedDirection), vec3(cyl.getDirection()), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(expectedVelocity), vec3(cyl.getAngularVelocity()), EPSILON);
//}
//TEST(basicYaw, Lab6Q3) {
//    Cylinder cyl(XMVectorZero(), XMQuaternionIdentity(), 1, 1);
//    cyl.addTorque(XMVectorSet(0, 0, XM_PIDIV2, 0));
//    cyl.updateEuler(0.5);
//    cyl.updateEuler(2.0);
//    // expected acceleration = 3pi/2
//    const XMVECTOR expectedVelocity = XMVectorSet(0, 0, XM_PI, 0);
//    const XMVECTOR expectedDisplacement = XMVectorSet(0, 0, 2.0f * XM_PI, 0);
//    const XMVECTOR expectedDirection = L6_convertRotationToDirection(expectedDisplacement);
//
//    EXPECT_VECTOR3_EQ(vec3(expectedDirection), vec3(cyl.getDirection()), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(expectedVelocity), vec3(cyl.getAngularVelocity()), EPSILON);
//}
//TEST(tiltedRoll, Lab6Q3) {
//    {
//        Cylinder cyl;
//        cyl.setDirection(XMVectorSet(1, 0, 0, 0));
//        EXPECT_VECTOR3_EQ(vec3(cyl.getDirection()), vec3(1, 0, 0), EPSILON);
//        cyl.setDirection(XMVectorSet(1, 1, 0, 0));
//        EXPECT_VECTOR3_EQ(vec3(cyl.getDirection()), vec3(1, 1, 0).normalize(), EPSILON);
//
//        const XMMATRIX conversion = cyl.getOrientation();
//        const XMVECTOR testVec = XMVectorSet(0, 1, 0, 0);
//        EXPECT_VECTOR3_EQ(vec3(XMVector3TransformNormal(testVec, conversion)), vec3(cyl.getDirection()), EPSILON);
//
//
//    }
//    {
//        // Create two cylinders. One is aligned with the global frame, the other is tilted by 45 degrees
//        Cylinder cyl1, cyl2;
//        cyl2.setDirection(XMVectorSet(1, 1, 0, 0));
//        const XMMATRIX cyl1ToCyl2 = cyl2.getOrientation();
//        // Test that the orientation transformation matrix can convert from global frame to local frame
//        EXPECT_VECTOR3_EQ(vec3(XMVector3TransformNormal(cyl1.getDirection(), cyl1ToCyl2)), vec3(cyl2.getDirection()), EPSILON);
//
//        // Add torque in local coordinates
//        cyl1.addTorque(XMVectorSet(XM_PIDIV2, 0, 0, 0));
//        cyl2.addTorque(XMVectorSet(XM_PIDIV2, 0, 0, 0));
//        cyl1.updateEuler(1.0);
//        cyl2.updateEuler(1.0);
//
//        // Test that both cylinders have the same local angular velocity
//        EXPECT_VECTOR3_EQ(vec3(cyl1.getAngularVelocity()), vec3(cyl2.getAngularVelocity()), EPSILON);
//
//        // Test that both cylinders have different global rotation
//        EXPECT_VECTOR3_EQ(vec3(cyl1.getDirection()), vec3(0, 1, 0), EPSILON);
//        EXPECT_VECTOR3_EQ(vec3(cyl2.getDirection()), vec3(1, 1, 0).normalize(), EPSILON);
//
//        // Simulate for 1 second with velocities
//        cyl1.updateEuler(1.0);
//        cyl2.updateEuler(1.0);
//
//        EXPECT_VECTOR3_EQ(vec3(cyl1.getDirection()), vec3(cyl2.getDirection()), EPSILON);
//    }
//    Cylinder cyl(XMVectorZero(), XMQuaternionIdentity(), 1, 1);
//    cyl.setDirection(XMVectorSet(1, 1, 0, 0));
//    // Convert from global to local coordinates
//    XMMATRIX transform = cyl.getOrientation();
//    cyl.addTorque(XMVectorSet(XM_PIDIV2, 0, 0, 0));
//    cyl.updateEuler(0.5);
//    EXPECT_VECTOR3_EQ(vec3(3.0 * XM_PIDIV4, 0, 0), vec3(cyl.getAngularVelocity()), EPSILON);
//    cyl.updateEuler(2.0);
//    // expected acceleration = 3pi/2
//    const XMVECTOR expectedVelocity = XMVectorSet(3.0f * XM_PI / 4.0f, 0, 0, 0);
//    const XMVECTOR expectedDisplacement = XMVectorSet(3.0f * XM_PI / 2.0f, 0, 0, 0);
//    XMVECTOR expectedDirection = XMVectorSet(0, 0, 1, 0);
//
//    EXPECT_VECTOR3_EQ(vec3(expectedDirection), vec3(cyl.getDirection()), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(expectedVelocity), vec3(cyl.getAngularVelocity()), EPSILON);
//}
//
//TEST(MassTest, Lab6Q3) {
//    Cylinder s1(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 1);
//    Cylinder s2(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 2);
//    Cylinder s3(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 3);
//    Cylinder s4(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 4);
//    Cylinder s5(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 5);
//
//    s1.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s2.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s3.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s4.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s5.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//
//    // add acceleration
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // simulate movement
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // velocity test
//    const XMVECTOR expectedVelocity = XMVectorSet(3.0 / 5.0 * XM_PI, 0, 0, 0);
//    EXPECT_VECTOR3_EQ(vec3(s1.getAngularVelocity()), vec3(expectedVelocity), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getAngularVelocity()), vec3(expectedVelocity / 2.0f), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getAngularVelocity()), vec3(expectedVelocity / 3.0f), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getAngularVelocity()), vec3(expectedVelocity / 4.0f), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getAngularVelocity()), vec3(expectedVelocity / 5.0f), EPSILON);
//
//    // position test
//    const XMVECTOR expectedDirection1 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-3.0f / 5.0f * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection2 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-3.0f / 5.0f * XM_PI / 2.0f, 0, 0));
//    const XMVECTOR expectedDirection3 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-3.0f / 5.0f * XM_PI / 3.0f, 0, 0));
//    const XMVECTOR expectedDirection4 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-3.0f / 5.0f * XM_PI / 4.0f, 0, 0));
//    const XMVECTOR expectedDirection5 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-3.0f / 5.0f * XM_PI / 5.0f, 0, 0));
//
//    EXPECT_VECTOR3_EQ(vec3(s1.getDirection()), vec3(expectedDirection1), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getDirection()), vec3(expectedDirection2), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getDirection()), vec3(expectedDirection3), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getDirection()), vec3(expectedDirection4), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getDirection()), vec3(expectedDirection5), EPSILON);
//}
//
//TEST(RadiusTest, Lab6Q3) {
//    Cylinder s1(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 1);
//    Cylinder s2(XMVectorZero(), XMQuaternionIdentity(), 2, 1, 1);
//    Cylinder s3(XMVectorZero(), XMQuaternionIdentity(), 3, 1, 1);
//    Cylinder s4(XMVectorZero(), XMQuaternionIdentity(), 4, 1, 1);
//    Cylinder s5(XMVectorZero(), XMQuaternionIdentity(), 5, 1, 1);
//
//    s1.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s2.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s3.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s4.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s5.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//
//    // add acceleration
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // simulate movement
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // velocity test
//    EXPECT_VECTOR3_EQ(vec3(s1.getAngularVelocity()), vec3((3.0f / 5.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getAngularVelocity()), vec3((12.0f / 65.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getAngularVelocity()), vec3((12.0f / 140.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getAngularVelocity()), vec3((12.0f / 245.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getAngularVelocity()), vec3((3.0f / 95.0f) * XM_PI, 0, 0), EPSILON);
//
//    // position test
//    const XMVECTOR expectedDirection1 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(3.0f / 5.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection2 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(12.0f / 65.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection3 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(12.0f / 140.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection4 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(12.0f / 245.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection5 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(3.0f / 95.0f) * XM_PI, 0, 0));
//
//    EXPECT_VECTOR3_EQ(vec3(s1.getDirection()), vec3(expectedDirection1), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getDirection()), vec3(expectedDirection2), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getDirection()), vec3(expectedDirection3), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getDirection()), vec3(expectedDirection4), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getDirection()), vec3(expectedDirection5), EPSILON);
//}
//
//TEST(HeightTest, Lab6Q3) {
//    Cylinder s1(XMVectorZero(), XMQuaternionIdentity(), 1, 1, 1);
//    Cylinder s2(XMVectorZero(), XMQuaternionIdentity(), 1, 2, 1);
//    Cylinder s3(XMVectorZero(), XMQuaternionIdentity(), 1, 3, 1);
//    Cylinder s4(XMVectorZero(), XMQuaternionIdentity(), 1, 4, 1);
//    Cylinder s5(XMVectorZero(), XMQuaternionIdentity(), 1, 5, 1);
//
//    s1.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s2.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s3.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s4.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//    s5.addTorque(XMVectorSet(0.2f * XM_PI, 0, 0, 0));
//
//    // add acceleration
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // simulate movement
//    s1.updateEuler(1.0);
//    s2.updateEuler(1.0);
//    s3.updateEuler(1.0);
//    s4.updateEuler(1.0);
//    s5.updateEuler(1.0);
//
//    // velocity test
//    EXPECT_VECTOR3_EQ(vec3(s1.getAngularVelocity()), vec3((3.0f / 5.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getAngularVelocity()), vec3((12.0f / 35.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getAngularVelocity()), vec3((1.0f / 5.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getAngularVelocity()), vec3((12.0f / 95.0f) * XM_PI, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getAngularVelocity()), vec3((3.0f / 35.0f) * XM_PI, 0, 0), EPSILON);
//
//    // position test
//    const XMVECTOR expectedDirection1 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(3.0f / 5.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection2 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(12.0f / 35.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection3 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(1.0f / 5.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection4 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(12.0f / 95.0f) * XM_PI, 0, 0));
//    const XMVECTOR expectedDirection5 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-(3.0f / 35.0f) * XM_PI, 0, 0));
//
//    EXPECT_VECTOR3_EQ(vec3(s1.getDirection()), vec3(expectedDirection1), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s2.getDirection()), vec3(expectedDirection2), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s3.getDirection()), vec3(expectedDirection3), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s4.getDirection()), vec3(expectedDirection4), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(s5.getDirection()), vec3(expectedDirection5), EPSILON);
//}
//
//TEST(Forces, Lab6Q3) {
//    Cylinder testCylinder;
//    const XMVECTOR point1 = XMVectorSet(-1, 0, 0, 0);
//    const XMVECTOR point2 = XMVectorSet(-1, sqrt(1.0f / 2.0f), 0, 0);
//    const XMVECTOR point3 = XMVectorSet(-1, 0.5, 0, 0);
//
//    testCylinder.addForce(XMVectorSet(3.0f, 0, 0, 0), point1);
//    testCylinder.updateSemiEuler(1.0);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getAngularVelocity()), vec3(0, 0, 0), EPSILON);
//
//    testCylinder.setLinearVelocity(XMVectorZero());
//    testCylinder.setAngularVelocity(XMVectorZero());
//    testCylinder.addForce(XMVectorSet(3.0f, 0, 0, 0), point2);
//    testCylinder.updateSemiEuler(1.0);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getAngularVelocity()), vec3(0, 0, 8.48528), EPSILON);
//
//    testCylinder.setLinearVelocity(XMVectorZero());
//    testCylinder.setAngularVelocity(XMVectorZero());
//    testCylinder.addForce(XMVectorSet(3.0f, 0, 0, 0), point3);
//    testCylinder.updateSemiEuler(1.0);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
//    EXPECT_VECTOR3_EQ(vec3(testCylinder.getAngularVelocity()), vec3(0, 0, 6), EPSILON);
//}

TEST(BasicRollTest, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);
    // Equivalent to accelerating to a yaw of PI/2 over 0.5 seconds
    // A torque of (x,0,0) is an acceleration of I^-1*-x radians/s^2 in roll

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-2.4f * XM_PIDIV2, 0, 0, 0));
    // Note that the sphere does not move in this timestep because oldVelocity = 0
    testSphere.updateEuler(0.5f);

    // Simulate with 0 acceleration
    testSphere.updateEuler(0.5f);
    testSphere.updateEuler(0.5f);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 0, 1), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, -1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 0, -1), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

TEST(BasicPitchTest, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, -2.4 * XM_PIDIV2, 0, 0));
    testSphere.updateEuler(0.5f);

    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

TEST(BasicYawTest, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, -2.4 * XM_PIDIV2, 0));
    testSphere.updateEuler(0.5f);

    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5f);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(-1, 0, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, -1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(1, 0, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesRoll, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);
    //pi/4
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 8.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 8.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 4.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 4.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(3 * (-2.4f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(3 * (-2.4f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-3.0f * XM_PIDIV4, 0, 0), EPSILON);


    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 2.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-2.4f * XM_2PI) / 2.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesPitch, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);
    //pi/4
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PIDIV4, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 8.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 8.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PIDIV2, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 4.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 4.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PI, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 3 * (-2.4f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 3 * (-2.4f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -3.0f * XM_PIDIV4, 0), EPSILON);


    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 2.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-2.4f * XM_2PI) / 2.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_2PI, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesYaw, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 8.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 8.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PIDIV2), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 4.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 4.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PI), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, 3 * (-2.4f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, 3 * (-2.4f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -3.0f * XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 2.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-2.4f * XM_2PI) / 2.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_2PI), EPSILON);
}

// Acceleration done in first timestep, should not change in second timestep
TEST(RollPitchYaw, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    const float val1 = -1.2f * XM_2PI / 8.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val1, val1, val1, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, -XM_PIDIV4, -XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val2 = -1.2f * XM_2PI / 4.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val2, val2, val2, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, -XM_PIDIV2, -XM_PIDIV2), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val3 = -1.2f * XM_2PI / 2.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val3, val3, val3, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, -XM_PI, -XM_PI), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val4 = 3.0f * (-1.2f * XM_2PI) / 8.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val4, val4, val4, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-3.0f * XM_PIDIV4, -3.0f * XM_PIDIV4, -3.0f * XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val5 = -1.2f * XM_2PI;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val5, val5, val5, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, -XM_2PI, -XM_2PI), EPSILON);
}

TEST(DifferentAngles, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    float radian1 = -1.0f * (30.0 / 360.0) * 1.2 * XM_2PI;
    float radian2 = -1.0f * (60.0 / 360.0) * 1.2 * XM_2PI;
    float radian3 = -1.0f * (90.0 / 360.0) * 1.2 * XM_2PI;

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(radian1, radian2, radian3, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI / 12.0f, -XM_2PI / 6.0f, -XM_2PI / 4.0f), EPSILON);
}

TEST(DifferentTimeSteps, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 2.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 2.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 4.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 4.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 8.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 8.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 16.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-1.2f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 16.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    for (int i = 0; i < 360; i++) {
        PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-1.2f * XM_2PI) / 360.0f, 0, 0, 0));
        testSphere.updateEuler(1.0f / 360.0f);
    }
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILONCOMPOUND);
}

TEST(SmallTimestepNewton, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    const XMVECTOR initialVelocity = XMVectorSet(-XM_PI, 0, 0, 0);
    const XMMATRIX initialRotation = XMMatrixRotationRollPitchYaw(XM_PIDIV2, 0, 0);
    const XMVECTOR initialDisplacement = XMQuaternionRotationMatrix(initialRotation);
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, initialVelocity);
    PhysicsObjectTestHelper::addRotationQuat(testSphere, -1.0f * initialDisplacement);
    const float simulationTime = 3.0f;
    const float timestep = 0.003f;

    const XMVECTOR torque = XMVectorSet(-1.2 * XM_2PI, 0, 0, 0);
    const float inertia = (2.0f / 5.0f) * testSphere.getMass() * testSphere.getRadius() * testSphere.getRadius();
    const XMVECTOR expectedAcceleration = torque * (1.0f / inertia);
    const XMVECTOR finalVelocity = simulationTime * expectedAcceleration;
    const XMVECTOR cumulativeRotation = initialVelocity * simulationTime + 0.5f * expectedAcceleration * pow(simulationTime, 2.0f);
    const XMVECTOR axis = XMVector3Normalize(cumulativeRotation);
    // -1.0f is because torque uses right hand rule and rotation uses left hand rule
    const float magnitude = -1.0f * XMVectorGetX(XMVector3Dot(cumulativeRotation, axis));
    const XMVECTOR deltaQuat = XMQuaternionRotationNormal(axis, magnitude);
    const XMVECTOR finalDisplacement = XMQuaternionMultiply(initialDisplacement, deltaQuat);
    const XMVECTOR finalDirection = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationQuaternion(finalDisplacement));

    for (auto i = 0; i < 1000; i++) {
        PhysicsObjectTestHelper::addTorque(testSphere, torque);
        testSphere.updateEuler(0.003f);
    }
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(finalDirection), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(finalVelocity), EPSILONCOMPOUND);
}

TEST(MassTest, Lab6Q2) {
    PhysicsObject::constructParams params1;
    params1._mass = 1.0f;
    Sphere s1(params1, 1.0f);
    PhysicsObject::constructParams params2;
    params2._mass = 2.0f;
    Sphere s2(params2, 1.0f);
    PhysicsObject::constructParams params3;
    params3._mass = 3.0f;
    Sphere s3(params3, 1.0f);
    PhysicsObject::constructParams params4;
    params4._mass = 4.0f;
    Sphere s4(params4, 1.0f);
    PhysicsObject::constructParams params5;
    params5._mass = 5.0f;
    Sphere s5(params5, 1.0f);

    PhysicsObjectTestHelper::addTorque(s1, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s2, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s3, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s4, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s5, XMVectorSet(0.2f * XM_PI, 0, 0, 0));

    // add acceleration
    s1.updateEuler(1.0);
    s2.updateEuler(1.0);
    s3.updateEuler(1.0);
    s4.updateEuler(1.0);
    s5.updateEuler(1.0);

    // simulate movement
    s1.updateEuler(1.0);
    s2.updateEuler(1.0);
    s3.updateEuler(1.0);
    s4.updateEuler(1.0);
    s5.updateEuler(1.0);

    // velocity test
    const XMVECTOR expectedVelocity = XMVectorSet(0.5f * XM_PI, 0, 0, 0);
    EXPECT_VECTOR3_EQ(vec3(s1.getAngularVelocity()), vec3(expectedVelocity), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s2.getAngularVelocity()), vec3(expectedVelocity / 2.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s3.getAngularVelocity()), vec3(expectedVelocity / 3.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s4.getAngularVelocity()), vec3(expectedVelocity / 4.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s5.getAngularVelocity()), vec3(expectedVelocity / 5.0f), EPSILON);

    // position test
    const XMVECTOR expectedDirection1 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI, 0, 0));
    const XMVECTOR expectedDirection2 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 2.0f, 0, 0));
    const XMVECTOR expectedDirection3 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 3.0f, 0, 0));
    const XMVECTOR expectedDirection4 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 4.0f, 0, 0));
    const XMVECTOR expectedDirection5 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 5.0f, 0, 0));

    EXPECT_VECTOR3_EQ(vec3(s1.getDirection()), vec3(expectedDirection1), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s2.getDirection()), vec3(expectedDirection2), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s3.getDirection()), vec3(expectedDirection3), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s4.getDirection()), vec3(expectedDirection4), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s5.getDirection()), vec3(expectedDirection5), EPSILON);
}

TEST(RadiusTest, Lab6Q2) {
    PhysicsObject::constructParams params1;
    Sphere s1(params1, 1.0f);
    PhysicsObject::constructParams params2;
    Sphere s2(params2, 2.0f);
    PhysicsObject::constructParams params3;
    Sphere s3(params3, 3.0f);
    PhysicsObject::constructParams params4;
    Sphere s4(params4, 4.0f);
    PhysicsObject::constructParams params5;
    Sphere s5(params5, 5.0f);

    PhysicsObjectTestHelper::addTorque(s1, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s2, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s3, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s4, XMVectorSet(0.2f * XM_PI, 0, 0, 0));
    PhysicsObjectTestHelper::addTorque(s5, XMVectorSet(0.2f * XM_PI, 0, 0, 0));

    // add acceleration
    s1.updateEuler(1.0);
    s2.updateEuler(1.0);
    s3.updateEuler(1.0);
    s4.updateEuler(1.0);
    s5.updateEuler(1.0);

    // simulate movement
    s1.updateEuler(1.0);
    s2.updateEuler(1.0);
    s3.updateEuler(1.0);
    s4.updateEuler(1.0);
    s5.updateEuler(1.0);

    // velocity test
    const XMVECTOR expectedVelocity = XMVectorSet(0.5f * XM_PI, 0, 0, 0);
    EXPECT_VECTOR3_EQ(vec3(s1.getAngularVelocity()), vec3(expectedVelocity), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s2.getAngularVelocity()), vec3(expectedVelocity / 4.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s3.getAngularVelocity()), vec3(expectedVelocity / 9.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s4.getAngularVelocity()), vec3(expectedVelocity / 16.0f), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s5.getAngularVelocity()), vec3(expectedVelocity / 25.0f), EPSILON);

    // position test
    const XMVECTOR expectedDirection1 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI, 0, 0));
    const XMVECTOR expectedDirection2 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 4.0f, 0, 0));
    const XMVECTOR expectedDirection3 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 9.0f, 0, 0));
    const XMVECTOR expectedDirection4 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 16.0f, 0, 0));
    const XMVECTOR expectedDirection5 = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationRollPitchYaw(-0.5f * XM_PI / 25.0f, 0, 0));

    EXPECT_VECTOR3_EQ(vec3(s1.getDirection()), vec3(expectedDirection1), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s2.getDirection()), vec3(expectedDirection2), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s3.getDirection()), vec3(expectedDirection3), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s4.getDirection()), vec3(expectedDirection4), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(s5.getDirection()), vec3(expectedDirection5), EPSILON);
}

TEST(Forces, Lab6Q2) {
    PhysicsObject::constructParams params;
    params._mass = 3.0f;
    Sphere testSphere(params, 1.0f);

    const XMVECTOR point1 = XMVectorSet(-1, 0, 0, 0);
    const XMVECTOR point2 = XMVectorSet(-sqrt(1.0f / 2.0f), sqrt(1.0f / 2.0f), 0, 0);
    const XMVECTOR point3 = XMVectorSet(-0.5f, sqrt(3.0f) / 2.0f, 0, 0);

    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point1);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(1, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point2);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(1, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 2.5f * sqrt(0.5f)), EPSILON);

    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point3);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(1, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 2.165067f), 100 * EPSILON);
}

TEST(BasicRollTest, Lab6Q1) {
    Sphere testSphere;
    // Equivalent to accelerating to a yaw of PI/2 over 0.5 seconds
    // A torque of (x,0,0) is an acceleration of I^-1*-x radians/s^2 in roll
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.8f * XM_PIDIV2, 0, 0, 0));
    // Note that the sphere does not move in this timestep because oldVelocity = 0
    testSphere.updateEuler(0.5f);

    // Simulate with 0 acceleration
    testSphere.updateEuler(0.5f);
    testSphere.updateEuler(0.5f);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 0, 1), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, -1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 0, -1), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

TEST(BasicPitchTest, Lab6Q1) {
    Sphere testSphere;

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, -0.8 * XM_PIDIV2, 0, 0));
    testSphere.updateEuler(0.5f);

    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

TEST(BasicYawTest, Lab6Q1) {
    Sphere testSphere;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, -0.8 * XM_PIDIV2, 0));
    testSphere.updateEuler(0.5f);

    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5f);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(-1, 0, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, -1, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(1, 0, 0), EPSILON);
    testSphere.updateEuler(0.5);
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(0, 1, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesRoll, Lab6Q1) {
    Sphere testSphere;
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);
    //pi/4
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 8.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 8.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 4.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 4.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(3 * (-0.8f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(3 * (-0.8f * XM_2PI) / 16.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-3.0f * XM_PIDIV4, 0, 0), EPSILON);


    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 2.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.8f * XM_2PI) / 2.0f, 0, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesPitch, Lab6Q1) {
    Sphere testSphere;
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);
    //pi/4
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PIDIV4, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 8.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 8.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PIDIV2, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 4.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 4.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_PI, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 3 * (-0.8f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 3 * (-0.8f * XM_2PI) / 16.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -3.0f * XM_PIDIV4, 0), EPSILON);


    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 2.0f, 0, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, (-0.8f * XM_2PI) / 2.0f, 0, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, -XM_2PI, 0), EPSILON);
}

// This test accelerates over two timesteps
TEST(OtherRadianValuesYaw, Lab6Q1) {
    Sphere testSphere;
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 8.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 8.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PIDIV2), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 4.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 4.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_PI), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, 3 * (-0.8f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, 3 * (-0.8f * XM_2PI) / 16.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -3.0f * XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 2.0f, 0));
    testSphere.updateEuler(0.5);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(0, 0, (-0.8f * XM_2PI) / 2.0f, 0));
    testSphere.updateEuler(0.5);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, -XM_2PI), EPSILON);
}

// Acceleration done in first timestep, should not change in second timestep
TEST(RollPitchYaw, Lab6Q1) {
    Sphere testSphere;

    const float val1 = -0.4f * XM_2PI / 8.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val1, val1, val1, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, -XM_PIDIV4, -XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val2 = -0.4f * XM_2PI / 4.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val2, val2, val2, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, -XM_PIDIV2, -XM_PIDIV2), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val3 = -0.4f * XM_2PI / 2.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val3, val3, val3, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, -XM_PI, -XM_PI), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val4 = 3.0f * (-0.4f * XM_2PI) / 8.0f;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val4, val4, val4, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-3.0f * XM_PIDIV4, -3.0f * XM_PIDIV4, -3.0f * XM_PIDIV4), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    const float val5 = -0.4f * XM_2PI;
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(val5, val5, val5, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, -XM_2PI, -XM_2PI), EPSILON);
}

TEST(DifferentAngles, Lab6Q1) {
    Sphere testSphere;
    float radian1 = -1.0f * (30.0 / 360.0) * 0.4 * XM_2PI;
    float radian2 = -1.0f * (60.0 / 360.0) * 0.4 * XM_2PI;
    float radian3 = -1.0f * (90.0 / 360.0) * 0.4 * XM_2PI;

    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(radian1, radian2, radian3, 0));
    testSphere.updateEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI / 12.0f, -XM_2PI / 6.0f, -XM_2PI / 4.0f), EPSILON);
}

TEST(DifferentTimeSteps, Lab6Q1) {
    Sphere testSphere;

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 2.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 2.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 4.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 4.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PI, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 8.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 8.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV2, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 16.0);
    PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet(-0.4f * XM_2PI, 0, 0, 0));
    testSphere.updateEuler(1.0 / 16.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_PIDIV4, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    for (int i = 0; i < 360; i++) {
        PhysicsObjectTestHelper::addTorque(testSphere, XMVectorSet((-0.4f * XM_2PI) / 360.0f, 0, 0, 0));
        testSphere.updateEuler(1.0f / 360.0f);
    }
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(-XM_2PI, 0, 0), EPSILONCOMPOUND);
}

TEST(SmallTimestepNewton, Lab6Q1) {
    Sphere testSphere;
    const XMVECTOR initialVelocity = XMVectorSet(-XM_PI, 0, 0, 0);
    const XMMATRIX initialRotation = XMMatrixRotationRollPitchYaw(XM_PIDIV2, 0, 0);
    const XMVECTOR initialDisplacement = XMQuaternionRotationMatrix(initialRotation);
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, initialVelocity);
    PhysicsObjectTestHelper::addRotationQuat(testSphere, -1.0f * initialDisplacement);
    const float simulationTime = 3.0f;
    const float timestep = 0.003f;

    const XMVECTOR torque = XMVectorSet(-0.4 * XM_2PI, 0, 0, 0);
    const float inertia = (2.0f / 5.0f) * testSphere.getMass() * testSphere.getRadius() * testSphere.getRadius();
    const XMVECTOR expectedAcceleration = torque * (1.0f / inertia);
    const XMVECTOR finalVelocity = simulationTime * expectedAcceleration;
    const XMVECTOR cumulativeRotation = initialVelocity * simulationTime + 0.5f * expectedAcceleration * pow(simulationTime, 2.0f);
    const XMVECTOR axis = XMVector3Normalize(cumulativeRotation);
    // -1.0f is because torque uses right hand rule and rotation uses left hand rule
    const float magnitude = -1.0f * XMVectorGetX(XMVector3Dot(cumulativeRotation, axis));
    const XMVECTOR deltaQuat = XMQuaternionRotationNormal(axis, magnitude);
    const XMVECTOR finalDisplacement = XMQuaternionMultiply(initialDisplacement, deltaQuat);
    const XMVECTOR finalDirection = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), XMMatrixRotationQuaternion(finalDisplacement));

    for (auto i = 0; i < 1000; i++) {
        PhysicsObjectTestHelper::addTorque(testSphere, torque);
        testSphere.updateEuler(0.003f);
    }
    EXPECT_VECTOR3_EQ(vec3(testSphere.getDirection()), vec3(finalDirection), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(finalVelocity), EPSILONCOMPOUND);
}

TEST(Forces, Lab6Q1) {
    Sphere testSphere;
    const XMVECTOR point1 = XMVectorSet(-1, 0, 0, 0);
    const XMVECTOR point2 = XMVectorSet(-sqrt(1.0f / 2.0f), sqrt(1.0f / 2.0f), 0, 0);
    const XMVECTOR point3 = XMVectorSet(-0.5f, sqrt(3.0f) / 2.0f, 0, 0);

    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point1);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 0), EPSILON);

    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point2);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 7.5f * sqrt(0.5f)), EPSILON);

    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorZero());
    PhysicsObjectTestHelper::setAngularVelocity(testSphere, XMVectorZero());
    testSphere.addForce(XMVectorSet(3.0f, 0, 0, 0), point3);
    testSphere.updateSemiEuler(1.0);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(3, 0, 0), EPSILON);
    EXPECT_VECTOR3_EQ(vec3(testSphere.getAngularVelocity()), vec3(0, 0, 6.4952f), 100 * EPSILON);
}

///////////////////////////////
//L4Q1
TEST(movingSphereFixedPlane, L4_Q1) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-4, 0, 0, 0);
    Sphere sphere1(params, 1.0f);

    PhysicsObjectTestHelper::setLinearVelocity(sphere1, XMVectorSet(3, 0, 0, 0));
    const XMVECTOR quat = directionToQuaternion(XMVectorSet(-1, 0, 0, 0));

    PhysicsObject::constructParams params2;
    params2._fixed = true;
    Plane plane1(params2);
    PhysicsObjectTestHelper::addRotationQuat(plane1, quat);

    vec3 expectedPosSphere1 = calculateNewton(XMVectorZero(), sphere1.getLinearVelocity(), sphere1.getPosition(), 1);
    expectedPosSphere1 = calculateNewton(XMVectorZero(), -sphere1.getLinearVelocity(), expectedPosSphere1.xmvect(), 1);

    vec3 expectedPosPlane1(plane1.getPosition());

    simulateUntilCollision(&sphere1, &plane1);
    for (auto i = 0; i < static_cast<int>(1.0 / TIMESTEP); i++) {
        PhysicsObjectTestHelper::kinematics(sphere1, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(plane1, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(sphere1.getPosition()), expectedPosSphere1, EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(plane1.getPosition()), expectedPosPlane1, EPSILONCOMPOUND);
}

TEST(sphereMovingDiagonallyFixedPlane, L4_Q1) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-4, 0, 0, 0);
    Sphere sphere1(params, 1.0f);

    PhysicsObjectTestHelper::setLinearVelocity(sphere1, XMVectorSet(3, 3, 0, 0));
    const XMVECTOR quat = directionToQuaternion(XMVectorSet(-1, 0, 0, 0));

    PhysicsObject::constructParams params2;
    params2._fixed = true;
    params2._rotation = quat;
    Plane plane1(params2);
    PhysicsObjectTestHelper::addRotationQuat(plane1, quat);

    vec3 expectedPosSphere1 = calculateNewton(XMVectorZero(), sphere1.getLinearVelocity(), sphere1.getPosition(), 1);
    vec3 newVelocity(sphere1.getLinearVelocity());
    newVelocity.x = -newVelocity.x;
    expectedPosSphere1 = calculateNewton(XMVectorZero(), newVelocity.xmvect(), expectedPosSphere1.xmvect(), 1);

    vec3 expectedPosPlane1(plane1.getPosition());

    simulateUntilCollision(&sphere1, &plane1);
    for (auto i = 0; i < static_cast<int>(1.0 / TIMESTEP); i++) {
        PhysicsObjectTestHelper::kinematics(sphere1, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(plane1, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(sphere1.getPosition()), expectedPosSphere1, EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(plane1.getPosition()), expectedPosPlane1, EPSILONCOMPOUND);
}

TEST(movingSphereFixedDiagonalPlane, L4_Q1) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-3-sqrt(0.5f), -1+sqrt(0.5f), 0, 0);
    Sphere sphere1(params, 1.0f);

    PhysicsObjectTestHelper::setLinearVelocity(sphere1, XMVectorSet(2, 0, 0, 0));
    const XMVECTOR quat = directionToQuaternion(XMVectorSet(-1, 1, 0, 0));

    PhysicsObject::constructParams params2;
    params2._fixed = true;
    params2._rotation = quat;
    Plane plane1(params2);
    PhysicsObjectTestHelper::addRotationQuat(plane1, quat);

    const XMVECTOR N = XMVector3Normalize(plane1.closestPointOnPlane(sphere1.getPosition()) - sphere1.getPosition());
    const float NdotV = XMVectorGetX(XMVector3Dot(N, sphere1.getLinearVelocity()));
    const XMVECTOR parallelComponent = N * NdotV;
    const XMVECTOR perpendicularComponent = sphere1.getLinearVelocity() - parallelComponent;

    vec3 expectedPosSphere1 = calculateNewton(XMVectorZero(), sphere1.getLinearVelocity(), sphere1.getPosition(), 1);
    expectedPosSphere1 = calculateNewton(XMVectorZero(), -parallelComponent + perpendicularComponent, expectedPosSphere1.xmvect(), 1);

    vec3 expectedPosPlane1(plane1.getPosition());

    simulateUntilCollision(&sphere1, &plane1);
    for (auto i = 0; i < static_cast<int>(1.0 / TIMESTEP); i++) {
        PhysicsObjectTestHelper::kinematics(sphere1, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(plane1, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(sphere1.getPosition()), expectedPosSphere1, EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(plane1.getPosition()), expectedPosPlane1, EPSILONCOMPOUND);
}

TEST(movingSphereFixedDiagonalPlaneInverted, L4_Q1) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-3 - sqrt(0.5f), -1 + sqrt(0.5f), 0, 0);
    Sphere sphere1(params, 1.0f);

    PhysicsObjectTestHelper::setLinearVelocity(sphere1, XMVectorSet(2, 0, 0, 0));
    const XMVECTOR quat = directionToQuaternion(XMVectorSet(1, -1, 0, 0));

    PhysicsObject::constructParams params2;
    params2._fixed = true;
    params2._rotation = quat;
    Plane plane1(params2);
    PhysicsObjectTestHelper::addRotationQuat(plane1, quat);

    const XMVECTOR N = XMVector3Normalize(plane1.closestPointOnPlane(sphere1.getPosition()) - sphere1.getPosition());
    const float NdotV = XMVectorGetX(XMVector3Dot(N, sphere1.getLinearVelocity()));
    const XMVECTOR parallelComponent = N * NdotV;
    const XMVECTOR perpendicularComponent = sphere1.getLinearVelocity() - parallelComponent;

    vec3 expectedPosSphere1 = calculateNewton(XMVectorZero(), sphere1.getLinearVelocity(), sphere1.getPosition(), 1);
    expectedPosSphere1 = calculateNewton(XMVectorZero(), -parallelComponent + perpendicularComponent, expectedPosSphere1.xmvect(), 1);

    vec3 expectedPosPlane1(plane1.getPosition());

    simulateUntilCollision(&sphere1, &plane1);
    for (auto i = 0; i < static_cast<int>(1.0 / TIMESTEP); i++) {
        PhysicsObjectTestHelper::kinematics(sphere1, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(plane1, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(sphere1.getPosition()), expectedPosSphere1, EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(plane1.getPosition()), expectedPosPlane1, EPSILONCOMPOUND);
}
/////////////////////
//L4 Q2
TEST(OneStillHeadOn, L4_Q2) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, -3, 0, 0);
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 3, 0, 0));

    Sphere otherSphere;

    simulateUntilCollision(&testSphere, &otherSphere);

    PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
    PhysicsObjectTestHelper::kinematics(otherSphere, TIMESTEP);
    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(XMVectorZero()), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(XMVectorSet(3, 3, 0, 0)), EPSILONCOMPOUND);
}

TEST(BothMovingHeadOn, L4_Q2) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, -3, 0, 0);
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 3, 0, 0));

    PhysicsObject::constructParams params2;
    params2._position = XMVectorSet(3, 3, 0, 0);
    Sphere otherSphere(params2, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(otherSphere, XMVectorSet(-3, -3, 0, 0));

    simulateUntilCollision(&testSphere, &otherSphere);

    PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
    PhysicsObjectTestHelper::kinematics(otherSphere, TIMESTEP);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(XMVectorSet(-3, -3, 0, 0)), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(XMVectorSet(3, 3, 0, 0)), EPSILONCOMPOUND);
}

TEST(OneMovingGlancing, L4_Q2) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, sqrt(2.0f), 0, 0);
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    Sphere otherSphere;

    simulateUntilCollision(&testSphere, &otherSphere);

    PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
    PhysicsObjectTestHelper::kinematics(otherSphere, TIMESTEP);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(XMVectorSet(1.5, 1.5, 0, 0)), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(XMVectorSet(1.5, -1.5, 0, 0)), EPSILONCOMPOUND);
}

TEST(BothMovingGlancing, L4_Q2) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, sqrt(2.0f), 0, 0);
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    PhysicsObject::constructParams params2;
    params2._position = XMVectorSet(3, 0, 0, 0);
    Sphere otherSphere(params2, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(otherSphere, XMVectorSet(-6, 0, 0, 0));

    simulateUntilCollision(&testSphere, &otherSphere, TIMESTEP * 0.1f);

    PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP * 0.1f);
    PhysicsObjectTestHelper::kinematics(otherSphere, TIMESTEP * 0.1f);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(XMVectorSet(-1.5, 4.5, 0, 0)), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(XMVectorSet(-1.5, -4.5, 0, 0)), EPSILONCOMPOUND);
}
/////////////////////////////
//Lab 4 Q3
TEST(OneStillHeadOn, L4_Q3) {
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(-3, 0, 0, 0);
    params._mass = 10.0f;
    Sphere testSphere(params, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));
    PhysicsObject::constructParams params2;
    params2._mass = 25.0f;
    Sphere otherSphere(params2, 1.0f);
    simulateUntilCollisionAdvanced(&testSphere, &otherSphere, TIMESTEP);

    testSphere.updateEuler(1);
    otherSphere.updateEuler(1);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(-9.0f / 7.0f, -9.0f / 7.0f, 0), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(12.0f / 7.0f, 12.0f / 7.0f, 0), EPSILONCOMPOUND);
}

TEST(BothMovingHeadOn, L4_Q3) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, -3, 0, 0);
    params1._mass = 10.0f;
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 3, 0, 0));

    PhysicsObject::constructParams params2;
    params2._position = XMVectorSet(3, 3, 0, 0);
    params2._mass = 25.0f;
    Sphere otherSphere(params2, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(otherSphere, XMVectorSet(-3, -3, 0, 0));

    simulateUntilCollisionAdvanced(&testSphere, &otherSphere, TIMESTEP);

    testSphere.updateEuler(1);
    otherSphere.updateEuler(1);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(-39.0f / 7.0f, -39.0f / 7.0f, 0), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(3.0f / 7.0f, 3.0f / 7.0f, 0), EPSILONCOMPOUND);
}

TEST(OneMovingGlancing, L4_Q3) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, sqrt(2.0f), 0, 0);
    params1._mass = 10.0f;
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    PhysicsObject::constructParams params2;
    params2._mass = 25.0f;
    Sphere otherSphere(params2, 1.0f);

    simulateUntilCollisionAdvanced(&testSphere, &otherSphere, TIMESTEP);

    testSphere.updateEuler(1);
    otherSphere.updateEuler(1);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(6.0f / 7.0f, 15.0f / 7.0f, 0), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(6.0f / 7.0f, -6.0f / 7.0f, 0), EPSILONCOMPOUND);
}

TEST(BothMovingGlancing, L4_Q3) {
    PhysicsObject::constructParams params1;
    params1._position = XMVectorSet(-3, sqrt(2.0f), 0, 0);
    params1._mass = 10.0f;
    Sphere testSphere(params1, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    PhysicsObject::constructParams params2;
    params2._position = XMVectorSet(3, 0, 0, 0);
    params2._mass = 25.0f;
    Sphere otherSphere(params2, 1.0f);
    PhysicsObjectTestHelper::setLinearVelocity(otherSphere, XMVectorSet(-3, 0, 0, 0));

    simulateUntilCollisionAdvanced(&testSphere, &otherSphere, TIMESTEP);

    testSphere.updateEuler(1);
    otherSphere.updateEuler(1);

    //test for backstep
    EXPECT_FALSE(testSphere.testIntersect(&otherSphere));
    EXPECT_FALSE(otherSphere.testIntersect(&testSphere));

    //test new velocities
    EXPECT_VECTOR3_EQ(vec3(testSphere.getLinearVelocity()), vec3(-24.0f / 7.0f, 45.0f / 7.0f, 0), EPSILONCOMPOUND);
    EXPECT_VECTOR3_EQ(vec3(otherSphere.getLinearVelocity()), vec3(-24.0f / 7.0f, -18.0f / 7.0f, 0), EPSILONCOMPOUND);
}
///////////////////////////

TEST(basicSphereXEuler, Lab3) {
    Sphere testSphere;
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testSphere.getLinearVelocity(), testSphere.getPosition(), 10 * TIMESTEP);

    //simulate euler
    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
        PhysicsObjectTestHelper::addLinearForce(testSphere, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testSphere, TIMESTEP);
    }

    //test
    EXPECT_VECTOR3_EQ(vec3(testSphere.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(diagonalSphereEuler, Lab3) {
    Sphere testSphere;
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 5, 2, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testSphere.getLinearVelocity(), testSphere.getPosition(), 10 * TIMESTEP);

    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
        PhysicsObjectTestHelper::addLinearForce(testSphere, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testSphere, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(testSphere.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(diagonalPlanSemiEuler, Lab3) {
    Plane testPlane;
    PhysicsObjectTestHelper::setDirection(testPlane, XMVectorSet(1, 0, 0, 0));
    PhysicsObjectTestHelper::setLinearVelocity(testPlane, XMVectorSet(3, 5, 2, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testPlane.getLinearVelocity(), testPlane.getPosition(), 10 * TIMESTEP);

    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::kinematics(testPlane, TIMESTEP);
        PhysicsObjectTestHelper::addLinearForce(testPlane, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testPlane, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(testPlane.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(basicSphereXSemiEuler, Lab3) {
    Sphere testSphere;
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 0, 0, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testSphere.getLinearVelocity(), testSphere.getPosition(), 10 * TIMESTEP);

    //simulate semi-implicit euler
    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::addLinearForce(testSphere, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testSphere, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
    }

    //test
    EXPECT_VECTOR3_EQ(vec3(testSphere.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(diagonalSphereSemiEuler, Lab3) {
    Sphere testSphere;
    PhysicsObjectTestHelper::setLinearVelocity(testSphere, XMVectorSet(3, 5, 2, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testSphere.getLinearVelocity(), testSphere.getPosition(), 10 * TIMESTEP);

    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::addLinearForce(testSphere, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testSphere, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(testSphere, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(testSphere.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(diagonalPlaneSemiEuler, Lab3) {
    Plane testPlane;
    PhysicsObjectTestHelper::setDirection(testPlane, XMVectorSet(1, 0, 0, 0));
    PhysicsObjectTestHelper::setLinearVelocity(testPlane, XMVectorSet(3, 5, 2, 0));

    //newtonian
    vec3 expectedPos = calculateNewton(XMVectorSet(0, GRAVITY, 0, 0), testPlane.getLinearVelocity(), testPlane.getPosition(), 10 * TIMESTEP);

    for (auto i = 0; i < 10; i++) {
        PhysicsObjectTestHelper::addLinearForce(testPlane, XMVectorSet(0, GRAVITY, 0, 0));
        PhysicsObjectTestHelper::resolveForceAndTorque(testPlane, TIMESTEP);
        PhysicsObjectTestHelper::kinematics(testPlane, TIMESTEP);
    }

    EXPECT_VECTOR3_EQ(vec3(testPlane.getPosition()), expectedPos, EPSILONCOMPOUND);
}

TEST(CapsuleFullyInsideSphere, CapsuleIntersect) {
    Sphere testSphere(5.0f);
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(1, 0, 0, 0);
    Capsule testCapsule(params, 1, 1);
    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(CapsulePokingOutOfSphere, CapsuleIntersect) {
    Sphere testSphere(2.0f);
    Capsule testCapsule(10,1);
    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(OneSideOfCapsuleInSphere, CapsuleIntersect) {
    Sphere testSphere(2.0f);
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(5, 0, 0, 0);
    Capsule testCapsule(params, 10,1);
    const XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0, 0, -XM_PIDIV2);
    PhysicsObjectTestHelper::addRotationQuat(testCapsule, quat);
    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(TipOfCapsuleTouchingSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(6.9999f, 0, 0, 0);
    Capsule testCapsule(params, 10,1);
    const XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0, 0, -XM_PIDIV2);
    PhysicsObjectTestHelper::addRotationQuat(testCapsule, quat);

    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(WallOfCapsuleTouchingSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(1.9999f, 0, 0, 0);
    Capsule testCapsule(params, 10,1);
    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(PartialIntersectSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(1.9999f, 0, 0, 0);
    Capsule testCapsule(params, 10,1.5);
    EXPECT_TRUE(testSphere.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testSphere));
}

TEST(OnSameAxisOutsideSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 4.001f, 0, 0);
    Capsule testCapsule(params, 3, 1);
    EXPECT_FALSE(testSphere.testIntersect(&testCapsule));
    EXPECT_FALSE(testCapsule.testIntersect(&testSphere));
}

TEST(TotallyFarSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(100, 100, 100, 0);
    Capsule testCapsule(params, 4,1);
    EXPECT_FALSE(testSphere.testIntersect(&testCapsule));
    EXPECT_FALSE(testCapsule.testIntersect(&testSphere));
}

TEST(WallOfCapsuleNotTouchingSphere, CapsuleIntersect) {
    Sphere testSphere;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(7.0001f, 0, 0, 0);
    Capsule testCapsule(params, 10,1);
    EXPECT_FALSE(testSphere.testIntersect(&testCapsule));
    EXPECT_FALSE(testCapsule.testIntersect(&testSphere));
}

TEST(CapsuleThroughPlane, CapsuleIntersect) {
    Plane testPlane;
    Capsule testCapsule(4,1);
    EXPECT_TRUE(testPlane.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testPlane));
}

TEST(CapsuleTouchPlane, CapsuleIntersect) {
    Plane testPlane;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 2.9999f, 0, 0);
    Capsule testCapsule(params, 4,1);
    EXPECT_TRUE(testPlane.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testPlane));
}

TEST(CapsuleNotTouchPlane, CapsuleIntersect) {
    Plane testPlane;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 3.001f, 0, 0);
    Capsule testCapsule(params, 4,1);
    EXPECT_FALSE(testPlane.testIntersect(&testCapsule));
    EXPECT_FALSE(testCapsule.testIntersect(&testPlane));
}

TEST(CapsuleWallTouchPlane, CapsuleIntersect) {
    Plane testPlane;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 0.9999f, 0, 0);
    Capsule testCapsule(params, 4,1);
    EXPECT_TRUE(testPlane.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testPlane));
}

TEST(CapsuleWallNotTouchPlane, CapsuleIntersect) {
    Plane testPlane;
    PhysicsObject::constructParams params;
    params._position = XMVectorSet(0, 0.9999f, 0, 0);
    Capsule testCapsule(params, 4,1);
    PhysicsObjectTestHelper::setDirection(testCapsule, XMVectorSet(1, 0, 0, 0));
    EXPECT_TRUE(testPlane.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testPlane));
}

TEST(CapsuleDiagonalThroughPlane, CapsuleIntersect) {
    Plane testPlane;
    Capsule testCapsule(4,1);
    PhysicsObjectTestHelper::setDirection(testCapsule, XMVectorSet(1, 0, 0, 0));
    EXPECT_TRUE(testPlane.testIntersect(&testCapsule));
    EXPECT_TRUE(testCapsule.testIntersect(&testPlane));
}