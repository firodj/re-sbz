# HaNaHiMe

0x49F0E1 | start / Entry Point
0x485CE0 | WinMain
0x485FC2 | WndProcMenuCheckSystem

  0xBD0  | WM Japan Locale
  0x2B10 | WM Disk Correct

  It check by sequentially `PostMessage` and will auto exit the `PeekMessage` when Done.

  0x4863C8 | CheckRunGame
  0x427DA8 | ShowTitleMenu
  0x428927 | ShowTitleMenuWnd
  0x428C11 | WndProcTitleMenu

  0x4B3488 | DWORD g_bRunGame 
  
0x47F013 | GameMainLoop

0x4799E2 | InGameMenu

0x401039 | ParseSekFile
0x441F19 | ParseODFFile

0x477F90 | SceneBeginning
0x45BCAE | Scene2_45BCAE
0x44C38D | Scene3
0x4703AE | Scene4



0x481637 | ThreadProc
0x4817B1 | ThreadProc2