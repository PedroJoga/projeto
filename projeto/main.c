#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> ///////////////////////// nota corrigir tamanho das strings talvez com uma funcao
#include <string.h>
#include <time.h>
#include <locale.h>

//constantes do struct "tipoUser"
#define N_UTENTE_S_MIN 100000000
#define N_UTENTE_S_MAX 999999999
#define TIPO_MEMBRO_STRING 10 // nota, trocar istas coisas por alocação dinamica de memoria, acho eu
#define ESTADO_CONFINAMENTO_STRING 23
#define ESTADO_VACINACAO_STRING 11

#define TIPO_MEMBRO_1 "estudante"
#define TIPO_MEMBRO_2 "docente"
#define TIPO_MEMBRO_3 "tecnico"
#define ESTADO_CONFINAMENTO_1 "nao confinado"
#define ESTADO_CONFINAMENTO_2 "quarentena"
#define ESTADO_CONFINAMENTO_3 "isolamento profilatico"
#define ESTADO_VACINACAO_1 "sem vacina"
#define ESTADO_VACINACAO_2 "dose1"
#define ESTADO_VACINACAO_3 "dose2"
#define ESTADO_VACINACAO_4 "dose3"

//constantes do struct "tipoTeste"
#define ID_MIN 1
#define ID_MAX 99999
#define TIPO_TESTE_STRING 10
#define TIPO_RESULTADO_STRING 13
#define TIPO_ESTADO_STRING 10

#define TIPO_TESTE_1 "PCR"
#define TIPO_TESTE_2 "antigenio"
#define RESUTADO_1 "positivo"
#define RESUTADO_2 "negativo"
#define RESUTADO_3 "inconclusivo"
#define ESTADO_1 "agendado"
#define ESTADO_2 "realizado"

//constantes gerais
#define MAX_MEMBROS 200
#define MAX_STRING 100
#define MAX_TESTES 9999
#define MAX_TESTES_PCR 15


typedef struct
{
    int dia, mes, ano;
}tipoData;

typedef struct
{
    int hora, minuto;
}tipoHora;

typedef struct
{
    long numeroUtenteS;
    char nome[MAX_STRING];
    char tipoMembro[TIPO_MEMBRO_STRING];
    tipoData dataNascimento;
    char estadoConfinamento[ESTADO_CONFINAMENTO_STRING];
    char estadoVacinacao[ESTADO_VACINACAO_STRING];
    tipoData dataUltimaVacina;
}tipoUser;

typedef struct
{
    int testeID;
    char tipoTeste[TIPO_TESTE_STRING];
    tipoData dataRealizacao;
    long numeroUtenteS;
    char resultado[TIPO_RESULTADO_STRING];
    tipoHora horaColheita;
    int duracao;
    char estado[TIPO_ESTADO_STRING];
}tipoTeste;

typedef struct
{
    long numeroUtenteS;
    char estadoConfinamento[ESTADO_CONFINAMENTO_STRING];
    tipoData dataConfinamento;
    int duracaoConfinamento;
}tipoConfinamento;

char menu(int, int, int, int);

//funcoes
void contarTestes(tipoTeste *, int, int *, int *);
int contarVacinados(tipoUser *, int);
int adicionarMembro(tipoUser *, int);
tipoUser lerDadosMembro(tipoUser *, int, int *);
int procurarMembro(tipoUser *, int, long);
void mostrarMembros(tipoUser *, int, tipoTeste *, int);
int agendarTeste(tipoTeste *, int , tipoUser *, int);
tipoTeste lerDadosTeste(tipoTeste *, int, long);
int gerarID(tipoTeste *, int);
void atRegVacMembro(tipoUser vetorMembros[], int numMembros);
void atRegConfMembro(tipoUser vetorMembros[MAX_MEMBROS], int numMembros);

//funcoes gerais
tipoData lerData(char *);
int compararData(tipoData, tipoData); //funcao que compara duas datas, devolte 1, se 1ºdata > 2ºdata, devolve 0 se 1ºdata == 2ºdata ou devolve -1, se 1ºdata < 2ºdata
int procurarData(tipoData, tipoData *, int); //funcao que devolve a quantidade de datas iguais
int lerInteiro(char *, int, int);
int lerLong(char *, long, long);
float lerFloat(char *, float, float);
void lerString(char *, char *, int);
void funcaoEscolha_2_opcao(char *, char *, char *, char *);
void funcaoEscolha_3_opcao(char *, char *, char *, char *, char *);
void funcaoEscolha_4_opcao(char *, char *, char *, char *, char *, char *);
int gerarNumero(int, int);
void limpaBufferStdin(void);

int main()
{
    setlocale(LC_ALL,"Portuguese");
    tipoConfinamento *pConf;
	pConf = malloc(1 * sizeof(tipoConfinamento));
    tipoUser vetorMembros[MAX_MEMBROS];
    tipoTeste vetorTestes[MAX_TESTES];
    FILE *ficheiro;
    int numMembros, numTestes, numTestesAgendados, numTestesRealizados, numVacinados;
    char opcao;

    numMembros = 0;
    numTestes = 0;
    numTestesAgendados = 0;
    numTestesRealizados = 0;

    srand(time(NULL)); //gerar semente aleatoria para a funcao gerarNumero();

    //Atualizar no inicio do programa as structs com dados
    ficheiro = fopen("membros.dat","rb");
    if(ficheiro == NULL)
    {
        printf("Erro ao atualizar os dados no programa");
    }
    else
    {
        fread(&numMembros,sizeof(int),1,ficheiro);
        fread(vetorMembros,sizeof(tipoUser),MAX_MEMBROS,ficheiro);
        fread(vetorTestes,sizeof(tipoUser),MAX_TESTES,ficheiro);
    }
    fclose(ficheiro);
    //------------


    do
    {

        //Atualizar dados no ficheiro
        ficheiro = fopen("membros.dat","wb");
        if(ficheiro == NULL)
        {
            printf("Erro ao guardar os dados no ficheiro");
        }
        else
        {
            fwrite(&numMembros,sizeof(int),1,ficheiro);
            fwrite(vetorMembros,sizeof(tipoUser),MAX_MEMBROS,ficheiro);
            fwrite(vetorTestes,sizeof(tipoUser),MAX_TESTES,ficheiro);
        }
        fclose(ficheiro);
        //----------------


        contarTestes(vetorTestes, numTestes, &numTestesAgendados, &numTestesRealizados);
        numVacinados = contarVacinados(vetorMembros, numMembros);

        opcao = menu(numMembros, numTestesAgendados, numTestesRealizados, numVacinados);

        switch(opcao)
        {
            case 'A':
                numMembros = adicionarMembro(vetorMembros, numMembros);
            break;
            case 'B':
                mostrarMembros(vetorMembros, numMembros, vetorTestes, numTestes);
            break;
            case 'C':
                atRegVacMembro(vetorMembros, numMembros);
            break;
            case 'D':
                atRegConfMembro(vetorMembros, numMembros);
            break;
            case 'E':
                numTestes = agendarTeste(vetorTestes, numTestes, vetorMembros, numMembros);
            break;
        }

    }while(opcao != 'F');

    return 0;
}

char menu(int nMembros, int nTestesAgendados, int nTestesRealizados, int nVacinados)
{
    char opcao;

    do
    {
        printf("\tMenu Principal\n\n");
        printf("\tN de membros - %d \tN de vacinados - %d\n", nMembros, nVacinados);
        printf("\tTestes agendados - %d \tTestes realizados - %d\n\n", nTestesAgendados, nTestesRealizados);
        printf("\tA - Inserir membro\n");
        printf("\tB - Listar membros\n");
        printf("\tC - Registar/atualizar vacinacao\n");
        printf("\tD - Registar/atualizar confinamento\n");
        printf("\tE - Agendar teste\n");
        printf("\tF - Fim\n");
        printf("\tOpcao --> ");
        scanf("%c", &opcao);
        opcao = toupper(opcao);
        limpaBufferStdin();
    }while (opcao != 'A' && opcao != 'B' && opcao != 'C' && opcao != 'D' && opcao != 'E' && opcao != 'F');

    return opcao;
}

void contarTestes(tipoTeste vetorTestes[], int numTestes, int *numAgendados, int *numRealizados)
{
    int i, validacao;

    if(numTestes > 0)
    {
        for(i = 0; i < numTestes; i++)
        {
            if(strcmp(vetorTestes[i].estado, ESTADO_1) == 0)
            {
                *numAgendados++;
            }
            printf("%s",vetorTestes[i].estado);
            printf("primeiro if %d,%d",*numAgendados,validacao);
            if(strcmp(vetorTestes[i].estado, ESTADO_2) == 0)
            {
                *numRealizados++;
            }
            printf("segundo if %d,%d",*numAgendados,validacao);
        }
    }
}

int contarVacinados(tipoUser vetorMembros[], int numMembros)
{
    int numero, i, validacao;

    numero = 0;

    if(numMembros > 0)
    {
        for(i = 0; i < numMembros; i++)
        {
            validacao = strcmp(vetorMembros[i].estadoVacinacao, ESTADO_VACINACAO_1);
            if(validacao != 0)
            {
                numero++;
            }
        }
    }

    return numero;
}

int adicionarMembro(tipoUser vetorMembros[MAX_MEMBROS], int numMembros)
{
    int posicao, confirmacao;
    tipoUser membro;

    if(numMembros == MAX_MEMBROS)
    {
        printf("Numero maximo de membros atingido\n");
    }else
    {
        membro = lerDadosMembro(vetorMembros, numMembros, &confirmacao);

        if(confirmacao == 1)
        {
            numMembros++; //incrementa um membro no contador
            vetorMembros[numMembros - 1] = membro;
        }
    }

    return numMembros;
}

tipoUser lerDadosMembro(tipoUser vetorMembros[], int numMembros, int *conf)
{
    tipoData data;
    int posicao, validacao;
    tipoUser membro;

    printf("INSERIRA OS DADOS DO MEMBRO\n");

    membro.numeroUtenteS = lerLong("Numero de utente de saude", N_UTENTE_S_MIN, N_UTENTE_S_MAX);

    posicao = procurarMembro(vetorMembros, numMembros, membro.numeroUtenteS);

    if(posicao == -1)
    {
        lerString("Nome", membro.nome, MAX_STRING);
        funcaoEscolha_3_opcao("Tipo de membro", membro.tipoMembro, TIPO_MEMBRO_1, TIPO_MEMBRO_2, TIPO_MEMBRO_3);
        membro.dataNascimento = lerData("Data de nascimento");
        funcaoEscolha_3_opcao("Estado de confinamento", membro.estadoConfinamento, ESTADO_CONFINAMENTO_1, ESTADO_CONFINAMENTO_2, ESTADO_CONFINAMENTO_3);
        funcaoEscolha_4_opcao("Estado de vacinacao", membro.estadoVacinacao, ESTADO_VACINACAO_1, ESTADO_VACINACAO_2, ESTADO_VACINACAO_3, ESTADO_VACINACAO_4);

        validacao = strcmp(membro.estadoVacinacao, ESTADO_VACINACAO_1);
        if(validacao == 0)
        {
            membro.dataUltimaVacina.dia = -1; //valor referente á ausencia de uma data
        }else
        {
            do
            {
                data = lerData("Data da ultima vacina");
                validacao = compararData(membro.dataNascimento, data);
                if(validacao != -1)
                {
                    printf("Data invalida\n");
                }
            }while(validacao != -1);

            membro.dataUltimaVacina = data;
        }
        *conf = 1;
    }else
    {
        printf("O n de utente inserido ja existe.\n");
        *conf = 0;
    }

    return membro;
}

int procurarMembro(tipoUser vetorMembros[], int numMembros, long numUtenteS)
{
    int i, posicao;

    posicao = -1;

    for(i = 0; i < numMembros; i++)
    {
        if(vetorMembros[i].numeroUtenteS == numUtenteS)
        {
            posicao = i;
            i = numMembros;  //caso aja correspondencia, altera o valor do indice para sair do loop
        }
    }

    return posicao;
}

void mostrarMembros(tipoUser vetorMembros[], int numMembros, tipoTeste vetorTestes[], int numTestes)
{
    int membro, i, validacao, totalTestes, testePCR, testeAntigenio, testeAgendado, testeRealizado;

    if(numMembros == 0)
    {
        printf("Nao ha membros para listar\n");
    }else
    {
        printf("N Utente  Nome\t\tTipo\t\tData Nasc.\tEstado de Conf\tEstado de Vac\tData da Ult Vac\t\n");

        for(membro = 0; membro < numMembros; membro++)
        {
            printf("%ld %s\t%s\t%d-%d-%d\t%s\t%s", vetorMembros[membro].numeroUtenteS
                                                   , vetorMembros[membro].nome
                                                   , vetorMembros[membro].tipoMembro
                                                   , vetorMembros[membro].dataNascimento.dia
                                                   , vetorMembros[membro].dataNascimento.mes
                                                   , vetorMembros[membro].dataNascimento.ano
                                                   , vetorMembros[membro].estadoConfinamento
                                                   , vetorMembros[membro].estadoVacinacao);
            if(vetorMembros[membro].dataUltimaVacina.dia == -1)
            {
                printf("----------");
            }else
            {
               printf("%d-%d-%d", vetorMembros[membro].dataUltimaVacina.dia
                                , vetorMembros[membro].dataUltimaVacina.mes
                                , vetorMembros[membro].dataUltimaVacina.ano);
            }

            totalTestes = 0;
            testePCR = 0;
            testeAntigenio = 0;
            testeAgendado = 0;
            testeRealizado = 0;

            printf("\n #   Tipo de Teste Data de Realiz. Resultado Hora  Duracao Estado\n");
            if(numTestes != 0)
            {
                for(i = 0; i < numTestes; i++)
                {
                    if(vetorTestes[i].numeroUtenteS == vetorMembros[membro].numeroUtenteS)
                    {
                        validacao = strcmp(vetorTestes[i].estado, ESTADO_2);
                        if(validacao == 0)
                        {
                            testeRealizado++;
                        }else
                        {
                            testeAgendado++;
                        }

                        validacao = strcmp(vetorTestes[i].tipoTeste, TIPO_TESTE_1);
                        if(validacao == 0)
                        {
                            testePCR++;
                        }else
                        {
                            testeAntigenio++;
                        }
                        printf("%4.d %s %d-%d-%d %s %d:%d %d %s\n", vetorTestes[i].testeID
                                                                  , vetorTestes[i].tipoTeste
                                                                  , vetorTestes[i].dataRealizacao.dia
                                                                  , vetorTestes[i].dataRealizacao.mes
                                                                  , vetorTestes[i].dataRealizacao.ano
                                                                  , vetorTestes[i].resultado
                                                                  , vetorTestes[i].horaColheita.hora
                                                                  , vetorTestes[i].horaColheita.minuto
                                                                  , vetorTestes[i].duracao
                                                                  , vetorTestes[i].estado);
                    }
                }
                totalTestes = testeAgendado + testeRealizado;
                printf("\nTestes realizados: %d Teste PCR: %d Teste antigenio: %d Testes agendados: %d Total testes: %d\n", testeRealizado, testePCR, testeAntigenio, testeAgendado, totalTestes);
                printf("=======================================================================================================\n");
            }
        }
/*
        for(aluno = 0; aluno < avaliados; aluno++)
        {
            if()
            {

            }
            printf("%d \t %s \t\t %.2f \t", vetorEstudantes[aluno].numero, vetorEstudantes[aluno].nome, vetorEstudantes[aluno].nota);
            printf("%d-%d-%d\n", vetorEstudantes[aluno].dataAvaliacao.dia, vetorEstudantes[aluno].dataAvaliacao.mes, vetorEstudantes[aluno].dataAvaliacao.ano);
        }*/
    }
}

void atRegVacMembro(tipoUser vetorMembros[MAX_MEMBROS], int numMembros)
{
    int ind = 0;
    long nUtente;
    int opVac,validacao;
    tipoData data;
    if(numMembros != 0)
    {
        nUtente = lerLong("Introduza o número do membro que pretende registar/atualizar: ",100000000,999999999);

            for(ind;ind < numMembros;ind++)
            {
                if(vetorMembros[ind].numeroUtenteS == nUtente)
                {
                    funcaoEscolha_4_opcao("Estado de vacinacao", vetorMembros[ind].estadoVacinacao, ESTADO_VACINACAO_1, ESTADO_VACINACAO_2, ESTADO_VACINACAO_3, ESTADO_VACINACAO_4);
                    validacao = strcmp(vetorMembros[ind].estadoVacinacao, ESTADO_VACINACAO_1);
                    if(validacao == 0)
                    {
                        vetorMembros[ind].dataUltimaVacina.dia = -1; //valor referente á ausencia de uma data
                    }else
                    {
                        do
                        {
                            data = lerData("Data da ultima vacina");
                            validacao = compararData(vetorMembros[ind].dataNascimento, data);
                            if(validacao != -1)
                            {
                                printf("Data invalida\n");
                            }
                        }while(validacao != -1);

                        vetorMembros[ind].dataUltimaVacina = data;
                    }
                }
                else
                {
                    printf("Não existe o membro selecionado");
                }
            }
    }
    else
    {
        printf("\t\nNão existem membros\n\n");
    }
}

void atRegConfMembro(tipoUser vetorMembros[MAX_MEMBROS], int numMembros)
{
    int ind = 0;
    long nUtente;
    int opVac,validacao;
    tipoData data;
    if(numMembros != 0)
    {
        nUtente = lerLong("Introduza o número do membro que pretende registar/atualizar: ",100000000,999999999);

            for(ind;ind < numMembros;ind++)
            {
                if(vetorMembros[ind].numeroUtenteS == nUtente)
                {
                    funcaoEscolha_3_opcao("Estado de confinamento", vetorMembros[ind].estadoConfinamento, ESTADO_CONFINAMENTO_1, ESTADO_CONFINAMENTO_2, ESTADO_CONFINAMENTO_3);
                }
                else
                {
                    printf("Não existe o membro selecionado");
                }
            }
    }
    else
    {
        printf("\t\nNão existem membros\n\n");
    }
}

int agendarTeste(tipoTeste vetorTestes[], int numTestes, tipoUser vetorMembros[], int numMembros)
{
    int posicao,numUtente;

    if(numMembros == 0)
    {
        printf("Nao ha membros para agendar teste");
    }else
    {
        printf("AGENDAR TESTE\n");
        numUtente = lerLong("Numero de utente de saude", N_UTENTE_S_MIN, N_UTENTE_S_MAX);
        posicao = procurarMembro(vetorMembros, numMembros, numUtente);

        if(posicao == -1)
        {
            printf("Membro inexistente");
        }else
        {
            numTestes++;
            vetorTestes[numTestes] = lerDadosTeste(vetorTestes, numTestes, numUtente);
        }
    }
    return numTestes;
}

tipoTeste lerDadosTeste(tipoTeste vetorTestes[], int numTestes, long numUtente)
{
    int validacao, i;
    tipoTeste teste;
    tipoData vetorDatas[MAX_TESTES];

    printf("INSIRA OS DADOS DO TESTE\n");
    teste.testeID = gerarID(vetorTestes, numTestes);
    strcpy(teste.estado, ESTADO_1);
    teste.numeroUtenteS = numUtente;
    teste.dataRealizacao = lerData("Data de realizacao");

    for(i = 0; i < numTestes; i++) //for para copiar os dados do vetor
    {
        vetorDatas[i] = vetorTestes[i].dataRealizacao;
    }

    validacao = procurarData(teste.dataRealizacao, vetorDatas, numTestes);
    if(validacao < MAX_TESTES_PCR)
    {
        funcaoEscolha_2_opcao("Tipo de teste a efetuar", teste.tipoTeste, TIPO_TESTE_1, TIPO_TESTE_2);
    }else
    {
        strcpy(teste.tipoTeste, TIPO_TESTE_2);
        printf("Foi lhe auto atribuido um teste do tipo antigenio, devido ao limite de testes PCR diarios atingido.\n");
    }

    return teste;
}

int gerarID(tipoTeste vetorTestes[], int numTestes)
{
    int id, validacao, i, posicao;

    do
    {
        validacao = 1;

        id = gerarNumero(ID_MIN, ID_MAX);
        for(i = 0; i < numTestes; i++)
        {
            if(vetorTestes[i].testeID == id)
            {
                validacao = 0;
                i = numTestes;
            }
        }
    }while(validacao == 0);

    return id;
}

tipoData lerData(char mensagem[])
{
    tipoData data;
    int maxDiasMes;

    printf("%s\n", mensagem);
    data.ano = lerInteiro("Ano", 1950, 2022);
    data.mes = lerInteiro("Mes", 1, 12);

    switch(data.mes)
    {
        case 2:
            if(data.ano % 400 == 0 || (data.ano % 4 == 0 && data.ano % 100 != 0))
            {
                maxDiasMes = 29;
            }else
            {
                maxDiasMes = 28;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            maxDiasMes = 30;
            break;
        default:
            maxDiasMes = 31;
    }
    data.dia = lerInteiro("Dia", 1, maxDiasMes);

    return data;
}

int compararData(tipoData data1, tipoData data2)
{
    int resultado;

    if(data1.ano > data2.ano)
    {
        resultado = 1;
    }else
    {
        if(data1.ano == data2.ano)
        {
            if(data1.mes > data2.mes)
            {
                resultado = 1;
            }else
            {
                if(data1.mes == data2.mes)
                {
                    if(data1.dia > data2.dia)
                    {
                        resultado = 1;
                    }else
                    {
                        if(data1.dia == data2.dia)
                        {
                            resultado = 0;
                        }else
                        {
                            resultado = -1;
                        }
                    }
                }else
                {
                    resultado = -1;
                }
            }
        }else
        {
            resultado = -1;
        }
    }
    return resultado;
}

int procurarData(tipoData dataReferencia, tipoData vetorData[], int tamanhoVetor)
{
    int i, quantidade, validacao;

    quantidade = 0;

    for(i = 0; i < tamanhoVetor; i++)
    {
        validacao = compararData(dataReferencia, vetorData[i]);
        if(validacao == 0)
        {
            quantidade++;
        }
    }

    return quantidade;
}

int lerInteiro(char mensagem[],int min, int max)
{
    int numero, controlo;

    do
    {
        printf("%s (%d a %d): ", mensagem, min, max);
        controlo = scanf("%d", &numero);
        limpaBufferStdin();
        if(controlo == 0)
        {
            printf("Insira um inteiro\n");
        }
        if(numero < min || numero > max)
        {
            printf("Numero invalido, insira novamente\n");
        }
    }while(numero < min || numero > max || controlo == 0);

    return numero;
}

int lerLong(char mensagem[],long min, long max)
{
    long numero;
    int controlo;

    do
    {
        printf("%s (%d a %d): ", mensagem, min, max);
        controlo = scanf("%d", &numero);
        limpaBufferStdin();
        if(controlo == 0)
        {
            printf("Insira um inteiro\n");
        }
        if(numero < min || numero > max)
        {
            printf("Numero invalido, insira novamente\n");
        }
    }while(numero < min || numero > max || controlo == 0);

    return numero;
}

float lerFloat(char mensagem[], float min, float max)
{
    float numero;
    int controlo;

    do
    {
        printf("%s (%f a %f): ", mensagem, min, max);
        controlo = scanf("%f", &numero);
        limpaBufferStdin();
        if(controlo == 0)
        {
            printf("Insira um numero\n");
        }
        if(numero < min || numero > max)
        {
            printf("Numero invalido, insira novamente\n");
        }

    }while(numero < min || numero > max || controlo == 0);

    return numero;
}

void lerString(char mensagem[], char vetorCaracteres[], int maxCaracteres)
{
    int tamanhoString;
    do
    {
        printf("%s: ", mensagem);
        fgets(vetorCaracteres, maxCaracteres, stdin);

        tamanhoString = strlen(vetorCaracteres);

        if(tamanhoString == 1)
        {
            printf("Insira algum text\n");
        }

    }while(tamanhoString == 1);

    if(vetorCaracteres[tamanhoString - 1] != '\n')
    {
        limpaBufferStdin();
    }else
    {
        vetorCaracteres[tamanhoString - 1] = '\0';
    }
}

void funcaoEscolha_2_opcao(char mensagem[], char vetor[], char opcao1[], char opcao2[])
{
    int opcao;

    do
    {
        printf("%s\n", mensagem);
        printf("1 - %s\n", opcao1);
        printf("2 - %s\n", opcao2);
        printf("Opcao --> ");
        scanf("%d", &opcao);
        limpaBufferStdin();

        if(opcao > 2 || opcao < 1)
        {
            printf("Opcao indisponivel\n");
        }
    }while (opcao > 2 || opcao < 1);

    switch(opcao)
    {
        case 1:
            strcpy(vetor, opcao1);
        break;
        default:
            strcpy(vetor, opcao2);
    }
}

void funcaoEscolha_3_opcao(char mensagem[], char vetor[], char opcao1[], char opcao2[], char opcao3[])
{
    int opcao;

    do
    {
        printf("%s\n", mensagem);
        printf("1 - %s\n", opcao1);
        printf("2 - %s\n", opcao2);
        printf("3 - %s\n", opcao3);
        printf("Opcao --> ");
        scanf("%d", &opcao);
        limpaBufferStdin();

        if(opcao > 3 || opcao < 1)
        {
            printf("Opcao indisponivel\n");
        }
    }while (opcao > 3 || opcao < 1);

    switch(opcao)
    {
        case 1:
            strcpy(vetor, opcao1);
        break;
        case 2:
            strcpy(vetor, opcao2);
        break;
        default:
            strcpy(vetor, opcao3);
    }
}

void funcaoEscolha_4_opcao(char mensagem[], char vetor[], char opcao1[], char opcao2[], char opcao3[], char opcao4[])
{
    int opcao;

    do
    {
        printf("%s\n", mensagem);
        printf("1 - %s\n", opcao1);
        printf("2 - %s\n", opcao2);
        printf("3 - %s\n", opcao3);
        printf("4 - %s\n", opcao4);
        printf("Opcao --> ");
        scanf("%d", &opcao);
        limpaBufferStdin();

        if(opcao > 4 || opcao < 1)
        {
            printf("Opcao indisponivel\n");
        }
    }while (opcao > 4 || opcao < 1);

    switch(opcao)
    {
        case 1:
            strcpy(vetor, opcao1);
        break;
        case 2:
            strcpy(vetor, opcao2);
        break;
        case 3:
            strcpy(vetor, opcao3);
        break;
        default:
            strcpy(vetor, opcao4);
    }
}

int gerarNumero(int min, int max)
{
    int numero;

    numero = (rand() % (max - min + 1)) + min;

    return numero;
}

void limpaBufferStdin(void)
{
    char chr;
    do
    {
        chr = getchar();
    }while(chr != '\n' && chr != EOF);
}
