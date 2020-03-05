// ConsoleApplication2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "mymath.h"
#include "geometryhelper.h"

#include "Validator.h"

using namespace mymath;

#define PI 3.14159265

class Assert {

public:
    static void AssertEqual(float v1, float v2, float acceptableErrorPercentage) {

        float errorPercentage = (v1 - v2) * 100.0f;

        if (v1 == v2)
            return;

        if (errorPercentage <= acceptableErrorPercentage)
            return;

        printf("!!!!Test Failed!!!!.   AssertEqual(float %7.7f, float %7.7f, float %7.7f )\n", v1, v2, acceptableErrorPercentage);

    }


};

Matrixf4x4 GetRotateMatrix4x4(float rotX, float rotY, float rotZ) {

    // Rotate about x-axis.
    Matrixf4x4 rotateAboutXMatriX = rotate(1.0f, 0.0f, 0.0f, rotX * (PI / 180.0f));

    // Rotate about y-axis.
    Matrixf4x4 rotateAboutYMatriY = rotate(0.0f, 1.0f, 0.0f, rotY * (PI / 180.0f));

    // Rotate about z-axis.
    Matrixf4x4 rotateAboutZMatriZ = rotate(0.0f, 0.0f, 1.0f, rotZ * (PI / 180.0f));

    // Combine all.
    Matrixf4x4 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

    return rotateMatrix;
}

bool GetEulerAngle(Matrixf3x3 rotMatInW, float* rotX, float* rotY, float* rotZ)
{
    float yAxis, xAxis, zAxis;

    float sp = -rotMatInW[2][1];
    if (sp <= -1.0f) {
        yAxis = -1.570796f;
    }
    else if (sp >= 1.0f) {
        yAxis = 1.570796f;
    }
    else {
        yAxis = asin(sp);
    }

    if (fabs(sp) > 0.9999f) {
        zAxis = 0.0f;
        xAxis = atan2f(-rotMatInW[0][2], rotMatInW[0][0]);
    }
    else {
        xAxis = atan2f(rotMatInW[2][0], rotMatInW[2][2]);
        zAxis = atan2f(rotMatInW[0][1], rotMatInW[1][1]);
    }

    *rotX = xAxis * (180.f / PI );
    *rotY = yAxis * (180.f / PI);
    *rotZ = zAxis * (180.f / PI);

    return true;
}

Matrixf4x4 genLoc4x4Mat( float x, float y, float z) {

    Matrixf4x4 translationMatrix = { { 1, 0, 0, 0},
                                     { 0, 1, 0, 0},
                                     { 0, 0, 1, 0},
                                     { x, y, z, 1} };

    return translationMatrix;
}

Matrixf3x3 genRot3x3Mat( Vectorf3 inAxis, float rotInDeg ){

    // Convert from degree to radian measure.
    float rot = rotInDeg * (M_PI / 180);

    // for x-axis.
    Vectorf3 xAxis{ 1,0,0 };
    Vectorf3 p = { (xAxis * inAxis) * inAxis + (xAxis - (xAxis * inAxis) * inAxis) * cos(rot) + inAxis.cross(xAxis) * sin(rot) };

    // for y-axis.
    Vectorf3 yAxis{ 0,1,0 };
    Vectorf3 q = { (yAxis * inAxis) * inAxis + (yAxis - (yAxis * inAxis) * inAxis) * cos(rot) + inAxis.cross(yAxis) * sin(rot) };

    // for z-axis.
    Vectorf3 zAxis{ 0,0,1 };
    Vectorf3 r = { (zAxis * inAxis) * inAxis + (zAxis - (zAxis * inAxis) * inAxis) * cos(rot) + inAxis.cross(zAxis) * sin(rot) };

    Matrixf3x3 rotationMatrix = { p, q, r };

    return rotationMatrix;

}

Matrixf3x3 genRot3x3Mat(float xRotInDeg, float yRotInDeg, float zRotInDeg) {

    Vectorf3 xAxis{ 1,0,0 };
    Matrixf3x3 xRotMat = genRot3x3Mat(xAxis, xRotInDeg);

    Vectorf3 yAxis{ 0,1,0 };
    Matrixf3x3 yRotMat = genRot3x3Mat(yAxis, yRotInDeg);

    Vectorf3 zAxis{ 0,0,1 };
    Matrixf3x3 zRotMat = genRot3x3Mat(zAxis, zRotInDeg);

    return xRotMat * yRotMat * zRotMat;


}

Matrixf4x4 genRot4x4Mat(float xRotInDeg, float yRotInDeg, float zRotInDeg) {

    Matrixf3x3 rotMat = genRot3x3Mat(xRotInDeg, yRotInDeg, zRotInDeg);
    Matrixf4x4 ret = { rotMat };
    return ret;
}

// For Unreal Engine.
Matrixf4x4 genRot4x4MatForUE(float xRotInDeg, float yRotInDeg, float zRotInDeg) {

    // In Unreal Engine, the axises for x and y rotate in the direction opposing to coordinate system. 
    Matrixf3x3 rotMat = genRot3x3Mat(-xRotInDeg, -yRotInDeg, zRotInDeg);
    Matrixf4x4 ret = { rotMat };
    return ret;
}

void test1() {

    Matrixf4x4 matCr = genRot4x4MatForUE(-22, 30, 0);
    Matrixf4x4 matCl = genLoc4x4Mat(3.3, 0, 0);

    Matrixf4x4 matAcr = genRot4x4MatForUE(0, 0, 0);
    Matrixf4x4 matAcl = genLoc4x4Mat(-300, 0, 0);

    Matrixf4x4 matAr = genRot4x4MatForUE(0, -20, 0);
    Matrixf4x4 matAl = genLoc4x4Mat(-136, 6.6, 151);

    Matrixf4x4 matOr = genRot4x4MatForUE(0, 0, -50);
    Matrixf4x4 matOl = genLoc4x4Mat(0, 0, 0);

    Matrixf4x4 transformMat = matCr * matCl * matAcr * matAcl * matAr * matAl * matOr * matOl;

    Matrixf3x3 transform3x3Mat = transformMat.homogeniousDivide();
    std::cout << "\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j + 1 < 3) {
                printf("%7.3f ", transform3x3Mat[i][j]);
            }
            else {
                printf("%7.3f \n", transform3x3Mat[i][j]);
            }
        }
    }

    Vectorf3 cameraLocal3Position = { 0, 0, 0 };
    Vectorf4 cameraLocal4Position = { cameraLocal3Position };

    Vectorf4 cameraWorld4Position = cameraLocal4Position * transformMat;
    std::cout << "\n";

    for (int i = 0; i < 4; i++) {
        if (i + 1 < 4) {
            printf("%7.3f ", cameraWorld4Position[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld4Position[i]);
        }
    }

    Vectorf3 cameraWorld3PositionHomogeniousDivided = cameraWorld4Position.homogeniousDivide();
    std::cout << "\n";

    for (int i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            printf("%7.3f ", cameraWorld3PositionHomogeniousDivided[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld3PositionHomogeniousDivided[i]);
        }
    }

}

void test2() {

    Matrixf4x4 matCr = genRot4x4MatForUE(10, 20, 30);
    Matrixf4x4 matCl = genLoc4x4Mat(25, 35, 15);

    Matrixf4x4 matAcr = genRot4x4MatForUE(0, 0, 0);
    Matrixf4x4 matAcl = genLoc4x4Mat(-300, 0, 0);

    Matrixf4x4 matAr = genRot4x4MatForUE( 120, 130, 140 );
    Matrixf4x4 matAl = genLoc4x4Mat( 70, 80, 90);

    Matrixf4x4 matOr = genRot4x4MatForUE(-35, 52, -16);
    Matrixf4x4 matOl = genLoc4x4Mat(50, 75, 80);

    Matrixf4x4 transformMat = matCr * matCl * matAcr * matAcl * matAr * matAl * matOr * matOl;

    Matrixf3x3 transform3x3Mat = transformMat.homogeniousDivide();

    std::cout << "\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j + 1 < 3) {
                printf("%7.3f ", transform3x3Mat[i][j]);
            }
            else {
                printf("%7.3f \n", transform3x3Mat[i][j]);
            }
        }
    }

    Vectorf3 cameraLocal3Position = { 0, 0, 0 };
    Vectorf4 cameraLocal4Position = { cameraLocal3Position };

    Vectorf4 cameraWorld4Position = cameraLocal4Position * transformMat;

    std::cout << "\n";
    for (int i = 0; i < 4; i++) {
        if (i + 1 < 4) {
            printf("%7.3f ", cameraWorld4Position[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld4Position[i]);
        }
    }

    std::cout << "\n";

    Vectorf3 cameraWorld3PositionHomogeniousDivided = cameraWorld4Position.homogeniousDivide();

    for (int i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            printf("%7.3f ", cameraWorld3PositionHomogeniousDivided[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld3PositionHomogeniousDivided[i]);
        }
    }

}

void test3() {

    Matrixf4x4 matCr = genRot4x4MatForUE(55, 66, 77);
    Matrixf4x4 matCl = genLoc4x4Mat(10, 20, 30);

    Matrixf4x4 matAcr = genRot4x4MatForUE(10, 20, 30);
    Matrixf4x4 matAcl = genLoc4x4Mat(44, 55, 99);

    Matrixf4x4 matOr = genRot4x4MatForUE(101, 140, 160);
    Matrixf4x4 matOl = genLoc4x4Mat(70, 80, 90);

    Matrixf4x4 transformMat = matCr * matCl * matAcr * matAcl * matOr * matOl;

    Matrixf3x3 transform3x3Mat = transformMat.homogeniousDivide();

    std::cout << "\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j + 1 < 3) {
                printf("%7.3f ", transform3x3Mat[i][j]);
            }
            else {
                printf("%7.3f \n", transform3x3Mat[i][j]);
            }
        }
    }

    Vectorf3 cameraLocal3Position = { 0, 0, 0 };
    Vectorf4 cameraLocal4Position = { cameraLocal3Position };

    Vectorf4 cameraWorld4Position = cameraLocal4Position * transformMat;

    std::cout << "\n";
    for (int i = 0; i < 4; i++) {
        if (i + 1 < 4) {
            printf("%7.3f ", cameraWorld4Position[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld4Position[i]);
        }
    }

    std::cout << "\n";

    Vectorf3 cameraWorld3PositionHomogeniousDivided = cameraWorld4Position.homogeniousDivide();

    for (int i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            printf("%7.3f ", cameraWorld3PositionHomogeniousDivided[i]);
        }
        else {
            printf("%7.3f \n", cameraWorld3PositionHomogeniousDivided[i]);
        }
    }

}

void test4() {

    Matrixf3x3 mat = {  { -0.1495f, -0.1986f, -0.9685f },
                        { -0.8256f,  0.5640f,  0.0117f },
                        { -0.5439f, -0.8015f,  0.2484f } };


    Matrixf3x3 orth = mat.orthogonilize();
    Matrixf3x3 norm = mat.normalize();
    Matrixf3x3 trans = norm.transpose();

    Matrixf3x3 iden = mat * trans;

    std::cout << "before orthogonilize\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", mat[h][w]);
            }
            else {
                printf("%7.3f \n", mat[h][w]);
            }
        }
    }

    std::cout << "orthogonilize\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", orth[h][w]);
            }
            else {
                printf("%7.3f \n", orth[h][w]);
            }
        }
    }

    std::cout << "validation\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", iden[h][w]);
            }
            else {
                printf("%7.3f \n", iden[h][w]);
            }
        }
    }




}

void test4_2() {

    Matrixf3x3 mat = {  { -0.7495f, -0.1986f, -0.3685f },
                        { -0.8256f,  0.5640f,  0.0117f },
                        { -0.5439f, -0.8015f,  0.3484f } };


    Matrixf3x3 orth = mat.orthogonilize();
    Matrixf3x3 norm = orth.normalize();
    Matrixf3x3 trans = norm.transpose();

    Matrixf3x3 iden = norm * trans;


    std::cout << "before orthogonilize\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", mat[h][w]);
            }
            else {
                printf("%7.3f \n", mat[h][w]);
            }
        }
    }

    std::cout << "orthogonilize\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", orth[h][w]);
            }
            else {
                printf("%7.3f \n", orth[h][w]);
            }
        }
    }

    std::cout << "normalize\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", norm[h][w]);
            }
            else {
                printf("%7.3f \n", norm[h][w]);
            }
        }
    }

    std::cout << "validation\n";
    for (int h = 0; h < 3; h++) {
        for (int w = 0; w < 3; w++) {
            if (w + 1 < 3) {
                printf("%7.3f ", iden[h][w]);
            }
            else {
                printf("%7.3f \n", iden[h][w]);
            }
        }
    }





}

void test5() {

    Vectorf2 vec1 = { 12.0f, 5.0f };
    Vectorf2 vec2 = { 0.0f, 743.632f };
    Vectorf3 vec3 = { 8.0f, -3.0f, 1.0f/2.0f };
    Vectorf3 vec4 = { -12.0f, 3.0f, -4.0f };
    Vectorf4 vec5 = { 1.0f, 1.0f, 1.0f, 1.0f };

    Vectorf2 vec1Norm = vec1.normalize();
    Vectorf2 vec2Norm = vec2.normalize();
    Vectorf3 vec3Norm = vec3.normalize();
    Vectorf3 vec4Norm = vec4.normalize();
    Vectorf4 vec5Norm = vec5.normalize();

    std::cout << "\n";
    printf("%7.3f %7.3f \n", vec1[0], vec1[1]);
    printf("%7.3f %7.3f \n", vec2[0], vec2[1]);
    printf("%7.3f %7.3f %7.3f \n", vec3[0], vec3[1], vec3[2] );
    printf("%7.3f %7.3f %7.3f \n", vec4[0], vec4[1], vec4[2]);
    printf("%7.3f %7.3f %7.3f %7.3f \n", vec5[0], vec5[1], vec5[2], vec5[3]);

    std::cout << "\n";
    printf("%7.3f %7.3f \n", vec1Norm[0], vec1Norm[1]);
    printf("%7.3f %7.3f \n", vec2Norm[0], vec2Norm[1]);
    printf("%7.3f %7.3f %7.3f \n", vec3Norm[0], vec3Norm[1], vec3Norm[2]);
    printf("%7.3f %7.3f %7.3f \n", vec4Norm[0], vec4Norm[1], vec4Norm[2]);
    printf("%7.3f %7.3f %7.3f %7.3f \n", vec5Norm[0], vec5Norm[1], vec5Norm[2], vec5Norm[3]);





}

void test6() {

    Vectorf3 vec1 = { 0.0f, -1.0f, 0.0f };
    Vectorf3 vec2 = { 0.0f, 0.0f, 1.0f };
    Vectorf3 vec12ans = vec1.cross(vec2);

    Vectorf3 vec3 = { -2.0f, 4.0f, 1.0f };
    Vectorf3 vec4 = { 1.0f, -2.0f, -1.0f };
    Vectorf3 vec34ans = vec3.cross(vec4);

    Vectorf3 vec5 = { 3.0f, 10.0f, 7.0f };
    Vectorf3 vec6 = { 8.0f, -7.0f, 4.0f };
    Vectorf3 vec56ans = vec5.cross(vec6);

    std::cout << "cross product\n";

    printf("%7.3f %7.3f %7.3f \n", vec12ans[0], vec12ans[1], vec12ans[2]);
    printf("%7.3f %7.3f %7.3f \n", vec34ans[0], vec34ans[1], vec34ans[2]);
    printf("%7.3f %7.3f %7.3f \n", vec56ans[0], vec56ans[1], vec56ans[2]);






}



#define PI 3.14159265
/*
void test7() {

    std::cout << "Conversion test" << std::endl;

    // Coordinate in world space.
    Vectorf4 posWorld{ 7.0f, -16.0f, -31.0f, 1.0f };
    std::cout << std::endl;
    std::cout << "Position in world space" << std::endl;
    printf("%7.3f %7.3f %7.3f \n", posWorld[0], posWorld[1], posWorld[2]);

    // Constants setup.
    float horizontalFov{ 60.0f };
    float rectRight = 1000;
    float rectLeft = 0;
    float rectBottom = 600;
    float rectTop = 0;

    float zoomX = 1.0f / tan(horizontalFov * (PI / 180.0f) / 2.0f);
    float zoomY = zoomX * (float(rectRight - rectLeft) / float(rectBottom - rectTop));

    float left = -100.0f;
    float right = +100.0f;
    float top = +100.0f;
    float bottom = -100.0f;
    float n = +20.0f;
    //    float f = -1000.0f;
    float f = +120.0f;

    float width = 800.0f;
    float height = 600.0f;

    // Triangle1 
    Vectorf3 v1{ 98.152f, -49.077f, -33.856f };
    Vectorf3 v2{ -49.076f, 98.155f, -33.856f };
    Vectorf3 v3{ -49.076f, -49.077f, -33.856f };

    // Triangle2
    Vectorf3 v4{ 95.252f, 0.57901f, -19.478f };
    Vectorf3 v5{ -51.975f, 147.81f, -19.478f };
    Vectorf3 v6{ -51.975f, 0.57901f, -19.478f };

    // Calculate perspective matrix.
    Matrixf4x4 persMat = frustum2(
        left,
        right,
        bottom,
        top,
        n,
        f
    );

    std::cout << std::endl;
    std::cout << "Perspective matrix." << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (j + 1 < 4) {
                printf("%7.3f ", persMat[i][j]);
            }
            else {
                printf("%7.3f \n", persMat[i][j]);
            }
        }
    }

    // Perspective transform.
    Vectorf4 posClip = posWorld * persMat;
    std::cout << std::endl;
    std::cout << "Position in homogeneous." << std::endl;
    printf("%7.3f %7.3f %7.3f %7.3f \n", posClip[0], posClip[1], posClip[2], posClip[3]);

    float ndcPosX = posClip[0] / posClip[3];
    float ndcPosY = posClip[1] / posClip[3];
    float ndcPosZ = posClip[2] / posClip[3];

    std::cout << std::endl;
    std::cout << "Position in clip space(note actually OpenGL is going to scale to fit [0,1] range." << std::endl;
    printf("%7.3f %7.3f %7.3f \n", ndcPosX, ndcPosY, ndcPosZ);

    // Convert NDC.(normalized device coordinate.) to screen coordinates.
    float screenPosX = (width / 2.0f) * ndcPosX + (width / 2.0f);
    float screenPosY = -(height / 2.0f) * ndcPosY + (-height / 2.0f) + height;

    std::cout << std::endl;
    std::cout << "Position in screen." << std::endl;
    printf("%7.3f %7.3f \n", screenPosX, screenPosY);

    // Convert screen coordinates to NDC.
    float ndcPosXrecon = (2.0f / width) * screenPosX - 1.0f;
    float ndcPosYrecon = -(2.0f / height) * screenPosY + 1.0f;

    std::cout << std::endl;
    std::cout << "Position in NDC." << std::endl;
    printf("%7.3f %7.3f \n", ndcPosXrecon, ndcPosYrecon );

    // Convert NDC to near plane coordinates.
    float npPosXrecon = (ndcPosXrecon * (right - left) + right + left) / 2.0f;
    float npPosYrecon = (ndcPosYrecon * (top - bottom) + top + bottom) / 2.0f;
    float npPosZrecon = -n;



    // Determine whether the eye direction intersect with the triangle and 
    // intersection point is in the triangle.

    Vectorf3 origin{ 0.0f, 0.0f, 0.0f };
    Vectorf3 direction{ npPosXrecon, npPosYrecon, npPosZrecon };
    Vectorf3 pointOfIntersection;

    bool ret = isRayIntersectWithPlane(v1, v2, v3, origin, direction, pointOfIntersection);

    std::cout << std::endl;
    std::cout << "Determine eye direction intersect with triangle." << std::endl;

    if (ret == true) {
        std::cout << "Eye direction intersect with the triangle." << std::endl;
    }
    else {
        std::cout << "Eye direction DO NOT intersect with the triangle." << std::endl;
    }

    printf("%7.3f %7.3f %7.3f \n", pointOfIntersection.getX(), pointOfIntersection.getY(), pointOfIntersection.getZ());


    ret = isRayIntersectWithPlane(v4, v5, v6, origin, direction, pointOfIntersection);

    std::cout << std::endl;
    std::cout << "Determine eye direction intersect with triangle." << std::endl;

    if (ret == true) {
        std::cout << "Eye direction intersect with the triangle." << std::endl;
    }
    else {
        std::cout << "Eye direction DO NOT intersect with the triangle." << std::endl;
    }

    printf("%7.3f %7.3f %7.3f \n", pointOfIntersection.getX(), pointOfIntersection.getY(), pointOfIntersection.getZ());








}
*/

void test7_2( Vectorf3 origin, Vectorf3 direction, Vectorf3 triangle[3] ) {

    std::cout << "Triangle hit test" << std::endl;

    Vectorf3 pointOfIntersection;

    Vectorf3 v1 = triangle[0];
    Vectorf3 v2 = triangle[1];
    Vectorf3 v3 = triangle[2];

    bool ret = isRayIntersectWithPlane(v1, v2, v3, origin, direction, pointOfIntersection);

    std::cout << std::endl;
    std::cout << "Determine eye direction intersect with triangle." << std::endl;

    if (ret == true) {
        std::cout << "Eye direction intersect with the triangle." << std::endl;
    }
    else {
        std::cout << "Eye direction DO NOT intersect with the triangle." << std::endl;
    }

    printf("%7.3f %7.3f %7.3f \n", pointOfIntersection.getX(), pointOfIntersection.getY(), pointOfIntersection.getZ());

}

void test7() {

    Vectorf3 origin{ -23.1379528, 300.000000, 171.331680};

    // Triangle3
    Vectorf3 v7{ -2.11093211, 8.31966591, 2.11100006 };
    Vectorf3 v8{ -2.11093211, 0.671500027, 2.11100006 };
    Vectorf3 v9{ 2.11093211, 8.31966591, 2.11100006 };

    Vectorf3 triangle3[] = { v7, v8, v9 };
    Vectorf3 posInWorld{ -23.1379528f, 300.000000f, 171.331680f };
    Vectorf3 direction = posInWorld - origin;
    direction.normalize();

    test7_2(posInWorld, direction, triangle3 );



}

void test8() {


    // Rotation -38 along y-axis.
    Matrixf4x4 modelRotMat = GetRotateMatrix4x4(0.0f, 19.98f, 0.0f);
    Matrixf4x4 modelTransMat = translate2(500.0f, 0.0f, -210.0f );

    Matrixf4x4 eyeRotMat = GetRotateMatrix4x4(0.0f, -15.12f, 0.0f);
    Matrixf4x4 eyeTransMat = translate2(-360.0f, 0.0f, 125.0f );
    Matrixf4x4 eyeRotMatRev = GetRotateMatrix4x4(-0.0f, +15.12f, -0.0f);
    Matrixf4x4 eyeTransMatRev = translate2(+360.0f, -0.0f, -125.0f );

    Vectorf4 Pm{ -160.0f, 0.0f, -155.0f, 1.0f };

//    Vectorf4 Pe = Pm * modelRotMat* modelTransMat* eyeTransMatRev* eyeRotMatRev;
    Vectorf4 Pw = Pm * modelRotMat * modelTransMat;
    Vectorf4 Pe = Pw * eyeTransMatRev * eyeRotMatRev;
    Vectorf4 Pe2 = Pm * modelRotMat * modelTransMat * eyeTransMatRev * eyeRotMatRev;

    printf("%7.3f %7.3f %7.3f \n", Pm[0], Pm[1], Pm[2]);
    printf("%7.3f %7.3f %7.3f \n", Pw[0], Pw[1], Pw[2]);
    printf("%7.3f %7.3f %7.3f \n", Pe[0], Pe[1], Pe[2]);
    printf("%7.3f %7.3f %7.3f \n", Pe2[0], Pe2[1], Pe2[2]);




    return;




}

Matrixf3x3 GetRotateMatrix3x3() {

    // Rotate about x-axis.
    Matrixf3x3 rotateAboutXMatriX = rotate3x3(1.0f, 0.0f, 0.0f, 0 * (PI / 180.0f));

    // Rotate about y-axis.
    Matrixf3x3 rotateAboutYMatriY = rotate3x3(0.0f, 1.0f, 0.0f, 30.0f * (PI / 180.0f));

    // Rotate about z-axis.
    Matrixf3x3 rotateAboutZMatriZ = rotate3x3(0.0f, 0.0f, 1.0f, 0 * (PI / 180.0f));

    // Combine all.
    Matrixf3x3 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

    return rotateMatrix;
}

void test9() {

    Matrixf3x3 rotMat = GetRotateMatrix3x3();

    Vectorf3 pos{ 0.0f, 0.0f, -10.0f };

    float xInW = pos * rotMat[0];
    float yInW = pos * rotMat[1];
    float zInW = pos * rotMat[2];

    printf("%7.3f %7.3f %7.3f \n", xInW, yInW, zInW );


    return;



}



void quaternion_test_slerp() {

    std::cout << "Start testing on quaternion slerp functions." << std::endl;

    Vectorf3 v1{ 45.0f, 30.0f, 0.0f };
    Vectorf3 v2{ 0.0f,  30.0f, 0.0f };

    for ( float i = 0.0f; i < 1.0f; i = i + 0.1f ) {

        Quaternionf q1 = { v1 };
        Quaternionf q2 = { v2 };

        q1.slerp(q2, i);

        Vectorf3 vResult = q1;
        printf("%7.3f %7.3f %7.3f \n", vResult.getX(), vResult.getY(), vResult.getZ() );


    }


}

void conversion_test_EulerAnglesAndMatrix() {

    std::cout << "Start testing on conversion functions between Euler angles and matrix." << std::endl;

    float EulerAngles[][3] = { 
        {   0.0f, 0.0f, 0.0f },
        {  10.0f, 0.0f, 0.0f },
        {  20.0f, 0.0f, 0.0f },
        {  30.0f, 0.0f, 0.0f },
        {  40.0f, 0.0f, 0.0f },
        {  50.0f, 0.0f, 0.0f },
        {  60.0f, 0.0f, 0.0f },
        {  70.0f, 0.0f, 0.0f },
        {  80.0f, 0.0f, 0.0f },
        {  90.0f, 0.0f, 0.0f },
        { 100.0f, 0.0f, 0.0f },
        { 110.0f, 0.0f, 0.0f },
        { 120.0f, 0.0f, 0.0f },
        { 130.0f, 0.0f, 0.0f },
        { 140.0f, 0.0f, 0.0f },
        { 150.0f, 0.0f, 0.0f },
        { 160.0f, 0.0f, 0.0f },
        { 170.0f, 0.0f, 0.0f },
        { 180.0f, 0.0f, 0.0f },

        {   0.0f,   0.0f, 0.0f },
        {   0.0f,  10.0f, 0.0f },
        {   0.0f,  20.0f, 0.0f },
        {   0.0f,  30.0f, 0.0f },
        {   0.0f,  40.0f, 0.0f },
        {   0.0f,  50.0f, 0.0f },
        {   0.0f,  60.0f, 0.0f },
        {   0.0f,  70.0f, 0.0f },
        {   0.0f,  80.0f, 0.0f },
        {   0.0f,  90.0f, 0.0f },
        {   0.0f, 100.0f, 0.0f },
        {   0.0f, 110.0f, 0.0f },
        {   0.0f, 120.0f, 0.0f },
        {   0.0f, 130.0f, 0.0f },
        {   0.0f, 140.0f, 0.0f },
        {   0.0f, 150.0f, 0.0f },
        {   0.0f, 160.0f, 0.0f },
        {   0.0f, 170.0f, 0.0f },
        {   0.0f, 180.0f, 0.0f },

        {   0.0f, 0.0f,   0.0f },
        {   0.0f, 0.0f,  10.0f },
        {   0.0f, 0.0f,  20.0f },
        {   0.0f, 0.0f,  30.0f },
        {   0.0f, 0.0f,  40.0f },
        {   0.0f, 0.0f,  50.0f },
        {   0.0f, 0.0f,  60.0f },
        {   0.0f, 0.0f,  70.0f },
        {   0.0f, 0.0f,  80.0f },
        {   0.0f, 0.0f,  90.0f },
        {   0.0f, 0.0f, 100.0f },
        {   0.0f, 0.0f, 110.0f },
        {   0.0f, 0.0f, 120.0f },
        {   0.0f, 0.0f, 130.0f },
        {   0.0f, 0.0f, 140.0f },
        {   0.0f, 0.0f, 150.0f },
        {   0.0f, 0.0f, 160.0f },
        {   0.0f, 0.0f, 170.0f },
        {   0.0f, 0.0f, 180.0f },

        {   0.0f, 150.0f, 0.0f },
        {  10.0f,  20.0f, 0.0f },
        {  20.0f,  30.0f, 0.0f },
        {  30.0f,  50.0f, 0.0f },
        {  40.0f, 170.0f, 0.0f },
        {  50.0f,   0.0f, 0.0f },
        {  60.0f, 120.0f, 0.0f },
        {  70.0f, 130.0f, 0.0f },
        {  80.0f,  10.0f, 0.0f },
        {  90.0f, 110.0f, 0.0f },
        { 100.0f, 140.0f, 0.0f },
        { 110.0f,  60.0f, 0.0f },
        { 120.0f,  70.0f, 0.0f },
        { 130.0f, 180.0f, 0.0f },
        { 140.0f, 100.0f, 0.0f },
        { 150.0f,  80.0f, 0.0f },
        { 160.0f,  90.0f, 0.0f },
        { 170.0f, 160.0f, 0.0f },
        { 180.0f,  40.0f, 0.0f },

        { 150.0f,   0.0f, 0.0f },
        {  20.0f,  10.0f, 0.0f },
        {  30.0f,  20.0f, 0.0f },
        {  50.0f,  30.0f, 0.0f },
        { 170.0f,  40.0f, 0.0f },
        {   0.0f,  50.0f, 0.0f },
        { 120.0f,  60.0f, 0.0f },
        { 130.0f,  70.0f, 0.0f },
        {  10.0f,  80.0f, 0.0f },
        { 110.0f,  90.0f, 0.0f },
        { 140.0f, 100.0f, 0.0f },
        {  60.0f, 110.0f, 0.0f },
        {  70.0f, 120.0f, 0.0f },
        { 180.0f, 130.0f, 0.0f },
        { 100.0f, 140.0f, 0.0f },
        {  80.0f, 150.0f, 0.0f },
        {  90.0f, 160.0f, 0.0f },
        { 160.0f, 170.0f, 0.0f },
        {  40.0f, 180.0f, 0.0f },

        { 150.0f, 0.0f,   0.0f },
        { 20.0f, 0.0f,  10.0f },
        { 30.0f, 0.0f,  20.0f },
        { 50.0f, 0.0f,  30.0f },
        { 170.0f, 0.0f,  40.0f },
        { 0.0f, 0.0f,  50.0f },
        { 120.0f, 0.0f,  60.0f },
        { 130.0f, 0.0f,  70.0f },
        { 10.0f, 0.0f,  80.0f },
        { 110.0f, 0.0f,  90.0f },
        { 140.0f, 0.0f, 100.0f },
        { 60.0f, 0.0f, 110.0f },
        { 70.0f, 0.0f, 120.0f },
        { 180.0f, 0.0f, 130.0f },
        { 100.0f, 0.0f, 140.0f },
        { 80.0f, 0.0f, 150.0f },
        { 90.0f, 0.0f, 160.0f },
        { 160.0f, 0.0f, 170.0f },
        { 40.0f, 0.0f, 180.0f },

        { 0.0f, 150.0f,   0.0f },
        { 0.0f,  20.0f,  10.0f },
        { 0.0f,  30.0f,  20.0f },
        { 0.0f,  50.0f,  30.0f },
        { 0.0f, 170.0f,  40.0f },
        { 0.0f,   0.0f,  50.0f },
        { 0.0f, 120.0f,  60.0f },
        { 0.0f, 130.0f,  70.0f },
        { 0.0f,  10.0f,  80.0f },
        { 0.0f, 110.0f,  90.0f },
        { 0.0f, 140.0f, 100.0f },
        { 0.0f,  60.0f, 110.0f },
        { 0.0f,  70.0f, 120.0f },
        { 0.0f, 180.0f, 130.0f },
        { 0.0f, 100.0f, 140.0f },
        { 0.0f,  80.0f, 150.0f },
        { 0.0f,  90.0f, 160.0f },
        { 0.0f, 160.0f, 170.0f },
        { 0.0f,  40.0f, 180.0f },

        { 140.0f, 150.0f,   0.0f },
        { 60.0f,  20.0f,  10.0f },
        { 70.0f,  30.0f,  20.0f },
        { 180.0f,  50.0f,  30.0f },
        { 120.0f, 170.0f,  40.0f },
        { 130.0f,   0.0f,  50.0f },
        { 10.0f, 120.0f,  60.0f },
        { 110.0f, 130.0f,  70.0f },
        { 100.0f,  10.0f,  80.0f },
        { 80.0f, 110.0f,  90.0f },
        { 90.0f, 140.0f, 100.0f },
        { 160.0f,  60.0f, 110.0f },
        { 40.0f,  70.0f, 120.0f },
        { 150.0f, 180.0f, 130.0f },
        { 20.0f, 100.0f, 140.0f },
        { 30.0f,  80.0f, 150.0f },
        { 50.0f,  90.0f, 160.0f },
        { 170.0f, 160.0f, 170.0f },
        { 0.0f,  40.0f, 180.0f },
    };

    Validator validator;
    validator.set(MINUS_ZERO_AND_PLUS_ZERO_SAME);
    validator.set(ROUND_TO_3DECIMAL_PLACES);

    for (int i = 0; i < sizeof(EulerAngles) / sizeof(float[3]); i++) {

        float heading = EulerAngles[i][0];
        float pitch = EulerAngles[i][1];
        float bank = EulerAngles[i][2];

        Matrixf3x3 m{ heading, pitch, bank };

        float heading_ret, pitch_ret, bank_ret;
        m.GetEulerAngles(heading_ret, pitch_ret, bank_ret);

//        printf("%d\n", i);
//        printf("%7.3f %7.3f %7.3f \n", heading, pitch, bank);
//        printf("%7.3f %7.3f %7.3f \n", heading_ret, pitch_ret, bank_ret );

        vector<float> before{ heading, pitch, bank };
        vector<float> after{ heading_ret, pitch_ret, bank_ret };
        validator.add(i, "EulerAngles and Matrix->EulerAngles", before, after);

    }

    validator.validate();


}

void conversion_test_QuaternionAndMatrix() {

    std::cout << "Start testing on conversion functions between quaternion and matrix." << std::endl;

    typedef struct strQuaternionTest {

        float theta;
        Vectorf3 normal;

    }QuaternionTest;

    Vectorf3 normalxy{ 1.0f, 1.0f, 0.0f };
    normalxy.normalize();

    Vectorf3 normalyz{ 0.0f, 1.0f, 1.0f };
    normalyz.normalize();

    Vectorf3 normalxz{ 1.0f, 0.0f, 1.0f };
    normalxz.normalize();


    QuaternionTest tests[] = {
        { 10.0f * (PI/180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 20.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 30.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 40.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 50.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 60.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 70.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 80.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 90.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 100.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 110.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 120.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 130.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 140.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 150.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 160.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 170.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},
        { 180.0f * (PI / 180.0f), Vectorf3 { 1.0f, 0.0f, 0.0f }},

        { 10.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 20.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 30.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 40.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 50.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 60.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 70.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 80.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 90.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 100.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 110.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 120.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 130.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 140.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 150.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 160.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 170.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},
        { 180.0f * (PI / 180.0f), Vectorf3 { 0.0f, 1.0f, 0.0f }},

        { 10.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 20.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 30.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 40.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 50.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 60.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 70.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 80.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 90.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 100.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 110.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 120.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 130.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 140.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 150.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 160.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 170.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},
        { 180.0f * (PI / 180.0f), Vectorf3 { 0.0f, 0.0f, 1.0f }},

        { 10.0f * (PI / 180.0f), normalxy},
        { 20.0f * (PI / 180.0f), normalxy},
        { 30.0f * (PI / 180.0f), normalxy},
        { 40.0f * (PI / 180.0f), normalxy},
        { 50.0f * (PI / 180.0f), normalxy},
        { 60.0f * (PI / 180.0f), normalxy},
        { 70.0f * (PI / 180.0f), normalxy},
        { 80.0f * (PI / 180.0f), normalxy},
        { 90.0f * (PI / 180.0f), normalxy},
        { 100.0f * (PI / 180.0f), normalxy},
        { 110.0f * (PI / 180.0f), normalxy},
        { 120.0f * (PI / 180.0f), normalxy},
        { 130.0f * (PI / 180.0f), normalxy},
        { 140.0f * (PI / 180.0f), normalxy},
        { 150.0f * (PI / 180.0f), normalxy},
        { 160.0f * (PI / 180.0f), normalxy},
        { 170.0f * (PI / 180.0f), normalxy},
        { 180.0f * (PI / 180.0f), normalxy},

        { 10.0f * (PI / 180.0f), normalyz},
        { 20.0f * (PI / 180.0f), normalyz},
        { 30.0f * (PI / 180.0f), normalyz},
        { 40.0f * (PI / 180.0f), normalyz},
        { 50.0f * (PI / 180.0f), normalyz},
        { 60.0f * (PI / 180.0f), normalyz},
        { 70.0f * (PI / 180.0f), normalyz},
        { 80.0f * (PI / 180.0f), normalyz},
        { 90.0f * (PI / 180.0f), normalyz},
        { 100.0f * (PI / 180.0f), normalyz},
        { 110.0f * (PI / 180.0f), normalyz},
        { 120.0f * (PI / 180.0f), normalyz},
        { 130.0f * (PI / 180.0f), normalyz},
        { 140.0f * (PI / 180.0f), normalyz},
        { 150.0f * (PI / 180.0f), normalyz},
        { 160.0f * (PI / 180.0f), normalyz},
        { 170.0f * (PI / 180.0f), normalyz},
        { 180.0f * (PI / 180.0f), normalyz},

        { 10.0f * (PI / 180.0f), normalxz},
        { 20.0f * (PI / 180.0f), normalxz},
        { 30.0f * (PI / 180.0f), normalxz},
        { 40.0f * (PI / 180.0f), normalxz},
        { 50.0f * (PI / 180.0f), normalxz},
        { 60.0f * (PI / 180.0f), normalxz},
        { 70.0f * (PI / 180.0f), normalxz},
        { 80.0f * (PI / 180.0f), normalxz},
        { 90.0f * (PI / 180.0f), normalxz},
        { 100.0f * (PI / 180.0f), normalxz},
        { 110.0f * (PI / 180.0f), normalxz},
        { 120.0f * (PI / 180.0f), normalxz},
        { 130.0f * (PI / 180.0f), normalxz},
        { 140.0f * (PI / 180.0f), normalxz},
        { 150.0f * (PI / 180.0f), normalxz},
        { 160.0f * (PI / 180.0f), normalxz},
        { 170.0f * (PI / 180.0f), normalxz},
        { 180.0f * (PI / 180.0f), normalxz},
    };

    Validator validator;

    for (int testNo = 0; testNo < sizeof(tests) / sizeof(tests[0]); testNo++) {

        float theta = tests[testNo].theta;
        Vectorf3 normal = tests[testNo].normal;

//        printf("# %d \n", i);
//        printf("theta %7.3f, normal.x %7.3f normal.y %7.3f normal.z %7.3f \n", theta, normal.getX(), normal.getY(), normal.getZ());

        Quaternionf q{ cos(theta / 2), sin(theta / 2) * normal };
//        printf("->Quaternion q.w %7.3f, q.x %7.3f q.y %7.3f q.z %7.3f \n", q.w, q.x, q.y, q.z);

        Matrixf3x3 rotMat = q;
//        printf("->Quaternion(q) to Matrix(rotMat) \n");
//        printf("m11 m12 m13 %7.3f %7.3f %7.3f \n", rotMat[0][0], rotMat[0][1], rotMat[0][2]);
//        printf("m21 m22 m23 %7.3f %7.3f %7.3f \n", rotMat[1][0], rotMat[1][1], rotMat[1][2]);
//        printf("m31 m32 m33 %7.3f %7.3f %7.3f \n", rotMat[2][0], rotMat[2][1], rotMat[2][2]);

        float x, y, z;
        rotMat.GetEulerAngles(x, y, z);
//        printf("->Matrix(rotMat) to EulerAngles(%7.3f, %7.3f, %7.3f) \n", x, y, z );

        Matrixf3x3 rotMat_ans = rotate3x3( normal[0], normal[1], normal[2], theta );
//        printf("->rotate3x3( %7.3f, %7.3f, %7.3f, %7.3f )->Matrix(rotMat_ans) \n", normal[0], normal[1], normal[2], theta );

//        printf("->Matrix(rotMat_ans) \n");
//        printf("m11 m12 m13 %7.3f %7.3f %7.3f \n", rotMat_ans[0][0], rotMat_ans[0][1], rotMat_ans[0][2]);
//        printf("m21 m22 m23 %7.3f %7.3f %7.3f \n", rotMat_ans[1][0], rotMat_ans[1][1], rotMat_ans[1][2]);
//        printf("m31 m32 m33 %7.3f %7.3f %7.3f \n", rotMat_ans[2][0], rotMat_ans[2][1], rotMat_ans[2][2]);

        float x2, y2, z2;
        rotMat_ans.GetEulerAngles(x2, y2, z2);
//        printf("->Matrix(rotMat_ans) to EulerAngles(%7.3f, %7.3f, %7.3f) \n", x2, y2, z2);

        Quaternionf q_ret = rotMat_ans;
//        printf("->Matrix(rotMat_ans) to Quaternion(q_ret) %7.3f, q.x %7.3f q.y %7.3f q.z %7.3f \n", q_ret.w, q_ret.x, q_ret.y, q_ret.z);

        Matrixf3x3 rotMat2 = q_ret;
//        printf("->Quaternion(q_ret) to Matrix(rotMat2) \n");
//        printf("m11 m12 m13 %7.3f %7.3f %7.3f \n", rotMat2[0][0], rotMat2[0][1], rotMat2[0][2]);
//        printf("m21 m22 m23 %7.3f %7.3f %7.3f \n", rotMat2[1][0], rotMat2[1][1], rotMat2[1][2]);
//        printf("m31 m32 m33 %7.3f %7.3f %7.3f \n", rotMat2[2][0], rotMat2[2][1], rotMat2[2][2]);

        Quaternionf q_ret2 = rotMat2;
//        printf("->Matrix(rotMat2) to Quaternion(q_ret2) q_ret2.w %7.3f, q_ret2.x %7.3f q_ret2.y %7.3f q_ret2.z %7.3f \n", q_ret2.w, q_ret2.x, q_ret2.y, q_ret2.z);

        float acceptableErrorPercentage = 0.01f;

//        Assert::AssertEqual(q.w, q_ret.w, acceptableErrorPercentage );
//        Assert::AssertEqual(q_ret.w, q_ret2.w, acceptableErrorPercentage);

//        Assert::AssertEqual(q.x, q_ret.x, acceptableErrorPercentage);
//        Assert::AssertEqual(q_ret.x, q_ret2.x, acceptableErrorPercentage);

//        Assert::AssertEqual(q.y, q_ret.y, acceptableErrorPercentage);
//        Assert::AssertEqual(q_ret.y, q_ret2.y, acceptableErrorPercentage);

//        Assert::AssertEqual(q.z, q_ret.z, acceptableErrorPercentage);
//        Assert::AssertEqual(q_ret.z, q_ret2.z, acceptableErrorPercentage);

        vector<float> quaternion    { q.w    , q.x    , q.y    , q.z };
        vector<float> quaternion_ret{ q_ret.w, q_ret.x, q_ret.y, q_ret.z };
        validator.add(testNo, "Quaternion and rotate()->Matrix->Quaternion", quaternion, quaternion_ret);

        float x3, y3, z3;
        rotMat2.GetEulerAngles(x3, y3, z3);

        vector<float> matrix;
        vector<float> matrix_ret;

        vector<float> matrix2;
        vector<float> matrix2_ret;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

//                Assert::AssertEqual(rotMat[i][j], rotMat_ans[i][j], acceptableErrorPercentage);
//                Assert::AssertEqual(rotMat_ans[i][j], rotMat2[i][j], acceptableErrorPercentage);
                matrix.push_back(rotMat[i][j]);
                matrix_ret.push_back(rotMat_ans[i][j]);

                matrix2.push_back(rotMat_ans[i][j]);
                matrix2_ret.push_back(rotMat2[i][j]);
            }
        }

        validator.add(testNo, "Quaternion->Matrix and rotate()->Matrix", matrix, matrix_ret);
        validator.add(testNo, "rotate()->Matrix and rotate()->Matrix->Quaternion->Matrix", matrix2, matrix2_ret);

        vector<float> eulerAngles2;
        vector<float> eulerAngles2_ret;

        eulerAngles2.push_back(x2);
        eulerAngles2.push_back(y2);
        eulerAngles2.push_back(z2);

        eulerAngles2_ret.push_back(x3);
        eulerAngles2_ret.push_back(y3);
        eulerAngles2_ret.push_back(z3);

        validator.add(testNo, "rotate()->Matrix->EulerAngles and rotate()->Matrix->Quaternion->Matrix->EulerAngles", eulerAngles2, eulerAngles2_ret);
 
//        Assert::AssertEqual(x, x2, acceptableErrorPercentage);
//        Assert::AssertEqual(y, y2, acceptableErrorPercentage);
//        Assert::AssertEqual(z, z2, acceptableErrorPercentage);

        vector<float> eulerAngles;
        vector<float> eulerAngles_ret;

        eulerAngles.push_back(x);
        eulerAngles.push_back(y);
        eulerAngles.push_back(z);

        eulerAngles_ret.push_back(x2);
        eulerAngles_ret.push_back(y2);
        eulerAngles_ret.push_back(z2);

        validator.add(testNo, "Quaternion->Matrix->EulerAngles and rotate()->Matrix->EulerAngles", eulerAngles, eulerAngles_ret);


    }

    validator.set(MINUS_ZERO_AND_PLUS_ZERO_SAME);
    validator.set(ROUND_TO_3DECIMAL_PLACES);
    validator.validate();

    return;
}

void conversion_test_EulerAnglesAndQuaternion() {

    std::cout << "Start testing on conversion functions between Euler angles and quaternion." << std::endl;

    float EulerAngles[][3] = {
        {   0.0f, 0.0f, 0.0f },
        {  10.0f, 0.0f, 0.0f },
        {  20.0f, 0.0f, 0.0f },
        {  30.0f, 0.0f, 0.0f },
        {  40.0f, 0.0f, 0.0f },
        {  50.0f, 0.0f, 0.0f },
        {  60.0f, 0.0f, 0.0f },
        {  70.0f, 0.0f, 0.0f },
        {  80.0f, 0.0f, 0.0f },
        {  90.0f, 0.0f, 0.0f },
        { 100.0f, 0.0f, 0.0f },
        { 110.0f, 0.0f, 0.0f },
        { 120.0f, 0.0f, 0.0f },
        { 130.0f, 0.0f, 0.0f },
        { 140.0f, 0.0f, 0.0f },
        { 150.0f, 0.0f, 0.0f },
        { 160.0f, 0.0f, 0.0f },
        { 170.0f, 0.0f, 0.0f },
        { 180.0f, 0.0f, 0.0f },

        {   0.0f,   0.0f, 0.0f },
        {   0.0f,  10.0f, 0.0f },
        {   0.0f,  20.0f, 0.0f },
        {   0.0f,  30.0f, 0.0f },
        {   0.0f,  40.0f, 0.0f },
        {   0.0f,  50.0f, 0.0f },
        {   0.0f,  60.0f, 0.0f },
        {   0.0f,  70.0f, 0.0f },
        {   0.0f,  80.0f, 0.0f },
        {   0.0f,  90.0f, 0.0f },
        {   0.0f, 100.0f, 0.0f },
        {   0.0f, 110.0f, 0.0f },
        {   0.0f, 120.0f, 0.0f },
        {   0.0f, 130.0f, 0.0f },
        {   0.0f, 140.0f, 0.0f },
        {   0.0f, 150.0f, 0.0f },
        {   0.0f, 160.0f, 0.0f },
        {   0.0f, 170.0f, 0.0f },
        {   0.0f, 180.0f, 0.0f },

        {   0.0f, 0.0f,   0.0f },
        {   0.0f, 0.0f,  10.0f },
        {   0.0f, 0.0f,  20.0f },
        {   0.0f, 0.0f,  30.0f },
        {   0.0f, 0.0f,  40.0f },
        {   0.0f, 0.0f,  50.0f },
        {   0.0f, 0.0f,  60.0f },
        {   0.0f, 0.0f,  70.0f },
        {   0.0f, 0.0f,  80.0f },
        {   0.0f, 0.0f,  90.0f },
        {   0.0f, 0.0f, 100.0f },
        {   0.0f, 0.0f, 110.0f },
        {   0.0f, 0.0f, 120.0f },
        {   0.0f, 0.0f, 130.0f },
        {   0.0f, 0.0f, 140.0f },
        {   0.0f, 0.0f, 150.0f },
        {   0.0f, 0.0f, 160.0f },
        {   0.0f, 0.0f, 170.0f },
        {   0.0f, 0.0f, 180.0f },

        {   0.0f, 150.0f, 0.0f },
        {  10.0f,  20.0f, 0.0f },
        {  20.0f,  30.0f, 0.0f },
        {  30.0f,  50.0f, 0.0f },
        {  40.0f, 170.0f, 0.0f },
        {  50.0f,   0.0f, 0.0f },
        {  60.0f, 120.0f, 0.0f },
        {  70.0f, 130.0f, 0.0f },
        {  80.0f,  10.0f, 0.0f },
        {  90.0f, 110.0f, 0.0f },
        { 100.0f, 140.0f, 0.0f },
        { 110.0f,  60.0f, 0.0f },
        { 120.0f,  70.0f, 0.0f },
        { 130.0f, 180.0f, 0.0f },
        { 140.0f, 100.0f, 0.0f },
        { 150.0f,  80.0f, 0.0f },
        { 160.0f,  90.0f, 0.0f },
        { 170.0f, 160.0f, 0.0f },
        { 180.0f,  40.0f, 0.0f },

        { 150.0f,   0.0f, 0.0f },
        {  20.0f,  10.0f, 0.0f },
        {  30.0f,  20.0f, 0.0f },
        {  50.0f,  30.0f, 0.0f },
        { 170.0f,  40.0f, 0.0f },
        {   0.0f,  50.0f, 0.0f },
        { 120.0f,  60.0f, 0.0f },
        { 130.0f,  70.0f, 0.0f },
        {  10.0f,  80.0f, 0.0f },
        { 110.0f,  90.0f, 0.0f },
        { 140.0f, 100.0f, 0.0f },
        {  60.0f, 110.0f, 0.0f },
        {  70.0f, 120.0f, 0.0f },
        { 180.0f, 130.0f, 0.0f },
        { 100.0f, 140.0f, 0.0f },
        {  80.0f, 150.0f, 0.0f },
        {  90.0f, 160.0f, 0.0f },
        { 160.0f, 170.0f, 0.0f },
        {  40.0f, 180.0f, 0.0f },

        { 150.0f, 0.0f,   0.0f },
        { 20.0f, 0.0f,  10.0f },
        { 30.0f, 0.0f,  20.0f },
        { 50.0f, 0.0f,  30.0f },
        { 170.0f, 0.0f,  40.0f },
        { 0.0f, 0.0f,  50.0f },
        { 120.0f, 0.0f,  60.0f },
        { 130.0f, 0.0f,  70.0f },
        { 10.0f, 0.0f,  80.0f },
        { 110.0f, 0.0f,  90.0f },
        { 140.0f, 0.0f, 100.0f },
        { 60.0f, 0.0f, 110.0f },
        { 70.0f, 0.0f, 120.0f },
        { 180.0f, 0.0f, 130.0f },
        { 100.0f, 0.0f, 140.0f },
        { 80.0f, 0.0f, 150.0f },
        { 90.0f, 0.0f, 160.0f },
        { 160.0f, 0.0f, 170.0f },
        { 40.0f, 0.0f, 180.0f },

        { 0.0f, 150.0f,   0.0f },
        { 0.0f,  20.0f,  10.0f },
        { 0.0f,  30.0f,  20.0f },
        { 0.0f,  50.0f,  30.0f },
        { 0.0f, 170.0f,  40.0f },
        { 0.0f,   0.0f,  50.0f },
        { 0.0f, 120.0f,  60.0f },
        { 0.0f, 130.0f,  70.0f },
        { 0.0f,  10.0f,  80.0f },
        { 0.0f, 110.0f,  90.0f },
        { 0.0f, 140.0f, 100.0f },
        { 0.0f,  60.0f, 110.0f },
        { 0.0f,  70.0f, 120.0f },
        { 0.0f, 180.0f, 130.0f },
        { 0.0f, 100.0f, 140.0f },
        { 0.0f,  80.0f, 150.0f },
        { 0.0f,  90.0f, 160.0f },
        { 0.0f, 160.0f, 170.0f },
        { 0.0f,  40.0f, 180.0f },

        { 140.0f, 150.0f,   0.0f },
        { 60.0f,  20.0f,  10.0f },
        { 70.0f,  30.0f,  20.0f },
        { 180.0f,  50.0f,  30.0f },
        { 120.0f, 170.0f,  40.0f },
        { 130.0f,   0.0f,  50.0f },
        { 10.0f, 120.0f,  60.0f },
        { 110.0f, 130.0f,  70.0f },
        { 100.0f,  10.0f,  80.0f },
        { 80.0f, 110.0f,  90.0f },
        { 90.0f, 140.0f, 100.0f },
        { 160.0f,  60.0f, 110.0f },
        { 40.0f,  70.0f, 120.0f },
        { 150.0f, 180.0f, 130.0f },
        { 20.0f, 100.0f, 140.0f },
        { 30.0f,  80.0f, 150.0f },
        { 50.0f,  90.0f, 160.0f },
        { 170.0f, 160.0f, 170.0f },
        { 0.0f,  40.0f, 180.0f },
    };

    Validator validator;

    for (int i = 0; i < sizeof(EulerAngles) / sizeof(float[3]); i++) {

        float heading = EulerAngles[i][0];
        float pitch = EulerAngles[i][1];
        float bank = EulerAngles[i][2];

        Quaternion<float> q{ heading, pitch, bank };

        float heading_ret;
        float pitch_ret;
        float bank_ret;

        q.GetEulerAngles(heading_ret, pitch_ret, bank_ret);

        vector<float> values{ heading, pitch, bank };
        vector<float> ans_values{ heading_ret, pitch_ret, bank_ret };

        validator.add( i, "Quaternion and EulerAngles", values, ans_values);

    }

    validator.set(MINUS_ZERO_AND_PLUS_ZERO_SAME);
    validator.set(ROUND_TO_INTEGER);
    validator.validate();




}

int main()
{
//    test1();
 //   test2();
//    test3();
//    test4();
//    test4_2();
//    test5();
//    test6();
    test7();
//    test8();
//    test9();

    quaternion_test_slerp();
    conversion_test_EulerAnglesAndMatrix();
    conversion_test_QuaternionAndMatrix();

//    conversion_test_EulerAnglesAndQuaternion();

    std::cout << "Hello World!\n";
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
