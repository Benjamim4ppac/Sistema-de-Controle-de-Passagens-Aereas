#include <stdio.h>
#include <string.h>

int validaCPF(char cpf[12]){ //Função para verificar a validade de um CPF
    int peso=10,cont,soma=0,resto;
    int digitoVerificador[2];
    if (strlen(cpf)!=11){ //Se o CPF não tem 11 digitos = CPF INVÁLIDO!
        return 0;
    }
    for(cont = 0; cont<11; cont++){ //Se o CPF contém digitos não numericos = CPF INVÁLIDO!
        if(cpf[cont]<'0' || cpf[cont]>'9'){
            return 0;
        }
    }
    int iguais=1;
    for (cont=1;cont<11;cont++){ //Verifica se todos os dígitos do CPF são iguais
        if (cpf[cont] != cpf[0]){
            iguais = 0;
            break;
        };
        
    }
    if(iguais == 1){ //Se todos os dígitos do CPF são iguais = CPF INVÁLIDO!
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

int existeCPF(char cpfConsulta[12], Passageiro consulta[], int qtdPassageiro){
    int cont;
    for(cont=0;cont<qtdPassageiro;cont++){
        if(strcmp(cpfConsulta,consulta[cont].cpf)==0){
            return 1; //CPF ja cadastrado
        }
    }
    return 0; //CPF não cadastrado
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

void cadastrarPassageiro(Passageiro cadastro[], int *qtdPassageiro){
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
        printf("CPF Inválido!"); //CPF invaliudo, fecha a função
        return;
    } 
   
    if(existeCPF(cadastro[*qtdPassageiro].cpf,cadastro,*qtdPassageiro)){  //Verifica se esse CPF ja foi cadastrado
        printf("CPF Já Cadastrado!\n"); //CPF ja cadastrado, fecha a função
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
    (*qtdPassageiro)++;
    printf("Passageiro cadastrado com sucesso!\n");
}

void consultarPassageiro(Passageiro *consulta){
    char cpfConsulta[12];
    printf("------- Consulta de Passageiro -------\n");
    printf("Digite o CPF do Passageiro que deseja consultar: ");
}

typedef struct
{
    char nome[100]; //Nome do  Passageiro
    char cpf[12]; //CPF do Passageiro (XXX.XXX.XXX-XX)
    char telefone[12]; //Telefone do Passageiro ((XX)XXXX-XXXX)
    char email[50]; //E-mail do Passageiro (XXXXXX@XX.com)
    char dataNascimento[11]; //DatadeNascimento (DD/MM/AAAA)

} Passageiro;

typedef struct {
    int num_passagem;
    char cpf_p[14]; // Copiar do "Passageiro" ou referenciar o cpf diretamente (Pode vir a virar um ponteiro)
    int codigo_voo_p; // Copiar do "Voo" ou referenciar o codigo_voo diretamente (Pode vir a virar um ponteiro)
    char assentos[5];
    char classe[20]; // (Econômica, Executiva ou Primeira Classe) -> Pode virar uma opção de mútipla escolha e guardar inteiros
    char status[20]; // (Confirmada, Cancelada, Pendente) -> Pode virar uma opção de mútipla escolha e guardar inteiros
} Passagem;

int main(){
    Passageiro passageiro[5];
    int qtdPassageiro=0;
    Passagem passagens[5];
    return 0;
}