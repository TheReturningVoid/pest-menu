#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <3ds.h>
#include <string.h>
#endif

Handle aptEvents[3];
LightEvent aptSleepEvent;

Handle aptLockHandle;

u32 gspProcess;
u32 codecProcess;
u32 hidProcess;
u32 dspProcess;
u32 amProcess;

Result startAPT() {
  aptInit();
  Result ret = 0;

  // Get lock handle
  ret = APT_GetLockHandle(0, &aptLockHandle);
  if (R_FAILED(ret)) return ret;

  // Initialize APT
  APT_AppletAttr attr = aptMakeAppletAttr(APTPOS_SYS, 0, 0);
  ret = APT_Initialize(envGetAptAppId(), attr, &aptEvents[1], &aptEvents[2]);
  if (R_FAILED(ret)) {
    svcCloseHandle(aptLockHandle);
    return ret;
  }

  // Enable APT
  ret = APT_Enable(attr);
  if (R_FAILED(ret)) {
    svcCloseHandle(aptEvents[1]);
    svcCloseHandle(aptEvents[2]);
    svcCloseHandle(aptLockHandle);
    return ret;
  }

  // Create APT close event
  ret = svcCreateEvent(&aptEvents[0], RESET_STICKY);
  if (R_FAILED(ret)) {
    svcCloseHandle(aptEvents[1]);
    svcCloseHandle(aptEvents[2]);
    svcCloseHandle(aptLockHandle);
    return ret;
  }

  // Create APT sleep event
  LightEvent_Init(&aptSleepEvent, RESET_ONESHOT);

  return ret;
}


void stopServices() {
  amExit();
  dspExit();
  hidExit();
  gspExit();
  psExit();
  fsExit();
  ptmSysmExit();
  nsExit();
  srvExit();
}

void startServices() {
  if (srvInit() >> 31) svcBreak(0);
  if (nsInit() >> 31) svcBreak(0);
  if (ptmSysmInit() >> 31) svcBreak(0);
  fsInit();
  cfguInit();
  if (psInit() >> 31) svcBreak(0);
  startAPT();
  if (NS_LaunchTitle(0x0004013000001C02, 0, &gspProcess) == 0xCBA12402) {
    stopServices();
    return;
  }
  gspInit();
  if (NS_LaunchTitle(0x0004013000001802, 0, &codecProcess) == 0xCBA12402) {
    stopServices();
    return;
  }
  if (NS_LaunchTitle(0x0004013000001C02, 0, &hidProcess) == 0xCBA12402) {
    stopServices();
    return;
  }
  hidInit();
  if (NS_LaunchTitle(0x0004013000001C02, 0, &dspProcess) == 0xCBA12402) {
    stopServices();
    return;
  }
  dspInit();
  if (NS_LaunchTitle(0x0004013000001502, 0, &amProcess) == 0xCBA12402) {
    stopServices();
    return;
  }
  amInit();
}
