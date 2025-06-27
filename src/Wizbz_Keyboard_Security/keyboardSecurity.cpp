#include "pch.h"
#include "keyboardSecurity.h"

using namespace std;

//static variable
map<KeyboardSecurityPtr, uint8_t*> KeyboardSecurity::processIdInfos_{}; //uint8_t* ProcessIdInfomationPtr

const unordered_set<uint16_t> KeyboardSecurity::specialKeys =  {
    VK_SHIFT, VK_LSHIFT, VK_RSHIFT,
    VK_CONTROL, VK_LCONTROL, VK_RCONTROL,
    VK_MENU, VK_LMENU, VK_RMENU,
    VK_CAPITAL, // CapsLock
    VK_TAB, VK_ESCAPE, VK_RETURN,
    VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
    VK_INSERT, VK_DELETE, VK_HOME, VK_END, VK_PRIOR, VK_NEXT, // PageUp/Down
    VK_NUMLOCK, VK_SCROLL, VK_PAUSE,
    VK_SNAPSHOT, // PrintScreen
    VK_LWIN, VK_RWIN, VK_APPS, // Windows/메뉴 키
    VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6,
    VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12
};

std::list<DWORD> KeyboardSecurity::originVkCode_{}, KeyboardSecurity::cryptVkCode_{};

std::list<shared_ptr<INPUT>> KeyboardSecurity::inputList_{};

mutex KeyboardSecurity::dataMtx_{};


//functions
LRESULT CALLBACK KeyboardSecurity::KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    try {
        if (nCode < 0) throw runtime_error("wrong code");

        DWORD pid{};

        {
            HWND hwnd{};
            wstring windowTitle(BUFSIZ, 0);

            if((hwnd = GetForegroundWindow()) == NULL) throw runtime_error("Failed to get forground window.");

            if(GetWindowText(hwnd, (LPWSTR)windowTitle.c_str(), windowTitle.size()) <= 0) throw runtime_error("Failed to get window text");

            if(GetWindowThreadProcessId(hwnd, &pid) == 0) throw runtime_error("Failed to get window thread process id.");
        }

        for(const auto& info : processIdInfos_) {
            if(pid == reinterpret_cast<ProcessIdInformationPtr>(info.second)->processId_) {
                LPKBDLLHOOKSTRUCT llHookSt = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);

                //unique_lock<mutex> lck(dataMtx_);

                originVkCode_.push_back(llHookSt->vkCode);

                while(CheckSpecialKeys(llHookSt->vkCode = KeyboardSecurity::Encrypt(llHookSt->vkCode))) {
                    auto randomFunc = []() -> DWORD {
                        std::mt19937_64 gen(std::chrono::steady_clock::now().time_since_epoch().count());
                        std::uniform_int_distribution<int> dis(0, 0xff);

                        return dis(gen);
                    };

                    llHookSt->vkCode = randomFunc();
                }

                cryptVkCode_.push_back(llHookSt->vkCode);


                llHookSt->scanCode = MapVirtualKey(llHookSt->vkCode, MAPVK_VK_TO_VSC);

                //single
                unique_ptr<INPUT> inputPtr(new INPUT{});

                inputPtr->type = INPUT_KEYBOARD;
                inputPtr->ki.wVk = llHookSt->vkCode;
                inputPtr->ki.wScan = llHookSt->scanCode;

                SendInput(INPUT_KEYBOARD, inputPtr.get(), sizeof(INPUT));

                //shared_ptr<INPUT> inputPtr(new INPUT{});

                //inputPtr->type = INPUT_KEYBOARD;
                //inputPtr->ki.wVk = llHookSt->vkCode;
                //inputPtr->ki.wScan = llHookSt->scanCode;

                //inputList_.push_back(inputPtr);

                return 1;
            }
        }
    }catch(const exception& e) {
        cerr<<string("<KeyHookProc> ").append(e.what())<<endl;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

KeyboardSecurity::KeyboardSecurity(string& processName)
    : processName_(processName) {
    if(!Init()) throw runtime_error("Failed to call Init");
}

KeyboardSecurity::~KeyboardSecurity() {
    End();
}

//statusode
bool KeyboardSecurity::Init() {
    try {
        pi_ = GetProcessIdInformation(processName_);
        if(pi_.isEmpty()) throw runtime_error("Failed to get process id information");

        processIdInfos_[this] = reinterpret_cast<uint8_t*>(&pi_);

        hookHandle_ = SetWindowsHookExA(WH_KEYBOARD_LL, &KeyboardSecurity::KeyHookProc, NULL, NULL);
        if(hookHandle_ == NULL)
            throw runtime_error("Failed to set window hook");

        //isRunning_ = true;
        //threadHandle_ = std::thread(&KeyboardSecurity::InputThreadFunc, this);
    }catch(const exception& e) {
        WarningMsg(string("<Init> ").append(e.what()));
        return false;
    }

    return true;
}

bool KeyboardSecurity::End() {
    try {
        processIdInfos_.erase(this);

        if(!UnhookWindowsHookEx(hookHandle_))
            throw runtime_error("Failed to unhook");

        unique_lock<mutex> lck(ctlMtx_);
        isRunning_ = false;
        if(threadHandle_.joinable()) threadHandle_.join();
    }catch(const exception& e) {
        WarningMsg(string("<End> ").append(e.what()));
        return false;
    }

    return true;
}

//key
bool KeyboardSecurity::CheckSpecialKeys(const DWORD& virtualKey) {
    return specialKeys.count(virtualKey);
}

DWORD KeyboardSecurity::Encrypt(const DWORD& virtualKey) {
    DWORD ret{};

    for(int idx=0; idx<ENCRYPT_CNT; idx++)
        ret = virtualKey ^5;

    return ret;
}

//log
void KeyboardSecurity::WarningMsg(const string& msg) {
    cerr<<"[KeyboardSecurity] "<<msg<<endl;
}

void KeyboardSecurity::ErrorMsg(const string& msg) {
    cerr<<"[KeyboardSecurity] "<<msg<<endl;
    exit(1);
}

void KeyboardSecurity::InputThreadFunc() {
    try{
        while(isRunning_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TICK));

            unique_lock<mutex> lck(dataMtx_);
            if(inputList_.empty()) continue;

            DWORD pid{};

            {
                HWND hwnd{};
                wstring windowTitle(BUFSIZ, 0);

                if((hwnd = GetForegroundWindow()) == NULL) throw runtime_error("Failed to get forground window.");

                if(GetWindowText(hwnd, (LPWSTR)windowTitle.c_str(), windowTitle.size()) <= 0) throw runtime_error("Failed to get window text");

                if(GetWindowThreadProcessId(hwnd, &pid) == 0) throw runtime_error("Failed to get window thread process id.");
            }

            if(pid == pi_.processId_) {
                for(const auto& input : inputList_)
                    SendInput(INPUT_KEYBOARD, input.get(), sizeof(INPUT));
            }

        }
    }catch(const exception& e) {
        WarningMsg(string("<InputThreadFunc> ").append(e.what()));
    }
}

//process
uint16_t KeyboardSecurity::GetProcessId(const string& processName) {
    uint16_t ret{};
    HANDLE handle{};

    try {
        if ((handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
            throw runtime_error("Failed to snapshot");

        PROCESSENTRY32 entry{};

        entry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(handle, &entry)) {
            while (Process32Next(handle, &entry)) {
                if ((memcmp(processName.data(), utils::WideToMBS(entry.szExeFile).data(), processName.size()-1)) == 0) {
                    ret = entry.th32ProcessID;
                    break;
                }
            }
        }
    }catch(const exception& e) {
        WarningMsg(string("<GetProcessId").append(e.what()));
    }

    if(!handle) CloseHandle(handle);

    return ret;
}

uint16_t KeyboardSecurity::GetThreadId(const uint16_t& pid) {
    uint16_t ret{};
    HANDLE handle{};

    try {
        if ((handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL)) == INVALID_HANDLE_VALUE)
            throw runtime_error("Failed to snapshot");

        THREADENTRY32 entry{};
        entry.dwSize = sizeof(THREADENTRY32);

        Thread32First(handle, &entry);

        do {
            if(entry.th32OwnerProcessID != pid) continue;

            ret = entry.th32ThreadID;

        }while(Thread32Next(handle, &entry));

    }catch(const exception& e) {
        WarningMsg(string("<GetThreadId> ").append(e.what()));
    }

    if(!handle) CloseHandle(handle);

    return ret;
}

KeyboardSecurity::ProcessIdInformation KeyboardSecurity::GetProcessIdInformation(const string& processName) {
    ProcessIdInformation ret{};

    try {
        if((ret.processId_ = GetProcessId(processName)) == NULL)
            throw runtime_error("Failed to get process id");

        if((ret.threadId_ = GetThreadId(ret.processId_)) == NULL)
            throw runtime_error("Failed to get process id");
    }
    catch (const exception& e) {
        WarningMsg(string("<GetProcessID> ").append(e.what()));
    }

    return ret;
}

//data
std::list<DWORD> KeyboardSecurity::ExportRegisteredVkCodes(const KeyboardSecurity::DataType& type) {
    list<DWORD> ret{};

    unique_lock<mutex> lck(dataMtx_);

    switch(type) {
    case ORIGINAL:
        ret = originVkCode_;
        originVkCode_.clear();
        break;
    case ENCRYPTED:
        ret = cryptVkCode_;
        cryptVkCode_.clear();
        break;
    };

    return ret;
}
