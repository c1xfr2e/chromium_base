// Copyright (c) 2010 The wandoujia Authors. All rights reserved.
#include "base/utf_string_conversions.h"
#include <Windows.h>
#include <malloc.h>
#include "sys_string_conversions.h"

// UTF-8 <-> Wide --------------------------------------------------------------

// bool WideToUTF8(const wchar_t* src, size_t src_len, std::string* output) {
//   int buffer_size = ::WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, false);
//   int bytes_written = 0;
//   if (buffer_size > 0) {
//     output->resize(buffer_size - 1);
//     char* result = const_cast<char*>(output->c_str());
//     bytes_written = ::WideCharToMultiByte(CP_UTF8, 0, src, -1, result, buffer_size - 1, NULL, false);
//     if (bytes_written != buffer_size) {
//       return false;
//     } else {
//       return true;
//     }
//   }
//   return false;
// }

std::string WideToUTF8(const std::wstring& wide) {
  return base::SysWideToUTF8(wide);
}

// bool UTF8ToWide(const char* src, size_t src_len, std::wstring* output) {
//   int buffer_size = ::MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
//   int bytes_written = 0;
//   if (buffer_size > 0) {
//     output->resize(buffer_size - 1);
//     wchar_t* result = const_cast<wchar_t*>(output->c_str());
//     bytes_written = ::MultiByteToWideChar(CP_UTF8, 0, src, -1, result, buffer_size - 1);
//     if (bytes_written != buffer_size) {
//       return false;
//     } else {
//       return true;
//     }
//   }
//   return false;
// }

std::wstring UTF8ToWide(const base::StringPiece& utf8) {
  return base::SysUTF8ToWide(utf8);
}
