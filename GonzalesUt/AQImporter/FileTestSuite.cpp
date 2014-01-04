#include <iostream>
#include "../gtest.h"

#include <QQueue>
#include <QStringList>

#include "../../GonzalesApp/file/fileserializer.h"
#include "../../GonzalesApp/file/filedeserializer.h"
#include "../../GonzalesApp/file/file.h"

using namespace testing;

class FileTestSuite : public testing::Test
{
protected:
    void SetUp()
    {
        ExistingFile.setFileName("C:/projects/Gonzales/GonzalesUt/AQImporter/toimport.txt");
        NotExistingFile.setFileName("WrongFileName.txt");
        toImportWithOneEmpty.setFileName("C:/projects/Gonzales/GonzalesUt/AQImporter/toImportWithOneEmpty.txt");
    }


    File ExistingFile;
    File NotExistingFile;
    File toImportWithOneEmpty;
};

TEST_F(FileTestSuite, testFileOpen)
{
    ASSERT_TRUE(ExistingFile.open(QFile::ReadOnly));
}

TEST_F(FileTestSuite, testCantOpenFile)
{
    ASSERT_FALSE(NotExistingFile.open(QFile::ReadOnly));
}

TEST_F(FileTestSuite, readOneLineFromFile)
{
    ExistingFile.open(QFile::ReadOnly);

    QString line = ExistingFile.readLine();
    EXPECT_TRUE(!line.isEmpty());
}

TEST_F(FileTestSuite, readTwoLineFromFile)
{
    ExistingFile.open(QFile::ReadOnly);

    QString currentLine = ExistingFile.readLine();
    QStringList lines;

    while(!currentLine.isEmpty())
    {
        lines.append(currentLine);
        currentLine = ExistingFile.readLine();
    };

    EXPECT_EQ(2, lines.size());
}

TEST_F(FileTestSuite, readTwoLinewsWhenOneBetweenIsEmpty)
{
    toImportWithOneEmpty.open(QFile::ReadOnly);

    QString currentLine = toImportWithOneEmpty.readLine();
    QStringList lines;

    while(!currentLine.isEmpty())
    {
        lines.append(currentLine);
        currentLine = toImportWithOneEmpty.readLine();
    };

    EXPECT_EQ(3, lines.size());
}

TEST_F(FileTestSuite, saveDataInFile)
{
    File fileToWrite("C:/projects/Gonzales/GonzalesUt/AQImporter/fileToWrite.txt");
    fileToWrite.open(QFile::WriteOnly);
    QIODevice *deviceToWrite = fileToWrite.getIODevice();
    std::unique_ptr<CanSerializeData> serializer(new FileSerializer(deviceToWrite));
    std::string str="testing string";
    int a = 64;
    float b = 56.45;

    serializer->serialize(str.c_str());
    serializer->serialize(a);
    serializer->serialize(b);
    ASSERT_TRUE(serializer->status()== QDataStream::Ok);
}

TEST_F(FileTestSuite, loadDataFromFile)
{
    File fileToRead("C:/projects/Gonzales/GonzalesUt/AQImporter/fileToWrite.txt");
    fileToRead.open(QFile::ReadOnly);
    QIODevice *deviceToWrite = fileToRead.getIODevice();
    std::unique_ptr<CanDeserializeData> deserializer(new FileDeserializer(deviceToWrite));
    char *str;
    int a = 0;
    float b = 0;

    deserializer->deserialize(str);
    deserializer->deserialize(a);
    deserializer->deserialize(b);

    std::string strToCheck(str);

    EXPECT_EQ("testing string", strToCheck);
    EXPECT_EQ(64, a);
    EXPECT_FLOAT_EQ(56.45, b);
    ASSERT_TRUE(deserializer->status()== QDataStream::Ok);
}

TEST_F(FileTestSuite, loadToNotEnoughDataFromFile)
{
    File fileToRead("C:/projects/Gonzales/GonzalesUt/AQImporter/fileToWrite.txt");
    fileToRead.open(QFile::ReadOnly);
    QIODevice *deviceToWrite = fileToRead.getIODevice();
    std::unique_ptr<CanDeserializeData> deserializer(new FileDeserializer(deviceToWrite));
    char *str;
    int a = 0;
    float b = 0;
    float c = 0;

    deserializer->deserialize(str);
    deserializer->deserialize(a);
    deserializer->deserialize(b);
    EXPECT_EQ(QDataStream::Ok, deserializer->status());
    deserializer->deserialize(c);
    EXPECT_EQ(QDataStream::ReadPastEnd, deserializer->status());

    std::string strToCheck(str);

    EXPECT_EQ("testing string", strToCheck);
    EXPECT_EQ(64, a);
    EXPECT_FLOAT_EQ(56.45, b);
    ASSERT_FALSE(deserializer->status()== QDataStream::Ok);
}
