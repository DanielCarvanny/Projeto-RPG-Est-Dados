# Documentação do Projeto RPG - Investigação Policial

## Visão Geral

Este projeto é um jogo de investigação policial em C++ que utiliza estruturas de dados como **Tabela Hash** e **Grafo** para gerenciar pistas e suas conexões.

---

## Estruturas de Dados (Structs)

### Clue
Estrutura que representa uma pista no jogo.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| idClue | int | Identificador único da pista |
| description | string | Descrição da pista encontrada |
| question | string | Pergunta feita ao jogador |
| correctReponse | string | Resposta correta para resolver a pista |
| resolvida | bool | Indica se a pista foi resolvida |
| descoberta | bool | Indica se a pista foi descoberta |

### No (TabelaHash)
Estrutura de entrada da tabela hash.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| key | string | Chave para hash |
| value | Clue | Valor armazenado (pista) |
| next | No* | Ponteiro para próximo nó |

### Suspects
Estrutura para suspeitos relacionados às pistas.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| name | string | Nome do suspeito |
| relatedClues | No*[] | Pistas relacionadas ao suspeito |
| guilty | bool | Indica se o suspeito é culpado |

### No (Grafo)
Estrutura de nó para o grafo de pistas.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| idClue | string | Identificador da pista |
| nivel | int | Nível da pista |
| connection | No*[] | Conexões com outras pistas |
| numConnections | int | Número de conexões |

---

## Classes

### TabelaHash
Classe que implementa uma tabela hash para armazenar e gerenciar pistas.

#### Métodos Privados

##### funcaoHash(string key)
- **Descrição**: Calcula o hash de uma chave usando o algoritmo polynomial rolling hash
- **Parâmetros**: `key` - Chave em string
- **Retorno**: Inteiro representando a posição na tabela hash

#### Métodos Públicos

##### TabelaHash()
- **Descrição**: Construtor que inicializa a tabela hash com todas as posições nulas
- **Parâmetros**: Nenhum
- **Retorno**: Nenhum

##### input(string key, Clue value)
- **Descrição**: Insere ou atualiza uma pista na tabela hash
- **Parâmetros**: 
  - `key` - Chave da pista
  - `value` - Objeto Clue com os dados da pista
- **Retorno**: Nenhum

##### search(string key)
- **Descrição**: Busca uma pista na tabela hash pela chave
- **Parâmetros**: `key` - Chave da pista a buscar
- **Retorno**: Objeto Clue (retorna pista com idClue = -1 se não encontrada)

##### remover(string key)
- **Descrição**: Remove uma pista da tabela hash
- **Parâmetros**: `key` - Chave da pista a remover
- **Retorno**: Nenhum

---

### Grafo
Classe que implementa um grafo para representar as conexões entre pistas.

#### Métodos Públicos

##### Grafo()
- **Descrição**: Construtor que inicializa o grafo com vértices nulos
- **Parâmetros**: Nenhum
- **Retorno**: Nenhum

##### addNo(string idClue, int nivel)
- **Descrição**: Cria e adiciona um novo nó (pista) ao grafo
- **Parâmetros**: 
  - `idClue` - Identificador da pista
  - `nivel` - Nível da pista
- **Retorno**: Ponteiro para o nó criado

##### addConnections(int v1, int v2)
- **Descrição**: Adiciona uma conexão entre dois vértices do grafo
- **Parâmetros**: 
  - `v1` - Índice do primeiro vértice
  - `v2` - Índice do segundo vértice
- **Retorno**: Nenhum

##### showConnections(int v)
- **Descrição**: Exibe todas as conexões de uma pista específica
- **Parâmetros**: `v` - Índice do vértice
- **Retorno**: Nenhum

---

## Funções Globais

### checkAnswer(Clue pista)
- **Descrição**: Verifica se a resposta do jogador está correta para resolver uma pista
- **Parâmetros**: `pista` - Objeto Clue contendo a pergunta e resposta correta
- **Retorno**: Bool - true se a resposta estiver correta, false caso contrário

---

## Função main()

A função main() é o ponto de entrada do jogo e realiza as seguintes operações:

1. **Inicialização**
   - Cria uma instância da classe TabelaHash
   - Define as constantes do jogo (MAX_VERTICES = 5, MAX = 10)

2. **Criação de Pistas**
   - Cria um objeto Clue com dados de exemplo (idClue, description, question, correctReponse)

3. **Interface do Jogo**
   - Exibe título "INVESTIGACAO POLICIAL"
   - Mostra a descrição da pista encontrada

4. **Loop de Tentativas**
   - Permite até 3 tentativas para o jogador responder corretamente
   - Chama a função checkAnswer() para verificar a resposta
   - Exibe mensagens de sucesso ou falha conforme o resultado

---

## Fluxo do Jogo

```
Início
  │
  ├─► Criar TabelaHash
  │
  ├─► Criar Pista (Clue)
  │
  ├─► Exibir interface
  │
  └─► Loop (até 3 tentativas)
        │
        ├─► Receber resposta
        │
        ├─► Verificar com checkAnswer()
        │
        └─► Acertou? ──► Vitória
            │
            └─► Errou? ──► Continuar loop
```

---

## Constantes

| Constante | Valor | Descrição |
|-----------|-------|-----------|
| MAX_VERTICES | 5 | Número máximo de vértices no grafo |
| MAX | 10 | Tamanho da tabela hash |

---

## Observações

- O projeto utiliza tabela hash com tratamento de colisões por encadeamento (separate chaining)
- O grafo utiliza representação por adjacência com vetor de ponteiros
- O jogo currently tem implementação básica com uma única pista de exemplo
- A estrutura suporta múltiplas pistas e suas conexões