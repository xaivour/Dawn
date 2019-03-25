/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/platform.h"

#if DAWN_PLATFORM_WINDOWS

#include "core/strings/string_stream.h"
#include <windows.h>
#include <dbghelp.h>

namespace dawn
{
namespace error
{
	void callstack(StringStream& ss)
	{
		SymInitialize(GetCurrentProcess(), NULL, TRUE);
		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
		DWORD mtype;
		CONTEXT ctx;
		ZeroMemory(&ctx, sizeof(CONTEXT));
		ctx.ContextFlags = CONTEXT_CONTROL;
		RtlCaptureContext(&ctx);

		STACKFRAME64 stack;
		ZeroMemory(&stack, sizeof(STACKFRAME64));
#if defined(_M_IX86)
		mtype = IMAGE_FILE_MACHINE_I386;
		stack.AddrPC.Offset = ctx.Eip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = ctx.Ebp;
		stack.AddrFrame.Mode = AddrModeFlat;
		stack.AddrStack.Offset = ctx.Esp;
		stack.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_X64)
		mtype = IMAGE_FILE_MACHINE_AMD64;
		stack.AddrPC.Offset = ctx.Rip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = ctx.Rsp;
		stack.AddrFrame.Mode = AddrModeFlat;
		stack.AddrStack.Offset = ctx.Rsp;
		stack.AddrStack.Mode = AddrModeFlat;
#endif

		DWORD ldsp = 0;
		IMAGEHLP_LINE64 line;
		ZeroMemory(&line, sizeof(IMAGEHLP_LINE64));
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		char buf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO sym = (PSYMBOL_INFO)buf;
		sym->SizeOfStruct = sizeof(SYMBOL_INFO);
		sym->MaxNameLen = MAX_SYM_NAME;

		UINT num = 0;
		while (StackWalk64(mtype
				, GetCurrentProcess()
				, GetCurrentThread()
				, &stack
				, &ctx
				, NULL
				, SymFunctionTableAccess64
				, SymGetModuleBase64
				, NULL
				))
		{
			if (stack.AddrPC.Offset == 0)
				break;

			++num;

			BOOL res = SymGetLineFromAddr64(GetCurrentProcess()
						, stack.AddrPC.Offset
						, &ldsp
						, &line
						);
			res = res && SymFromAddr(GetCurrentProcess(), stack.AddrPC.Offset, 0, sym);

			char buf[512];

			if (res == TRUE)
			{
				snprintf(buf
					, sizeof(buf)
					, "    [%2i] %s in %s:%d\n"
					, num
					, sym->Name
					, line.FileName
					, line.LineNumber
					);
			}
			else
			{
				snprintf(buf
					, sizeof(buf)
					, "    [%2i] 0x%p\n"
					, num
					, stack.AddrPC.Offset
					);
			}

			ss << buf;
		}

		SymCleanup(GetCurrentProcess());
	}

} // namespace error

} // namespace dawn

#endif // DAWN_PLATFORM_WINDOWS
