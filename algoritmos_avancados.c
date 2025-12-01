#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Constantes do jogo
#define MAX_NOME 50
#define MAX_PISTA 100
#define MAX_SUSPEITO 50
#define TAMANHO_HASH 10

// Estrutura para um cômodo (nó da árvore binária)
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    bool pista_coletada;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para uma pista (nó da árvore BST)
typedef struct PistaBST {
    char descricao[MAX_PISTA];
    struct PistaBST *esquerda;
    struct PistaBST *direita;
} PistaBST;

// Estrutura para entrada na tabela hash
typedef struct EntradaHash {
    char pista[MAX_PISTA];
    char suspeito[MAX_SUSPEITO];
    struct EntradaHash *proximo; // Encadeamento para resolver colisões
} EntradaHash;

// Estrutura para a tabela hash
typedef struct {
    EntradaHash *tabela[TAMANHO_HASH];
} TabelaHash;

// Protótipos das funções
Sala* criarSala(const char *nome, const char *pista);
Sala* construirArvoreSalas();
void explorarSalas(Sala *raiz, PistaBST **raizPistas, TabelaHash *hash);
void inserirPistaBST(PistaBST **raiz, const char *pista);
void exibirPistasOrdenadas(PistaBST *raiz);
int calcularHash(const char *chave);
void inserirNaHash(TabelaHash *hash, const char *pista, const char *suspeito);
const char* encontrarSuspeito(TabelaHash *hash, const char *pista);
void inicializarTabelaHash(TabelaHash *hash);
void carregarPistasHash(TabelaHash *hash);
void verificarSuspeitoFinal(PistaBST *raizPistas, TabelaHash *hash);
void liberarArvoreSalas(Sala *raiz);
void liberarArvorePistas(PistaBST *raiz);
void liberarTabelaHash(TabelaHash *hash);
void exibirMenu();

int main() {
    printf("=========================================\n");
    printf("       JOGO DE INVESTIGACAO CRIMINAL\n");
    printf("=========================================\n\n");
    
    // Inicializar estruturas
    Sala *arvoreSalas = construirArvoreSalas();
    PistaBST *arvorePistas = NULL;
    TabelaHash tabelaHash;
    
    inicializarTabelaHash(&tabelaHash);
    carregarPistasHash(&tabelaHash);
    
    printf("Um crime ocorreu na mansao! Explore os comodos para coletar pistas.\n");
    printf("Voce deve acusar o suspeito correto ao final da investigacao.\n\n");
    
    // Iniciar exploração
    explorarSalas(arvoreSalas, &arvorePistas, &tabelaHash);
    
    // Verificar acusação final
    verificarSuspeitoFinal(arvorePistas, &tabelaHash);
    
    // Liberar memória
    liberarArvoreSalas(arvoreSalas);
    liberarArvorePistas(arvorePistas);
    liberarTabelaHash(&tabelaHash);
    
    printf("\nFim do jogo. Obrigado por jogar!\n");
    
    return 0;
}

/**
 * Cria dinamicamente um cômodo (sala)
 * @param nome Nome identificador do cômodo
 * @param pista Pista associada ao cômodo
 * @return Ponteiro para a sala criada
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memoria para sala!\n");
        exit(1);
    }
    
    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    strncpy(novaSala->pista, pista, MAX_PISTA - 1);
    novaSala->pista_coletada = false;
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Constrói a árvore binária de cômodos da mansão
 * @return Raiz da árvore de salas
 */
Sala* construirArvoreSalas() {
    // Criação da árvore de salas
    Sala *salaPrincipal = criarSala("Sala Principal", 
        "Uma mancha de tinta azul foi encontrada no tapete.");
    
    Sala *biblioteca = criarSala("Biblioteca", 
        "Um livro sobre quimica estava aberto na mesa.");
    
    Sala *cozinha = criarSala("Cozinha", 
        "Faca com marcas de sangue foi encontrada na pia.");
    
    Sala *escritorio = criarSala("Escritorio", 
        "Uma carta ameacadora estava na gaveta da escrivaninha.");
    
    Sala *quarto = criarSala("Quarto", 
        "Relogio de pulso parado as 21:47.");
    
    Sala *porao = criarSala("Porao", 
        "Uma luva de latex foi encontrada perto da escada.");
    
    Sala *jardim = criarSala("Jardim", 
        "Pegadas com barro foram encontradas perto do portao.");
    
    Sala *banheiro = criarSala("Banheiro", 
        "Um fio de cabelo loiro foi encontrado na pia.");
    
    // Construção da estrutura da árvore
    salaPrincipal->esquerda = biblioteca;
    salaPrincipal->direita = cozinha;
    
    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;
    
    cozinha->esquerda = porao;
    cozinha->direita = jardim;
    
    quarto->esquerda = banheiro;
    
    return salaPrincipal;
}

/**
 * Navega pela árvore de salas e ativa o sistema de pistas
 * @param raiz Raiz da árvore de salas
 * @param raizPistas Ponteiro para raiz da árvore BST de pistas
 * @param hash Ponteiro para tabela hash
 */
void explorarSalas(Sala *raiz, PistaBST **raizPistas, TabelaHash *hash) {
    Sala *atual = raiz;
    char movimento;
    int pistas_coletadas = 0;
    
    printf("\n=== EXPLORACAO DE COMODOS ===\n");
    printf("Use 'e' (esquerda), 'd' (direita), 's' (sair) para navegar.\n");
    
    do {
        printf("\nVoce esta no comodo: [%s]\n", atual->nome);
        
        // Verificar se há pista neste cômodo
        if (!atual->pista_coletada) {
            printf("PISTA ENCONTRADA: %s\n", atual->pista);
            
            // Inserir pista na árvore BST
            inserirPistaBST(raizPistas, atual->pista);
            atual->pista_coletada = true;
            pistas_coletadas++;
            
            printf("Pista coletada! (%d pistas no total)\n", pistas_coletadas);
        } else {
            printf("Voce ja coletou a pista deste comodo.\n");
        }
        
        // Mostrar opções de movimento
        printf("\nOpcoes de movimento:\n");
        if (atual->esquerda != NULL) {
            printf("  'e' - Ir para %s (esquerda)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("  'd' - Ir para %s (direita)\n", atual->direita->nome);
        }
        printf("  's' - Sair da exploracao\n");
        if (atual != raiz) {
            printf("  'v' - Voltar para sala anterior\n");
        }
        
        printf("\nDigite seu movimento: ");
        scanf(" %c", &movimento);
        movimento = tolower(movimento);
        
        // Processar movimento
        switch (movimento) {
            case 'e':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("\nNao ha comodo a esquerda!\n");
                }
                break;
                
            case 'd':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("\nNao ha comodo a direita!\n");
                }
                break;
                
            case 'v':
                // Sistema simples para voltar (poderia ser melhorado com pilha)
                printf("\nVoltando para a Sala Principal...\n");
                while (atual != raiz && raiz != NULL) {
                    // Encontrar o caminho de volta (implementação simplificada)
                    if (raiz->esquerda == atual || raiz->direita == atual) {
                        atual = raiz;
                    } else if (raiz->esquerda != NULL) {
                        // Verificar subárvore esquerda
                        Sala *temp = raiz;
                        while (temp->esquerda != NULL && temp->esquerda != atual) {
                            temp = temp->esquerda;
                        }
                        if (temp->esquerda == atual) {
                            atual = temp;
                            break;
                        }
                    }
                    // Simplificação: voltar para raiz
                    atual = raiz;
                }
                break;
                
            case 's':
                printf("\nSaindo da exploracao...\n");
                break;
                
            default:
                printf("\nMovimento invalido! Tente novamente.\n");
        }
        
    } while (movimento != 's');
}

/**
 * Insere uma pista na árvore BST de forma ordenada
 * @param raiz Ponteiro para raiz da árvore BST
 * @param pista Descrição da pista a ser inserida
 */
void inserirPistaBST(PistaBST **raiz, const char *pista) {
    if (*raiz == NULL) {
        // Criar novo nó
        PistaBST *novoNo = (PistaBST*)malloc(sizeof(PistaBST));
        if (novoNo == NULL) {
            printf("Erro ao alocar memoria para pista!\n");
            exit(1);
        }
        
        strncpy(novoNo->descricao, pista, MAX_PISTA - 1);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        
        *raiz = novoNo;
        return;
    }
    
    // Comparar para determinar se vai para esquerda ou direita
    int comparacao = strcmp(pista, (*raiz)->descricao);
    
    if (comparacao < 0) {
        inserirPistaBST(&((*raiz)->esquerda), pista);
    } else if (comparacao > 0) {
        inserirPistaBST(&((*raiz)->direita), pista);
    }
    // Se for igual, não insere duplicado
}

/**
 * Exibe as pistas em ordem (travessia in-order)
 * @param raiz Raiz da árvore BST de pistas
 */
void exibirPistasOrdenadas(PistaBST *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    exibirPistasOrdenadas(raiz->esquerda);
    printf("- %s\n", raiz->descricao);
    exibirPistasOrdenadas(raiz->direita);
}

/**
 * Calcula o valor hash para uma chave
 * @param chave String para calcular hash
 * @return Índice na tabela hash
 */
int calcularHash(const char *chave) {
    int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash = (hash * 31 + chave[i]) % TAMANHO_HASH;
    }
    return hash;
}

/**
 * Inicializa a tabela hash
 * @param hash Ponteiro para tabela hash
 */
void inicializarTabelaHash(TabelaHash *hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

/**
 * Insere uma associação pista-suspeito na tabela hash
 * @param hash Ponteiro para tabela hash
 * @param pista Chave (pista)
 * @param suspeito Valor (suspeito)
 */
void inserirNaHash(TabelaHash *hash, const char *pista, const char *suspeito) {
    int indice = calcularHash(pista);
    
    // Criar nova entrada
    EntradaHash *novaEntrada = (EntradaHash*)malloc(sizeof(EntradaHash));
    if (novaEntrada == NULL) {
        printf("Erro ao alocar memoria para entrada hash!\n");
        exit(1);
    }
    
    strncpy(novaEntrada->pista, pista, MAX_PISTA - 1);
    strncpy(novaEntrada->suspeito, suspeito, MAX_SUSPEITO - 1);
    
    // Inserir no início da lista encadeada (resolução de colisões)
    novaEntrada->proximo = hash->tabela[indice];
    hash->tabela[indice] = novaEntrada;
    
    // printf("DEBUG: Inserido '%s' -> '%s' no indice %d\n", pista, suspeito, indice);
}

/**
 * Carrega as associações pista-suspeito na tabela hash
 * @param hash Ponteiro para tabela hash
 */
void carregarPistasHash(TabelaHash *hash) {
    // Associações pista -> suspeito
    inserirNaHash(hash, 
        "Uma mancha de tinta azul foi encontrada no tapete.", 
        "Joao");
    
    inserirNaHash(hash, 
        "Um livro sobre quimica estava aberto na mesa.", 
        "Maria");
    
    inserirNaHash(hash, 
        "Faca com marcas de sangue foi encontrada na pia.", 
        "Carlos");
    
    inserirNaHash(hash, 
        "Uma carta ameacadora estava na gaveta da escrivaninha.", 
        "Ana");
    
    inserirNaHash(hash, 
        "Relogio de pulso parado as 21:47.", 
        "Pedro");
    
    inserirNaHash(hash, 
        "Uma luva de latex foi encontrada perto da escada.", 
        "Joao");
    
    inserirNaHash(hash, 
        "Pegadas com barro foram encontradas perto do portao.", 
        "Carlos");
    
    inserirNaHash(hash, 
        "Um fio de cabelo loiro foi encontrado na pia.", 
        "Ana");
}

/**
 * Encontra o suspeito correspondente a uma pista
 * @param hash Ponteiro para tabela hash
 * @param pista Pista para buscar
 * @return Nome do suspeito ou NULL se não encontrado
 */
const char* encontrarSuspeito(TabelaHash *hash, const char *pista) {
    int indice = calcularHash(pista);
    EntradaHash *atual = hash->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return NULL; // Pista não encontrada
}

/**
 * Conduz à fase de julgamento final
 * @param raizPistas Raiz da árvore BST de pistas
 * @param hash Ponteiro para tabela hash
 */
void verificarSuspeitoFinal(PistaBST *raizPistas, TabelaHash *hash) {
    printf("\n=== FASE DE JULGAMENTO ===\n");
    
    // Mostrar todas as pistas coletadas
    printf("\nPISTAS COLETADAS (ordenadas alfabeticamente):\n");
    printf("--------------------------------------------\n");
    
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada!\n");
        return;
    }
    
    exibirPistasOrdenadas(raizPistas);
    
    // Coletar acusação do jogador
    char suspeitoAcusado[MAX_SUSPEITO];
    printf("\nSUSPEITOS POSSIVEIS: Joao, Maria, Carlos, Ana, Pedro\n");
    printf("\nDigite o nome do suspeito que voce acusa: ");
    scanf(" %[^\n]", suspeitoAcusado);
    
    // Contar quantas pistas apontam para o suspeito acusado
    int contagem = 0;
    const char *suspeitos[MAX_PISTA];
    int idx = 0;
    
    // Função auxiliar para contar pistas
    void contarPistas(PistaBST *no) {
        if (no == NULL) return;
        
        const char *suspeito = encontrarSuspeito(hash, no->descricao);
        if (suspeito != NULL && strcmp(suspeito, suspeitoAcusado) == 0) {
            contagem++;
            if (idx < MAX_PISTA) {
                suspeitos[idx++] = no->descricao;
            }
        }
        
        contarPistas(no->esquerda);
        contarPistas(no->direita);
    }
    
    contarPistas(raizPistas);
    
    // Verificar resultado
    printf("\n=========================================\n");
    printf("          RESULTADO FINAL\n");
    printf("=========================================\n");
    
    if (contagem >= 2) {
        printf("\nPARABENS! Voce resolveu o caso!\n");
        printf("Voce coletou %d pista(s) que apontam para %s.\n", contagem, suspeitoAcusado);
        printf("\nPistas que incriminam %s:\n", suspeitoAcusado);
        for (int i = 0; i < idx; i++) {
            printf("- %s\n", suspeitos[i]);
        }
        printf("\n%s foi preso e confessou o crime!\n", suspeitoAcusado);
    } else if (contagem == 1) {
        printf("\nHmm... Poucas evidencias contra %s.\n", suspeitoAcusado);
        printf("Apenas %d pista aponta para ele/ela.\n", contagem);
        printf("Talvez voce devesse coletar mais pistas...\n");
        printf("\nCaso arquivado por falta de provas.\n");
    } else {
        printf("\nVoce acusou %s, mas nenhuma pista coletada aponta para ele/ela!\n", suspeitoAcusado);
        printf("O verdadeiro culpado continua solto...\n");
    }
}

/**
 * Libera a memória da árvore de salas
 * @param raiz Raiz da árvore de salas
 */
void liberarArvoreSalas(Sala *raiz) {
    if (raiz == NULL) return;
    
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

/**
 * Libera a memória da árvore BST de pistas
 * @param raiz Raiz da árvore BST
 */
void liberarArvorePistas(PistaBST *raiz) {
    if (raiz == NULL) return;
    
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

/**
 * Libera a memória da tabela hash
 * @param hash Ponteiro para tabela hash
 */
void liberarTabelaHash(TabelaHash *hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        EntradaHash *atual = hash->tabela[i];
        while (atual != NULL) {
            EntradaHash *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}