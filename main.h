#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <lrtypes.h>

#include <algorithm>

#include "ui.h"
#include "data.h"
#include "stockchart.h"

// helpers.cpp
void die(const char fmt[], ...);
void nukenewline(char buf[]);

#endif
