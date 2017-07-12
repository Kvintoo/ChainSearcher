#include <iostream>
#include <sys/stat.h>
#include "fileReader.h"


CFileReader::CFileReader()
{

}

CFileReader::CFileReader(int argc_, char** argv_) :
m_currentPosition(0),
m_prevPercent(0),
m_fileSize(0)
{
  OpenFile(argc_, argv_);
}

bool CFileReader::OpenFile(int argc_, char** argv_)
{
  if ((argc_ < 1) || (!argv_))
  {
    return false;
  }

  if ((argc_ < 2))
  {
    std::cout << "Enter file name to analyze.";
    return false;
  }

  
  m_is.open(argv_[1], std::ios::binary);
  if (!m_is)
  {
    std::cerr << "Could not open file.\n";
    return false;
  }

  m_argv = argv_[1];

  GetStatisticInfo();

  return true;
}

void CFileReader::GetStatisticInfo()
{
  struct stat fi;
  stat(m_argv, &fi);
  m_fileSize = static_cast<long>(fi.st_size);
}

void CFileReader::ShowProgress()
{
  int currentPercent = m_currentPosition * NUMBER_SIZE * 100 / m_fileSize;
  if (currentPercent - m_prevPercent > 5)//сообщаем о прогрессе чтения, если считали больше цифр, 
  {                                      //чем 5% от предыдущего выведенного значения прогресса
    std::cout << currentPercent << "% red\n";
    m_prevPercent = currentPercent;
  }
}

bool CFileReader::ReadNumber(uint64_t& number_)
{
  m_is.read((char*)&number_, NUMBER_SIZE);
  std::cout << number_ << "\n";
  auto count = m_is.gcount();
  if (m_is.gcount() != NUMBER_SIZE)
    return false;

  // показать прогресс чтения файла
  ShowProgress();

  ++m_currentPosition;
  return true;
}