#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <unordered_set>

std::string getNextString(const std::string &s) {
    std::string result = s;
    int i = result.size() - 1;

    // Incrementa a última letra
    while (i >= 0 && result[i] == 'Z') {
        result[i] = 'A'; // Volta para 'A' e continua a incrementar a próxima letra
        i--;
    }

    if (i < 0) {
        result = "A" + result; // Adiciona uma nova letra na frente se todas as letras são 'Z'
    } else {
        result[i]++; // Incrementa a letra atual
    }

    return result;
}

std::vector<std::string> generateStrings(int n) {
    std::vector<std::string> strings;
    std::string currentString = "A";

    for (int i = 0; i < n; ++i) {
        strings.push_back(currentString);
        currentString = getNextString(currentString);
    }

    return strings;
}


int getRandomNumber(int min, int max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}


int main(int argc, char const *argv[])
{
    int graphCount = 1;
    int edgeCount = 0;
    int vertexCount = 0;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-generate") {
            // Código para a opção generate
        } else if (arg.find("-vertex=") == 0) {
            std::string vertex = arg.substr(8); // Pegar o número após "-vertex="
            vertexCount = std::stoi(vertex);
            // Código para lidar com a quantidade de vértices
        } else if (arg.find("-edges=") == 0) {
            std::string edges = arg.substr(7); // Pegar o número após "-edges="
            edgeCount = std::stoi(edges);
            // Código para lidar com a quantidade de arestas
        } else if (arg.find("-graphs=") == 0) {
            std::string graphs = arg.substr(8); // Pegar o número após "-vertex="
            graphCount = std::stoi(graphs);
        }

    }

    if (vertexCount-1 > edgeCount || edgeCount > vertexCount*(vertexCount-1))
    {
        std::cout << "Grafo inválido: o número de arestas não é válido"<< std::endl;
        return -1;
    }

    std::ofstream file("meuarquivo.txt"); // Abre o arquivo para escrita

    if (file.is_open())
    {
        file << graphCount << std::endl;
        for (int i = 0; i < graphCount; i++)
        {
            int numVertexs = getRandomNumber(1,vertexCount);
            file << numVertexs << std::endl;
            std::vector<std::string> vertexs = generateStrings(numVertexs);
            for (int v = 0; v < numVertexs; v++)
            {
                file << vertexs[v] << " ";
            }
            file << std::endl;

            int root = getRandomNumber(0, numVertexs-1);
            file << vertexs[root] << std::endl;

            int edges = 0;
            do
            {
                edges = getRandomNumber(numVertexs-1,edgeCount);
            }while(numVertexs-1 > edges || edges > numVertexs*(numVertexs-1));
            file << edges << std::endl;
            for (int e = 0; e < edges; e++)
            {
                int v1 = 0,v2 = 0;
                do
                {
                    v1 = getRandomNumber(0,numVertexs-1);
                    v2 = getRandomNumber(0,numVertexs-1);
                } while (v1==v2);
                int peso = getRandomNumber(1, 20);
                file << vertexs[v1] << " " << vertexs[v2] << " " << peso << std::endl;
            }
            

        }
        

    }
    
    
    return 0;
}

