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

void print_array(int n, int *array)
{
  puts("#PrintArray:");
  for (int i = 0; i < n; i++)
  {
    printf("%d\n", array[i]);
  }
  puts("\n");
}

void up(int child, int *T)
{
  int parent = (child - 1) / 2; // Pick up the parent

  if (parent >= 0) // Is a valid parent?
  {
    if (T[child] > T[parent]) // Swap parent and child if the child is bigger than the parent
    {
      int swap = T[child];
      T[child] = T[parent];
      T[parent] = swap;
      up(parent, T);
    }
  }
}

void down(int parent, int n, int *T)
{
  int child = parent * 2 + 1; // Pick up left child

  if (child < n) // Valid child
  {
    if (child < n - 1) // Check if there is a right child
    {
      if (T[child + 1] > T[child]) // What child is bigger? Left or right?
      {
        child = child + 1; // Pick up the bigger child
      }
    }
    if (T[parent] < T[child]) // Swap parent and child if the child is bigger than the parent
    {
      int swap = T[parent];
      T[parent] = T[child];
      T[child] = swap;
      down(child, n, T);
    }
  }
}

void construct(int n, int *T)
{
  for (int i = n / 2; i >= 0; i--)
  {
    down(i, n, T);
  }
}

void operate(int element)
{
  printf("#Operate: %d\n", element);
}

void add(int newElement, int *n, int *T)
{
  printf("#Add %d\n", newElement);
  T[*n] = newElement;
  *n = *n + 1;
  up(*n - 1, T);
}

void rem(int *n, int *T)
{
  puts("#Remove");
  if (*n > 0)
  {
    operate(T[0]);
    T[0] = T[*n - 1];
    *n = *n - 1;
    down(0, *n, T);
    return;
  }

  puts("Não há elementos para remover!");
}

void heap_sort(int n, int *T)
{
  construct(n, T);
  for (int i = n - 1; i > 0; i--)
  {
    int swap = T[0];
    T[0] = T[i];
    T[i] = swap;
    down(0, i, T);
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
    file = fopen("./array-files/outputs-heap-sort/array-size-1000-ordered.txt", "w");
    break;
  case HUNDRED_THOUSAND:
    file = fopen("./array-files/outputs-heap-sort/array-size-100000-ordered.txt", "w");
    break;

  case TEN_MILLION:
    file = fopen("./array-files/outputs-heap-sort/array-size-10000000-ordered.txt", "w");
    break;
  case ONE_BILLION:
    file = fopen("./array-files/outputs-heap-sort/array-size-1000000000-ordered.txt", "w");
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
  int size_array = ONE_THOUSAND;
  int *array = safe_malloc(size_array);
  fill_array_from_file(size_array, array);

  printf("# EXECUNTANDO O HEAPSORT PARA O ARRAY DE TAMANHO %d\n", ONE_THOUSAND);
  clock_t begin = clock();
  heap_sort(size_array, array);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(size_array, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-heap-sort/array-size-1000-ordered.txt\n");
  free(array);

  size_array = HUNDRED_THOUSAND;
  array = safe_malloc(size_array);
  fill_array_from_file(size_array, array);
  printf("# EXECUNTANDO O HEAPSORT PARA O ARRAY DE TAMANHO %d\n", size_array);
  begin = clock();
  heap_sort(size_array, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(size_array, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-heap-sort/array-size-100000-ordered.txt\n");
  free(array);

  size_array = TEN_MILLION;
  array = safe_malloc(size_array);
  fill_array_from_file(size_array, array);
  printf("# EXECUNTANDO O HEAPSORT PARA O ARRAY DE TAMANHO %d\n", size_array);
  begin = clock();
  heap_sort(size_array, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(size_array, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-heap-sort/array-size-10000000-ordered.txt\n");
  free(array);

  size_array = ONE_BILLION;
  array = safe_malloc(size_array);
  fill_array_from_file(size_array, array);
  printf("# EXECUNTANDO O HEAPSORT PARA O ARRAY DE TAMANHO %d\n", size_array);
  begin = clock();
  heap_sort(size_array, array);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" - TEMPO PARA ORDENAR: %f SEGUNDOS\n", time_spent);
  generate_ordered_array_file(size_array, array);
  puts(" - Foi gerado um arquivo .txt do vetor ordenado em > ./array-files/outputs-heap-sort/array-size-1000000000-ordered.txt\n");
  free(array);

  return 0;
}