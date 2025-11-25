# aed2-aerodromo-indexing

Este projeto foi realizado na disciplina de Algoritmo e Estrutura de Dados II, utilizando ANSI C (C89/C90). Ele implementa estruturas de dados para visualização de informações aeroportuárias, incluindo parsing de CSV, conversão de coordenadas geográficas (DMS ↔ Decimal) e indexação eficiente para consultas rápidas.

## Implementa as estruturas de dados
- Árvore Binária de Busca
- Árvore B+

## Requisitos

- **GCC**
- **Make**

## Estrutura

```
aed2-aerodromo-indexing/
├── data/                          # Arquivos de dados
│   └── G12.csv                    
├── include/                       # Headers (.h)
│   ├── aerodromo.h               # Estruturas de aeródromo
│   ├── arvore_bb.h               # Árvore binária de busca
│   ├── arvore_b_mais.h           # Árvore B+
│   ├── auxiliar.h                # Funções auxiliares
│   ├── gerenciador_csv.h         # Parser de CSV
│   ├── interface.h               # Interface com usuário
│   └── relatorio.h               # Geração de relatórios
├── src/                          # Código-fonte (.c)
│   ├── aerodromo.c
│   ├── arvore_bb.c
│   ├── arvore_b_mais.c
│   ├── auxiliar.c
│   ├── gerenciador_csv.c
│   ├── interface.c
│   ├── main.c
│   └── relatorio.c
├── bin/                          # Executáveis
│   └── aerodromo_indexing
├── obj/                          # Arquivos objeto
│   ├── *.o                       # Arquivos compilados
│   └── *.d                       # Arquivos de dependências
├── Makefile                      # Build
├── README.md                     
└── .gitignore                    # Arquivos ignorados pelo Git
```

## Compilação e Execução

### **Usando Make (Recomendado)**

O projeto inclui um Makefile completo para facilitar a compilação.

#### **Comandos Disponíveis:**

```bash
# Compilar o projeto
make

# Compilar e executar
make run

# Limpar arquivos compilados
make clean

# Recompilar do zero
make rebuild
```

#### **Exemplo:**

```bash
make clean

make

make run
```

### **Usando GCC Diretamente**

Se você não tiver Make instalado ou preferir compilar manualmente:

#### **Compilação Básica:**

```bash
mkdir -p bin && \
gcc -ansi -Wall -Wextra -Wpedantic -I./include -o bin/aerodromo_indexing \
    src/aerodromo.c \
    src/arvore_bb.c \
    src/arvore_b_mais.c \
    src/auxiliar.c \
    src/gerenciador_csv.c \
    src/interface.c \
    src/main.c \
    src/relatorio.c \
    -lm
```
#### **Executar:**

```bash
./bin/aerodromo_indexing
```

#### **Flags usadas:**

- **`-ansi`**: Garante conformidade com ANSI C (C89/C90)
- **`-Wall -Wextra -Wpedantic`**: Habilita todos os warnings
- **`-I./include`**: Especifica o diretório de headers
- **`-lm`**: Linka a biblioteca matemática (necessário para `floor`, `fabs`, etc.)

## Formato dos Dados

### **Arquivo CSV:**

O sistema processa arquivos CSV com o seguinte formato:

```csv
Código OACI;Nome;Município;UF;Longitude;Latitude;Altitude;Operação Diurna;Comprimento 1;Largura 1
SBGR;Guarulhos;Guarulhos;SP;"46°28'33.37""W";"23°26'14.00""S";749;IFR;3000;45
```

### **Campos:**

A chave utilizada para os dados nas estruturas foi uma chave composta dos campos Latitude e Longitude.

| Campo | Tipo | Descrição | Exemplo |
|-------|------|-----------|---------|
| Código OACI | string | Identificador | `SJ3D` |
| Nome | string| Nome do aeródromo | `FAZENDA SINIMBU` |
| Município | string | Cidade | `JABORANDI` |
| UF | string | Estado | `BA` |
| Longitude | double | Coordenada Oeste/Leste | `"045°09'51""W"` |
| Latitude | double | Coordenada Sul/Norte | `"13°51'59""S"` |
| Altitude | int | Altitude em metros | `769` |
| Operação | enum | VFR, IFR ou VFR/IFR | `VFR` |
| Comprimento | int | Comprimento pista (m) | `1500` |
| Largura | int | Largura pista (m) | `22` |
