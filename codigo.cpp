#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defina a estrutura de dados para produtos
struct Produto {
    int codigo;
    char descricao[100];
    char categoria[20];
    int quantidade;
    float preco;
    struct Produto* proximo;
};

// Defina a estrutura de dados para itens do carrinho
struct ItemCarrinho {
    int codigo;
    int quantidade;
    float precoTotal;
    struct ItemCarrinho* proximo;
};

// Função para carregar o estoque a partir do arquivo
struct Produto* carregarEstoque(const char* arquivo) {
	FILE* arquivoEstoque = fopen(arquivo, "r");
    if (arquivoEstoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return NULL;
    }

    struct Produto* inicioEstoque = NULL;
    struct Produto* ultimoProduto = NULL;

    while (!feof(arquivoEstoque)) {
        struct Produto* novoProduto = (struct Produto*)malloc(sizeof(struct Produto));
        if (novoProduto == NULL) {
            printf("Erro ao alocar memória para o produto.\n");
            break;
        }

        int lidos = fscanf(arquivoEstoque, "%d %99s %19s %d %f",
            &novoProduto->codigo, novoProduto->descricao, novoProduto->categoria,
            &novoProduto->quantidade, &novoProduto->preco);

        if (lidos != 5) {
            free(novoProduto);
            break;
        }

        novoProduto->proximo = NULL;

        if (inicioEstoque == NULL) {
            inicioEstoque = novoProduto;
            ultimoProduto = novoProduto;
        } else {
            ultimoProduto->proximo = novoProduto;
            ultimoProduto = novoProduto;
        }
    }

    fclose(arquivoEstoque);
    return inicioEstoque;
}


// Função para listar todos os produtos disponíveis no estoque
void listarProdutos(struct Produto* estoque) {
   printf("\nLista de Produtos Disponiveis:\n");
    printf("Codigo  | Descricao               | Categoria         | Quantidade | Preco\n");
    printf("--------------------------------------------------------------------------\n");

    struct Produto* atual = estoque;

    while (atual != NULL) {
        printf("%-7d | %-23s | %-17s | %-10d | %-6.2f\n", 
               atual->codigo, atual->descricao, atual->categoria, 
               atual->quantidade, atual->preco);    
        atual = atual->proximo;
    }
    printf("--------------------------------------------------------------------------\n");   
}

// Função para buscar produtos por categoria
void buscarPorCategoria(struct Produto* estoque,const char* categoria) {
	int encontrou;
		
	struct Produto* atual = estoque;

	
	while (atual != NULL) {
	  	
        if (strcmp(atual->categoria, categoria) == 0) {
        	
            printf("%-7d | %-23s | %-17s | %-10d | %-6.2f\n", 
                atual->codigo, atual->descricao, atual->categoria, 
                atual->quantidade, atual->preco);
                
            	encontrou = 1; 
        }
        atual = atual->proximo;
    }

    if (!encontrou) {
        printf("\nNao achou produtos na categoria '%s'\n", categoria);
    }

    printf("--------------------------------------------------------------------------\n");
	

}

//se retornar 1 achou se 0 não acho, ele recebe o endereço do inicio do estoque e no final esse endereço fica apontado o valor que parou
int buscarItemEstoquePeloCodigo(struct Produto ** aux, int codigo){
	while((*aux)!=NULL){
		if( (*aux)->codigo == codigo){
			return 1;
		}
		
		(*aux)=(*aux)->proximo;
	}
	return 0;
}

//se retornar 1 achou se 0 não acho, ele recebe o endereço do inicio do carrinho e no final esse endereço fica apontado o valor que parou
int temUmProdutoComEsseCodigoNoCarrinho(struct  ItemCarrinho ** aux, int codigo){
	while((*aux)!=NULL){
		if( (*aux)->codigo == codigo){
			return 1;
		}


		(*aux)=(*aux)->proximo;
	}
	
	return 0;
}

//retorna um novo item no carrinho
struct  ItemCarrinho *  criarNovoItemCarrinho(int codigo, int quantidade, float precoTotal){
	struct  ItemCarrinho * novoItem = (struct  ItemCarrinho *) malloc(sizeof(struct  ItemCarrinho * ));
	
	novoItem->codigo =  codigo;
    novoItem->quantidade = quantidade;
    novoItem->precoTotal = precoTotal;
	novoItem->proximo = NULL;
	
	return novoItem;
	
}


//essa função recebe o inicio do carrinho e um Item do carrinho e coloca no final
void inserirNoFinalCarrinho(struct  ItemCarrinho ** carrinho, struct  ItemCarrinho * novoItemCarrinho){

	if(*carrinho == NULL ) {
		*carrinho = novoItemCarrinho;
	}else{
		struct  ItemCarrinho * aux = *carrinho;
		while(aux->proximo!=NULL){
			aux=aux->proximo;
		}
		aux->proximo = novoItemCarrinho;
	}
	
}



// Função para adicionar produtos ao carrinho
void adicionarAoCarrinho( struct Produto* estoque, struct ItemCarrinho** carrinho, int codigo, int quantidade){
	struct  ItemCarrinho * aux = *carrinho;
	struct  ItemCarrinho * novoItemCarrinho;
	struct  Produto * aux_estoque = estoque;
	
	int achouEsseProdutoNoEstoque = buscarItemEstoquePeloCodigo(&aux_estoque, codigo);
	
	//verificar se existe o produto no carrinho com esse codigo
	if(temUmProdutoComEsseCodigoNoCarrinho(&aux, codigo)==1){
		//se existe incremente a quantidade ao produto	
		aux->quantidade += quantidade;
	}else{
		//senao crie um novo no com as informacoes que estao no estoque
		
		if(achouEsseProdutoNoEstoque == 1){
			
			//achou o produto no estoque, se a quantidade que quer inserir for maior que tem no estoque
			if(quantidade>aux_estoque->quantidade){
				//a quantidade que vai colocar no carrinho é o que está em estoque
				quantidade = aux_estoque->quantidade;
				
			}
			
			if(quantidade>0){
				novoItemCarrinho = criarNovoItemCarrinho(codigo, quantidade, aux_estoque->preco);
				inserirNoFinalCarrinho(carrinho,novoItemCarrinho);
				
			}else{
				printf("\n quantidade invalida");
			}
			
			
		}else{
			printf("\n Não existe produto com esse código");
		}
		
	}
	
}

// Função para visualizar o carrinho de compra
void visualizarCarrinho(struct ItemCarrinho* carrinho) {
    /*struct  ItemCarrinho * aux = carrinho;
	
	printf("\n===Carrinho==================================");
	while(aux!=NULL){
		printf("\nProduto codigo %d ; tem %d", aux->codigo, aux->quantidade);
		aux=aux->proximo;
	}
	
	
	TEM QUE MELHORAR 
	
	
	*/
}

// Função para editar o carrinho de compra
void editarCarrinho(struct ItemCarrinho** carrinho, int codigo, int novaQuantidade) {
     struct ItemCarrinho* atual = *carrinho;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
            if (novaQuantidade <= 0) {
                struct ItemCarrinho* itemRemovido = atual;
                *carrinho = (*carrinho)->proximo;
                free(itemRemovido);
                return;
            } else {
                atual->quantidade = novaQuantidade;
                atual->precoTotal = novaQuantidade * atual->precoTotal; 
                return;
            }
        }
        atual = atual->proximo;
    }

    printf("Produto com código %d nao encontrado no carrinho.\n", codigo);
}

// Função para efetuar a compra
void efetuarCompra(struct ItemCarrinho** carrinho, struct Produto** estoque) {
     
}

// Função para salvar o estoque no arquivo "estoque" antes de encerrar o programa
void salvarEstoque(const char* arquivo, struct Produto* estoque) {
  
}

int main() {
	
    const char* arquivoEstoque = "estoque.txt";
    struct Produto* estoque = carregarEstoque(arquivoEstoque);
    struct ItemCarrinho* carrinho = NULL;
    int opcao;

    do {
       // Exibir o menu de opções
        printf("\n-------------- MENU --------------\n");
        printf("1 - Listar todos os produtos\n");
        printf("2 - Buscar produto por categoria\n");
        printf("3 - Adicionar produtos ao carrinho\n");
        printf("4 - Visualizar carrinho de compra\n");
        printf("5 - Editar carrinho de compra\n");
        printf("6 - Efetuar a compra\n");
        printf("0 - Sair\n");
        printf("----------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);


        switch (opcao) {
            case 1:
                listarProdutos(estoque);
                break;
            case 2: {
                char categoria[20];
                printf("Digite a categoria desejada: ");
                scanf("%s", categoria);
                buscarPorCategoria(estoque, categoria);
                break;
            }
            case 3: {
                int codigo, quantidade;
                printf("Digite o código do produto: ");
                scanf("%d", &codigo);
                printf("Digite a quantidade desejada: ");
                scanf("%d", &quantidade);
                adicionarAoCarrinho(estoque, &carrinho, codigo, quantidade);
                break;
            }
            case 4:
                visualizarCarrinho(carrinho);
                break;
            case 5: {
                int codigo, novaQuantidade;
                printf("Digite o código do produto que deseja editar: ");
                scanf("%d", &codigo);
                printf("Digite a nova quantidade desejada: ");
                scanf("%d", &novaQuantidade);
                editarCarrinho(&carrinho, codigo, novaQuantidade);
                break;
            }
            case 6:
                efetuarCompra(&carrinho, &estoque);
                break;
            case 0:
                // Antes de encerrar o programa, salve o estoque no arquivo "estoque"
                salvarEstoque(arquivoEstoque, estoque);

                // Libere a memória alocada para as listas encadeadas
                // ...

                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // Antes de encerrar o programa, salve o estoque no arquivo "estoque"
    salvarEstoque(arquivoEstoque, estoque);

    // Libere a memória alocada para as listas encadeadas
    // ...

    return 0;
}
