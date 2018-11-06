BOOL errDll(char *filename)
{
       HINSTANCE hDll;
      typedef void (WINAPI* LPFUNC)(char *);
       hDll = LoadLibrary(_T("errlib.dll"));//装载动态连接库
       if(hDll != NULL)
       {
              LPFUNC lpfnDllFunc = (LPFUNC)GetProcAddress(hDll,"callpro");//装载函数
              if(!lpfnDllFunc)
              {
                     FreeLibrary(hDll);
                     return FALSE;
              }
              else
              {
               lpfnDllFunc(filename);//调用函数
            }            
       }
       return TRUE;
}
