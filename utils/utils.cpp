#include "utils.h"

QString Utils::formatMillisecondsToDateString(long milliseconds) {
    int seconds = milliseconds / 1000.;

    int secondsInt = seconds%60;
    QString secondsStr;
    if (secondsInt < 10) secondsStr = "0";
    secondsStr = secondsStr + QString::number(secondsInt);

    int minutesInt = (seconds%3600) / 60;
    QString minutesStr;
    if (minutesInt < 10) minutesStr = "0";
    minutesStr = minutesStr + QString::number(minutesInt);

    int hoursInt = seconds / 3600;
    if (hoursInt >= 1) {
        QString hoursStr = QString::number(hoursInt);
        return hoursStr + ":" + minutesStr + ":" + secondsStr;
    } else {
        return  minutesStr + ":" + secondsStr;
    }

}
