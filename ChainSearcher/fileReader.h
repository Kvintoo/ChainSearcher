#ifndef __FileReader__
#define __FileReader__


#include "utils.h"
#include <fstream>


class CFileReader
{
public:
  CFileReader();
  CFileReader(int argc_, char** argv_);

  bool OpenFile(int argc_, char** argv_);//!< Открывает файл, переданный как аргумент командной строки
  bool ReadNumber(uint64_t& number_);

private:

  void GetStatisticInfo();
  void ShowProgress();

  int m_currentPosition;
  int m_prevPercent;

  int m_fileSize;
  char* m_argv;

  std::ifstream m_is;

};


#endif