#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>


/*
Documentação do jogo

Regras de utilização de nomenclatura da main.

Variáveis:
char palavra: é a variável que vai armazenar a palavra sorteada;
int rodada: indica quantas tentativas a pessoa ainda tem ao final da tentativa corrente.

Arquivos:
lista.txt: É o arquivo com a lista de palavras para sorteio - UTILIZADO O PONTEIRO LISTA;
listab.txt: É o arquivo com as palavras bloqueadas;
salvos.txt: É o arquivo com as palavras sorteadas;
backup.txt: É o arquivo com o histórico do jogo.


Funções:

sorteia_pal: Sorteia uma palavra da lista;
pega_pal: Ele scaneia o STDIN para pegar a palavra que o usuário digitou e armazenará no char palavra;
checa_pal: Checa se a tentativa do usuário é válida dentro dos parâmetros do jogo;
testa_pal: Testa a correspondência da palavra sorteada com a digitada, e armazena os acertos e erros no backup.txt;
salva_pal: Salva palavra sorteada em uma lista de palavras para futura checagem na sorteia_pal;
salva_jogo: Salva o estado do jogo em um arquivo externo;
reiniciar_jogo: Reinicia as variáveis e arquivos externos.
*/

void sorteia_pal();
void pega_pal();
void checa_pal();
void testa_pal();
void salva_pal();
void salva_jogo();

char palavra[5], tentativa[5], letras[5];
//Palavra sorteada é definida globalmente para permitir a manipulação por todas as funções sem necessitar de abstrações mais pesadas.
//Palavra: Palavra sorteada pelo joogo.
//Tentativa: tentativa do usuário de acertar a palavra.
//Letras: Letras que o usuário acertou da palavra.

int main (){
    
    setlocale(LC_ALL, "Portuguese");

    FILE  *sorteadas;

    int rodada = 5, palavra_valida = 0;
    int acertos = 0;

    while (rodada >= 0){

        //printf("Bem vindo ao jogo das palavras, nesse jogo você deve acertar a palavra de 5 letras escolhida pelo programa");
        sorteia_pal();
        pega_pal();
        salva_pal();

        //Printf de debug para verificar se as checagens estão funcionando.
        printf("Tentativa passou pelas checagens.");

        testa_pal(&acertos);
        return 0;

        rodada--;
    }

    

}

/*
Aqui começa a manipulação das funções do arquivo
*/

void sorteia_pal(){

//Essa função vai sortear uma palavra aleatória da lista

    int linha;

    FILE *lista;

    lista = fopen("lista.txt", "r");

    if (lista == NULL){
        printf("Arquivos essenciais do jogo não foram encontrados");
        
    }

    srand(time(NULL));

    linha = rand() % 14;


    for (int i = 0; i <= linha; i++){
        fscanf(lista, "%s", palavra);
        printf("\npalavra selecionada foi %s", palavra);
    }

    // Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);

    fclose(lista);
}

void pega_pal(){
    
    //Essa função pega a tentativa do usuário e verifica se ela está válida.

    int pega_erro;
    const char proibido[] = "012345789/*-+,!@#$&*()_?~}]{[";

    do{
        printf("Digite a sua tentativa válida\n");
        pega_erro = 0;

        scanf("%s", tentativa);
        checa_pal(&pega_erro);

         
        if (strpbrk(proibido,tentativa) != 0 || strlen(tentativa) != 5){
            pega_erro = 1;
            printf("Detecada uma palavra inválida, tente novamente.\n");
        }

    }while(pega_erro == 1);
    //Parte do código que testa se a tentativa tem 5 caracteres ou se tem número, seria interessante refatorar para deixar claro o erro. Função protótipo.
    
}

void checa_pal(int *passa_erro){

//Essa função checa se a palavra sorteada está de acordo com as especificações da mecânica de jogo.

FILE *listab;
char compara[5];

listab = fopen("listab.txt", "r");

 if (listab == NULL){
        printf("Arquivos essenciais do jogo não foram encontrados.");
    }

do{
    fscanf(listab, "%s", compara);

    //printf("Comparando %s com %s\n", tentativa, compara);

    if (strcmp(tentativa,compara)== 0){
        printf("Detecada uma palavra inválida, tente novamente.\n");
        *passa_erro = 1;
    }
    else
    *passa_erro = 0;
    //Permite o jogo continuar.
    
}while(feof(listab)== 0);

fclose(listab);

}

void testa_pal(int *acertos)
{
    char erros[5] = {' ', ' ', ' ', ' ', ' '};  // inicializar os possíveis erros da rodada (importante para ter índices futuros)
    char certos[5] = {' ', ' ', ' ', ' ', ' '}; // inicializar os possíveis acertos da rodada (importante para ter índices futuros)
    for (int i = 0; i < 5; i++)
    {
        if (tentativa[i] == palavra[i])
        {
            letras[i] = tentativa[i];
            certos[i] = tentativa[i];
            *acertos++;
            // printf("\nVocê acertou a letra %c da palavra %s", tentativa[i], palavra); //Aqui pode colocar uma corzinha marota
        }
        else
        {
            erros[i] = tentativa[i];
        }
    }
    if (*acertos == 5)
    {
        // Podia aparecer uma pop up lindona dando parabens
    }
    else
    {
        // perdeu. Vai salvar os erros no arquivo backup
        for (int i = 0; i < 5; i++)
        {
            if (erros[i] != ' ')
                salva_jogo(erros[i]);
        }
        salva_jogo("\n"); // pular linha
    }
    // Vai salvar os acertos da rodada no arquivo backup
    for (int i = 0; i < 5; i++)
    {
        if (certos[i] != ' ')
            salva_jogo(certos[i]);
    }
    salva_jogo("\n"); // pular linha
}

void salva_pal(){
    // Essa função deve salvar palavras sorteadas em um arquivo.txt para futura checagem no sorteia_pal()
    

}

void reiniciar_jogo() // Por ora só vai esvaziar o backup, mas no futuro pode servir como restart
{
    FILE *arquivo;
    arquivo = fopen("backup.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    fclose(arquivo);
}

void salva_jogo(char termo[5]) // será passado os termos a serem salvos
{
    FILE *arquivo; // cria variável ponteiro para o arquivo
    // abrindo o arquivo com tipo de abertura a
    arquivo = fopen("backup.txt", "a"); // salvos.txt é do jay pi
    // testando se o arquivo foi realmente criado
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    // usando fprintf para armazenar a string no arquivo
    fprintf(arquivo, "%s ", termo);
    // usando fclose para fechar o arquivo
    fclose(arquivo);
}
