/*
2020-3-1.c

�R�}���h���C������ϐ������擾����v���O������


�R���p�C�����@��
cl 2020-3-1.c

by mokam@cis
*/

#include<stdio.h>
#include<stdlib.h>

#define SAMPLESIZE 8000  //�ǂݍ��ރf�[�^�̐�

void main(int argc, char* argv[]) {
	char inFile[256]; //�ǂݍ��݃t�@�C�������i�[����ϐ�
	char outFile[256]; //�o�̓t�@�C�������i�[����ϐ�

	/*�R�}���h���C���̈���������Ȃ��Ɠ����Ȃ��̂ŁA�����`�F�b�N����*/
	if (argc < 3) {
		printf("�����ɓ�ȏ�̕�����������Ă�������\n");
		exit(0);
	}
	/*�R�}���h���C������t�@�C�����̎擾*/
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/*�t�@�C�����̊m�F*/
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);



}