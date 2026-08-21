#pragma once

bool CheckIsHNH();
void InitHNH();
void FinishHNH();
void DrawHNH();

LPVOID GetTargetEntryPoint();

void InitializePythonInterpreter();
void DrawPythonREPL();
void DrawJupyterNotebookREPL();
