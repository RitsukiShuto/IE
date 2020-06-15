/*
CT_fft.c

*x : real part of result.
*y : imaginary part of result.
n  : length (2^m)
l  : switch (if l = 1 then fft, if l = -1 then rft)

*/

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

int CT_fft(double *x, double *y, int n, int fr)
{
	/*
	使い方：
	入力と出力はx,yでポインタを示した配列で行う。
	出力結果は入力の配列に上書きされる
	xが実数部分,yは虚数部分
	nはFFTの長さ(フレーム長)
	frは「1」の時FFT、「-1」の時、逆FFTとして動作する。1か-1を
	使うこと。それ以外では計算結果がおかしくなる場合がある。

	関数の返り値は必ず「1」
	*/

	
	
	int i, i0, i1, j, l1, ns, ns2, n1, k;
    double s, c, sv1, cv1, arg, x1, y1, t;

/*    double cos( ), sin( ); */
/* initialize parameters */
    n1 = n / 2; 
    arg = PI;

/* bit reversi counter */
    j = 0;
    for(i = 0; i < n - 1; i++){
	if(i <= j){
	    t = *(x + i);  *(x + i) = *(x + j);  *(x + j) = t;
	    t = *(y + i);  *(y + i) = *(y + j);  *(y + j) = t;
	}
	k = n1;
	while(k <= j){
	    j = j - k;  k /= 2;
	}
	j = j + k;
    }

/* Fast Fourier Transforms */
    ns = 1;
    while(ns <= n1){
	cv1 = cos(arg);  sv1 = sin(- fr * arg);
	c = 1.0;  s = 0.0; ns2 = ns * 2;
	for(l1 = 0; l1 < ns; l1++){
	    for(i0 = l1; i0 < n; i0 += ns2){
		i1 = i0 + ns;
		x1 = *(x + i1) * c - *(y + i1) * s;
		y1 = *(y + i1) * c + *(x + i1) * s;
		*(x + i1) = *(x + i0) - x1;
		*(y + i1) = *(y + i0) - y1; 
		*(x + i0) = *(x + i0) + x1;
		*(y + i0) = *(y + i0) + y1;
	    }
	    t = cv1 * c - sv1 * s; s = sv1 * c + cv1 * s; c = t;
	}
	ns = ns2;
	arg /= 2.0;
    }

/* fft or rft */
    if(fr >= 0)
    for(i = 0; i < n; i++){
	*(x + i) /= n; *(y + i) /= n;
    }
    return 1;
}
