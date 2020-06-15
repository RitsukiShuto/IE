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
	�g�����F
	���͂Əo�͂�x,y�Ń|�C���^���������z��ōs���B
	�o�͌��ʂ͓��͂̔z��ɏ㏑�������
	x����������,y�͋�������
	n��FFT�̒���(�t���[����)
	fr�́u1�v�̎�FFT�A�u-1�v�̎��A�tFFT�Ƃ��ē��삷��B1��-1��
	�g�����ƁB����ȊO�ł͌v�Z���ʂ����������Ȃ�ꍇ������B

	�֐��̕Ԃ�l�͕K���u1�v
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
