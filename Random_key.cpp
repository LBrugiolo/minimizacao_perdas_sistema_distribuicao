#include "Random_keys.h"

using namespace std;

bool ordenacaoIndividuo(Individuo *i1, Individuo *i2){return i1->getPerdaAtiva() > i2->getPerdaAtiva();}

Random_keys::Random_keys(int tamPop, int numGeracoes){
    this->tamPop = tamPop;
    this->numGeracoes = numGeracoes;
}

void Random_keys::geraPopAleatoria(Grafo *g){

    Individuo *ind;
    for(int i=0; i<this->tamPop; i++){

        ind = new Individuo(g->getNumeroArcos());
        ind->geraPesosAleatorios();

        popAtual.push_back(ind);
    }
    popAnterior = popAtual;
}

/** ordena populacao em ordem decrescente por valor da funcao objetivo
dado que queremos minimizar a perda os piores individuos ficam no inicio(perda maior)
queremos os melhore (menor perda, fim da lista) **/
void Random_keys::ordenaPopulacaoAtual(Grafo *g){
    for(int i=0; i<popAtual.size(); i++)
        popAtual.at(i)->calculaFuncaoObjetivo(g);

    sort(popAtual.begin(), popAtual.end(), ordenacaoIndividuo);
}

void Random_keys::avancaGeracoes(Grafo *g){

    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
        e ordena a populacao da maior perda(pior individuo)
        pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        Individuo *best = popAtual.at(this->tamPop-1);

        printf("\ngeracao (%d)  melhor individuo: %f kw",
        k, 100*1000*best->getPerdaAtiva());//resultado ja em kw

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;
            int pai2 = rand() % this->tamPop;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

            popAnterior.at(pai1)->cruzamentoMedia(popAnterior.at(pai2), popAtual.at(i));
            popAtual.at(i)->mutacao();
        }
    }

}
