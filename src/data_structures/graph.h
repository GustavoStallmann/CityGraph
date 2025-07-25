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
#include "list.h"

typedef void *Graph;
typedef int Node;
typedef void *Edge;
typedef void *Info;

/*
  Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada".
  Tambem informa os tempos de descoberta e finalizacao
 */
typedef bool (*procEdge)(Graph g, Edge e, void *td,void *tf, void *extra);

/*
  Invocado quando percurso e' recomecado
 */
typedef bool (*dfsRestarted)(Graph g, void *extra);

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
 * @param "Edge" edge que deseja-se definir a informação
 * @param "info" nova informação que será associado ao edge
 */
void setEdgeInfo(Graph g, Edge e, Info info);

/*
 * Remove uma edge associada ao grafo
 * @param "Graph" grafo que a aresta está associada
 * @param "Edge" edge que será removida
 */
void removeEdge(Graph g, Edge e);

/*
 * Veifica se dois nodes são adjacentes
 * @param "Graph" grafo que os nodes estão associados
 * @param "from" node inicial da associação
 * @param "to" node de destino da associação
 * @return "bool" verdadeiro se forem adjacentes, falso caso contrário
 */
bool isAdjacent(Graph g, Node from, Node to);

/*
 * Copia a lista de adjacencia do node para a lista de destino
 * @param "Graph" grafo que o node está associado
 * @param "node" nó que deseja-se copiar a lista de adjacentes
 * @param "nosAdjacentes" lista de destino 
 */
void adjacentNodes(Graph g, Node node, List nosAdjacentes);

/*
 * Insere na lista dada as arestas (x, y), tal que x == node
 * @param "Graph" grafo cujo nó está associado
 * @param "Node" nó de início da aresta
 * @param "List" lista que será inserida as arestas
 */
void adjacentEdges(Graph g, Node node, List arestasAdjacentes);

/*
 * Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 * @param "Graph" grafo que deseja-se obter o nome dos nodes
 * @param "List" lista de destino dos nomes
 */
void getNodeNames(Graph g, List nomesNodes);

/*
 * Insere na lista "arestas", as arestas de g.
 * @param "Graph" grafo que deseja-se obter a lista de arestas
 * @param "List" lista de destino das arestas
 */
void getEdges(Graph g, List arestas);

/*
   Faz percurso em profundidade sobre  g, a partir do no' node, classificando
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */
// bool dfs(Graph g, Node node, procEdge treeEdge, forwardEdge, returnEdge,
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
 * Calcula o subgrafo composto pelos vertices cujos nomes estao no vetor nomesVerts
 * (nVerts e' o tamanho deste vetor). Caso comAresta seja true calcula o subgrafo
 * induzido pelos vertices em nomesVerts
 * @param "g" grafo principal onde o subgrafo será criado
 * @param "nomeSubgrafo" nome do subgrafo a ser criado
 * @param "nomesVerts" array com os nomes dos vertices que farao parte do subgrafo
 * @param "nVert" tamanho do array nomesVerts
 * @param "comArestas" se true, cria um subgrafo induzido (inclui todas as arestas entre os vertices)
 */
void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas);

/*
 * Adiciona a aresta ao subgrafo.
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo onde a aresta sera adicionada
 * @param "e" aresta a ser adicionada ao subgrafo
 * @return "Edge" a aresta adicionada, ou NULL se o subgrafo nao existir
 */
Edge includeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
 * Retorna verdadeiro se a aresta "e" pertence ao subgrafo "nomeSubgrafo" do grafo g;
 * falso, caso contrario.
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo para verificar
 * @param "e" aresta a ser verificada
 * @return "bool" true se a aresta pertence ao subgrafo, false caso contrario
 */
bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
 * Retira a aresta "e" do subgrafo "nomeSubgrafo". Ou seja, desfaz a correspondente
 * operacao includeEdgeSg previamente executada.
 * Note que a aresta "e" NAO e' removida do grafo g.
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo de onde a aresta sera removida
 * @param "e" aresta a ser removida do subgrafo
 */
void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e);

/*
 * Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que:
 * x == node; x pertence ao subgrafo "nomeSubgrafo", (x,y) tambem e' aresta
 * do subgrafo.
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo para buscar as arestas
 * @param "node" no de origem das arestas adjacentes
 * @param "arestasAdjacentes" lista onde serao inseridas as arestas adjacentes
 */
void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, List arestasAdjacentes);

/*
 * Adiciona 'a lista "lstNodes" (Lista<Node>) os nos do subgrafo "nomeSubgrafo".
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo para obter os nos
 * @param "lstNodes" lista onde serao inseridos os nos do subgrafo
 */
void getAllNodesSDG(Graph g, char *nomeSubgrafo, List lstNodes);

/*
 * Adiciona 'a lista "lstEdges" (Lista<Edge>) as arestas do subgrafo "nomeSubgrafo".
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo para obter as arestas
 * @param "lstEdges" lista onde serao inseridas as arestas do subgrafo
 */
void getAllEdgesSDG(Graph g, char *nomeSubgrafo, List lstEdges);

/*
 * Cria um novo grafo a partir de um subgrafo existente.
 * @param "g" grafo principal que contem o subgrafo
 * @param "nomeSubgrafo" nome do subgrafo a ser convertido em grafo
 * @return "Graph" novo grafo criado a partir do subgrafo, ou NULL se o subgrafo nao existir
 */
Graph produceGraph(Graph g, char *nomeSubgrafo);
#endif
