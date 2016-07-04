// Purpose: This head file declare the the functions among the library of libudkfat.a
//			which support the FAT16/FAT32 file system for PXUT device
// Author : Wang Shutian 
// Date   : 2010-1-12
// Version: 1.0

#define USB_HOST_MODE 0     //Set USB Controller to Host mode
#define USB_SLAVE_MODE 1	//Set USB Controller to Slave mode

/*  
 * purpose  : U盘初始化
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkInitialize(void);
/*  
 * purpose  : U盘安全拔除
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkSafelyClosed(void);
/*  
 * purpose  : 列举当前目录信息
 * paras	: dirbuf -- 输出目录信息
 * return	: 0 - failure ; 1 - success
 */
int UdkListDir(unsigned char *dirbuf);
/*  
 * purpose  : 建立一个文件
 * paras	:	name -- 文件名
 *				extname -- 文件名后缀
 *				attribute -- 文件属性
 * return	: 0 - failure ; 其它 - 文件句柄
 * comment  : 如果文件已经存在，则打开该文件
 */
int UdkCreateFile(unsigned char name[8],unsigned char extname[3],unsigned char attribute);
/*  
 * purpose  : 写入数据到文件中
 * paras	: file_no -- 文件句柄
 *            length  -- 写入字节数
 *			  pBuffer -- 缓冲区	
 * return	: 0 - failure ; 1 - success
 */
int UdkWriteFile(int file_no,unsigned int length, unsigned char *pBuffer);
/*  
 * purpose  : 从文件中读取数据
 * paras	: file_no -- 文件句柄
 *            length  -- 读取字节数
 *			  pBuffer -- 缓冲区	
 * return	: 0 - failure ; 1 - success
 */
int UdkReadFile(int file_no,unsigned int length, unsigned char *pBuffer);
/*  
 * purpose  : 打开一个已经存在的文件
 * paras	: name - 文件名
 *				extname	- 文件名后缀	
 * return	: 0 - failure ; 其它 -- 文件句柄
 */
int UdkOpenFile(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : 删除文件或者目录
 * paras	: name - 文件名
 *				extname	- 文件名后缀	
 * return	: 0 - failure ; 1 - success
 */
int UdkDeleteFile(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : 关闭指定文件
 * paras	: file_no -- 文件句柄
 * return	: 0 - failure ; 1 - success
 */
int UdkCloseFile(int file_no);
/*  
 * purpose  : 得到文件长度
 * paras	: file_no -- 文件句柄
 * return	: 0 - failure ; 1 - success
 */
unsigned int UdkGetFileLength(int file_no);
/*  
 * purpose  : 设置文件指针
 * paras	: file_no -- 文件句柄
 *            pointer -- 文件指针[偏移]
 * return	: 0 - failure ; 1 - success
 */
int UdkSetFilePointer(int file_no,unsigned long pointer);
/*  
 * purpose  : 建立目录
 * paras	:	name -- 目录名
 *				extname -- 目录名后缀
 *				attribute -- 目录属性 
 * return	: 0 - failure ; 1 - success
 */
int UdkCreateDir(unsigned char name[8],unsigned char extname[3],unsigned char attribute);
/*  
 * purpose  : 进入子目录
 * paras	: name -- 目录名
 *				extname -- 目录名后缀
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterChildDir(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : 进入父目录
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterParentDir(void);
/*  
 * purpose  : 进入根目录
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterRootDir(void);
/*  
 * purpose  : 获取U盘容量信息
 * paras	: pFreeSpaceLowPart   -- U盘自由空间低四字节
 *			  pFreeSpaceHighPart  -- U盘自由空间高四字节		
 *			  pTotalSpaceLowPart  -- U盘总空间低四字节
 *			  pTotalSpaceHighPart -- U盘总空间高四字节
 * return	: Cluster Size
 */
int UdkGetCapacity(unsigned int *pFreeSpaceLowPart, unsigned int *pFreeSpaceHighPart, unsigned int *pTotalSpaceLowPart,unsigned int *pTotalSpaceHighPart);
/*  
 * purpose  : 返回上次错误信息
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkGetLastError(void);

/*
 * purpose	: Initialize LZW Workspace 
 */
void LzwInitBuf(void);
/*
 * purpose	: Destroy LZW Workspace 
 */
void LzwDestroyBuf(void);
/*
 * purpose   : Compress Data
 * paras     : 
 *				in_len   --  input data length
 *				in_buf   --  pointer to input data buffer
 *				pOut_len --  pointer to compressed data length
 *				out_buf  --  pointer to compressed data
 */
void LzwEncode(u_int in_len, u_char *in_buf, u_long *pOut_len, u_char *out_buf);

/*
 * purpose	: Decompress Data
 * paras		:
 *				in_len   --  input compressed data length
 *				in_buf   --  pointer to compressed data buffer
 *				pOut_len --  pointer to decompressed data length
 *				out_buf  --  pointer to decompressed data
 */
void LzwDecode(u_int in_len, u_char *in_buf, u_long *pOut_len, u_char *out_buf );
