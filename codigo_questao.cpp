#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int TAMANHO_TABELA = 1009;
const int TAMANHO_GRUPO = 6;

struct Celula {
    string grupo;
    int frequencia;
    Celula* proximo;

    Celula(string g) : grupo(g), frequencia(1), proximo(nullptr) {}
};

class TabelaDispersao {
private:
    vector<Celula*> estrutura;
    vector<string> ordemInsercao;

    int funcaoHash(const string& cadeia) {
        long resultado = 0, fator = 37, peso = 1;
        for (char letra : cadeia) {
            resultado = (resultado + (letra - 'A' + 1) * peso) % TAMANHO_TABELA;
            peso = (peso * fator) % TAMANHO_TABELA;
        }
        return resultado;
    }

public:
    TabelaDispersao() : estrutura(TAMANHO_TABELA, nullptr) {}

    void registrarGrupo(const string& cadeia) {
        int posicao = funcaoHash(cadeia);
        Celula* ponteiro = estrutura[posicao];

        while (ponteiro != nullptr) {
            if (ponteiro->grupo == cadeia) {
                ponteiro->frequencia++;
                return;
            }
            ponteiro = ponteiro->proximo;
        }

        Celula* nova = new Celula(cadeia);
        nova->proximo = estrutura[posicao];
        estrutura[posicao] = nova;
        ordemInsercao.push_back(cadeia);
    }

    void gerarArquivo(const string& nomeSaida) {
        ofstream arquivo(nomeSaida);
        if (!arquivo) {
            cerr << "Erro ao abrir arquivo de saída!\n";
            return;
        }

        arquivo << "Grupo\tFrequencia\n";
        for (const string& item : ordemInsercao) {
            int indice = funcaoHash(item);
            Celula* atual = estrutura[indice];

            while (atual != nullptr) {
                if (atual->grupo == item) {
                    arquivo << item << "\t" << atual->frequencia << "\n";
                    break;
                }
                atual = atual->proximo;
            }
        }

        arquivo.close();
        cout << "Resultado salvo em: " << nomeSaida << "\n";
    }

    ~TabelaDispersao() {
        for (auto inicio : estrutura) {
            while (inicio) {
                Celula* apagar = inicio;
                inicio = inicio->proximo;
                delete apagar;
            }
        }
    }
};

int main() {
    ifstream leitura("sequencia.txt");
    if (!leitura) {
        cerr << "Arquivo sequencia.txt não encontrado.\n";
        return 1;
    }

    string linha, conteudo;
    while (getline(leitura, linha)) {
        conteudo += linha;
    }
    leitura.close();

    TabelaDispersao dispersao;

    for (size_t i = 0; i + TAMANHO_GRUPO <= conteudo.size(); i += TAMANHO_GRUPO) {
        string trecho = conteudo.substr(i, TAMANHO_GRUPO);
        dispersao.registrarGrupo(trecho);
    }

    dispersao.gerarArquivo("resposta_genoma.txt");

    return 0;
}
