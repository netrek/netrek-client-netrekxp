#ifdef VSHIELD_BITMAPS
#define shield_width 20
#define shield_height 20
#define SHIELD_FRAMES   5
#define SHIELD_BYTE_SIZE 60

static char shield_bits[SHIELD_FRAMES][SHIELD_BYTE_SIZE] = {
    {
     0x40, 0x29, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02,
     0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08,
     0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00,
     0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00,
     0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x40, 0x29, 0x00},
    {
     0xc0, 0x36, 0x00, 0x20, 0x40, 0x00, 0x08, 0x00, 0x01, 0x04, 0x00, 0x02,
     0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
     0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00,
     0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00,
     0x04, 0x00, 0x02, 0x08, 0x00, 0x01, 0x20, 0x40, 0x00, 0xc0, 0x36, 0x00},
    {
     0xc0, 0x3f, 0x00, 0x30, 0xc0, 0x00, 0x08, 0x00, 0x01, 0x04, 0x00, 0x02,
     0x02, 0x00, 0x04, 0x02, 0x00, 0x04, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x02, 0x00, 0x04, 0x02, 0x00, 0x04,
     0x04, 0x00, 0x02, 0x08, 0x00, 0x01, 0x30, 0xc0, 0x00, 0xc0, 0x3f, 0x00},
    {
     0xc0, 0x3f, 0x00, 0x70, 0xe0, 0x00, 0x18, 0x80, 0x01, 0x0c, 0x00, 0x03,
     0x06, 0x00, 0x06, 0x02, 0x00, 0x04, 0x03, 0x00, 0x0c, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x03, 0x00, 0x0c, 0x02, 0x00, 0x04, 0x06, 0x00, 0x06,
     0x0c, 0x00, 0x03, 0x18, 0x80, 0x01, 0x70, 0xe0, 0x00, 0xc0, 0x3f, 0x00},
    {
     0xc0, 0x3f, 0x00, 0x70, 0xe0, 0x00, 0x18, 0x80, 0x01, 0x0c, 0x00, 0x03,
     0x06, 0x00, 0x06, 0x02, 0x00, 0x04, 0x03, 0x00, 0x0c, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
     0x01, 0x00, 0x08, 0x03, 0x00, 0x0c, 0x02, 0x00, 0x04, 0x06, 0x00, 0x06,
     0x0c, 0x00, 0x03, 0x18, 0x80, 0x01, 0x70, 0xe0, 0x00, 0xc0, 0x3f, 0x00},
};

#else
static char shield_bits[] = {
    0xc0, 0x3f, 0x00, 0x30, 0xc0, 0x00, 0x08, 0x00, 0x01, 0x04, 0x00, 0x02,
    0x02, 0x00, 0x04, 0x02, 0x00, 0x04, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
    0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x01, 0x00, 0x08,
    0x01, 0x00, 0x08, 0x01, 0x00, 0x08, 0x02, 0x00, 0x04, 0x02, 0x00, 0x04,
    0x04, 0x00, 0x02, 0x08, 0x00, 0x01, 0x30, 0xc0, 0x00, 0xc0, 0x3f, 0x00
};

#endif


static char cloak_bits[] = {
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0x08, 0x82, 0x00,
    0x04, 0x00, 0x01, 0x20, 0x20, 0x00, 0x00, 0x02, 0x00, 0x80, 0x08, 0x00,
    0x01, 0x00, 0x04, 0x49, 0x90, 0x04, 0x01, 0x00, 0x04, 0x80, 0x08, 0x00,
    0x00, 0x02, 0x00, 0x20, 0x20, 0x00, 0x04, 0x00, 0x01, 0x08, 0x82, 0x00,
    0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00
};


static char icon_bits[] = {
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x73, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0x00, 0xf8, 0x01, 0x26, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xc1, 0x1f, 0x7c, 0x20, 0x71, 0xaa, 0xab, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x81, 0x0f, 0x38, 0x20, 0x17, 0xb2, 0x10, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x0f, 0x18, 0x20, 0x75, 0xa2, 0x2b, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x1e, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0x81, 0x01, 0x1e, 0x0c, 0x00,
    0xe0, 0x01, 0x00, 0x88, 0x04, 0x80, 0xff, 0xff, 0xff, 0x81, 0x01, 0x3c,
    0x06, 0x00, 0x20, 0xdc, 0xdc, 0xd5, 0x71, 0x83, 0xe0, 0xc3, 0x07, 0x81,
    0x01, 0x3c, 0x03, 0x00, 0x20, 0x45, 0x45, 0x9c, 0x54, 0x05, 0xc0, 0x81,
    0x03, 0x80, 0x01, 0x78, 0x03, 0x00, 0xe0, 0x5d, 0x5d, 0x94, 0x75, 0x05,
    0xc0, 0x81, 0x03, 0x80, 0x01, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0xf0, 0x00, 0xfc,
    0x8f, 0x7f, 0x80, 0x8f, 0xe1, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0xe0,
    0x01, 0xfe, 0x87, 0xff, 0xe0, 0x87, 0x71, 0x00, 0xc0, 0x81, 0x03, 0x80,
    0x01, 0xe0, 0x01, 0x60, 0x80, 0xe1, 0x70, 0x80, 0x39, 0x00, 0xc0, 0x81,
    0x03, 0x80, 0x01, 0xf0, 0x03, 0x60, 0x80, 0x71, 0x38, 0x80, 0x1d, 0x00,
    0xc0, 0x81, 0x03, 0x80, 0x01, 0xd8, 0x03, 0x60, 0x80, 0x39, 0x18, 0x80,
    0x0f, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0x98, 0x07, 0x60, 0x80, 0x1d,
    0xf8, 0x87, 0x3f, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0x8c, 0x07, 0x60,
    0x80, 0x7f, 0xf8, 0x83, 0x7f, 0x00, 0xc0, 0x81, 0x03, 0x80, 0x01, 0x06,
    0x0f, 0x60, 0x80, 0xe1, 0x18, 0x80, 0xe1, 0x00, 0xc0, 0x81, 0x03, 0x80,
    0x01, 0x06, 0x0f, 0x60, 0x80, 0xc1, 0x38, 0x80, 0xc1, 0x00, 0xc0, 0x81,
    0x03, 0x80, 0x01, 0x03, 0x1f, 0x60, 0x80, 0xc1, 0x70, 0x88, 0xc1, 0x80,
    0xe0, 0xc3, 0x07, 0x81, 0x81, 0x03, 0x3e, 0x60, 0x80, 0xc1, 0xe0, 0x8f,
    0xc1, 0x87, 0xff, 0xff, 0xff, 0x81, 0xe1, 0x07, 0xff, 0xf0, 0xc0, 0xe3,
    0x80, 0xc7, 0xe3, 0x83, 0xff, 0xff, 0xff, 0x81, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0xff, 0x3f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xfc, 0xff, 0xff,
    0x07, 0x00, 0xc0, 0xff, 0x03, 0xfe, 0xff, 0xff, 0x07, 0x80, 0x01, 0xfe,
    0xff, 0xff, 0x0f, 0x00, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x01, 0x80,
    0x01, 0x40, 0x00, 0xc0, 0x80, 0x00, 0xff, 0xff, 0xff, 0x00, 0x06, 0x00,
    0x00, 0x80, 0x81, 0x80, 0xff, 0xbf, 0xff, 0x00, 0x00, 0xff, 0xf8, 0xff,
    0xff, 0x03, 0x00, 0x80, 0x81, 0x7f, 0xff, 0x1f, 0x80, 0x00, 0x00, 0x18,
    0xf0, 0xff, 0xff, 0x03, 0x00, 0x80, 0x81, 0x04, 0xff, 0x1f, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0x01, 0x04, 0xfd, 0x17,
    0xee, 0x01, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80,
    0xf9, 0x13, 0x78, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x80,
    0x01, 0x00, 0xf1, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x01, 0x00, 0xfd, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xf9, 0x13, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xf9, 0x13, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xf0, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0xf0, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x80,
    0x01, 0x00, 0xf8, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x2e,
    0x00, 0x80, 0x01, 0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x5f, 0x00, 0x80, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xfc, 0xff, 0x07, 0x80, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xe7, 0xee, 0x3c, 0x80, 0x01, 0x00, 0x41, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x80, 0x31, 0xc0, 0x80, 0x01, 0x00,
    0x47, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x60, 0xce, 0x00, 0x81,
    0x01, 0x00, 0xf9, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x81, 0x01, 0x00, 0xe4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xe0, 0x00, 0x00, 0x80, 0x7f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xe0, 0x00, 0x00, 0xc0,
    0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xe0, 0x00,
    0x00, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0xf0, 0x01, 0x00, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x01, 0x00, 0xe0, 0x00, 0x00, 0x7c, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x01, 0x00, 0x40, 0x00, 0x00, 0xfa, 0xff, 0xff, 0xfe, 0x7f,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xe0, 0x00, 0x00, 0x84, 0x0f, 0x7d,
    0xff, 0xff, 0x07, 0x00, 0x00, 0x80, 0x01, 0x00, 0x40, 0x00, 0xf0, 0xff,
    0xff, 0xfb, 0xff, 0xff, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
    0xf0, 0xff, 0xff, 0x3f, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
    0x00, 0x00, 0x00, 0xfe, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f
};
