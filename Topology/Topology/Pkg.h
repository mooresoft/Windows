#pragma once
#pragma pack(push, 1)

#define UWB_MAGIC 0x5857

typedef struct UWBPKG{
	unsigned short magic; // 0x5758
	unsigned char type; //0x1B
	unsigned char body_len;
	unsigned char body[1];
}UWBPKG;

typedef struct UWB_DEV_RANGE{
	unsigned short id1;
	unsigned short id2;
	int d;
}UWBDEVRANGE;

typedef struct UWB_TOF_EX{
	unsigned char item_size; // 8
	unsigned char item_num;
	UWBDEVRANGE dr[1];
}UWBTOFEX;

typedef struct UWBPKG40BODY{
	unsigned short seq;
	unsigned short id;
	short x;
	short y;
	short z;
	short a;
}UWBPKG40BODY;
#pragma pack(pop)