#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <3ds.h>
#include <string.h>
#include <pestmenu/service.h>
#endif

Handle aptEvents[3];
LightEvent aptSleepEvent;

Handle *nssHandle = NULL;
Handle *ptmsysmHandle = NULL;
Handle *psHandle = NULL;
Handle *amHandle = NULL;
Handle *cfgHandle = NULL;
Handle aptLockHandle;

u32 gspProcess;

void __appInit() {
  startServices();
}

int main(int argc, char **argv) {

    printf("PestMenu v0.0.1\n");

    while (aptMainLoop()) {
      hidScanInput();

      if (hidKeysDown() & KEY_A) {
        //FS_ProgramInfo inf = {0x000400000FF40002, MEDIATYPE_NAND};
        u32 prep_start_app_cmdbuf[16];
        prep_start_app_cmdbuf[0] = IPC_MakeHeader(0x15,5,0); // 0x150140; APT_PrepareToStartApplication
        prep_start_app_cmdbuf[1] = 0x00040000;
        prep_start_app_cmdbuf[2] = 0x0FF40002;
        prep_start_app_cmdbuf[3] = MEDIATYPE_NAND;
        prep_start_app_cmdbuf[4] = 0x00000000;
        prep_start_app_cmdbuf[5] = 0;
        u32 ret = 0xDEADBABE;
        ret = aptSendCommand(prep_start_app_cmdbuf);
        printf("APT_PrepareToStartApplication: %08X\n", ret);

        GSPGPU_ReleaseRight();

        u32 start_app_cmdbuf[16];
        start_app_cmdbuf[0] = IPC_MakeHeader(0x1B,3,4); // 0x1B00C4; APT_StartApplication
        start_app_cmdbuf[1] = 0x300;
        start_app_cmdbuf[2] = 0x20;
        start_app_cmdbuf[3] = 0;
        start_app_cmdbuf[4] = (0x300 << 14) | 2;
        start_app_cmdbuf[5] = 0;
        start_app_cmdbuf[6] = (0x20 << 14) | 0x802;
        start_app_cmdbuf[7] = 0;
        u32 ret2 = 0xDEADBEEF;
        ret2 = aptSendCommand(start_app_cmdbuf);
        printf("APT_StartApplication: %08X\n", ret2);
      }

      gfxFlushBuffers();
      gfxSwapBuffers();
      gspWaitForVBlank();
    }
}
