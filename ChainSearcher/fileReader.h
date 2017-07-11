#ifndef __FileReader__
#define __FileReader__


#include "utils.h"


class CFileReader
{
public:
  CFileReader();

  bool OpenFile(int argc_, char** argv_);//!< Открывает файл, переданный как аргумент командной строки
  bool ReadNumber(uint64_t& number_);

private:

  void GetStatisticInfo();
  void ShowProgress();

  int m_currentPosition;
  int m_prevPercent;

  int m_fileSize;
  char* m_argv;

};


#endif