#include <gmodule.h>
#include <assert.h>

#include "struct.h"

/**
@file 10BetterAnswer.c
Resposta à interrogação 10.
*/

/*
Dado o ID de uma pergunta, obtem a melhor resposta.
Para isso, usa a função de média ponderada abaixo:
(score da resposta × 0.45) + (reputação do utilizador × 0.25) +
(número de votos recebidos pela resposta × 0.2) +
(número de comentários recebidos pela resposta × 0.1)
@param com Apontador para a TCD_community.
@param id Identificador da Pergunta.
@returns long - Resposta com melhor média ponderada.
*/

long better_answer(TAD_community com, long id) {
  int i, total_answers, reputation;
  long answer_id;
  double total, max = 0.0;

  Questions question = lookQuestion(com, id);


  if (question == NULL) {
     return -1;
   }

  total_answers = getNAnswers(question);
  assert(total_answers == getAnswersArraySize(question)); // isto tem de dar 1 senão o programa estoura

  for(i = 0; i < total_answers; i++) {
       Users user = lookUsers(com, getAnswerUserIdAtIndex(question, i));

       if (user == NULL) {
         reputation = 0;
       }
       else {
         reputation = getReputation(user);
       }

       total = (getAnswerScoreAtIndex(question, i) * 0.65) + // votos = score
               (reputation * 0.25) +
               (getAnswerCommentAtIndex(question, i) * 0.1);

      if(total > max) {
          max = total;
          answer_id = getAnswerIdAtIndex(question, i);
      }
   }

   return answer_id;
}
