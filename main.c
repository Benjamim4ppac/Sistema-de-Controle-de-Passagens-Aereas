#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int num_passagem;
    char cpf[12]; // Copiar do "Passageiro" ou referenciar o cpf diretamente (Pode vir a virar um ponteiro)
    char codigo_voo[6]; // Copiar do "Voo" ou referenciar o codigo_voo diretamente (Pode vir a virar um ponteiro)
    char assentos[4];
    char classe[20]; // (Econômica, Executiva ou Primeira Classe) -> Pode virar uma opção de mútipla escolha e guardar inteiros
    char status[20]; // (Confirmada, Cancelada, Pendente) -> Pode virar uma opção de mútipla escolha e guardar inteiros
} Passagem;

typedef struct
{
    char nome[100]; //Nome do  Passageiro
    char cpf[12]; //CPF do Passageiro (XXX.XXX.XXX-XX)
    char telefone[16]; //Telefone do Passageiro ((XX)XXXX-XXXX)
    char email[50]; //E-mail do Passageiro (XXXXXX@XX.com)
    char dataNascimento[11]; //DatadeNascimento (DD/MM/AAAA)

} Passageiro;

typedef struct{
    char codigo_voo[6];  //Código do voo (LXXXX)
    char origem[50];    //Cidade de Origem do voo
    char destino[50];  //Cidade de Destino do voo
    char dataHora[20]; //Data e Hora do voo (DD/MM/AAAA HH:MM)
    char duracao[10]; //Duração do voo (HH:MM)
    char companhia[50]; //Companhia aérea do voo
} Voo;


void menuPrincipal(){
    printf("====================================\n");
    printf("  SISTEMA DE CONTROLE DE PASSAGENS\n");
    printf("====================================\n\n");

    printf("1 - Gerenciar Passageiros\n");
    printf("2 - Gerenciar Voos\n");
    printf("3 - Gerenciar Passagens\n");
    printf("4 - Consultas\n");
    printf("5 - Relatorios\n");
    printf("0 - Sair\n\n");

    printf("Escolha uma opcao: ");
}     

void carregarPassageiro(Passageiro passageiros[],int *qtdPassageiro){

    FILE *arquivo;
    arquivo = fopen("passageirosSalvos.bin","rb");

    if(arquivo == NULL){
    *qtdPassageiro = 0;
    return;
}
    fseek(arquivo,sizeof(int),SEEK_SET); //Pula os quanto inteiros da qtdPassageiro já lida na main para declarar o vetor passageiros de forma dinâmica
    fread(passageiros, sizeof(Passageiro), *qtdPassageiro, arquivo);
    fclose(arquivo);
    printf("Dados dos Passageiros carregados!\n");

}

void  salvarPassageiro(Passageiro passageiros[], int qtdPassageiro){

    FILE *arquivo;
    arquivo = fopen("passageirosSalvos.bin","wb");
    if(arquivo == NULL){
        printf("Erro ao salvar os dados dos passageiros!\n");
        return;
    }
    fwrite(&qtdPassageiro, sizeof(int), 1, arquivo);
    fwrite(passageiros, sizeof(Passageiro), qtdPassageiro, arquivo);
    fclose(arquivo);
    printf("Operacao Salva!\n");

}

void  salvarVoo(Voo voos[], int qtdVoos){

    FILE *arquivo;
    arquivo = fopen("voosSalvos.bin","wb");
    if(arquivo == NULL){
        printf("Erro ao salvar os dados dos voos!\n");
        return;
    }
    fwrite(&qtdVoos, sizeof(int), 1, arquivo);
    fwrite(voos, sizeof(Voo), qtdVoos, arquivo);
    fclose(arquivo);
    printf("Operacao Salva!\n");

}

void carregarVoo(Voo voos[], int *qtdVoos){

    FILE *arquivo;
    arquivo = fopen("voosSalvos.bin","rb");

    if(arquivo == NULL){
    *qtdVoos = 0;
    return;
    }
    fseek(arquivo,sizeof(int),SEEK_SET); //Pula os quanto inteiros da qtdVoos já lida na main para declarar o vetor passageiros de forma dinâmica
    fread(voos, sizeof(Voo), *qtdVoos, arquivo);
    fclose(arquivo);
    printf("Dados dos Voos carregados!\n");

}

int validaCPF(char cpf[12]){ //Função para verificar a validade de um CPF
    int peso=10,cont,soma=0,resto;
    int digitoVerificador[2];
    if (strlen(cpf)!=11){ //Se o CPF não tem 11 digitos = CPF INVÁLIDO!
        printf("Formato Nao Correspondente\n");
        return 0;
    }
    /*for(cont = 0; cont<11; cont++){ //Se o CPF contém digitos não numericos = CPF INVÁLIDO!
        if(cpf[cont]<'0' || cpf[cont]>'9'){
            printf("CPF Contém Dígitos Não Numéricos\n");
            return 0;
        }
    }*/ //Retirado pois a função limparCPF após a entrada do usuario já remove os dígitos não numericos
    int iguais=1;
    for (cont=1;cont<11;cont++){ //Verifica se todos os dígitos do CPF são iguais
        if (cpf[cont] != cpf[0]){
            iguais = 0;
            break;
        };
        
    }
    if(iguais == 1){ //Se todos os dígitos do CPF são iguais = CPF INVÁLIDO!
        printf("CPF possui todos os dígitos Iguais\n");
        return 0;
    }

    for (cont=0;cont<9;cont++){ //Calculo para gerar o 1° dígito verificador
        soma=(cpf[cont] - '0') * peso + soma;
        peso--;
    }

    resto=soma%11;
    if (resto<2){ //Define o 1° dígito verificador
        digitoVerificador[0] = 0;
    } else{
        digitoVerificador[0]= 11 - resto;
    }

    peso=11,soma=0;
    for (cont=0;cont<10;cont++){ //Calculo para gerar o 2° dígito verificador
        soma=(cpf[cont] - '0') * peso + soma;
        peso--;
    }

    resto=soma%11;
    if (resto<2){ //Define o 2° dígito verificador
        digitoVerificador[1] = 0;
    } else{
        digitoVerificador[1]= 11 - resto;
    }

    if((cpf[9]-'0') == digitoVerificador[0] && (cpf[10]-'0') == digitoVerificador[1]){
        return 1;
    } else { //Se os dígitos verificadores do CPF são diferentes = CPF INVÁLIDO!
        return 0;
    }
}

int buscaCPFPassagem(char cpfConsulta[12], Passagem consulta[], int qtdPassagens){ //Função para verificar se o CPF já existe no banco de dados de CPFs cadastrados em uma passagem
    int cont;

    for(cont=0;cont<qtdPassagens;cont++){
        if(strcmp(cpfConsulta,consulta[cont].cpf)==0){
            return cont; //CPF ja cadastrado em uma passagem - Retorna o indice do vetor que o CPF ja existe
        }
    }
    return -1; //CPF não cadastrado
}

int buscaCPF(char cpfConsulta[12], Passageiro consulta[], int qtdPassageiro){ //Função para verificar se o CPF já existe no banco de dados de CPFs cadastrados dos passageiros
    int cont;

    for(cont=0;cont<qtdPassageiro;cont++){
        if(strcmp(cpfConsulta,consulta[cont].cpf)==0){
            return cont; //CPF ja cadastrado - Retorna o indice do vetor que o CPF ja existe
        }
    }
    return -1; //CPF não cadastrado
}

int buscaVoo(char codigo[], Voo voos[], int qtdVoos){
    int i;

    for(i = 0; i < qtdVoos; i++){
        if(strcmp(codigo, voos[i].codigo_voo) == 0){
            return i;  // Procura um voo pelo código e retorna o índice do vetor se encontrado
        }
    }

    return -1;
}
int buscaPassagemPorVoo(char codigo[], Passagem passagens[], int qtdPassagens){
    for (int i = 0; i < qtdPassagens; i++)
    {
        if (strcmp(passagens[i].codigo_voo, codigo) == 0)
            return i;
    }

    return -1;
}

void limparCPF(char original[], char limpo[]){//Função que remove (. e -) do CPF caso o usuário digite na forma(XXX.XXX.XXX-XX) e limpa para (XXXXXXXXXXX)
    int contOriginal,contLimpo=0;

    for(contOriginal =0; original[contOriginal]!= '\0';contOriginal++){
        if(original[contOriginal]>='0' && original[contOriginal]<='9'){
            limpo[contLimpo] = original[contOriginal];
            contLimpo++;
        }
    }
    limpo[contLimpo]='\0';

}

void limparCodVoo(char original[]){//Função que remove (-) do Codígo do voo caso o usuário digite na forma(LXXX-X) e limpa para (LXXXX) e corrije a primeira letra caso o usuario digite (lXXX-X)
    int contOriginal,contLimpo=1;
    char limpo[8];
    limpo[0]=original[0]; //Preserva o primeiro caracter que é a letra
    limpo[0] = toupper(limpo[0]); //Deixa a primeira letra do código em maiusculo
    for(contOriginal = 0; original[contOriginal]!= '\0';contOriginal++){
        if(original[contOriginal]>='0' && original[contOriginal]<='9'){
            limpo[contLimpo] = original[contOriginal];
            contLimpo++;
        }
    }
    limpo[contLimpo]='\0';
    strcpy(original,limpo);

}

int validacodigo_voo(char codigo_voo[6]){
    limparCodVoo(codigo_voo);
    
    if(strlen(codigo_voo)!=5){//O formato esperado deve ter 5 caracteres (LXXXX)
        printf("Formato Não Correspondente\n");
        return 0;
    }
    if(codigo_voo[0]!='L' && codigo_voo[0]!='A' && codigo_voo[0]!='G'){
        printf("Companhia Aerea Nao Correspondente\n");
        printf("Companhias Compativeis:\n");
        printf("- Azul\n");
        printf("- Gol\n");
        printf("- Latam\n");
        return 0;
    }

    int soma=0;//Soma é utilizada para gerar o digito verificador

    for (int i=1;i<4;i++){
        soma+=codigo_voo[i]-'0'; //Soma os digitos do código para gerar o digito verificador
    }
    int digitoInformado = codigo_voo[4]-'0';
    int digitoVerificador = soma%10;

    if(digitoInformado!= digitoVerificador){
        printf("Código Inválido\n");
        return 0;
    }

    return 1;
}

void formatarData(char dataNascimento[]){ //Função que deixa a data no formato: (dd/mm/aaaa)
    char dataFormatada[11];
    int contOriginal,contFormatado=0;
    //Percorre toda a string dataNascimento digitada no cadastro
    for(contOriginal =0; dataNascimento[contOriginal]!= '\0';contOriginal++){
        //Copia apenas os caracteres numéricos
        if(dataNascimento[contOriginal]>='0' && dataNascimento[contOriginal]<='9'){
           dataFormatada[contFormatado] = dataNascimento[contOriginal];
            contFormatado++;
        }
    } //Coloca o caracter '\0' para indicar o fim da string
    dataFormatada[10] = '\0';
    //Desloca os digitos do ano para a direita
    dataFormatada[9] = dataFormatada[7];
    dataFormatada[8] = dataFormatada[6];
    dataFormatada[7] = dataFormatada[5];
    dataFormatada[6] = dataFormatada[4];
    //Coloca a / para separar mes de ano
    dataFormatada[5] = '/';
    //Desloca os digitos do mes para a direita
    dataFormatada[4] = dataFormatada[3];
    dataFormatada[3] = dataFormatada[2];
    //Coloca / para separar dia de mes
    dataFormatada[2] = '/';

    strcpy(dataNascimento, dataFormatada); //Copia o conteudo da data formatada para o parametro da função
    }

void formatarCPF(char cpfFormatado[15]){// formata o CPF recebido no formato: (XXX.XXX.XXX-XX)

    cpfFormatado[14] = '\0';

    // Desloca os dois últimos dígitos para a direita
    cpfFormatado[13] = cpfFormatado[10];
    cpfFormatado[12] = cpfFormatado[9];

    // Coloca o hífen antes dos dígitos verificadores
    cpfFormatado[11] = '-';

    // Desloca os três dígitos anteriores para a direita
    cpfFormatado[10] = cpfFormatado[8];
    cpfFormatado[9] = cpfFormatado[7];
    cpfFormatado[8] = cpfFormatado[6];

    // Coloca o segundo ponto
    cpfFormatado[7] = '.';

    // Desloca os três dígitos anteriores para a direita
    cpfFormatado[6] = cpfFormatado[5];
    cpfFormatado[5] = cpfFormatado[4];
    cpfFormatado[4] = cpfFormatado[3];

    // Coloca o primeiro ponto
    cpfFormatado[3] = '.';
}

int menuEditarPassageiro(){//SubMenu acessado dentro da função editarPassageiro para saber qual dado do passageiro sera editado
    int menu;
    printf("---O que deseja Editar?---\n");
    printf("1. Nome\n2. Telefone\n3. E-mail\n4. Data de Nascimento\nOpção Selecionada: ");
    scanf("%d",&menu);
    switch (menu)
    {
    case 1 :
        return 1;
    case 2 :
        return 2;
    case 3 :
        return 3;
    case 4 :
        return 4;
    default:
        printf("Entrada Inválida!\n");
        break;
    }
    
}

int menuEditarVoo(){
    int opcao;

    printf("\n--- O que deseja editar? ---\n");
    printf("1 - Origem\n");
    printf("2 - Destino\n");
    printf("3 - Data/Hora\n");
    printf("4 - Duracao\n");
    printf("5 - Companhia\n");
    printf("Opcao: "); // Mostra o menu de opções para o usuário escolher qual dado do voo deseja editar

    scanf("%d",&opcao);

    return opcao;
}

void cadastrarPassageiro(Passageiro cadastro[], int *qtdPassageiro){ //Função que efetua o cadastro de um novo passageiro
    printf("------- Cadastro de Novo Passageiro -------\n");

    char cpfDigitado[20]; //Variavel para armazenar o CPF digitado
    
    printf("CPF: ");
    scanf("%s",cpfDigitado); //Recebe do usuário o CPF a ser cadastrado

    limparCPF(cpfDigitado, cadastro[*qtdPassageiro].cpf); //Caso o usuario digite o CPF com (. ou -) ele sera convertido em apenas caracteres numéricos

    if(validaCPF(cadastro[*qtdPassageiro].cpf) == 0){ //verifica se é um CPF válido
        printf("CPF Inválido!\n"); //CPF invaliudo, fecha a função
        return;
    } 
   
    if(buscaCPF(cadastro[*qtdPassageiro].cpf,cadastro,*qtdPassageiro)!= -1){  //Verifica se esse CPF ja foi cadastrado
        printf("CPF já Cadastrado!\n"); //CPF ja cadastrado, fecha a função
        return;
    }

    printf("Nome: ");
    scanf(" %[^\n]",cadastro[*qtdPassageiro].nome);

    printf("Telefone: ");
    scanf("%s",cadastro[*qtdPassageiro].telefone);

    printf("E-mail: ");
    scanf("%s",cadastro[*qtdPassageiro].email);

    printf("Data de Nascimento: ");
    scanf("%s",cadastro[*qtdPassageiro].dataNascimento);
    formatarData(cadastro[*qtdPassageiro].dataNascimento);

    (*qtdPassageiro)++; //Soma 1 a quantidade de passageiros cadastrados
    printf("Passageiro cadastrado com sucesso!\n");
    printf("Passageiros cadastrados: %d\n",*qtdPassageiro);
}

void buscarPassageiro(Passageiro consulta[], int qtdPassageiro){ //Função que imprime na tela os dados do passageiro que teve o CPF informado
    char cpfDigitado[20]; //Variavel para armazenar o cpf digitado
    char cpfConsulta[12]; //Variavel para armazenar o cpf limpo

    if(qtdPassageiro==0){
        printf("Nenhum passageiro cadastrado!\n");
        return;
    }

    printf("------- Consulta de Passageiro -------\n");
    printf("Digite o CPF do Passageiro que deseja consultar: ");
    scanf("%s",cpfDigitado); //Recebe CPF do passageiro a ser consultado

    limparCPF(cpfDigitado,cpfConsulta); //cpfConsulta recebe o cpfDigitado com pontos e traços removidos
    if(validaCPF(cpfConsulta)==0){ //verifica se é um CPF válido
        printf("CPF Inválido!\n");
        return;
    }

    int indiceConsulta = buscaCPF(cpfConsulta,consulta,qtdPassageiro);

    if(indiceConsulta == -1){
        printf("Passageiro não cadastrado!\n");
        return;
    } else{
        printf("---Passageiro Encontrado---\n");
        printf("Nome: %s\n",consulta[indiceConsulta].nome);
        char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
        strcpy(cpfExibicao, consulta[indiceConsulta].cpf); //Copia o conteudo do CPF do vetor
        formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
        printf("CPF: %s\n",cpfExibicao);
        printf("Telefone: %s\n",consulta[indiceConsulta].telefone);
        printf("E-mail: %s\n",consulta[indiceConsulta].email);
        formatarData(consulta[indiceConsulta].dataNascimento);
        printf("Data de nascimento: %s\n",consulta[indiceConsulta].dataNascimento);
    }
}

void editarPassageiro(Passageiro editar[], int qtdPassageiro){

    char cpfDigitado[20]; //Variavel para armazenar o cpf digitado
    char cpfEditar[12]; //Variavel para armazenar o cpf do passageiro editado

    if(qtdPassageiro==0){
        printf("Nenhum passageiro cadastrado!\n");
        return;
    }

    printf("------- Editar dados do Passageiro -------\n");
    printf("Digite o CPF do Passageiro que deseja editar: ");
    scanf("%s",cpfDigitado); //Recebe CPF do passageiro a ser editado

    limparCPF(cpfDigitado,cpfEditar); //cpfEditar recebe o cpfDigitado com pontos e traços removidos
    
    if(validaCPF(cpfEditar)==0){ //verifica se é um CPF válido
        printf("CPF Inválido!\n");
        return;
    }

    limparCPF(cpfDigitado,cpfEditar);
    int indiceEditar = buscaCPF(cpfEditar,editar,qtdPassageiro);

    if(indiceEditar == -1){
        printf("Passageiro não cadastrado!\n");
        return;
    } else{
        printf("---Passageiro Encontrado---\n");
        int opcao = menuEditarPassageiro();
        switch (opcao){
        case 1:
            printf("---Editar Nome---\n");
            printf("Nome Atual: %s\n",editar[indiceEditar].nome);
            printf("Novo Nome: ");
            scanf(" %[^\n]",editar[indiceEditar].nome);
            break;
        case 2:
            printf("---Editar Telefone---\n");
            printf("Telefone Atual: %s\n",editar[indiceEditar].telefone);
            printf("Novo Telefone: ");
            scanf(" %[^\n]",editar[indiceEditar].telefone);
            break;
        case 3:
            printf("---Editar E-mail---\n");
            printf("E-mail Atual: %s\n",editar[indiceEditar].email);
            printf("Novo E-mail: ");
            scanf(" %[^\n]",editar[indiceEditar].email);
            break;
        case 4:
            printf("---Editar Data de Nascimento---\n");
            formatarData(editar[indiceEditar].dataNascimento);
            printf("Data de Nascimento Atual: %s\n",editar[indiceEditar].dataNascimento);
            printf("Nova Data de Nascimento: ");
            scanf(" %[^\n]",editar[indiceEditar].dataNascimento);
            break;
        default:
            break;
        }
        
    }
}

Passageiro *deletarPassageiro(Passageiro deletar[], int *qtdPassageiro, int *passageirosAlocados, Passagem passagens[], int qtdPassagens){
    
    if(*qtdPassageiro==0){
        printf("Nenhum passageiro cadastrado!\n");
        return deletar;
    }
    
    char cpfDigitado[20], cpfDeletar[15];
    printf("------- Deletar Passageiro -------\n");
    printf("Digite o CPF do Passageiro que deseja excluir: ");
    scanf(" %s",cpfDigitado); //Recebe CPF do passageiro a ser deletado
 
    limparCPF(cpfDigitado,cpfDeletar); //cpfDeletado recebe o cpfDigitado com pontos e traços removidos
    
    if(validaCPF(cpfDeletar)==0){ //verifica se é um CPF válido
        printf("CPF Inválido!\n");
        return deletar;
    }
    if(buscaCPFPassagem(cpfDeletar,passagens,qtdPassagens)!=-1){
        printf("Operação Negada!\n");
        printf("Passageiro vinculado a uma Passagem\n");
        return deletar;
    }

    int indiceDeletar = buscaCPF(cpfDeletar,deletar,*qtdPassageiro);


    if(indiceDeletar == -1){
        printf("Passageiro não cadastrado!\n");
        return deletar;
    } else{
        printf("---Passageiro Encontrado---\n");
        char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
        strcpy(cpfExibicao, deletar[indiceDeletar].cpf); //Copia o conteudo do CPF do vetor
        formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
        char confirmar; //Variavel para confirmar a exclusão do passageiro

    do {
    printf("Confirma a exclusão do passageiro: %s (CPF: %s) [S/N]",deletar[indiceDeletar].nome,cpfExibicao);
    scanf(" %c", &confirmar);
    confirmar = toupper(confirmar);
    if (confirmar != 'S' && confirmar != 'N'){
        printf("Opcao invalida! Digite apenas S ou N.\n");
    }
    } while (confirmar != 'S' && confirmar != 'N');
    
    if(confirmar == 'S'){
        int cont;
        for(cont=indiceDeletar;cont<*qtdPassageiro-1;cont++){
            deletar[cont]=deletar[cont+1];//Desloca os elementos do vetor para a esquerda, assim excluindo as informações do passageiro selecionado
        }

        (*qtdPassageiro)--; //Diminui a quantidade de passageiros cadastrados devido a exclusção
        deletar[*qtdPassageiro].nome[0] = '\0';
        deletar[*qtdPassageiro].cpf[0] = '\0';
        deletar[*qtdPassageiro].telefone[0] = '\0';
        deletar[*qtdPassageiro].email[0] = '\0';
        deletar[*qtdPassageiro].dataNascimento[0] = '\0';
        (*passageirosAlocados)--;
                Passageiro *aux = realloc(deletar,(*passageirosAlocados)*sizeof(Passageiro));
                if(aux==NULL){
                        printf("Erro ao realocar memoria! (Passageiros/Exclusao)\n");
                        (*passageirosAlocados)++;
                    }else{
                        deletar = aux;
                    }
        printf("Passageiro Deletado\n");
        printf("Passageiros Cadastrados: %d\n",*qtdPassageiro);
        //Apaga as informações do ultimo passageiro, ja que seus dados estavam duplicados devido ao deslocamento
    } else{
        printf("Exclusão Cancelada!\n");
    }

    }
    return deletar;
}

void listarPassageiros(Passageiro listar[], int qtdPassageiros){

    int cont;

    if(qtdPassageiros == 0){
        printf("Nenhum passageiro cadastrado!\n");
        return;
    }

    for(cont=0;cont<qtdPassageiros;cont++){

        
        printf("\n----- PASSAGEIRO %d -----\n",cont+1);

        printf("Nome: %s\n",listar[cont].nome);
        char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
        strcpy(cpfExibicao, listar[cont].cpf); //Copia o conteudo do CPF do vetor
        formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
        printf("CPF: %s\n",cpfExibicao);
        printf("Telefone: %s\n",listar[cont].telefone);
        printf("E-mail: %s\n",listar[cont].email);
        formatarData(listar[cont].dataNascimento);
        printf("Data de nascimento: %s\n",listar[cont].dataNascimento);
    }
}

void cadastrarVoo(Voo voos[], int *qtdVoos){
    printf("\n--- Cadastro de Voo ---\n");

    printf("Codigo do voo: ");
    scanf("%s", voos[*qtdVoos].codigo_voo);

    limparCodVoo(voos[*qtdVoos].codigo_voo);

    if(validacodigo_voo(voos[*qtdVoos].codigo_voo) == 0){
        return;
    }

    if(buscaVoo(voos[*qtdVoos].codigo_voo, voos, *qtdVoos) != -1){
        printf("Codigo ja cadastrado!\n");
        return;
    }
    //Seleciona a companhia aerea automaticamente com base no digito verificador do código de voo
    if(voos[*qtdVoos].codigo_voo[0] == 'A'){
        strcpy(voos[*qtdVoos].companhia, "Azul");
    }else if(voos[*qtdVoos].codigo_voo[0] == 'G'){
        strcpy(voos[*qtdVoos].companhia, "Gol");
    }else{
        strcpy(voos[*qtdVoos].companhia, "Latam");
    }
    printf("Companhia selecionada: %s\n", voos[*qtdVoos].companhia);

    printf("Origem: ");
    scanf(" %[^\n]", voos[*qtdVoos].origem);

    printf("Destino: ");
    scanf(" %[^\n]", voos[*qtdVoos].destino);

    printf("Data/Hora: ");
    scanf(" %[^\n]", voos[*qtdVoos].dataHora);

    printf("Duracao: ");
    scanf(" %[^\n]", voos[*qtdVoos].duracao);

    (*qtdVoos)++;

    printf("Voo cadastrado com sucesso!\n");
}

void consultarVoo(Voo voos[], int qtdVoos){

    char codigo[10];

    printf("\nDigite o codigo do voo: ");
    scanf("%s", codigo);

    limparCodVoo(codigo);

    int indice = buscaVoo(codigo, voos, qtdVoos);

    if(indice == -1){
        printf("Voo nao encontrado!\n");
        return;
    }

    printf("\nCodigo: %s\n", voos[indice].codigo_voo);
    printf("Origem: %s\n", voos[indice].origem);
    printf("Destino: %s\n", voos[indice].destino);
    printf("Data/Hora: %s\n", voos[indice].dataHora);
    printf("Duracao: %s\n", voos[indice].duracao);
    printf("Companhia: %s\n", voos[indice].companhia);
}

void editarVoo(Voo voos[], int qtdVoos){

    char codigo[10];

    printf("Digite o codigo do voo: ");
    scanf("%s", codigo);

    limparCodVoo(codigo);

    int indice = buscaVoo(codigo, voos, qtdVoos);

    if(indice == -1){
        printf("Voo nao encontrado!\n");
        return;
    }

    switch(menuEditarVoo()){

        case 1:
            printf("Nova origem: ");
            scanf(" %[^\n]", voos[indice].origem);
            break;

        case 2:
            printf("Novo destino: ");
            scanf(" %[^\n]", voos[indice].destino);
            break;

        case 3:
            printf("Nova data/hora: ");
            scanf(" %[^\n]", voos[indice].dataHora);
            break;

        case 4:
            printf("Nova duracao: ");
            scanf(" %[^\n]", voos[indice].duracao);
            break;

        case 5:
            printf("Nova companhia: ");
            scanf(" %[^\n]", voos[indice].companhia);
            break;

        default:
            printf("Opcao invalida!\n");
    }
}

Voo *deletarVoo(Voo voos[], int *qtdVoos, int *voosAlocados, Passagem passagens[], int *qtdPassagem){

    char codigo[10];

    printf("Digite o codigo do voo: ");
    scanf("%s", codigo);

    limparCodVoo(codigo);

    int indice = buscaVoo(codigo, voos, *qtdVoos);
    if(indice == -1){
        printf("Voo nao encontrado!\n");
        return voos;
    }
    int indice2 = buscaPassagemPorVoo(codigo, passagens, *qtdPassagem);
    if(indice2 != -1){
        printf("Voo com passagens cadastradas nao podem ser apagados!\nDesvincule todas as passagens desse voo primeiro!\n");
        return voos;
    }

    for(int i = indice; i < *qtdVoos - 1; i++){
        voos[i] = voos[i + 1];
    }

    (*qtdVoos)--;
    (*voosAlocados)--;
    Voo *aux = realloc(voos,(*voosAlocados)*sizeof(Voo));
    if(aux==NULL){
        printf("Erro ao realocar memoria! (Voos/Exclusao)\n");
       (*voosAlocados)++;
    }else voos = aux;
    printf("Voo removido com sucesso!\n");
    return voos;
}

void listarVoos(Voo voos[], int qtdVoos){

    int i;

    if(qtdVoos == 0){
        printf("Nenhum voo cadastrado!\n");
        return;
    }

    for(i = 0; i < qtdVoos; i++){

        printf("\n===== VOO %d =====\n", i + 1);

        printf("Codigo: %s\n", voos[i].codigo_voo);
        printf("Origem: %s\n", voos[i].origem);
        printf("Destino: %s\n", voos[i].destino);
        printf("Data/Hora: %s\n", voos[i].dataHora);
        printf("Duracao: %s\n", voos[i].duracao);
        printf("Companhia: %s\n", voos[i].companhia);
    }
}

Passageiro *menuPassageiros(Passageiro passageiros[], int *qtdPassageiro, int *passageirosAlocados, Passagem passagens[], int qtdPassagens){
    int opcao;
    do{
        printf("\n===== MENU PASSAGEIROS =====\n");
        printf("1 - Cadastrar Passageiro\n");
        printf("2 - Consultar Passageiro\n");
        printf("3 - Editar Passageiro\n");
        printf("4 - Deletar Passageiro\n");
        printf("5 - Listar Passageiros\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        if(scanf("%d", &opcao) != 1){
            while(getchar() != '\n'); // limpa o buffer
            opcao = -1; // garante que não saia do menu
        }

        switch(opcao){
            case 1:
                if(*qtdPassageiro==*passageirosAlocados){
                    (*passageirosAlocados)+=5;
                    Passageiro *aux = realloc(passageiros,(*passageirosAlocados)*sizeof(Passageiro));
                    if(aux==NULL){
                        printf("Erro ao realocar memoria! (Passageiros/Cadastro)\n");
                        (*passageirosAlocados) -= 5;
                    }else{
                        passageiros = aux;
                    }

                }
                cadastrarPassageiro(passageiros, qtdPassageiro);
                salvarPassageiro(passageiros,*qtdPassageiro);
                break;

            case 2:
                buscarPassageiro(passageiros, *qtdPassageiro);
                break;

            case 3:
                editarPassageiro(passageiros, *qtdPassageiro);
                salvarPassageiro(passageiros,*qtdPassageiro);
                break;

            case 4:
                passageiros = deletarPassageiro(passageiros, qtdPassageiro, passageirosAlocados,passagens,qtdPassagens);
                salvarPassageiro(passageiros,*qtdPassageiro);
                break;

            case 5:
                listarPassageiros(passageiros, *qtdPassageiro);
                break;
            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    }while(opcao != 0);
    return passageiros;
}

Voo *menuVoos(Voo voos[], int *qtdVoos, int *voosAlocados, Passagem passagens[], int *qtdPassagem){

    int opcao;

    do{

        printf("\n===== MENU VOOS =====\n");
        printf("1 - Cadastrar Voo\n");
        printf("2 - Consultar Voo\n");
        printf("3 - Editar Voo\n");
        printf("4 - Remover Voo\n");
        printf("5 - Listar Voos\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        if(scanf("%d", &opcao) != 1){ //Evita o loop infinito caso o digitado pelo usuario não seja um inteiro
            while(getchar() != '\n'); // limpa o buffer
            opcao = -1; // garante que não saia do menu
        }

        switch(opcao){

            case 1:
                if(*qtdVoos==*voosAlocados){
                    (*voosAlocados)+=5;
                    Voo *aux = realloc(voos,(*voosAlocados)*sizeof(Voo));
                    if(aux==NULL){
                        printf("Erro ao realocar memoria! (Voos/Cadastro)\n");
                        (*voosAlocados) -= 5;
                    }else{
                        voos = aux;
                    }

                }
                cadastrarVoo(voos, qtdVoos);
                salvarVoo(voos,*qtdVoos);
                break;

            case 2:
                consultarVoo(voos, *qtdVoos);
                break;

            case 3:
                editarVoo(voos, *qtdVoos);
                salvarVoo(voos,*qtdVoos);
                break;

            case 4:
                voos = deletarVoo(voos, qtdVoos,voosAlocados, passagens, qtdPassagem);
                salvarVoo(voos,*qtdVoos);
                break;

            case 5:
                listarVoos(voos, *qtdVoos);
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    }while(opcao != 0);
    return voos;
}
void carregarPassagem(Passagem passagens[],int *qtdPassagem, int *proximoNumero){

    FILE *arquivo;
    arquivo = fopen("passagensSalvas.bin","rb");

    if(arquivo == NULL){
    printf("Erro ao abrir passagensSalvos.bin\n");
    *qtdPassagem = 0;
    *proximoNumero = 1000;
    return;
    }
    fread(qtdPassagem, sizeof(int), 1, arquivo);
    fread(proximoNumero, sizeof(int), 1, arquivo);
    fread(passagens, sizeof(Passagem), *qtdPassagem, arquivo);
    printf("Quantidade de Passagens lidas: %d\n", *qtdPassagem);
    printf("Dados das Passagens carregadas!\n");
    fclose(arquivo);
}
void listarPassagem(Passagem passagens[], int qtdPassagem){
    int cont;

    if(qtdPassagem == 0){
        printf("Nenhuma passagem cadastrada!\n");
        return;
    }

    for(cont=0;cont<qtdPassagem;cont++){        
        printf("\n----- PASSAGEM %d -----\n",passagens[cont].num_passagem);
        char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
        strcpy(cpfExibicao, passagens[cont].cpf); //Copia o conteudo do CPF do vetor
        formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
        printf("CPF: %s\n",cpfExibicao);

        printf("Codigo do Voo: %s\n",passagens[cont].codigo_voo);
        printf("Assento: %s\n",passagens[cont].assentos);
        printf("Classe: %s\n",passagens[cont].classe);
        printf("Status: %s\n",passagens[cont].status);
    }
}
int menuEditarPassagem(){
    int menu;
    printf("---O que deseja Editar?---\n");
    printf("1. Assento e/ou Classe\n2. Status\nOpção Selecionada: ");
    scanf("%d",&menu);
    switch (menu)
    {
    case 1 :
        return 1;
    case 2 :
        return 2;
    default:
        printf("Entrada Inválida!\n");
        return -1;
    }
    
}
int buscaNum (int numero_digitado,Passagem passagens[], int qtdPassagem){
    for (int i=0;i<qtdPassagem;i++){
        if(passagens[i].num_passagem==numero_digitado) return i;
    }
    return -1;
}

int validaClasse(char assentos[], int op){
    if(op!=1){
        if(assentos[1]=='3' && assentos[2]>'2')return 1;
        else if(assentos[1]=='4' && assentos[2]=='0')return 1;
    }
    else{
        if(assentos[1]=='3' && assentos[2]<='2')return 1;
        else if(assentos[1]=='2' && assentos[2]<='9')return 1;
        else if(assentos[1]=='1' && assentos[2]<='9')return 1;
        else if(assentos[1]<='9' && assentos[2]=='\0')return 1;
    }
    return 0;
}
void salvarPassagem(Passagem passagens[], int qtdPassagem, int proximoNumero){

    FILE *arquivo;
    arquivo = fopen("passagensSalvas.bin","wb");
    if(arquivo == NULL){
        printf("Erro ao salvar os dados das passagens!\n");
        return;
    }
    fwrite(&qtdPassagem, sizeof(int), 1, arquivo);
    fwrite(&proximoNumero, sizeof(int), 1, arquivo);
    fwrite(passagens, sizeof(Passagem), qtdPassagem, arquivo);
    fclose(arquivo);
    printf("Operacao Salva!\n");
}
int buscaAssento(char assentos[],char codigo_voo[], Passagem passagens[], int qtdPassagem, int ignorar){
    for (int i=0; i<qtdPassagem;i++){
        if(i == ignorar) continue;
        if((strcmp(passagens[i].assentos,assentos)==0) && (strcmp(passagens[i].codigo_voo,codigo_voo)==0)) return 0;
    }
    return 1;
}
int validaAssento(char assentos[]){
    if(assentos[0]<'A' || assentos[0]>'D'){
        return 0;}
    else if(assentos[1]<'1' || assentos[1]>'9'){
        return 0;}
    else if (assentos[1] == '4' && assentos[2] == '0' && assentos[3] == '\0') {
        return 1;}
    else if(assentos[1]>'3' && assentos[2]!='\0'){
        return 0;}
    else if(assentos[2]=='\0') {
        return 1;}
    else if(assentos[2]<'0' || assentos[2]>'9'){
        return 0;}
    else if (assentos[3] == '\0'){
    return 1;}
    else return 0;
}
void cadastrarPassagem(Passageiro passageiros[],Voo voos[],Passagem passagens[],int *qtdPassagem,int *qtdPassageiro,int *qtdVoos,int *proximoNumero){
    printf("------- Cadastro de Nova Passagem -------\n");
    passagens[*qtdPassagem].num_passagem = *proximoNumero;
    printf("Passagem n. %d\n",passagens[*qtdPassagem].num_passagem);
    char cpfDigitado[20];
    printf("CPF: ");
    scanf("%s",cpfDigitado);
    limparCPF(cpfDigitado, passagens[*qtdPassagem].cpf);
    if(validaCPF(passagens[*qtdPassagem].cpf) == 0){
        printf("CPF Invalido!\n");
        return;
    } 
    if(buscaCPF(passagens[*qtdPassagem].cpf,passageiros,*qtdPassageiro)== -1){  //Verifica se esse CPF ja foi cadastrado
        printf("CPF nao encontrado em Passageiros!\n"); //CPF ja cadastrado, fecha a função
        return;
    }

    printf("Codigo do voo: ");
    scanf("%s", passagens[*qtdPassagem].codigo_voo);
    limparCodVoo(passagens[*qtdPassagem].codigo_voo);
    if(validacodigo_voo(passagens[*qtdPassagem].codigo_voo) == 0){
        return;
    }
    if(buscaVoo(passagens[*qtdPassagem].codigo_voo, voos, *qtdVoos) == -1){
        printf("Codigo nao encontrado em Voos!\n");
        return;
    }

    printf("!!!Lembre-se:\nAs fileiras de 1 a 32 sao para Economicas ou Executivas.\nAlem disso, as fileiras de 33 a 40 sao Primeira Classe ou Executivas\n\nAssento:");
    scanf("%s",passagens[*qtdPassagem].assentos);
    if(validaAssento(passagens[*qtdPassagem].assentos)!=1){
        printf("Assento inválido!\n");
        return;
    }
    if(buscaAssento(passagens[*qtdPassagem].assentos,passagens[*qtdPassagem].codigo_voo, passagens, *qtdPassagem,-1)!=1){
        printf("Assento ocupado nesse Voo!\n");
        return;
    }
    printf("--Classe--\n");
    printf("1. Economica\n");
    printf("2. Executiva\n");
    printf("3. Primeira Classe\n");
    printf("Opcao:");
    int op;
    scanf("%d",&op);
    if(op<1 || op>3){printf("Classe Invalida\n");
        return;
    }
    if(op==1) strcpy(passagens[*qtdPassagem].classe,"Economica");
    if(op==2) strcpy(passagens[*qtdPassagem].classe,"Executiva");
    if(op==3) strcpy(passagens[*qtdPassagem].classe,"Primeira Classe");
    if(validaClasse(passagens[*qtdPassagem].assentos, op)!=1){
        printf("Classe incompativel com o assento!\n");
        return;
    }

    printf("--Status--\n");
    printf("1. Confirmada\n");
    printf("2. Cancelada\n");
    printf("3. Embarcada\n");
    printf("Opcao:");
    int op2;
    scanf("%d",&op2);
    if(op2<1 || op2>3){printf("Status Inválido\n");return;}
    if(op2==1) strcpy(passagens[*qtdPassagem].status,"Confirmada");
    if(op2==2) strcpy(passagens[*qtdPassagem].status,"Cancelada");
    if(op2==3) strcpy(passagens[*qtdPassagem].status,"Embarcada");
    
    (*proximoNumero)++;
    (*qtdPassagem)++; //Soma 1 a quantidade de passageiros cadastrados
    printf("Passagem cadastrada com sucesso!\n");
    printf("Passagens cadastradas: %d\n",*qtdPassagem);
}
void buscarPassagem(Passagem passagens[], int qtdPassagem){
    int numero_digitado; 

    if(qtdPassagem==0){
        printf("Nenhuma passagem cadastrada!\n");
        return;
    }

    printf("------- Consulta da Passagem -------\n");
    printf("Digite o numero da Passagem que deseja consultar: ");
    scanf("%d",&numero_digitado);
    if(numero_digitado<1000 || numero_digitado>9999){
        printf("Numero Invalido!\n");
        return;
    }

    int indiceConsulta = buscaNum(numero_digitado,passagens,qtdPassagem);

    if(indiceConsulta == -1){
        printf("Passagem não cadastrada!\n");
        return;
    } else{
        printf("---Passagem Encontrada---\n");
        printf("Numero: %d\n",passagens[indiceConsulta].num_passagem);
        char cpfExibicao[15];
        strcpy(cpfExibicao, passagens[indiceConsulta].cpf);
        formatarCPF(cpfExibicao);
        printf("CPF: %s\n",cpfExibicao);
        printf("Codigo do Voo: %s\n",passagens[indiceConsulta].codigo_voo);
        printf("Assento: %s\n",passagens[indiceConsulta].assentos);
        printf("Classe: %s\n",passagens[indiceConsulta].classe);
        printf("Status: %s\n",passagens[indiceConsulta].status);
    }
}
void editarPassagem(Passagem passagens[], int qtdPassagem){
    int numero_digitado;

    if(qtdPassagem==0){
        printf("Nenhuma passagem cadastrada!\n");
        return;
    }

    printf("------- Editar dados da Passagem -------\n");
    printf("Digite o Numero da Passagem que deseja editar: ");
    scanf("%d",&numero_digitado);
    
    if(numero_digitado<1000 || numero_digitado>9999){
        printf("Numero Invalido!\n");
        return;
    }

    int indiceEditar = buscaNum(numero_digitado,passagens,qtdPassagem);

    if(indiceEditar == -1){
        printf("Passagem não cadastrada!\n");
        return;
    } else{
        printf("---Passagem Encontrada---\n");
        int opcao = menuEditarPassagem();
        switch (opcao){
        case 1:
            printf("---Editar Assento ou/e Classe---\n");
            printf("Assento Atual: %s\n",passagens[indiceEditar].assentos);
            printf("Classe Atual: %s\n",passagens[indiceEditar].classe);
            printf("!!!Lembre-se:\nAs fileiras de 1 a 32 sao para Economicas ou Executivas.\nAlem disso, as fileiras de 33 a 40 sao Primeira Classe ou Executivas\n\nAssentos:\n");
            printf("Digite o assento atual ou o assento novo: ");
            char copiaAssento[5];
            scanf("%4s",copiaAssento);
            if(validaAssento(copiaAssento)!=1){
            printf("Assento inválido!\n");
            return;}
            if(buscaAssento(copiaAssento,passagens[indiceEditar].codigo_voo, passagens, qtdPassagem,indiceEditar)!=1){
                printf("Assento ocupado neste Voo. Consulte qual Passagem esta ocupando esse assento e edite ela primmeiro!\n");
                return;
            }
            printf("Digite a classe atual ou a classe nova: ");
            printf("\nEconomica = 1\nExecutiva = 2\nPrimeira Classe = 3\n");
            int op;
            scanf("%d",&op);
            if(op<1 || op>3){printf("Classe Invalida\n");
                return;
            }
            char copiaClasse[20];
            if(op==1) strcpy(copiaClasse,"Economica");
            if(op==2) strcpy(copiaClasse,"Executiva");
            if(op==3) strcpy(copiaClasse,"Primeira Classe");
            if(validaClasse(copiaAssento, op)!=1){
                printf("Classe incompativel com o assento!\n");
                return;
            }
            strcpy(passagens[indiceEditar].classe,copiaClasse);
            strcpy(passagens[indiceEditar].assentos,copiaAssento);

            break;
        case 2:
            printf("---Editar Status---\n");
            printf("Status Atual: %s\n",passagens[indiceEditar].status);
            printf("Para o novo status, digite:\nConfirmada = 1\nCancelada = 2\nEmbarcada = 3\n");
            int op2;
            scanf("%d",&op2);
            if(op2<1 || op2>3){printf("Status Invalido\n");return;}
            if(op2==1) strcpy(passagens[indiceEditar].status,"Confirmada");
            if(op2==2) strcpy(passagens[indiceEditar].status,"Cancelada");
            if(op2==3) strcpy(passagens[indiceEditar].status,"Embarcada");
            break;
        default:
            break;
        }
        
    }
}

void relatorioCompanhia(Passagem passagens[], int qtdPassagens,Passageiro passageiros[], int qtdPassageiros,Voo voos[], int qtdVoos){
    printf("---Gerar relatorio (Companhia Aerea)---\n");
    int comp;
    char companhia[50];
    int qtdPassagensEcontradas=0;
    do {
    printf("Selecione uma companhia:\n");
    printf("1. Azul\n");
    printf("2. Gol\n");
    printf("3. Latam\n");
    printf("0. Voltar\n");
    printf("Opcao: ");

    if (scanf("%d", &comp) != 1) {
        printf("Entrada invalida!\n");

        while (getchar() != '\n'); // limpa o buffer
        comp = -1;
    }
    else if (comp < 0 || comp > 3) {
        printf("Entrada invalida!\n");
    }

    } while (comp < 0 || comp > 3);

    if (comp == 0) {
    return;
    }
    else if (comp == 1) {
    strcpy(companhia, "Azul");
    printf("Gerando Relatorio (Azul)...\n");
    }
    else if (comp == 2) {
    strcpy(companhia, "Gol");
    printf("Gerando Relatorio (Gol)...\n");
    }
    else if (comp == 3) {
    strcpy(companhia, "Latam");
    printf("Gerando Relatorio (Latam)...\n");
    }
    FILE *relatorioCompanhia;
    relatorioCompanhia = fopen("RelatorioCompanhia.txt","w");
    if (relatorioCompanhia == NULL)
    {
        printf("Erro ao criar arquivo!\n");
        return;
    }

    fprintf(relatorioCompanhia, "========================================\n");
    fprintf(relatorioCompanhia, "     RELATORIO DE PASSAGENS AEREAS\n");
    fprintf(relatorioCompanhia, "               (%s)\n",companhia);
    fprintf(relatorioCompanhia, "========================================\n\n");

    for(int i=0;i<qtdPassagens;i++){
        if(passagens[i].codigo_voo[0]==companhia[0]){
            qtdPassagensEcontradas++;
            fprintf(relatorioCompanhia,"----------------------------------------------------\n");
            fprintf(relatorioCompanhia,"Numero da passagem: %d\n",passagens[i].num_passagem);
            int indicePassageiro = buscaCPF(passagens[i].cpf, passageiros, qtdPassageiros);
            fprintf(relatorioCompanhia,"Nome: %s\n",passageiros[indicePassageiro].nome);
            char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
            strcpy(cpfExibicao, passagens[i].cpf); //Copia o conteudo do CPF do vetor
            formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
            fprintf(relatorioCompanhia,"CPF: %s\n",cpfExibicao);
            fprintf(relatorioCompanhia,"Codigo de Voo: %s\n",passagens[i].codigo_voo);
            int indiceVoo = buscaVoo(passagens[i].codigo_voo,voos,qtdVoos);
            fprintf(relatorioCompanhia,"Origem: %s\n",voos[indiceVoo].origem);
            fprintf(relatorioCompanhia,"Destino: %s\n",voos[indiceVoo].destino);
            fprintf(relatorioCompanhia,"Assento: %s\n",passagens[i].assentos);
            fprintf(relatorioCompanhia,"Classe: %s\n",passagens[i].classe);
            fprintf(relatorioCompanhia,"Classe: %s\n",passagens[i].status);
        }
        
    }
    fprintf(relatorioCompanhia,"-----------------------------------------\n");
    fprintf(relatorioCompanhia,"Passagens Encontradas: %d\n",qtdPassagensEcontradas);
    printf("Relatorio Gerado!\n");
    fclose(relatorioCompanhia);

}

void relatorioDestino(Passagem passagens[], int qtdPassagens, Passageiro passageiros[], int qtdPassageiros, Voo voos[], int qtdVoos){

    char (*destinos)[50]; //Vetor de Vetor que armazena por linha os nomes das cidades de origem sem repetição
    destinos = malloc(qtdVoos * sizeof(*destinos));

    if(destinos == NULL){
        printf("Erro de alocacao(Relatorio/Destino)!\n");
        return;
    }
    int qtdDestinos = 0;
    for(int i=0;i<qtdVoos;i++){
        int existe = 0;
        for(int j=0;j<qtdDestinos;j++){
            if(strcmp(voos[i].destino,destinos[j])==0){
                existe = 1;
                break;
            }

        }
        if(existe==0){
            strcpy(destinos[qtdDestinos],voos[i].destino);
            qtdDestinos++;
        }
    }

    printf("\n----------------------- Destinos Disponiveis ----------------------\n");
    for(int i = 0; i < qtdDestinos; i++){
        printf("%2d. %-20s", i + 1, destinos[i]);//faz com que sejam impressos 3 destinos por linha
        if((i + 1) % 3 == 0){
            printf("\n");
        }
    }

if(qtdDestinos % 3 != 0){
    printf("\n");
}
    int opcao;
    int retorno;
    do{
        printf("Opcao: ");
        retorno = scanf("%d", &opcao);
        if(retorno != 1){
            printf("Digite apenas numeros!\n");
            while(getchar() != '\n'); // limpa buffer
            opcao = -1;
        }
    }while(opcao < 1 || opcao > qtdDestinos);
    char destinoEscolhido[50];
    strcpy(destinoEscolhido, destinos[opcao - 1]);
    printf("Gerando Relatorio (%s)...\n",destinoEscolhido);
    FILE *relatorioDestino;
    relatorioDestino = fopen("RelatorioDestino.txt","w");
    fprintf(relatorioDestino, "========================================\n");
    fprintf(relatorioDestino, "     RELATORIO DE PASSAGENS AEREAS\n");
    fprintf(relatorioDestino, "               (%s)\n",destinoEscolhido);
    fprintf(relatorioDestino, "========================================\n\n");
    int totalPassagensDestino=0;
    for(int k=0;k<qtdPassagens;k++){
        int indiceVoo = buscaVoo(passagens[k].codigo_voo, voos, qtdVoos);
        if(indiceVoo == -1){
            continue;
        }
        if(strcmp(voos[indiceVoo].destino,destinoEscolhido)==0){
            totalPassagensDestino++;
            int indicePassageiro = buscaCPF(passagens[k].cpf,passageiros,qtdPassageiros);
            if(indicePassageiro == -1){
                continue;
            }
            fprintf(relatorioDestino,"----------------------------------------\n");
            fprintf(relatorioDestino,"Número da Passagem: %d\n",passagens[k].num_passagem);
            fprintf(relatorioDestino,"Nome: %s\n",passageiros[indicePassageiro].nome);
            char cpfExibicao[15]; //Cria uma variavel temporaria para armazenar o CPF que sera mostrado, para não editar o conteudo do vetor principal
            strcpy(cpfExibicao, passagens[k].cpf); //Copia o conteudo do CPF do vetor
            formatarCPF(cpfExibicao); //Formata somente o cpf a ser exibido no formato XXX.XXX.XXX-XX
            fprintf(relatorioDestino,"CPF: %s\n",cpfExibicao);
            fprintf(relatorioDestino,"Companhia: %s\n",voos[indiceVoo].companhia);
            fprintf(relatorioDestino, "Origem: %s\n",voos[indiceVoo].origem);
            fprintf(relatorioDestino,"Assento: %s\n",passagens[k].assentos);
            fprintf(relatorioDestino,"Classe: %s\n",passagens[k].classe);
            fprintf(relatorioDestino,"Status: %s\n\n",passagens[k].status);
        }
    }
    fprintf(relatorioDestino,"----------------------------------------\n");
    fprintf(relatorioDestino,"Passagens para %s: %d\n",destinoEscolhido,totalPassagensDestino);
    free(destinos);
    fclose(relatorioDestino);
}

Passagem *deletarPassagem(Passagem passagens[], int *qtdPassagem, int *passagensAlocadas){
    int numero_digitado;
    printf("Digite o numero da passagem a ser deletada: ");
    scanf("%d",&numero_digitado);
    if(*qtdPassagem==0){
        printf("Nenhuma passagem cadastrada!\n");
        return passagens;
    }
    if(numero_digitado<1000 || numero_digitado>9999){
        printf("Numero Invalido!\n");
        return passagens;
    }
    int indice=buscaNum (numero_digitado,passagens,*qtdPassagem);
    if (indice==-1){
    printf("Passagem nao encontrada!\n");
    return passagens;
    }else{
        for(int cont=indice;cont<*qtdPassagem-1;cont++){
            passagens[cont]=passagens[cont+1];//Desloca os elementos do vetor para a esquerda, assim excluindo as informações do passageiro selecionado
        }

        (*qtdPassagem)--;
        passagens[*qtdPassagem].num_passagem=0;
        passagens[*qtdPassagem].cpf[0] = '\0';
        passagens[*qtdPassagem].codigo_voo[0] = '\0';
        passagens[*qtdPassagem].assentos[0] = '\0';
        passagens[*qtdPassagem].classe[0] = '\0';
        passagens[*qtdPassagem].status[0] = '\0';
        (*passagensAlocadas)--;
                Passagem *aux = realloc(passagens,(*passagensAlocadas)*sizeof(Passagem));
                if(aux==NULL){
                        printf("Erro ao realocar memoria! (Passagens/Exclusao)\n");
                        (*passagensAlocadas)++;
                    }else{
                        passagens = aux;
                    }

        printf("Passagem Deletada\n");
        printf("Passagens Cadastradas: %d\n",*qtdPassagem);
    }
    return passagens;
}
Passagem *menuPassagens(Passageiro passageiros[],int *qtdPassageiro,Voo voos[],int *qtdVoos,Passagem passagens[],int *qtdPassagem, int *proximoNumero, int *passagensAlocadas){
    int opcao;
    do{
        printf("\n===== MENU PASSAGENS =====\n");
        printf("1 - Cadastrar Passagem\n");
        printf("2 - Consultar Passagem\n");
        printf("3 - Editar Passagem\n");
        printf("4 - Deletar Passagem\n");
        printf("5 - Listar Passagem\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        if(scanf("%d", &opcao) != 1){ //Evita o loop infinito caso o digitado pelo usuario não seja um inteiro
            while(getchar() != '\n'); // limpa o buffer
            opcao = -1; // garante que não saia do menu
        }

        switch(opcao){
            case 1:
                if(*qtdPassagem==*passagensAlocadas){
                    (*passagensAlocadas)+=5;
                    Passagem *aux = realloc(passagens,(*passagensAlocadas)*sizeof(Passagem));
                    if(aux==NULL){
                        printf("Erro ao realocar memoria! (Passagens/Cadastro)\n");
                        (*passagensAlocadas) -= 5;
                    }else{
                        passagens = aux;
                    }

                }
                cadastrarPassagem(passageiros,voos,passagens,qtdPassagem,qtdPassageiro,qtdVoos,proximoNumero);
                salvarPassagem(passagens,*qtdPassagem,*proximoNumero);
                break;

            case 2:
                buscarPassagem(passagens, *qtdPassagem);
                break;

            case 3:
                editarPassagem(passagens, *qtdPassagem);
                salvarPassagem(passagens,*qtdPassagem,*proximoNumero);
                break;

            case 4:
                passagens = deletarPassagem(passagens, qtdPassagem, passagensAlocadas);
                salvarPassagem(passagens,*qtdPassagem,*proximoNumero);
                break;

            case 5:
                listarPassagem(passagens, *qtdPassagem);
                break;
            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    }while(opcao != 0);
    return passagens;
}
/*void menuConsultas(){
     int opcao;

    do{
        printf("\n===== CONSULTAS =====\n");
        printf("1 - Consultar por Codigo do Voo\n");
        printf("2 - Consultar por CPF do Passageiro\n");
        printf("3 - Consultar por Status\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        scanf("%d",&opcao);

        switch(opcao){
            case 1:
                consultarPorVoo();
                break;

            case 2:
                consultarPorCPF();
                break;

            case 3:
                consultarPorStatus();
                break;
        }

    }while(opcao != 0);

}
void menuRelatorios(){
    int opcao;

    do{
        printf("\n===== RELATORIOS =====\n");
        printf("1 - Passagens por Companhia Aerea\n");
        printf("2 - Passagens por Destino\n");
        printf("3 - Historico de Voos por CPF\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        scanf("%d",&opcao);

        switch(opcao){
            case 1:
                relatorioCompanhia();
                break;

            case 2:
                relatorioDestino();
                break;

            case 3:
                relatorioHistoricoCPF();
                break;
        }

    }while(opcao != 0);
}
*/
int main(){
    int qtdPassageiro=0;
    FILE *arquivoPassageiros;
    arquivoPassageiros = fopen("passageirosSalvos.bin","rb");
    if(arquivoPassageiros == NULL){
    qtdPassageiro = 0;
    }else{
    fread(&qtdPassageiro, sizeof(int), 1, arquivoPassageiros);
    fclose(arquivoPassageiros);
    printf("Quantidade de Passageiros lidos: %d\n", qtdPassageiro);
    }
    int passageirosAlocados = qtdPassageiro+5;
    Passageiro *passageiros;
    passageiros = (Passageiro*) malloc(passageirosAlocados * sizeof(Passageiro));
    if(passageiros == NULL){
    printf("Erro ao alocar memoria (Passageiros)!\n");
    return 1;
    }

    int qtdVoos = 0;
    FILE *arquivoVoos;
    arquivoVoos=fopen("voosSalvos.bin","rb");
    if(arquivoVoos == NULL){
        printf("Erro ao abrir voosSalvos.bin\n");
        qtdVoos = 0;
    }else{
        fread(&qtdVoos, sizeof(int), 1, arquivoVoos);
        fclose(arquivoVoos);
        printf("Quantidade de Voos lidos: %d\n", qtdVoos);
    }
    int voosAlocados = qtdVoos+5;
    Voo *voos;
    voos = (Voo*) malloc(voosAlocados * sizeof(Voo));
    if(voos == NULL){
        printf("Erro ao alocar memoria (Voos)!\n");
        return 1;
    }

    int qtdPassagem=0;
    int proximoNumero=1000;
    Passagem *passagens;

    FILE *arquivoPassagem;
    arquivoPassagem=fopen("passagensSalvas.bin","rb");
    if(arquivoPassagem == NULL){
        printf("Erro ao abrir passagensSalvas.bin\n");
        qtdPassagem = 0;
    }else{
        fread(&qtdPassagem, sizeof(int), 1, arquivoPassagem);
        fread(&proximoNumero, sizeof(int), 1, arquivoPassagem);
        fclose(arquivoPassagem);
    }

    int passagensAlocadas = qtdPassagem + 5;
    passagens = (Passagem*) malloc(passagensAlocadas * sizeof(Passagem));
    if (passagens == NULL){
        printf("Erro ao alocar memoria (Passagens)!\n");
        return 1;
    }
    carregarPassagem(passagens,&qtdPassagem,&proximoNumero);
    int opcao;
    carregarPassageiro(passageiros, &qtdPassageiro);
    carregarVoo(voos,&qtdVoos);
    

    do{
        
        menuPrincipal();

        if(scanf("%d", &opcao) != 1){ //Evita o loop infinito caso o digitado pelo usuario não seja um inteiro
            while(getchar() != '\n'); // limpa o buffer
            opcao = -1; // garante que não saia do menu
        }

        switch(opcao){
            case 1:
                passageiros = menuPassageiros(passageiros, &qtdPassageiro,&passageirosAlocados,passagens,qtdPassagem);
                printf("Passageiros cadastrados: %d\nPassageiros Alocados: %d\n",qtdPassageiro,passageirosAlocados);
                break;

            case 2:
                voos = menuVoos(voos, &qtdVoos, &voosAlocados, passagens, &qtdPassagem);
                printf("Voos cadastrados: %d\nVoos Alocados: %d\n",qtdVoos,voosAlocados);
                break;

            case 3:
                passagens = menuPassagens(passageiros, &qtdPassageiro, voos, &qtdVoos, passagens, &qtdPassagem,&proximoNumero,&passagensAlocadas);
                printf("Passagens cadastradas: %d\nPassagens Alocadas: %d\n",qtdPassagem,passagensAlocadas);
                break;
            case 4:
               // menuConsultas();
                break;

            case 5:
                relatorioDestino(passagens,qtdPassagem,passageiros,qtdPassageiro,voos,qtdVoos);
               // menuRelatorios();
                break;

            case 0:
                printf("Encerrando sistema...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    }while(opcao!=0);
    free(passageiros);
    free(passagens);
    free(voos);
    return 0;
}
