/*
 * Timer.h
 *
 *  Created on: 20 juin 2013
 *      Author: dosimont
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <cstdlib>
#include <sys/time.h>

class Timer {

	private:
		timeval timeQ1, timeQ2, timeBC1, timeBC2, timeBP1, timeBP2;

	public:
		Timer();
		void startQualityTimer();
		void stopQualityTimer();
		void startBestCutTimer();
		void stopBestCutTimer();
		void startBestPartitionTimer();
		void stopBestPartitionTimer();
		int getQualityDuration(); //ms
		int getBestCutDuration(); //ms
		int getBestPartitionDuration(); //ms
		virtual ~Timer();
};

#endif /* TIMER_H_ */
