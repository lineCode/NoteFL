﻿#include "stdafx.h"
#include "included.h"


// 应用程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
#ifdef _DEBUG
    ::AllocConsole();
    ::_cwprintf(L"Battle Control  Online! \n");
#endif
    if (SUCCEEDED(::CoInitialize(nullptr)))
    {
        {
            ThisApp app;
            if (SUCCEEDED(app.Initialize(hInstance, nCmdShow)))
            {
                app.RunMessageLoop();
            }
        }
        ::CoUninitialize();
    }
#ifdef _DEBUG
    ::_cwprintf(L"Battle Control Terminated! \n");
    ::FreeConsole();
#endif
    return 0;
}

#include <VersionHelpers.h>

// 初始化库
class InitializeLibrary {
    typedef enum PROCESS_DPI_AWARENESS {
        PROCESS_DPI_UNAWARE = 0,
        PROCESS_SYSTEM_DPI_AWARE = 1,
        PROCESS_PER_MONITOR_DPI_AWARE = 2
    } PROCESS_DPI_AWARENESS;
    // SetProcessDpiAwareness
    static HRESULT STDAPICALLTYPE SetProcessDpiAwarenessF(PROCESS_DPI_AWARENESS);
public:
    //
    InitializeLibrary() {
        // >= Win8.1 ?
        if (::IsWindows8OrGreater()) {
            m_hDllShcore = ::LoadLibraryW(L"Shcore.dll");
            assert(m_hDllShcore);
            if (m_hDllShcore) {
                auto setProcessDpiAwareness =
                    reinterpret_cast<decltype(&InitializeLibrary::SetProcessDpiAwarenessF)>(
                        ::GetProcAddress(m_hDllShcore, "SetProcessDpiAwareness")
                        );
                assert(setProcessDpiAwareness);
                if (setProcessDpiAwareness) {
                    setProcessDpiAwareness(InitializeLibrary::PROCESS_PER_MONITOR_DPI_AWARE);
                }
            }
        }
    };
    //
    ~InitializeLibrary() {
        if (m_hDllShcore) {
            ::FreeLibrary(m_hDllShcore);
            m_hDllShcore = nullptr;
        }
    }
private:
    // Shcore
    HMODULE     m_hDllShcore = nullptr;
} instance;