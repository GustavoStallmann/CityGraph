#ifndef _SMUTREAP_H__
#define _SMUTREAP_H__

#include <stdbool.h>
#include "list.h"

/*
 * Uma SmuTreap e' uma treap espacial com promocao dinamica de nos mais
 * usados.
 * Assim como qualquer Treap, a SmuTreap associa a cada nó inserido na
 * árvore uma prioriodade.
 * A SmuTreap permite que esta prioridade seja alterada posteriormente, 
 * por dois meios:
 *  (a) diretamente pelas operacoes promoteNodeSmuT e demoteNodeSmuT
 *  (b) indiretamente pela frequencia com que o nó e' selecionado.
 *
 * Em relacao ao item (b), dois parametros, informados na criacao
 * da arvore sao considerados: hitCount, promotionRate.
 * O primeiro (hitCount) é a quantidade de vezes que o no' deve ser 
 * selecionado tenha sua prioridade aumentada. O segundo e´ o fator
 * multiplicativo usado para calcular a nova prioridade do nó.
 *
 * A cada no' r da Arvore e' associado um ponto de 2 dimensoes (r.x e r.y) 
 * denominado ancora do no' e uma informacao relacionada tal ponto e 
 * um "descritor" do tipo da informacao.
 * O significado dos valores de tal descritor é definido pela aplicacao.
 * A ancora determina a relacao de ordem entre os elementos armazenados 
 * na arvore. 
 * Sejam dois  nos da arvore: s e t. 
 *            s < t: se (s.x < t.x) OU (s.x = t.x E s.y < t.y)
 *
 * A SmuTreap implementa 2 percursos: em largura e em profundidade.
 * Tais percursos recebem como parametro uma funcao que processa a
 * informacao associada ao no' visitado.
 *
 * Tambem sao providas operacoes de selecao espacial:
 *   A PRIMEIRA seleciona os nos da arvore cujas ancoras
 * sao internas a uma regiao retangular.
 *   A SEGUNDA seleciona os nos da Arvore cujas informacoes associadas
 * podem ser consideradas internas a uma regiao retangular.
 * Esta operacao recebe por parametro uma funcao que decide se
 * tal informacao e' considerada (ou nao) interna.
 *   A TERCEIRA, similarmente, seleciona os nos da arvore cujo um dado
 * ponto e' interno 'a informacao associada ao no'.
 *
 * A SmuTreap e' uma estrutura que se rearranja dinamicamente visando tornar 
 * consultas mais eficientes, a saber:
 *
 *    (a) Cada nó da árvore mantem o bounding box (minimo) que engloba as 
 *        ancoras de suas sub-arvores. Este bounding box é usado para 
          otimizar as operacoes de busca espacial.
 *    (b) Nós mais selecionados sao posicionados mais próximos a raiz. 
 *
 *  Quando for necessario comparar a igualdade de coordenada (por exemlo,
 *  comparar a ancora de um no' com um outro ponto), aceita-se uma pequena 
 * discrepancia entre a coordenada da ancora (anc.x,anc.y) e o ponto (x,y) 
 * de epsilon unidades.
 * Ou seja, distancia(anc,x,y) < epsilon.
 *
 * Modulo a ser implementado para o trabalho 1 da disciplina "Estrutura de Dados 1",
 * do curso de Bacharelado em Ciencia da Computacao da Universidade Estadual 
 * de Londrina, no primeiro semestre de 2025. Obviamente e' expressamente proibido 
 * usar ferramentas de geracao automatica de codigo para fazer a implementacao 
 * deste modulo. Qualquer duvida ou inconsistencia, favor reportar rapidamente 
 * para que correcoes possam ser rapidamente efetuadas.
 */

#define EPSILON 0.5

typedef void *SmuTreap;
typedef void *SmuNode;
typedef void *Info;
typedef int DescritorTipoInfo;

typedef bool (*FdentroDeRegiao)(SmuTreap t, SmuNode n, Info i, double x1, double y1, double x2, double y2);
/*
 * Uma funcao deste tipo deve retornar verdadeiro se a informacao i esta'
 * "dentro" da regiao retangular delimitada pelos pontos opostos (x1,y1) e (x2,y2).
 * Retorna falso, caso contrario.
 */

typedef bool (*FpontoInternoAInfo)(SmuTreap t, SmuNode n, Info i, double x, double y);
/*
 * Uma funcao deste tipo deve retornar verdadeiro se o ponto (x,y)
 * deva ser considerado "interno" `a informacao i.
 */

typedef void (*FvisitaNo)(SmuTreap t, SmuNode n, Info i, double x, double y, void *aux);
/*
 * Processa a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y). O parametro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocacoes a esta funcao.
 */

typedef void (*FCalculaBoundingBox)(DescritorTipoInfo tp, Info i, double *x, double *y, double *w, double *h);
/*
 * Calcula o bounding box da informacao i.
 * Atribui a x,y,w,h, respectivamente, a ancora do retangulo (x,y), a largura e altura
 * do retangulo.
 */

typedef bool (*FsearchNo)(SmuTreap t, SmuNode n, Info i, double x, double y, void *aux);
/*
 * Verifica se a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y) e' a informacao procurada. Retorna verdadeiro, em caso
 * afirmativo; falso, caso contrário. O parametro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocacoes a esta funcao, incluindo (provavelmente) uma chave de busca.
 */

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon, int prio);
/*
 * Retorna uma SmuTreap vazia.
 * hitCount >= 1; promotionRate > 0.0, tipicamente e' um numero positivo maior 
 * que 1.0 (por exemplo, 1.10 significa aumentar em 10%). 
 * Valores na faixa (0.0,1.0) sao possiveis, mas diminuirao a prioridade
 * do no'.
 */

SmuNode insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb);
/*
 * Insere a informacao i, associada 'a ancora (x,y) na arvore t.
 * d e' um valor (definido pela aplicacao) que identifica, caso existam varias
 * categorias, um categoria específica da informacao i.
 * fCalcBb calcula o bounding box da informacao i.
 * Retorna um indicador para o no' inserido.
 */

SmuNode getNodeSmuT(SmuTreap t, double x, double y);
/*
 * Retorna o no' cuja ancora seja o ponto (x,y), admitida a discrepancia
 * epsilon definida na criacao da arvore.
 * Retorna NULL caso nao tenha encontrado o no'.
 */

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, SmuNode n);
/* 
 * Retorna o tipo da informacao associada ao no' n 
*/

void promoteNodeSmuT(SmuTreap t, SmuNode n, double promotionRate);
/*
 * Aumenta a prioridade do no' n pelo fator promotionRate.
 */

void removeNoSmuT(SmuTreap t, SmuNode n);
/*
 * Remove o no' n da arvore. O no' n deve ser um no' valido.
 */

Info getInfoSmuT(SmuTreap t, SmuNode n);
/* 
 * Retorna a informacao associada ao no' n 
 */

Info getBoundingBoxSmuT(SmuTreap t, SmuNode n, double *x, double *y, double *w, double *h);
/* 
 * Retorna o bounding box associado ao no' n 
 */

bool getNodesDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, List L);
/* Insere na List L os nos (SmuNode) da arvore t cujas ancoras estao dentro da regiao 
   delimitada pelos pontos (x1,y1) e (x2,y2).
   Retorna falso, caso nao existam nos dentro da regiao; verdadeiro, caso contrario.
 */

bool getInfosDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, List L);
/* Insere na List L os nos cujas respectivas informacoes associadas estao inteiramente dentro da regiao
   delimitada pelos pontos (x1,y1) e (x2,y2). A funcao f e' usada para determinar se uma
   informacao armazenada na arvore esta' dentro da regiao.
   Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

bool getInfosAtingidoPontoSmuT(SmuTreap t, double x, double y, FpontoInternoAInfo f, List L);
/* Insere na List L  os nos para os quais o ponto (x,y) possa ser considerado
  interno 'as  informacoes associadas ao no'. A funcao f e' invocada para determinar
  se o ponto (x,y) e' interno a uma informacao especifica.
  Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void *aux);
/* Percorre a arvore em profundidade. Invoca a funcao f em cada no visitado.
   O apontador aux e' parametro em cada invocacao de f; assim alguns
   dados podem ser compartilhados entre as diversas invocacoes de f.
 */

void visitaLarguraSmuT(SmuTreap t, FvisitaNo f, void *aux);
/* Similar a visitaProfundidadeSmuT, porem, faz o percurso em largura.
 */

SmuNode procuraNoSmuT(SmuTreap t, FsearchNo f, void *aux);
/* Procura o no' da arvore que contenha um dado especifico.
   Visita cada no' da arvore e invoca a funcao f. A funcao f
   retornara' verdadeiro se o no' contem o dado procurado.
   Neste caso, retorna o no' encontrado. Caso a busca falhe,
   retorna NULL.
 */

bool printDotSmuTreap(SmuTreap t, char *fn);
/* Gera representacao da arvore no arquivo fn, usando a Dot Language
   (ver https://graphviz.org/). Retorna falso, caso o arquivo nao possa
   ser criado (para escrita); true, caso contrario)
*/

void killSmuTreap(SmuTreap t);
/* Libera a memoria usada pela arvore t.
 */

#endif