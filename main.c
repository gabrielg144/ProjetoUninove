#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para o nó da lista encadeada
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Estrutura para a lista encadeada
typedef struct LinkedList {
    Node *head;
} LinkedList;

// Funções para manipulação da lista encadeada
LinkedList *createLinkedList() {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    return list;
}

void appendLinkedList(LinkedList *list, int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void insertLinkedList(LinkedList *list, int index, int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        Node *temp = list->head;
        for (int i = 0; i < index - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        if (temp != NULL) {
            new_node->next = temp->next;
            temp->next = new_node;
        }
    }
}

void deleteLinkedList(LinkedList *list, int index) {
    if (list->head == NULL) return;
    Node *temp = list->head;
    if (index == 0) {
        list->head = temp->next;
        free(temp);
    } else {
        for (int i = 0; i < index - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        if (temp != NULL && temp->next != NULL) {
            Node *to_delete = temp->next;
            temp->next = to_delete->next;
            free(to_delete);
        }
    }
}

int getLinkedList(LinkedList *list, int index) {
    Node *temp = list->head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp->next;
    }
    if (temp != NULL) {
        return temp->data;
    }
    return -1; // Se o índice estiver fora do limite
}

// Função para medir o tempo de execução
double measure_time(void (*func)(void *), void *arg) {
    clock_t start, end;
    start = clock();
    func(arg);
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// Funções de manipulação para medir o tempo
void insert_array(void *arg) {
    int *params = (int *)arg;
    int *array = (int *)params[0];
    int index = params[1];
    int value = params[2];
    int size = params[3];
    for (int i = size; i > index; i--) {
        array[i] = array[i - 1];
    }
    array[index] = value;
}

void insert_linkedlist(void *arg) {
    int *params = (int *)arg;
    LinkedList *list = (LinkedList *)params[0];
    int index = params[1];
    int value = params[2];
    insertLinkedList(list, index, value);
}

void get_array(void *arg) {
    int *params = (int *)arg;
    int *array = (int *)params[0];
    int index = params[1];
    volatile int value = array[index];
}

void get_linkedlist(void *arg) {
    int *params = (int *)arg;
    LinkedList *list = (LinkedList *)params[0];
    int index = params[1];
    volatile int value = getLinkedList(list, index);
}

void delete_array(void *arg) {
    int *params = (int *)arg;
    int *array = (int *)params[0];
    int index = params[1];
    int size = params[2];
    for (int i = index; i < size - 1; i++) {
        array[i] = array[i + 1];
    }
}

void delete_linkedlist(void *arg) {
    int *params = (int *)arg;
    LinkedList *list = (LinkedList *)params[0];
    int index = params[1];
    deleteLinkedList(list, index);
}

// Teste de desempenho
void performance_test() {
    int size = 10000;
    int index = 5000;
    int value = -1;

    // Array
    int *array = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }

    // Lista encadeada
    LinkedList *linked_list = createLinkedList();
    for (int i = 0; i < size; i++) {
        appendLinkedList(linked_list, i);
    }

    // Preparando parâmetros
    int array_params[] = { (int)array, index, value, size };
    int linked_list_params[] = { (int)linked_list, index, value };

    // Medindo o tempo de inserção
    double array_insert_time = measure_time(insert_array, array_params);
    double linked_list_insert_time = measure_time(insert_linkedlist, linked_list_params);

    // Medindo o tempo de busca
    double array_get_time = measure_time(get_array, array_params);
    double linked_list_get_time = measure_time(get_linkedlist, linked_list_params);

    // Medindo o tempo de exclusão
    array_params[2] = size - 1; // tamanho - 1 para exclusão
    double array_delete_time = measure_time(delete_array, array_params);
    double linked_list_delete_time = measure_time(delete_linkedlist, linked_list_params);

    printf("Inserção no array: %f segundos\n", array_insert_time);
    printf("Inserção na lista encadeada: %f segundos\n", linked_list_insert_time);
    printf("Busca no array: %f segundos\n", array_get_time);
    printf("Busca na lista encadeada: %f segundos\n", linked_list_get_time);
    printf("Exclusão no array: %f segundos\n", array_delete_time);
    printf("Exclusão na lista encadeada: %f segundos\n", linked_list_delete_time);

    free(array);
    free(linked_list);
}

// Função principal
int main() {
    performance_test();
    return 0;
}
