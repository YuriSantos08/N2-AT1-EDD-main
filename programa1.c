#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    long long timestamp;
    char id_sensor[50];
    char valor[50];
    
} Leitura;

int sensor_existente(const char *nome) {
    return (
        strcmp(nome, "temp") == 0 ||
        strcmp(nome, "umidade") == 0 ||
        strcmp(nome, "status") == 0 ||
        strcmp(nome, "ligado") == 0
    );
}

FILE *abrir_arquivo_sensor(const char *sensor) {
    char nome_arquivo[64];
    sprintf(nome_arquivo, "%s.txt", sensor);
    FILE *final = fopen(nome_arquivo, "w");
    if (final == NULL) {
        printf("Erro ao abrir arquivo para sensor: %s\n", sensor);
        exit(1);
    }
    return final;
}

int comparacao_timestamp_desc(const void *a, const void *b) {
    Leitura *l1 = (Leitura *)a;
    Leitura *l2 = (Leitura *)b;
    if (l1->timestamp < l2->timestamp) return 1;
    else if (l1->timestamp > l2->timestamp) return -1;
    else return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilize: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arq = fopen(argv[1], "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return 1;
    }

    Leitura temp_leituras[2000], umidade_leituras[2000];
    Leitura status_leituras[2000], ligado_leituras[2000];
    int count_temp = 0, count_umidade = 0, count_status = 0, count_ligado = 0;

    char linha[256];

    while (fgets(linha, sizeof(linha), arq)) {
        Leitura l;
        int campos = sscanf(linha, "%lld %s %s", &l.timestamp, l.id_sensor, l.valor);
        if (campos != 3) {
            printf("Linha mal formatada: %s", linha);
            continue;
        }

        if (!sensor_existente(l.id_sensor)) {
            printf("Sensor desconhecido: %s\n", l.id_sensor);
            continue;
        }

        if (strcmp(l.id_sensor, "temp") == 0) {
            if (count_temp < 2000)
                temp_leituras[count_temp++] = l;
            else
                printf("Limite do temp alcançado!\n");
        }
        else if (strcmp(l.id_sensor, "umidade") == 0) {
            if (count_umidade < 2000)
                umidade_leituras[count_umidade++] = l;
            else
                printf("Limite do umidade alcançado!\n");
        }
        else if (strcmp(l.id_sensor, "status") == 0) {
            if (count_status < 2000)
                status_leituras[count_status++] = l;
            else
                printf("Limite do status alcançado!\n");
        }
        else if (strcmp(l.id_sensor, "ligado") == 0) {
            if (count_ligado < 2000)
                ligado_leituras[count_ligado++] = l;
            else
                printf("Limite do ligado alcançado!\n");
        }
    }
    fclose(arq);

    qsort(temp_leituras, count_temp, sizeof(Leitura), comparacao_timestamp_desc);
    qsort(umidade_leituras, count_umidade, sizeof(Leitura), comparacao_timestamp_desc);
    qsort(status_leituras, count_status, sizeof(Leitura), comparacao_timestamp_desc);
    qsort(ligado_leituras, count_ligado, sizeof(Leitura), comparacao_timestamp_desc);

    FILE *arq_temp = abrir_arquivo_sensor("temp");
    for (int i = 0; i < count_temp; i++)
        fprintf(arq_temp, "%lld %s %s\n", temp_leituras[i].timestamp, temp_leituras[i].id_sensor, temp_leituras[i].valor);
    fclose(arq_temp);

    FILE *arq_umidade = abrir_arquivo_sensor("umidade");
    for (int i = 0; i < count_umidade; i++)
        fprintf(arq_umidade, "%lld %s %s\n", umidade_leituras[i].timestamp, umidade_leituras[i].id_sensor, umidade_leituras[i].valor);
    fclose(arq_umidade);

    FILE *arq_status = abrir_arquivo_sensor("status");
    for (int i = 0; i < count_status; i++)
        fprintf(arq_status, "%lld %s %s\n", status_leituras[i].timestamp, status_leituras[i].id_sensor, status_leituras[i].valor);
    fclose(arq_status);

    FILE *arq_ligado = abrir_arquivo_sensor("ligado");
    for (int i = 0; i < count_ligado; i++)
        fprintf(arq_ligado, "%lld %s %s\n", ligado_leituras[i].timestamp, ligado_leituras[i].id_sensor, ligado_leituras[i].valor);
    fclose(arq_ligado);

    printf("Leituras foram separadas e ordenadas em ordem decrescente com sucesso.\n");
    return 0;
}
