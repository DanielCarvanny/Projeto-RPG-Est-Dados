import re

file_path = 'main.cpp'
with open(file_path, 'r', encoding='utf-8') as f:
    content = f.read()

replacements = {
    # 1
    r'faca_crime\.description\s*=\s*".*?";': 'faca_crime.description = "Uma faca de cozinha cravada no peito da vitima. A lamina possui partes brilhantes e partes ensanguentadas, mas o metal nao contem impressoes digitais visiveis a olho nu. No entanto, as pequenas ranhuras e reentrancias proximas ao cabo estao escuras com residuos coagulados.";',
    # 2
    r'angulo_facada\.description\s*=\s*".*?";': 'angulo_facada.description = "Holmes inspeciona a ferida. A lamina entrou no corpo da vitima em um angulo descendente agudo (de cima para baixo). Ao olhar para o relatorio de Raymond (1,82m e porte imponente), a fisica desse golpe parece nao se encaixar em um ataque direto padrao.";',
    # 3
    r'microfissuras_cabo\.description\s*=\s*".*?";': 'microfissuras_cabo.description = "Examinando com a lupa, existem varias microfissuras recentes na estrutura de polimero do cabo da faca. Elas nao decorrem de desgaste natural, mas de uma compressao violenta. Quem empunhou esta faca apertou os dedos com uma forca absurda, muito alem da necessidade fisica de desferir um unico golpe.";',
    # 4
    r'cadeira_caida\.description\s*=\s*".*?";': 'cadeira_caida.description = "Ha uma unica cadeira caida de lado, distante do centro da cozinha. No assoalho de madeira sob ela, ve-se uma marca longa de arrasto rapido. Contudo, as panelas no fogao e os talheres nos balcoes em volta estao perfeitamente alinhados, sem nenhum sinal de disturbio.";',
    # 5
    r'taca_vinho\.description\s*=\s*".*?";': 'taca_vinho.description = "Uma elegante taca de cristal estilhacada no chao, rodeada por uma poca de vinho tinto seco. A garrafa de origem nao esta a vista. Holmes examina a pia, os escorredores e os balcoes laterais: nao existe absolutamente nenhum vestigio de uma segunda taca na cozinha.";',
    # 6
    r'sangue_pia\.description\s*=\s*".*?";': 'sangue_pia.description = "Pequenas gotas circulares de sangue secaram na borda de ceramica da pia. O fundo da cuba de metal esta umido, e no ralo nota-se uma leve coloracao rosada de sangue muito diluido em agua. Acima, o frasco de sabonete liquido exala um forte aroma no ar.";',
    # 7
    r'cheiro_lavanda\.description\s*=\s*".*?";': 'cheiro_lavanda.description = "O sabonete usado na pia exala um perfume doce, floral e intenso de lavanda francesa. E um produto importado e luxuoso, completamente diferente dos saboes industriais e asperos que os empregados da casa costumam usar para higienizar potes e talheres na rotina da cozinha.";',
    # 8
    r'lenco_ensanguentado\.description\s*=\s*".*?";': 'lenco_ensanguentado.description = "Um lenco de seda fina abandonado no chao. O tecido impecavel possui o monograma bordado \'E.W.\'. Uma das pontas esta manchada de sangue recente. Na outra extremidade, ha manchas asperas e secas de um po marrom e alaranjado, com um distinto cheiro de metal oxidado.";',
    # 9
    r'cigarros_turcos\.description\s*=\s*".*?";': 'cigarros_turcos.description = "Uma bituca esmagada encostada no rodape da porta. E feita de tabaco turco importado, com filtro dourado e um cheiro persistente de baunilha. E um artigo carissimo. Holmes guarda a bituca em um frasco de vidro.";',
    # 10
    r'discussao_arthur\.description\s*=\s*".*?";': 'discussao_arthur.description = "Os empregados relatam cochichando que presenciaram uma discussao acalorada entre o filho mais velho e a vitima horas antes do crime. O embate foi sobre aposentadoria e testamentos. Raymond supostamente gritou que retiraria o nome de Arthur dos conselhos de administracao.";',
    # 11
    r'frasco_antidepressivo\.description\s*=\s*".*?";': 'frasco_antidepressivo.description = "No lixo da cozinha jaz um pequeno frasco de vidro ambar de farmacia. O rotulo esta parcialmente rasgado, mas o nome do paciente impresso comeca com \'Edwa...\'. A formula quimica rotulada pertence a uma classe de fortes antidepressivos controlados.";',
    # 13 (12 is unchanged)
    r'passos_leves\.description\s*=\s*".*?";': 'passos_leves.description = "Mais de uma pessoa relatou ter ouvido movimentacao no longo corredor espelhado perto da cozinha no horario aproximado do incidente. Foram descritos ritmos de passadas muito rapidas e os impactos no assoalho eram suaves e leves demais.";',
    # 17
    r'sintomas_eleanor\.description\s*=\s*".*?";': 'sintomas_eleanor.description = "Ao observar Eleanor, Holmes nota detalhes sutis: ela apresenta uma forte sonolencia, os musculos das pernas possuem pequenos espasmos involuntarios e as pupilas estao dilatadas. Ela afirma estar sob o efeito de seus sedativos padrao para insonia.";',
    # 18
    r'testamento_sucessao\.description\s*=\s*".*?";': 'testamento_sucessao.description = "Durante uma pergunta incisiva sobre possiveis alteracoes no testamento da familia, Arthur congelou a voz repentinamente e virou o rosto para encarar sua mae, Eleanor, buscando contato visual. Eleanor fechou os olhos e desviou o rosto rapidamente para o lado oposto.";',
    # 19
    r'mordomo_invisivel\.description\s*=\s*".*?";': 'mordomo_invisivel.description = "Alfred serve os Whitmore ha quarenta anos. Com amargura na voz, ele comenta: \'Para o Sr. Raymond, homens da minha classe sao como a mobilia. Somos invisiveis.\' Ele tem livre acesso as chaves e circula pelos corredores tao silenciosamente que os moradores ja nem notam sua presenca.";',
    # 20
    r'cachimbo_alfred\.description\s*=\s*".*?";': 'cachimbo_alfred.description = "Apesar do forte cheiro de fumante impregnado nas roupas de Alfred, ele tira de seu bolso interno um cachimbo de madeira velha e desgastada. Ele resmunga que o salario nao lhe permite comprar os luxuosos tabacos importados vendidos na cidade.";',
    # 21
    r'fones_ouvido\.description\s*=\s*".*?";': 'fones_ouvido.description = "Violet sustenta firmemente que passou toda a noite trancada no quarto de costura, focada em bordar enquanto escutava fonografias musicais em alto volume com seus fones. Ela alega que seu isolamento foi tao profundo que os gritos da policia foram a primeira coisa que ouviu.";',
    # 22
    r'frieza_violet\.description\s*=\s*".*?";': 'frieza_violet.description = "A postura de Violet e de extrema polidez. Ela se refere a tragedia usando as palavras corretas: \'dor\', \'tristeza\' e \'perda\'. No entanto, Holmes observa seu rosto de perto; a simetria de seus sorrisos e a secura absoluta de seus olhos compoem uma reacao quase robotica para uma jovem que acabou de perder o pai.";',
    # 23
    r'perdeu_medo\.description\s*=\s*".*?";': 'perdeu_medo.description = "Ao ser pressionada a dar sua visao pessoal do crime, Violet suaviza a expressao facial e, olhando para o teto, profere uma frase metodica: \'Acho que alguem finalmente perdeu o medo dele.\'";',
    # 24
    r'adrenalina_violet\.description\s*=\s*".*?";': 'adrenalina_violet.description = "Enquanto fala metodicamente, os bracos de Violet estao travados nas laterais do corpo. Ela aperta as proprias maos com tanta forca que os nodulos dos dedos ficam brancos. Sua respiracao e acelerada. E uma tensao muscular caracteristica de uma imensa descarga de adrenalina fisica que ainda nao abandonou o corpo.";',
    # 25
    r'vinho_unhas\.description\s*=\s*".*?";': 'vinho_unhas.description = "Watson chamou a atencao de Holmes para as maos de Violet. O forte perfume de lavanda recende em sua pele. Bem no canto da unha de seu polegar direito e na dobra da manga de seda repousam crostas microscopicas arroxeadas, exalando um levissimo odor fermentado de uva.";',
    # 26
    r'alibi_eleanor\.description\s*=\s*".*?";': 'alibi_eleanor.description = "Eleanor afirma que estava em seu quarto tomando sedativos para insonia. No entanto, mais tarde admitiu ter ido a cozinha para roubar os antidepressivos de Edward.";',
    # 27
    r'alibi_edward\.description\s*=\s*".*?";': 'alibi_edward.description = "Edward afirma que estava no jardim observando o ceu. Mais tarde admitiu ter entrado na cozinha para pegar gelo. Ele diz que viu que o pai nao estava la, ouviu passos de mulher no corredor e fugiu de volta para o jardim.";',
    # 28
    r'alibi_violet\.description\s*=\s*".*?";': 'alibi_violet.description = "Violet afirma estar no quarto de costura no fim do corredor com fones de ouvido.";',
    # 29
    r'tirano_domestico\.description\s*=\s*".*?";': 'tirano_domestico.description = "Quando confrontada sobre as microfissuras na faca, Violet revelou sua verdadeira opiniao sobre o pai: \'Meu pai era um tirano domestico, detetive Holmes. Ele esmagava a individualidade de todos nesta casa. Arthur virou um ganancioso previsivel, Edward um feixe de nervos ansioso e minha mae uma dependente de remedios.\'";',
    # 30
    r'karma_violet\.description\s*=\s*".*?";': 'karma_violet.description = "Violet disse a Sherlock: \'E uma dor imensa que sinto, mas ele nao era uma pessoa boa, acho que isso foi apenas o karma dele.\'";',
}

for pattern, replacement in replacements.items():
    content = re.sub(pattern, replacement, content, flags=re.DOTALL)

with open(file_path, 'w', encoding='utf-8') as f:
    f.write(content)
print("Substituições concluídas.")
