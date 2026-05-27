#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>    // SetConsoleOutputCP
#endif

using namespace std;

const int MAX_VERTICES = 5;
int op;

// ---------------[Utilidades]---------------
void limparTela(){
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}


//---------------[Struct]---------------
struct Clue { //Estrutura da Pista
  int idClue;
  
  string title;
  string description;
  string question;
  string options[4];
  int correctReponse;
  
  bool resolvida = false;
  bool descoberta = false;
  bool realClue = false;
};

//---------------[Auxiliar]---------------
void next(){ //Função pra pausa

    string continuar;
    getline(cin, continuar);
}

//---------------[Classes]---------------
class TabelaHash{
  private:
    static const int MAX = 10;

    struct No{ //Estrutura de entrada
      string key;
      Clue value;
      No* next;
    };

    No* tableHash[MAX];
    struct Suspects{
      string name;
      No* relatedClues[MAX];
      bool guilty;
    };

    int funcaoHash(string key) { //Pega o valor em ASCII e cria uma chave a partir do número feito, ou seja, pega o ascii de "P1"
      int sum = 0;

      for (int i = 0; i < key.size(); i++) {
        sum = (sum * 31) + key[i];
      }

      return sum % MAX;
    }

  public:
    TabelaHash() {
      for (int i = 0; i < MAX; i++) {
        tableHash[i] = NULL;
      }
    }

    void input(string key, Clue value) {
      int position = funcaoHash(key);

      No* temp = tableHash[position];
      while(temp != NULL){
        if(temp -> key == key){
          temp -> value = value;
          return;
        }
        temp = temp -> next;
      }

      No* element = new No;
      element -> key = key;
      element -> value = value;
      element -> next = tableHash[position];
      tableHash[position] = element;
    }

    Clue search(string key) {
      int position = funcaoHash(key);
      No* temp = tableHash[position];
      while (temp != NULL) {
        if(temp -> key == key){
          return temp -> value;
        }
        temp = temp -> next;
      }
      Clue null;
      null.idClue = -1;
      return null;
    }

    void remover(string key) {
      int position = funcaoHash(key);
      No* temp = tableHash[position];
      No* ant = NULL;
      while (temp != NULL) {
        if (temp -> key == key) {
          if (ant == NULL) {
            tableHash[position] = temp -> next;
          } else {
            ant->next = temp->next;
          }

          delete temp;
          return;
        }
        ant = temp;
        temp = temp->next;
      }
    }
};
class Grafo{
  public:
      Grafo(){
          numVertices = 0;                          
          for(int i = 0; i < MAX_VERTICES; i++){
              vertices[i] = NULL;
          }
      }
  private:           
    struct No{
      string idClue;
      int nivel;
      No* connection[MAX_VERTICES];
      int numConnections = 0;
    };

    No* vertices[MAX_VERTICES];
    int numVertices;

  public:
    No* addNo(string idClue, int nivel){ //Criação do nó(Níveis)

      No* newNo = new No;
      newNo->idClue = idClue;
      newNo->nivel = nivel;
      newNo->numConnections = 0; 

      for(int i = 0; i < MAX_VERTICES; i++){ //Colocando conexões nulas
        newNo->connection[i] = NULL;
      }

      if(numVertices < MAX_VERTICES){ //Adicionando Nó criado no vetor de nível
        vertices[numVertices] = newNo; 
        numVertices++;
      }

      return newNo; //Retornando Nó criado
    }

    void addConnections(int v1, int v2) { //Adicionando conexões
      
      if(v1 < MAX_VERTICES && v2 < MAX_VERTICES){
        No* original = vertices[v1];
        
        if(original->numConnections < MAX_VERTICES){
          original->connection[original->numConnections] = vertices[v2];
          original->numConnections++;
        }
      }
    }
    
    void showConnections(int v){ //Exibindo coneções 
        No* present = vertices[v];
        
        cout << "Ligações entre as pistas:"<< present->idClue << endl;
        
        for(int i = 0; i < present-> numConnections; i++){
            cout<< present->connection[i]->idClue << endl;
        }
    }
    
    void showReasoningLine(string clueKey){ //Exibindo linha de raciocínio visualmente com grafo
      for(int i = 0; i < numVertices; i++){
          if(vertices[i]->idClue == clueKey){
              cout << "\nLinha de raciocinio:\n\n";

              cout << vertices[i]->idClue;

              No* current = vertices[i];

              while(current->numConnections > 0){
                  current = current->connection[0];
                  cout << " -> " << current->idClue;
              }

              cout << endl;
              return;
          }
      }
    }

};
void showDeduction(Grafo &graph, string clueKey);

//---------------[Inventário]---------------
void addInventory(Clue inventory[],int &totalClues,Clue clue){
  inventory[totalClues] = clue;
  totalClues++;

  cout << "\n[Pista adicionada ao inventario]\n";
}
void showInventory(Clue inventory[],int totalClues){
  cout << "\n===== INVENTARIO DE PISTAS =====\n";

  if(totalClues == 0){
    cout << "Nenhuma pista encontrada.\n";
    return;
  }

  for(int i = 0; i < totalClues; i++){

    cout << "\n[" << inventory[i].title << "]\n";

    cout << inventory[i].description << endl;
  }

  cout << endl;
}


//---------------[Sistemas]---------------
bool checkAnswer(Clue &pista){ //Verifica Pista
  limparTela();

  int response;
  
  cout << "\n" << pista.question << endl;

  for(int i = 0; i < 4; i++){
    cout << i + 1 << " - " << pista.options[i] << endl;
  }

  cin >> response;
  cin.ignore();

  if(response == pista.correctReponse){
    cout << "\nHolmes observa a cena em silencio.\n";
    cout << "\"Interessante... muito interessante.\"\n";

    pista.resolvida = true;

    return true;
  }

  return false;
}
void investigateClue(TabelaHash &table, Grafo &graph,string key, Clue inventory[], int &totalClues, int &realCluesSolved){
  limparTela();
  Clue clue = table.search(key);

  if(clue.idClue == -1){
    return;
  }

  if(clue.descoberta){
    cout << "\nEssa pista ja foi investigada.\n";
    return;
  }
  clue.descoberta = true;

  cout << "\n------[" << clue.title << "]------\n";
  cout << clue.description << endl;

  addInventory(inventory, totalClues, clue);

  if(clue.correctReponse != -1){
    bool correct = checkAnswer(clue);
    if(correct && clue.realClue){
      realCluesSolved++;
      showDeduction(graph, key);
    }
  }

  cout << "\n[Holmes liga essa pista ao assassinato]\n";

  //cout << "\nProgresso da investigacao: " << realCluesSolved << "/3 pistas principais.\n";
  table.input(key, clue);
}
// AVALIAR: URGENTE
void showDeduction(Grafo &graph, string clueKey){
    
    cout << "\n[Holmes analisa as conexoes mentais]\n";
    graph.showReasoningLine(clueKey);


    if(clueKey == "P1"){
        cout << "A faca foi parcialmente limpa.\n";
        cout << "Isso sugere tentativa de esconder o assassinato.\n";
    }

    else if(clueKey == "P2"){
        cout << "O golpe veio de baixo para cima.\n";
        cout << "O assassino provavelmente era menor.\n";
    }

    else if(clueKey == "P3"){
        cout << "A violencia foi emocional.\n";
        cout << "Nao foi um assassinato frio.\n";
    }
}

//---------------[História]---------------
void introStory(){
  cout << "===== INVESTIGACAO POLICIAL =====" << endl;
  next();

  cout << "Londres -- 15 de Janeiro de 2010, 22:20" << endl; //Nenhuma alteração necessária
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A chuva fina castigava as ruas de Londres, a nevoa envolvia os postes de luz, transformando o brilho amarelado em manchas fantasmagoricas no asfalto molhado." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Dentro de um táxi escuro, Sherlock Holmes observava silenciosamente o lado de fora enquanto John Watson terminava de vestir suas luvas." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "O veiculo desacelera." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A frente, uma enorme residencia vitoriana cercada por grades negras emerge da neblina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Luzes policiais iluminam a fachada.\nDuas viaturas.\nUma ambulancia."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Assim que descem do carro, um policial os aguarda proximo a entrada principal."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Ele estende a mão."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Policial ==="<<endl;
  cout<<"-- Boa noite, Detetive Holmes e Doutor Watson, sou o inspetor William Carter."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- O patriarca da familia foi brutalmente assassinado ha cerca de uma hora."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Nao encontramos sinais de arrombamento. Nenhuma testemunha. Nenhuma pista concreta."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Mas quem fez isso... sabia exatamente o que estava fazendo."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Os familiares estao reunidos na sala de estar. Ninguem saiu da casa desde nossa chegada. O corpo esta na cozinha."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  limparTela();

}
void crimeScene(){
  cout << endl << "=== CENA DO CRIME -- COZINHA ===" << endl;

  cout << "Ao entrarem, o cheiro metalico do sangue domina imediatamente o ambiente." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A cozinha luxuosa parece congelada no tempo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "No centro da cozinha..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "o corpo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Um homem de meia idade, elegante, caido ao lado da ilha central." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A camisa social branca esta completamente tingida de vermelho." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Uma faca de cozinha permanece cravada em seu peito." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Ao redor do corpo:" << endl;
  cout << "sangue." << endl;
  cout << "Pegadas borradas." << endl;
  cout << "Marcas de luta." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Watson se aproxima lentamente." << endl;
  
  cout<<"=== Watson ==="<<endl;
  cout << "-- Meu Deus..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Sherlock permanece em silencio. Observando. Calculando." << endl;
  cout << "Para prosseguir apere Enter" << endl;
  next();
  
  cout<<"=== Sherlock ==="<<endl;
  cout << "-- Vamos conversar um pouco com nossos suspeitos..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  limparTela();
  
}

//---------------[Interrogatórios]---------------
void eleanorInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue P12 = table.search("P12");
  Clue P4 = table.search("P4");
  Clue P7 = table.search("P7");
  op = -1;
  do{
    cout << endl << "=== A ESPOSA -- ELEANOR WHITMORE ===" << endl;
    cout<<"=== Características ==="<<endl;
    cout << "Elegante." << endl;
    cout << "Olhos cansados." << endl;
    cout << "Maos tremulas." << endl;
    
    if(P7.descoberta){
      cout<< "Ela apresenta uma agitacao extrema nos pes; Espasmos musculares e uma forte sonolência incapacitante. Isso pode indicar uso de remédios antidepressivos." <<endl;
    }
    
    cout << "\nPara prosseguir aperte Enter..." << endl;
    next();

    // Perguntas
    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu marido?" << endl; //"relação" não precisa de ajuste aqui, mas vou ajustar para consistência
    cout <<" 2. Onde estava durante o assassinato?" << endl;
    if(P12.descoberta){
      cout <<" 3. A senhora suspeitava de traicao?" << endl; //"traição" não precisa de ajuste aqui, mas vou ajustar para consistência
    }
    if(P4.descoberta){
      cout <<" 4. Por que seu lenço estava ensanguentado e com marcas de ferrugem pesada na cozinha?" << endl;
    }
    if(P7.descoberta && P.descoberta){
      cout <<" 5. Encontramos o frasco de manipulação do seu filho Edward no lixo da cozinha. Estava vazio, alguém andou consumindo ele, e não era o seu filho. \nVocê sabe de algo sobre o frasco?" << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Distante... ultimamente muito distante." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Ele nao ficava em casa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Vivia voltando tarde do trabalho." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Ela evita olhar diretamente para Holmes, projetando uma postura defensiva que esconde um ressentimento profundo." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        P12.descoberta = true;
      break;
    
      case 2:
        limparTela();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- No meu quarto." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Estou enfrentando insonia recentemente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Tenho tomado remedios para dormir." << endl; //"remédios" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 3:
        limparTela();
        cout << "Ela demora alguns segundos para responder." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Um pouco." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Uma intuição que me corroía há meses, detetive." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        cout << "Ela projeta um olhar arrependido." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        investigateClue(table, graph, "P8", inventory, totalClues, realCluesSolved); // Pista 8
        break;

      
      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;

      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}
void arthurInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue P6 = table.search("P6");
  Clue P2 = table.search("P2");
  Clue P9 = table.search("P9");
  
  op = -1;
  do{
    cout << endl << "=== FILHO MAIS VELHO -- ARTHUR WHITMORE ===" << endl;
    cout<<"=== Características ==="<<endl;
    cout << "Controlado." << endl;
    cout << "Ambicioso." << endl;
    cout << "Frio." << endl;

    cout << "\nPara prosseguir aperte Enter..." << endl;
    next();


    cout << "Perguntas:" << endl;
    cout<<" 1. Voce queria assumir a empresa?" << endl;
    cout<<" 2. Seu pai pretendia mudar o testamento?" << endl;
    if(P6.descoberta){
      cout<<" 3. Voces discutiram hoje?" << endl;
    }
    if(P2.descoberta && P9.descoberta){ // A Pergunta importa  
      cout<<" 4. " << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout << "Ele responde rapidamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Alguem precisava assumir eventualmente!" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Meu pai nao poderia continuar para sempre." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 2:
        limparTela();
        cout << "Arthur hesita por um breve instante." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Nao." << endl; //"Não" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Nao que eu saiba." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Arthur encara Eleanor por alguns segundos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "Ela desvia o olhar." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        limparTela();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Sim." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Mas nao foi nada fora do normal." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Arthur fecha os punhos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Se acha que eu mataria meu proprio pai..." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- entao esta completamente maluco." << endl; //"está" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;

      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}
void edwardInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  cout << endl << "=== FILHO DO MEIO -- EDWARD WHITMORE ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Nervoso." << endl;
  cout << "Ansioso." << endl;
  cout << "Instavel." << endl; //"Instável" não precisa de ajuste aqui, mas vou ajustar para consistência

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  do{
    cout << "Perguntas:" << endl;
    cout<<" 1. Voce tinha problemas com seu pai?" << endl;
    cout<<" 2. Onde estava durante o assassinato?" << endl;
    cout<<" 3. Fazendo o que?" << endl;
    cout<<" 4. Voce ouviu alguma coisa suspeita?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Ele nunca me respeitou." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Sempre comparava tudo o que eu fazia com Arthur." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "Os olhos dele demonstram ressentimento." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 2:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- No jardim." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Pensando." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Eu ouvi a discussao..." << endl; //"discussão" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Nao queria participar daquilo." << endl; //"Não" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Holmes percebe um cheiro de cigarro em sua roupa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 4:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Passos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Alguem andando rapido pelo corredor." << endl; //"rápido" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Mas nao consegui ver quem era." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}
void violetInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  cout << endl << "=== FILHA CACULA -- VIOLET WHITMORE ===" << endl; //"CAÇULA" não precisa de ajuste aqui, mas vou ajustar para consistência

  cout<<"=== Características ==="<<endl;
  cout << "Calma demais." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  do{
    cout << "Perguntas:" << endl;
    cout<<" 1. Voce gostava do seu pai?" << endl;
    cout<<" 2. Esta triste?" << endl; //"Está" não precisa de ajuste aqui, mas vou ajustar para consistência
    cout<<" 3. O que acha que aconteceu?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela sorri discretamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Muito." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Nossa relacao era maravilhosa." << endl; //"relação" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 2:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela encara Holmes em silencio." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- E como nao estaria?" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Sherlock observa atentamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela fala sobre tristeza." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Mas nao demonstra nenhuma." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Acho que alguem finalmente perdeu o medo dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "A sala inteira permanece em silencio por alguns segundos." << endl; //"silêncio" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}
void alfredInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  cout << endl << "=== MORDOMO -- ALFRED ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Velho." << endl;
  cout << "Exausto." << endl;
  cout << "Amargurado." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  do{
    cout << "Perguntas:" << endl;
    cout<<" 1. Como Raymond o tratava?" << endl;
    cout<<" 2. O senhor ouviu algo?" << endl;
    cout<<" 3. Reconheceu os passos?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Como mobilia." << endl; //"mobília" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Eu servia a esta familia ha mais de vinte anos." << endl; //"família" e "há" não precisam de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 2:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Passos rapidos no corredor." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "Ele hesita." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Leves demais para serem de um homem." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "Holmes permanece em silencio." << endl; //"silêncio" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}
void interrogationScene(){
  cout << endl << "=== INTERROGATORIO ===" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  
  op = -1;
  while(op != 0){
    limparTela();
    cout<<"Suspeitos: "<<endl;
    cout<<" 1. Eleanor - Esposa"<<endl;
    cout<<" 2. Arthur - Filho mais velho"<<endl;
    cout<<" 3. Edward - Filho do meio"<<endl;
    cout<<" 4. Violet - Filha"<<endl;
    cout<<" 5. Alfred - Mordomo"<<endl;
    cout<<"Opcao: ";
    cin>> op;

    switch(op){
      case 1:
        eleanorInterrogation();
        break;
      case 2:
        arthurInterrogation();
        break;
      case 3:
        edwardInterrogation();
        break;
      case 4:
        violetInterrogation();
        break;
      case 5:
        alfredInterrogation();
        break;
      case 0:
        cout<<"Saindo"<<endl;
        break;
      default:
          cout<<"Opcao invalida"<<endl;
          cout<<"Tente novamente"<<endl; //Não precisa de ajuste
          break;
    }
  }
}


//---------------[Final]---------------
void bestEnding(){
  cout << "Sherlock reunia todos." << endl; //"reunia" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E explica:" << endl;
  cout << "- as facadas eram exageradas demais;" << endl;
  cout << "- a cena foi manipulada;" << endl;
  cout << "- o assassino era menor;" << endl;
  cout << "- alguem lavou as maos;" << endl;
  cout << "- passos leves;" << endl;
  cout << "- ausencia de emocao genuina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Entao Holmes encara Violet." << endl; //"Então" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E ela..." << endl; //"..." não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "sorri." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Nao porque foi pega." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Mas porque:" << endl;
  cout << "finalmente encontrou alguem capaz de entende-la." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << endl << "===== FIM =====" << endl;
};

//---------------[Menus Acusações]---------------
void accusationMenu(){ //Cena de acusação
  int opt = 0;
  limparTela();

  cout << "\nQuem e o assassino?\n"; //"é" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "1 - Eleanor\n2 - Arthur\n3 - Edward\n4 - Violet\n5 - Alfred\n";

  cin >> opt;

  if(opt == 4){
    cout << "\nSherlock observa Violet em silencio...\n";
    cout << "Ela sorri.\n";
    
    bestEnding();
  }
  else{
    cout << "\nHolmes fecha os olhos.\n";
    cout << "A acusacao nao se sustenta.\n";
  }
}

//---------------[Menu Cozinha]---------------
void kitchenMenu(TabelaHash &table, Grafo &graph,Clue inventory[], int &totalClues, int &realCluesSolved){ //Menu da cena da cozinha
  op = -1;
  int sub_op;
  do{
    limparTela();
    cout<<"\n[COZINHA]\n"<< endl;
    
    cout << "O que deseja investigar?" << endl;
    cout << " 1. A Faca na Vitima" << endl;
    cout << " 2. Cadeira Caida" << endl;
    cout << " 3. Taca de Vinho Quebrada" << endl;
    cout << " 4. Sangue na Pia" << endl;
    cout << " 5. Lenco no Chao" << endl;
    cout << " 6. Bituca de Cigarro" << endl;
    cout << " 7. Frasco de Remedio no lixo" << endl;
    cout << " 8. Conversar com alguns empregados" << endl;
    cout << " 9. Ver Inventario" << endl;
    cout << " 0. Se retirar" << endl;

    cin >> op;
    cin.ignore();
      
    switch(op){
      case 1: { // Pistas da Faca
        cout << "\nAnalisando a faca..." << endl;
        cout << "1. Examinar limpeza da lamina" << endl;
        cout << "2. Examinar angulo do golpe" << endl;
        cout << "3. Examinar cabo da faca" << endl;
        cout << "0. Voltar" << endl;
        cin >> sub_op;
        cin.ignore();

        switch(sub_op) {
            case 1:
                investigateClue(table, graph, "P1", inventory, totalClues, realCluesSolved); 
                break;
            case 2:
                investigateClue(table, graph, "P2", inventory, totalClues, realCluesSolved);
                break;
            case 3:
                investigateClue(table, graph, "P3", inventory, totalClues, realCluesSolved);
                break;
            case 0:
                break; // Volta ao menu da cozinha
            default:
              cout << "Opcao invalida." << endl;
        }
        break;
      }
      case 2:
        investigateClue(table, graph, "P9", inventory, totalClues, realCluesSolved);
        break;
      case 3:
        investigateClue(table, graph, "P10", inventory, totalClues, realCluesSolved);
        break;
      case 4:
        investigateClue(table, graph, "P11", inventory, totalClues, realCluesSolved);
        break;
      case 5:
        investigateClue(table, graph, "P4", inventory, totalClues, realCluesSolved);
        break;
      case 6:
        investigateClue(table, graph, "P5", inventory, totalClues, realCluesSolved);
        break;
      case 7:
        investigateClue(table, graph, "P7", inventory, totalClues, realCluesSolved);
        break;
      case 8:
        investigateClue(table, graph, "P6", inventory, totalClues, realCluesSolved);
        break;
      case 9:
        showInventory(inventory, totalClues);
        break;
      case 0:
        cout << "Saindo da cozinha..." << endl;
        break;
      default:
        if (op != 0) cout << "Opcao invalida." << endl;
    }
  } while(op != 0);
}

//---------------[Menu INVESTIGACAO]---------------
void menuManager(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  op = -1;
  TabelaHash t;
  Grafo gInvestigation;

  do{
    limparTela();
    cout << "\n=== MENU DE INVESTIGACAO ===" << endl; 
    cout << "1. Interrogar Suspeitos" << endl;
    cout << "2. Investigar Cena do Crime" << endl;
    cout << "3. Fazer uma acusacao" << endl;
    cout << "0. Desistir" << endl;
    cout << "Opcao: ";
    cin >> op;

    switch(op){
      case 1:
        kitchenMenu(t, gInvestigation, inventory, totalClues, realCluesSolved);
        break;
      case 2:
        interrogationScene();
        break;
      case 3:
        if(realCluesSolved >= 3){
          accusationMenu();
        }else{
          cout << "\nHolmes ainda nao possui evidencias suficientes.\n"; //"evidências" não precisa de ajuste aqui, mas vou ajustar para consistência
        }
        break;
      case 0:
        cout << "Voce foi incapaz de resolver o caso." << endl;
        break;
      default:
        cout << "Opcao invalida." << endl;
        break;
    }
  }while(op != 0);
  
}

//---------------[Criações]---------------
void createClues(TabelaHash &table){
  //----------[Pista 1]----------
  Clue knife;
  knife.idClue = 1;
  knife.title = "Faca Limpa Parcialmente";
  knife.description = "A faca foi limpa parcialmente. Ha sangue na lamina, mas quase nenhuma impressao digital no corpo metalico; o assassino tentou limpar, mas esqueceu ranhuras pequenas proximas ao cabo.";

  knife.question = "O que a ausencia parcial de digitais indica?"; //"ausência" não precisa de ajuste aqui, mas vou ajustar para consistência
  knife.options[0] = "A arma foi trocada";
  knife.options[1] = "O assassino tentou limpar a faca";
  knife.options[2] = "A vitima segurou a faca"; //"vítima" não precisa de ajuste aqui, mas vou ajustar para consistência
  knife.options[3] = "O sangue foi plantado";

  knife.correctReponse = 2;
  knife.realClue = true;
  table.input("P1", knife);
  
  
  
  //----------[Pista 2]----------
  Clue positionKnife;
  positionKnife.idClue = 2;
  positionKnife.title = "Angulo da Faca"; //"Ângulo" não precisa de ajuste aqui, mas vou ajustar para consistência
  positionKnife.description = "A lamina entrou em um angulo descendente incomum. Isso significa que o assassino era significativamente mais baixo que o patriarca OU que ele golpeou a vitima por tras enquanto ela estava sentada.";
  positionKnife.question = "O que o angulo do golpe sugere sobre a natureza do ataque?";
  positionKnife.options[0] = "Foi um confronto direto e equilibrado.";
  positionKnife.options[1] = "A vitima era mais forte que o assassino.";
  positionKnife.options[2] = "O ataque foi covarde, contra uma vitima indefesa ou sentada.";
  positionKnife.options[3] = "A luta foi longa e desordenada.";

  positionKnife.correctReponse = 3;
  positionKnife.realClue = true;
  table.input("P2", positionKnife);
  
  
 
  //----------[Pista 3]----------
  Clue knifeHandle;
  knifeHandle.idClue = 3;
  knifeHandle.title = "Cabo com microfissuras";
  knifeHandle.description = "Existem microfissuras na estrutura de polimero do cabo.\nIsso indica que quem a segurou aplicou uma forca excessiva induzida por tensao extrema, adrenalina e forte obsessao emocional.";
  knifeHandle.question = "Que tipo de estado emocional o assassino demonstrava?";
  knifeHandle.options[0] = "Frieza, calculismo e precisao profissional.";
  knifeHandle.options[1] = "Odio e furia obsessiva.";
  knifeHandle.options[2] = "Medo e legitima defesa.";
  knifeHandle.options[3] = "Ansiedade.";

  knifeHandle.correctReponse = 2; //[1]
  knifeHandle.realClue = true;
  table.input("P3", knifeHandle);
  
  
  
  //----------[Pista Falsa 1]----------
  Clue tissue;
  tissue.idClue = 4;
  tissue.title = "Lenco ensanguentado e com ferrugem";
  tissue.description = "Um lenco de seda fina com o monograma 'E.W.', encontrado muito proximo a faca e parcialmente ensanguentado. Ao inspecionar a fundo, nota-se uma mancha escura de ferrugem de metal velho."; // Descrição já alinhada
  tissue.question = "";
  tissue.options[0] = "";
  tissue.options[1] = "";
  tissue.options[2] = "";
  tissue.options[3] = "";

  tissue.correctReponse = -1;
  table.input("P4", tissue);
  
  
  
  //----------[Pista Falsa 2]----------
  Clue cigarette;
  cigarette.idClue = 5;
  cigarette.title = "Cigarros Turcos";
  cigarette.description = "Uma bituca de cigarro encontrada perto da porta da cozinha. E de uma marca cara de tabaco turco com aroma adocicado de baunilha e filtro dourado.";
  cigarette.question = "Quem possui o habito de fumar e compraria cigarros caros?";
  cigarette.options[0] = "Arthur";
  cigarette.options[1] = "Edward";
  cigarette.options[2] = "Alfred";
  cigarette.options[3] = "William Carter";

  cigarette.correctReponse = 2;
  table.input("P5", cigarette);
 
 
 
  //----------[Pista Falsa 3]----------
  Clue discussion;
  discussion.idClue = 6;
  discussion.title = "Discussao recente do filho mais velho";
  discussion.description = "Empregados relataram uma discussao intensa horas antes do assassinato. O assunto envolvia heranca e controle da empresa da familia.";
  discussion.question = "";
  
  discussion.options[0] = "";
  discussion.options[1] = "";
  discussion.options[2] = "";
  discussion.options[3] = "";

  discussion.correctReponse = -1;
  table.input("P6", discussion);
 
 
 
  //----------[Pista Falsa 4]----------
  Clue medication;
  medication.idClue = 7;
  medication.title = "Remedio antdepressivo";
  medication.description = "Um frasco de antidepressivo encontrado no lixo. O rotulo do paciente esta parcialmente rasgado, mas e possivel ler o nome: 'Edwa...'.";
  medication.question = "";  
  medication.options[0] = "";
  medication.options[1] = "";
  medication.options[2] = "";
  medication.options[3] = "";

  medication.correctReponse = -1;
  table.input("P7", medication);
  

  //----------[Pista 9 - Ambiente]----------
  Clue fallenChair;
  fallenChair.idClue = 9;
  fallenChair.title = "Cadeira Caida";
  fallenChair.description = "Uma cadeira caida distante do centro da cozinha. No chao, ha marcas de arrasto rapido. A disposicao faz parecer que foi colocada ou derrubada propositalmente para simular uma briga corporal.";
  fallenChair.question = "O que a posicao da cadeira sugere?";
  fallenChair.options[0] = "Uma luta longa e violenta";
  fallenChair.options[1] = "A vítima tropeçou e caiu";
  fallenChair.options[2] = "Uma tentativa de forjar a cena do crime";
  fallenChair.options[3] = "A cadeira estava quebrada";

  fallenChair.correctReponse = 3;
  fallenChair.realClue = false;
  table.input("P9", fallenChair);

  //----------[Pista 10 - Ambiente]----------
  Clue wineGlass;
  wineGlass.idClue = 10;
  wineGlass.title = "Taca de Vinho Quebrada";
  wineGlass.description = "Uma unica taca de vinho quebrada no chao. O liquido derramado indica que o homem estava sozinho apreciando a bebida antes do ataque.";
  wineGlass.question = "O que a presenca de uma unica taca indica?";
  wineGlass.options[0] = "A vítima estava esperando um convidado";
  wineGlass.options[1] = "Houve uma festa na cozinha";
  wineGlass.options[2] = "A vitima estava bebendo sozinha";
  wineGlass.options[3] = "O assassino bebeu com a vitima";

  wineGlass.correctReponse = 3;
  wineGlass.realClue = false;
  table.input("P10", wineGlass);

  //----------[Pista 11 - Ambiente]----------
  Clue bloodSink;
  bloodSink.idClue = 11;
  bloodSink.title = "Sangue na Pia";
  bloodSink.description = "Pequenos pingos de sangue perto da pia. O padrao de diluicao mostra que alguem lavou rapidamente as maos ali logo apos o ataque. Um cheiro do detergente permanece no local, um cheiro agradavel de Lavanda.";
  bloodSink.question = "O que os pingos de sangue e o cheiro indicam?";
  bloodSink.options[0] = "A vitima tentou se limpar";
  bloodSink.options[1] = "O assassino lavou as mãos após o crime";
  bloodSink.options[2] = "Alguem se cortou cozinhando";
  bloodSink.options[3] = "A pia estava suja de antes";

  bloodSink.correctReponse = 2;
  bloodSink.realClue = false;
  table.input("P11", bloodSink);

  // ---------[Pista 8 - Conversa]----------
  Clue cheating;
  cheating.idClue = 8;
  cheating.title = "Traicao";
  cheating.description = "Eleanor afirma que suspeitava de uma traicao vinda do marido.";
  cheating.question = "";
  cheating.options[0] = "";
  cheating.options[1] = "";
  cheating.options[2] = "";
  cheating.options[3] = "";

  cheating.correctReponse = -1;
  table.input("P8", cheating);

  // ---------[Pista 12 - Conversa]----------
  Clue suspectCheating;
  suspectCheating.idClue = 12;
  suspectCheating.title = "Suspeita de Traicao";
  suspectCheating.description = "Eleanor demonstrar esconder algo.";
  suspectCheating.question = "";
  suspectCheating.options[0] = "";
  suspectCheating.options[1] = "";
  suspectCheating.options[2] = "";
  suspectCheating.options[3] = "";

  suspectCheating.correctReponse = -1;
  table.input("P12", suspectCheating);

  // ---------[Pista 13 - Conversa]----------
  Clue insomnia;
  insomnia.idClue = 13;
  insomnia.title = "";

  
}
void createConnectionsClue(Grafo &graph){
  graph.addNo("P1", 1);
  graph.addNo("P2", 1);
  graph.addNo("P3", 1);

  graph.addNo("P4", 2);
  graph.addNo("P5", 2);
  graph.addNo("P6", 2);
  graph.addNo("P7", 2);

  //Conexões entre pistas que existem
  graph.addConnections(0,1);
  graph.addConnections(1,2);

  //Conexões falsas
  graph.addConnections(4,6);
}

//---------------[Main]---------------
int main() {
  TabelaHash table;

  Grafo gInvestigation;

  Clue inventory[20];
  int totalClues = 0;

  int realCluesSolved = 0;

  createClues(table);
  createConnectionsClue(gInvestigation);
  
  introStory();
  crimeScene();
  interrogationScene();
  menuManager(table, gInvestigation, inventory, totalClues, realCluesSolved);

  return 0;
}