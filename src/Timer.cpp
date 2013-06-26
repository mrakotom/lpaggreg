/*
 * Timer.cpp
 *
 *  Created on: 20 juin 2013
 *      Author: dosimont
 */

#include "Timer.h"

Timer::Timer()/*: timeQ1(NULL), timeQ2(0), timeBC1(0), timeBC2(0), timeBP1(0), timeBP2(0)*/ {
}

void Timer::startQualityTimer() {
	gettimeofday(&timeQ1,NULL);
}

void Timer::stopQualityTimer() {
	gettimeofday(&timeQ2, NULL);
}

void Timer::startBestCutTimer() {
	gettimeofday(&timeBC1,NULL);
}

void Timer::stopBestCutTimer() {
	gettimeofday(&timeBC2,NULL);
}

void Timer::startBestPartitionTimer() {
	gettimeofday(&timeBP1,NULL);
}

void Timer::stopBestPartitionTimer() {
	gettimeofday(&timeBP2,NULL);
}

int Timer::getQualityDuration() {
	return ((timeQ2.tv_sec-timeQ1.tv_sec)*1000)+((timeQ2.tv_usec-timeQ1.tv_usec)/1000);
}

int Timer::getBestCutDuration() {
	return ((timeBC2.tv_sec-timeBC1.tv_sec)*1000)+((timeBC2.tv_usec-timeBC1.tv_usec)/1000);
}

int Timer::getBestPartitionDuration() {
	return ((timeBP2.tv_sec-timeBP1.tv_sec)*1000)+((timeBP2.tv_usec-timeBP1.tv_usec)/1000);
}

Timer::~Timer() {
}

