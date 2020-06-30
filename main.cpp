#include<iostream>
#include<vector>
#include "readData.h"
#include <string>
#include <random>
#include <bits/stdc++.h>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <filesystem>
#include <dirent.h>
#include <string.h>
//namespace fs = std::filesystem;

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

struct{
	int posicaoVertice1;
	int posicaoVertice2;
}typedef tSwap;

bool comparaCusto(const custoDeInsercao &dado1, const custoDeInsercao &dado2){
  return dado1.custo < dado2.custo;
}

int CustoDaSolucao(vector<int> solucao){
	int custo = 0;

	for(int i = 0; i < solucao.size()-1; i++){
		custo += matrizAdj[solucao[i]][solucao[i+1]];
		//cout << "custo de :"  << solucao[i] << " " << solucao[i+1]  << ": "<< custo << " " << matrizAdj[solucao[i]][solucao[i+1]] << "\n";
	}

	cout << endl << "custo total : " << custo << endl;

	return custo;
}

void PrintSolucao(vector<int> solucao){
	cout << "solucao: " << endl;

	for(int i = 0; i < solucao.size(); i++){
		cout << solucao[i] << " ";
	}

	cout << endl;
}

int Construtivo(vector<int> &solucao, int inicio, float alpha){
	vector<int> listaDecandidatos;
	int custoDaSolucao = 0;

	for(int i = 1; i <= dimension; i++){
		if(i == inicio){
			continue;
		}

		listaDecandidatos.push_back(i);
	}

	//solucao.emplace(solucao.begin(), inicio);
	//solucao.emplace(solucao.begin(), inicio);
	solucao.push_back(inicio);
	solucao.push_back(inicio);

	for(int i = 0; i < 3; i++){//coloca 3 nós aleatórios na solução
		int verticeEscolhido = rand() % listaDecandidatos.size();
		solucao.emplace(solucao.begin() + 1, listaDecandidatos[verticeEscolhido]);
		listaDecandidatos.erase(listaDecandidatos.begin() + verticeEscolhido);
		custoDaSolucao += matrizAdj[solucao[0]][solucao[1]] + matrizAdj[solucao[1]][solucao[2]] - matrizAdj[solucao[0]][solucao[2]];
	}

	custoDaSolucao += 1;

	int sizeListaDeCandidatos = listaDecandidatos.size();

	while(1){

		vector<custoDeInsercao> listaDecandidatosRestrita((solucao.size() - 2) * listaDecandidatos.size());

		for(int i = 0, j = 1, l = 0; i < solucao.size() - 2; i++, j++){
			for(int k : listaDecandidatos){
				listaDecandidatosRestrita[l].noInserido = k;
				listaDecandidatosRestrita[l].posicaoNaSolucao = j;
				listaDecandidatosRestrita[l].custo = matrizAdj[solucao[i]][k] + matrizAdj[solucao[j]][k] - matrizAdj[solucao[i]][solucao[j]];
				l++;
			}
		}




		sort(listaDecandidatosRestrita.begin(), listaDecandidatosRestrita.end(), comparaCusto);

		int verticeEscolhido = (int)(rand() % listaDecandidatosRestrita.size());
		//cout << listaDecandidatosRestrita.size() << " " << listaDecandidatosRestrita[verticeEscolhido].noInserido << " " << verticeEscolhido << " " << listaDecandidatos.size() << endl;

		solucao.emplace(solucao.begin() + listaDecandidatosRestrita[verticeEscolhido].posicaoNaSolucao, listaDecandidatosRestrita[verticeEscolhido].noInserido);
		custoDaSolucao += listaDecandidatosRestrita[verticeEscolhido].custo;

		listaDecandidatos.erase(remove(listaDecandidatos.begin(), listaDecandidatos.end(), listaDecandidatosRestrita[verticeEscolhido].noInserido), listaDecandidatos.end());
		sizeListaDeCandidatos --;

		//listaDecandidatosRestrita.clear();

		if(sizeListaDeCandidatos == 0){
			break;
		}

		// cout << "vet: " << listaDecandidatosRestrita[verticeEscolhido].noInserido << " vet escolhdo: " << verticeEscolhido << " posicao: " << listaDecandidatosRestrita[verticeEscolhido].posicaoNaSolucao << endl;

		listaDecandidatosRestrita.clear();
	}



	return custoDaSolucao;
}

int Swap(vector<int> &solucao, int custoInicial){
	int custoAtual = custoInicial, custoNaIteracao = 0, custoRetiradoparcial = 0;
	int numeroDeIteracoes = dimension - 2, numeroDeSwaps = dimension - 1;
	bool ehMelhor = false;
	tSwap verticesATrocar;

	//cout << "entrei terminei swao com custo: " << custoInicial << endl;

	for(int i = 1; i < dimension; i++){
		custoRetiradoparcial = - matrizAdj[solucao[i-1]][solucao[i]] - matrizAdj[solucao[i+1]][solucao[i]];
		for(int j = i + 2; j < dimension; j++){
			custoNaIteracao = matrizAdj[solucao[j]][solucao[i-1]] + matrizAdj[solucao[j]][solucao[i+1]] + matrizAdj[solucao[i]][solucao[j-1]] + matrizAdj[solucao[i]][solucao[j+1]]
							+ custoRetiradoparcial - matrizAdj[solucao[j-1]][solucao[j]] - matrizAdj[solucao[j+1]][solucao[j]];

			if((custoInicial + custoNaIteracao) < custoAtual){
				verticesATrocar.posicaoVertice1 = i;
				verticesATrocar.posicaoVertice2 = j;

				custoAtual = custoNaIteracao + custoInicial;

				ehMelhor = true;
			}
		}
	}



	if(ehMelhor){
		int aux = solucao[verticesATrocar.posicaoVertice1];

		solucao[verticesATrocar.posicaoVertice1] = solucao[verticesATrocar.posicaoVertice2];
		solucao[verticesATrocar.posicaoVertice2] = aux;
	}

	// PrintSolucao(solucao);
	// CustoDaSolucao(solucao);
	// cout << "custo no fim do swap: " << custoAtual << endl;
	return custoAtual;
}

int Reinsercao(vector<int> &solucao, int custoInicial, int tamanhoDoBloco){
	int custoAtual = custoInicial, custoRetirado = 0, custoInserido = 0;
	int numeroDeIteracoes =  dimension - tamanhoDoBloco -1, numeroDeSwaps = dimension - 1 - tamanhoDoBloco;
	bool ehMelhor = false;
	tSwap posicoes;

	//vector<int> vertices(tamanhoDoBloco);


	//cout << "entrei reinsercao com susto: " << custoInicial << endl;

	for(int i = 1; i < numeroDeIteracoes; i++){
		custoRetirado = matrizAdj[solucao[i-1]][solucao[i+tamanhoDoBloco]] - matrizAdj[solucao[i-1]][solucao[i]] - matrizAdj[solucao[i+tamanhoDoBloco]][solucao[i+tamanhoDoBloco-1]];
		for(int j = i + tamanhoDoBloco; j < numeroDeSwaps; j++){
			custoInserido = - matrizAdj[solucao[j]][solucao[j+1]] + matrizAdj[solucao[i]][solucao[j]] + matrizAdj[solucao[i+tamanhoDoBloco-1]][solucao[j+1]];
			if((custoInicial + custoRetirado + custoInserido) < custoAtual){
				posicoes.posicaoVertice1 = i; //de onde retira
				posicoes.posicaoVertice2 = j+1; //onde coloca

				custoAtual = custoRetirado + custoInserido + custoInicial;

				//vertices.insert(vertices.begin(), solucao.begin()+i, solucao.begin()+i+tamanhoDoBloco);

				ehMelhor = true;
			}
		}
	}


	if(ehMelhor){
		if(tamanhoDoBloco == 1){
			solucao.insert(solucao.begin() + posicoes.posicaoVertice2, solucao[posicoes.posicaoVertice1]);
			solucao.erase(solucao.begin() + posicoes.posicaoVertice1);
		}else{
			solucao.insert(solucao.begin() + posicoes.posicaoVertice2, solucao.begin() + posicoes.posicaoVertice1, solucao.begin() + posicoes.posicaoVertice1 + tamanhoDoBloco);
			solucao.erase(solucao.begin() + posicoes.posicaoVertice1, solucao.begin() + posicoes.posicaoVertice1 + tamanhoDoBloco);
		}
	}

	// PrintSolucao(solucao);
	// CustoDaSolucao(solucao);
	// cout << "custo no fim do reinsercao: " << custoAtual << endl;

	return custoAtual;
}

int DoisOpt(vector<int> &solucao, int custoInicial){
	int custoAtual = custoInicial, custoRetirado = 0, custoInserido = 0;
	bool ehMelhor = false;
	tSwap posicoes;

	//vector<int> vertices(tamanhoDoBloco);


	// cout << "entrei 2opt com custo: " << custoInicial << endl;
	// PrintSolucao(solucao);

	for(int i = 1; i < dimension; i++){
		custoRetirado = - matrizAdj[solucao[i -1]][solucao[i]];
		for(int j = i + 2; j < dimension; j++){
			custoInserido = - matrizAdj[solucao[j]][solucao[j+1]] + matrizAdj[solucao[i]][solucao[j+1]] + matrizAdj[solucao[j]][solucao[i-1]];

			if((custoInicial + custoRetirado + custoInserido) < custoAtual){
				posicoes.posicaoVertice1 = i; //comeco da sequencia
				posicoes.posicaoVertice2 = j+1; //fim da sequencia

				custoAtual = custoRetirado + custoInserido + custoInicial;

				//vertices.insert(vertices.begin(), solucao.begin()+i, solucao.begin()+i+tamanhoDoBloco);

				ehMelhor = true;
			}
		}
	}

	if(ehMelhor){
		reverse(solucao.begin()+posicoes.posicaoVertice1, solucao.begin()+posicoes.posicaoVertice2);
	}
	// PrintSolucao(solucao);
	// CustoDaSolucao(solucao);
	// cout << "custo no fim do 2opt: " << custoAtual << endl;
	return custoAtual;
}

int RVND(vector<int> &solucao, int custoDaSolucao){
	int i;
	int custoAtual = 0, custoDaSolucaoParcial = custoDaSolucao;
	vector<int> solucaoParcial;
	vector<int> vizinhanca = {1, 2, 3, 4, 5};
	int tamanhoDaSolucao = solucao.size(), tamanhoVizinhanca = vizinhanca.size(), vizinhancaEscolhida = 0;

	// cout << "\t entrei ni rvnd" << endl;
	// PrintSolucao(solucao);

	// for(i = 0; i < tamanhoDaSolucao; i++){
	// 	solucaoParcial.push_back(solucao[i]);
	// }
	solucaoParcial = solucao;

	custoAtual = custoDaSolucao;

	while(1){
		vizinhancaEscolhida = (int)(rand() % (vizinhanca.size()));
		//cout << "\t entrando co case:" << vizinhanca[vizinhancaEscolhida] << endl;

		if(vizinhanca[vizinhancaEscolhida] == 1){
			custoAtual = Reinsercao(solucaoParcial, custoAtual, 1);
		}else if(vizinhanca[vizinhancaEscolhida] == 2){
			custoAtual = Swap(solucaoParcial, custoAtual);
		}else if(vizinhanca[vizinhancaEscolhida] == 3){
			custoAtual = DoisOpt(solucaoParcial, custoAtual);
		}else if(vizinhanca[vizinhancaEscolhida] == 4){
			custoAtual = Reinsercao(solucaoParcial, custoAtual, 2);
		}else if(vizinhanca[vizinhancaEscolhida] == 5){
			custoAtual = Reinsercao(solucaoParcial, custoAtual, 3);
		}
		// switch(vizinhanca[vizinhancaEscolhida]){
		// 	case 1:
		// 		//cout << "rnvd comco reinserca:"<< custoDaSolucao  << endl;
		// 		custoAtual = Reinsercao(solucaoParcial, custoAtual, 1);
		// 		// cout<< "custo atual rein: " << custoAtual << endl;
		// 		// CustoDaSolucao(solucaoParcial);
		// 		// cout << "revnd fim reinerca" << endl;
		// 		break;

		// 	case 2:
		// 		//cout << "rvnd coemco swap: " << custoDaSolucao <<  endl;
		// 		custoAtual = Swap(solucaoParcial, custoAtual);
		// 		// cout<< "custo atual swa: " << custoAtual << endl;
		// 		// CustoDaSolucao(solucaoParcial);
		// 		// cout << "revnd fim swap" << endl;
		// 		break;

		// 	case 3:
		// 		//cout << "rvnd comeco dois:" << custoDaSolucao << endl;
		// 		custoAtual = DoisOpt(solucaoParcial, custoAtual);
		// 		// cout<< "custo atual dois: " << custoAtual << endl;
		// 		// CustoDaSolucao(solucaoParcial);

		// 		//cout << "rvns fim dois" << endl;
		// 		break;

		// 	case 4:
		// 		//cout << "rnvd comco reinserca:"<< custoDaSolucao  << endl;
		// 		custoAtual = Reinsercao(solucaoParcial, custoAtual, 2);
		// 		// cout<< "custo atual rein: " << custoAtual << endl;
		// 		// CustoDaSolucao(solucaoParcial);
		// 		// cout << "revnd fim reinerca" << endl;
		// 		break;

		// 	case 5: 
		// 		//cout << "rnvd comco reinserca:"<< custoDaSolucao  << endl;
		// 		custoAtual = Reinsercao(solucaoParcial, custoAtual, 3);
		// 		// cout<< "custo atual rein: " << custoAtual << endl;
		// 		// CustoDaSolucao(solucaoParcial);
		// 		// cout << "revnd fim reinerca" << endl;
		// 		break;


		// 	default:
		// 		break;
		// }

		if(custoAtual < custoDaSolucao){
			custoDaSolucao = custoAtual;

			solucao.clear();
			solucao = solucaoParcial;

			vizinhanca.clear();
			vizinhanca.reserve(5);
			for(i = 0; i < tamanhoVizinhanca; i++){
				vizinhanca.push_back(i+1);
			}
		}else{
			solucaoParcial = solucao;
			custoAtual = custoDaSolucao;
			// PrintSolucao(solucao);
			// cout << endl << endl;
			// PrintSolucao(solucaoParcial);
			// getchar();
			vizinhanca.erase(vizinhanca.begin() + vizinhancaEscolhida);
		}

		// cout << "vizinhanca no rvnd comeco com vizinhanca escolhda " << vizinhancaEscolhida << " " << vizinhanca[vizinhancaEscolhida] <<  endl;
		// PrintSolucao(vizinhanca);
		// cout << "vizinhanca no rvnd fim " << endl;
		if(vizinhanca.size() == 0){
				break;
		}
	}

	return custoDaSolucao;
}

vector<int> DoubleBridge(vector<int> solucao1, int &custoDasolucaoFinal, int custoDasolucao){
	int tamanhoSubsequencia = (dimension)/4;
	vector<int> solucao(solucao1);

	// cout << "double brigde dentro da funcao comeca: " << tamanhoSubsequencia << " " << custoDasolucao << endl;
	// PrintSolucao(solucao);
	// CustoDaSolucao(solucao);


	int custoInicial = matrizAdj[solucao[tamanhoSubsequencia - 1]][solucao[tamanhoSubsequencia]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 2 - 1]][solucao[(tamanhoSubsequencia * 2)]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 3 - 1]][solucao[tamanhoSubsequencia * 3]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 4 - 1]][solucao[tamanhoSubsequencia * 4]];

	//cout << solucao[tamanhoSubsequencia - 1] << " " << solucao[tamanhoSubsequencia] << " " << solucao[tamanhoSubsequencia * 2 - 1] << solucao[(tamanhoSubsequencia * 2)] << " ";
	//cout << solucao[tamanhoSubsequencia * 3 - 1] << " " << solucao[tamanhoSubsequencia * 3] << " " << solucao[tamanhoSubsequencia * 4 - 1] << " " << solucao[tamanhoSubsequencia * 4] << endl;
	vector<int> subSequenica1;
	vector<int> subSequenica2;

	subSequenica1.reserve(tamanhoSubsequencia);
	subSequenica2.reserve(tamanhoSubsequencia);

	subSequenica1.insert(subSequenica1.begin(), solucao.begin() + (tamanhoSubsequencia), solucao.begin()+(tamanhoSubsequencia + tamanhoSubsequencia));
	subSequenica2.insert(subSequenica2.begin(), solucao.begin() + (tamanhoSubsequencia * 3), solucao.begin()+(tamanhoSubsequencia * 3 + tamanhoSubsequencia));

	solucao.erase(solucao.begin() + (tamanhoSubsequencia), solucao.begin()+(tamanhoSubsequencia + tamanhoSubsequencia));
	solucao.erase(solucao.begin() + (tamanhoSubsequencia * 2), solucao.begin()+(tamanhoSubsequencia * 2 + tamanhoSubsequencia));
	solucao.insert(solucao.begin() + tamanhoSubsequencia, subSequenica2.begin(), subSequenica2.end());
	solucao.insert(solucao.begin() + tamanhoSubsequencia * 3, subSequenica1.begin(), subSequenica1.end());

	int custoFinal = matrizAdj[solucao[tamanhoSubsequencia - 1]][solucao[tamanhoSubsequencia]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 2 - 1]][solucao[tamanhoSubsequencia * 2]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 3 - 1]][solucao[tamanhoSubsequencia * 3]]
					 + matrizAdj[solucao[tamanhoSubsequencia * 4 - 1]][solucao[tamanhoSubsequencia * 4]];

	custoFinal = custoFinal - custoInicial + custoDasolucao;

	custoDasolucaoFinal = custoFinal;

	// PrintSolucao(solucao);
	// CustoDaSolucao(solucao)
	return solucao;
}

int GILS(vector<int> &solucaoFinal, int Imax, int Iils){
	random_device rd;
  	mt19937 mt(rd());
  	uniform_real_distribution<float> linear_f(0.0, 0.5);
	float alpha;
	
	cout << "entrei" << endl;
	int iterIls = 0;

	int custoDaSolucaoFinal = 800000, custoDaSolucaoParcial = 1000000, custoDaSolucao = 800000;

	vector<int> solucaoParcial, solucao;

	for(int i = 0; i < Imax; i++){
		alpha = linear_f(mt);
		//cout << "comeco do do ils" << endl;
		solucao.clear();
		custoDaSolucao = Construtivo(solucao, 1, alpha);
		solucaoParcial.clear();
		solucaoParcial = solucao;
		custoDaSolucaoParcial = custoDaSolucao;
		//cout << "depois do construtico" << endl;

		iterIls = 0;
		while(iterIls < Iils){
			custoDaSolucao = RVND(solucao, custoDaSolucao);

			// cout << "\t\tsai do rvnd dentro do ils " << custoDaSolucao << endl;
			// PrintSolucao(solucao);
			// CustoDaSolucao(solucao);
			// cout << "custo da solucao parcial " << custoDaSolucaoParcial << endl;
			// PrintSolucao(solucaoParcial);
			// CustoDaSolucao(solucaoParcial);

			if(custoDaSolucao < custoDaSolucaoParcial){
				solucaoParcial.clear();
				solucaoParcial = solucao;
				// cout << "\t\t\t\t\tif do GUILS" << endl;
				// PrintSolucao(solucaoParcial);
				// CustoDaSolucao(solucaoParcial);
				// cout << "\t\t\t\t\tcusto da solucao " << custoDaSolucao;
				// cout << "\t\t\t\t\tfim do if GILS" << endl;
				
				custoDaSolucaoParcial = custoDaSolucao;

				iterIls = 0;
			}
			// cout << "\t xomeco double vbrisge: " << custoDaSolucao << endl;
			// PrintSolucao(solucaoParcial);
			// CustoDaSolucao(solucaoParcial);
			solucao.clear();
			solucao = DoubleBridge(solucaoParcial, custoDaSolucao, custoDaSolucaoParcial); //função já atualiza o custo da solucao
			// PrintSolucao(solucao);
			// CustoDaSolucao(solucao);
			// cout <<  "fim do double cridge" << custoDaSolucao << endl;

			iterIls++;
			//getchar();
		}

		if(custoDaSolucaoParcial < custoDaSolucaoFinal){
			solucaoFinal.clear();
			solucaoFinal = solucaoParcial;
			custoDaSolucaoFinal = custoDaSolucaoParcial;
		}
		//cout << "Fim do for" << endl;
	}


	return custoDaSolucaoFinal;
}

int main( int argc, char *argv[ ] ){
	
	ofstream arquivoSaida;
	//ofstream arqsaida;
 	arquivoSaida.open("resultados.csv", ios::out);
	//arqsaida.open( "result.csv" , ios::out );
	if (arquivoSaida.is_open()) { cout << "arquivo aberto" << endl;/* ok, proceed with output */ }

	
	char arquivoIgnorado[] = ".tsp";

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("C:\\Users\\'Lays\\Documents\\codigo\\tsp\\tspAtual\\arquivos")) != NULL) {
	/* print all the files and directories within directory */
	while ((ent = readdir (dir)) != NULL) {
		printf ("%s\n", ent->d_name);

		if(strcmp(ent->d_name, arquivoIgnorado) < 0){
			cout << strcmp(ent->d_name, arquivoIgnorado) << endl;
			continue;
		}

		char *nomeArquivo[] = {ent->d_name, ent->d_name};
		
		readData(argc, nomeArquivo, &dimension, &matrizAdj);
		cout << "dimendio: " << dimension << endl;
		vector<int> solucao;
		solucao.reserve(dimension);

		//int custoDaSolucao;
		int custoDaSolucao = GILS(solucao, 50, 140);

		cout << "custo da solucao " << custoDaSolucao <<  endl;

		//arqsaida << "Cabecalho do arquivo" << endl;
		//arquivoSaida << "gravei" << endl;
		arquivoSaida << ent->d_name << ";" << custoDaSolucao << endl;
		

		solucao.erase(solucao.begin(), solucao.end());
	}
	closedir (dir);
	} else {
	/* could not open directory */
	perror ("");
	return EXIT_FAILURE;
	}

	arquivoSaida.close();

	readData(argc, argv , &dimension, &matrizAdj);
		printf ("%s\n", argv[1]);
	
	 vector<int> solucao;
	 solucao.reserve(dimension);

	//vector<int> solucao{1, 2, 14, 3, 4, 5, 6, 12, 7, 13, 8, 11, 10, 9, 1};


	//printData();
	//getchar();

	//int custoDaSolucao = Construtivo(solucao, 1, 0.5);
	//cout << "Aqui " << endl;


	//int custoDaSolucao = CustoDaSolucao(solucao);
	//cout << "custo da solucao antes do double : " << custoDaSolucao << endl;
	//DoisOpt(solucao, custoDaSolucao);

	//Swap(solucao, custoDaSolucao);
	//Reinsercao(solucao, custoDaSolucao, 2);
	//solucao = DoubleBridge(solucao, custoDaSolucao);
	

	// RVND(solucao, custoDaSolucao);
	// RVND(solucao, custoDaSolucao);

	// RVND(solucao, custoDaSolucao);
	// RVND(solucao, custoDaSolucao);


	int custoDaSolucao = GILS(solucao, 10, 10);
	cout << "Aqui " << endl;


	// for(int i = 0; i < solucao.size(); i++){
	// 	cout << solucao[i] << " ";
	// }

	cout << "custo quando sai da construcao: " << custoDaSolucao << endl;


	//CustoDaSolucao(solucao);
	cout << "sai" << endl;

	return 0;
}
