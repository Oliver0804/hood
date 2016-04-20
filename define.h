#ifndef _DEFINE_H
#define _DEFINE_H
#define LED13  13
#define s0  11
#define s1  10
#define s2  9
#define s3  8

#define Top_Light             7
#define Bottom_Light          6
#define Fan                   5
#define Relay_4               4

#define Buzz                  27

#define esp8266_ret           3

#define set_warning_temp 50
#define set_max_temp 100// set=238

#define HMI_TOUCH_EVENT       0x65
#define HMI_PAGE_ID           0x66
#define HMI_TOUCH_XY          0x67
#define HMI_SLEEP_MODE_TOUCH  0x68
#define HMI_TEXT              0x70
#define HMI_VALUE             0x71
#define HMI_DEVICE_SLEEP      0x86
#define HMI_DEVICE_WAKE_UP    0x87
#define HMI_SYS_START_UP      0x88
#define HMI_SD_CARD_UPDATE    0x89

#define HMI_INVALID_CMD       0x00
#define HMI_CMD_EXECUTE       0x01
#define HMI_OBJECT_ID_INVALID 0x02
#define HMI_PAGE_ID_INVALID   0x03
#define HMI_PIC_ID_INVALID    0x04
#define HMI_FONT_ID_INVALID   0x05
#define HMI_BAUDRATE_INVALID  0x11
#define HMI_CURVE_INVALID     0x12
#define HMI_VARIABLE_INVALID  0x1A
#define HMI_VALUE_INVALID     0x1B

#endif
