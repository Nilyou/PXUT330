// Purpose: This head file declare the the functions among the library of libudkfat.a
//			which support the FAT16/FAT32 file system for PXUT device
// Author : Wang Shutian 
// Date   : 2010-1-12
// Version: 1.0

#define USB_HOST_MODE 0     //Set USB Controller to Host mode
#define USB_SLAVE_MODE 1	//Set USB Controller to Slave mode

/*  
 * purpose  : U�̳�ʼ��
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkInitialize(void);
/*  
 * purpose  : U�̰�ȫ�γ�
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkSafelyClosed(void);
/*  
 * purpose  : �оٵ�ǰĿ¼��Ϣ
 * paras	: dirbuf -- ���Ŀ¼��Ϣ
 * return	: 0 - failure ; 1 - success
 */
int UdkListDir(unsigned char *dirbuf);
/*  
 * purpose  : ����һ���ļ�
 * paras	:	name -- �ļ���
 *				extname -- �ļ�����׺
 *				attribute -- �ļ�����
 * return	: 0 - failure ; ���� - �ļ����
 * comment  : ����ļ��Ѿ����ڣ���򿪸��ļ�
 */
int UdkCreateFile(unsigned char name[8],unsigned char extname[3],unsigned char attribute);
/*  
 * purpose  : д�����ݵ��ļ���
 * paras	: file_no -- �ļ����
 *            length  -- д���ֽ���
 *			  pBuffer -- ������	
 * return	: 0 - failure ; 1 - success
 */
int UdkWriteFile(int file_no,unsigned int length, unsigned char *pBuffer);
/*  
 * purpose  : ���ļ��ж�ȡ����
 * paras	: file_no -- �ļ����
 *            length  -- ��ȡ�ֽ���
 *			  pBuffer -- ������	
 * return	: 0 - failure ; 1 - success
 */
int UdkReadFile(int file_no,unsigned int length, unsigned char *pBuffer);
/*  
 * purpose  : ��һ���Ѿ����ڵ��ļ�
 * paras	: name - �ļ���
 *				extname	- �ļ�����׺	
 * return	: 0 - failure ; ���� -- �ļ����
 */
int UdkOpenFile(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : ɾ���ļ�����Ŀ¼
 * paras	: name - �ļ���
 *				extname	- �ļ�����׺	
 * return	: 0 - failure ; 1 - success
 */
int UdkDeleteFile(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : �ر�ָ���ļ�
 * paras	: file_no -- �ļ����
 * return	: 0 - failure ; 1 - success
 */
int UdkCloseFile(int file_no);
/*  
 * purpose  : �õ��ļ�����
 * paras	: file_no -- �ļ����
 * return	: 0 - failure ; 1 - success
 */
unsigned int UdkGetFileLength(int file_no);
/*  
 * purpose  : �����ļ�ָ��
 * paras	: file_no -- �ļ����
 *            pointer -- �ļ�ָ��[ƫ��]
 * return	: 0 - failure ; 1 - success
 */
int UdkSetFilePointer(int file_no,unsigned long pointer);
/*  
 * purpose  : ����Ŀ¼
 * paras	:	name -- Ŀ¼��
 *				extname -- Ŀ¼����׺
 *				attribute -- Ŀ¼���� 
 * return	: 0 - failure ; 1 - success
 */
int UdkCreateDir(unsigned char name[8],unsigned char extname[3],unsigned char attribute);
/*  
 * purpose  : ������Ŀ¼
 * paras	: name -- Ŀ¼��
 *				extname -- Ŀ¼����׺
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterChildDir(unsigned char name[8],unsigned char extname[3]);
/*  
 * purpose  : ���븸Ŀ¼
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterParentDir(void);
/*  
 * purpose  : �����Ŀ¼
 * paras	: 
 * return	: 0 - failure ; 1 - success
 */
int UdkEnterRootDir(void);
/*  
 * purpose  : ��ȡU��������Ϣ
 * paras	: pFreeSpaceLowPart   -- U�����ɿռ�����ֽ�
 *			  pFreeSpaceHighPart  -- U�����ɿռ�����ֽ�		
 *			  pTotalSpaceLowPart  -- U���ܿռ�����ֽ�
 *			  pTotalSpaceHighPart -- U���ܿռ�����ֽ�
 * return	: Cluster Size
 */
int UdkGetCapacity(unsigned int *pFreeSpaceLowPart, unsigned int *pFreeSpaceHighPart, unsigned int *pTotalSpaceLowPart,unsigned int *pTotalSpaceHighPart);
/*  
 * purpose  : �����ϴδ�����Ϣ
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
