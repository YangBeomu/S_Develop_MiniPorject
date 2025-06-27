#ifndef KEYBOARDSECURITY_H
#define KEYBOARDSECURITY_H

#include "pch.h"
#include "util.hpp"

class KeyboardSecurity
{
public:
    enum DataType {
        ORIGINAL,
        ENCRYPTED
    };
private:
    enum VKCode : unsigned int {
        // Numbers 0-9
        VK_0          = 0x30,
        VK_1          = 0x31,
        VK_2          = 0x32,
        VK_3          = 0x33,
        VK_4          = 0x34,
        VK_5          = 0x35,
        VK_6          = 0x36,
        VK_7          = 0x37,
        VK_8          = 0x38,
        VK_9          = 0x39,

        // Letters A-Z
        VK_A          = 0x41,
        VK_B          = 0x42,
        VK_C          = 0x43,
        VK_D          = 0x44,
        VK_E          = 0x45,
        VK_F          = 0x46,
        VK_G          = 0x47,
        VK_H          = 0x48,
        VK_I          = 0x49,
        VK_J          = 0x4A,
        VK_K          = 0x4B,
        VK_L          = 0x4C,
        VK_M          = 0x4D,
        VK_N          = 0x4E,
        VK_O          = 0x4F,
        VK_P          = 0x50,
        VK_Q          = 0x51,
        VK_R          = 0x52,
        VK_S          = 0x53,
        VK_T          = 0x54,
        VK_U          = 0x55,
        VK_V          = 0x56,
        VK_W          = 0x57,
        VK_X          = 0x58,
        VK_Y          = 0x59,
        VK_Z          = 0x5A,
    };

    typedef struct tagKBDLLHOOKSTRUCT {
        DWORD     vkCode;
        DWORD     scanCode;
        DWORD     flags;
        DWORD     time;
        ULONG_PTR dwExtraInfo;
    } KBDLLHOOKSTRUCT, *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

    struct ProcessIdInformation {

        uint16_t processId_{};
        uint16_t threadId_{};

        bool isEmpty() {
            ProcessIdInformation pi{};

            return !memcmp(this, &pi, sizeof(ProcessIdInformation));
        }
    };

    using ProcessIdInformationPtr = ProcessIdInformation*;

private:
    static constexpr int ENCRYPT_CNT = 5;
    static constexpr uint32_t THREAD_TICK = 100;

    std::string processName_{};
    ProcessIdInformation pi_{};

    HHOOK hookHandle_{};

    //status
    bool Init();
    bool End();

    //key
    static bool CheckSpecialKeys(const DWORD& virtualKey);
    static DWORD Encrypt(const DWORD& virtualKey);

    //process
    uint16_t GetProcessId(const std::string& process);
    uint16_t GetThreadId(const uint16_t& pid);

    //thread
    std::thread threadHandle_{};
    std::mutex ctlMtx_{};
    bool isRunning_{};

    //process
    ProcessIdInformation GetProcessIdInformation(const std::string& processName);

    //log
    void WarningMsg(const std::string& msg);
    void ErrorMsg(const std::string& msg);

    //thread
    void InputThreadFunc();

public:
    static const std::unordered_set<uint16_t> specialKeys;
    static std::map<KeyboardSecurity*, uint8_t*> processIdInfos_;
    static std::list<DWORD> originVkCode_, cryptVkCode_;
    static std::list<shared_ptr<INPUT>> inputList_;

    //thread
    static std::mutex dataMtx_;

    static LRESULT KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

    std::list<DWORD> ExportRegisteredVkCodes(const DataType& type);

    explicit KeyboardSecurity(std::string& processName);
    ~KeyboardSecurity();


};

using KeyboardSecurityPtr = KeyboardSecurity*;
#endif // KEYBOARDSECURITY_H
