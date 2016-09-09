#include "image.h"
#include "platform.h"
#include "ff.h"

#define RAMDRV_BUFFER_O3DS ((u8*)0x22200000) // in O3DS FCRAM
#define RAMDRV_SIZE_O3DS (0x01C00000) // 28MB
#define RAMDRV_BUFFER_N3DS ((u8*)0x28000000) // in N3DS FCRAM
#define RAMDRV_SIZE_N3DS (0x08000000) // 128MB

static u8* ramdrv_buffer = NULL;
static u32 ramdrv_size = 0;

static FIL mount_file;
static u32 mount_state = 0;

int ReadImageSectors(u8* buffer, u32 sector, u32 count) {
    UINT bytes_read;
    UINT ret;
    if (!count) return -1;
    if (mount_state == IMG_RAMDRV) {
        if ((sector + count) * 0x200 > ramdrv_size) return -1;
        memcpy(buffer, ramdrv_buffer + (sector * 0x200), count * 0x200);
        return 0;
    }
    if (!mount_state) return FR_INVALID_OBJECT;
    if (f_tell(&mount_file) != sector * 0x200) {
        if (f_size(&mount_file) < sector * 0x200) return -1;
        f_lseek(&mount_file, sector * 0x200); 
    }
    ret = f_read(&mount_file, buffer, count * 0x200, &bytes_read);
    return (ret != 0) ? (int) ret : (bytes_read != count * 0x200) ? -1 : 0;
}

int WriteImageSectors(const u8* buffer, u32 sector, u32 count) {
    UINT bytes_written;
    UINT ret;
    if (!count) return -1;
    if (mount_state == IMG_RAMDRV) {
        if ((sector + count) * 0x200 > ramdrv_size) return -1;
        memcpy(ramdrv_buffer + (sector * 0x200), buffer, count * 0x200);
        return 0;
    }
    if (!mount_state) return FR_INVALID_OBJECT;
    if (f_tell(&mount_file) != sector * 0x200)
        f_lseek(&mount_file, sector * 0x200);
    ret = f_write(&mount_file, buffer, count * 0x200, &bytes_written);
    return (ret != 0) ? (int) ret : (bytes_written != count * 0x200) ? -1 : 0;
}

int SyncImage(void) {
    return (mount_state == IMG_RAMDRV) ? FR_OK :
        mount_state ? f_sync(&mount_file) : FR_INVALID_OBJECT;
}

u64 GetMountSize(void) {
    return (mount_state == IMG_RAMDRV) ? ramdrv_size :
        mount_state ? f_size(&mount_file) : 0;
}

u32 GetMountState(void) {
    return mount_state;
}

u32 IdentifyImage(const char* path) {
    u8 header[0x200];
    FIL file;
    if (f_open(&file, path, FA_READ | FA_OPEN_EXISTING) != FR_OK)
        return 0;
    f_lseek(&file, 0);
    f_sync(&file);
    UINT fsize = f_size(&file);
    UINT bytes_read;
    if ((f_read(&file, header, 0x200, &bytes_read) != FR_OK) || (bytes_read != 0x200)) {
        f_close(&file);
        return 0;
    }
    f_close(&file);
    if ((getbe32(header + 0x100) == 0x4E435344) && (getbe64(header + 0x110) == (u64) 0x0104030301000000) &&
        (getbe64(header + 0x108) == (u64) 0) && (fsize >= 0x8FC8000)) {
        return IMG_NAND;
    } else if (getbe16(header + 0x1FE) == 0x55AA) { // migt be FAT or MBR
        if ((strncmp((char*) header + 0x36, "FAT12   ", 8) == 0) || (strncmp((char*) header + 0x36, "FAT16   ", 8) == 0) ||
            (strncmp((char*) header + 0x36, "FAT     ", 8) == 0) || (strncmp((char*) header + 0x52, "FAT32   ", 8) == 0)) {
            return IMG_FAT; // this is an actual FAT header
        } else if (((getle32(header + 0x1BE + 0x8) + getle32(header + 0x1BE + 0xC)) < (fsize / 0x200)) && // check file size
            (getle32(header + 0x1BE + 0x8) > 0) && (getle32(header + 0x1BE + 0xC) >= 0x800) && // check first partition sanity
            ((header[0x1BE + 0x4] == 0x1) || (header[0x1BE + 0x4] == 0x4) || (header[0x1BE + 0x4] == 0x6) || // filesystem type
             (header[0x1BE + 0x4] == 0xB) || (header[0x1BE + 0x4] == 0xC) || (header[0x1BE + 0x4] == 0xE))) {
            return IMG_FAT; // this might be an MBR -> give it the benefit of doubt
        }
    }
    return 0;
}

u32 MountRamDrive(void) {
    if (mount_state && (mount_state != IMG_RAMDRV))
        f_close(&mount_file);
    if (GetUnitPlatform() == PLATFORM_3DS) {
        ramdrv_buffer = RAMDRV_BUFFER_O3DS;
        ramdrv_size = RAMDRV_SIZE_O3DS;
    } else {
        ramdrv_buffer = RAMDRV_BUFFER_N3DS;
        ramdrv_size = RAMDRV_SIZE_N3DS;
    }
    return (mount_state = IMG_RAMDRV);
}

u32 MountImage(const char* path) {
    if (mount_state) {
        if (mount_state != IMG_RAMDRV) f_close(&mount_file);
        mount_state = 0;
    }
    if (!path || !IdentifyImage(path)) return 0;
    if (f_open(&mount_file, path, FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
        return 0;
    f_lseek(&mount_file, 0);
    f_sync(&mount_file);
    return (mount_state = IdentifyImage(path));
}
