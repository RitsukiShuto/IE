/*
* IE Signal Processing #2
* 2020/05/18. 15:44:56

�M����������@�ۑ�K01�̂��߂̎Q�l�t�@�C���B
����������ǂ݁A����Ȃ�������ǋL�����������邱�ƁB

by mokam@cis
*/
#include <stdio.h>

void main()
{
	char inFile[] = "1000Sine-8k.raw"; // �ǂݍ��݃t�@�C����
	FILE *ifp;						   // �t�@�C���ɕR�Â���|�C���^. �ǂ��܂œǂ񂾂����L�^����Ă���
	short dataln;					   // �ǂݍ���ރf�[�^�̗̈�(1�v�f)

	ifp = fopen(inFile, "rb"); //�t�@�C����ǂݍ��݂Ń��[�h�ŊJ���B����ȍ~��ifp�Ńt�@�C���𑀍삷��

	for (int i = 0; i < 8000; i++)
	{
		fread(&dataln, sizeof(short), 1, ifp); // �t�@�C����ǂݏo�� => 'dataln'�Ɋi�[�����
		printf("%d\n", dataln);				   // �f�[�^���o��
	}

	fclose(ifp); //�t�@�C�������
}