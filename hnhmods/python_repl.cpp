#include "pch.h"

#include <string>
#include <vector>
#include "imgui.h"
#include "console.hpp"

#include <pybind11/embed.h>
namespace py = pybind11;

enum class CellType { Code, Markdown };
struct NotebookCell {
    size_t id;                       // Unique tracking ID for ImGui ID stacks
    CellType type = CellType::Code;
    std::string inputBuffer = "";    // The text edit area for this block
    std::vector<std::string> outputs;// Execution history specific to this block
    int executionCount = 0;          // e.g., In [1]: or In [2]:
    bool isPendingClear = false;
};

// Global notebook collection state
std::vector<NotebookCell> NotebookCells;
size_t NextCellID = 0;
int GlobalExecutionCounter = 1;

// Global log history for ImGui to read
std::vector<std::string> LogHistory;      // Console output lines
std::vector<std::string> CommandHistory;  // Executed commands only
int HistoryPos = -1;                      // -1 means we are on a fresh input line

// Helper to append a new empty block to our notebook
void AddNotebookCell(size_t index) {
    NotebookCell newCell;
    newCell.id = NextCellID++;
    if (index >= NotebookCells.size()) {
        NotebookCells.push_back(newCell);
    }
    else {
        NotebookCells.insert(NotebookCells.begin() + index + 1, newCell);
    }
}

void ExecuteCell(NotebookCell& cell) {
    if (cell.inputBuffer.empty()) return;

    py::dict globalScope = py::globals();

    cell.outputs.clear();
    cell.executionCount = GlobalExecutionCounter++;

    try {
        py::object result;
        bool isExpression = true;

        try {
            // Check if it's a pure expression (like 5 + 5 or player.name)
            result = py::eval<py::eval_expr>(cell.inputBuffer, globalScope);
        }
        catch (py::error_already_set&) {
            PyErr_Clear();
            isExpression = false;
            // Fallback to statement code block execution (loops, defs, imports)
            py::exec(cell.inputBuffer, globalScope);
        }

        if (isExpression && !result.is_none()) {
            globalScope["_"] = result;
            std::string repr_str = py::repr(result).cast<std::string>();
            cell.outputs.push_back(repr_str);
        }
    }
    catch (py::error_already_set& e) {
        // Append traceback error streams straight onto this block's local layout
        cell.outputs.push_back("Error: " + std::string(e.what()));
    }
}

// Simple C++ class to capture Python's print calls
class ImGuiStdoutRedirector {
public:
    void write(const std::string& text) {
        // Avoid adding empty spam lines, but catch actual data
        if (text != "\n" || (!LogHistory.empty() && LogHistory.back() != "\n")) {
            LogHistory.push_back(text);
        }
    }
    void flush() { /* Python files require a flush method, even if empty */ }
};

// Create the pybind11 binding module
PYBIND11_EMBEDDED_MODULE(cpp_console, m) {
    py::class_<ImGuiStdoutRedirector>(m, "Redirector")
        .def(py::init<>())
        .def("write", &ImGuiStdoutRedirector::write)
        .def("flush", &ImGuiStdoutRedirector::flush);
}

std::string get_current_dir() {
    // Import Python's 'os' module
    py::module_ os = py::module_::import("os");

    // Call os.getcwd() and cast the result to std::string
    return os.attr("getcwd")().cast<std::string>();
}

void InitializePythonInterpreter() {
    try {
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("append")("."); // Add current working directory

        std::string cwd = get_current_dir();
        DebugLog("[Python] python cwd = %s\n", cwd.c_str());

        //py::print("anything else");
        //py::eval("a = 2");
    
        // 2. Fetch the global execution scope dictionary
        py::dict globalScope = py::globals();

        // 3. Inject our redirection logic using clean Python syntax
        py::exec(R"(
            import sys
            import cpp_console
            import ctypes
        
            redirector = cpp_console.Redirector()
            sys.stdout = redirector
            sys.stderr = redirector

            def read_mem(address: int, size: int) -> bytes:
                """Reads 'size' bytes starting from a raw memory address."""
                return ctypes.string_at(address, size)

            def write_mem(address: int, data: bytes):
                """Writes raw bytes directly to a memory address."""
                # Convert data to bytes if passed as something else
                src = bytes(data) 
                # Use standard standard-library C memmove to copy the raw payload
                ctypes.memmove(address, src, len(src))
        )", globalScope);

        LogHistory.push_back("Python REPL Initialized (via pybind11).\n");
        LogHistory.push_back(sys.attr("version").cast<std::string>());
    }
    catch (py::error_already_set& e) {
        // Access the Python error message
        DebugLog("[Python] Python error: %s\n", e.what());
    }
    catch (const std::exception& e) {
        // Catch by reference to avoid slicing
        DebugLog("[Python] Caught: %s\n", e.what());
    }
    catch (...) {
        // Ellipsis catches any exception not caught by previous blocks
        DebugLog("[Python] Uknown exception %s:%d\n", __FILE__, __LINE__);
    }

}

// --- Tab Auto-Complete Engine ---
std::vector<std::string> FetchAutocompleteCandidates(const std::string& currentInput) {
    std::vector<std::string> candidates;
    try {
        // Dynamically invoke Python's standard library reflection engine
        py::dict globalScope = py::globals();
        py::dict localDict;
        localDict["text"] = currentInput;
        localDict["globals_scope"] = globalScope;

        // Initialize rlcompleter tied specifically to our game scope instance
        py::exec(R"(
            import rlcompleter
            completer = rlcompleter.Completer(globals_scope)
            
            candidates = []
            state = 0
            while True:
                res = completer.complete(text, state)
                if res is None:
                    break
                candidates.append(res)
                state += 1
        )", globalScope, localDict);

        // Extract result array back to C++
        py::list py_candidates = localDict["candidates"].cast<py::list>();
        for (auto item : py_candidates) {
            candidates.push_back(item.cast<std::string>());
        }
    }
    catch (py::error_already_set&) {
        PyErr_Clear(); // Silence background lookup exceptions gracefully
    }
    return candidates;
}

// Standard ImGui text input callback wrapper
int REPLTextCallback(ImGuiInputTextCallbackData* data) {
    // 1. Handle Tab Key Completion
    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
        std::string currentInput(data->Buf, data->BufTextLen);
        std::vector<std::string> matches = FetchAutocompleteCandidates(currentInput);

        if (!matches.empty()) {
            // Single Match found -> Autocomplete the line instantly
            if (matches.size() == 1) {
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, matches[0].c_str());
            }
            // Multiple Matches -> Display options in log space like a standard Unix bash layout
            else {
                LogHistory.push_back("\nCandidates:\n");
                for (const auto& match : matches) {
                    LogHistory.push_back("  " + match + "\n");
                }
            }
        }
    }

    // 2. Handle Up/Down Arrow History Tracking
    else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
        int prev_history_pos = HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow) {
            if (HistoryPos == -1) HistoryPos = (int)CommandHistory.size() - 1;
            else if (HistoryPos > 0) HistoryPos--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow) {
            if (HistoryPos != -1) {
                if (++HistoryPos >= (int)CommandHistory.size()) HistoryPos = -1;
            }
        }
        if (prev_history_pos != HistoryPos) {
            const char* history_str = (HistoryPos >= 0) ? CommandHistory[HistoryPos].c_str() : "";
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, history_str);
        }
    }
    return 0;
}

void ExecuteREPLCommand(const std::string& command) {
    py::dict globalScope = py::globals();

    // 1. Echo the prompt line onto the logs
    LogHistory.push_back(">>> " + command + "\n");

    try {
        py::object result;
        bool isExpression = true;

        try {
            // First, try to evaluate the input line strictly as an expression (e.g. "2 + 2" or "player.hp")
            result = py::eval<py::eval_expr>(command, globalScope);
        }
        catch (py::error_already_set&) {
            // If it failed because it contains statements (like a variable assignment or loops),
            // clear the active Python exception state and treat it as raw execution code block
            PyErr_Clear();
            isExpression = false;
            py::exec(command, globalScope);
        }

        // 2. Format result values for Expressions
        if (isExpression) {
            // Check if the result is not None before processing
            if (!result.is_none()) {
                // Update the global underscore '_' storage state inside our scope
                globalScope["_"] = result;

                // Extract its internal string representation (like Python's built-in repr())
                std::string repr_str = py::repr(result).cast<std::string>();
                LogHistory.push_back(repr_str + "\n");
            }
        }
    }
    catch (py::error_already_set& e) {
        // Log standard runtime or syntax exceptions safely
        LogHistory.push_back(std::string(e.what()) + "\n");
    }
}

void DrawPythonREPL() {
    static char inputBuffer[256] = "";
    static bool scrollToBottom = false;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Python REPL")) {
        ImGui::End();
        return;
    }

    // --- Output History Region ---
    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), true);

    for (const auto& logLine : LogHistory) {
        if (logLine.find("Traceback") != std::string::npos || logLine.find("Error:") != std::string::npos) {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", logLine.c_str());
        }
        else {
            ImGui::TextUnformatted(logLine.c_str());
        }
    }

    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }
    ImGui::EndChild();

    ImGui::Separator();

    // --- Input Field Region ---
    ImGui::PushItemWidth(-1);

    // Combine the flags: trigger on Enter, and activate the History Callback pipeline
    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue 
                                    | ImGuiInputTextFlags_CallbackHistory
                                    | ImGuiInputTextFlags_CallbackCompletion;

    if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), input_flags, &REPLTextCallback)) {
        std::string command(inputBuffer);

        if (!command.empty()) {
            // 1. Log the visual prompt echo
            //LogHistory.push_back(">>> " + command + "\n");

            // 2. Track this command in our history ring (avoid duplicating back-to-back inputs)
            if (CommandHistory.empty() || CommandHistory.back() != command) {
                CommandHistory.push_back(command);
            }
            HistoryPos = -1; // Reset historical index to current line state

            // 3. Execute
            ExecuteREPLCommand(command);

            memset(inputBuffer, 0, sizeof(inputBuffer));
            scrollToBottom = true;
        }
        ImGui::SetKeyboardFocusHere(-1);
    }
    ImGui::PopItemWidth();

    ImGui::End();
}

int CellTextCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
        data->InsertChars(data->CursorPos, "    "); // Tab indentation match
    }
    return 0;
}

void DrawJupyterNotebookREPL() {
    // Initialize notebook with a single default cell if empty
    if (NotebookCells.empty()) {
        AddNotebookCell(0);
    }

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("ImGui C++ Jupyter Notebook")) {
        ImGui::End();
        return;
    }

    ImGui::TextDisabled("Notebook Mode. Variables persist globally across blocks.");
    if (ImGui::Button("Add Code Cell at Bottom")) {
        AddNotebookCell(NotebookCells.size());
    }
    ImGui::Separator();

    // Begin a scrolling region for the notebook canvas page
    ImGui::BeginChild("NotebookCanvas", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    size_t cellToDelete = -1;

    for (size_t i = 0; i < NotebookCells.size(); ++i) {
        NotebookCell& cell = NotebookCells[i];

        // Push unique identifier to prevent ImGui widget naming collisions
        ImGui::PushID(cell.id);

        // --- Cell Outer Framing Border ---
        ImGui::BeginGroup();

        // Style Left Side Info Label Box (In [x]: block column matching jupyter)
        char inputLabel[32];
        if (cell.executionCount > 0)
            _snprintf_s(inputLabel, sizeof(inputLabel), "In [%d]:", cell.executionCount);
        else
            _snprintf_s(inputLabel, sizeof(inputLabel), "In [ ]:");

        ImGui::TextColored(ImVec4(0.3f, 0.6f, 0.9f, 1.0f), "%s", inputLabel);
        ImGui::SameLine(70.0f);

        // Calculate a safe width bounding box for the multi-line input edit line
        float contentWidth = ImGui::GetContentRegionAvail().x - 120.0f;

        // Allocate local frame buffer copy for string data manipulation
        char buf[2048];
        strncpy_s(buf, cell.inputBuffer.c_str(), sizeof(buf));
        buf[sizeof(buf) - 1] = '\0';

        // Combine the flags: trigger on Enter, and activate the History Callback pipeline
        ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue
            | ImGuiInputTextFlags_CallbackHistory
            | ImGuiInputTextFlags_CallbackCompletion;

        //ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackCompletion
        // 
        // static char inputBuffer[256] = "";
        // inputBuffer, IM_ARRAYSIZE(inputBuffer);

        if (ImGui::InputText("##Input", buf, sizeof(buf), input_flags, &REPLTextCallback)) {
            //std::string command(inputBuffer);
            cell.inputBuffer = buf;
        }

#if 0
        if (ImGui::InputTextMultiline("##CellInput", buf, sizeof(buf), ImVec2(contentWidth, 60.0f), input_flags, &CellTextCallback)) {
            cell.inputBuffer = buf;
        }
#endif
        // --- Action Buttons Block ---
        ImGui::SameLine();
        ImGui::BeginGroup();
        if (ImGui::Button("▶ Run", ImVec2(55, 24)) || (ImGui::IsItemActive() && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Enter))) {
            ExecuteCell(cell);
        }
        if (ImGui::Button("+ Cell", ImVec2(55, 22))) {
            AddNotebookCell(i);
        }
        if (NotebookCells.size() > 1) { // Only allow delete if it isn't the last cell left
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("Delete", ImVec2(55, 22))) {
                cellToDelete = i;
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndGroup();

        // --- Output Display Pass ---
        if (!cell.outputs.empty()) {
            ImGui::Spacing();
            ImGui::SameLine(70.0f);
            ImGui::BeginGroup();
            ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "Out [%d]:", cell.executionCount);
            ImGui::SameLine();

            ImGui::BeginChild(ImGui::GetID("OutFrame"), ImVec2(contentWidth, 0), ImGuiWindowFlags_NoScrollbar | ImGuiChildFlags_FrameStyle);
            for (const auto& line : cell.outputs) {
                if (line.rfind("Error:", 0) == 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", line.c_str());
                }
                else {
                    ImGui::TextUnformatted(line.c_str());
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }

        ImGui::EndGroup();

        // Visual cell separator spacing line
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PopID();
    }

    // Safely remove flagged cells outside of iteration processing pass loop
    if (cellToDelete != -1) {
        NotebookCells.erase(NotebookCells.begin() + cellToDelete);
    }

    ImGui::EndChild();
    ImGui::End();
}
