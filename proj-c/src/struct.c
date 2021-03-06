#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "interface.h"

/**
@file struct.c
Estrutura de dados principal, respetivos
getters e setters e funções auxiliares necessárias (inclusivé as
função que inicializa as estruturas de dados).
*/

/**
\brief Estrutura de dados que armazena informação relativa a todas as estruturas
de elementos necessários para responder às 11 interrogações.
*/
typedef struct TCD_community {
  GHashTable * users;
  GHashTable * questions;
  GHashTable * answers;
  GList * questionsList;
  GList * usersList;
  GPtrArray * day;
  GHashTable * tags;
} TCD_community;


/**
\brief Função que inicializa a estrutura TCD_community.
@returns TAD_community - Apontador para TCD_community
*/
TAD_community init() {
  TAD_community com;

  com = malloc(sizeof(TCD_community));

  com->users =  g_hash_table_new_full
    (g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)cleanUser);

  com->questions = g_hash_table_new_full
    (g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)cleanQuestion);

  com->answers = g_hash_table_new_full
    (g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)cleanAnswer);

  com->tags = g_hash_table_new_full
    (g_str_hash, g_str_equal, g_free, (GDestroyNotify)cleanTags);

  com->questionsList = NULL;

  com->usersList = NULL;

  com->day = g_ptr_array_sized_new(3650);
  g_ptr_array_set_size (com->day, 3650);

  GDate * actualDate = g_date_new_dmy(15, 9, 2008);
  int day, month, year;

  for (int i = 0; i < 3650; i++) {
    Day pointerDay = malloc(sizeDay());

    initDAYQuestions(pointerDay);
    initDAYAnswers(pointerDay);

    day = g_date_get_day(actualDate);
    month = g_date_get_month(actualDate);
    year = g_date_get_year(actualDate);

    setDay(pointerDay, day);
    setMonth(pointerDay, month);
    setYear(pointerDay, year);

    g_ptr_array_insert(com->day, i, pointerDay);

    g_date_add_days(actualDate, 1);
  }

    g_date_free(actualDate);

    return com;
}

/**
\brief Função que procura um user na tabela de hash designada users.
@param t Apontador para TCD_community.
@param id Identificador do user.
@returns Users - Apontador para users.
*/
Users lookUsers(TAD_community t, long id) {
  Users u = g_hash_table_lookup(t->users, GINT_TO_POINTER(id));
  return u;
}

/**
\brief Função que insere um user na tabela de hash designada users.
@param t Apontador para TCD_community.
@param id Identificador do user.
@param pointer Apontador para users.
*/
void insertUser(TAD_community t, long id, Users pointer) {
  g_hash_table_insert(t->users, GINT_TO_POINTER(id), pointer);
}

/**
\brief Função que procura uma pergunta na tabela de hash designada questions.
@param t Apontador para TCD_community.
@param id Identificador da pergunta.
@returns Questions - Apontador para Ghashtable questions.
*/
Questions lookQuestion(TAD_community t, long id) {
  Questions q = g_hash_table_lookup(t->questions, GINT_TO_POINTER(id));
  return q;
}

/**
\brief Função que insere uma pergunta na tabela de hash questions.
@param t Apontador para TCD_community.
@param id Identificador da pergunta.
@param pointer Apontador para questions.
*/
void insertQuestion(TAD_community t, long id, Questions pointer) {
  g_hash_table_insert(t->questions, GINT_TO_POINTER(id), pointer);
}

/**
\brief Função que devolve o número de perguntas armazenadas na tabela de hash questions.
@param t Apontador para TCD_community.
@returns guint - Número de perguntas.
*/
guint getNumberOfQuestions(TAD_community t) {
    guint size = g_hash_table_size(t->questions);

    return size;
}

/**
\brief Função que devolve o endereço da tabela de hash users.
@param t Apontador para TCD_community.
@returns GHashTable * - Apontador para a Ghashtable users.
*/
GHashTable * getHashTableUsers(TAD_community t) {
  return t->users;
}

/**
\brief Função que devolve um apontador para uma lista de perguntas ordenada por data.
@param t Apontador para TCD_community.
@returns GList * - Apontador para GList questionsList.
*/
GList * getQuestions(TAD_community t) {
    if(t->questionsList != NULL)
        return t->questionsList;

    GList * q = g_hash_table_get_values(t->questions);

    q = g_list_sort(q, (GCompareFunc)sortQDate);

    t->questionsList = q;

    return q;
}

/**
\brief Função que devolve um apontador para uma lista de users ordenada por reputação.
@param t Apontador para TCD_community.
@returns GList * - Apontador para GList usersList.
*/
GList * getUsers(TAD_community t) {
    if(t->usersList != NULL)
        return t->usersList;

    GList * u = g_hash_table_get_values(t->users);

    u = g_list_sort(u, (GCompareFunc)sortReputation);

    t->usersList = u;

    return u;
}

/**
\brief Função que devolve o apontador para o dia contido num determinado índice
do GPtrArray day.
@param t Apontador para TCD_community.
@param indexDay Índice de um dia no GPtrArray day.
@returns Day - Apontador para dia do GPtrArray day.
*/
Day lookDay(TAD_community t, long indexDay){
  Day d = g_ptr_array_index(t->day, indexDay);
  return d;
}

/**
\brief Função que insere o apontador do respetivo dia no GPtrArray day.
@param t Apontador para TCD_community.
@param indexDay Índice de um dia no GPtrArray day.
@param pointer Apontador para day.
*/
void insertDay(TAD_community t, long indexDay, Day pointer) {
  g_ptr_array_insert(t->day, indexDay, pointer);
}

/**
\brief Função que devolve o apontador para uma tag contida num determinado índice
da hash table tags.
@param t Apontador para TCD_community.
@param itagName Nome da tag.
@returns Tags - Apontador  para uma tag da GHashTable tags.
*/
Tags lookTag(TAD_community t, char * tagName) {
  gchar * tag_name = g_strdup(tagName);
  Tags tag = g_hash_table_lookup(t->tags, tag_name);
  g_free(tag_name);

  return tag;
}

/**
\brief Função que adiciona uma tag à hash table tags.
@param t Apontador para TCD_community.
@param tagName Nome da tag.
@param tag Apontador para tags.
*/
void insertTag(TAD_community t, char * tagName, Tags tag) {
  g_hash_table_insert(t->tags, g_strdup(tagName), tag);
}


/**
\brief Função que insere uma resposta na tabela de hash designada answers.
@param t Apontador para TCD_community.
@param id Identificador da resposta.
@param pointer Apontador para answers.
*/
void insertAnswers(TAD_community t, long id, Answers pointer) {
  g_hash_table_insert(t->answers, GINT_TO_POINTER(id), pointer);
}

/**
\brief Função que procura uma resposta na tabela de hash designada answers.
@param t Apontador para TCD_community.
@param id Identificador da resposta.
@returns Answers - Apontador para answers.
*/
 Answers lookAnswer(TAD_community t, long id) {
   Answers answer = g_hash_table_lookup(t->answers, GINT_TO_POINTER(id));
  return answer;
}

/**
\brief Função que liberta o espaço de memória da TCD_community.
@param com Apontador para TCD_community.
*/
void cleanStruct(TAD_community com) {
    g_hash_table_destroy(com->users);

    g_hash_table_destroy(com->questions);

    g_hash_table_destroy(com->answers);

    g_list_free(com->questionsList);

    g_list_free(com->usersList);

    g_ptr_array_set_free_func(com->day, (GDestroyNotify)cleanDay);
    g_ptr_array_free(com->day, TRUE);

    g_hash_table_destroy(com->tags);
}
