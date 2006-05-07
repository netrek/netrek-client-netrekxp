#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#if defined(__BORLANDC__)
extern HHOOK g_hMsgHook;
extern HHOOK g_hKeyHook;

extern int   g_iFlags;
#else
#pragma data_seg(".shared")
HHOOK g_hMsgHook = NULL;
HHOOK g_hKeyHook = NULL;

int   g_iFlags = NULL;
#pragma data_seg()
#endif

const int KILL_WINKEY =  0x0001;
const int KILL_CTRLESC = 0x0002;
const int KILL_COMBOS =  0x0004;
const int KILL_CONTEXT = 0x0008;

LRESULT CALLBACK Msg_HookProc (int iCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Key_HookProc (int iCode, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) void __fastcall Kill (int iFlags);


extern "C" BOOL __stdcall _DllMainCRTStartup (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID /*lpvReserved*/)
{
   if (fdwReason == DLL_PROCESS_ATTACH)
   {
      DisableThreadLibraryCalls (hinstDLL);
   }

   return TRUE;
}


LRESULT CALLBACK Msg_HookProc (int iCode, WPARAM wParam, LPARAM lParam)
{
   if (wParam != PM_NOREMOVE && iCode >= 0)
   {
      if (((MSG*)lParam)->message == WM_SYSCOMMAND && ((MSG*)lParam)->wParam == SC_TASKLIST)
      {
         if ((g_iFlags & KILL_WINKEY) && !(g_iFlags & KILL_CTRLESC) && !((GetAsyncKeyState (VK_CONTROL) & 32768) && (GetAsyncKeyState (VK_ESCAPE) & 32768)))
         {
            ((MSG*)lParam)->message = WM_NULL;
         }
         else if ((g_iFlags & KILL_WINKEY) && (g_iFlags & KILL_CTRLESC))
         {
            ((MSG*)lParam)->message = WM_NULL;
         }
         else if (!(g_iFlags & KILL_WINKEY) && (g_iFlags & KILL_CTRLESC) && GetAsyncKeyState (VK_CONTROL) && GetAsyncKeyState (VK_ESCAPE))
         {
            ((MSG*)lParam)->message = WM_NULL;
         }
      }
      else if (((MSG*)lParam)->message == WM_CONTEXTMENU && GetAsyncKeyState (VK_APPS) && (g_iFlags & KILL_CONTEXT))
      {
         ((MSG*)lParam)->message = WM_NULL;
      }
      else if (((MSG*)lParam)->message == WM_HOTKEY && (LOWORD(((MSG*)lParam)->lParam) & MOD_WIN) && (g_iFlags & KILL_COMBOS))
      {
         ((MSG*)lParam)->message = WM_NULL;
      }
   }

   return CallNextHookEx (g_hMsgHook, iCode, wParam, lParam);
}


LRESULT CALLBACK Key_HookProc (int iCode, WPARAM wParam, LPARAM lParam)
{
   if (iCode != HC_NOREMOVE && iCode >= 0)
   {
      if ((wParam == VK_LWIN || wParam == VK_RWIN) && (g_iFlags & KILL_COMBOS))
      {
         return 1;
      }

      if (wParam == VK_APPS && (g_iFlags & KILL_CONTEXT))
      {
         return 1;
      }
   }

   return CallNextHookEx (g_hKeyHook, iCode, wParam, lParam);
}


extern "C" __declspec(dllexport) void __fastcall Kill (int iFlags)
{
   if(iFlags)
   {
      g_iFlags = iFlags;
      HINSTANCE hInst = GetModuleHandle ("winkey.dll");
      
      if ((iFlags & KILL_WINKEY) || (iFlags & KILL_CTRLESC))
      {
         g_hMsgHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)Msg_HookProc, hInst, 0);
      }

      if ((iFlags & KILL_CONTEXT) || (iFlags & KILL_COMBOS))
      {
         g_hKeyHook = SetWindowsHookEx (WH_KEYBOARD, (HOOKPROC)Key_HookProc, hInst, 0);

         if (!g_hMsgHook)
         {
            g_hMsgHook = SetWindowsHookEx (WH_GETMESSAGE, (HOOKPROC)Msg_HookProc, hInst, 0);
         }
      }
      return;
   }

   if (g_hMsgHook)
   {
      UnhookWindowsHookEx (g_hMsgHook);
   }   

   if (g_hKeyHook)
   {
      UnhookWindowsHookEx (g_hKeyHook);
   }
}
