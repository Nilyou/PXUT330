
#ifndef _UDISK_H_
#define _UDISK_H_


//#ifndef _SYS_TYPES_H
//#include <sys/types.h>
//#endif


#define USB_HOST_MODE 0     //Set USB Controller to Host mode
#define USB_SLAVE_MODE 1	//Set USB Controller to Slave mode
#define TRUE 1
#define FALSE 0

#define BUFFER_LENGTH 32768
#define GATE_PEAK_SIZE 300  //24
#define ECHO_PACKAGE_SIZE 500 //300

#define C_WRITE_TO_UDISK 1
#define C_WRITE_INTERVAL 1000


typedef struct _UDISK_INFO
{
    u_int IsAttached;           // U盘是否连接
    u_int filelen;				// 文件大小
    u_int operatormode;			// 1 -- Write ; 2 --Read
    u_int writemode;			// 0 ,1, 2
    u_int sample_num;
    u_int transfer_surplus;
    u_int FreeSpaceLowPart;     // U盘自由空间低四字节
    u_int FreeSpaceHighPart;    // U盘自由空间高四字节
    u_int TotalSpaceLowPart;    // U盘总空间低四字节
    u_int TotalSpaceHighPart;   // U盘总空间高四字节
    u_int DiskIsFull;			// U盘满标志
    u_int DataHeaderMark;		//0,参数不变；1,写参数
    u_int last_tick_count;
    u_int TotalWriteTimes;
    u_int LastFileLength;	//最后一次存的长度
    char  filename[8];
    TIME_CLOCK system_time;
    char *pEchoDataBuffer;
    char *pEchoWriteBuffer;

    u_char DataBuffer[2][BUFFER_LENGTH*16]; // 采集数据BUFFER [8]
    u_char UDiskBuffer[BUFFER_LENGTH*8]; // U盘交换BUFFER [3]
} UDISK_INFO;

#define  ShowProblemLine()  {\
			char msg[200];\
			memset(msg,0,200);\
			strcat(msg, "ERROR:");\
			strcat(msg, __FILE__);\
			char temp[10];\
			memset(temp,0,10);\
			DecimalToAscii(temp, __LINE__,3);\
			strcat(msg, temp );\
			strcat(msg, __FUNCTION__);\
			FTextOut(10, 200,msg,strlen(msg),COLOR_YELLOW);\
			RDBuzzerOn(10); \
				}

#define BCDtoDecimal(x) (((x&0xf0)>>4)*10 + (x&0x0f))

extern UDISK_INFO g_UDiskInfo;

extern int EchoDataStorageFunc(int keycode,int mode);
extern void StorageData(int keycode);
extern int FileClose(int mode);
extern void Write_UDisk_Len(int xpos,int ypos,int SpacePart,int Cluster,int filelen);
extern int SaveToUDisk(int file_no);
#endif


