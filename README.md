# Bares (Basic ARithmetic Expression Evaluator based on Stacks)
O programa BARES (Basic ARithmetic Expression Evaluator based on Stacks) deve ser
capaz de receber expressões aritméticas simples, formadas por:
* constantes num´ericas inteiras (-32.768 a 32.767);
* operadores (+, -, /, *,ˆ, %), com precedˆencia descrita em Tabela 1; e
* parênteses.

| Precedência | Operadores | Associação | Descrição                                        |
|-------------|------------|------------|--------------------------------------------------|
|      1      |      ^     |      <-    | Potenciação ou Exponenciação                     |
|      2      |    * / %   |      ->    | Multiplicação, divisão, resto da divisão inteira |
|      3      |     + -    |      ->    | Adição, subtração                                |

Segue abaixo exemplos de expressões válidas para o bares:
* 35 - 3 * (-2 + 5)^2
* 54 / 3 ^ (12%5) * 2
* ((2-3)*10 - (2^3*5))
* ---3

## Autores
* Carlos Eduardo Azevedo dos Santos
* Wattson Jose Saenz Perales