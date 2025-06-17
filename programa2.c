#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct   {

    long long timestamp;
    char sensor[50];
    char valor[50];
} Leitura;

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    time_t ts = mktime(&t);
    if (ts == -1) {
        printf("Data invalida.\n");
        exit(1);
    }
    return ts;
}

int busca_binaria_proxima_decrescente(Leitura *leituras, int n, long long ts) {
    int inicio = 0;
    int fim = n - 1;
    int meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (leituras[meio].timestamp == ts) return meio;
        else if (leituras[meio].timestamp > ts) inicio = meio + 1;
        else fim = meio - 1;
    }

    if (inicio >= n) return fim;
    if (fim < 0) return inicio;

    long long diff_inicio = llabs(leituras[inicio].timestamp - ts);
    long long diff_fim = llabs(leituras[fim].timestamp - ts);

    return (diff_inicio < diff_fim) ? inicio : fim;
}

int main(int argc, char *argv[]) {
    if (argc != 9) {
        printf("Uso: %s <arquivo> <sensor> <dia> <mes> <ano> <hora> <min> <seg>\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];
    char sensor[50];
    strcpy(sensor, argv[2]);

    time_t ts_consulta = converter_para_timestamp(
        atoi(argv[3]), atoi(argv[4]), atoi(argv[5]),
        atoi(argv[6]), atoi(argv[7]), atoi(argv[8])
    );

    FILE *arq = fopen(nome_arquivo, "r");
    if (!arq) {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        return 1;
    }

    Leitura *leituras = NULL;
    size_t capacidade = 1000, n = 0;
    leituras = malloc(sizeof(Leitura) * capacidade);
    if (!leituras) {
        printf("Erro de memoria\n");
        fclose(arq);
        return 1;
    }

    Leitura l;
    while (fscanf(arq, "%lld %s %s", &l.timestamp, l.sensor, l.valor) == 3) {
        if (strcmp(l.sensor, sensor) == 0) {
            if (n >= capacidade) {
                capacidade *= 2;
                leituras = realloc(leituras, sizeof(Leitura) * capacidade);
                if (!leituras) {
                    printf("Erro de memoria\n");
                    fclose(arq);
                    return 1;
                }
            }
            leituras[n++] = l;
        }
    }
    fclose(arq);

    if (n == 0) {
        printf("Nenhuma leitura encontrada para o sensor %s informado\n", sensor);
        free(leituras);
        return 1;
    }

    int idx = busca_binaria_proxima_decrescente(leituras, n, ts_consulta);
    printf("%lld %s %s\n", leituras[idx].timestamp, leituras[idx].sensor, leituras[idx].valor);

    free(leituras);
    return 0;
}
