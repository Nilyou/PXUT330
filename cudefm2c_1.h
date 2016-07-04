
#ifndef _LIMITS_H___
#include <limits.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

/* int:   signed 32-bit integer , u_int:   unsigned 32-bit integer */
/* short: signed 16-bit integer , u_short: unsigned 16-bit integer */
/* char:  signed  8-bit integer , u_char:  unsigned  8-bit integer */

#define SAMPLE_FREQ			100		// MHz
#define MAX_GAIN_DB			1200	// 0.1dB

#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		240
#define FULL_ECHO_POINTS	308
#define FULL_ECHO_HEIGHT	200
#define EFFE_ECHO_POINTS	280

#define ASK_PARAMETER		0x100

#define AMP_BAND_NARROW_1	0x110
#define AMP_BAND_NARROW_2	(AMP_BAND_NARROW_1 + 1)
#define AMP_BAND_WIDE		(AMP_BAND_NARROW_2 + 1)

#define FULL_WAVE			0x120
#define POSI_HALF_WAVE		(FULL_WAVE + 1)
#define NEGA_HALF_WAVE		(POSI_HALF_WAVE + 1)
#define RF_WAVE				(NEGA_HALF_WAVE + 1)

#define LOW_DAMPING			0x130
#define HIGH_DAMPING		(LOW_DAMPING + 1)

#define T_T_SINGLE			0x140
#define T_R_DUAL			(T_T_SINGLE + 1)

#define LOW_PULSE			0x150					/*  50V */
#define MED1_PULSE			(LOW_PULSE + 1)			/* 100V */
#define MED2_PULSE			(MED1_PULSE + 1)		/* 200V */
#define MED3_PULSE			(MED2_PULSE + 1)		/* 300V */
#define MED4_PULSE			(MED3_PULSE + 1)		/* 400V */
#define MED5_PULSE			(MED4_PULSE + 1)		/* 500V */
#define HIGH_PULSE			(MED5_PULSE + 1)		/* 600V */

#define PM_COPY_READ		0x210
#define PM_COPY_WRITE		(PM_COPY_READ + 1)
#define PM_COPY_MOVE		(PM_COPY_WRITE + 1)

#define USB_DATA_TRANSFER_OK		0
#define USB_DATA_TRANSFER_BUSY		1
#define USB_DATA_TRANSFER_ERROR		2
#define USB_DATA_TRANSFER_DECLINED	3
#define USB_DATA_TRANSFER_NOT_READY 4

#define RED_B0				0x01
#define RED_B1				0x02
#define RED_B2				0x04
#define GREEN_B0			0x08
#define GREEN_B1			0x10
#define GREEN_B2			0x20
#define BLUE_B0				0x40
#define BLUE_B1				0x80

#define COLOR_RED			(RED_B0 | RED_B1 | RED_B2)
#define COLOR_RED_L			(RED_B2)
#define COLOR_GREEN			(GREEN_B0 | GREEN_B1 | GREEN_B2)
#define COLOR_GREEN_L		(GREEN_B2)
#define COLOR_BLUE			(BLUE_B0 | BLUE_B1)
#define COLOR_BLUE_L		(BLUE_B1)
#define COLOR_YELLOW		(COLOR_RED | COLOR_GREEN)
#define COLOR_PINK			(COLOR_RED | COLOR_BLUE)
#define COLOR_CYAN			(COLOR_GREEN | COLOR_BLUE)
#define COLOR_WHITE			(COLOR_RED | COLOR_GREEN | COLOR_BLUE)
#define COLOR_GREY			(COLOR_RED_L | COLOR_GREEN_L | COLOR_BLUE_L)
#define COLOR_BLACK			0x000

#define ECHO_RED_B0			0x01
#define ECHO_RED_B1			0x02
#define ECHO_RED_B2			0x04
#define ECHO_GREEN_B0		0x08
#define ECHO_GREEN_B1		0x10
#define ECHO_GREEN_B2		0x20
#define ECHO_BLUE_B0		0x40
#define ECHO_BLUE_B1		0x80

#define ECHO_COLOR_RED		(ECHO_RED_B0 | ECHO_RED_B1 | ECHO_RED_B2)
#define ECHO_COLOR_GREEN	(ECHO_GREEN_B0 | ECHO_GREEN_B1 | ECHO_GREEN_B2)
#define ECHO_COLOR_BLUE		(ECHO_BLUE_B0 | ECHO_BLUE_B1)
#define ECHO_COLOR_YELLOW	(ECHO_COLOR_RED | ECHO_COLOR_GREEN)
#define ECHO_COLOR_PINK		(ECHO_COLOR_RED | ECHO_COLOR_BLUE)
#define ECHO_COLOR_CYAN		(ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_WHITE	(ECHO_COLOR_RED | ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_BLACK	0x000

typedef struct
{
    u_int 	Gate1PeakSampPoints ;
    u_int 	Gate2PeakSampPoints ;
    u_int 	Gate3PeakSampPoints ;
    u_short Gate1PeakPos ;
    u_short Gate2PeakPos ;
    u_short Gate3PeakPos ;
    u_short Gate1PeakAmp ;
    u_short Gate2PeakAmp ;
    u_short Gate3PeakAmp ;
} GATE_PEAK;

typedef struct
{
    u_short Position ;
    u_short Width ;
    u_short Height ;
    u_char  Type ;
    u_char  AlarmEnable ;
} GATE_PARA;

typedef struct
{
    u_char	second ;
    u_char	minute ;
    u_char	hour ;
    u_char	date ;
    u_char	month ;
    u_char	day ;
    u_char	yearl ;
    u_char  yearh ;
} TIME_CLOCK;

/* set repeat rate of data acquisition in Hz(100, 1000)
   return value: previous rate */
void	SetRepeatRate(u_int rate) ;
/* switch on-off echo display , ena = 1 -> on ; mode_fill = 1 -> echo fill display */
void 	EnableEchoDisplay(u_int ena, u_int mode_fill) ;
/* set screen background color */
void	SetBackgroundColor(u_int color) ;
/* set screen render color */
void 	SetDisplayColor(u_int color) ;
/* mode_xor: 1 -> read-xor-write; 0 -> overwrite */
void 	SetDisplayMode(u_int mode_xor) ;
/* buffer echo color data, pos_b: echo point start position(<508)
   pos_e: echo point end position(<508) , rgb(b0-b7): echo display */
void	SetEchoColor(u_int pos_s, u_int pos_e, u_int rgb) ;
/* validate echo color */
void 	UpdateEchoColor() ;
/* set echo dc level */
void 	SetEchoBaseLine(u_int level) ;
/* h_start: horizontal start(>1), : h_end: horizontal end(>h_start),
   v_start: vertical offset, v_half: vertical half screen display */
void 	SetEchoLayout(u_int h_start, u_int h_end, u_int v_start, u_int v_half) ;
/* size_xy: (b15-b0) -> x-dot size; (b31-b16) -> y-dot size */
void 	UserDotsOut(u_int x, u_int y, u_short* pDots, u_int size_xy) ;
/* display chinese character string pointed to (pzText) with length (len) at (x,y) */
void 	CTextOut(u_int x, u_int y, u_short* pzText, u_int len) ;
/* display english character string pointed to (pzText) with length (len) at (x,y) */
void 	ETextOut(u_int x, u_int y, char* pzText, u_int len) ;
/* draw a line from point(x1,y1) to (x2,y2), including both points */
void 	DrawLine(u_int x1, u_int y1, u_int x2, u_int y2) ;
/* draw a point at (x,y) */
void 	DrawPixel(u_int x, u_int y) ;
/* get one screen pixel color value from (x,y) to (x,y) */
u_int 	GetPixel(u_int x, u_int y) ;
/* erase screen window */
void 	EraseWindow(u_int left, u_int top, u_int width, u_int height) ;
/* invert screen window */
void 	InvertWindow(u_int left, u_int top, u_int width, u_int height) ;
/* left_sd : b0-b15  -> screen window left-top corner x,
			 b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
void 	PushWindow(u_int left_sd, u_int top_sd, u_int width, u_int height) ;
/* left_sd : b0-b15  -> screen window left-top corner x,
			 b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
void	PopWindow(u_int left_sd, u_int top_sd, u_int width, u_int height) ;
/* convert integer decimal to ascii string according to the assigned Format */
/* Format: (b0-b7) is total digit number(including sign and point) ,
	   	   (b8-b15) is decimal digit number(excluding point) ,
	       (b16-b19)=0, 1, 2 -> no align, front align, back align
	       (b20-b23)=0 -> no positive sign, 1 -> add positive sign
	        b24 : determines if the null character is appended */
int 	DecimalToAscii(char* pzText, int Para, u_int Format) ;
/* convert float to integer decimal, ten_radix: digit number after point */
int 	FloatToDecimal(float fpar, u_int ten_radix) ;
/* set the system gain of flaw detector, gain >=0 and <=MAX_GAIN_DB with precision 0.1dB */
u_int 	SetAmplifierGain(u_int gain) ;
/* set the system frequency band of flaw detector,
   (band) must be in {AMP_BAND_NARROW_1, AMP_BAND_NARROW_2, AMP_BAND_WIDE} */
u_int 	SetAmplifierBand(u_int band) ;
/* set echo amplitude rejection, reject >=0 and <=800 with precision 0.1% */
u_int 	SetEchoReject(u_int reject) ;
/* set echo acquisition mode ,
   (mode) must be in {FULL_WAVE, POSI_HALF_WAVE, NEGA_HALF_WAVE, RF_WAVE} */
u_int 	SetEchoMode(u_int mode) ;
/* set ultrasonic inspection range in sampling points ,  */
u_int 	SetEchoRange(u_int range) ;
/* set inspection delay in sampling points */
u_int 	SetEchoDelay(u_int edelay) ;
/* set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2 */
void 	SetPeakGate(GATE_PARA* pGate , int gate_index) ;
/* set the connection mode of transmitting and receiving probes,
   (mode) must be in {T_T_SINGLE, T_R_DUAL} */
void 	SetProbeMode(u_int mode) ;
/* set the damping for the transmitting probe,
   (damp) must be in {LOW_DAMPING, HIGH_DAMPING} */
void 	SetProbeDamping(int damp) ;
/* set the pulse strength for the transmitting ptobe,
   (pulse) must be in {LOW_PULSE, MEDIUM1_PULSE, MEDIUM2_PULSE, HIGH_PULSE} */
void 	SetTransmitterVoltage(u_int pulse) ;
/* width: 1-127 */
void SetPulseWidth(u_int width) ;
/* bEnable = 1 for activating data acquisition ,
   bEnable = 0 for disabling data acquisition */
u_int 	EnableDataAcquisition(u_int bEnable, u_int bWait) ;
u_int 	IfDataAcquisitionActivated() ;
u_int 	ActivateEchoEnvelope(int act) ;
/* get the pointer to kernel gate peak buffer, don't modify! */
GATE_PEAK* GetGatePeakBuffer() ;
/* get the pointer to kernel echo data buffer, don't modify! */
u_char* GetSampleBuffer() ;
u_char* GetEchoEnvelopeBuffer() ;
/* get the pointer to kernel dac data buffer, don't modify! */
/* transfer data to/from/within the system protected memory,
   the address of the protected memory start from 0,
   copymode must be in {PM_COPY_READ, PM_COPY_WRITE, PM_COPY_MOVE} ,
   it's better to check the return value to see if succeeded(>0) or failed(0) */
int 	CopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode) ;
/* noise reduction , grade: 0-4 */
void 	DeNoise(u_int grade) ;
/* get the elapsed time from power-on, resolution is 1 ms */
u_int 	GetElapsedTime() ;
/* fill the assigned structure TIME_CLOCK, (tm) must be already declared */
void 	GetSystemTime(TIME_CLOCK* tm) ;
/* return value = 0, the printer is not ready, otherwise ready */
u_int 	IsPrinterReady() ;
/* send data to printer, pdata: the least byte is effective */
u_int 	SendByteToPrinter(u_int pdata) ;
/* return value > 0 , denoting some key has been and is being pressed,
   return value < 0 , denoting some key was pressed and just released,
   =0 , no key is pressed */
int 	GetKeyIndex(void) ;
/* increment: b6-b0, decrement: b8-b14, switch pressed: b15 = 1 */
u_int 	GetRotaryValue() ;
/* get the return value from battery management
   the return vaule: b0 -> battery charge fast(1) or trickle(0) ;
   		             b1 -> battery charge on(1) or off(0) ;
   		             b2 -> battery connected(1) or disconnected(0) ;
   		             b3 -> DC adapter connected(1) or disconnected(0) ;
   		             b8-b15 -> battery capacity value < 32 */
u_int 	GetBatteryStatus() ;
/* bEnable = 0 -> buzzer off, bEnable > 0 -> buzzer on */
void 	SwitchBuzzerOn(u_int bEnable) ;
/* bEnable = 0 -> alarm led off, bEnable > 0 -> alarm led on */
void 	SwitchAlarmOn(u_int bEnable) ;
/* adjust lcd brightness, 0 <= grade <= 31 */
void 	SetLCDBrightness(u_int grade) ;
/* set USB mode, 0 -> HOST, 1 -> SLAVE */
u_int	SetUsbMode(u_int mode) ;
/* turn off power to usb device */
void 	SwitchUsbDevicePowerOff() ;
/* 1 -> Inserted Or Removed, 0 -> no action */
u_int 	IsUsbDeviceInsertedOrRemoved() ;
/* 1 -> connected, 0 -> disconnected */
u_int 	IsUsbDeviceAttached() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
u_int	UsbHostDataOutStatus() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
u_int	UsbHostDataInStatus() ;
/* usb host data out setting, 1 -> success, 0 -> fail */
u_int 	UsbHostDataOut(char* pAddr, u_int length) ;
void 	UsbHostDataOutToggle() ;
void 	UsbHostOutAbort() ;
/* usb host data in setting, 1 -> success, 0 -> fail */
u_int 	UsbHostDataIn(char* pAddr, u_int length) ;
void 	UsbHostDataInToggle() ;
void 	UsbHostInAbort() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
u_int	UsbDevDataOutStatus() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
u_int	UsbDevDataInStatus() ;
/* usb data out setting */
void 	UsbDevDataOut(char* pAddr, u_int length) ;
/* usb data in setting  */
void    UsbDevDataIn(char* pAddr, u_int length) ;
/* clear usb transfer error, dir: 0-> in; 1-> out,
   return value: 0-> unsuccessful, 1-> done */
u_int	UsbDevClearError(u_int dir) ;
