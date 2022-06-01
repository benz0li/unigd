#ifdef _WIN32
#include "r_thread.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <cpp11/R.hpp>
#include <cpp11/protect.hpp>
#include <thread>

namespace unigd
{
    namespace async
    {
        namespace
        {
            const auto *UNIGD_WINDOW_CLASS_NAME = TEXT("unigd_window_class");
            const UINT UNIGD_MESSAGE_ID = WM_USER + 217;
            threadsafe_queue<function_wrapper> work_queue;
            bool ipc_initialized{false};
            HWND message_hwind;
            
            inline void r_print_error(const char *message) {
                REprintf("Error (unigd IPC): %s\n", message);
            }

            inline void process_tasks() {
                function_wrapper task;
                while (work_queue.try_pop(task))
                {
                    task.call();
                }
            }

            LRESULT CALLBACK window_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
            {
                switch (message)
                {
                case UNIGD_MESSAGE_ID:
                    process_tasks();
                    return 0;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }

            inline bool window_class_exists() {
                WNDCLASSEX wc{};
                return GetClassInfoEx(GetModuleHandle(NULL), UNIGD_WINDOW_CLASS_NAME, &wc) != 0;
            }

            inline bool register_window_class() {
                WNDCLASSEX wc{};
                wc.cbSize = sizeof(WNDCLASSEX);
                wc.lpfnWndProc = window_callback;
                wc.hInstance = GetModuleHandle(NULL);
                wc.lpszClassName = UNIGD_WINDOW_CLASS_NAME;
                return RegisterClassEx(&wc) != 0;
            }
            
            inline HWND create_message_window() {
                return CreateWindowEx(0, UNIGD_WINDOW_CLASS_NAME, TEXT("unigd"), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
            }

            inline void notify()
            {
                PostMessage(message_hwind, UNIGD_MESSAGE_ID, 0, 0);
            }
        }

        void ipc_open() 
        {
            if (ipc_initialized) return;

            if (!window_class_exists()) { 
                // If there is already another version of unigd loaded. (E.g. with pkgload::load_all,
                // which, unfortunately does not call .onUnload),
                // we have to re-use the (unique) window class, but should not reuse the message window. 
                // Otherwise we could not destroy the window while it is in use.
                // As the callback lives inside the window class, messages be received only once.

                //message_hwind = FindWindowEx(NULL, NULL, UNIGD_WINDOW_CLASS_NAME, TEXT("unigd"));

                if (!register_window_class())
                {
                    r_print_error("unigd: Failed to register window class.");
                }
            }

            message_hwind = create_message_window();
            if (!message_hwind)
            {
                r_print_error("unigd: Failed to create message window.");
                return;
            }
            ipc_initialized = true;
        }
        
        void ipc_close() 
        {
            if (!ipc_initialized) return;

            if (DestroyWindow(message_hwind) == 0) {
                r_print_error("unigd: Failed to destroy message window.");
            }

            // We can not be sure if there is another dll instance loaded with pkgload::load_all
            // so we cannot unregister the window class.

            //if (UnregisterClass(UNIGD_WINDOW_CLASS_NAME, NULL) == 0) {
            //    r_print_error("unigd: Failed to unregister window class.");
            //}

            ipc_initialized = false;
        }

        void r_thread_impl(function_wrapper &&task)
        {
            work_queue.push(std::move(task));
            notify();
        }

    } // namespace service
} // namespace unigd

#endif