#ifndef __Chain__
#define __Chain__
#include "utils.h"


class CChain
{
public:

  CChain(uint64_t firstElem_, int position_);


  uint64_t GetFirstElem() const;
  void SetFirstElem(const uint64_t firstElement_);

  uint64_t GetLastElem() const;
  void SetLastElem(const uint64_t lastElement_);

  int GetFirstElemPosition() const;
  void SetFirstElemPosition(const int firstElemPosition_);

  int GetLastElemPosition() const;
  void SetLastElemPosition(const int lastElemPosition_);

  int GetSize() const;
  void SetSize(const int size_);

  bool operator< (const CChain& chain_) const;


private:

  uint64_t m_firstElem;
  uint64_t m_lastElem;

  int m_firstElemPosition;
  int m_lastElemPosition;

  int m_size;
};



#endif