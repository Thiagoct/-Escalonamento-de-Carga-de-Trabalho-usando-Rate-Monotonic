# Escalonamento de Carga de Trabalho usando Rate Monotonic
As especificações desse projeto foram oriundas da dicisplina Sistemas em tempo real.
## Descriçao projeto
Este projeto implementa um verificador de escalonamento de tarefas usando o algoritmo Rate Monotonic (RM). Ele lê um arquivo JSON contendo uma lista de tarefas, verifica se essas tarefas podem ser escalonadas de acordo com o algoritmo RM, e __gera um arquivo de saída JSON__ contendo a viabilidade e a sugestão de prioridade das tarefas.
O algoritmo Rate Monotonic é um método de escalonamento de tarefas em sistemas de tempo real, onde as tarefas com o menor período têm maior prioridade. Este programa realiza as seguintes etapas:

* Lê um arquivo JSON contendo as informações das tarefas (ID, período e tempo de execução).
* Calcula se as tarefas podem ser escalonadas de acordo com o algoritmo RM.
* Gera um arquivo output.json que informa se o escalonamento é viável e sugere a ordem de execução das tarefas.
### Instalação
Pré-requisitos
* Compilador C (ex: gcc)
* Biblioteca json-c para manipulação de arquivos JSON. No Linux (debian), você pode instalar com o seguinte comando:
sudo apt-get install libjson-c-dev

## Como usar
Em um terminal, entre no repositorio do projeto e utilize o comando make. Isso irá compilar e chamar o executavel.
## Arquitetura do software
__1. Estrutura Task__


A struct Task armazena as informações de cada tarefa:

id: Identificador da tarefa (string).


period: Período da tarefa (inteiro).


execution_time: Tempo de execução da tarefa (inteiro).

__2. Função load_json_file__


Essa função lê o conteúdo de um arquivo JSON e retorna uma string com o conteúdo. Ela:

Abre o arquivo.


Determina o tamanho do arquivo.


Aloca memória para armazenar o conteúdo.


Retorna a string com o conteúdo do arquivo.


__3. Função compare_tasks__

Essa função é usada para ordenar as tarefas pelo período usando a função qsort. No algoritmo Rate Monotonic, tarefas com menor período têm maior prioridade. Essa função simplesmente compara o campo period de duas tarefas.

__4. Função calcular_utilizacao__


Essa função calcula a utilização do processador somando a razão entre o tempo de execução e o período de cada tarefa:

 U = Sum(tempo de execução / período)
 
__5. Função verificar_escalonamento__


Essa função faz três coisas principais:

Ordena as tarefas pelo período (maior prioridade para menor período).


Calcula a utilização total do processador.


Compara (U <= L) a utilização com o limite teórico de utilização para n tarefas, que é dado por:

L = n * (2^(1/n)-1)

Onde __n__ é o número de tarefas.

__6. Função gerar_output_json__

Essa função gera um arquivo output.json com o resultado da viabilidade e a sugestão de escalonamento.

Cria um objeto JSON com a viabilidade ("viable" ou "not viable").


Adiciona as tarefas ao objeto JSON, incluindo a prioridade (que é baseada na ordem dos períodos).


Escreve o JSON em um arquivo output.json

### Main 
A função main realiza os seguintes passos:

1. Carrega o arquivo JSON: Lê as tarefas do arquivo tarefas.json usando a função load_json_file.


2. Parseia o JSON: Converte o conteúdo JSON em um objeto manipulável e extrai as informações das tarefas.


3. Exibe as tarefas: Mostra no console as tarefas carregadas do arquivo JSON.


4. Verifica o escalonamento: Chama a função verificar_escalonamento para determinar se as tarefas podem ser escalonadas usando RM.


5. Gera o arquivo de saída: Cria o arquivo output.json com o resultado da verificação de escalonamento e a sugestão de prioridades.


6. Limpeza: Libera a memória alocada e finaliza o programa.