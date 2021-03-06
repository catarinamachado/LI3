#include <gmodule.h>
#include <stdlib.h>

#include "struct.h"

/**
@file 07mostAnsweredQuestions.c
Resposta à interrogação 7.
*/

/**
\brief Dado um intervalo de tempo arbitrário,
devolve os IDs das N perguntas com mais respostas,
em ordem decrescente do número de respostas.
@param com Apontador para a TCD_community.
@param N Número de perguntas pretendidas.
@param begin Data inicial do intervalo de tempo.
@param end Data final do intervalo de tempo.
@returns LONG_list - IDs das N perguntas com mais respostas,
em ordem decrescente do número de respostas.
*/
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end) {
  int capacity = 10;
  int used = 0;
  int *list = (int *) malloc(sizeof(int) * capacity);

  int n_days, count_day, n_questions, i, sizeTQ = 0;
  long question_id;

  GDate * begin_stackOverflow = g_date_new_dmy (15, 9, 2008);
  GDate * beginDate = g_date_new_dmy(get_day(begin), get_month(begin), get_year(begin));
  GDate * endDate = g_date_new_dmy(get_day(end), get_month(end), get_year(end));

  n_days = g_date_days_between(beginDate, endDate);
  count_day = g_date_days_between(begin_stackOverflow, beginDate);

  GPtrArray * total_questions = g_ptr_array_sized_new(N);
  Questions info_question;

  while(n_days >= 0){
    Day d = lookDay(com, count_day);
    n_questions = getDAYNQuestions(d);
    sizeTQ += n_questions;

    for (i = 0; i < n_questions; i++){
      info_question = getDAYQuestionAtIndex(d, i);
      g_ptr_array_add(total_questions, info_question);
    }

    count_day++; n_days--;
  }

  sortMoreAnswers(total_questions);

  if (N > sizeTQ) N = sizeTQ;

  for(i = 0; ((i < N) && (info_question != NULL)); i++){
    info_question = g_ptr_array_index(total_questions, i);
    if (info_question != NULL){
      question_id = getQuestionId(info_question);
      if(used == capacity){
        capacity *= 2;
        list = (int *) realloc(list, sizeof(int) * capacity);
      }
      list[used] = question_id;
      used++;
    }
  }

  g_date_free(begin_stackOverflow);
  g_date_free(beginDate);
  g_date_free(endDate);

  LONG_list ll = create_list(used);

  for(sizeTQ = 0; sizeTQ < used; sizeTQ++){
    set_list(ll, sizeTQ, list[sizeTQ]);
  }

  free(list);
  g_ptr_array_free(total_questions, TRUE);

  return ll;
}
