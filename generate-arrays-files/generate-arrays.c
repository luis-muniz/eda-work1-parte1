#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TWO_HUNDRED_THOUSAND 200000
#define FOUR_HUNDRED_THOUSAND 400000
#define SIX_HUNDRED_THOUSAND 600000
#define EIGHT_HUNDRED_THOUSAND 800000
#define ONE_MILLION 1000000

#define ONE_THOUSAND 1000
#define HUNDRED_THOUSAND 100000
#define TEN_MILLION 10000000
#define ONE_BILLION 1000000000

void write_in_array_file(int array_file_size, FILE *array_file)
{
   srand(time(NULL));
   for (int i = 0; i < array_file_size; i++)
   {
      fprintf(array_file, "%d\n", rand() % 2000000001);
   }
}

int main(void)
{
   FILE *random_array_file_hash_table_1, *random_array_file_hash_table_2, *random_array_file_hash_table_3, *random_array_file_hash_table_4, *random_array_file_hash_table_5;

   random_array_file_hash_table_1 = fopen("./array-files/inputs-hash-table/array-size-200000.txt", "w");
   random_array_file_hash_table_2 = fopen("./array-files/inputs-hash-table/array-size-400000.txt", "w");
   random_array_file_hash_table_3 = fopen("./array-files/inputs-hash-table/array-size-600000.txt", "w");
   random_array_file_hash_table_4 = fopen("./array-files/inputs-hash-table/array-size-800000.txt", "w");
   random_array_file_hash_table_5 = fopen("./array-files/inputs-hash-table/array-size-1000000.txt", "w");

   puts("# Criando os arquivos de arrays para a tabela hash...\n");
   write_in_array_file(TWO_HUNDRED_THOUSAND, random_array_file_hash_table_1);
   write_in_array_file(FOUR_HUNDRED_THOUSAND, random_array_file_hash_table_2);
   write_in_array_file(SIX_HUNDRED_THOUSAND, random_array_file_hash_table_3);
   write_in_array_file(EIGHT_HUNDRED_THOUSAND, random_array_file_hash_table_4);
   write_in_array_file(ONE_MILLION, random_array_file_hash_table_5);

   fclose(random_array_file_hash_table_1);
   fclose(random_array_file_hash_table_2);
   fclose(random_array_file_hash_table_3);
   fclose(random_array_file_hash_table_4);
   fclose(random_array_file_hash_table_5);

   FILE *file1, *file2, *file3, *file4;
   srand(time(NULL));

   file1 = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-1000.txt", "w");
   file2 = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-100000.txt", "w");
   file3 = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-10000000.txt", "w");
   file4 = fopen("./array-files/inputs-insertion-and-heap-sort/array-size-1000000000.txt", "w");

   puts("# Criando os arquivos de arrays para o heapsorte insertionsort hash...");
   if (file1 == NULL || file2 == NULL || file3 == NULL || file4 == NULL)
   {
      printf("Erro na criação dos arquivos dos arrays\n");
      return -1;
   }

   srand(time(NULL));
   for (int i = 0; i < ONE_THOUSAND; i++)
   {
      fprintf(file1, "%d\n", rand() % 2000000001);
   }
   srand(time(NULL));
   for (int i = 0; i < HUNDRED_THOUSAND; i++)
   {
      fprintf(file2, "%d\n", rand() % 2000000001);
   }
   srand(time(NULL));
   for (int i = 0; i < TEN_MILLION; i++)
   {
      fprintf(file3, "%d\n", rand() % 2000000001);
   }
   srand(time(NULL));
   for (int i = 0; i < ONE_BILLION; i++)
   {
      fprintf(file4, "%d\n", rand() % 2000000001);
   }

   fclose(file1);
   fclose(file2);
   fclose(file3);
   fclose(file4);

   return 0;
}