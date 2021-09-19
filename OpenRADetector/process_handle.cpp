#include <windows.h>
#include <tlhelp32.h>

#include <string_view>

#include "defer.h"

const std::wstring D2k = TEXT("Dune2000.exe");

bool DetectProcess(std::wstring_view target) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof PROCESSENTRY32;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	defer _(nullptr, [&snapshot](...) -> void { CloseHandle(snapshot); });

	if (Process32First(snapshot, &entry)) {
		while (Process32Next(snapshot, &entry)) {
			if (target == entry.szExeFile) return true;
		}
	}

	return false;
}

bool DetectD2kProcess() {
	return DetectProcess(D2k);
}

bool OpenD2K(const LPSTR cmdline, STARTUPINFOA& si, PROCESS_INFORMATION &pi) {
	return CreateProcessA(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}

bool OpenD2KandWaitFor(const LPSTR cmdline) {
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof si);
	si.cb = sizeof si;
	ZeroMemory(&pi, sizeof pi);

	if (!OpenD2K(cmdline, si, pi)) return false;

	printf("process is running successful.");

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}