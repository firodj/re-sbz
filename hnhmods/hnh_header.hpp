#pragma once

bool CheckIsHNH();
void InitHNH();
void FinishHNH();
bool DrawHNH(const char *window_name);

LPVOID GetTargetEntryPoint();

void InitializePythonInterpreter();
void DrawPythonREPL();
void DrawJupyterNotebookREPL();
bool IsGameInputOn();
