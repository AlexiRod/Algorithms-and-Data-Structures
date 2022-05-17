# Дерево выражений

### Описание

Реализация дерева выражений со следующим функционалом:

1. Создать дерево выражений в соответствии с вводимым выражением. Структура узла дерева включает: информационная часть узла – символьного типа: знак операции +, -, * или цифра; указатель на левое и указатель на правое поддерево. В дереве выражения операнды выражений находятся в листьях дерева.
* Исходное выражение имеет формат:
<формула>::=цифра|<формула><знак операции><формула>
Примеры: 5; 1+2; 1+2+3*4-5/6.
Отобразить дерево на экране, повернув его против часовой стрелки.
2. Построить дерево выражения по префиксной форме арифметического выражения, в котором операнды одно буквенные идентификаторы и знаки арифметических операций.
Вывести построенное дерево прямым обходом в глубину.