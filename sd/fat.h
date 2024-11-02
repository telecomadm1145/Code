#include "base.h"
#include "classwiz_bsp.h"
byte init_fat32();

// MBR结构定义
typedef struct {
    byte bootstrap[446];
    struct {
        byte status;
        byte start_head;
        byte start_sector;
        byte start_cylinder;
        byte type;
        byte end_head;
        byte end_sector;
        byte end_cylinder;
        unsigned long start_sector_lba;
        unsigned long total_sectors;
    } partitions[4];
    unsigned short signature;
} MBR;

// FAT32 BPB结构定义
typedef struct {
    byte jump_boot[3];
    char oem_name[8];
    unsigned short bytes_per_sector;
    byte sectors_per_cluster;
    unsigned short reserved_sectors;
    byte num_fats;
    unsigned short root_entries;
    unsigned short total_sectors_16;
    byte media;
    unsigned short fat_size_16;
    unsigned short sectors_per_track;
    unsigned short number_of_heads;
    unsigned long hidden_sectors;
    unsigned long total_sectors_32;
    unsigned long fat_size_32;
    unsigned short flags;
    unsigned short version;
    unsigned long root_cluster;
    unsigned short fsinfo_sector;
    unsigned short backup_boot_sector;
    byte reserved[12];
    byte drive_number;
    byte reserved1;
    byte boot_signature;
    unsigned long volume_id;
    char volume_label[11];
    char fs_type[8];
} __attribute__((packed)) FAT32_BPB;

// FAT32文件系统参数
typedef struct {
    unsigned long first_data_sector;
    unsigned long first_fat_sector;
    unsigned long sectors_per_cluster;
    unsigned long root_cluster;
    unsigned long bytes_per_cluster;
} FAT32_Parameters;

typedef struct {
    char name[8];
    char ext[3];
    byte attributes;
    byte reserved;
    byte create_time_tenth;
    unsigned short create_time;
    unsigned short create_date;
    unsigned short access_date;
    unsigned short first_cluster_high;
    unsigned short write_time;
    unsigned short write_date;
    unsigned short first_cluster_low;
    unsigned long file_size;
} __attribute__((packed)) DIR_Entry;
extern FAT32_Parameters fat32_params;
// 文件结构定义
typedef struct {
    unsigned long start_cluster;    // 文件起始簇号
    unsigned long current_cluster;  // 当前簇号
    unsigned long file_size;        // 文件大小
    unsigned long current_position; // 当前读取位置
} File;

byte list_directory(unsigned long cluster);
byte find_file(unsigned long dir_cluster, const char* filename, DIR_Entry * result);
byte OpenFile(File& f,const char* filename) ;
void ReadFile(File& file, byte* buffer, unsigned int offset, unsigned int size);