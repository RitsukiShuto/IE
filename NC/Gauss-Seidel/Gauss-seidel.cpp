// IE CN03 2020/07/13-15:14:14.
// Gauss-seidel.cpp
//
#include<cstdio>
#include<cmath>
#define MAX_ITER 100

int main(){
    double a[3][3] = {{4.0, 1.0, 1.0},
                      {1.0, 3.0, 1.0},
                      {2.0, 1.0, 5.0}};     // 左辺の係数部
    double b[3] = {9.0, 10.0, 19.0};        // 右辺

    double x[3] = {0.0, 0.0, 0.0};          // 今回の解
    double xp[3] = {0.0, 0.0, 0.0};         // 前回の解

    double err = 1.0e-4;    // 誤差の判定値

    int n = 0;          // 反復用ループカウンタ
    int is_solved = 0;  // (収束: 1 / 非収束: 0)

    /* 反復開始 */
    while(n < MAX_ITER && ! is_solved){
        /* 反復式: 古いx, yを使って計算し、新しいx, yを代入 */
        x[0] = (b[0] - a[0][1] * x[1] - a[0][2] * x[2]) / a[0][0];
        x[1] = (b[1] - a[1][0] * x[0] - a[1][2] * x[2]) / a[1][1];
        x[2] = (b[2] - a[2][0] * x[0] - a[2][1] * x[1]) / a[2][2];
        fprintf(stderr, "(x, y, z) = (%.4f, %.4f, %.4f)\n", x[0], x[1], x[2]);
        printf("x[0] = %lf - %lf * %lf - %lf * %lf / %lf\n", b[0], a[0][1], x[1], a[0][2], x[2], a[0][0]);
        printf("x[1] = %lf - %lf * %lf - %lf * %lf / %lf\n", b[1], a[1][0], x[0], a[1][2], x[2], a[1][1]);
        printf("x[2] = %lf - %lf * %lf - %lf * %lf / %lf\n", b[2], a[2][0], x[0], a[2][1], x[1], a[2][2]);


        /* 収束したか？ */
        if(fabs(x[0] -xp[0]) + fabs(x[1] - xp[1]) + fabs(x[2] - xp[2]) < err){
            is_solved = 1;
        }else{
            for(int i = 0;i < 3;i++){
                xp[i] = x[i];
            }
            n++;    // 反復数をカウント
        }
    }

    /* 収束したかMAX_ITERに達したか判定 */
    if(is_solved == 1){
        fprintf(stderr, "Complete, (x, y, z) = (%.4f, %.4f, %.4f)\nITER : %d\n", x[0], x[1], x[2], n);
    }else{
        fprintf(stderr, "Not solved...\n");
    }

    return 0;
}
