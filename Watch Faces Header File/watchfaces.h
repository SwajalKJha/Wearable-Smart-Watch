#ifndef watchfaces_h
#define watchfaces_h

class wfclass
{
public:
	void setup(void);
	void loop();
	void updateNTPTime();
	void updateClock();
	void drawAnalogClock(int hh, int mm, int ss, int dayOfWeek);
	void drawDigitalClockWithDate(int hh, int mm, int ss, int dayOfMonth, int dayOfWeek);
	void drawWatchFace(int hh, int mm, int ss, int dayOfMonth, int dayOfWeek);
	void buttonPressed();
};

extern wfclass watchfaces;

#endif
