#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BYTESOFSECTOR   512

// 함수 선언
int AdjustInSectorSize(FILE *iFd, unsigned long long iSourceSize);
void WriteKernelInformation(FILE *iTargetFd, int iKernelSectorCount);
unsigned long long CopyFile(FILE *iSourceFd, FILE *iTargetFd);

// main 함수
int main(int argc, char *argv[]) {
    FILE *iSourceFd;
    FILE *iTargetFd;
    int iBootLoaderSize;
    int iKernel32SectorCount;
    unsigned long long iSourceSize;

    // 커맨드 라인 옵션 검사
    if(argc < 3) {
        fprintf(stderr, "[ERROR] ImageMaker BootLoader.bin Kernel32.bin\n");
        exit(-1);
    }

    // Disk.img 파일을 생성
    if((iTargetFd = fopen("Disk.img", "wb")) == NULL) {
        fprintf(stderr, "[ERROR] Disk.img open fail.\n");
        exit(-1);
    }

    //-----------------------------------------------------------------------
    // 부트 로더 파일을 열어서 모든 내용을 디스크 이미지 파일로 복사
    //-----------------------------------------------------------------------
    printf("[INFO] Copy boot loader to image file\n");
    if((iSourceFd = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "[ERROR] %s open fail.\n", argv[1]);
        exit(-1);
    }

    iSourceSize = CopyFile(iSourceFd, iTargetFd);
    fclose(iSourceFd);

    // 파일 크기를 섹터 크기인 512바이트로 맞추기 위해 나머지 부분을 0x00으로 채움
    iBootLoaderSize = AdjustInSectorSize(iTargetFd, iSourceSize);
    printf("[INFO] %s size = [%llu] and sector count = [%d]\n", argv[1], iSourceSize, iBootLoaderSize);

    //-----------------------------------------------------------------------
    // 32비트 커널 파일을 열어서 모든 내용을 디스크 이미지 파일로 복사
    //-----------------------------------------------------------------------
    printf("[INFO] Copy protected mode kernel to image file.\n");
    if((iSourceFd = fopen(argv[2], "rb")) == NULL) {
        fprintf(stderr, "[ERROR] %s open fail.\n", argv[2]);
        exit(-1);
    }

    iSourceSize = CopyFile(iSourceFd, iTargetFd);
    fclose(iSourceFd);

    // 파일 크기를 섹터 크기인 512바이트로 맞추기 위해 나머지 부분을 0x00으로 채움
    iKernel32SectorCount = AdjustInSectorSize(iTargetFd, iSourceSize);
    printf("[INFO] %s size = [%llu] and sector count = [%d]\n", argv[2], iSourceSize, iKernel32SectorCount);

    //-----------------------------------------------------------------------
    // 디스크 이미지에 커널 정보를 갱신
    //-----------------------------------------------------------------------
    printf("[INFO] Start to write kernel information\n");
    // 부트섹터의 5번째 바이트부터 커널에 대한 정보를 넣음
    WriteKernelInformation(iTargetFd, iKernel32SectorCount);
    printf("[INFO] Image file create complete\n");

    fclose(iTargetFd);

    return 0;
}

// 현재 위치부터 512바이트 배수 위치까지 맞추어 0x00으로 채움
int AdjustInSectorSize(FILE *iFd, unsigned long long iSourceSize) {
    int i;
    int iAdjustSizeToSector;
    char cCh;
    int iSectorCount;

    iAdjustSizeToSector = iSourceSize % BYTESOFSECTOR;
    cCh = 0x00;

    if(iAdjustSizeToSector != 0) {
        iAdjustSizeToSector = 512 - iAdjustSizeToSector;
        printf("[INFO] File size [%llu] and fill [%u] byte.\n", iSourceSize, iAdjustSizeToSector);
        for(i=0; i<iAdjustSizeToSector; i++) {
            fwrite(&cCh, sizeof(cCh), 1, iFd);
        }
    }else {
        printf("[INFO] File size is aligned 512 byte.\n");
    }

    // 섹터 수를 되돌려줌
    iSectorCount = (iSourceSize + iAdjustSizeToSector) / BYTESOFSECTOR;
    return iSectorCount;
}

// 부트 로더에 커널에 대한 정보를 삽입
void WriteKernelInformation(FILE *iTargetFd, int iKernelSectorCount) {
    unsigned short usData;
    long lPosition;

    // 파일의 시작에서 5바이트 떨어진 위치가 커널의 총 섹터 수 정보를 나타냄
    lPosition = fseek(iTargetFd, 5, SEEK_SET);
    if(lPosition == -1) {
        fprintf(stderr, "[ERROR] fseek fail. Return value = %ld, errno = %d, %d\n", lPosition, errno, SEEK_SET);
        exit(-1);
    }

    usData = (unsigned short)iKernelSectorCount;
    fwrite(&usData, sizeof(usData), 1, iTargetFd);

    printf("[INFO] Total sector count except boot loader [%d]\n", iKernelSectorCount);
}

// 소스 파일(Source FD)의 내용을 목표 파일(Target FD)에 복사하고 그 크기를 되돌려줌
unsigned long long CopyFile(FILE *iSourceFd, FILE *iTargetFd) {
    unsigned long long iSourceFileSize;
    int iRead;
    int iWrite;
    char vcBuffer[BYTESOFSECTOR];

    iSourceFileSize = 0;
    while(1) {
        iRead = fread(vcBuffer, 1, sizeof(vcBuffer), iSourceFd);
        iWrite = fwrite(vcBuffer, 1, iRead, iTargetFd);

        if(iRead != iWrite) {
            fprintf(stderr, "[ERROR] iRead != iWrite.. \n");
            exit(-1);
        }
        iSourceFileSize += iRead;

        if(iRead != sizeof(vcBuffer)) {
            break;
        }
    }
    return iSourceFileSize;
}