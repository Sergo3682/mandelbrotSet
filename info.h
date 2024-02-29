#pragma pack(1)

struct bmp_info_header 
{
  uint32_t size_of_info_header;
  int32_t  width;
  int32_t  height;
  uint16_t  biPlanes  /* This value must be set to 1 */;
  uint16_t  biBitCount/* 24? */;
  uint32_t biCompression /* 0 */;
  uint32_t biSizeImage /* 0 */;
  int32_t  biXPelsPerMeter /* 0x0ec4 */;
  int32_t  biYPelsPerMeter /* 0x0ec4 */;
  uint32_t biClrUsed /* If this value is 0, the bitmap uses the maximum number of colors*/;
  uint32_t biClrImportant /*If this value is 0, all colors are required*/;
};
