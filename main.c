typedef struct passageiro
{
    char nome[100];
    char cpf[14];
    char telefone[11];
    char email[50];
    char dataNascimento[11];

};

typedef struct {
    int num_passagem;
    char cpf_p[14]; // Copiar do "Passageiro" ou referenciar o cpf diretamente (Pode vir a virar um ponteiro)
    int codigo_voo_p; // Copiar do "Voo" ou referenciar o codigo_voo diretamente (Pode vir a virar um ponteiro)
    char assentos[5];
    char classe[20]; // (Econômica, Executiva ou Primeira Classe) -> Pode virar uma opção de mútipla escolha e guardar inteiros
    char status[20]; // (Confirmada, Cancelada, Pendente) -> Pode virar uma opção de mútipla escolha e guardar inteiros
} Passagem;







#include <stdio.h>
#include <string.h>

int main(){
    Passagem passagens[5];
    return 0;
}