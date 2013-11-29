// Copyright (c) 2012 The Wandoujia Authors. All rights reserved.
#include <objbase.h>
#include "base/time.h"
#include "base/file_path.h"
#include "base/path_service.h"
using namespace base;

int main() {
  unsigned int i = 4294967295;
  int ii = i;

  FilePath executableDir;
  PathService::Get(base::DIR_MODULE, &executableDir);

  base::Time::Exploded exploded = {0};
  exploded.year = 2013;
  exploded.month = 11;
  exploded.day_of_month = 18;
  exploded.hour = 0;
  exploded.minute = 0;
  int64 now_20131118_00 = base::TimeDelta::FromMicroseconds(base::Time::FromLocalExploded(exploded).ToInternalValue()).InMilliseconds();

  exploded.day_of_month = 19;
  int64 now_20131119_00 = base::TimeDelta::FromMicroseconds(base::Time::FromLocalExploded(exploded).ToInternalValue()).InMilliseconds();

  int64 d = now_20131119_00 - now_20131118_00;

  int d_hour = base::TimeDelta::FromMilliseconds(d).InHours();

  return 0;
} 
