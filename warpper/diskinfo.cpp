// MFCLibrary1.cpp: 定义 DLL 的初始化例程。
//

#include "../stdafx.h"
#include "diskinfo.h"
#include "../AtaSmart.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern "C" __declspec(dllexport) int testDll(int a, int b)
{
	return a + b;
}

char* g_strbuffer=NULL;

extern "C" __declspec(dllexport) CAtaSmart * CreateAtaSmart()
{

	g_strbuffer = new char[500];
	auto ata = new CAtaSmart;
	ata->FlagUsbSat = TRUE;
	ata->FlagUsbIodata = TRUE;
	ata->FlagUsbSunplus = TRUE;
	ata->FlagUsbLogitec = TRUE;
	ata->FlagUsbProlific = TRUE;
	ata->FlagUsbJmicron = TRUE;
	ata->FlagUsbCypress = TRUE;
	ata->FlagUsbASM1352R = TRUE;
	ata->FlagUsbRealtek9220DP = TRUE;
	ata->FlagUsbMemory = TRUE;
	//	ata->FlagUsbSat16 = TRUE;
	ata->FlagUsbNVMeJMicron3 = TRUE;
	ata->FlagUsbNVMeJMicron = TRUE;
	ata->FlagUsbNVMeASMedia = TRUE;
	ata->FlagUsbNVMeRealtek = TRUE;
	ata->FlagAMD_RC2 = TRUE;
	ata->FlagMegaRAID = TRUE;
	ata->FlagIntelVROC = TRUE;
	return ata;
}

extern "C" __declspec(dllexport) void DestroyAtaSmart(CAtaSmart * ptr)
{
	if (g_strbuffer != NULL) {
		delete g_strbuffer;
		g_strbuffer = NULL;
	}
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

extern "C" __declspec(dllexport) void InitAtaSmart(CAtaSmart * ptr, bool useWmi, bool advancedDiskSearch,
	bool flagChangeDisk, bool workaroundHD204UI, bool workaroundAdataSsd,
	bool flagHideNoSmartDisk)
{
	auto flagCD = flagChangeDisk ? TRUE : FALSE;
	ptr->Init(useWmi ? TRUE : FALSE, advancedDiskSearch ? TRUE : FALSE, &flagCD, workaroundHD204UI ? TRUE : FALSE,
		workaroundAdataSsd ? TRUE : FALSE, flagHideNoSmartDisk ? TRUE : FALSE, false);
}


char* csToString(CString str)
{
	auto len = str.GetLength() + 1;
	auto buff = new TCHAR[len];
	memcpy(buff, (LPCTSTR)str, sizeof(TCHAR) * (len));
	size_t converted;
	wcstombs_s(&converted, g_strbuffer, len, buff, len);
	delete[] buff;
	return g_strbuffer;

}

extern "C" __declspec(dllexport) int GetPhysicalDriveId(CAtaSmart * ptr, int index)
{
	return ptr->vars[index].PhysicalDriveId;
}

extern "C" __declspec(dllexport) int GetInterfaceType(CAtaSmart * ptr, int index)
{
	return ptr->vars[index].InterfaceType;
}

extern "C" __declspec(dllexport) char* GetModel(CAtaSmart * ptr, int index)
{
	return csToString(ptr->vars[index].Model);
}

extern "C" __declspec(dllexport) char* GetSerialNumber(CAtaSmart * ptr, int index)
{
	return csToString(ptr->vars[index].SerialNumber);
}

extern "C" __declspec(dllexport) char* GetDrivemap(CAtaSmart * ptr, int index)
{
	return csToString(ptr->vars[index].DriveMap);
}

extern "C" __declspec(dllexport) int GetDriveLettermap(CAtaSmart * ptr, int index)
{
	auto drive = ptr->vars[index];
	return drive.DriveLetterMap;
}

extern "C" __declspec(dllexport) char* GetFirmware(CAtaSmart * ptr, int index)
{
	return csToString(ptr->vars[index].FirmwareRev);
}

extern "C" __declspec(dllexport) _SMART_ATTRIBUTE * GetSmart(CAtaSmart * ptr, int index)
{
	return ptr->vars[index].Attribute;
}

extern "C" __declspec(dllexport) unsigned long GetTotalDiskSize(CAtaSmart * ptr, int index)
{
	return ptr->vars[index].TotalDiskSize;
}

extern "C" __declspec(dllexport) int GetDiskCount(CAtaSmart * ptr)
{
	return (int)ptr->vars.GetCount();
}

extern "C" __declspec(dllexport) bool CreateRandomFile(const char* path, int size)
{
	CString TestFilePath(path);
	auto hFile = ::CreateFile(TestFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// COMPRESSION_FORMAT_NONE
	USHORT lpInBuffer = COMPRESSION_FORMAT_NONE;
	DWORD lpBytesReturned = 0;
	DeviceIoControl(hFile, FSCTL_SET_COMPRESSION, (LPVOID)&lpInBuffer,
		sizeof(USHORT), NULL, 0, (LPDWORD)&lpBytesReturned, NULL);

	// Fill Test Data
	char* buf = NULL;
	int BufSize;
	int Loop;
	int i;
	DWORD writesize;
	BufSize = 1024 * 1024;
	Loop = size;

	buf = (char*)VirtualAlloc(NULL, BufSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (buf == NULL)
	{
		return false;
	}

	for (i = 0; i < BufSize; i++)
	{
		buf[i] = (char)(rand() % 256);
	}


	for (i = 0; i < Loop; i++)
	{
		WriteFile(hFile, buf, BufSize, &writesize, NULL);
	}
	VirtualFree(buf, 0, MEM_RELEASE);
	CloseHandle(hFile);
	return true;
}
