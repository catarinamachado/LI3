#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <answers.h>
#include <questions.h>

#include "users.h"


/*
Estrutura de dados destinada a armazenar as informações referentes aos usuários
*/
typedef struct users {
  long user_id;
  char * shortbio;
  char * username;
  int reputation;
  int n_posts;
  GArray * last_posts;
} users;


/*
Estrutura de dados que armazena o id do utilizador e a quantidade
de posts que este publicou. Esta estrutura será relevante para
responder à interrogação 2.
*/
typedef struct totalPosts {
  long user_id;
  int n_posts;
} totalPosts;


/*
Função que devolve o tamanho da estrutura dos totalPosts.
*/
int getTotalPostsSize() {
    return sizeof(struct totalPosts);
}

/*
Função que devolve o número total de posts de um utilizador, sendo passado como parametro
um apontador para totalPosts
*/
int getTotalNPosts(PtrTotalPosts total_post) {
  return total_post->n_posts;
}

/*
Função que devolve o id de um utilizador, sendo passado como parametro
um apontador para totalPosts
*/
int getTotalPostsUserId(PtrTotalPosts total_post) {
  return total_post->user_id;
}

/*
Função que estabelece o número de posts da estrutura totalPosts.
*/
void setTotalNPosts(PtrTotalPosts total_posts, int n_posts) {
  total_posts->n_posts = n_posts;
}

/*
Função que estabelece o id do utilizador a figurar na estrutura totalPosts.
*/
void setTotalPostsUserId(PtrTotalPosts total_posts, long user_id) {
  total_posts->user_id = user_id;
}

/*
Função que insere num GArray os dois campos da estrutura totalPosts.
*/
void insertTotalPostsArray(GArray * array, int user_id, int n_posts) {
  totalPosts total;

  total.user_id = user_id;
  total.n_posts = n_posts;

  g_array_append_val(array, total);
}

/*
Função que devolve o id de um user, contido na estrutura totalPosts,
que se encontra numa determinada posição de um GArray.
*/
long getTotalPostsUserIdAtIndex(GArray * array, int index) {
   totalPosts total;

   total = g_array_index(array, totalPosts, index);

   return total.user_id;
}
/*
//Função utilizada só para testar
int getTotalPostsAtIndex(GArray * array, int index) {
  totalPosts total;

  total = g_array_index(array, totalPosts, index);

  return total.n_posts;
}
*/

/*
Função que inicializa um GArray com a estrutura totalPosts.
*/
GArray * initArrayTotalPosts() {
    return g_array_new(FALSE, FALSE, getTotalPostsSize());
}

/*
Função que compara o número de posts de duas posições da estutura totalPost
*/
gint compare_func(gconstpointer n_posts_a, gconstpointer n_posts_b) {
  PtrTotalPosts a = (PtrTotalPosts) n_posts_a;
  PtrTotalPosts b = (PtrTotalPosts) n_posts_b;

//se a < b return n.º negativo se a = b return 0 e se a > b return n.º positivo
  return getTotalNPosts(b) - getTotalNPosts(a);
}

/*
Função que ordena um GArray por ordem decrescente do número de posts.
*/
void sortArrayTotalPosts(GArray * array) {

  g_array_sort(array, (GCompareFunc)compare_func);
}

/*
Função que liberta o espaço de memória ocupada pelo GArray.
*/
void freeArrayTotalPosts(GArray * array) {
  g_array_free (array, TRUE);
}

/*
Devolve o tamanho da estrutura dos users.
*/
int getUsersSize() {
    return sizeof(struct users);
}

/*
Devolve o id de um utilizador, sendo passado como parametro a estrutura users.
*/
long getUserId(Users u) {
    return u->user_id;
}

/*
Estabelece o id de um utilizador, sendo passado como parametro a estrutura users.
*/
void setUserId(Users u, long i) {
    u->user_id = i;
}

/*
Devolve a informação sobre o perfil de um utilizador.
*/
char * getBio(Users u) {
    char * bio = malloc(sizeof(char) * (strlen(u->shortbio)+2));
    strcpy(bio, u->shortbio);

    return bio;
}

/*
Estabelece a informação sobre o perfil de um utilizador.
*/
void setBio(Users u, char * bio) {
    char * shortbio = malloc(sizeof(char) * (strlen(bio)+2));
    strcpy(shortbio, bio);

    u->shortbio = shortbio;
}

/*
Devolve o nome de um utilizador.
*/
char * getUsername(Users u) {
    char * user = malloc(sizeof(char) * (strlen(u->username)+2));
    strcpy(user, u->username);

    return user;
}

/*
Estabelece o nome de um utilizador.
*/
void setUsername(Users u, char * user) {
    char * username = malloc(sizeof(char) * (strlen(user)+2));
    strcpy(username, user);

    u->username = username;
}

/*
Devolve a reputação de um utilizador.
*/
int getReputation(Users u) {
    return u->reputation;
}

/*
Estabelece a reputação de um utilizador.
*/
void setReputation(Users u, int reputation) {
    u->reputation = reputation;
}

/*
Devolve o número de posts de um utilizador.
*/
int getNPosts(Users u) {
    return u->n_posts;
}

/*
Estabelece o número de posts de um utilizador.
*/
void setNPosts(Users u, int posts) {
    u->n_posts = posts;
}

/*
Incrementa em uma unidade o número de posts de um utilizador.
*/
void incrementNPosts(Users u) {
    u->n_posts++;
}

/*
Acrescenta ao array last_posts informação sobre a data do post.
*/
void appendPost(Users u, postDate pd) {
    g_array_append_val(u->last_posts, pd);
}

/*
Inicializa o array de post do utilizador com dez elementos.
*/
void initLastPosts(Users u) {
    u->last_posts = g_array_sized_new(FALSE, TRUE, sizeof(postDate), 10);
}

/*
Devolve um apontador para o l elemento do array last_post,
dando-nos informação sobre a data do post.
*/
postDate getPDindex(Users u, int l) {
    postDate pd = g_array_index(u->last_posts, postDate, l);

    return pd;
}

/*
Função que compara as datas de dois posts recebidos como parametro.
*/
static gint sortDate(postDate a, postDate b) {
    if (getPDYear(a) < getPDYear(b))
        return -1;
    else if (getPDYear(a) > getPDYear(b))
        return 1;

    else if (getPDMonth(a) < getPDMonth(b))
        return -1;
    else if (getPDMonth(a) > getPDMonth(b))
        return 1;

    else if (getPDDay(a) < getPDDay(b))
        return -1;
    else if (getPDDay(a) > getPDDay(b))
        return 1;

    else if (getPDHour(a) < getPDHour(b))
        return -1;
    else if (getPDHour(a) > getPDHour(b))
        return 1;

    else if (getPDMin(a) < getPDMin(b))
        return -1;
    else if (getPDMin(a) > getPDMin(b))
        return 1;

    else if (getPDSec(a) < getPDSec(b))
        return -1;
    else if (getPDSec(a) > getPDSec(b))
        return 1;

    else if (getPDMili(a) < getPDMili(b))
        return -1;

    else if (getPDMili(a) > getPDMili(b))
        return 1;

    return 1;
}

/*
Função que ordena os posts do utilizador de acordo com a sua data,
(do mais recente para o mais antigo).
*/
void sortPosts(Users u) {
    g_array_sort(u->last_posts, (GCompareFunc)sortDate);
}

/*
Função que compara as reputações de dois users recebidos como parametro.
*/
static gint sortReputation(Users * u1, Users * u2) {
  int rep1, rep2;
  if ((*u1) != NULL)
    rep1 = getReputation(*u1);
  else
    rep1 = 0;
  if ((*u2) != NULL)
    rep2 = getReputation(*u2);
  else
    rep2 = 0;

  if(rep1 < rep2)
    return 1;
  else
    return -1;
}

/*
Função que ordena os users de acordo com a sua reputação,
(do user com maior reputação para o user com menos).
*/
void sortUsersReputation(GPtrArray * total_users) {
  g_ptr_array_sort(total_users, (GCompareFunc)sortReputation);
}

void cleanUser(Users u) {
    free(u->shortbio);
    free(u->username);

    g_array_set_clear_func(u->last_posts, (GDestroyNotify)cleanPDp);
    g_array_free(u->last_posts, TRUE);

    free(u);
}
