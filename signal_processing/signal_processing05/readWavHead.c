/*
readWavHead.c
RIFF-Wav�̃w�b�_��ǂݍ���
 */

#include <stdio.h>

#include "wav.h"

int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short  *chNum, int *sampSize)
{
  int i;
  char c;
  RIFF_HDR ckHead;
  COMMON_FIELD comCk;
  ID iD;
  long fileSize, ckLength;
  /*  PCM_SPECIFIC BitPS;	 Sample size(8 * byte)*/

/*   �擪��ID�ƃt�@�C���T�C�Y��ǂ� */
  fread(&ckHead, sizeof(ckHead), 1, fp);
  if(ckHead.ckID != RIFF){
    puts("This is not riff file.");
    exit(-1);
  }
  fileSize = ckHead.ckSize + 8;
//  printf("read lenght %d byte\n", fileSize);

/* Wav���ǂ�����ID�ɂ�蒲�ׂ� */
  fread(&iD, sizeof(ID), 1, fp);
  if(iD != WAVE){
    puts("This is not riff-wave file.");
    exit(-1);
  }

/*   ����Chunk��ID�ƃT�C�Y��ǂ� */
  fread(&ckHead, sizeof(ckHead), 1, fp);

  /*     fmt Chunk�̏ꍇ */
  if(ckHead.ckID == fmt_){
    ckLength = ckHead.ckSize;
  //  printf("chunk Size %dbyte\n", ckHead.ckSize);
/*     printf("comckSize %d\n", sizeof(comCk)); */
    /*     Chunk�̓��e��ǂ� */
    fread(&comCk, sizeof(comCk), 1, fp);
/*     printf("size comCk is %d\", sizeof(comCk)); */
/*     PCM�f�[�^�̏ꍇ */
    if(comCk.wFormatTag == WAVE_FORMAT_PCM){
//      printf("PCM FILE: ");
      *chNum = comCk.wChannels;
      *fs = comCk.wSamplesPerSec;
      *sampSize = comCk.wBitsPerSample;
//      printf("%d channels: fs %dkHz: ", *chNum, *fs);
//      printf("%d byte data: ", comCk.wAvgBytesPerSec / (*fs * *chNum));
//      printf("1 block %d bytes: ", comCk.wBlockAlign);
//      printf("%d bit per sample\n", *sampSize);
    }else{
      puts("not PCM");
      exit(-1);
    }
  }else{
    puts("Cannot search Format Field.");
    exit(-1);
  }

/*   �����ăw�b�_��ǂ� ID ��data�ɂȂ�܂ŃX�L�b�v */
  while(1){
    fread(&ckHead, sizeof(ckHead), 1, fp);
    if(ckHead.ckID != data){
/*       data����Ȃ������� �X�L�b�v*/
//      printf("chunk is 0x%x: ", ckHead.ckID);
//      printf("skip %d byte \n", ckHead.ckSize);
      for(i = 0; i < ckHead.ckSize; i++){
	fread(&c, sizeof(char), 1, fp);
      }
    }else{
      /*       data�������� */
      *dataLength = ckHead.ckSize / (comCk.wBlockAlign);
 //     printf("data length %d sample / ch\n", *dataLength);
      return(fileSize);
    }
    if(feof(fp) != 0){
      puts("file end");
      return(-1);
    }
  }
/*   return(-1); */
}
