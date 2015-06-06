////////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

#include <windows.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TimerClass
////////////////////////////////////////////////////////////////////////////////
namespace ZShadeSandboxGlobal {
	class Timer
	{
	public:
		Timer();
		Timer(const Timer&);
		~Timer();

		bool Initialize();
		void Frame();

		float GetTime();

		INT64 ElapsedSeconds() const;
		float ElapsedSecondsF() const;
		double ElapsedSecondsD() const;
		INT64 DeltaSeconds() const;
		float DeltaSecondsF() const;
		double DeltaSecondsD() const;

		INT64 ElapsedMilliseconds() const;
		float ElapsedMillisecondsF() const;
		double ElapsedMillisecondsD() const;
		INT64 DeltaMilliseconds() const;
		float DeltaMillisecondsF() const;
		double DeltaMillisecondsD() const;

		INT64 ElapsedMicroseconds() const;
		float ElapsedMicrosecondsF() const;
		double ElapsedMicrosecondsD() const;
		INT64 DeltaMicroseconds() const;
		float DeltaMicrosecondsF() const;
		double DeltaMicrosecondsD() const;

		float ConstantTime() const;

	private:
		INT64 m_frequency;
		float m_ticksPerMs;
		INT64 m_startTime;
		float m_frameTime;
		float m_elapsedTime;

		//Constant flowing time in one direction for water
		float m_ConstantTime;

		INT64 startTime;

		INT64 frequency;
		double frequencyD;

		INT64 elapsed;
		INT64 delta;

		float elapsedF;
		float deltaF;

		double elapsedD;
		double deltaD;

		INT64 elapsedSeconds;
		INT64 deltaSeconds;

		float elapsedSecondsF;
		float deltaSecondsF;

		double elapsedSecondsD;
		double deltaSecondsD;

		INT64 elapsedMilliseconds;
		INT64 deltaMilliseconds;

		float elapsedMillisecondsF;
		float deltaMillisecondsF;

		double elapsedMillisecondsD;
		double deltaMillisecondsD;

		INT64 elapsedMicroseconds;
		INT64 deltaMicroseconds;

		float elapsedMicrosecondsF;
		float deltaMicrosecondsF;

		double elapsedMicrosecondsD;
		double deltaMicrosecondsD;
	};
}

#endif