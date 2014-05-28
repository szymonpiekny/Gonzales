
TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG += qt
CONFIG += warn_off

SOURCES += \
    ../../GonzalesApp/teaching/teacher.cpp \
    "../../GonzalesApp/qa/QAsSelection/QAsToLearnProvider.h" \
    TeacherTestSuite.cpp

HEADERS += ../gtest.h \
        ../gmock.h \
    ../../GonzalesApp/IUIAdapters.h \
    ../../GonzalesApp/teaching/Teacher.h \
    PresentersMock.h \
    ../../GonzalesApp/common/qtproxies/DataDeserializerFactory.h \
    ../../GonzalesApp/qa/QAsSelection/QAsToLearnProvider.cpp \
    ../../GonzalesApp/common/qtproxies/DataDeserializer.h \
    ../../GonzalesApp/common/SimpleTree/Node.h \
    QAsToLearnProviderMock.h

LIBPATH += C:/gtest/lib
LIBPATH += C:/gmock/lib

LIBS += -lgtest
LIBS += -lgmock
