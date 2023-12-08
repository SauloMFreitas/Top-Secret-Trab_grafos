# GRAPHY

## Como rodar

Para rodar o algoritmo de Edmonds, basta rodar o comando `make run` na pasta `functional_edmonds`.

## Todo
1 > Colocar a saida do programa em arquivo *.txt

2 > Automatizar para a execução do programa funcionar a partir de args -in=nome.txt

3 > Gerar grafos automaticamente com o comando -generate -vertex=? -edges=? -out=?

### Exemplos
./Graphy -in=a.txt  
(automaticamente ele vai criar um arquivo chamado a_.txt)

./Graphy -in=a.txt -out=b.txt

./Graphy -generate -vertex=100 -edgex=400 -out=a.txt

## Obs:
A implementação funcional foi adaptada de um [código em haskell do mesmo algoritmo](https://github.com/prokls/edmonds-branching-algorithm/). Várias partes do algoritmo são mantidas com a mesma lógica da implementação de onde são baseadas.

A pasta `original` contém uma implementação não finalizada do algoritmo. Não é garantido que conseguirá o resultado esperado.


## Referências
https://github.com/prokls/edmonds-branching-algorithm/
