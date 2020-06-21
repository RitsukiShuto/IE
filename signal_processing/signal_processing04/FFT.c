/*
2020-4-1.c a.k.a. FFT.c
Edited by RitsukiShuto on 2020/06/18-22:18:51.

Coding 'UTF-8'

Wav�t�@�C����ǂݍ���ŁAFFT����B
���ʂ����̂܂ܕ��f���Ƃ��āA�R���\�[���ɏo�͂���B


�R���p�C�����@��
cl 2020-4-1.c readWavHead.c CT_fft.c

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

#define FRAMESIZE 256 //�t���[�����A2�ׂ̂���ł��邱��

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char* argv[]) 
{
	char inFile[256]; //�ǂݍ��݃t�@�C�������i�[����ϐ�
	FILE* ifp;
	short dataIn[FRAMESIZE];//wav�t�@�C������ǂݍ��ރf�[�^������z��
	double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];//FFT�̌v�Z�Ɏg���z���dobule�Ő錾

	/*�e��ϐ�*/
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	if (argc < 2) {
		printf("�ǂݍ��ރt�@�C�������w�肵�Ă�������");
		exit(0);
	}
	/*�R�}���h���C������t�@�C�����̎擾*/
	strcpy(inFile, argv[1]);
	/*�t�@�C�����̊m�F*/
	printf("input file is %s.\n", inFile);

	ifp = fopen(inFile, "rb");//�t�@�C�����J���|�C���^��ifp�ɓ����

	/*Wav�̃w�b�_��ǂ�*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}
	/*�ǂݍ��񂾃p�����[�^���R���\�[���ɏo��*/
	printf("�t�@�C���̒����� %d �T���v��\n", len);
	printf("�T���v�����O���g���� %d Hz\n", fs);
	printf("�`���l������ %d\n", chNum);
	printf("1�T���v���̃r�b�g���� %d �r�b�g\n", sampSize);

	fNum = (int)floor(len / FRAMESIZE); //���s�����(�t���[����)�̌v�Z

	for (int i = 0; i < fNum; i++) {
		fread(dataIn, sizeof(short), FRAMESIZE, ifp); //ifp����f�[�^���t���[�������ǂݍ���

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataInX[j] = (double)dataIn[j];  //CT_fft��double�̕ϐ����g���̂ŁAdouble�̕ϐ��ɑ���B
			dDataInY[j] = 0.0; //��������0.0������B

		}
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT�̎��s
		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%lf\n", sqrt(pow(dDataInX[j], 2) + pow(dDataInY[j], 2))); //FFT���ʂ̕\���BdData��double�Ȃ�%lf�ŕ\������B
		}
		printf("\n");
	}

	fclose(ifp);

}