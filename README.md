# CrystalDiskInfo_dll_lib

· 中文· [English](./README-en.md)

CrystalDiskInfo项目的dll动态库的封装，方便其它语言使用。

## 为什么做这个

我在做一个软件，需要读取电脑上所有连接的硬盘信息（主要是硬件型号和序列号）。项目是使用wpf开发的。
本来我是使用C#的wmi接口去获取的，代码如下：
```
  ManagementObjectSearcher moSearcher = new ManagementObjectSearcher("SELECT * FROM Win32_DiskDrive");
  foreach (var mo in moSearcher.Get())
  {
      var diskinfo = new DiskInfo();
      diskinfo.device_name = mo["Model"].ToString();
      diskinfo.searial_num = mo["SerialNumber"].ToString();  
  }
```
但发现只能正确识别到STAT接口的硬盘，如果硬盘是通过nvme接口或者是usb连接的，就无法识别。
意外发现，有一款开源软件[crystaldiskinfo](https://github.com/hiyohiyo/CrystalDiskInfo)识别的很准确。于时看了一眼他的代码，发现是用c++写的，里面的除了调用wmi接口外，还有很多和硬件驱动交互的逻辑。
于是我就想，能不能把crystaldiskinfo封装成一个dll库给c#调用，这样就能识别准确了。在参考了网上的资料后，我实现了这个库。

## dll使用方法

先下载编译好的(dll)[https://github.com/ftyszyx/CrystalDiskInfo_dll_lib/releases/tag/1.0.0],dll分32位和64位

### 导入接口

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

### 初始化atasmart

```
 static IntPtr smartPtr  ;
 static void Init()
 {
     smartPtr = CreateAtaSmart();
     InitAtaSmart(smartPtr, true, false, false, false, true, true);
 }
```

### 获取硬盘数量

```
 public static int GetDiskCount()
 {
     if (smartPtr == IntPtr.Zero)
         Init();
     return GetDiskCount(smartPtr);
 }
```

### 获取硬盘modelname和序列号
```
 public static string GetDiskModel(int index)
 {
     if (smartPtr==IntPtr.Zero)
         Init();
     var textptr=GetModel(smartPtr, index);
     var text = Marshal.PtrToStringAnsi(textptr);
     return text;
 }

 public static string GetDiskSerialNumber(int index)
 {
     if (smartPtr == IntPtr.Zero)
         Init();
     var textptr=GetSerialNumber(smartPtr, index);
     return Marshal.PtrToStringAnsi(textptr);
 }
```

### 销毁
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

## 自己编译

使用visual studio打开DiskInfo.sln自行编译

## 注意事项

### 程序需要管理员权限才能成功获取硬盘信息

如果是winform工程，需要增加清单文件app.manifest，并增加权限
```
 <requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
```
具体可以上网查询

### winform下有可能会识别错操作系统，导致nvme硬盘无法识别
需要在清单文件app.manifest增加系统配置
![image](https://github.com/user-attachments/assets/1bf7c682-b6ed-47cd-a41e-c1851b5de5c2)



## 其它

有问题可以咨询我whyzi@qq.com

## 参考项目

[crystaldiskinfo](https://github.com/hiyohiyo/CrystalDiskInfo)
