# GRAPHY

## Como rodar
Para rodar o programa atualmente basta executar o comando 'make' ou 'mingw32-make'

## Compilador
Compilador padrao = clang++  
Caso precise mudar basta acessar o Makefile e alterar a primeira linha para 'CXX = g++'

Seria bom ser capaz de automatizar o reconhecimento da existencia do compilador...  
mas eu não sei fazer isso, porque o comando é diferente no windows e no linux, então tem uma certa complexidade

## Todo
1 > Fazer a leitura do grafo de um arquivo .txt

2 > Colocar a saida do programa em outro arquivo _.txt

3 > Automatizar para a execução do programa funcionar a partir de args -in=nome.txt

4 > Gerar grafos automaticamente com o comando -generate -vertex=? -edges=? -out=?

### Exemplos
./Graphy -in=a.txt  
(automaticamente ele vai criar um arquivo chamado a_.txt)

./Graphy -in=a.txt -out=b.txt

./Graphy -generate -vertex=100 -edgex=400 -out=a.txt

## Obs:
**feito com as boas praticas de c++ e usando o padrão de biblioteca**
