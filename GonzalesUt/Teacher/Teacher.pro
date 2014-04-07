
TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG += qt
CONFIG += warn_off

SOURCES += \
    ../../GonzalesApp/teaching/teacher.cpp \
    TeacherTestSuite.cpp

HEADERS += ../gtest.h \
        ../gmock.h \
    ../../GonzalesApp/IUIAdapters.h \
    ../../GonzalesApp/teaching/teacher.h \
    QasProviderMock.h \
    PresentersMock.h \
    ../../GonzalesApp/common/qtproxies/filedeserializerfactory.h \
    ../../GonzalesApp/common/qtproxies/filedeserializer.h

LIBPATH += C:/gtest/lib
LIBPATH += C:/gmock/lib

LIBS += -lgtest
LIBS += -lgmock
