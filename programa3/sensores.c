#include "sensores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t gerar_timestamp_aleatorio(struct tm *inicial, struct tm *final) {
    time_t timestamp_inicial = mktime(inicial);
    if (timestamp_inicial == -1) {
        printf("Data inicial invalida.\n");
        return -1;
    }

    time_t timestamp_final = mktime(final);
    if (timestamp_final == -1) {
        printf("Data final invlida.\n");
        return -1;
    }

    return timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
}

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon  = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min  = min;
    t.tm_sec  = seg;
    t.tm_isdst = -1;

    time_t ts = mktime(&t);
    if (ts == -1) {
        printf("Data invalida: %02d/%02d/%04d %02d:%02d:%02d\n", dia, mes, ano, hora, min, seg);
    }
    return ts;
}

void converter_tipo(const char *tipo_entrada, char *tipo_convertido) {
    if (strcmp(tipo_entrada, "CONJ_Z") == 0) {
        strcpy(tipo_convertido, "int");
    } else if (strcmp(tipo_entrada, "CONJ_Q") == 0) {
        strcpy(tipo_convertido, "float");
    } else if (strcmp(tipo_entrada, "TEXTO") == 0) {
        strcpy(tipo_convertido, "string");
    } else if (strcmp(tipo_entrada, "BINARIO") == 0) {
        strcpy(tipo_convertido, "bool");
    } else {
        strcpy(tipo_convertido, "tipo_invalido");
        printf("Tipo invalido recebido: %s\n", tipo_entrada);
    }
}

void gerar_valor_aleatorio_por_tipo(const char *tipo, char *saida) {
    if (strcmp(tipo, "int") == 0) {
            int valor = rand() % 1000;
        sprintf(saida, "%d", valor);
    } else if (strcmp(tipo, "float") == 0) {
             float valor = (float)(rand() % 10000) / 100.0;
        sprintf(saida, "%.2f", valor);
    } else if (strcmp(tipo, "bool") == 0) {
            int b = rand() % 2;
        strcpy(saida, b ? "true" : "false");
    } else if (strcmp(tipo, "string") == 0) {
            const char *opcoes[] = {"OK", "ERRO", "ALERTA"};
        int idx = rand() % 3;
            strcpy(saida, opcoes[idx]);
    } else {
        
        strcpy(saida, "tipo_invalido");
    }
}
