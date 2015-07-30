/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */
#ifndef WIN32HEAD
#define WIN32HEAD
#ifdef _WIN32
static inline int
waitpid(DWORD pid, void *unused1 __UNUSED__, int unused2 __UNUSED__)
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
