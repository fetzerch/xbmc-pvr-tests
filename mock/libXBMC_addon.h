#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <stdarg.h>

#include <gmock/gmock.h>

namespace ADDON
{
  typedef enum addon_log
  {
    LOG_DEBUG,
    LOG_INFO,
    LOG_NOTICE,
    LOG_ERROR
  } addon_log_t;

  typedef enum queue_msg
  {
    QUEUE_INFO,
    QUEUE_WARNING,
    QUEUE_ERROR
  } queue_msg_t;

  class CHelper_libXBMC_addon
  {
  public:
    CHelper_libXBMC_addon()
    {
    }

    ~CHelper_libXBMC_addon()
    {
    }

    bool RegisterMe(void *Handle)
    {
      (void)Handle;
      return true;
    }

    void Log(const addon_log_t loglevel, const char *format, ... )
    {
      char buffer[16384];
      va_list args;
      va_start (args, format);
      vsprintf (buffer, format, args);
      va_end (args);
      MockedLog(loglevel, buffer);
    }
    MOCK_METHOD2(MockedLog, void(const addon_log_t loglevel, char *buffer));

    MOCK_METHOD2(GetSetting, bool(const char* settingName, void *settingValue));

    void QueueNotification(const queue_msg_t type, const char *format, ... )
    {
      char buffer[16384];
      va_list args;
      va_start (args, format);
      vsprintf (buffer, format, args);
      va_end (args);
      MockedQueueNotification(type, buffer);
    }
    MOCK_METHOD2(MockedQueueNotification, void(const queue_msg_t type, char *buffer));

    MOCK_METHOD1(WakeOnLan, bool(const char* mac));
    MOCK_METHOD1(UnknownToUTF8, char*(const char* str));
    MOCK_METHOD1(GetLocalizedString, char*(int dwCode));
    MOCK_METHOD0(GetDVDMenuLanguage, char*());
    MOCK_METHOD1(FreeString, void(char* str));
    MOCK_METHOD2(OpenFile, void*(const char* strFileName, unsigned int flags));
    MOCK_METHOD2(OpenFileForWrite, void*(const char* strFileName, bool bOverWrite));
    MOCK_METHOD3(ReadFile, unsigned int(void* file, void* lpBuf, int64_t uiBufSize));
    MOCK_METHOD3(ReadFileString, bool(void* file, char *szLine, int iLineLength));
    MOCK_METHOD3(WriteFile, int(void* file, const void* lpBuf, int64_t uiBufSize));
    MOCK_METHOD1(FlushFile, void(void* file));
    MOCK_METHOD3(SeekFile, int64_t(void* file, int64_t iFilePosition, int iWhence));
    MOCK_METHOD2(TruncateFile, int(void* file, int64_t iSize));
    MOCK_METHOD1(GetFilePosition, int64_t(void* file));
    MOCK_METHOD1(GetFileLength, int64_t(void* file));
    MOCK_METHOD1(CloseFile, void(void* file));
    MOCK_METHOD1(GetFileChunkSize, void(void* file));
    MOCK_METHOD2(FileExists, bool(const char *strFileName, bool bUseCache));
    MOCK_METHOD2(StatFile, int(const char *strFileName, struct __stat64* buffer));
    MOCK_METHOD1(DeleteFile, bool(const char *strFileName));
    MOCK_METHOD1(CanOpenDirectory, bool(const char* strUrl));
    MOCK_METHOD1(CreateDirectory, bool(const char *strPath));
    MOCK_METHOD1(DirectoryExists, bool(const char *strPath));
    MOCK_METHOD1(RemoveDirectory, bool(const char *strPath));
  };
}

