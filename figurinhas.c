#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ALBUM_TAM 300
#define NOME_TAM 50
#define TEL_TAM 14
#define QTDE_FIG_PACOTE 5
#define REGISTRO_TAM 150


//Para usar o clear tanto em Linux quanto em Windows
#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif



/**********************Definição dos tipos de dados***************/

typedef struct Figura {
    int numero;
    struct Figura *next;
}Figura;


typedef struct Amigo {
    char nome[NOME_TAM + 1];
    char telefone[TEL_TAM + 1];
    int album[ALBUM_TAM + 1]; //Vetor de inteiros, se album[i] = 1, significa que a figura número i está no álbum
    Figura *repetidas[ALBUM_TAM + 1]; //Vetor de ponteiros, cada índice é o número da figura. Lista de listas
    struct Amigo *next;
}Amigo;

/*********************Prototipos*****************************/
int insereFiguraEspecifica(Amigo *amigo, int figuraNum);
void selecionaAmigosTroca(Amigo *head);
int popRepetida(Figura *repetidas[], int figuraNum);


/*************************Funções*********************************/

//Limpar o buffer de entrada após o scanf
void clearInputBuffer () {    
  while ( getchar() != '\n' );
}

//Remover a nova linha após o fgets
void removerNovaLinha(char *texto) {
    if(strlen(texto) > 0 && texto[strlen(texto) - 1] == '\n')
        texto[strlen(texto) - 1] = '\0';
}

void pause() {
    printf("Pressione qualquer tecla para continuar[]\n");
    getchar();
}

//Inicializa o album com todas as posições zeradas, ou seja, sem figurinhas
//Também inicializa a lista de repetidas com todas as posições do vetor apontando para NULL
void initAlbum(Amigo *amigo) {
    int i;
    for(i = 0; i <= ALBUM_TAM; i++) {
        amigo->album[i] = 0;
        amigo->repetidas[i] = NULL;
    }
}

//Mostrar figurinhas de cada amigo, inclusive as repetidas
void mostrarFiguras(Amigo *amigo) {
    int i;
    Figura *cursor;
    
    for(i = 1; i <= ALBUM_TAM; i++) {
        //Se album[i] = 1, tem a figurinha   
        if( amigo->album[i] == 1 ) {
            printf("%d\n", i);
            //Repetidas
            cursor = amigo->repetidas[i];
            while(cursor != NULL) {
                printf("%d\n", i);
                cursor = cursor->next;
            }
        }
    }
}

//Mostrar figurinhas de cada amigo, inclusive as repetidas, sem quebrar linha
void mostrarFigurasInline(Amigo *amigo) {
    int i;
    Figura *cursor;
    
    for(i = 1; i <= ALBUM_TAM; i++) {
        //Se album[i] = 1, tem a figurinha   
        if( amigo->album[i] == 1 ) {
            printf("%d ", i);
            //Repetidas
            cursor = amigo->repetidas[i];
            while(cursor != NULL) {
                printf("%d ", i);
                cursor = cursor->next;
            }
        }
    }
    printf("\n");
}

//Cria um Amigo, pedindo nome, telefone, e inicializando o Album vazio
Amigo *criarAmigo() {
    Amigo *novo = malloc(sizeof(Amigo));
    if(!novo) {
        printf("Nao foi possivel alocar memoria\n");
        return NULL;
    }
    printf("Informe o nome completo\n");
    fgets(novo->nome, NOME_TAM + 1, stdin);
    removerNovaLinha(novo->nome);
    printf("Informe o telefone\n");
    fgets(novo->telefone, TEL_TAM, stdin);
    removerNovaLinha(novo->telefone);
    initAlbum(novo);
    
    return novo;
    
}

//Para criar amigo automaticamente a partir dos dados importados
Amigo *amigoAutoCreate(char *linha) {
    Amigo *novo = malloc(sizeof(Amigo));
    if(novo == NULL) {
        return NULL;
    }
    initAlbum(novo);
    
    
    char nome[NOME_TAM];
    char telefone[TEL_TAM];
    char figuraNum[6];
    int i = 0, j = 0;
    //Ler nome
    while(linha[i] != ',') {
        nome[i] = linha[i];
        i++;
    }
    nome[i] = '\0';
    i++;
    //Atribui nome ao amigo
    strcpy(novo->nome, nome);
    //Ler telefone
    j = 0;
    while(linha[i] != ',') {
        telefone[j] = linha[i];
        i++;
        j++;
    }
    telefone[j] = '\0';
    i++;
    //Atribui o telefone ao amigo
    strcpy(novo->telefone, telefone);
    //Ler numeros das figuras
    j = 0;
    while(linha[i] != '\n') {
        
        if( isdigit(linha[i]) ) {
            figuraNum[j] = linha[i];
            j++;
        }
        if( linha[i] == ',') {
            figuraNum[j] = '\0';
            insereFiguraEspecifica(novo, atoi(figuraNum));
            j = 0;
        }
        
        i++;
    }
    figuraNum[j] = '\0';
    insereFiguraEspecifica(novo, atoi(figuraNum));
    
    
    return novo;
}

//Inserir amigo criado através da importação de dados do arquivo
int amigoAutoInsert(Amigo **head, Amigo *novo) {
       
       Amigo *cursor = *head;
       
       if( *head == NULL || strcmp(novo->nome, (*head)->nome) < 0 ) {
           novo->next = *head;
           *head = novo;
       } else {
           while(cursor->next != NULL && strcmp(cursor->next->nome, novo->nome) < 0) {
               cursor = cursor->next;
           }
           novo->next = cursor->next;
           cursor->next = novo;
       }
       return 1;
}



//Inserir Amigo ordenadamente de acordo com o nome
int cadastrarAmigo(Amigo **head) {
    system(CLEAR);
    Amigo *novo = criarAmigo();
    if(novo == NULL) {
        return 0;
    }
    
    Amigo *cursor = *head;
    //Lista vazia ou primeiro da lista
    if( *head == NULL || strcmp(novo->nome, (*head)->nome) < 0 ) {
        novo->next = *head; //Caso HEAD seja NULL, novo->next será NULL :D LEGAL!
        *head = novo;
        
    } else {
        //Enquanto o proximo não for NULL, ou o nome novo for maior que o próximo
        while( cursor->next != NULL && strcmp(cursor->next->nome, novo->nome) < 0 ) {
            cursor = cursor->next;
        }
        novo->next = cursor->next;
        cursor->next = novo;
    }
    return 1;
}


//Busca em uma lista de Amigo pelo nome, retornando o endereço caso localize, senão retorna NULL
Amigo *buscaNome(Amigo *head, const char *nome) {
    
    Amigo *cursor = head;
    while(cursor != NULL) {
        if( strcmp(cursor->nome, nome) == 0 ) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
    
}



//Mostra nome, telefone, e figurinhas do Amigo
void mostrarAmigo(Amigo *amigo) {
    
    if(amigo != NULL) {
        printf("%s\n", amigo->nome);
        printf("%s\n", amigo->telefone);
        mostrarFigurasInline(amigo);
        
        
    }
}

//Seleciona amigo e mostra
void mostrarAmigoSelecionado(Amigo *head) {
    
    system(CLEAR);
    char nome[NOME_TAM + 1];
    printf("Selecione o nome do amigo\n");
    fgets(nome, NOME_TAM, stdin);
    removerNovaLinha(nome);
    
    Amigo *amigo = buscaNome(head, nome);
    
    mostrarAmigo(amigo);
    
}




//Mostrar dados de todos os amigos
void mostrarTodosAmigos(Amigo *head) {
    
    system(CLEAR);
    Amigo *cursor = head;
    while(cursor != NULL) {
        mostrarAmigo(cursor);
        printf("---------------------\n");
        cursor = cursor->next;
    }
}

//Mostrar menu principal
void showMenu() {
    printf("1. CADASTRAR AMIGO\n");
    printf("2. INSERIR FIGURINHA\n");
    printf("3. MOSTRAR AMIGO\n");
    printf("4. MOSTRAR TODOS\n");
    printf("5. TROCAR FIGURINHAS\n");
    printf("6. IMPORTAR DADOS DE ARQUIVO\n");
    printf("7. SAIR\n");
}

void showMenuInsercao() {
    printf("1. INSERIR FIGURA ESPECIFICA\n");
    printf("2. INSERIR FIGURAS ALEATORIAMENTE\n");
}

//Verifica se a opcao selecionada é inválida. Retorna Verdadeiro para opção Inválida
int opcaoInvalida(int opcao) {
    return(opcao > 7 || opcao < 1);
}

//Selecionar opcao
int selecionaOpcao() {
    int opcao;
    
    scanf("%d", &opcao);
    clearInputBuffer();
    while( opcaoInvalida(opcao) ) {
        system(CLEAR);
        showMenu();
        printf("Opcao invalida, digite novamente\n");
        scanf("%d", &opcao);
        clearInputBuffer();
    }
    return opcao;
}

//Insere a figura na lista de figuras repetidas.
//Visto que temos um vetor de ponteiros, e cada vetor é o início de uma lista encadeada cujos valores são iguais, 
//inserimos o novo nodo como primeiro elemento da lista
int insereFiguraRepetida(Amigo *head, int figuraNum) {
    
    Figura *nova = malloc(sizeof(Figura));
    if(nova == NULL) {
        printf("Não foi possivel alocar memoria para nova figura\n");
        return 0;
    }
    nova->numero = figuraNum;
    
    nova->next = head->repetidas[figuraNum];
    head->repetidas[figuraNum] = nova;
    
    return 1;
    
}

//Insere uma figura
int insereFiguraEspecifica(Amigo *amigo, int figuraNum) {
    
   
    //Figura não repetida
    if(amigo->album[figuraNum] == 0) {
        amigo->album[figuraNum] = 1;
    }else {
        //Figura repetida
        if (! insereFiguraRepetida(amigo, figuraNum) ) {
            return 0;
        }
    }
    return 1;
    
}

//Insere um numero x de pacotes contendo 5 figurinhas aleatórias cada
int insereFigurasAleatorias(Amigo *amigo) {
    
    
    int qtdePacotes;
    printf("Selecione a quantidade de pacotes que deseja abrir\n");
    scanf("%d", &qtdePacotes);
    clearInputBuffer();
    int i, figuraNum;
    //Aloca memória para guardar todas as figurinhas que vierem nos pacotes
    //Será uma área de memória com tamanho qtdePacotes * QTDE_FIG_PACOTE
    
    for(i = 0; i < qtdePacotes * QTDE_FIG_PACOTE; i++) {
        figuraNum = (rand() % ALBUM_TAM) + 1; //DE 1 A ALBUM_TAM
        if ( insereFiguraEspecifica(amigo, figuraNum) ) {
            printf("Figura = [%d]. Insercao bem sucedida\n", figuraNum);
        } else {
            return 0;
        }
    }
    
    return 1;
}

//Mostrar somente os nomes dos amigos. Para auxiliar na hora de escolher
int showNomesAmigos(Amigo *head) {
    Amigo *cursor = head;
    if(cursor == NULL) {
        printf("Nenhum amigo cadastrado\n");
        return 0;
    }
    
    while(cursor != NULL) {
        printf("%s\n", cursor->nome);
        cursor = cursor->next;
    }
    return 1;
}


//Permite selecionar qual o tipo de insercao
int selecionaTipoInsercao(Amigo *head) {
    
    
    system(CLEAR);
        
    int opcao;
    int figuraNum;
    char nome[NOME_TAM + 1];
    Amigo *amigo;
    
    printf("Amigos: \n");
    if (! showNomesAmigos(head) ) {
        return 0;
    }
    
    printf("Escreva o nome do amigo que irá receber as figurinhas\n");
    fgets(nome, NOME_TAM, stdin);
    removerNovaLinha(nome);
    //Tenta localizar o amigo na lista
    amigo = buscaNome(head, nome);
    
    if(amigo == NULL) {
        printf("Nao foi possivel localizar o amigo %s\n", nome);
        return 0;
    }else {
        printf("Localizado\n");
    }
    
    showMenuInsercao();
    
    printf("Selecione o tipo de insercao desejado\n");
    scanf("%d", &opcao);
    clearInputBuffer();
    if(opcao == 1) {
        
        printf("Selecione o numero da figura que deseja inserir\n");
        scanf("%d", &figuraNum);
        clearInputBuffer();    
        if ( insereFiguraEspecifica(amigo, figuraNum) ) {
            return 1;
        }
        
    }else {
        
        if( insereFigurasAleatorias(amigo) ) {
            return 1;
        }
    }
}

//Importar dados do arquivo
int importarDados(Amigo **head) {
    
    
    
    FILE *arquivo = fopen("DADOS.txt", "r");
    char linha[REGISTRO_TAM];
    if(arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }
    Amigo *novo;
    
    
    while( fgets(linha, REGISTRO_TAM, arquivo) ) {
        novo = amigoAutoCreate(linha);
        amigoAutoInsert(head, novo);
    }
    return 1;
    
}




//Função para realizar a troca, recebendo dois amigos como parâmetros. RECURSIVA
int trocaFigura(Amigo *amigo1, Amigo *amigo2) {
    int i = 1, j = 1, figuraNum;
    if(amigo2 == NULL) {
        return 1;
    }else {
        printf("\n");
        printf("----------------------------------------\n");
        printf("%s <=> %s\n", amigo1->nome, amigo2->nome);
        printf("----------------------------------------\n");
        printf("Antes da troca\n");
        printf("%s\n", amigo1->nome);
        mostrarFigurasInline(amigo1);
        printf("%s\n", amigo2->nome);
        mostrarFigurasInline(amigo2);
        while(i <= ALBUM_TAM) {
            
            //Se o amigo1 não tiver uma figurinha em seu álbum
            if( amigo1->album[i] == 0 ) {
                //Procura se o amigo2 tem a figurinha repetida
                if( amigo2->repetidas[i] != NULL ) {
                    j = 1;
                    while(j <= ALBUM_TAM) {
                        //Se o amigo2 não tiver uma figurinha no album, e o amigo1 tiver a figurinha em suas repetidas
                        if(amigo2->album[j] == 0 && amigo1->repetidas[j] != NULL) {
                            figuraNum = popRepetida(amigo2->repetidas, i); //Tira a figura repetida de amigo2
                            amigo1->album[i] = 1; //Coloca a figura no album de amigo1
                            figuraNum = popRepetida(amigo1->repetidas, j);//Tira a figura repetida de amigo1
                            amigo2->album[j] = 1;//Coloca a figura no album de amigo2
                            
                            printf("----------------------------------------\n");
                            printf("%s recebeu [%d] de %s\n", amigo1->nome, i, amigo2->nome);
                            printf("%s recebeu [%d] de %s\n", amigo2->nome, j, amigo1->nome);
                            printf("----------------------------------------\n");
                            
                            break; //Sai do loop interno
                        }
                        j++;
                    }
                }
        }
            i++;
        }
        printf("Após a troca\n");
        printf("%s\n", amigo1->nome);
        mostrarFigurasInline(amigo1);
        printf("%s\n", amigo2->nome);
        mostrarFigurasInline(amigo2);
        trocaFigura(amigo1, amigo2->next);
    }
    return 1;
}

//Percorre a lista de amigos selecionando dois a cada vez para a troca de figuras
void startTroca(Amigo *head) {
    
    Amigo *cursor = head;
    while(cursor->next != NULL) {
        trocaFigura(cursor, cursor->next);
        cursor = cursor->next;
    }
    
}

int popRepetida(Figura *repetidas[], int figuraNum) {
    int hold = repetidas[figuraNum]->numero;
    Figura *temp = repetidas[figuraNum];
    repetidas[figuraNum] = repetidas[figuraNum] -> next;
    free(temp);
    return hold;
    
}

int main() {
    
    int opcao = 0;
    srand(time(NULL));
    Amigo *head = NULL;
    
    while(opcao != 7) {
        system(CLEAR);
        showMenu();
        printf("Selecione a opcao desejada\n");
        opcao = selecionaOpcao();
        
        switch(opcao) {
            
            
            case 1: if( cadastrarAmigo(&head) ) {
                        printf("Amigo cadastrado com sucesso\n");
                    } else {
                        printf("Nao foi possivel cadastrar\n");
                    }
                    break;
            
            case 2: if( selecionaTipoInsercao(head) ) {
                        printf("Figuras adicionadas com sucesso!\n");
                    }else {
                        printf("Nao foi possivel adicionar figuras\n");
                    }
                    break;
            
            case 3: mostrarAmigoSelecionado(head);
                    break;
            case 4: mostrarTodosAmigos(head);
                    break;
            case 5: startTroca(head);
                    break;
            case 6: importarDados(&head);
                    break;
            case 7: break;
            default: break;
        }
        pause();
        
    }
    
    
    system(CLEAR);
    
    
    return 0;
}
