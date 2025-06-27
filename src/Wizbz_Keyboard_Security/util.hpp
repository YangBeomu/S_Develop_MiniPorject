#ifndef UTIL_H
#define UTIL_H

#include "pch.h"

using namespace std;

namespace utils {
    static string WideToMBS(const WCHAR* str) {
        string ret{};

        try {
            auto size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
            if(size == 0) throw runtime_error("Failed to call WideCharToMultiByte");

            ret.resize(size);

            size = WideCharToMultiByte(CP_UTF8, 0, str, -1, ret.data(), ret.size(), NULL, NULL);
        }catch(const exception& e) {
            cerr<<"<WideToMBS> "<<e.what()<<endl;
        }

        return ret;
    }

    static vector<string> GetProcessNames() {
        vector<string> ret{};
        HANDLE handle{};

        try {
            if ((handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
                throw runtime_error("Failed to snapshot");

            PROCESSENTRY32 et32{};
            et32.dwSize = sizeof(PROCESSENTRY32);

            Process32First(handle, &et32);
            Process32Next(handle, &et32);

            do{
                auto tmp = WideToMBS(et32.szExeFile);
                if(tmp.empty()) throw runtime_error("Failed to convert mbs");

                ret.push_back(tmp);
            }while(Process32Next(handle, &et32));
        }
        catch (const exception& e) {
            cerr<<"<GetProcessName> "<<e.what()<<endl;
        }

        return ret;
    }
}

#endif // UTIL_H
