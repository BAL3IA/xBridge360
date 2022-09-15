#pragma once 


#include "libusb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <errno.h> 
#include <linux/usb/ch9.h>
#include <usbg/usbg.h>
#include <usbg/function/hid.h>

#define idPD 0x028e
#define idVD 0x045e
#define ADD_IN 0x81;
#define ADD_OUT 0x02;
#define DATA_LEN 20 
 
const unsigned char bBreak[] = { 0x00, 0x14, 0x30, 0x03, 0xff, 0xff};
  
// retorna 1 se a combinação de botoes indicar SAÍDA do programa
int cmdSAIDA(const unsigned char* _1) { 

    for (int i = 0; i < 6; i++) {
 
        if (_1[i] != bBreak[i]) { 
            return 0;}
    }
    return 1;
}

// // report descriptor do controle obtido de https://github.com/ViGEm/ViGEmBus/issues/40
// static char report_des[] = {

//     0x05, 0x01,         //(GLOBAL) USAGE_PAGE         0x0001 Generic Desktop Page 
//     0x09, 0x05,         //(LOCAL)  USAGE              0x00010005 Game Pad (Application Collection)  
//     0xA1, 0x01,         //(MAIN)   COLLECTION         0x01 Application (Usage=0x00010005: Page=Generic Desktop Page, Usage=Game Pad, Type=Application Collection)
//     0xA1, 0x00,         //  (MAIN)   COLLECTION         0x00 Physical (Usage=0x0: Page=, Usage=, Type=) <-- Error: COLLECTION must be preceded by a USAGE <-- Warning: USAGE type should be CP (Physical Collection)
//     0x09, 0x30,         //    (LOCAL)  USAGE              0x00010030 X (Dynamic Value)  
//     0x09, 0x31,         //    (LOCAL)  USAGE              0x00010031 Y (Dynamic Value)  
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0)  <-- Info: Consider replacing 15 00 with 14
//     0x27, 0xFF, 0xFF, 0x00, 0x00,  //    (GLOBAL) LOGICAL_MAXIMUM    0x0000FFFF (65535)  
//     0x95, 0x02,         //    (GLOBAL) REPORT_COUNT       0x02 (2) Number of fields  
//     0x75, 0x10,         //    (GLOBAL) REPORT_SIZE        0x10 (16) Number of bits per field  
//     0x81, 0x02,         //    (MAIN)   INPUT              0x00000002 (2 fields x 16 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xC0,               //  (MAIN)   END_COLLECTION     Physical 
//     0xA1, 0x00,         //  (MAIN)   COLLECTION         0x00 Physical (Usage=0x0: Page=, Usage=, Type=) <-- Error: COLLECTION must be preceded by a USAGE <-- Warning: USAGE type should be CP (Physical Collection)
//     0x09, 0x33,         //    (LOCAL)  USAGE              0x00010033 Rx (Dynamic Value)  
//     0x09, 0x34,         //    (LOCAL)  USAGE              0x00010034 Ry (Dynamic Value)  
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x27, 0xFF, 0xFF, 0x00, 0x00,  //    (GLOBAL) LOGICAL_MAXIMUM    0x0000FFFF (65535) <-- Redundant: LOGICAL_MAXIMUM is already 65535 
//     0x95, 0x02,         //    (GLOBAL) REPORT_COUNT       0x02 (2) Number of fields <-- Redundant: REPORT_COUNT is already 2 
//     0x75, 0x10,         //    (GLOBAL) REPORT_SIZE        0x10 (16) Number of bits per field <-- Redundant: REPORT_SIZE is already 16 
//     0x81, 0x02,         //    (MAIN)   INPUT              0x00000002 (2 fields x 16 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xC0,               //  (MAIN)   END_COLLECTION     Physical 
//     0x05, 0x01,         //  (GLOBAL) USAGE_PAGE         0x0001 Generic Desktop Page <-- Redundant: USAGE_PAGE is already 0x0001
//     0x09, 0x32,         //  (LOCAL)  USAGE              0x00010032 Z (Dynamic Value)  
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x26, 0xFF, 0x03,   //  (GLOBAL) LOGICAL_MAXIMUM    0x03FF (1023)  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields  
//     0x75, 0x0A,         //  (GLOBAL) REPORT_SIZE        0x0A (10) Number of bits per field  
//     0x81, 0x02,         //  (MAIN)   INPUT              0x00000002 (1 field x 10 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //  (GLOBAL) LOGICAL_MAXIMUM    0x00 (0)  <-- Info: Consider replacing 25 00 with 24
//     0x75, 0x06,         //  (GLOBAL) REPORT_SIZE        0x06 (6) Number of bits per field  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x81, 0x03,         //  (MAIN)   INPUT              0x00000003 (1 field x 6 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x05, 0x01,         //  (GLOBAL) USAGE_PAGE         0x0001 Generic Desktop Page <-- Redundant: USAGE_PAGE is already 0x0001
//     0x09, 0x35,         //  (LOCAL)  USAGE              0x00010035 Rz (Dynamic Value)  
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x26, 0xFF, 0x03,   //  (GLOBAL) LOGICAL_MAXIMUM    0x03FF (1023)  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x75, 0x0A,         //  (GLOBAL) REPORT_SIZE        0x0A (10) Number of bits per field  
//     0x81, 0x02,         //  (MAIN)   INPUT              0x00000002 (1 field x 10 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //  (GLOBAL) LOGICAL_MAXIMUM    0x00 (0)  <-- Info: Consider replacing 25 00 with 24
//     0x75, 0x06,         //  (GLOBAL) REPORT_SIZE        0x06 (6) Number of bits per field  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x81, 0x03,         //  (MAIN)   INPUT              0x00000003 (1 field x 6 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x05, 0x09,         //  (GLOBAL) USAGE_PAGE         0x0009 Button Page 
//     0x19, 0x01,         //  (LOCAL)  USAGE_MINIMUM      0x00090001 Button 1 Primary/trigger (Selector, On/Off Control, Momentary Control, or One Shot Control)  
//     0x29, 0x0A,         //  (LOCAL)  USAGE_MAXIMUM      0x0009000A Button 10 (Selector, On/Off Control, Momentary Control, or One Shot Control)  
//     0x95, 0x0A,         //  (GLOBAL) REPORT_COUNT       0x0A (10) Number of fields  
//     0x75, 0x01,         //  (GLOBAL) REPORT_SIZE        0x01 (1) Number of bits per field  
//     0x81, 0x02,         //  (MAIN)   INPUT              0x00000002 (10 fields x 1 bit) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //  (GLOBAL) LOGICAL_MAXIMUM    0x00 (0) <-- Redundant: LOGICAL_MAXIMUM is already 0 <-- Info: Consider replacing 25 00 with 24
//     0x75, 0x06,         //  (GLOBAL) REPORT_SIZE        0x06 (6) Number of bits per field  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields  
//     0x81, 0x03,         //  (MAIN)   INPUT              0x00000003 (1 field x 6 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x05, 0x01,         //  (GLOBAL) USAGE_PAGE         0x0001 Generic Desktop Page 
//     0x09, 0x39,         //  (LOCAL)  USAGE              0x00010039 Hat switch (Dynamic Value)  
//     0x15, 0x01,         //  (GLOBAL) LOGICAL_MINIMUM    0x01 (1)  
//     0x25, 0x08,         //  (GLOBAL) LOGICAL_MAXIMUM    0x08 (8)  
//     0x35, 0x00,         //  (GLOBAL) PHYSICAL_MINIMUM   0x00 (0)  <-- Info: Consider replacing 35 00 with 34
//     0x46, 0x3B, 0x01,   //  (GLOBAL) PHYSICAL_MAXIMUM   0x013B (315)  
//     0x66, 0x14, 0x00,   //  (GLOBAL) UNIT               0x0014 Rotation in degrees [1° units] (4=System=English Rotation, 1=Rotation=Degrees)  <-- Info: Consider replacing 66 1400 with 65 14
//     0x75, 0x04,         //  (GLOBAL) REPORT_SIZE        0x04 (4) Number of bits per field  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x81, 0x42,         //  (MAIN)   INPUT              0x00000042 (1 field x 4 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 1=Null 0=NonVolatile 0=Bitmap 
//     0x75, 0x04,         //  (GLOBAL) REPORT_SIZE        0x04 (4) Number of bits per field <-- Redundant: REPORT_SIZE is already 4 
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0)  <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //  (GLOBAL) LOGICAL_MAXIMUM    0x00 (0)  <-- Info: Consider replacing 25 00 with 24
//     0x35, 0x00,         //  (GLOBAL) PHYSICAL_MINIMUM   0x00 (0) <-- Redundant: PHYSICAL_MINIMUM is already 0 <-- Info: Consider replacing 35 00 with 34
//     0x45, 0x00,         //  (GLOBAL) PHYSICAL_MAXIMUM   0x00 (0)  <-- Info: Consider replacing 45 00 with 44
//     0x65, 0x00,         //  (GLOBAL) UNIT               0x00 No unit (0=None)  <-- Info: Consider replacing 65 00 with 64
//     0x81, 0x03,         //  (MAIN)   INPUT              0x00000003 (1 field x 4 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xA1, 0x02,         //  (MAIN)   COLLECTION         0x02 Logical (Usage=0x0: Page=, Usage=, Type=) <-- Error: COLLECTION must be preceded by a USAGE <-- Warning: USAGE type should be CL (Logical Collection)
//     0x05, 0x0F,         //    (GLOBAL) USAGE_PAGE         0x000F Physical Interface Device Page 
//     0x09, 0x97,         //    (LOCAL)  USAGE              0x000F0097 DC Enable Actuators (Selector)  
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x01,         //    (GLOBAL) LOGICAL_MAXIMUM    0x01 (1)  
//     0x75, 0x04,         //    (GLOBAL) REPORT_SIZE        0x04 (4) Number of bits per field <-- Redundant: REPORT_SIZE is already 4 
//     0x95, 0x01,         //    (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x91, 0x02,         //    (MAIN)   OUTPUT             0x00000002 (1 field x 4 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //    (GLOBAL) LOGICAL_MAXIMUM    0x00 (0)  <-- Info: Consider replacing 25 00 with 24
//     0x91, 0x03,         //    (MAIN)   OUTPUT             0x00000003 (1 field x 4 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x09, 0x70,         //    (LOCAL)  USAGE              0x000F0070 Magnitude (Dynamic Value)  
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x64,         //    (GLOBAL) LOGICAL_MAXIMUM    0x64 (100)  
//     0x75, 0x08,         //    (GLOBAL) REPORT_SIZE        0x08 (8) Number of bits per field  
//     0x95, 0x04,         //    (GLOBAL) REPORT_COUNT       0x04 (4) Number of fields  
//     0x91, 0x02,         //    (MAIN)   OUTPUT             0x00000002 (4 fields x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x09, 0x50,         //    (LOCAL)  USAGE              0x000F0050 Duration (Dynamic Value)  
//     0x66, 0x01, 0x10,   //    (GLOBAL) UNIT               0x1001 Time in seconds [1 s units] (1=System=SI Linear, 1=Time=Seconds)  
//     0x55, 0x0E,         //    (GLOBAL) UNIT_EXPONENT      0x0E (Unit Value x 10⁻²)  
//     0x26, 0xFF, 0x00,      //    (GLOBAL) LOGICAL_MAXIMUM    0x00FF (255)  
//     0x95, 0x01,         //    (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields  
//     0x91, 0x02,         //    (MAIN)   OUTPUT             0x00000002 (1 field x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x09, 0xA7,         //    (LOCAL)  USAGE              0x000F00A7 Start Delay (Dynamic Value)  
//     0x91, 0x02,         //    (MAIN)   OUTPUT             0x00000002 (1 field x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x65, 0x00,         //    (GLOBAL) UNIT               0x00 No unit (0=None)  <-- Info: Consider replacing 65 00 with 64
//     0x55, 0x00,         //    (GLOBAL) UNIT_EXPONENT      0x00 (Unit Value x 10⁰)  <-- Info: Consider replacing 55 00 with 54
//     0x09, 0x7C,         //    (LOCAL)  USAGE              0x000F007C Loop Count (Dynamic Value)  
//     0x91, 0x02,         //    (MAIN)   OUTPUT             0x00000002 (1 field x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xC0,               //  (MAIN)   END_COLLECTION     Logical 
//     0x05, 0x01,         //  (GLOBAL) USAGE_PAGE         0x0001 Generic Desktop Page 
//     0x09, 0x80,         //  (LOCAL)  USAGE              0x00010080 System Control (Application Collection)  
//     0xA1, 0x00,         //  (MAIN)   COLLECTION         0x00 Physical (Usage=0x00010080: Page=Generic Desktop Page, Usage=System Control, Type=Application Collection) <-- Warning: USAGE type should be CP (Physical Collection)
//     0x09, 0x85,         //    (LOCAL)  USAGE              0x00010085 System Main Menu (One Shot Control)  
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x01,         //    (GLOBAL) LOGICAL_MAXIMUM    0x01 (1)  
//     0x95, 0x01,         //    (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x75, 0x01,         //    (GLOBAL) REPORT_SIZE        0x01 (1) Number of bits per field  
//     0x81, 0x02,         //    (MAIN)   INPUT              0x00000002 (1 field x 1 bit) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0x15, 0x00,         //    (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x25, 0x00,         //    (GLOBAL) LOGICAL_MAXIMUM    0x00 (0)  <-- Info: Consider replacing 25 00 with 24
//     0x75, 0x07,         //    (GLOBAL) REPORT_SIZE        0x07 (7) Number of bits per field  
//     0x95, 0x01,         //    (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x81, 0x03,         //    (MAIN)   INPUT              0x00000003 (1 field x 7 bits) 1=Constant 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xC0,               //  (MAIN)   END_COLLECTION     Physical 
//     0x05, 0x06,         //  (GLOBAL) USAGE_PAGE         0x0006 Generic Device Controls Page 
//     0x09, 0x20,         //  (LOCAL)  USAGE              0x00060020 Battery Strength (Dynamic Value)  
//     0x15, 0x00,         //  (GLOBAL) LOGICAL_MINIMUM    0x00 (0) <-- Redundant: LOGICAL_MINIMUM is already 0 <-- Info: Consider replacing 15 00 with 14
//     0x26, 0xFF, 0x00,   //  (GLOBAL) LOGICAL_MAXIMUM    0x00FF (255)  
//     0x75, 0x08,         //  (GLOBAL) REPORT_SIZE        0x08 (8) Number of bits per field  
//     0x95, 0x01,         //  (GLOBAL) REPORT_COUNT       0x01 (1) Number of fields <-- Redundant: REPORT_COUNT is already 1 
//     0x81, 0x02,         //  (MAIN)   INPUT              0x00000002 (1 field x 8 bits) 0=Data 1=Variable 0=Absolute 0=NoWrap 0=Linear 0=PrefState 0=NoNull 0=NonVolatile 0=Bitmap 
//     0xC0, 
// };         //(MAIN)   END_COLLECTION     Application 



// // device descriptor
// struct usbg_gadget_attrs atributos = {

//     .idVendor = idVD,
//     .idProduct = idPD,
//     .bMaxPacketSize0 = 0x08,
//     .bDeviceSubClass = 0xFF,
//     .bDeviceProtocol = 0xFF,
//     .bDeviceClass = 0xFF,
//     .bcdUSB = 0x0200,
//     .bcdDevice = 0x0114, 

// };


// // controller strings
// struct usbg_gadget_strs gadget_strings = {

//     .manufacturer = "©Microsoft Corporation",
//     .product = "Controller",
//     .serial = "08FEC93"
// };


// //config descriptor
// struct usbg_config_strs config_str = {

//     .configuration = "1xHID"

// };

// struct usbg_f_hid_attrs hid_attrs = {

//     .protocol = 1,
//     .report_desc = {
//         .desc = report_des,
//         .len = sizeof(report_des)
//     },
//     .report_length = 8,
//     .subclass = 0x5D
// };
