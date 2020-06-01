/* wav.h
 RIFF-WAV��ǂނ��߂̍\���̂̒�`
*/

/* chunk�̃^�C�v�𒲂ׂ邽�� */
#define RIFF 0x46464952
#define WAVE 0x45564157
#define fmt_ 0x20746d66
#define fact 0x74636166
#define data 0x61746164

/* data format */
#define WAVE_FORMAT_PCM	(0x0001)  /*Microsoft Pulse Code Modulation (PCM)*/
#define FORMAT_MULAW	(0x0101)  /*IBM mu-law format*/
#define IBM_FORMAT_ALAW	(0x0102)  /*IBM a-law format*/
#define IBM_FORMAT_ADPCM (0x0103) /*IBM AVC Adaptive Differential PCM format*/

/*
typedef struct _ID
{
  char RIFF_ID[4];
}ID;
*/
typedef long ID;

typedef struct _RIFF_HDR{     
  ID  ckID;  	/* identifier string = "RIFF"*/
  unsigned long ckSize;    	/* remaining length after this header*/
} RIFF_HDR;

typedef struct _COMMON_FIELD // ���̍\���̂�PCM�ȊO�Ŏg���Ƃ��͒���
{
  unsigned short wFormatTag;         // Format category
  unsigned short wChannels;          // Number of channels
  unsigned long wSamplesPerSec;    // Sampling rate
  unsigned long wAvgBytesPerSec;   // For buffer estimation(fs * ch * byte(bit/8))
  unsigned short wBlockAlign;        // Data block size(ch * byte)
  unsigned short wBitsPerSample; /* Sample size(8 * byte) ���̗v�f��PCM�ȊO�ł͖���*/
} COMMON_FIELD;

typedef struct _PCM_SPECIFIC
{
  unsigned short wBitsPerSample; /* Sample size(8 * byte)*/
} PCM_SPECIFIC;
