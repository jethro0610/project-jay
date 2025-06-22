#pragma once
#include <cmath>
#include <string>
class Clock {
public:
    enum TimeOfDay {
        Midnight,
        Dawn,
        Sunrise,
        Morning,
        Afternoon,
        Sunset,
        Dusk,
        Night, 
    };

    Clock() {
        time_ = 0;
        day_ = 0;
        SetTimeOfDay(Dawn);
    }
    static constexpr int TIME_IN_DAY = 1024 * 64;
    static constexpr int TIME_IN_DAY_SECTION = TIME_IN_DAY / 8;
    int time_;
    int day_;

    void AddTime(int amount = 1) { 
        time_ += amount; 
        while (time_ >= TIME_IN_DAY) {
            time_ -= TIME_IN_DAY;
            day_++;
        }
    }

    void Reset() {
        *this = Clock();
    }

    TimeOfDay GetTimeOfDay() {
        return (TimeOfDay)std::floor(
            ((float)time_ / TIME_IN_DAY) * 8
        );
    }

    void SetTimeOfDay(TimeOfDay tod, float percent = 0.0) {
        time_ = tod * TIME_IN_DAY_SECTION + TIME_IN_DAY_SECTION * percent;
    }

    std::string GetTimeOfDayString() {
        TimeOfDay tod = GetTimeOfDay();
        switch (tod) {
            case Midnight:
                return "Midnight";

            case Dawn:
                return "Dawn";

            case Sunrise:
                return "Sunrise";

            case Morning:
                return "Morning";

            case Afternoon:
                return "Afternoon";

            case Sunset:
                return "Sunset";

            case Dusk:
                return "Dusk";

            case Night:
                return "Night";
        }
        return "Invalid";
    }
};
