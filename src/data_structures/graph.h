#ifndef _GRAPH_
#define _GRAPH_

/*
Um Grafo  G e' constituido por um conjunto de vertices V e
um conjunto de arestas E, denotado por G=(V,E).

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Podem ser definidos subgrafos de um dado grafo. Um subgrafo pode ser entendido
como uma visão (ou filtro) sobre o grafo original. Ou, ainda, um subgrafo
define (delimita) uma "região" do grafo a ser acessada.

Cada subgrafo possui um nome. Um grafo pode pode possuir vários subgrafos.
Arestas podem ser acrescentas ou retiradas do subgrafo
Existem tambem outras operacoes que possibilitam percorrer o subgrafo,
invocando funcoes quando arestas do grafo
sao "descobertas", podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Daddos do
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/

#include <stdbool.h>
#include <string.h>

typedef void *Graph;
typedef int Node;
typedef void *Edge;
typedef void *Info;

/*
  Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada".
  Tambem informa os tempos de descoberta e finalizacao
 */
// bool (*procEdge)(Graph g, Edge e, void *td,void *tf, void *extra);

/*
  Invocado quando percurso e' recomecado
 */
// bool (*dfsRestarted)(Graph g, void *extra);

/*
 * Cria uma nova instancia de grafo
 * @param "nVert" quantidade máxima de vértices
 * @param "direct" determina se o grafo é direcionado ou não
 * @param "nomeGrafo" define o nome dado ao grafo
 * @retunr "Graph" nova instancia de grafo
*/
Graph createGraph(int nVert, bool directed, char *nomeGrafo);

/*
 * Retorna numero máximo de vertices de um grafo
 * @param "Graph" grafo que deseja obter a quantidade
 * @param "int" quantidade máxima de vértices
 */
int getMaxNodes(Graph g);

/*
 * Retorna numero total de vertices adicionados ao grafo
 * @param "Graph" grafo que deseja obter a quantidade
 * @return "int" quantidade de vertices
 */
int getTotalNodes(Graph g);

/*
 * Adiciona um novo vértice ao grafo
 * @param "Graph" grafo que terá o vértice adicionado
 * @param "nome" nome do node adicionado
 * @param "info" informação associada ao nó
 * @return "Node" novo nó adicionado
*/
Node addNode(Graph g, char *nome, Info info);

/*
 * Busca um nó de um grafo pelo seu nome
 * @param "g" grafo para busca
 * @param "nome" nome do node para busca
 * @return "Node" id do node ou -1 caso não encontre uma correspondência
 */
Node getNode(Graph g, char *nome);

/*
 * Retorna a informação associada ao nó dado
 * @param "Graph" grafo que o nó é associado
 * @param "node" nó que deseja obter a informação
 * @return "Info" a informação associada ao nó
 */
Info getNodeInfo(Graph g, Node node);

/*
 * Retorna o nome do node dado
 * @param "Graph" grafo que o nó é associado
 * @param "node" nó que deseja-se obter o nome
 * @return O nome associado ao nó
 */
char* getNodeName(Graph g, Node node);

/*
 * Define a informação associada a um vértice
 * @param "Graph" grafo que o nó está associado
 * @param "node" nó que terá a informação alterada
 * @param "info"  nova informação do node
 */
void setNodeInfo(Graph g, Node node, Info info);

/*
 * Adiciona uma nova aresta ao nó
 * @param "Graph" grafo que os nós estão associados
 * @param "from" nó de partida da aresta
 * @param "to" nó final de destino da aresta
 * @param "info" informação que a aresta armazenará
 * @return "Edge" nova aresta criada
 */
Edge addEdge(Graph g, Node from, Node to, Info info);

/*
 * Retorna uma aresta que relaciona os nós
 * @param "Graph" grafo que os nós estão associados
 * @param "from" nó de partida da aresta
 * @param "to" nó final de destino da aresta
 * @return "Edge" aresta que associa os nós
 */
Edge getEdge(Graph g, Node from, Node to);

/*
 * Retorna o node "from" associado a uma aresta
 * @param "Graph" grafo que o edge está associado
 * @param "e" edge que deseja-se obter o node "from"
 * @return "Node" node associado como "from" no edge
*/
Node getFromNode(Graph g, Edge e);

/*
 * Retorna o node "to" associado a uma aresta
 * @param "Graph" grafo que o edge está associado
 * @param "e" edge que deseja-se obter o node "to"
 * @return "Node" associado como "to" no edge
 */
Node getToNode(Graph g, Edge e);

/*
 * Retorna a iformação associada a uma aresta
 * @param "Graph" grafo que a aresta está associada
 * @param "e" edge que deseja-se obter a informação
 * @return "Info" a informação associada ao edge
 */
Info getEdgeInfo(Graph g, Edge e);

/*
 * Define a informação associada a um edge 
 * @param "Graph" grafo que a aresta está associada
 * @param "e" edge que deseja-se definir a informação
 * @param "info" nova informação que será associado ao edge
 */
void setEdgeInfo(Graph g, Edge e, Info info);

/*
 */
void removeEdge(Graph g, Edge e);

/*
 */
bool isAdjacent(Graph g, Node from, Node to);

/*
   Adiciona 'a lista "nosAdjacentes" os nos adjacentes 'a "node".
 */
// void adjacentNodes(g, node, nosAdjacentes);

/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que,
   x == node.
 */
// void adjacentEdges(g, node, Lista arestasAdjacentes);

/*
   Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 */
// void  getNodeNames(Graph g, Lista nomesNodes);

/*
   Insere na lista "arestas", as arestas de g.
 */
// void getEdges(g, Lista arestas);

/*
   Faz percurso em profundidade sobre  g, a partir do no' node, classificando
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */
// bool dfs(g, node, procEdge treeEdge, forwardEdge, returnEdge,
// 	 crossEdge, newTree, void *extra);

/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
// bool bfs(g, node, discoverNode, void *extra);

/*
   Destroi o grafo "g".
 */
void killDG(Graph g);

/*
 **********************
 * SUB-GRAFOS
 **********************
*/

/*
    Calcula o subgrafo composto  pelos vertices cujos nomes estao no vetor nomesVerts
(nVerts e' o tamanho deste vetor). Caso comAresta seja true calcula o subgrafo
induzido pelos vertices em nomesVers
*/

// void  createSubgraphDG(g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas);

/*
    Adiciona a aresta ao subgrafo.
 */
// Edge includeEdgeSDG(g, char *nomeSubgrafo, e);

/*
  Retorna verdadeiro se a aresta "e" pertence ao subgrafo "nomeSubgrafo" do grafo g;
  falso, caso contrario.
 */
bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
  Retira a aresta "e" do subgrafo "nomeSubgrafo". Ou seja, desfaz a correspondente
  operacao includeEdgeSg previamente executada.
  Note que a aresta  "e" NAO e' removida do grafo g.
 */
void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que:
   x == node; x pertence ao subgrafo "nomeSubgrafo", (x,y) tambem e' aresta
   do subgrafo.
 */
// void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, Lista arestasAdjacentes);

/*
   Adiciona 'a lista "lstNodes" (Lista<Node>) os nós do subgrafo "nomeSubgrafo".
 */
// void getAllNodesSDG(Graph g, char *nomeSubgrafo,  Lista lstNodes);

/*
   Adiciona 'a lista "lstEdges" (Lista<Edge>) as arestas do subgrafo "nomeSubgrafo".
 */
// void getAllEdgesSDG(Graph g, char *nomeSubgrafo, Lista lstEdges);

/*
  Novo grafo.
 */
// Graph produceGraph(g, nomeSubgrafo);
#endif
