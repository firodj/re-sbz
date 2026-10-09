#pragma once

bool CheckIsHNH();
void InitHNH();
void FinishHNH();
void DrawHNH(const char *window_name);

LPVOID GetTargetEntryPoint();

void InitializePythonInterpreter();
void DrawPythonREPL();
void DrawJupyterNotebookREPL();
