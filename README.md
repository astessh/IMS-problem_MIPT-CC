## Обзор
В проекте рассмотрена задача [**NP**-полная](https://en.wikipedia.org/wiki/NP-completeness) задача [**IMS**](https://en.wikipedia.org/wiki/Identical-machines_scheduling) - задача о поиске оптимальной последовательности выполения разных задач на идентичных машинах. Более подробное описание см. в [paper.pdf](paper.pdf).

## Описание работы
 - Приведена формальная постановка, доказана **NP**-полнота задачи **IMS**;
 - Описан [алгоритм Грэма](https://mathweb.ucsd.edu/~ronspubs/69_02_multiprocessing.pdf) приближения решения задачи, приведена возможная реализация на языке C++;
 - Практически подтверждена степень приближения, проведен анализ скорости работы алгоритма по сравнению с наивным алгоритмом поиска точного решения.

## Запуск
### Требования
- C++20 или выше.

### Компиляция
```bash
mkdir build
cd build
cmake ..
make
cd ..
rm -r build
```
### Запуск
```bash
./bin/runner
```
Результы тестирования находятся в файлах `test_approximation.csv`, `test_time_2_machines.csv`, `test_time_3_machines.csv`.
