// MFCLibrary1.cpp: 定义 DLL 的初始化例程。
//

#include "../stdafx.h"
#include "diskinfo.h"
#include <iostream>
#include "../AtaSmart.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DLL_EXPORT extern "C" __declspec(dllexport)
#define MAX_LEN 512

DLL_EXPORT int testDll(int a, int b)
{
	return a + b;
}

typedef struct TestStruct
{
	INT a;
	double b;
	char text[MAX_LEN];
} TestStruct;


typedef struct _MY_SMART_INFO_DEF
{
	INT PhysicalDriveId{};
	INT ScsiPort{};
	INT ScsiTargetId{};
	INT ScsiBus{};
	INT SiliconImageType{};
	DWORD TotalDiskSize{};
	DWORD Cylinder{};
	DWORD Head{};
	DWORD Sector{};
	DWORD Sector28{};
	ULONGLONG Sector48{};
	ULONGLONG NumberOfSectors{};
	DWORD DiskSizeChs{};
	DWORD DiskSizeLba28{};
	DWORD DiskSizeLba48{};
	DWORD LogicalSectorSize{};
	DWORD PhysicalSectorSize{};
	DWORD DiskSizeWmi{};
	DWORD BufferSize{};
	ULONGLONG NvCacheSize{};
	DWORD TransferModeType{};
	DWORD DetectedTimeUnitType{};
	DWORD MeasuredTimeUnitType{};
	DWORD AttributeCount{};
	INT DetectedPowerOnHours{};
	INT MeasuredPowerOnHours{};
	INT PowerOnRawValue{};
	INT PowerOnStartRawValue{};
	DWORD PowerOnCount{};
	INT Temperature{};
	double TemperatureMultiplier{};
	DWORD NominalMediaRotationRate{};
	INT HostWrites{};
	INT HostReads{};
	INT GBytesErased{};
	INT NandWrites{};
	INT WearLevelingCount{};
	INT Life{};
	DWORD Major{};
	DWORD Minor{};
	DWORD DiskStatus{};
	DWORD DriveLetterMap{};
	INT AlarmTemperature{};
	DWORD DiskVendorId{};
	DWORD UsbVendorId{};
	DWORD UsbProductId{};
	char SerialNumber[MAX_LEN];
	char SerialNumberReverse[MAX_LEN];
	char FirmwareRev[MAX_LEN];
	char FirmwareRevReverse[MAX_LEN];
	char Model[MAX_LEN];
	char ModelReverse[MAX_LEN];
	char ModelWmi[MAX_LEN];
	char ModelSerial[MAX_LEN];
	char DriveMap[MAX_LEN];
	char MaxTransferMode[MAX_LEN];
	char CurrentTransferMode[MAX_LEN];
	char MajorVersion[MAX_LEN];
	char MinorVersion[MAX_LEN];
	char Interface[MAX_LEN];
	char Enclosure[MAX_LEN];
	char CommandTypeString[MAX_LEN];
	char SsdVendorString[MAX_LEN];
	char DeviceNominalFormFactor[MAX_LEN];
	char PnpDeviceId[MAX_LEN];
	char SmartKeyName[MAX_LEN];
} MY_SMART_INFO;

char* g_strbuffer = NULL;
// MY_SMART_INFO *g_smart_info = NULL;

DLL_EXPORT CAtaSmart* CreateAtaSmart()
{
	g_strbuffer = new char[MAX_LEN];
	// g_smart_info = new MY_SMART_INFO;
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

DLL_EXPORT void DestroyAtaSmart(CAtaSmart* ptr)
{
	if (g_strbuffer != NULL)
	{
		delete g_strbuffer;
		g_strbuffer = NULL;
	}
	// if (g_smart_info != NULL)
	// {
	// 	delete g_smart_info;
	// 	g_smart_info = NULL;
	// }
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

DLL_EXPORT void InitAtaSmart(CAtaSmart* ptr, bool useWmi, bool advancedDiskSearch,
	bool flagChangeDisk, bool workaroundHD204UI, bool workaroundAdataSsd,
	bool flagHideNoSmartDisk)
{
	auto flagCD = flagChangeDisk ? TRUE : FALSE;
	ptr->Init(useWmi ? TRUE : FALSE, advancedDiskSearch ? TRUE : FALSE, &flagCD, workaroundHD204UI ? TRUE : FALSE,
		workaroundAdataSsd ? TRUE : FALSE, flagHideNoSmartDisk ? TRUE : FALSE, false);
}

char* copyString(CString src, char* dest, int dest_len)
{
	auto len = src.GetLength() + 1;

	auto buff = src.GetBuffer();
	errno_t err;
	size_t converted;
	setlocale(LC_ALL, "");
	err = wcstombs_s(&converted, dest, dest_len, buff, len);
	setlocale(LC_ALL, "C");     //默认
	if (err != 0)
	{
		std::cerr << "error" << err << std::endl;
	}
	src.ReleaseBuffer();
	return dest;
}


DLL_EXPORT void GetTestInfo(TestStruct& info)
{
	info.a = 1;
	info.b = 2.0;
	auto text = new CString("你好hello1244");
	char* textpr = info.text;
	copyString(*text, textpr, MAX_LEN);
	return;
}

DLL_EXPORT bool GetDiskInfo(CAtaSmart* ptr, int index, MY_SMART_INFO& info)
{
	auto drive = ptr->vars[index];
	info.PhysicalDriveId = drive.PhysicalDriveId;
	info.ScsiPort = drive.ScsiPort;
	info.ScsiTargetId = drive.ScsiTargetId;
	info.ScsiBus = drive.ScsiBus;
	info.SiliconImageType = drive.SiliconImageType;
	info.TotalDiskSize = drive.TotalDiskSize;
	info.Cylinder = drive.Cylinder;
	info.Head = drive.Head;
	info.Sector = drive.Sector;
	info.Sector28 = drive.Sector28;
	info.Sector48 = drive.Sector48;
	info.NumberOfSectors = drive.NumberOfSectors;
	info.DiskSizeChs = drive.DiskSizeChs;
	info.DiskSizeLba28 = drive.DiskSizeLba28;
	info.DiskSizeLba48 = drive.DiskSizeLba48;
	info.LogicalSectorSize = drive.LogicalSectorSize;
	info.PhysicalSectorSize = drive.PhysicalSectorSize;
	info.DiskSizeWmi = drive.DiskSizeWmi;
	info.BufferSize = drive.BufferSize;
	info.NvCacheSize = drive.NvCacheSize;
	info.TransferModeType = drive.TransferModeType;
	info.DetectedTimeUnitType = drive.DetectedTimeUnitType;
	info.MeasuredTimeUnitType = drive.MeasuredTimeUnitType;
	info.AttributeCount = drive.AttributeCount;
	info.DetectedPowerOnHours = drive.DetectedPowerOnHours;
	info.MeasuredPowerOnHours = drive.MeasuredPowerOnHours;
	info.PowerOnRawValue = drive.PowerOnRawValue;
	info.PowerOnStartRawValue = drive.PowerOnStartRawValue;
	info.PowerOnCount = drive.PowerOnCount;
	info.Temperature = drive.Temperature;
	info.TemperatureMultiplier = drive.TemperatureMultiplier;
	info.NominalMediaRotationRate = drive.NominalMediaRotationRate;
	info.HostWrites = drive.HostWrites;
	info.HostReads = drive.HostReads;
	info.GBytesErased = drive.GBytesErased;
	info.NandWrites = drive.NandWrites;
	info.WearLevelingCount = drive.WearLevelingCount;
	info.Life = drive.Life;
	info.Major = drive.Major;
	info.Minor = drive.Minor;
	info.DiskStatus = drive.DiskStatus;
	info.DriveLetterMap = drive.DriveLetterMap;
	info.AlarmTemperature = drive.AlarmTemperature;
	info.DiskVendorId = drive.DiskVendorId;
	info.UsbVendorId = drive.UsbVendorId;
	info.UsbProductId = drive.UsbProductId;

	copyString(drive.SerialNumber, info.SerialNumber, MAX_LEN);
	copyString(drive.SerialNumberReverse, info.SerialNumberReverse, MAX_LEN);
	copyString(drive.FirmwareRev, info.FirmwareRev, MAX_LEN);
	copyString(drive.FirmwareRevReverse, info.FirmwareRevReverse, MAX_LEN);
	copyString(drive.Model, info.Model, MAX_LEN);
	copyString(drive.ModelReverse, info.ModelReverse, MAX_LEN);
	copyString(drive.ModelWmi, info.ModelWmi, MAX_LEN);
	copyString(drive.ModelSerial, info.ModelSerial, MAX_LEN);
	copyString(drive.DriveMap, info.DriveMap, MAX_LEN);
	copyString(drive.MaxTransferMode, info.MaxTransferMode, MAX_LEN);
	copyString(drive.CurrentTransferMode, info.CurrentTransferMode, MAX_LEN);
	copyString(drive.MajorVersion, info.MajorVersion, MAX_LEN);
	copyString(drive.MinorVersion, info.MinorVersion, MAX_LEN);
	copyString(drive.Interface, info.Interface, MAX_LEN);
	copyString(drive.Enclosure, info.Enclosure, MAX_LEN);
	copyString(drive.CommandTypeString, info.CommandTypeString, MAX_LEN);
	copyString(drive.SsdVendorString, info.SsdVendorString, MAX_LEN);
	copyString(drive.DeviceNominalFormFactor, info.DeviceNominalFormFactor, MAX_LEN);
	copyString(drive.PnpDeviceId, info.PnpDeviceId, MAX_LEN);
	copyString(drive.SmartKeyName, info.SmartKeyName, MAX_LEN);
	return true;

}

DLL_EXPORT int GetPhysicalDriveId(CAtaSmart* ptr, int index)
{
	return ptr->vars[index].PhysicalDriveId;
}

DLL_EXPORT int GetInterfaceType(CAtaSmart* ptr, int index)
{
	return ptr->vars[index].InterfaceType;
}

DLL_EXPORT char* GetModel(CAtaSmart* ptr, int index)
{
	return copyString(ptr->vars[index].Model, g_strbuffer, MAX_LEN);
}

DLL_EXPORT char* GetSerialNumber(CAtaSmart* ptr, int index)
{
	return copyString(ptr->vars[index].SerialNumber, g_strbuffer, MAX_LEN);
}


DLL_EXPORT char* GetDrivemap(CAtaSmart* ptr, int index)
{
	return copyString(ptr->vars[index].DriveMap, g_strbuffer, MAX_LEN);
}


DLL_EXPORT int GetDriveLettermap(CAtaSmart* ptr, int index)
{
	auto drive = ptr->vars[index];
	return drive.DriveLetterMap;
}

DLL_EXPORT char* GetFirmware(CAtaSmart* ptr, int index)
{
	return copyString(ptr->vars[index].FirmwareRev, g_strbuffer, MAX_LEN);
}


DLL_EXPORT unsigned long GetTotalDiskSize(CAtaSmart* ptr, int index)
{
	return ptr->vars[index].TotalDiskSize;
}

DLL_EXPORT int GetDiskCount(CAtaSmart* ptr)
{
	return (int)ptr->vars.GetCount();
}

DLL_EXPORT bool CreateRandomFile(const char* path, int size)
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
