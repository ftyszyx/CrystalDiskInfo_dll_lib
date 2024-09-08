# CrystalDiskInfo_dll_lib

· English · [中文](./README.md) 

The dll dynamic library of the CrystalDiskInfo project is encapsulated to facilitate the use of other languages.

## Why do this?

I am working on a software that needs to read the information of all hard disks connected to the computer (disk model name and serial numbers). The project is developed using wpf.
Originally I used the wmi interface of C# to obtain it. The code is as follows:
```
ManagementObjectSearcher moSearcher = new ManagementObjectSearcher("SELECT * FROM Win32_DiskDrive");
foreach (var mo in moSearcher.Get())
{
var diskinfo = new DiskInfo();
diskinfo.device_name = mo["Model"].ToString();
diskinfo.searial_num = mo["SerialNumber"].ToString();
}
```
But I found that only the hard disk with the STAT interface can be correctly recognized. If the hard disk is connected through the nvme interface or USB interface, it cannot be recognized correctly.
I accidentally discovered that there is an open source software [crystaldiskinfo](https://github.com/hiyohiyo/CrystalDiskInfo) that can identify it very accurately. took a look at his code and found that it was written in C++. In addition to calling the wmi interface, there was also a lot of logic for interacting with the hardware driver.
So I thought, could I encapsulate crystaldiskinfo into a dll library for c# to call, so that the identification can be accurate. After referring to information on the Internet, I implemented this library.

## How to use dll

First download the compiled (dll) [https://github.com/ftyszyx/CrystalDiskInfo_dll_lib/releases/tag/1.0.0], the dll is divided into 32-bit and 64-bit

### Import interface

```
[DllImport(dllname, EntryPoint = "CreateAtaSmart", CallingConvention = CallingConvention.Cdecl )]
static extern IntPtr CreateAtaSmart();

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern void DestroyAtaSmart(IntPtr ptr);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern void InitAtaSmart(IntPtr ptr, bool useWmi, bool advancedDiskSearch,
bool flagChangeDisk, bool workaroundHD204UI, bool workaroundAdataSsd,
bool flagHideNoSmartDisk);
[DllImport(dllname, CharSet = CharSet.Auto)]
static extern int GetPhysicalDriveId(IntPtr ptr, int index);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern int GetInterfaceType(IntPtr ptr, int index);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern IntPtr GetModel(IntPtr ptr, int index);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern IntPtr GetSerialNumber(IntPtr ptr, int index);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern IntPtr GetDrivemap(IntPtr ptr, int index);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern int GetDiskCount(IntPtr ptr);

[DllImport(dllname, CharSet = CharSet.Auto)]
static extern int GetDriveLettermap(IntPtr ptr,int index);
```

### Initialize atasmart

```
static IntPtr smartPtr  ;
static void Init()
{
smartPtr = CreateAtaSmart();
InitAtaSmart(smartPtr, true, false, false, false, true, true);
}
```

### Get the number of hard disks

```
public static int GetDiskCount()
{
if (smartPtr == IntPtr.Zero)
Heat();
return GetDiskCount(smartPtr);
}
```

### Get the hard disk modelname and serial number
```
public static string GetDiskModel(int index)
{
if (smartPtr==IntPtr.Zero)
Heat();
var textptr=GetModel(smartPtr, index);
var text = Marshal.PtrToStringAnsi(textptr);
return text;
}

public static string GetDiskSerialNumber(int index)
{
if (smartPtr == IntPtr.Zero)
Heat();
var textptr=GetSerialNumber(smartPtr, index);
return Marshal.PtrToStringAnsi(textptr);
}
```

### Destroy
```
public static void destory()
{
if (smartPtr == IntPtr.Zero)
{
return;
}
DestroyAtaSmart(smartPtr);
}
```

## Compile yourself

Use visual studio to open DiskInfo.sln and compile it yourself

## Notes

### The program requires administrator rights to successfully obtain hard disk information.

If it is a winform project, you need to add the manifest file app.manifest and add permissions
```
<requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
```
For details, you can check online

### The wrong operating system may be recognized under winform, resulting in the nvme hard drive being unrecognizable.
System configuration needs to be added to the manifest file app.manifest
![image](https://github.com/user-attachments/assets/1bf7c682-b6ed-47cd-a41e-c1851b5de5c2)




## Others

If you have any questions, please contact me whyzi@qq.com

## Reference project

[crystaldiskinfo](https://github.com/hiyohiyo/CrystalDiskInfo)
