#include "chain.h"


CChain::CChain(uint64_t firstElem_, int position_) :
m_firstElem(firstElem_),
m_lastElem(0),
m_firstElemPosition(position_),
m_lastElemPosition(0),
m_size(1)
{

}

uint64_t CChain::GetFirstElem() const
{
  return m_firstElem;
}

void CChain::SetFirstElem(const uint64_t firstElement_)
{
  m_firstElem = firstElement_;
}

uint64_t CChain::GetLastElem() const
{
  return m_lastElem;
}

void CChain::SetLastElem(const uint64_t lastElement_)
{
  m_lastElem = lastElement_;
}

int CChain::GetFirstElemPosition() const
{
  return m_firstElemPosition;
}

void CChain::SetFirstElemPosition(const int firstElemPosition_)
{
  m_firstElemPosition = firstElemPosition_;
}

int CChain::GetLastElemPosition() const
{
  return m_lastElemPosition;
}

void CChain::SetLastElemPosition(const int lastElemPosition_)
{
  m_lastElemPosition = lastElemPosition_;
}

int CChain::GetSize() const
{
  return m_size;
}

void CChain::SetSize(const int size_)
{
  m_size = size_;
}

bool CChain::operator< (const CChain& chain_) const
{
  //   Меньше тот, у которого
  //     - длина меньше
  //     - меньше первый элемент
  //     - смещение первого элемента больше

  if (m_size < chain_.m_size)
    return true;

  else if (m_size == chain_.m_size)
  {
    if (m_firstElem < chain_.m_firstElem)
      return true;
    else if (m_firstElem == chain_.m_firstElem)
    {
      return m_firstElemPosition > chain_.m_firstElemPosition;
    }
    return false;
  }

  return false;
}
