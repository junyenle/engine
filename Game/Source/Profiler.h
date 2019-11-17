#pragma once
#define PROFILE_SCOPE(name) \
Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))

class Profiler
{
private:
	Profiler();
	~Profiler();
	class Timer
	{
		friend Profiler;
	public:
		void Start()
		{
			mStart = std::chrono::high_resolution_clock::now();
			char buf[128];
			sprintf_s(buf, 128, "{\"name\": \"%s\", \"ph\": \"B\", \"ts\": %lld, \"pid\": 1, \"tid\": 1},\n", mName.c_str(), (long long)(mStart.time_since_epoch().count() / 1000));
			Profiler::Get()->chromeString.append(buf);
		};
		void Stop()
		{
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			double nS = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStart).count();
			mCurrent_ms = nS / 1000000;
			char buf[128];
			sprintf_s(buf, 128, "{\"name\": \"%s\", \"ph\": \"E\", \"ts\": %lld, \"pid\": 1, \"tid\": 1},\n", mName.c_str(), (long long)(end.time_since_epoch().count() / 1000));
			Profiler::Get()->chromeString.append(buf);
		}
		void Reset()
		{
			mTotalTimes_ms += mCurrent_ms;
			mNumSample++;
			mMax_ms = max(mMax_ms, mCurrent_ms);
			mCurrent_ms = 0;
		};
		const std::string &GetName() const
		{
			return mName;
		};
		double GetTime_ms() const
		{
			return mCurrent_ms;
		};
		double GetMax_ms() const
		{
			return mMax_ms;
		};
		double GetAvg_ms() const
		{
			return mTotalTimes_ms / mNumSample;
		};
	private:
		std::string mName;
		double mCurrent_ms = 0;
		double mMax_ms = 0;
		double mTotalTimes_ms = 0;
		int mNumSample = 0;
		std::chrono::high_resolution_clock::time_point mStart;
		Timer()
		{
		};
		~Timer()
		{
		};

	};
	std::unordered_map<std::string, Timer*> mTimers;
	FILE* chromeFile;
public:
	std::string chromeString;
	class ScopedTimer
	{
		friend Profiler;
	public:
		ScopedTimer(Timer* timer)
		{
			mTimer = timer;
			timer->Start();
		};
		~ScopedTimer()
		{
			mTimer->Stop();
		};
	private:
		Timer* mTimer;
	};
	static Profiler* Get();
	Timer* GetTimer(const std::string &name);
	void ResetAll();

};

