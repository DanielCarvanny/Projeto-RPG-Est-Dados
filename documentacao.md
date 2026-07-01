# Documentação do Projeto RPG - Investigação Policial

## Visão Geral

Este projeto é um jogo de investigação policial em C++ que utiliza estruturas de dados como **Tabela Hash** e **Grafo** para gerenciar pistas e suas conexões.

---

# Estruturas de Dados (Structs)

## Clue

* **Descrição:** Estrutura responsável por representar uma pista investigativa.

| Campo          | Tipo      | Descrição                                   |
| -------------- | --------- | ------------------------------------------- |
| idClue         | int       | Identificador único da pista                |
| title          | string    | Nome/título da pista                        |
| description    | string    | Descrição da evidência                      |
| question       | string    | Pergunta investigativa relacionada à pista  |
| options        | string[4] | Alternativas de resposta                    |
| correctReponse | int       | Índice da resposta correta                  |
| resolvida      | bool      | Indica se a pista foi resolvida             |
| descoberta     | bool      | Indica se a pista já foi descoberta         |
| realClue       | bool      | Define se a pista é uma evidência principal |

---

## No (TabelaHash)

* **Descrição:** Estrutura utilizada internamente pela tabela hash.

| Campo | Tipo   | Descrição                       |
| ----- | ------ | ------------------------------- |
| key   | string | Chave de identificação da pista |
| value | Clue   | Objeto da pista armazenada      |
| next  | No*    | Ponteiro para o próximo nó      |

---

## Suspects

* **Descrição:** Estrutura utilizada para representar suspeitos.

| Campo        | Tipo   | Descrição                       |
| ------------ | ------ | ------------------------------- |
| name         | string | Nome do suspeito                |
| relatedClues | No*[]  | Pistas relacionadas ao suspeito |
| guilty       | bool   | Indica se o suspeito é culpado  |

---

## No (Grafo)

* **Descrição:** Estrutura responsável pelos nós do grafo investigativo.

| Campo          | Tipo   | Descrição                  |
| -------------- | ------ | -------------------------- |
| idClue         | string | Identificador da pista     |
| nivel          | int    | Nível hierárquico da pista |
| connection     | No*[]  | Conexões com outras pistas |
| numConnections | int    | Quantidade de conexões     |

---

## NoArvore

* **Descrição:** Estrutura utilizada para os nós da árvore de decisão no dossiê de acusação.

| Campo      | Tipo       | Descrição                                 |
| ---------- | ---------- | ----------------------------------------- |
| question   | string     | Pergunta ou constatação da etapa atual    |
| suspect    | string     | Nome do suspeito acusado na folha (se houver)|
| esquerda   | NoArvore*  | Ponteiro para decisão afirmativa (Sim)    |
| direita    | NoArvore*  | Ponteiro para decisão negativa (Não)      |

---

# Classes

## TabelaHash

Classe responsável pelo armazenamento e gerenciamento das pistas.


---

### Métodos Privados

#### funcaoHash(string key)

* **Descrição:** Calcula o índice da tabela hash utilizando os valores ASCII da chave.
* **Parâmetros:** `key` — chave textual da pista.
* **Retorno:** inteiro representando a posição na tabela.

---

### Métodos Públicos

#### TabelaHash()

* **Descrição:** Inicializa todas as posições da tabela como nulas.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

---

#### input(string key, Clue value)

* **Descrição:** Insere ou atualiza uma pista na tabela hash.
* **Parâmetros:**

  * `key` — chave textual da pista.
  * `value` — objeto Clue.
* **Retorno:** Nenhum.

---

#### search(string key)

* **Descrição:** Busca uma pista na tabela hash.
* **Parâmetros:** `key` — chave da pista.
* **Retorno:** Objeto Clue.
  Caso não exista, retorna uma pista com `idClue = -1`.

---

#### searchId(int id)

* **Descrição:** Busca uma pista na tabela hash pelo ID;
* **Parâmetros:** id — ID da pista.
* **Retorno:** pista completa.
  Caso não exista, retorna uma pista vazia .

---

#### remover(string key)

* **Descrição:** Remove uma pista da tabela hash.
* **Parâmetros:** `key` — chave da pista.
* **Retorno:** Nenhum.

---

## Arvore

Classe responsável por gerenciar a árvore de decisão do dossiê final de acusação.

---

### Métodos Privados

#### temProva(int idBuscado)

* **Descrição:** Verifica se uma prova específica (pelo seu ID) foi adicionada ao dossiê.
* **Parâmetros:** `idBuscado` — inteiro representando o ID da pista.
* **Retorno:** Booleano indicando se a prova existe no dossiê.

---

### Métodos Públicos

#### Arvore()

* **Descrição:** Inicializa a árvore, definindo a raiz como nula e a contagem de evidências como 0.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

---

#### criarArvore(TabelaHash &table)

* **Descrição:** Constrói estaticamente a árvore de possibilidades lógicas da acusação com base nas pistas (coletadas ou não).
* **Parâmetros:** `&table` — Referência para a tabela hash de pistas.
* **Retorno:** Nenhum.

---

#### montarDossie(Clue inventory[], int totalClues)

* **Descrição:** Interface interativa para o jogador selecionar entre 8 e 10 provas do inventário para o dossiê final.
* **Parâmetros:**
  * `inventory[]`: O array que armazena as pistas do inventário.
  * `totalClues`: O número total de pistas no inventário.
* **Retorno:** Nenhum.

---

#### iniciarAcusacao(Clue inventory[], int totalClues)

* **Descrição:** Inicia o fluxo de acusação baseado nas perguntas da árvore, direcionando o jogador para o final correto ou falha na dedução.
* **Parâmetros:**
  * `inventory[]`: O array que armazena as pistas do inventário.
  * `totalClues`: O número total de pistas no inventário.
* **Retorno:** Nenhum.

---

## Grafo

Classe responsável pela representação lógica das conexões entre pistas.

O grafo é utilizado para exibir a linha de raciocínio investigativa de Sherlock Holmes.

---

### Métodos Públicos

#### Grafo()

* **Descrição:** Inicializa o grafo e define os vértices como nulos.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

---

#### addNo(string idClue, int nivel)

* **Descrição:** Cria e adiciona um novo nó ao grafo.
* **Parâmetros:**

  * `idClue` — identificador da pista.
  * `nivel` — nível investigativo da pista.
* **Retorno:** Ponteiro para o nó criado.

---

#### addConnections(int v1, int v2)

* **Descrição:** Adiciona uma conexão (aresta) entre dois vértices do grafo.
* **Parâmetros:**
  * `v1` — índice do vértice de origem.
  * `v2` — índice do vértice de destino.
* **Retorno:** Nenhum.

---

#### dfsRaciocinio(No* atual, bool visitados[], TabelaHash &table)

* **Descrição:** Busca em profundidade (recursiva) para a criação de uma linha de racioncínio baseada nas pistas.
* **Parâmetros:**

  * `No* atual`: cria um nó.
  * `visitados[]`: array que guarda nó(s) visitados.
  * `&table`: Referência para a tabela hash de pistas.
* **Retorno:** Nenhum.

---

#### showConnections(int v)

* **Descrição:** Exibe todas as conexões de uma pista.
* **Parâmetros:** `v` — índice do vértice.
* **Retorno:** Nenhum.

---

#### showReasoningLine(string clueKey, TabelaHash &table)

* **Descrição:** Exibe visualmente a linha de raciocínio das pistas conectadas.
* **Parâmetros:** 
  * `clueKey` — identificador da pista.
  * `&table`: Referência para a tabela hash de pistas.
* **Retorno:** Nenhum.

---

#### printDFS(int nodeIdx, TabelaHash &table, bool visited[], string prefix, bool isLast)

* **Descrição:** Imprime a árvore de conexões em profundidade no terminal, auxiliando a visualização gráfica do palácio mental.
* **Parâmetros:**
  * `nodeIdx`: índice atual do vértice.
  * `&table`: Referência para a tabela hash.
  * `visited[]`: Array de nós visitados.
  * `prefix`: String de prefixo para formatação de níveis (hierarquia visual).
  * `isLast`: Booleano indicando se é o último nó de uma ramificação.
* **Retorno:** Nenhum.

---

#### showMentalWeb(TabelaHash &table)

* **Descrição:** Exibe o palácio mental completo, estruturando e formatando todas as conexões do grafo como árvores dependentes e nós circulares/isolados.
* **Parâmetros:** `&table` — Referência para a tabela hash de pistas.
* **Retorno:** Nenhum.

---

# Sistemas do Projeto

### Sistema de Investigação
* **Descrição:** Gerencia a lógica central de investigação, desde a descoberta de uma pista até a dedução de Holmes.
* **Funções Associadas:** `investigateClue()`.

---

### Sistema de Inventário
* **Descrição:** Controla as pistas que o jogador coleta, permitindo que sejam adicionadas e visualizadas.
* **Funções Associadas:** `addInventory()`, `showInventory()`.

#### addInventory(Clue inventory[], int &totalClues, Clue clue)
* **Descrição:** Adiciona uma pista recém-descoberta ao inventário do jogador.
* **Parâmetros:**
  * `inventory[]`: O array que armazena as pistas.
  * `&totalClues`: Referência para o contador de pistas no inventário.
  * `clue`: O objeto `Clue` a ser adicionado.
* **Retorno:** Nenhum.

#### showInventory(Clue inventory[], int totalClues)
* **Descrição:** Exibe todas as pistas que o jogador já coletou.
* **Parâmetros:**
  * `inventory[]`: O array que armazena as pistas.
  * `totalClues`: O número total de pistas no inventário.
* **Retorno:** Nenhum.

---

### Sistema de Perguntas
* **Descrição:** Apresenta ao jogador um desafio de múltipla escolha para cada pista, validando a resposta para determinar se a pista foi "resolvida".
* **Funções Associadas:** `checkAnswer()`.

#### checkAnswer(Clue &pista)
* **Descrição:** Apresenta uma pergunta de múltipla escolha ao jogador e verifica se a resposta está correta.
* **Parâmetros:** `&pista` — Referência para a pista que está sendo investigada.
* **Retorno:** `bool` — Retorna `true` se a resposta for correta, `false` caso contrário.

---

### Sistema de Dedução
* **Descrição:** Quando uma pista principal é resolvida, este sistema exibe a linha de raciocínio de Holmes, mostrando a conexão visual no grafo e uma dedução textual.
* **Funções Associadas:** `showDeduction()`.

#### investigateClue(TabelaHash &table, Grafo &graph, string key, Clue inventory[], int &totalClues, int &realCluesSolved)
* **Descrição:** Orquestra a investigação de uma pista. Busca a pista na tabela hash, adiciona ao inventário, chama a verificação de resposta e, se for o caso, aciona o sistema de dedução.
* **Parâmetros:**
  * `&table`: Referência para a tabela hash de pistas.
  * `&graph`: Referência para o grafo de investigação.
  * `key`: Chave da pista a ser investigada.
  * `inventory[]`: Array de pistas do inventário.
  * `&totalClues`: Referência para o contador total de pistas.
  * `&realCluesSolved`: Referência para o contador de pistas principais resolvidas.
* **Retorno:** Nenhum.

#### showDeduction(Grafo &graph, TabelaHash &table, string clueKey)
* **Descrição:** Exibe a linha de raciocínio de Holmes, mostrando a conexão visual no grafo e uma dedução textual associada à pista resolvida.
* **Parâmetros:**
  * `&graph`: Referência para o grafo de investigação.
  * `clueKey`: A chave da pista que acionou a dedução.
* **Retorno:** Nenhum.

---

# História e Narrativa

O jogo contém:

* introdução cinematográfica
* cena do crime
* interrogatórios individuais
* sistema de acusação final

---

## Personagens Interrogados

| Personagem       | Papel            |
| ---------------- | ---------------- |
| Eleanor Whitmore | Esposa           |
| Arthur Whitmore  | Filho mais velho |
| Edward Whitmore  | Filho do meio    |
| Violet Whitmore  | Filha caçula     |
| Alfred           | Mordomo          |

---

# Sistema de Acusação

O jogador só pode realizar uma acusação após resolver pelo menos:

* **3 pistas principais**

Caso a acusação correta seja feita:

* o final verdadeiro é exibido.

---

# Pistas Implementadas

## Pistas Principais

| Código | Título                 |
| ------ | ---------------------- |
| P1     | Faca                   |
| P2     | Posição da Faca        |
| P3     | Cabo com microfissuras |

---

## Pistas Secundárias/Falsas

| Código | Título                                |
| ------ | ------------------------------------- |
| P4     | Lenço da esposa                       |
| P5     | Cigarros do Mordomo                   |
| P4     | Lenço ensanguentado e com ferrugem    |
| P5     | Cigarros Turcos                       |
| P6     | Discussão recente do filho mais velho |
| P7     | Remédio antidepressivo                |
| P8     | Traição                               |

---

## Pistas de Ambiente

| Código | Título                |
| ------ | --------------------- |
| P9     | Cadeira Caída         |
| P10    | Taça de Vinho Quebrada |
| P11    | Sangue na Pia         |

---

# Fluxo do Jogo

```text
Início
  │
  ├─► Introdução da história
  │
  ├─► Cena do crime
  │
  ├─► Interrogatórios
  │
  ├─► Exploração da cozinha
  │       │
  │       ├─► Investigar pistas
  │       ├─► Responder perguntas
  │       ├─► Adicionar ao inventário
  │       └─► Resolver pistas principais
  │
  ├─► Sistema de dedução
  │
  └─► Acusação final
          │
          ├─► Acerto → Final verdadeiro
          │
          └─► Erro → Acusação inválida
```

---

# Constantes

| Constante    | Valor | Descrição                              |
| ------------ | ----- | -------------------------------------- |
| MAX_VERTICES | 5     | Quantidade máxima de vértices do grafo |
| MAX          | 10    | Tamanho da tabela hash                 |

---

# Funções Auxiliares

## limparTela()

* **Descrição:** Limpa a tela do console utilizando comandos nativos dependentes do sistema operacional (Windows/Unix).

---

## pausa()

* **Descrição:** Cria pausas para leitura do jogador entre diálogos e cenas, exigindo que a tecla Enter seja pressionada.

---

# Funções Narrativas

## IntroStory()

* **Descrição:** Responsável pela introdução narrativa do jogo.

---

## crimeScene() 

* **Descrição:** Exibe a cena inicial do crime.

---

## Funções de Interação e Menus

#### menuManager(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved)
* **Descrição:** Controla o menu principal do jogo, permitindo ao jogador escolher entre interrogar suspeitos ou investigar a cena do crime.
* **Parâmetros:**
  * `&table`: Referência para a tabela hash de pistas.
  * `&graph`: Referência para o grafo de investigação.
  * `inventory[]`: Array de pistas do inventário.
  * `&totalClues`: Referência para o contador total de pistas.
  * `&realCluesSolved`: Referência para o contador de pistas principais resolvidas.
* **Retorno:** Nenhum.

#### kitchenMenu(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved)
* **Descrição:** Apresenta as opções de investigação na cozinha, permitindo ao jogador examinar diferentes objetos e pistas.
* **Parâmetros:**
  * `&table`: Referência para a tabela hash de pistas.
  * `&graph`: Referência para o grafo de investigação.
  * `inventory[]`: Array de pistas do inventário.
  * `&totalClues`: Referência para o contador total de pistas.
  * `&realCluesSolved`: Referência para o contador de pistas principais resolvidas.
* **Retorno:** Nenhum.

#### interrogationScene()
* **Descrição:** Apresenta o menu para selecionar qual suspeito interrogar.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

#### Funções de Interrogatório
* `eleanorInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved)`
* `arthurInterrogation(...)`, `edwardInterrogation(...)`, `violetInterrogation(...)`, `alfredInterrogation(...)`
* **Descrição:** Cada uma dessas funções é responsável por gerenciar o diálogo e as perguntas para um suspeito específico. Todas recebem as estruturas principais do jogo por referência, além do inventário.
* **Retorno:** Nenhum.

---

## Funções de Configuração

#### createClues(TabelaHash &table)
* **Descrição:** Inicializa todas as pistas do jogo (principais, falsas e de ambiente) e as insere na tabela hash.
* **Parâmetros:** `&table` — Referência para a tabela hash a ser populada.
* **Retorno:** Nenhum.

#### createConnectionsClue(Grafo &graph)
* **Descrição:** Cria os nós no grafo de investigação e estabelece as conexões lógicas entre as pistas.
* **Parâmetros:** `&graph` — Referência para o grafo a ser configurado.
* **Retorno:** Nenhum.

---

# Final do Jogo

#### iniciarAcusacao(Clue inventory[], int totalClues)
* *(Nota: A integração com o sistema principal ocorre através das chamadas feitas na classe `Arvore`)*
* **Descrição:** Inicia a sequência de dedução utilizando as provas selecionadas no dossiê. É o ponto culminante do sistema de dedução final.

#### wrongEnding(string suspect, Clue inventory[], int &totalClues)
* **Descrição:** Exibe uma narrativa de acusação falha para um suspeito incorreto e permite voltar ao jogo.
* **Parâmetros:** `suspect` (nome do suspeito acusado), array do inventário, total de pistas.
* **Retorno:** Nenhum.

#### bestEnding(Clue inventory[], int &totalClues)
* **Descrição:** Exibe a narrativa do final verdadeiro, onde Sherlock revela o assassino (Violet) e suas motivações precisas baseando-se nas provas.
* **Parâmetros:** Array do inventário de pistas, referência para a contagem.
* **Retorno:** Nenhum.

---

# Observações

* O projeto utiliza programação estruturada e orientação a objetos.
* O sistema de pistas utiliza tabela hash com encadeamento.
* O grafo representa relações investigativas entre evidências.
* O inventário armazena todas as pistas encontradas.
* Existem pistas reais e pistas falsas para aumentar a complexidade investigativa.
* O jogo possui narrativa interativa baseada em escolhas.
* A investigação é linear, porém permite exploração livre das pistas no menu principal.
