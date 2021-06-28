#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ONE_THOUSAND 1000
#define HUNDRED_THOUSAND 100000
#define TEN_MILLION 10000000
#define ONE_BILLION 1000000000

int *safe_malloc(int array_size)
{
  int *array = malloc(array_size * sizeof(int));

  if (array == NULL)
  {
    puts("Não foi possível alocar espaço suficiente para o array");
    exit(-1);
  }
  return array;
}

void insertion_sort(int size_array, int *array)
{
  for (int i = 1; i < size_array; i++)
  {
    int elected = array[i];
    int j = i - 1;
    while (j >= 0 && array[j] > elected)
    {
      array[j + 1] = array[j];
      j--;
    }
    array[j + 1] = elected;
  }
}

void fill_array_from_file(int array_size, int *T)
{
  FILE *file;
  char element[15];
  int i = 0;

  switch (array_size)
  {
  case ONE_THOUSAND:
    file = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-1000.txt", "r");
    break;
  case HUNDRED_THOUSAND:
    file = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-100000.txt", "r");
    break;
  case TEN_MILLION:
    file = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-10000000.txt", "r");
    break;
  case ONE_BILLION:
    file = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-1000000000.txt", "r");
    break;
  default:
    puts("Não foi possível achar o arquivo do array!");
    exit(-1);
    break;
  }

  if (file == NULL)
  {
    printf("Erro na abertura dos arquivos dos arrays\n");
    exit(-1);
  }

  while (fgets(element, 15, file) != NULL)
  {
    T[i] = atoi(element);
    i++;
  }
  fclose(file);
}

void generate_ordered_array_file(int array_size, int *T)
{
  FILE *file;

  switch (array_size)
  {
  case ONE_THOUSAND:
    file = fopen("./array-files/outputs-insertion-sort/array-size-1000-ordered.txt", "w");
    break;
  case HUNDRED_THOUSAND:
    file = fopen("./array-files/outputs-insertion-sort/array-size-100000-ordered.txt", "w");
    break;

  case TEN_MILLION:
    file = fopen("./array-files/outputs-insertion-sort/array-size-10000000-ordered.txt", "w");
    break;
  case ONE_BILLION:
    file = fopen("./array-files/outputs-insertion-sort/array-size-1000000000-ordered.txt", "w");
    break;
  default:
    puts("Não foi possível criar o arquivo do array!");
    exit(-1);
    break;
  }

  if (file == NULL)
  {
    puts("Erro na criação do arquivo do array\n");
    exit(-1);
  }

  for (int i = 0; i < array_size; i++)
  {
    fprintf(file, "%d\n", T[i]);
  }
  fclose(file);
}

int main()
{

  int array_size = ONE_THOUSAND;
  int *array = safe_malloc(array_size);
  fill_array_from_file(array_size, array);

  printf("# EXECUTANDO O INSERTION SORT PARA O ARRAY DE TAMANHO %d\n", array_size);
  clock_t begin = clock();
  insertion_sort(array_size, array);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(array_size, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-insertion-sort/array-size-1000-ordered.txt\n");
  free(array);

  array_size = HUNDRED_THOUSAND;
  array = safe_malloc(array_size);
  fill_array_from_file(array_size, array);

  printf("# EXECUTANDO O INSERTION SORT PARA O ARRAY DE TAMANHO %d\n", array_size);
  begin = clock();
  insertion_sort(array_size, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(array_size, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-insertion-sort/array-size-100000-ordered.txt\n");
  free(array);

  array_size = TEN_MILLION;
  array = safe_malloc(array_size);
  fill_array_from_file(array_size, array);

  printf("# EXECUTANDO O INSERTION SORT PARA O ARRAY DE TAMANHO %d\n", array_size);
  begin = clock();
  insertion_sort(array_size, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(array_size, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-insertion-sort/array-size-10000000-ordered.txt\n");
  free(array);

  array_size = ONE_BILLION;
  array = safe_malloc(array_size);
  fill_array_from_file(array_size, array);

  printf("# EXECUTANDO O INSERTION SORT PARA O ARRAY DE TAMANHO %d\n", array_size);
  begin = clock();
  insertion_sort(array_size, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(array_size, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-insertion-sort/array-size-1000000000-ordered.txt\n");
  free(array);

  return 0;
}