using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;
namespace QuickAssist
{


    static class NativeMethods
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);


        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FreeLibrary(IntPtr hModule);




    }
    class DevAssistIFAdapter
    {
       // [DllImport(@"DevAssistIF.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]

        [DllImportAttribute(@"DevAssistIF.dll",
            EntryPoint = "OpenSettings", SetLastError = true, CharSet = CharSet.Unicode, ExactSpelling = true,
         CallingConvention = CallingConvention.Cdecl)]
        private static extern long OpenSettings();


        [DllImport(@"DevAssistIF.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern long ToggleSoruce(
            string fileName,
            System.Int32 nOutBufferSize_i,
            StringBuilder lpFile_o);
        [DllImport(@"DevAssistIF.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern long SearchFile(string fileName, System.Int32 nOutBufferSize_i, StringBuilder lpFile_o);


//        [DllImport("kernel32.dll", SetLastError = true)]
//        [PreserveSig]
//        public static extern uint GetModuleFileName
//        (
//            [In]
//    IntPtr hModule,

//            [Out] 
//    StringBuilder lpFilename,

//            [In]
//    [MarshalAs(UnmanagedType.U4)]
//    int nSize
//);




        public string OpenFile(string strInputItem)
        {
            StringBuilder fileOut = new StringBuilder(512);
            long lRet = SearchFile(strInputItem, fileOut.Capacity, fileOut);
            return fileOut.ToString();
        }

        public string ToggleFile(string strCurrentFile)
        {
            StringBuilder fileOut = new StringBuilder(512);
            long lRet = ToggleSoruce(strCurrentFile, fileOut.Capacity, fileOut);
            return fileOut.ToString();
        }

        // extern "C" __declspec(dllexport) const TCHAR * getName();
        //[DllImport(@"D:\Divin\FLaunch\Final\DevAssist\Del\Bin\Debug\NppPluginTemplate.dll", 
        //   CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        //public static extern bool isUnicode();




       //  [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
       //  private delegate long OpenSettings();

        public void ShowSettings()
        {

            //IntPtr pDll = NativeMethods.LoadLibrary(@"D:\Divin\FLaunch\Final\DevAssist\Del\Bin\Debug\NppPluginTemplate.dll");
            //int nVal =  Marshal.GetLastWin32Error();

            ////oh dear, error handling here
            ////if (pDll == IntPtr.Zero)

            //IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "OpenSettings");
            ////oh dear, error handling here
            ////if(pAddressOfFunctionToCall == IntPtr.Zero)

            //OpenSettings multiplyByTen = (OpenSettings)Marshal.GetDelegateForFunctionPointer(
            //                                                                        pAddressOfFunctionToCall,
            //                                                                        typeof(OpenSettings));

            //long theResult = multiplyByTen();

            long lRet = OpenSettings();
            // Works

            //// string strs = Directory.GetCurrentDirectory();
            //Directory.SetCurrentDirectory(@"D:\Divin\FLaunch\Final\DevAssist\Del\Bin\Debug\");
            // bool str = isUnicode();

            // //Directory.SetCurrentDirectory(strs);
            // MessageBox.Show(str.ToString());
        }



    }
}
