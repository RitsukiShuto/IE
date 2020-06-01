/*
2020-2-6.c
Wav�t�@�C�����J���ăt�@�C���̃t�H�[�}�b�g���R���\�[���ɏo�͂���
�f�[�^��wav�t�@�C���ɏo�͂���

�o�̓t�@�C���̓w�b�_�������ďI�����Ă�̂ŁA���̂܂܂��Ɛ�����wav�t�@�C���͂ł��Ȃ�
���g��ǋL���邱��

�R���p�C�����@��
cl 2020-2-6.c readWavHead.c writeWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <math.h>

#define FRAMESIZE 256

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);

void main()
{
	//�ϐ��̐錾
	char inFile[] = "VoiceMono.wav";	  //�ǂݍ��݃t�@�C����
	char outFile[] = "VoiceMono_out.wav"; //�ǂݍ��݃t�@�C����
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	//�t�@�C�����J��
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

	//�f�[�^�����烋�[�v�̉񐔂��v�Z�B
	fNum = (int)floor(len / FRAMESIZE);

	//fNum���珑�����݃t�@�C���̃T�C�Y���v�Z
	outlen = fNum * FRAMESIZE;

	/*
	�������ݗp��wav�t�@�C���̃w�b�_���������ށB�f�[�^�̃T�C�Y�܂�,
	��������ł���̂ŁA���̌�͂��Ȃ炸outlen�Ɠ�������fwrite���邱��
	*/
	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(&dataIn[i], sizeof(short), 1, ifp);
		for (int j = 0; j < FRAMESIZE; j++)
		{
			dataOut[j] = dataIn[j];
		}
		outFile[i] = fwrite(dataOut, sizeof(short), 256, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}
