




#define _YY_APPLY_TO_LATE_BOUND_MODULES(_APPLY)                                                          \
    _APPLY(ntdll,                                        "ntdll"                                       ) \
    _APPLY(kernel32,                                     "kernel32"                                    ) \
    _APPLY(psapi,                                        "psapi"                                       ) \
    _APPLY(version,                                      "version"                                     ) \
	_APPLY(advapi32,                                     "advapi32"                                    ) \
    _APPLY(user32,                                       "user32"                                      ) \
    _APPLY(ws2_32,                                       "ws2_32"                                      ) \
    _APPLY(shell32,                                      "shell32"                                     ) 


//全局可能使用到的函数
#define _YY_APPLY_TO_LATE_BOUND_FUNCTIONS(_APPLY)                                                        \
    _APPLY(NtQueryDirectoryFile,                         ntdll                                         ) \
    _APPLY(NtQueryInformationFile,                       ntdll                                         ) \
    _APPLY(NtSetInformationFile,                         ntdll                                         ) \
    _APPLY(RtlNtStatusToDosError,                        ntdll                                         ) \
    _APPLY(RtlDosPathNameToNtPathName_U_WithStatus,      ntdll                                         ) \
    _APPLY(RtlFreeUnicodeString,                         ntdll                                         ) \
    _APPLY(NtQueryObject,                                ntdll                                         ) \
    _APPLY(NtQueryInformationThread,                     ntdll                                         ) \
    _APPLY(NtQueryInformationProcess,                    ntdll                                         ) \
    _APPLY(NtOpenKeyedEvent,                             ntdll                                         ) \
    _APPLY(NtWaitForKeyedEvent,                          ntdll                                         ) \
    _APPLY(NtReleaseKeyedEvent,                          ntdll                                         ) 



#ifndef YY_Thunks_Support_Version
#define YY_Thunks_Support_Version WDK_NTDDI_VERSION
#endif

#define _WINSOCKAPI_
#define PSAPI_VERSION 1

#define _Disallow_YY_KM_Namespace
#include "km.h"
#include <Shlwapi.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <psapi.h>
#include <winnls.h>
#include "YY_Thunks.h"

#if (YY_Thunks_Support_Version < NTDDI_WS03SP1)
#pragma comment(lib, "Advapi32.lib")
#endif

#if (YY_Thunks_Support_Version < NTDDI_WIN6)
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "ole32.lib")
#endif

#if (YY_Thunks_Support_Version < NTDDI_WIN7)
#pragma comment(lib, "psapi.lib")
#endif



//展开函数的所有的 声明 以及 try_get_ 函数
#define YY_Thunks_Defined
#define __YY_Thunks_Expand_Function(_MODULE, _FUNCTION, _SIZE)                                 \
	static auto __cdecl _CRT_CONCATENATE(try_get_, _FUNCTION)() noexcept                       \
	{                                                                                          \
		__declspec(allocate(".YYThu$AAB")) static void* _CRT_CONCATENATE(pFun_, _FUNCTION);    \
		return reinterpret_cast<decltype(_FUNCTION)*>(try_get_function(                        \
		&_CRT_CONCATENATE( pFun_ ,_FUNCTION),                                                  \
		_CRT_STRINGIZE(_FUNCTION),                                                             \
		_MODULE));                                                                             \
	}
#include "YY_Thunks_List.hpp"

#undef __YY_Thunks_Expand_Function
#undef YY_Thunks_Defined


namespace YY
{
	namespace Thunks
	{
		namespace internal
		{
			static DWORD __fastcall BaseSetLastNTError(
				_In_ NTSTATUS Status
				)
			{
				auto pRtlNtStatusToDosError = try_get_RtlNtStatusToDosError();

				//如果没有RtlNtStatusToDosError就直接设置Status代码吧，反正至少比没有错误代码强
				DWORD lStatus = pRtlNtStatusToDosError ? pRtlNtStatusToDosError(Status) : Status;
				SetLastError(lStatus);
				return lStatus;
			}

			EXTERN_C typedef struct _ThreadContext
			{
				union
				{
					CALINFO_ENUMPROCEXEX pCalInfoEnumProcExEx;
					DATEFMT_ENUMPROCEXEX lpDateFmtEnumProcExEx;
				};

				LPARAM lParam;
			} ThreadContext;

			static ThreadContext* __fastcall BaseGetThreadContext()
			{
				static thread_local ThreadContext ThreadContext = {};

				return &ThreadContext;
			}

			static void __fastcall RaiseStatus(NTSTATUS Status)
			{
				RaiseException(Status, EXCEPTION_NONCONTINUABLE, 0, NULL);
			}

			static LARGE_INTEGER* __fastcall BaseFormatTimeOut(LARGE_INTEGER* Timeout, DWORD dwMilliseconds)
			{
				if (dwMilliseconds == INFINITE)
					return nullptr;

				Timeout->QuadPart = -10000ll * dwMilliseconds;

				return Timeout;
			}

		}
	}//namespace Thunks

} //namespace YY

//导入实际的实现
#define __YY_Thunks_Expand_Function(_MODULE, _FUNCTION, _SIZE) _LCRT_DEFINE_IAT_SYMBOL(_FUNCTION, _SIZE)
#include "YY_Thunks_List.hpp"