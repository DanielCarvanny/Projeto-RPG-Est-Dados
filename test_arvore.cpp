#define UNIT_TEST // Desativa o main() original e injeta os metodos de teste na Arvore
#include "main.cpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// Funcao auxiliar para simular as escolhas do jogador e injetar no Dossie
void mockPlayerChoice(TabelaHash &table, string key, int chosen, Clue dossier[], int &count) {
    Clue c = table.search(key);
    c.optionChosen = chosen;
    table.input(key, c); 
    
    dossier[count] = c;
    count++;
}

void test_missing_knife_fails_early() {
    TabelaHash table;
    createClues(table); 
    
    Clue dossier[10];
    int count = 0;
    
    // Jogador leva passos e lavanda, mas ESQUECE a faca.
    mockPlayerChoice(table, "passos_leves", 2, dossier, count);
    mockPlayerChoice(table, "cheiro_lavanda", 1, dossier, count);
    
    Arvore tree;
    tree.injectDossier(dossier, count);
    tree.criarArvore(table); 
    
    vector<int> inputs = {1};
    string result = tree.simulateTraversal(inputs);
    assert(result == "Inconclusivo");
    cout << "  [PASS] test_missing_knife_fails_early" << endl;
}

void test_violet_true_ending() {
    TabelaHash table;
    createClues(table);
    
    Clue dossier[10];
    int count = 0;
    
    // Configurando as 5 Provas Fisicas no Dossie
    mockPlayerChoice(table, "faca_crime", 1, dossier, count);
    mockPlayerChoice(table, "cheiro_lavanda", 1, dossier, count);
    mockPlayerChoice(table, "passos_leves", 2, dossier, count);
    mockPlayerChoice(table, "fones_ouvido", 1, dossier, count);
    mockPlayerChoice(table, "unhas_violet", 2, dossier, count);
    
    // Configurando a Psicologia apenas na Hash (Sem gastar slot de dossie)
    Clue micro = table.search("microfissuras_cabo"); micro.optionChosen = 2; table.input("microfissuras_cabo", micro);
    Clue f = table.search("frieza_violet"); f.optionChosen = 2; table.input("frieza_violet", f);
    Clue pm = table.search("perdeu_medo"); pm.optionChosen = 2; table.input("perdeu_medo", pm);
    Clue ad = table.search("adrenalina_violet"); ad.optionChosen = 1; table.input("adrenalina_violet", ad);
    Clue t = table.search("tirano_domestico"); t.optionChosen = 1; table.input("tirano_domestico", t);
    Clue k = table.search("karma_violet"); k.optionChosen = 2; table.input("karma_violet", k);
    Clue av = table.search("alibi_violet"); av.optionChosen = 1; table.input("alibi_violet", av);
    Clue angulo = table.search("angulo_facada"); angulo.optionChosen = 2; table.input("angulo_facada", angulo);
    
    Arvore tree;
    tree.injectDossier(dossier, count);
    tree.criarArvore(table);
    
    // O caminho do sucesso: 
    // Q1 (Passional?) -> 1 (Sim)
    // Q2 (Mulheres/Ciume?) -> 2 (Nao, foi odio familiar)
    // Q3 (Violet forjou?) -> 1 (Sim)
    // Q4 (Prova sangue?) -> 1 (Sim)
    vector<int> inputs = {1, 2, 1, 1}; 
    string result = tree.simulateTraversal(inputs);
    
    assert(result == "Violet");
    cout << "  [PASS] test_violet_true_ending" << endl;
}

void test_arthur_ending() {
    TabelaHash table;
    createClues(table);
    
    Clue dossier[10];
    int count = 0;
    
    // Profiling Errado (Profissional)
    mockPlayerChoice(table, "faca_crime", 0, dossier, count);
    mockPlayerChoice(table, "discussao_arthur", 1, dossier, count);
    mockPlayerChoice(table, "tensao_arthur", 1, dossier, count);
    mockPlayerChoice(table, "alibi_arthur", 1, dossier, count);
    
    // Falso positivo para o angulo para entrar na condicao do Arthur
    Clue angulo = table.search("angulo_facada"); angulo.optionChosen = 2; table.input("angulo_facada", angulo);
    
    Arvore tree;
    tree.injectDossier(dossier, count);
    tree.criarArvore(table);
    
    // Caminho Arthur:
    // Q1 (Motivacao heranca?) -> 1 (Sim)
    // Q2 (Arthur mandou matar?) -> 1 (Sim)
    vector<int> inputs = {1, 1}; 
    string result = tree.simulateTraversal(inputs);
    
    assert(result == "Arthur");
    cout << "  [PASS] test_arthur_ending" << endl;
}

int main() {
    cout << "\n[TESTES] Rodando Suite de Testes da Arvore..." << endl;
    
    test_missing_knife_fails_early();
    test_violet_true_ending();
    test_arthur_ending();
    
    cout << "[TESTES] Todos os testes passaram com sucesso!\n" << endl;
    return 0;
}
