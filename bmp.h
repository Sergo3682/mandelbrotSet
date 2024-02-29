#pragma pack(1)

struct bmp_file_header
{
	uint16_t bmp_type;
	uint32_t size;
	uint16_t reserved[2];
	uint32_t image_data_offset;
};
