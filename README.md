# Sistema de Controle de Passagens Aéreas

Trabalho da disciplina Algoritmos e Programação II (FACOM/UFU).

## Integrantes

- Pedro Henrique Pereira da Mata - 12521BSI243
- Benjamim Martins Magalhães Sena - 12521BSI246
- Gustavo Henrique Silva Lima - MATRÍCULA
- William Ferreira Rabelo - MATRÍCULA

## Descrição

O projeto consiste no desenvolvimento de um Sistema de Controle de Passagens Aéreas, responsável pelo gerenciamento de passageiros, voos e passagens.

O sistema é desenvolvido em linguagem C, seguindo os requisitos definidos para o Trabalho da disciplina.

## Arquivos

### main.c
Arquivo principal contendo todas as estruturas, menus e funções implementadas até o momento.

### Alg2_Trabalho_2026_1.pdf
Arquivo contendo o enunciado e os requisitos do trabalho. Está presente apenas para facilitar a consulta dos integrantes do grupo em relação as entregas.

## Validação de CPF e Código de Voo

### Validação do CPF

O sistema realiza a validação do CPF informado pelo usuário antes de permitir o cadastro de um passageiro.

As validações realizadas são:

- O CPF deve possuir exatamente 11 dígitos numéricos.
- Não são aceitos letras ou espaços.
- CPFs com todos os números iguais são considerados inválidos.
- O CPF informado deve possuir dígitos verificadores válidos.

Caso o usuário informe o CPF no formato com separadores, como "529.982.247-25" ou "529.982247-25", os caracteres extras são removidos antes da validação pela função limparCPF.

O CPF também é utilizado como campo de valor único, portanto não é permitido cadastrar dois passageiros com o mesmo CPF.

Exemplos de CPFs:

CPFs válidos:
- 529.982.247-25
- 168995350-09
- 93541134780
- 123-456-789-09
- 987654.321-00
- 11122233396
- 529.982.247-25

CPFs inválidos:
- 12345678900 (dígitos verificadores incorretor)
- 11111111111 (todos os dígitos iguais)

- 12A 456 789 B0 (formato com letras/espaços não aceito)


### Validação do Código de Voo

O código do voo também passa por validação antes do cadastro.

O sistema aceita códigos no padrão:

- Uma letra inicial obrigatória (Incial da Companhia Áerea - Gol/Latam/Azul).
- Seguida por três números e um dígito verificador. (LXXX-D) (Letra da Companhia, 3 Dígitos Numéricos, 1 Dígito Verificador)
- A letra pode ser informada em maiúsculo ou minúsculo, sendo convertida automaticamente para maiúsculo.
- Caso o usuário informe o código no formato com separadores, como "L123-6" ou "L.1236", os caracteres extras são removidos antes da validação.

#### Cálculo do Dígito Verificador
1. Somar os três dígitos numéricos do Voo
2. Resto da Divisão da Soma dos dígitos por 10 (Soma%10)
3. O resultado encontrado será o Dígito Verificador 

#### Formato esperado:

LETRA ( A, G ou L ) + 3 NÚMEROS + 1 DÍGITO VERIFICADOR

#### Exemplos de códigos:

Códigos válidos:
- a123-6
- G6703
- L001.1
- g1247
- A7850
- L999-7

Códigos inválidos:
- 1234 (não possui letra inicial)
- GOL23 (possui mais de uma letra)
- a123 (quantidade de números incorreta)
- L1235 (dígito verificador incorreto)
- 1234g (letra na posição incorreta)
- k1236 (Companhia Inexistente)

O código do voo também é um campo de valor único, portanto não é permitido cadastrar dois voos diferentes utilizando o mesmo código.
