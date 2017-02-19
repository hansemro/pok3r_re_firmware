#ifndef USB_H
#define USB_H

#include "../ht32f165x.h"

// Request Data Direction
#define REQUEST_DIR_MASK    0x80
#define REQUEST_DIR_H2D     0x00
#define REQUEST_DIR_D2H     0x80

// Request Type
#define REQUEST_TYPE_MASK       0x60
#define REQUEST_TYPE_STANDARD   0x00
#define REQUEST_TYPE_CLASS      0x20
#define REQUEST_TYPE_VENDOR     0x40

// Request Recipient
#define REQUEST_RECIPIENT_MASK      0x1F
#define REQUEST_RECIPIENT_DEVICE    0x00
#define REQUEST_RECIPIENT_INTERFACE 0x01
#define REQUEST_RECIPIENT_ENDPOINT  0x02
#define REQUEST_RECIPIENT_OTHER     0x03

// Standard Requests
#define GET_STATUS          0
#define CLEAR_FEATURE       1
#define SET_FEATURE         3
#define SET_ADDRESS         5
#define GET_DESCRIPTOR      6
#define SET_DESCRIPTOR      7
#define GET_CONFIGURATION   8
#define SET_CONFIGURATION   9
#define GET_INTERFACE       10
#define SET_INTERFACE       11
#define SYNCH_FRAME         12

// Device Features
#define FEATURE_ENDPOINT_HALT           0
#define FEATURE_DEVICE_REMOTE_WAKEUP    1
#define FEATURE_TEST_MODE               2

#define OPT_SELF_POWERED    0x01
#define OPT_REMOTE_WAKEUP   0x02

typedef enum {
    H2D = 0,
    D2H = 1,
} Request_Direction;

typedef enum {
    STANDARD    = 0,
    CLASS       = 1,
    VENDOR      = 2,
} Request_Type;

typedef enum {
    DEVICE      = 0,
    INTERFACE   = 1,
    ENDPOINT    = 2,
    OTHER       = 3,
} Request_Recipient;

typedef enum {
    DATA_IN,
    DATA_OUT,
    STALL,
} Control_Action;

typedef struct {
    // Request Fields
    u8 bmRequestType;
    u8 bRequest;
    u16 wValue;
    u16 wIndex;
    u16 wLength;

    Request_Direction direction;
    Request_Type type;
    Request_Recipient recipient;

    // Control Buffer
    Control_Action action;
    u8 controlLength;
    const u8 *controlData;
    u8 controlBuffer[64];
} USB_Request;

typedef struct {
    const u8 *desc;
    u8 size;
} USB_Descriptor;

typedef struct {
    // Device descriptor
    const USB_Descriptor *device;

    // Configuration descriptors
    const USB_Descriptor *config;
    u8 numConfig;

    // String descriptors
    const USB_Descriptor *string;
    u8 numString;
} USB_Descriptors;

typedef enum {
    ATTACHED = 0,   //!< Attached.
    POWERED,        //!< Attached, powered, not reset.
    DEFAULT,        //!< Attached, powered, reset, no address.
    ADDRESS,        //!< Attached, powered, reset, address, not configured.
    CONFIGURED,     //!< Attached, powered, reset, address, configured. Ready for use.
    SUSPENDED,      //!< Attached, powered, suspended.
} USB_Status;

typedef struct {
    u8 enable;
    u16 length;
    volatile u8 *buffer;

    // endpoint config backup
    u32 cfgr;
    u32 ier;
} USB_Endpoint;

typedef void (*usb_suspend_func)(void);

typedef struct {
    USB_Status currStatus;
    USB_Status prevStatus;

    u8 deviceFeature;
    u8 bConfigurationValue;

    USBIER_reg ier;

    USB_Endpoint ep[8];
    USB_Descriptors descriptors;

    usb_suspend_func suspend_callback;
} USB_Device;

// API
void usb_init();
void usb_pull_up(char en);

void usb_callback_suspend(usb_suspend_func call);

#endif // USB_H
