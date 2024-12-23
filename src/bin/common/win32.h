
#ifndef WIN32HEAD
#define WIN32HEAD
#ifdef _WIN32
static inline int
waitpid(DWORD pid, void *unused1 EINA_UNUSED, int unused2 EINA_UNUSED)
{
   DWORD rez;
   HANDLE process_handle = OpenProcess(SYNCHRONIZE, FALSE, pid);
   if (!process_handle) return -1;
   rez = WaitForSingleObject(process_handle, INFINITE);
   CloseHandle(process_handle);
   if (rez == WAIT_FAILED)
      return -1;
   else
      return 1;
}

static inline char *
escape_colons(const char *str)
{
   char *ret;
   int len = strlen(str);
   int colons = 0;
   int i, j;
   for (i = 0; i < len; i++)
      if (str[i] == ':') colons++;
   ret = mem_malloc((len + colons + 1) * sizeof(char));
   for (i = 0, j = 0; i < len + 1; i++, j++)
     {
        if (str[i] == ':') ret[j++] = '\\';
        ret[j] = str[i];
     }
   return ret;
}
#endif /* _WIN32 */
#endif /* WIN32HEAD */
