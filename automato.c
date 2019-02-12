#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATE_LEN 10
#define ALPHABET_LEN 10

#define NIL __INT_MAX__

struct afnd {
  size_t q_len;
  size_t e_len;
  size_t f_len;
  size_t q0;
  char **q;
  char **e;
  size_t *f;
  size_t **afnd;
};

static inline size_t find_idx(char **haystack, size_t len, char *needle) {
  for (size_t i = 0; i < len; i++) {
    if (strcmp(haystack[i], needle) == 0) {
      return i;
    }
  }
  fprintf(stderr, "%d, nao encontrado %s em %s", __LINE__, needle, haystack[0]);
  return -1;
}

static inline size_t find_size_idx(size_t *haystack, size_t len,
                                   size_t needle) {
  for (size_t i = 0; i < len; i++) {
    if (haystack[i] == needle) {
      return needle;
    }
  }
  // fprintf(stderr, "%d nao encontrado %zu em %zu", __LINE__, needle,
  //         haystack[0]);
  return NIL;
}

void AFND_populate(struct afnd *a) {
  char tmp[1 << 7];
  scanf("%s", tmp);
  // puts(tmp);
  // get states
  for (size_t i = 0; i < a->q_len; i++) {
    scanf("%s", a->q[i]);
  }
  scanf("%s", tmp); // ignore name
  // get q0
  scanf("%s", tmp);
  a->q0 = find_idx(a->q, a->q_len, tmp);
  scanf("%s", tmp); // ignore name
  // get final states
  for (size_t i = 0; i < a->f_len; i++) {
    scanf("%s", tmp);
    a->f[i] = find_idx(a->q, a->q_len, tmp);
  }
  scanf("%s", tmp); // ignore name
  // get alphabets
  strcpy(a->e[0], "$");
  for (size_t i = 1; i < a->e_len; i++) {
    scanf("%s", a->e[i]);
  }
  scanf("%s", tmp); // ignore name
  // fill up matrix
  char q0[128];
  char e[128];
  char qf[128];
  size_t i0 = 0, ie = 0;
  while (1) {
    scanf("%s", tmp);
    // printf("getting transition: %s\n",tmp);
    if (!strcmp(tmp, "EOF")) {
      break;
    }
    // sscanf(tmp,"%s:%s>%s",q0,e,qf);
    i0 = 0;
    ie = (strchr(tmp, ':') - tmp);
    memcpy(q0, tmp, ie);
    q0[ie] = '\0';
    i0 = ie;
    ie = (strchr(tmp, '>') - tmp);
    memcpy(e, tmp + i0 + 1, ie - i0);
    e[ie - i0 - 1] = '\0';
    memcpy(qf, tmp + ie + 1, strlen(tmp) - ie);
    qf[strlen(tmp) - ie - 1] = '\0';
    // printf("got: %s %s %s\n",q0,e,qf);
    a->afnd[find_idx(a->q, a->q_len, q0)][find_idx(a->e, a->e_len, e)] =
        find_idx(a->q, a->q_len, qf);
  }
}

// returns a matrix large enough to deal with Q states and E inputs
struct afnd *AFND_new(size_t Q, size_t E, size_t F) {
  struct afnd *out = calloc(1, sizeof(struct afnd));
  E++; // don't ignore empty transition
  out->q_len = Q;
  out->e_len = E;
  out->f_len = F;
  out->q = (char **)malloc(sizeof(char *) * Q);
  out->f = malloc(sizeof(size_t) * F);
  out->e = (char **)malloc(sizeof(char *) * E);
  out->afnd = (size_t **)malloc(Q * sizeof(size_t *));
  for (size_t i = 0; i < Q; i++) {
    out->afnd[i] = (size_t *)malloc(E * sizeof(size_t));
    out->q[i] = malloc(sizeof(char) * STATE_LEN);
  }
  for (size_t i = 0; i < E; i++) {
    out->e[i] = malloc(sizeof(char) * ALPHABET_LEN);
  }
  for (size_t i = 0; i < Q; i++) {
    for (size_t j = 0; j < E; j++) {
      out->afnd[i][j] = NIL;
    }
  }
  AFND_populate(out);
  return out;
}

void AFND_free(struct afnd *automato) {
  for (size_t i = 0; i < automato->q_len; i++) {
    free(automato->afnd[i]);
    free(automato->q[i]);
  }
  for (size_t i = 0; i < automato->e_len; i++) {
    free(automato->e[i]);
  }
  free(automato->afnd);
  free(automato->e);
  free(automato->q);
  free(automato->f);
  free(automato);
}

void AFND_print(struct afnd *automato) {
  printf("len(q) = %zu; len(e) = %zu\n", automato->q_len, automato->e_len);
  printf("q0 = %zu; F = ", automato->q0);
  for (size_t i = 0; i < automato->f_len; i++) {
    printf("%zu, ", automato->f[i]);
  }
  puts("");
  for (size_t i = 0; i < automato->e_len; i++) {
    printf("\t");
    printf("%s, ", automato->e[i]);
  }
  puts("");
  for (size_t i = 0; i < automato->q_len; i++) {
    printf("%s: ", automato->q[i]);
    for (size_t j = 0; j < automato->e_len; j++) {
      if (automato->afnd[i][j] != NIL) {
        printf("\t%s, ", automato->q[automato->afnd[i][j]]);
      } else {
        printf("\tnil");
      }
      // printf("\t%zu",automato->afnd[i][j]);
    }
    puts("");
  }
}

size_t remove_duplicates(size_t vec[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    for (size_t j = 0; j < len; j++) {
      if (i != j && vec[i] == vec[j]) {
        vec[j] = vec[len - 1];
        len--;
      }
    }
  }
  for (size_t i = 0; i < len; i++) {
    for (size_t j = 0; j < len; j++) {
      if (i != j && vec[i] == vec[j]) {
        vec[j] = vec[len - 1];
        len--;
      }
    }
  }
  return len;
}

int AFND_run(struct afnd *a, const size_t *input, size_t len) {
  size_t c_pos = 0;
  size_t *c_states = malloc(a->q_len * sizeof(size_t));
  for (size_t i = 0; i < a->q_len; i++) {
    c_states[i] = NIL;
  }
  c_states[0] = a->q0;
  size_t c_max = 1, tmp;
  int ret = -1;
  while (c_pos < len) {
    printf("%zu\n",c_pos);
    printf("\tPalavra: %s\n", a->e[input[c_pos]]);
    printf("\tEstados: ");
    // printf("%s\n", );
    // printf("c_pos:%zu, c_max:%zu\nc_states:", c_pos, c_max);
    for (size_t i = 0; i < c_max; i++) {
      printf("%s%c",a->q[c_states[i]],(i==c_max-1)?'.':',');
    }
    puts("");
    for (size_t i = 0; i < c_max; i++) {
      if (c_states[i] != NIL) {
        // printf("%d, c_states[i] is not null\n", __LINE__);
        if (a->afnd[c_states[i]][0] != NIL) {
          // if the state has an empty transition
          // check if the state already is in the running states
          // printf("%d, an empty transition exists\n", __LINE__);
          // printf("%zu\n", a->afnd[c_states[i]][0]);
          if ((tmp = find_size_idx(c_states, c_max - 1,
                                   a->afnd[c_states[i]][0])) == NIL) {
            // printf("%d, if it does exist in the running states, state = %zu\n",
            //        __LINE__, a->afnd[c_states[i]][0]);
            // it is not, so we add it to the running state list
            c_states[c_max] = a->afnd[c_states[i]][0];
            c_max++;
          }
        }
        if (a->afnd[c_states[i]][input[c_pos]] == NIL) {
          // invalid input, remove this state from list
          // printf("%d, if it's an invalid transition\n", __LINE__);
          c_max--; // removed by "swapping" with the last state and decreasing
                   // c_max
          // tmp = c_states[i];
          c_states[i] = c_states[c_max];
          c_states[c_max] = NIL;
        } // else { move the state
        // printf("state %zu is now %zu\n", c_states[i],
        //        a->afnd[c_states[i]][input[c_pos]]);
        c_states[i] = a->afnd[c_states[i]][input[c_pos]];
        for (size_t j = i + 1; c_max > 2 && j < c_max - 1; j++) {
          if (c_states[i] == c_states[j]) { // is a duplicate
            // printf("%d, found duplicate %zu %zu\n", __LINE__, c_states[i],
            //        c_states[j]);
            c_max--;
            // tmp = c_states[i];
            c_states[i] = c_states[c_max];
            c_states[c_max] = NIL;
            if (j < c_max - 1) {
              i--; // gotta run on the same i again, as it now is the last
                   // element
              // printf("broke");
              break;
            }
          }
        }
      }
    }
    c_max = remove_duplicates(c_states, c_max);
    c_pos++;
  }
  for (size_t i = 0; i < c_max; i++) {
    if (a->afnd[c_states[i]][0] != NIL) {
      if ((tmp = find_size_idx(c_states, c_max - 1, a->afnd[c_states[i]][0])) ==
          NIL) {
        c_states[c_max] = a->afnd[c_states[i]][0];
        c_max++;
      }
    }
  }
  c_max = remove_duplicates(c_states, c_max);
  printf("%zu\n", c_pos);
  printf("\tPalavra: %s\n", "$");
  printf("\tEstados: ");
  // printf("%s\n", );
  // printf("c_pos:%zu, c_max:%zu\nc_states:", c_pos, c_max);
  for (size_t i = 0; i < c_max; i++) {
    printf("%s%c", a->q[c_states[i]], (i == c_max - 1) ? '.' : ',');
  }
  puts("");
  for (size_t i = 0; i < c_max; i++) {
    // printf("%zu, ", c_states[i]);
    if (find_size_idx(a->f, a->f_len, c_states[i]) != NIL) {
      ret = 1;
    }
  }
  free(c_states);
  return ret;
}

// #undef for_each_node

size_t *stringptr_to_sizetptr(char **base, size_t base_len, char **input,
                              size_t inp_len) {
  size_t *out = malloc(sizeof(size_t) * inp_len);
  for (size_t i = 0; i < inp_len; i++) {
    out[i] = find_idx(base, base_len, input[i]);
  }
  return out;
}

struct explode_out {
  size_t len;
  char *stg[];
}; // this is real nice

// input must be null terminated, otherwise undefined behaviour shall occur,
// it's also destructive to the input string's dividers
struct explode_out *explode(char *input, const char divider) {
  char *tmp = &input[0];
  size_t cont = 1;
  while (tmp[0]) { // count the number of dividers inside the string
                   // printf("%s\n",tmp);
    if (tmp[0] == divider) {
      tmp[0] = '\0';
      cont++;
      // printf("%p\n",(void*)tmp);
    }
    tmp++;
  }
  struct explode_out *out =
      malloc(sizeof(struct explode_out) + sizeof(char *) * cont);
  out->len = cont;
  size_t i = 0;
  tmp = input;
  out->stg[i] = tmp;
  for (; i < cont - 1;) {
    if (tmp[0] == '\0') {
      i++;
      out->stg[i] = ++tmp;
      --tmp;
    }
    // printf("%c",tmp[0]);
    tmp++;
  }
  return out;
}

int main() {
  size_t q = 10, e = 5, f = 1;

  scanf("%zu %zu %zu", &q, &e, &f);
  struct afnd *automato = AFND_new(q, e, f);
  AFND_print(automato);
  char input[1 << 10];
  fgets(input, 1 << 10, stdin);
  fgets(input, 1 << 10, stdin);
  puts(input);
  fflush(stdout);
  struct explode_out *input_separated = explode(input, ',');
  size_t *run = stringptr_to_sizetptr(
      automato->e, automato->e_len, input_separated->stg, input_separated->len);
  size_t run_len = input_separated->len;
  // for(size_t i = 0; i < input_separated->len;i++){
  //  printf("%zu",run[i]);
  // }
  free(input_separated);
  printf("%s\n ", (AFND_run(automato, run, run_len) == 1) ? "Valida" : "Invalida");
  free(run);
  AFND_free(automato);
  return 0;
}
