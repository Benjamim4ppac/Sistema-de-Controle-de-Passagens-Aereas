#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int num_passagem;
    char cpf_p[14]; // Copiar do "Passageiro" ou referenciar o cpf diretamente (Pode vir a virar um ponteiro)
    char codigo_voo_p[6]; // Copiar do "Voo" ou referenciar o codigo_voo diretamente (Pode vir a virar um ponteiro)
    char assentos[5];
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

int validacodigo_voo(char codigo_voo[6]){
    limparCodVoo(codigo_voo);
    
    if(strlen(codigo_voo)!=5){//O formato esperado deve ter 5 caracteres (LXXXX)
        printf("Formato Não Correspondente\n");
        return 0;
    }
    if(codigo_voo[0]!='L' && codigo_voo[0]!='A' && codigo_voo[0]!='G'){
        printf("Companhia Áerea Não Correspondente\n");
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

void menuPrincipal(){
    printf("====================================\n");
    printf("  SISTEMA DE CONTROLE DE PASSAGENS\n");
    printf("====================================\n\n");

    printf("1 - Gerenciar Passageiros\n");
    printf("2 - Gerenciar Voos\n");
    printf("3 - Gerenciar Passagens\n");
    printf("4 - Consultas\n");
    printf("5 - Relatórios\n");
    printf("0 - Sair\n\n");

    printf("Escolha uma opcao: ");
}        

int validaCPF(char cpf[12]){ //Função para verificar a validade de um CPF
    int peso=10,cont,soma=0,resto;
    int digitoVerificador[2];
    if (strlen(cpf)!=11){ //Se o CPF não tem 11 digitos = CPF INVÁLIDO!
        printf("Formato Não Correspondente\n");
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

int buscaCPF(char cpfConsulta[12], Passageiro consulta[], int qtdPassageiro){ //Função para verificar se o CPF já existe no banco de dados de CPFs cadastrados
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

    if(*qtdPassageiro>=5){ //Se houver 5 cadastros, a função é encerrada
        printf("Vetor Cheio!\n");
        return;
    }

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

void deletarPassageiro(Passageiro deletar[], int *qtdPassageiro){
    char cpfDigitado[20], cpfDeletar[15];
    printf("------- Deletar Passageiro -------\n");
    printf("Digite o CPF do Passageiro que deseja excluir: ");
    scanf(" %s",cpfDigitado); //Recebe CPF do passageiro a ser deletado
 
    limparCPF(cpfDigitado,cpfDeletar); //cpfDeletado recebe o cpfDigitado com pontos e traços removidos

    if(validaCPF(cpfDeletar)==0){ //verifica se é um CPF válido
        printf("CPF Inválido!\n");
        return;
    }
    
    int indiceDeletar = buscaCPF(cpfDeletar,deletar,*qtdPassageiro);


    if(indiceDeletar == -1){
        printf("Passageiro não cadastrado!\n");
        return;
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
        printf("Passageiro Deletado\n");
        printf("Passageiros Cadastrados: %d\n",*qtdPassageiro);
        //Apaga as informações do ultimo passageiro, ja que seus dados estavam duplicados devido ao deslocamento
    } else{
        printf("Exclusão Cancelada!\n");
    }

    }
}

void cadastrarVoo(Voo voos[], int *qtdVoos){

    if(*qtdVoos >= 5){
        printf("Limite de voos atingido!\n");
        return;
    }

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

    printf("Origem: ");
    scanf(" %[^\n]", voos[*qtdVoos].origem);

    printf("Destino: ");
    scanf(" %[^\n]", voos[*qtdVoos].destino);

    printf("Data/Hora: ");
    scanf(" %[^\n]", voos[*qtdVoos].dataHora);

    printf("Duracao: ");
    scanf(" %[^\n]", voos[*qtdVoos].duracao);

    printf("Companhia: ");
    scanf(" %[^\n]", voos[*qtdVoos].companhia);

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

void deletarVoo(Voo voos[], int *qtdVoos){

    char codigo[10];

    printf("Digite o codigo do voo: ");
    scanf("%s", codigo);

    limparCodVoo(codigo);

    int indice = buscaVoo(codigo, voos, *qtdVoos);

    if(indice == -1){
        printf("Voo nao encontrado!\n");
        return;
    }

    for(int i = indice; i < *qtdVoos - 1; i++){
        voos[i] = voos[i + 1];
    }

    (*qtdVoos)--;

    printf("Voo removido com sucesso!\n");
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

void menuPassageiros(Passageiro passageiros[], int *qtdPassageiro){
    int opcao;
    do{
        printf("\n===== MENU PASSAGEIROS =====\n");
        printf("1 - Cadastrar Passageiro\n");
        printf("2 - Consultar Passageiro\n");
        printf("3 - Editar Passageiro\n");
        printf("4 - Deletar Passageiro\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        scanf("%d", &opcao);
        
        switch(opcao){
            case 1:
                cadastrarPassageiro(passageiros, qtdPassageiro);
                break;

            case 2:
                buscarPassageiro(passageiros, *qtdPassageiro);
                break;

            case 3:
                editarPassageiro(passageiros, *qtdPassageiro);
                break;

            case 4:
                deletarPassageiro(passageiros, qtdPassageiro);
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    }while(opcao != 0);
}

void menuVoos(){

}

void menuPassagens(){


}

void menuConsultas(){

}

void menuRelatorios(){
    
}

int main(){
    Passageiro passageiros[5];
    int qtdPassageiro=0;
    Passagem passagens[5];
    int opcao;

    do{
        menuPrincipal();
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                menuPassageiros(passageiros, &qtdPassageiro);
                break;

            case 2:
                menuVoos();
                break;

            case 3:
                menuPassagens();
                break;

            case 4:
                menuConsultas();
                break;

            case 5:
                menuRelatorios();
                break;

            case 0:
                printf("Encerrando sistema...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    }while(opcao!=0);
    
    return 0;
}
