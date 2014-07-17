#pragma once
#include "../../../QA.h"
#include <set>
#include <deque>

typedef unsigned Day;

class QARepeatPeriodChecker
{
public:
    QARepeatPeriodChecker(std::set<Day> periods);
    bool ShouldBeRepeated(const std::map<QDate, QA::AnswerRating> &answerHistory) const;

private:
    QDate getDateSinceThereWasNoBadAnswer(const std::map<QDate, QA::AnswerRating> &answerHistory) const;
    bool ShouldBeRepeated_deprecated(std::map<QDate, QA::AnswerRating> answerHistory);
    Day currentPeriod(std::deque<Day> &periodsToCheck) const;
    Day currentMaxPeriod(std::deque<Day> periodsToCheck, Day currentPeriod)const;
    QDate currentPeriodDate(const QDate& nBA, Day currentPer) const;
    QDate currentMaxPeriodDate(const QDate& nBA, Day currentMaxPer, QDate currentDay)const;
    bool isHistoryEntryInRange(QDate currentPerDate, QDate currentMaxPerDate, std::map<QDate, QA::AnswerRating> answerHistory) const;
    QDate getPeriodDate(std::deque<Day> periodsToCheck);
    QDate getMaxPeriodDate(std::deque<Day> periodsToCheck);
    std::set<Day> periods;
};