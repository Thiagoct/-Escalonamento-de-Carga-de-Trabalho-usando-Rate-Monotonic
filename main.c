#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_TAREFAS 3

// Estrutura para armazenar os dados das tarefas
typedef struct {
    char id[10];
    int period;
    int execution_time;
} Task;

// Função para carregar o conteúdo do arquivo JSON
char *load_json_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(file_size + 1);
    fread(json_string, 1, file_size, file);
    fclose(file);
    json_string[file_size] = '\0'; // Adiciona o caractere nulo ao final da string

    return json_string;
}

// Função de comparação para ordenar as tarefas pelo período (menor período = maior prioridade)
int compare_tasks(const void *a, const void *b) {
    Task *taskA = (Task *)a;
    Task *taskB = (Task *)b;
    return taskA->period - taskB->period;
}

// Função para calcular a utilização do processador
double calcular_utilizacao(Task tasks[], int n) {
    double utilizacao = 0.0;
    for (int i = 0; i < n; i++) {
        utilizacao += (double)tasks[i].execution_time / tasks[i].period;
    }
    return utilizacao;
}

// Função para verificar a condição de escalonamento Rate Monotonic
int verificar_escalonamento(Task tasks[], int n) {
    // Ordenar as tarefas pelo período
    qsort(tasks, n, sizeof(Task), compare_tasks);

    // Calcular a utilização total do processador
    double utilizacao = calcular_utilizacao(tasks, n);
    double limite = n * (pow(2.0, 1.0/n) - 1.0); // Limite de utilizacao para n tarefas

    printf("Utilização total: %.2f\n", utilizacao);
    printf("Limite de escalonamento: %.2f\n", limite);

    return utilizacao <= limite;
}

// Função para gerar o arquivo output.json com a viabilidade e sugestão de escalonamento
void gerar_output_json(Task tasks[], int n, int viavel) {
    struct json_object *json_output = json_object_new_object();
    struct json_object *json_schedulability = json_object_new_string(viavel ? "viable" : "not viable");
    struct json_object *json_suggested_schedule = json_object_new_array();

    for (int i = 0; i < n; i++) {
        struct json_object *json_task = json_object_new_object();
        json_object_object_add(json_task, "id", json_object_new_string(tasks[i].id));
        json_object_object_add(json_task, "priority", json_object_new_int(i + 1)); // Prioridade: 1, 2, 3, ...
        json_object_array_add(json_suggested_schedule, json_task);
    }

    json_object_object_add(json_output, "schedulability", json_schedulability);
    json_object_object_add(json_output, "suggested_schedule", json_suggested_schedule);

    // Escreve o JSON formatado para o arquivo output.json
    FILE *file = fopen("output.json", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo output.json para escrita.\n");
        json_object_put(json_output);
        return;
    }
    fprintf(file, "%s\n", json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY));
    fclose(file);

    // Limpeza
    json_object_put(json_output);
}

int main() {
    // Carregar conteúdo do arquivo JSON
    char *json_string = load_json_file("tarefas.json");
    if (json_string == NULL) {
        return 1;
    }

    // Parsing do JSON
    struct json_object *parsed_json;
    struct json_object *tasks_array;
    struct json_object *task;
    struct json_object *id, *period, *execution_time;

    // Criar array de tarefas
    Task tasks[N_TAREFAS]; // Ajuste conforme necessário

    // Parseia a string JSON
    parsed_json = json_tokener_parse(json_string);

    // Obtém o array "tasks"
    json_object_object_get_ex(parsed_json, "tasks", &tasks_array);
    
    // Loop para processar cada tarefa no array JSON
    for (size_t i = 0; i < json_object_array_length(tasks_array); i++) {
        task = json_object_array_get_idx(tasks_array, i);

        // Extrai os campos id, period e execution_time de cada tarefa
        json_object_object_get_ex(task, "id", &id);
        json_object_object_get_ex(task, "period", &period);
        json_object_object_get_ex(task, "execution_time", &execution_time);

        // Preenche a estrutura com os dados da tarefa
        strcpy(tasks[i].id, json_object_get_string(id));
        tasks[i].period = json_object_get_int(period);
        tasks[i].execution_time = json_object_get_int(execution_time);
    }

    // Exibe as tarefas lidas do JSON
    printf("Tarefas lidas do arquivo JSON:\n");
    for (size_t i = 0; i < N_TAREFAS; i++) {
        printf("Tarefa %s - Periodo: %d, Tempo de execução: %d\n", 
               tasks[i].id, tasks[i].period, tasks[i].execution_time);
    }

    // Verifica se as tarefas podem ser escalonadas
    int viavel = verificar_escalonamento(tasks, N_TAREFAS);
    if (viavel) {
        printf("As tarefas podem ser escalonadas usando Rate Monotonic.\n");
    } else {
        printf("As tarefas NÃO podem ser escalonadas usando Rate Monotonic.\n");
    }

    // Gera o arquivo output.json
    gerar_output_json(tasks, N_TAREFAS, viavel);

    // Limpeza
    json_object_put(parsed_json);
    free(json_string);

    return 0;
}
