TEMPLATE = app

QT       += core gui

CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11
CONFIG += warn_off

HEADERS += ../gtest.h \
           ../gmock.h \
    FileMock.h \
    DataSerializerFactoryMock.h \
    DataDeserializerMock.h \
    DataDeserializerFactoryMock.h \
    ../../GonzalesApp/qa/qaio/QASaver.h \
    ../../GonzalesApp/qa/qaio/QALoader.h \
    ../../GonzalesApp/qa/qaio/QAImporter.h \
    ../../GonzalesApp/qa/QA.h \
    ../../GonzalesApp/common/qtproxies/IFile.h \
    ../../GonzalesApp/common/qtproxies/DataSerializer.h \
    ../../GonzalesApp/common/qtproxies/FileFactory.h \
    ../../GonzalesApp/common/qtproxies/DataDeserializer.h \
    ../../GonzalesApp/common/qtproxies/File.h \
    DataSerializerMock.h \
    ../../GonzalesApp/common/SimpleTree/Node.h \
    ../../GonzalesApp/common/SimpleTree/Utils.h \
    ../../GonzalesApp/common/FileException.h \
    DataSerializerFactoryMock.h \
    DataSerializerMock.h \
    DataDeserializerFactoryMock.h \
    DataDeserializerMock.h \
    QAsFilePathProviderMock.h \
    LineSplitterMock.h \
    ../../GonzalesApp/qa/IQA.h \
    QANodeSerializerMock.h \
    ../../GonzalesApp/common/qtproxies/IFileFactory.h

SOURCES += \
    QAImporterTestSuite.cpp \
    FileTestSuite.cpp \
    QALoaderTestSuite.cpp \
    QASaverTestSuite.cpp \
    ../../GonzalesApp/qa/qaio/QASaver.cpp \
    ../../GonzalesApp/qa/qaio/QALoader.cpp \
    ../../GonzalesApp/qa/qaio/QAImporter.cpp \
    AllTests.cpp \
    ../../GonzalesApp/qa/QA.cpp

unix
{
    LIBS += -pthread
}
win32
{
    LIBPATH += C:/gtest/lib
    LIBPATH += C:/gmock/lib
}

LIBS += -lgtest
LIBS += -lgmock

OTHER_FILES += \
    toimport.txt \
    toImportWithOneEmpty.txt \
    fileToWrite.txt

