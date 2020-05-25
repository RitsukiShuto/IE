/*
2020-2-4.c
Wav�t�@�C�����J���ăt�@�C���̃t�H�[�}�b�g���R���\�[���ɏo�͂���
�f�[�^��ǂݍ���ŃR���\�[���ɏo�͂�����

�ꕔ����菜���Ă�̂ł��̂܂܂ł͓��삵�Ȃ��B���Ǝ�����ǂ݁A
���������邱��

�R���p�C�����@��
cl 2020-2-4.c readWavHead.c 

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

//�t���[���������߂Ă����B�ȉ�FRAMESIZE�Ə�����256�ɒu�������悤�ɂȂ�B
#define FRAMESIZE 256

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);

void main()
{
	/*�ϐ��̐錾*/
	char inFile[] = "VoiceMono.wav"; //�ǂݍ��݃t�@�C����
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE]; //�ǂݍ��񂾃f�[�^������z��
	//�e��ϐ�
	int len, sampSize;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	/*�t�@�C�����J��*/
	ifp = fopen(inFile, "rb");

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

	/*�f�[�^������J��Ԃ��̐����v�Z����*/
	/*������fNum���v�Z���鎮������Ɨǂ�*/

	
	/*���炩���ߌv�Z�������[�v�̉񐔂����J��Ԃ�*/
	for (int i = 0; i < fNum; i++) {

		/*�t���[���P�ʂł܂Ƃ߂ăf�[�^��ǂݍ���*/
		//������fread�����t���[���P�ʂœǂݍ��ނ悤�ɒǋL����		
		
		/*�ǂݍ��񂾃f�[�^���R���\�[���ɏo��*/
		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%d\n", dataIn[j]);
		}
	}
	/*�t�@�C�������*/
	fclose(ifp);

}
