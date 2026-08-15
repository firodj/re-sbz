#pragma once

bool CheckIsSBZ();
void InitSBZ();
void FinishSBZ();
LPVOID GetTargetEntryPoint();

void InitializePythonInterpreter();
void DrawPythonREPL();
void DrawJupyterNotebookREPL();
void DrawSBZ();