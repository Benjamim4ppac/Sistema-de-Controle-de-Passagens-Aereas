#include <stdio.h>
#include <string.h>

int validaCPF(char cpf[12]){
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


typedef struct
{
    char nome[100];
    char cpf[12];
    char telefone[12];
    char email[50];
    char dataNascimento[11];

} Passageiro;

typedef struct {
    char assentos[5];
} Passagem;








int main(){
    printf("Gustavo Lima!\n");
    return 0;
}