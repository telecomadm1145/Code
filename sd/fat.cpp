#include "base.h"
#include "classwiz_bsp.h"
#include "sd.h"
#include "fat.h"
FAT32_Parameters fat32_params;

// 读取MBR并返回第一个分区的起始扇区
unsigned long read_mbr() {
    MBR mbr;
    if (!sd_read_block(0, (byte*)&mbr)) {
        return 0;
    }
    
    // 验证MBR签名
    if (mbr.signature != 0xAA55) {
        return 0;
    }
    
    // 返回第一个分区的起始扇区
    return mbr.partitions[0].start_sector_lba;
}

// 初始化FAT32文件系统
byte init_fat32() {
    // 读取MBR获取分区起始扇区
    unsigned long partition_start = read_mbr();
    if (partition_start == 0) return 0;
    
    // 读取BPB
    byte buf[512];
    if (!sd_read_block(partition_start, buf)) {
        return 0;
    }
    FAT32_BPB& bpb = *(FAT32_BPB*)buf;
    
    // 验证文件系统类型
    if (bpb.bytes_per_sector != 512) return 0;
    
    // 计算重要参数
    fat32_params.first_fat_sector = partition_start + bpb.reserved_sectors;
    fat32_params.sectors_per_cluster = bpb.sectors_per_cluster;
    fat32_params.root_cluster = bpb.root_cluster;
    fat32_params.bytes_per_cluster = bpb.bytes_per_sector * bpb.sectors_per_cluster;
    fat32_params.first_data_sector = fat32_params.first_fat_sector + 
                                    (bpb.num_fats * bpb.fat_size_32);
    
    return 1;
}

// 获取FAT表项的值
unsigned long get_fat_entry(unsigned long cluster) {
    unsigned long fat_offset = cluster * 4;
    unsigned long fat_sector = fat32_params.first_fat_sector + (fat_offset / 512);
    unsigned int entry_offset = fat_offset % 512;
    
    byte sector_buffer[512];
    if (!sd_read_block(fat_sector, sector_buffer)) {
        return 0;
    }
    
    unsigned long* fat_table = (unsigned long*)sector_buffer;
    return fat_table[entry_offset/4] & 0x0FFFFFFF;
}

// 读取指定簇的数据
byte read_cluster(unsigned long cluster, byte* buffer) {
    unsigned long sector = fat32_params.first_data_sector + 
                          ((cluster - 2) * fat32_params.sectors_per_cluster);
    
    for (byte i = 0; i < fat32_params.sectors_per_cluster; i++) {
        if (!sd_read_block(sector + i, buffer + (i * 512))) {
            return 0;
        }
    }
    return 1;
}

// 读取目录内容
byte list_directory(unsigned long cluster) {
    byte cluster_buffer[512 * 8]; // 最大支持8个扇区/簇
    DIR_Entry __near* dir_entry;
    
    while (cluster >= 2 && cluster < 0x0FFFFFF8) {
        if (!read_cluster(cluster, cluster_buffer)) {
            return 0;
        }
        
        // 遍历目录项
        dir_entry = (DIR_Entry __near*)cluster_buffer;
        for (unsigned int i = 0; i < (fat32_params.bytes_per_cluster / sizeof(DIR_Entry)); i++) {
            // 检查是否是有效目录项
            if (dir_entry[i].name[0] == 0x00) continue; // 目录结束
            if (dir_entry[i].name[0] == (char)0xE5) continue; // 已删除的项
            if (dir_entry[i].attributes == 0x0F) continue; // 长文件名项
            
            // 处理文件名
            char filename[13];
            byte j;
            
            // 复制文件名
            for (j = 0; j < 8 && dir_entry[i].name[j] != ' '; j++) {
                filename[j] = dir_entry[i].name[j];
            }
            
            // 如果有扩展名，添加点和扩展名
            if (dir_entry[i].ext[0] != ' ') {
                filename[j++] = '.';
                for (byte k = 0; k < 3 && dir_entry[i].ext[k] != ' '; k++) {
                    filename[j++] = dir_entry[i].ext[k];
                }
            }
            filename[j] = '\0';
            
            // 这里你可以添加处理文件信息的代码
            // 比如打印文件名、大小等

            DebugOutputString(filename);
            DebugOutputString("\n");
            if (filename[0] == 'N'){
                byte cluster_buffer2[512 * 8];
                read_cluster((unsigned long)(dir_entry[i].first_cluster_high << 16) | (unsigned long)dir_entry[i].first_cluster_low,cluster_buffer2);
                DebugOutputString("Content:");
                DebugOutputString((char*)cluster_buffer2);
                DebugOutputString("\n");
            }
        }
        
        // 获取下一个簇
        cluster = get_fat_entry(cluster);
    }
    
    return 1;
}

// 在目录中查找文件
byte find_file(unsigned long dir_cluster, const char* filename, DIR_Entry * result) {
    byte cluster_buffer[512 * 8];
    DIR_Entry * dir_entry;
    char current_filename[13];
    
    while (dir_cluster >= 2 && dir_cluster < 0x0FFFFFF8) {
        if (!read_cluster(dir_cluster, cluster_buffer)) {
            return 0;
        }
        
        dir_entry = (DIR_Entry*)cluster_buffer;
        for (unsigned int i = 0; i < (fat32_params.bytes_per_cluster / sizeof(DIR_Entry)); i++) {
            if (dir_entry[i].name[0] == 0x00) break;
            if (dir_entry[i].name[0] == (char)0xE5) continue;
            if (dir_entry[i].attributes == 0x0F) continue;
            
            // 构建当前文件名
            byte j;
            for (j = 0; j < 8 && dir_entry[i].name[j] != ' '; j++) {
                current_filename[j] = dir_entry[i].name[j];
            }
            
            if (dir_entry[i].ext[0] != ' ') {
                current_filename[j++] = '.';
                for (byte k = 0; k < 3 && dir_entry[i].ext[k] != ' '; k++) {
                    current_filename[j++] = dir_entry[i].ext[k];
                }
            }
            current_filename[j] = '\0';
            
            // 比较文件名
            if (strcmp(current_filename, filename) == 0) {
                *result = dir_entry[i];
                return 1;
            }
        }
        
        dir_cluster = get_fat_entry(dir_cluster);
    }
    
    return 0;
}
// 打开文件
byte OpenFile(File& f,const char* filename) {
    DIR_Entry entry;
    // 在根目录查找文件
    if(!find_file(fat32_params.root_cluster, filename, &entry)) {
        return 0;
    }
    
    // 初始化文件结构
    f.start_cluster = (((unsigned long)entry.first_cluster_high) << 16) | 
                           entry.first_cluster_low;
    f.current_cluster = f.start_cluster;
   f.file_size = entry.file_size;
    f.current_position = 0;
    
    return 0;
}

// 读取文件数据
void ReadFile(unsigned long start_cluster, byte* buffer, unsigned int offset, unsigned int size) {
    if(!buffer) return;
    
    // 定位到指定偏移
    unsigned long target_cluster = start_cluster;
    unsigned int cluster_size = fat32_params.bytes_per_cluster;
    unsigned int clusters_to_skip = offset / cluster_size;
    
    // 跳过前面的簇
    for(unsigned int i = 0; i < clusters_to_skip; i++) {
        target_cluster = get_fat_entry(target_cluster);
        if(target_cluster >= 0x0FFFFFF8) return; // 到达文件末尾
    }
    
    // 计算簇内偏移
    unsigned int cluster_offset = offset % cluster_size;
    unsigned int bytes_remaining = size;
    unsigned int buffer_offset = 0;
    
    // 临时缓冲区用于读取整簇数据
    byte cluster_buffer[512 * 8]; // 假设最大簇大小为4KB
    
    while(bytes_remaining > 0) {
        // 读取当前簇
        if(!read_cluster(target_cluster, cluster_buffer)) {
            return;
        }
        
        // 计算本次要复制的字节数
        unsigned int bytes_to_copy = cluster_size - cluster_offset;
        if(bytes_to_copy > bytes_remaining) {
            bytes_to_copy = bytes_remaining;
        }
        
        // 复制数据
        memcpy(buffer + buffer_offset, 
               cluster_buffer + cluster_offset, 
               bytes_to_copy);
        
        bytes_remaining -= bytes_to_copy;
        buffer_offset += bytes_to_copy;
        cluster_offset = 0; // 后续簇从头开始读取
        
        // 获取下一簇
        if(bytes_remaining > 0) {
            target_cluster = get_fat_entry(target_cluster);
            if(target_cluster >= 0x0FFFFFF8) break; // 到达文件末尾
        }
    }
}

// 长文件名目录项结构
typedef struct {
    byte sequence_number;    // 序列号，最后一个项bit 6设为1
    uint16_t name1[5];      // 名称字符(1-5)
    byte attributes;        // 始终为0x0F
    byte type;             // 始终为0x00
    byte checksum;         // 短文件名的校验和
    uint16_t name2[6];      // 名称字符(6-11)
    uint16_t first_cluster; // 始终为0
    uint16_t name3[2];      // 名称字符(12-13)
} __attribute__((packed)) LFN_Entry;

// 用于存储完整的长文件名
typedef struct {
    uint16_t name[256];     // UCS2编码的文件名
    uint16_t length;        // 名称长度
} LFN_Name;

// 计算短文件名校验和
byte calculate_sfn_checksum(const byte* sfn) {
    byte sum = 0;
    for (int i = 0; i < 11; i++) {
        sum = ((sum & 1) << 7) + (sum >> 1) + sfn[i];
    }
    return sum;
}

// 从目录项读取长文件名
void read_lfn(DIR_Entry* entries, int entry_count, LFN_Name* lfn) {
    lfn->length = 0;
    
    // 检查是否有长文件名项
    if (entries[entry_count - 1].attributes != 0x0F) {
        // 没有长文件名，使用短文件名
        char sfn[13];
        int j = 0;
        
        // 复制主文件名
        for (int i = 0; i < 8 && entries[entry_count - 1].name[i] != ' '; i++) {
            sfn[j++] = entries[entry_count - 1].name[i];
        }
        
        // 添加扩展名
        if (entries[entry_count - 1].ext[0] != ' ') {
            sfn[j++] = '.';
            for (int i = 0; i < 3 && entries[entry_count - 1].ext[i] != ' '; i++) {
                sfn[j++] = entries[entry_count - 1].ext[i];
            }
        }
        sfn[j] = '\0';
        
        // 转换为UCS2
        for (int i = 0; i < j; i++) {
            lfn->name[i] = sfn[i];
        }
        lfn->length = j;
        return;
    }
    
    // 处理长文件名项
    for (int i = 0; i < entry_count - 1; i++) {
        LFN_Entry* lfn_entry = (LFN_Entry*)&entries[i];
        int seq = lfn_entry->sequence_number & 0x1F;
        int offset = (seq - 1) * 13;
        
        // 复制名称片段
        for (int j = 0; j < 5; j++) {
            if (lfn_entry->name1[j] == 0 || lfn_entry->name1[j] == 0xFFFF) break;
            lfn->name[offset + j] = lfn_entry->name1[j];
            lfn->length = offset + j + 1;
        }
        
        for (int j = 0; j < 6; j++) {
            if (lfn_entry->name2[j] == 0 || lfn_entry->name2[j] == 0xFFFF) break;
            lfn->name[offset + 5 + j] = lfn_entry->name2[j];
            lfn->length = offset + 5 + j + 1;
        }
        
        for (int j = 0; j < 2; j++) {
            if (lfn_entry->name3[j] == 0 || lfn_entry->name3[j] == 0xFFFF) break;
            lfn->name[offset + 11 + j] = lfn_entry->name3[j];
            lfn->length = offset + 11 + j + 1;
        }
    }
}