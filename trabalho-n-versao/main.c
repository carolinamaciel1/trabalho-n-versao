#include <stdio.h>
#include <pthread.h>


#define CANAL_0 0
#define CANAL_1 1
#define CANAL_2 2
#define CANAL_3 3
#define CANAL_4 4
#define CANAL_5 5
#define VOTO_CORRETO 0
#define VOTO_ERRADO 1
#define STATUS_ERRO 1
#define STATUS_OK 0


int vector_shared_memory[6] = {-1,-1,-1, -1, -1, -1};

void send_async(int *buf, int c) {
    vector_shared_memory[c] = *buf;
    return;
}

void receive(int *buf, int c) {
    while (vector_shared_memory[c]== -1);
        *buf = vector_shared_memory[c];
        vector_shared_memory[c] = -1;
    return;
}

int vector_compare(int vector[], int *wrong_version) {
    if((vector[0]==vector[1]) && (vector[1]==vector[2])) {
        *wrong_version = -1;
        return vector[0];
    }
    if((vector[0]==vector[1]) && (vector[1]!=vector[2])) {
       *wrong_version = 2;
       return vector[0];
    }
    if((vector[0]!=vector[1]) && (vector[1]==vector[2])) {
       *wrong_version = 0;
       return vector[1];
    }
    if((vector[0]==vector[2]) && (vector[1]!=vector[2])) {
       *wrong_version = 1;
       return vector[0];
    }
}

void *thread_a(void *threadno) {
    printf("\nThread A inicia \n");
    int voto = 1, status;
    send_async(&voto, CANAL_0);
    receive(&status, CANAL_3);
    if (status == 0) {
        printf("\nThread A vai\n");
        while (1);
    } else {
        printf("\nThread A finaliza\n");
        return NULL;
    }
}

void *thread_b(void *threadno) {
    printf("\nThread B inicia \n");
    int voto =  2, status;
    send_async(&voto, CANAL_1);
    receive(&status, CANAL_4);
    if (status == 0) {
        printf("\nThread B vai\n");
        while (1);
    } else {
        printf("\nThread B finaliza\n");
        return NULL;
    }
}

void *thread_c(void *threadno) {
    printf(" \nThread C inicia\n");
    int voto = 1, status;
    send_async(&voto, CANAL_2);
    receive(&status, CANAL_5);
    if (status == 0) {
        printf("\nThread C vai\n");
        while (1);
    } else {
        printf("\nThread C finaliza\n");
        return NULL;
    }
}


void *driver(void *threadno) {
    int vetor_comp[3];
    int voto_maj;
    int wrong_version;
    int statuserro = VOTO_ERRADO, statusok = VOTO_CORRETO;

  for (int i = 0; i <= 2; i++)
  {
    receive(&vetor_comp[i], i);
  }


    voto_maj = vector_compare(vetor_comp, &wrong_version);
    printf("\n Voto majoritario: %d ", voto_maj);
    printf("\n");


    if (wrong_version == -1) {
        printf("\nTodos errados\n");
        send_async(&statusok, CANAL_3);
        send_async(&statusok, CANAL_4);
        send_async(&statusok, CANAL_5);
    }
    if (wrong_version == 0) {
    printf("\Thread A gerou voto errado\n");
        send_async(&statuserro, CANAL_3);
        send_async(&statusok, CANAL_4);
        send_async(&statusok, CANAL_5);
    }
    if (wrong_version == 1) {
    printf("\Thread B gerou voto errado \n");
        send_async(&statusok, CANAL_3);
        send_async(&statuserro, CANAL_4);
        send_async(&statusok, CANAL_5);
    }
    if (wrong_version == 2) {
    printf("\Thread C gerou voto errado \n");
        send_async(&statusok, CANAL_3);
        send_async(&statuserro, CANAL_5);
        send_async(&statusok, CANAL_4);
    }
    return NULL;
}


int main(void) {
  pthread_t TA, TB, TC, TD;
    pthread_create(&TA, NULL, thread_a, NULL);
    printf("\n");
    pthread_create(&TB, NULL, thread_b, NULL);
    printf("\n");
    pthread_create(&TC, NULL, thread_c, NULL);
    printf("\n");
    pthread_create(&TD, NULL, driver, NULL);
    printf("\n");
    system("PAUSE");
    return 0;
}
