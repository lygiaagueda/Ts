#include<iostream>

using namespace std;

// Pega da instâncias
double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices

void printData(){
	int i, j;
	cout << "Dimension: " << dimension << endl;
	for(i = 1; i <= dimension; i++){
		for(j = 1; j <= dimension; j++){
			cout << matrizAdj[i][j] << " ";
		}
		cout << endl;
	}
}

struct{
	int noInserido;
	int posicaoNaSolucao;
	float custo;
}typedef custoDeInsercao;


int Construtivo(vector<int> &solucao, int inicio, float alpha){
	vector<int> listaDecandidatos;

	for(int i = 1; i <= dimension; i++){
		if(i == inicio){
			continue;
		}

		listaDecandidatos.push_back(i);
	}

	solucao.push_back(inicio)
	solucao.push_back(inicio)


	for(int i = 0; i < 3; i++){//coloca 3 nós aleatórios na solução
		int verticeEscolhido = rand() % listaDecandidatos.size();
		solucao.push_back(listaDecandidatos[verticeEscolhido]);
		listaDecandidatos.erase(listaDecandidatos.begin() + verticeEscolhido);
	}


	vector<custoDeInsercao> listaDecandidatosRestrita(solucao.size() * listaDecandidatos.size());

	for(int i = 0, j = 1, k = 0; i < solucao.size() - 2; i++, j++){
		for(int k : listaDecandidatos){
			listaDecandidatosRestrita[l].noInserido = k;
			listaDecandidatosRestrita[l].posicaoNaSolucao = i;
			listaDecandidatosRestrita[l].custo = matrizAdj[listaDecandidatos[i]][k] + matrizAdj[listaDecandidatos[j]][k] - matrizAdj[listaDecandidatos[i]][listaDecandidatos[j]];
		}
	}
}

int main(void){
	readData(argc, argv, &dimension, &matrizAdj);

	return 0;
}3