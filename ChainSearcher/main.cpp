﻿#include <set>
#include <iostream>
#include <random>
#include "fileReader.h"
#include "chain.h"



uint64_t ModExp(uint64_t x_, uint64_t y_, uint64_t N_)
{
  if (y_ == 0)
    return 1;

  uint64_t z = ModExp(x_, y_ / 2, N_);
  if (y_ % 2 == 0)
    return (z*z) % N_;
  else
    return (x_*z*z) % N_;
}

void Factorization(uint64_t number_, int& power_, uint64_t& residue_)
{
  while (residue_ % 2 != 1)//пока остаток от деления нечётный
  {
    residue_ = number_ / 2;
    ++power_;
    number_ = residue_;
  }
}

bool NeedNewCycle(int power_, uint64_t mod_, uint64_t number_)
{
  int counter = 0;
  while (counter < power_ - 1)
  {
    //x < x^2 mod n
    mod_ = ModExp(mod_, (uint64_t)2, number_);// (x)
    if (mod_ == 1)
      return false;// результат - составное число
    if (mod_ == number_ - 1)
      return true;//нужна ещё одна итерация

    ++counter;
  }
  return false;// результат - составное число
}

bool IsPrimeNumber(uint64_t number_)
{
  //test Miller-Rabin

  if (number_ == 1 || number_ == 2 || number_ == 3)
    return true;

  if (number_ % 2 == 0)//делится нацело на 2
    return false;

  //Представить number_ − 1 в виде (2^s)·t, где t нечётно, можно сделать последовательным делением n - 1 на 2.
  int power = 0;//множитель (s)
  uint64_t residue = 0;//остаток (t)
  Factorization(number_ - 1, power, residue);

  //для простоты зададим k = 5. Далее k можно вычислять в зависимости от длины числа
  for (int i = 1; i <= 5; ++i)
  {
    //Выбрать случайное целое число a в отрезке[2, number_ − 2]
    std::random_device randomDevice;
    std::mt19937_64 generator(randomDevice());
    const int MAX_INT = 2147483647;
    int generatedNumber = 0;
    if (number_ - 2 > MAX_INT)
    {
      std::uniform_int_distribution<> distribution(2, MAX_INT);
      generatedNumber = distribution(generator);// (a)
    }
    else
    {
      std::uniform_int_distribution<> distribution(2, static_cast<int>(number_ - 2));
      generatedNumber = distribution(generator);// (a)
    }

    //x ← a^t mod n, вычисляется с помощью алгоритма возведения в степень по модулю
    uint64_t mod = ModExp(generatedNumber, residue, number_);// (x)

    if (mod == 1 || mod == number_ - 1)
      continue;

    if (NeedNewCycle(power, mod, number_))
      continue;
    else
      return false;
  }

  return true;
}

void SaveChain(CChain * pChain, uint64_t lastAnalyzeNumber, int counter, std::set<CChain> &chains)
{
  if (pChain)//сохраним текущую цепочку
  {
    pChain->SetLastElem(lastAnalyzeNumber);
    pChain->SetLastElemPosition(counter);

    chains.insert(*pChain);
    delete pChain;
    pChain = nullptr;
  }
}

int main(int argc, char *argv[])
{
  CFileReader fileReader;
  if (!fileReader.OpenFile(argc, argv))
  {
    std::system("PAUSE");
    return 0;
  }

  std::set<CChain> chains;
  int counter = 0;
  bool haveCurentChain = false;

  CChain *pChain = nullptr;
  uint64_t lastAnalyzeNumber = 0;
  int lastAnalyzeNumberPosition = 0;

  uint64_t testNumber = 0;
  while (fileReader.ReadNumber(testNumber))//пока есть значение
  {

    //если оно простое
    if (IsPrimeNumber(testNumber))
    {
      //проверяем, новое значение - продолжение текущей цепочки или начало новой
      //значение текущее должно быть больше последнего в цепочке

      if (!haveCurentChain || lastAnalyzeNumber >= testNumber)//нет цепочки или новое простое число меньше или равно предыдущему - создаём
      {
        SaveChain(pChain, lastAnalyzeNumber, lastAnalyzeNumberPosition, chains);


        pChain = new CChain(testNumber, counter);
        lastAnalyzeNumber = testNumber;
        lastAnalyzeNumberPosition = counter;
        haveCurentChain = true;
      }
      else//есть цепочка - увеличиваем её длину на 1, запоминаем последнее проанализированное чило
      {
        int currentSize = pChain->GetSize();
        ++currentSize;
        pChain->SetSize(currentSize);

        lastAnalyzeNumber = testNumber;
        lastAnalyzeNumberPosition = counter;
      }
    }
    ++counter;
  }

  //сохраним последнюю созданную цепочку
  SaveChain(pChain, lastAnalyzeNumber, lastAnalyzeNumberPosition, chains);

  //не найдено ни одной цепочки
  if (chains.size() == 0)
  {
    std::cout << "No chains founded." << "\n";
    std::system("PAUSE");
    return 1;
  }

  //выбрать результирующую цепочку по критериям
  auto pResultChain = std::prev(chains.end());
  std::cout << "First element " << pResultChain->GetFirstElem() << "\n"
    << "First element shift " << pResultChain->GetFirstElemPosition() << "\n"
    << "Last element " << pResultChain->GetLastElem() << "\n"
    << "Last element shift " << pResultChain->GetLastElemPosition() << "\n";

  std::system("PAUSE");
  return 0;
}

