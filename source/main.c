#include <stdio.h>
#include <3ds.h>
#include <string.h>

Handle *signalEvent = NULL;
Handle *resumeEvent = NULL;

Handle *nssHandle = NULL;

aptMessageCb callback = {NULL, APPID_HOMEMENU, "boop", sizeof("boop")};

void clearCmdBuf() {
  memset(getThreadCommandBuffer(), 0, sizeof(getThreadCommandBuffer()));
}

void __appInit() {
  srvInit();
  hidInit();

  fsInit();
  sdmcInit();

  gfxInitDefault();
  consoleInit(GFX_TOP, NULL);
}

int main(int argc, char **argv) {

    printf("PestMenu v0.0.1\n");
    printf("APT_Initialize: %08X\n", APT_Initialize(APPID_HOMEMENU, aptMakeAppletAttr(APTPOS_SYS, true, true), signalEvent, resumeEvent));

    while (aptMainLoop()) {
      hidScanInput();
      //if (hidKeysDown() & KEY_START) break;

      if (hidKeysDown() & KEY_A) {

        //u32 *prep_leave_hm_cmdbuf = getThreadCommandBuffer();
        //prep_leave_hm_cmdbuf[0] = IPC_MakeHeader(0x2D,0,0); // 0x2D0000, APT_PrepareToLeaveHomeMenu
        //ret = aptSendCommand(prep_leave_hm_cmdbuf);
        //printf("APT_PrepareToLeaveHomeMenu: %08X\n", ret);
        //clearCmdBuf();

        //u32 *leave_hm_cmdbuf = getThreadCommandBuffer();
        //leave_hm_cmdbuf[0] = IPC_MakeHeader(0x2E,1,4); // 0x2E0044, APT_LeaveHomeMenu
        //leave_hm_cmdbuf[1] = sizeof(hmxParam);
        //leave_hm_cmdbuf[2] = 0;
        //leave_hm_cmdbuf[3] = aptHandle;
        //leave_hm_cmdbuf[4] = (sizeof(hmxParam) << 14) | 2;
        //leave_hm_cmdbuf[5] = hmxParam;
        //ret = aptSendCommand(leave_hm_cmdbuf);
        //printf("APT_LeaveHomeMenu: %08X\n", ret);

        FS_ProgramInfo inf = {0x000400000FF40002, MEDIATYPE_NAND};
        u32 prep_start_app_cmdbuf[16];
        prep_start_app_cmdbuf[0] = IPC_MakeHeader(0x15,5,0); // 0x150140; APT_PrepareToStartApplication
        prep_start_app_cmdbuf[1] = 0x0FF40002;
        prep_start_app_cmdbuf[2] = 0x00040000;
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