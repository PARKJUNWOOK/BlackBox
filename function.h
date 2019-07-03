#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include "subMain.c"
#include "function.h"


char* getCurrentTime(char typeFlag);
boolean createFolder(char* folderName);
boolean deleteFolder(char* folderName); // only Dir delete
int delDirFile(const char* folder);     // file/dir delete
int getRepoSize();

