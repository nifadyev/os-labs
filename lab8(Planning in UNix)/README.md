# Лабораторная работа №8. Планирование.

## Постановка задачи
-----------------

Требуется создать программу, выполняющую построение диаграммы выполнения потоков в многозадачной среде.

Строка запуска программы: `./program infile outfile`

Входной файл имеет текстовый формат и имеет следующую структуру:

1. Первая строка содержит одно целое число `1 <= N <= 100` - число потоков.
2. Далее следует N пар строк, описывающих диаграммы выполнения потоков в однозадачной среде.
    * Первая строка пары содержит число временных интервалов выполнения потока `T[i], i=1..N`.
    * Вторая строка содержит строку из T[i] символов, представляющая диаграмму. Символ "0" означает, что на данном
интервале поток находился в состоянии "Выполнение". Символ "-" означает, что поток на данном интервале
находился в состоянии "Ожидание".
3. Программа должна выполнить  чтение входных данных, сформировать диаграмму выполнения потоков при использовании
алгоритма планирования `FCFS` и сохранить ее в выходной файл. 
    * Предполагается, что если в конце временного интервала произошли события с несколькими потоками (завершение 
ввода-вывода, завершение кванта и т.д.), то сначала произошло событие первого потока, затем второго и т.д.
    * Выходной файл имеет текстовый формат. Каждая строка содержит (N + 1) поле. В первом поле выводится номер временного
интервала. В последующих полях выводятся состояния потоков на этом интервале. Символ "0" означает, что на данном
интервале поток находился в состоянии "Выполнение". Символ "." означает, что поток на данном интервале находился
в состоянии "Готов к выполнению". Символ "|" означает, что поток на данном интервале находился в состоянии "Ожидание".
Символ "x" означает, что к данному интервалу поток завершил выполнение.
    * После диаграммы должны быть приведены вычисленные значения характеристик "Пропускная способность", "Оборотное время",
"Эффективность", "Время ожидания", "Количество переключений контекста". Характеристики должны быть выведены в виде
дробей.

### Пример входного файла.

```txt
2
10
000--00--0
5
0-0-0
```
### Пример выходного файла

```txt
1	0	 .
2	0	.
3	0	.
4	|	0
5	|	|
6	0	.
7	0	.
8	|	0
9	|	|
10	0	.
11	x	0

Пропускная способность = 2 / 11  
Оборотное время = (10 + 11) / 2  
Эффективность = 9 / 11  
Время ожидания = (2 + 1 + 0 + 0) / 4  
Количество переключений контекста = 5  
```
