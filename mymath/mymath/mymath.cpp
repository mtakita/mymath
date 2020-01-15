// ConsoleApplication2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "mymath.h"

using namespace mymath;

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

int main()
{
    test1();
    test2();
    test3();
    test4();
    test4_2();
    test5();
    test6();

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
