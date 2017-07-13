#include <set>
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

bool IsPrimeNumber(std::mt19937_64& generator_, uint64_t number_)
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

  const int MAX_INT = 2147483647;

  //для простоты зададим k = 5. Далее k можно вычислять в зависимости от длины числа
  for (int i = 1; i <= 5; ++i)
  {
    //Выбрать случайное целое число a в отрезке[2, number_ − 2]
    int generatedNumber = 0;
    if (number_ - 2 > MAX_INT)
    {
      std::uniform_int_distribution<> distribution(2, MAX_INT);
      generatedNumber = distribution(generator_);// (a)
    }
    else
    {
      std::uniform_int_distribution<> distribution(2, static_cast<int>(number_ - 2));
      generatedNumber = distribution(generator_);// (a)
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

void SaveChain(CChain& chain, uint64_t lastAnalyzeNumber, int counter)
{
  if (chain.IsInit())
  {
    chain.SetLastElem(lastAnalyzeNumber);
    chain.SetLastElemPosition(counter);
  }
}

void ChooseWinnerChain(CChain& winner_, CChain& candidat_)
{
  //или цепочка-кандидат длиннее
  if (winner_.GetSize() < candidat_.GetSize())
  {
    winner_ = candidat_;
    return;
  }
  
  //при равной длине у цепочки-кандидата 1-й элемент должен быть больше
  bool sameSizeFirstElemMore = winner_.GetSize() == candidat_.GetSize() && 
                               winner_.GetFirstElem() < candidat_.GetFirstElem();

  //смещение не проверяем, т.к. читаем файл сначала и у следующей цепочки смещение первого элемента всегда больше
  if (sameSizeFirstElemMore)
  {
      winner_ = candidat_;
  }
}

int main(int argc, char *argv[])
{
  CFileReader fileReader(argc, argv);
  if (!fileReader.IsInitialized())
  {
    std::system("PAUSE");
    return 0;
  }

  CChain winnerChain;//цепочка-победитель (результат работы программы)
  int currentNumberPositon = 0;
  bool haveCurentChain = false;

  CChain currentChain;//текущая формируемая цепочка
  uint64_t lastAnalyzeNumber = 0;
  int lastAnalyzeNumberPosition = 0;

  uint64_t testNumber = 0;
  std::random_device randomDevice;
  std::mt19937_64 generator(randomDevice());

  while (fileReader.ReadNumber(testNumber))//пока есть значение
  {
    fileReader.ShowProgress();//показываем прогресс чтения файла

    //если число простое
    if (IsPrimeNumber(generator, testNumber))
    {
      //проверяем, новое значение - продолжение текущей цепочки или начало новой
      //значение текущее должно быть больше последнего в цепочке

      //нет цепочки или тестируемое число меньше или равно предыдущему - создаём цепочку
      if (!haveCurentChain || testNumber <= lastAnalyzeNumber)
      {
        SaveChain(currentChain, lastAnalyzeNumber, lastAnalyzeNumberPosition);
        ChooseWinnerChain(winnerChain, currentChain);

        currentChain.Init(testNumber, currentNumberPositon);
        lastAnalyzeNumber = testNumber;
        lastAnalyzeNumberPosition = currentNumberPositon;
        haveCurentChain = true;
      }
      //есть цепочка - увеличиваем её длину на 1, запоминаем последнее проанализированное чиcло и его позицию
      else
      {
        currentChain.IncrementSize();

        lastAnalyzeNumber = testNumber;
        lastAnalyzeNumberPosition = currentNumberPositon;
      }
    }
    ++currentNumberPositon;
  }

  //сохраним последнюю созданную цепочку
  SaveChain(currentChain, lastAnalyzeNumber, lastAnalyzeNumberPosition);
  ChooseWinnerChain(winnerChain, currentChain);

  //не найдено ни одной цепочки
  if (!winnerChain.IsInit())
  {
    std::cout << "No chains founded." << "\n";
    std::system("PAUSE");
    return 1;
  }

  //вывести параметры результирующей цепочки
  std::cout << "\nFirst element " << winnerChain.GetFirstElem() << "\n"
    << "First element shift " << winnerChain.GetFirstElemPosition() << "\n"
    << "Last element " << winnerChain.GetLastElem() << "\n"
    << "Last element shift " << winnerChain.GetLastElemPosition() << "\n"
    << "Chain size " << winnerChain.GetSize() << "\n";

  std::system("PAUSE");
  return 0;
}

