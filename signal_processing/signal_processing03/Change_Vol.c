/*
Edited by RitsukiShuto on 2020/06/08.
2020-3-4.c a.k.a. Change_Vol.c

Coding "Shift JIS"

wav�t�@�C����ǂ݁A�֐��Ńf�[�^�𑀍삵�Awav�t�@�C���ɏo�͂���
�������Ȃ̂ŁA���������Ċ���������Ɨǂ�

�R���p�C�����@��
cl 2020-3-4.c readWavHead.c writeWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FRAMESIZE 256

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);
int volChange(short *in, short *out, float val, int len);

void main(int argc, char *argv[])
{
	char inFile[256];  //�ǂݍ��݃t�@�C�������i�[����ϐ�
	char outFile[256]; //�o�̓t�@�C�������i�[����ϐ�
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	/*�R�}���h���C���̈���������Ȃ��Ɠ����Ȃ��̂ŁA�����`�F�b�N����*/
	if (argc < 3)
	{
		printf("�����ɓ�ȏ�̕�����������Ă�������\n");
		exit(0);
	}
	/*�R�}���h���C������t�@�C�����̎擾*/
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/*�t�@�C�����̊m�F*/
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/*Wav�̃w�b�_��ǂ�*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0)
	{
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}
	/*�ǂݍ��񂾃p�����[�^���R���\�[���ɏo��*/
	printf("�t�@�C���̒����� %d �T���v��\n", len);
	printf("�T���v�����O���g���� %d Hz\n", fs);
	printf("�`���l������ %d\n", chNum);
	printf("1�T���v���̃r�b�g���� %d �r�b�g\n", sampSize);

	fNum = (int)floor(len / FRAMESIZE);
	outlen = fNum * FRAMESIZE;

	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);
		volChange(dataIn, dataOut, 0.5, FRAMESIZE);
		fwrite(dataOut, sizeof(short), FRAMESIZE, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}

int volChange(short *in, short *out, float val, int len)
{
	float ff;
	int i;

	for (i = 0; i < len; i++)
	{
		ff = *in * val;
		*out = (short)floor(ff);
		in++;
		out++;
	}

	return i;
}