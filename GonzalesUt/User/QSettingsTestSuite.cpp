#include <iostream>
#include <stdexcept>
#include <QString>
#include "../gtest.h"
#include "../../GonzalesApp/user/accountcreator.h"
#include "applicationsettingsmock.h"
#include "../../GonzalesApp/common/qtproxies/applicationsettings.h"
#include <QStringList>
#include <algorithm>

using namespace testing;


// only for experiments
//TEST(QSettingsTestSuite, should_retriveChildGropEntries)
//{
//    QSettings settings(AppSettingsGlobals::g_Company, AppSettingsGlobals::g_Project);
//    settings.beginGroup("Users");
//    QStringList entries = settings.childGroups();
//}
