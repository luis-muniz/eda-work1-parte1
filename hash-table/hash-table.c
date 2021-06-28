#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define HUNDRED_THOUSAND 100000 // To size hash table
#define EMPTY -1                // To empty array position

#define TWO_HUNDRED_THOUSAND 200000
#define FOUR_HUNDRED_THOUSAND 400000
#define SIX_HUNDRED_THOUSAND 600000
#define EIGHT_HUNDRED_THOUSAND 800000
#define ONE_MILLION 1000000

#define TOTAL_DIGITS_IN_KEY 5     // 00000 a 99999 = 5 digits (for fold method)
#define TOTAL_MAX_DIGIT_IN_KEY 10 // 2 billion has a maximum of 10 digits (for digit analyse method)

typedef struct
{
  int digit;
  float value;
} Digit_Devitation_Calc;

/* After doing the first analysis of the keys in the digit analysis method, this 
variable will be responsible for storing the variations of each digit of the 
key array, otherwise the time to analyze each insertion would be 10*n² to finish
all insertions, for n = 200000, 400000, 600000, 800000 e 1000000.
*/
Digit_Devitation_Calc *sort_array_minor_deviations;

/* AUXILIARY FUNCTIONS */
void print_array(int array_size, int *array)
{
  puts("#PrintArray:");
  for (int i = 0; i < array_size; i++)
  {
    printf("%d\n", array[i]);
  }
  puts("\n");
}

void init_array(int array_size, int *array)
{
  for (int i = 0; i < array_size; i++)
  {
    array[i] = EMPTY;
  }
}

int total_empty_positions_in_array(int array_size, int *array)
{
  int total_empty = 0;
  for (int i = 0; i < array_size; i++)
  {
    if (array[i] == EMPTY)
    {
      total_empty++;
    }
  }
  return total_empty;
}

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

void *safe_malloc_choose_type(int array_size, int type)
{
  void *array = malloc(array_size * type);

  if (array == NULL)
  {
    puts("Não foi possível alocar espaço suficiente para o array");
    exit(-1);
  }
  return array;
}

void fill_array_from_file(int array_size, int *array)
{
  FILE *file;
  char element[15];
  int i = 0;

  switch (array_size)
  {
  case TWO_HUNDRED_THOUSAND:
    file = fopen("./array-files/inputs-hash-table/array-size-200000.txt", "r");
    break;
  case FOUR_HUNDRED_THOUSAND:
    file = fopen("./array-files/inputs-hash-table/array-size-400000.txt", "r");
    break;
  case SIX_HUNDRED_THOUSAND:
    file = fopen("./array-files/inputs-hash-table/array-size-600000.txt", "r");
    break;
  case EIGHT_HUNDRED_THOUSAND:
    file = fopen("./array-files/inputs-hash-table/array-size-800000.txt", "r");
    break;
  case ONE_MILLION:
    file = fopen("./array-files/inputs-hash-table/array-size-1000000.txt", "r");
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
    array[i] = atoi(element);
    i++;
  }
  fclose(file);
}

int get_total_digits_higher_key(int array_size, int *array)
{
  return TOTAL_MAX_DIGIT_IN_KEY;
}

int cmpfunc(const void *a, const void *b)
{
  Digit_Devitation_Calc fa = *(const Digit_Devitation_Calc *)a;
  Digit_Devitation_Calc fb = *(const Digit_Devitation_Calc *)b;
  return (fa.value > fb.value) - (fa.value < fb.value);
}

void analyse_insertion(int *array_method, int hash, int key, int *total_success_insertion, int *same_key_hash, int *colision_total_hash)
{
  if (array_method[hash] == -1) // check colision
  {
    array_method[hash] = key;
    *total_success_insertion = *total_success_insertion + 1;
  }
  else if (array_method[hash] == key) // Simulate how to search. No colision
  {
    *same_key_hash = *same_key_hash + 1;
  }
  else // Colision detected
  {
    *colision_total_hash = *colision_total_hash + 1;
  }
}

void print_info_method(int total_empty_positions, int colision_total, int same_key, int total_insertion)
{
  printf("- Total Empty Positions: %d\n", total_empty_positions);
  printf("- Total colision       : %d\n", colision_total);
  printf("- Keys same            : %d\n", same_key);
  printf("- Total insertion      : %d\n\n", total_insertion);
}

int total_digits_number(int num)
{
  int digits_total = 0;
  if (num != 0)
  {
    while (num != 0)
    {
      num = num / 10;
      digits_total++;
    }
    return digits_total;
  }
  return 1;
}

/* HASH METHODS */
int method_hash_division(int key, int hash_table_size)
{
  return key % hash_table_size;
}

int method_hash_fold(int key)
{
  char key_shift_0[30] = {0};
  char key_in_string[30] = {0};

  sprintf(key_in_string, "%d", key);

  int total_digits_in_key = total_digits_number(key);

  if ((total_digits_in_key % TOTAL_DIGITS_IN_KEY != 0))
  {
    for (int i = 0; i < TOTAL_DIGITS_IN_KEY - (total_digits_in_key % TOTAL_DIGITS_IN_KEY); i++)
    {
      strcat(key_shift_0, "0");
    }
  }

  strcat(key_shift_0, key_in_string);

  for (int i = 0; i < (strlen(key_shift_0) / TOTAL_DIGITS_IN_KEY) - 1; i++)
  {
    int shift_left = ((i + 1) * TOTAL_DIGITS_IN_KEY) - 1;
    int shift_right = ((i + 1) * TOTAL_DIGITS_IN_KEY);

    for (int j = 0; j < TOTAL_DIGITS_IN_KEY; j++)
    {
      char digit_left[2], digit_right[2], digit_sum[2] = {0};

      memcpy(digit_left, &key_shift_0[shift_left - j], 1);
      memcpy(digit_right, &key_shift_0[shift_right + j], 1);

      sprintf(digit_sum, "%d", (atoi(digit_left) + atoi(digit_right)) % 10);

      key_shift_0[shift_right + j] = digit_sum[0];
    }
  }

  char hash[TOTAL_DIGITS_IN_KEY + 10] = {0};

  memcpy(hash, &key_shift_0[strlen(key_shift_0) - TOTAL_DIGITS_IN_KEY], TOTAL_DIGITS_IN_KEY);

  return atoi(hash);
}

int method_hash_multiplication(int key)
{
  char key_in_string[30] = {0};
  char hash_in_string[30] = {0};
  unsigned long hash = (unsigned long)key * (unsigned long)key;

  sprintf(key_in_string, "%d", (int)hash);

  if (strlen(key_in_string) < TOTAL_DIGITS_IN_KEY)
  {
    return (int)hash;
  }

  memcpy(hash_in_string, &key_in_string[(strlen(key_in_string) / 2) - 2], TOTAL_DIGITS_IN_KEY);

  return atoi(hash_in_string); // Returns a hash with the central 5 digits of key*key
}

int method_digit_analysis_method_1(int key, int total_digits, int *array_with_keys, int array_with_keys_size)
{
  int total_digits_higher_key = get_total_digits_higher_key(array_with_keys_size, array_with_keys);

  // Check if the analysis has not been done yet
  if (sort_array_minor_deviations == NULL)
  {
    float *array_minor_deviations = (float *)safe_malloc_choose_type(total_digits_higher_key + 1, sizeof(float));

    for (int i = 1; i <= total_digits_higher_key; i++) // maximum total a digit can have according to the key array
    {
      array_minor_deviations[i] = array_with_keys_size;
    }

    for (int i = 1; i <= total_digits_higher_key; i++) // 1° digit, 2° digit, ...
    {
      float array[10] = {0};
      for (int k = 0; k < array_with_keys_size; k++) // analyze if (i)° digit is equals to number (j) in the all keys (k)
      {
        int key_under_analysis = array_with_keys[k];
        int x = total_digits_number(key_under_analysis);
        if (x < total_digits_higher_key && i <= total_digits_higher_key - x) // 0's to the left, digit is equal to number 0
        {
          array[0]++;
          continue;
        }

        array[(key_under_analysis / ((int)pow(10, total_digits_higher_key - i))) % 10]++;
      }

      float deviation = 0.0f;
      for (int l = 0; l <= 9; l++)
      {
        float operation = array[l] - ((float)array_with_keys_size / 10.0f);
        deviation += operation < 0 ? -operation : operation;
      }

      array_minor_deviations[i] = deviation;
    }

    sort_array_minor_deviations = (Digit_Devitation_Calc *)safe_malloc_choose_type(total_digits_higher_key + 1, sizeof(Digit_Devitation_Calc));

    for (int i = 1; i <= total_digits_higher_key; i++)
    {
      sort_array_minor_deviations[i].digit = i;
      sort_array_minor_deviations[i].value = array_minor_deviations[i];
    }

    qsort(sort_array_minor_deviations, total_digits_higher_key + 1, sizeof(Digit_Devitation_Calc), cmpfunc);

    free(array_minor_deviations);
  }

  char key_in_string[30];
  sprintf(key_in_string, "%d", key);

  char string_shift_0[30] = "0";
  for (int i = 0; i < (total_digits_higher_key - strlen(key_in_string)); i++)
  {
    strcat(string_shift_0, "0");
  }
  strcat(string_shift_0, key_in_string);

  char hash[30] = "";
  for (int i = 1; i <= total_digits; i++)
  {
    char c[2] = {0};
    sprintf(c, "%c", string_shift_0[sort_array_minor_deviations[i].digit]);
    strcat(hash, c);
  }

  return atoi(hash);
}

int method_digit_analysis_method_2(int key, int total_digits, int *array_with_keys, int array_with_keys_size)
{
  int total_digits_higher_key = get_total_digits_higher_key(array_with_keys_size, array_with_keys);

  // Check if the analysis has not been done yet
  if (sort_array_minor_deviations == NULL)
  {
    float *array_minor_deviations = (float *)safe_malloc_choose_type(total_digits_higher_key + 1, sizeof(float));

    for (int i = 1; i <= total_digits_higher_key; i++) // maximum total a digit can have according to the key array
    {
      array_minor_deviations[i] = array_with_keys_size;
    }

    for (int i = 1; i <= total_digits_higher_key; i++) // 1° digit, 2° digit, ...
    {
      float array[10] = {0};
      for (int k = 0; k < array_with_keys_size; k++) // analyze if (i)° digit is equals to number (j) in the all keys (k)
      {
        int key_under_analysis = array_with_keys[k];
        int x = total_digits_number(key_under_analysis);
        if (x < total_digits_higher_key && i <= total_digits_higher_key - x) // 0's to the left, digit is equal to number 0
        {
          array[0]++;
          continue;
        }

        array[(key_under_analysis / ((int)pow(10, total_digits_higher_key - i))) % 10]++;
      }

      float deviation = 0.0f;
      for (int l = 0; l <= 9; l++)
      {
        float operation = (array[l] - ((float)array_with_keys_size / 10.0f)) * (array[l] - ((float)array_with_keys_size / 10.0f));
        deviation += operation;
      }

      array_minor_deviations[i] = deviation;
    }

    sort_array_minor_deviations = (Digit_Devitation_Calc *)safe_malloc_choose_type(total_digits_higher_key + 1, sizeof(Digit_Devitation_Calc));

    for (int i = 1; i <= total_digits_higher_key; i++)
    {
      sort_array_minor_deviations[i].digit = i;
      sort_array_minor_deviations[i].value = array_minor_deviations[i];
    }

    qsort(sort_array_minor_deviations, total_digits_higher_key + 1, sizeof(Digit_Devitation_Calc), cmpfunc);

    free(array_minor_deviations);
  }

  char key_in_string[30];
  sprintf(key_in_string, "%d", key);

  char string_shift_0[30] = "0";
  for (int i = 0; i < (total_digits_higher_key - strlen(key_in_string)); i++)
  {
    strcat(string_shift_0, "0");
  }
  strcat(string_shift_0, key_in_string);

  char hash[30] = "";
  for (int i = 1; i <= total_digits; i++)
  {
    char c[2] = {0};
    sprintf(c, "%c", string_shift_0[sort_array_minor_deviations[i].digit]);
    strcat(hash, c);
  }

  return atoi(hash);
}

/* EXECUTION FUNCTION */
int main()
{
  int size_array = HUNDRED_THOUSAND;

  /* Allocating memory and initializing hash tables for methods */
  int *array_division_method = safe_malloc(size_array);
  int *array_fold_method = safe_malloc(size_array);
  int *array_multiplication_method = safe_malloc(size_array);
  int *array_digit_analysis_method_1 = safe_malloc(size_array);
  int *array_digit_analysis_method_2 = safe_malloc(size_array);
  init_array(size_array, array_division_method);
  init_array(size_array, array_fold_method);
  init_array(size_array, array_multiplication_method);
  init_array(size_array, array_digit_analysis_method_1);
  init_array(size_array, array_digit_analysis_method_2);

  /* Allocating memory and filling arrays with keys randomly */
  int *array_size_200000 = safe_malloc(TWO_HUNDRED_THOUSAND);
  int *array_size_400000 = safe_malloc(FOUR_HUNDRED_THOUSAND);
  int *array_size_600000 = safe_malloc(SIX_HUNDRED_THOUSAND);
  int *array_size_800000 = safe_malloc(EIGHT_HUNDRED_THOUSAND);
  int *array_size_1000000 = safe_malloc(ONE_MILLION);
  fill_array_from_file(TWO_HUNDRED_THOUSAND, array_size_200000);
  fill_array_from_file(FOUR_HUNDRED_THOUSAND, array_size_400000);
  fill_array_from_file(SIX_HUNDRED_THOUSAND, array_size_600000);
  fill_array_from_file(EIGHT_HUNDRED_THOUSAND, array_size_800000);
  fill_array_from_file(ONE_MILLION, array_size_1000000);

  /* Information for method collisions */
  int colision_total_hash_division = 0, same_key_hash_division = 0, total_division = 0, colision_total_hash_fold = 0, same_key_hash_fold = 0, total_fold = 0, colision_total_hash_multiplication = 0, same_key_hash_multiplication = 0, total_multiplication = 0, colision_total_hash_digit_analysis_1 = 0, same_key_hash_digit_analysis_1 = 0, total_digit_analysis_1 = 0, colision_total_hash_digit_analysis_2 = 0, same_key_hash_digit_analysis_2 = 0, total_digit_analysis_2 = 0;

  /* ARRAY SIZE: 200.000 */
  printf("# [ARRAY SIZE 200.000] Making the inserts for the 5 methods:\n[%d%%]", 0);
  fflush(stdout);
  int print = TWO_HUNDRED_THOUSAND / 10;
  int percent = 10;
  for (int i = 0; i < TWO_HUNDRED_THOUSAND; i++)
  {

    if (i == print - 1)
    {
      printf("[%d%%]", percent);
      fflush(stdout);
      percent += 10;
      print += TWO_HUNDRED_THOUSAND / 10;
    }

    int key = array_size_200000[i];
    int hash_division = method_hash_division(key, HUNDRED_THOUSAND);
    // clock_t begin = clock();
    int hash_multiplication = method_hash_multiplication(key);
    int hash_fold = method_hash_fold(key);
    int hash_digit_analysis_1 = method_digit_analysis_method_1(key, TOTAL_DIGITS_IN_KEY, array_size_200000, TWO_HUNDRED_THOUSAND);
    int hash_digit_analysis_2 = method_digit_analysis_method_2(key, TOTAL_DIGITS_IN_KEY, array_size_200000, TWO_HUNDRED_THOUSAND);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("Duração do HeapSort: %f segundos\n", time_spent);
    // printf("key: %d\n", i);
    analyse_insertion(array_division_method, hash_division, key, &total_division, &same_key_hash_division, &colision_total_hash_division);
    analyse_insertion(array_fold_method, hash_fold, key, &total_fold, &same_key_hash_fold, &colision_total_hash_fold);
    analyse_insertion(array_multiplication_method, hash_multiplication, key, &total_multiplication, &same_key_hash_multiplication, &colision_total_hash_multiplication);
    analyse_insertion(array_digit_analysis_method_1, hash_digit_analysis_1, key, &total_digit_analysis_1, &same_key_hash_digit_analysis_1, &colision_total_hash_digit_analysis_1);
    analyse_insertion(array_digit_analysis_method_2, hash_digit_analysis_2, key, &total_digit_analysis_2, &same_key_hash_digit_analysis_2, &colision_total_hash_digit_analysis_2);
  }

  puts("\n# RESULTS: ");
  puts("# DIVISION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_division_method), colision_total_hash_division, same_key_hash_division, total_division);
  puts("# FOLD METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_fold_method), colision_total_hash_fold, same_key_hash_fold, total_fold);
  puts("# MULTIPLICATION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_multiplication_method), colision_total_hash_multiplication, same_key_hash_multiplication, total_multiplication);
  puts("# DIGIT ANALYSE 1 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_1), colision_total_hash_digit_analysis_1, same_key_hash_digit_analysis_1, total_digit_analysis_1);
  puts("# DIGIT ANALYSE 2 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_2, same_key_hash_digit_analysis_2, total_digit_analysis_2);

  // "zeroing" the variables for the next array
  free(sort_array_minor_deviations);
  init_array(size_array, array_division_method);
  init_array(size_array, array_fold_method);
  init_array(size_array, array_multiplication_method);
  init_array(size_array, array_digit_analysis_method_1);
  init_array(size_array, array_digit_analysis_method_2);
  colision_total_hash_division = same_key_hash_division = total_division = colision_total_hash_fold = same_key_hash_fold = total_fold = colision_total_hash_multiplication = same_key_hash_multiplication = total_multiplication = colision_total_hash_digit_analysis_1 = same_key_hash_digit_analysis_1 = total_digit_analysis_1 = colision_total_hash_digit_analysis_2 = same_key_hash_digit_analysis_2 = total_digit_analysis_2 = 0;
  sort_array_minor_deviations = NULL;

  /* ARRAY SIZE: 400.000 */
  printf("# [ARRAY SIZE 400.000] Making the inserts for the 5 methods:\n[%d%%]", 0);
  fflush(stdout);
  print = FOUR_HUNDRED_THOUSAND / 10;
  percent = 10;
  for (int i = 0; i < FOUR_HUNDRED_THOUSAND; i++)
  {

    if (i == print - 1)
    {
      printf("[%d%%]", percent);
      fflush(stdout);
      percent += 10;
      print += FOUR_HUNDRED_THOUSAND / 10;
    }

    int key = array_size_400000[i];
    int hash_division = method_hash_division(key, HUNDRED_THOUSAND);
    // clock_t begin = clock();
    int hash_multiplication = method_hash_multiplication(key);
    int hash_fold = method_hash_fold(key);
    int hash_digit_analysis_1 = method_digit_analysis_method_1(key, TOTAL_DIGITS_IN_KEY, array_size_400000, FOUR_HUNDRED_THOUSAND);
    int hash_digit_analysis_2 = method_digit_analysis_method_2(key, TOTAL_DIGITS_IN_KEY, array_size_400000, FOUR_HUNDRED_THOUSAND);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("Duração do HeapSort: %f segundos\n", time_spent);
    // printf("key: %d\n", i);
    analyse_insertion(array_division_method, hash_division, key, &total_division, &same_key_hash_division, &colision_total_hash_division);
    analyse_insertion(array_fold_method, hash_fold, key, &total_fold, &same_key_hash_fold, &colision_total_hash_fold);
    analyse_insertion(array_multiplication_method, hash_multiplication, key, &total_multiplication, &same_key_hash_multiplication, &colision_total_hash_multiplication);
    analyse_insertion(array_digit_analysis_method_1, hash_digit_analysis_1, key, &total_digit_analysis_1, &same_key_hash_digit_analysis_1, &colision_total_hash_digit_analysis_1);
    analyse_insertion(array_digit_analysis_method_2, hash_digit_analysis_2, key, &total_digit_analysis_2, &same_key_hash_digit_analysis_2, &colision_total_hash_digit_analysis_2);
  }

  puts("\n# RESULTS: ");
  puts("# DIVISION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_division_method), colision_total_hash_division, same_key_hash_division, total_division);
  puts("# FOLD METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_fold_method), colision_total_hash_fold, same_key_hash_fold, total_fold);
  puts("# MULTIPLICATION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_multiplication_method), colision_total_hash_multiplication, same_key_hash_multiplication, total_multiplication);
  puts("# DIGIT ANALYSE 1 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_1), colision_total_hash_digit_analysis_1, same_key_hash_digit_analysis_1, total_digit_analysis_1);
  puts("# DIGIT ANALYSE 2 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_2, same_key_hash_digit_analysis_2, total_digit_analysis_2);

  // "zeroing" the variables for the next array
  free(sort_array_minor_deviations);
  init_array(size_array, array_division_method);
  init_array(size_array, array_fold_method);
  init_array(size_array, array_multiplication_method);
  init_array(size_array, array_digit_analysis_method_1);
  init_array(size_array, array_digit_analysis_method_2);
  colision_total_hash_division = same_key_hash_division = total_division = colision_total_hash_fold = same_key_hash_fold = total_fold = colision_total_hash_multiplication = same_key_hash_multiplication = total_multiplication = colision_total_hash_digit_analysis_1 = same_key_hash_digit_analysis_1 = total_digit_analysis_1 = colision_total_hash_digit_analysis_2 = same_key_hash_digit_analysis_2 = total_digit_analysis_2 = 0;
  sort_array_minor_deviations = NULL;

  /* ARRAY SIZE: 600.000 */
  printf("# [ARRAY SIZE 600.000] Making the inserts for the 5 methods:\n[%d%%]", 0);
  fflush(stdout);
  print = SIX_HUNDRED_THOUSAND / 10;
  percent = 10;
  for (int i = 0; i < SIX_HUNDRED_THOUSAND; i++)
  {

    if (i == print - 1)
    {
      printf("[%d%%]", percent);
      fflush(stdout);
      percent += 10;
      print += SIX_HUNDRED_THOUSAND / 10;
    }

    int key = array_size_600000[i];
    int hash_division = method_hash_division(key, HUNDRED_THOUSAND);
    // clock_t begin = clock();
    int hash_multiplication = method_hash_multiplication(key);
    int hash_fold = method_hash_fold(key);
    int hash_digit_analysis_1 = method_digit_analysis_method_1(key, TOTAL_DIGITS_IN_KEY, array_size_600000, SIX_HUNDRED_THOUSAND);
    int hash_digit_analysis_2 = method_digit_analysis_method_2(key, TOTAL_DIGITS_IN_KEY, array_size_600000, SIX_HUNDRED_THOUSAND);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("Duração do HeapSort: %f segundos\n", time_spent);
    // printf("key: %d\n", i);
    analyse_insertion(array_division_method, hash_division, key, &total_division, &same_key_hash_division, &colision_total_hash_division);
    analyse_insertion(array_fold_method, hash_fold, key, &total_fold, &same_key_hash_fold, &colision_total_hash_fold);
    analyse_insertion(array_multiplication_method, hash_multiplication, key, &total_multiplication, &same_key_hash_multiplication, &colision_total_hash_multiplication);
    analyse_insertion(array_digit_analysis_method_1, hash_digit_analysis_1, key, &total_digit_analysis_1, &same_key_hash_digit_analysis_1, &colision_total_hash_digit_analysis_1);
    analyse_insertion(array_digit_analysis_method_2, hash_digit_analysis_2, key, &total_digit_analysis_2, &same_key_hash_digit_analysis_2, &colision_total_hash_digit_analysis_2);
  }

  puts("\n# RESULTS: ");
  puts("# DIVISION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_division_method), colision_total_hash_division, same_key_hash_division, total_division);
  puts("# FOLD METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_fold_method), colision_total_hash_fold, same_key_hash_fold, total_fold);
  puts("# MULTIPLICATION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_multiplication_method), colision_total_hash_multiplication, same_key_hash_multiplication, total_multiplication);
  puts("# DIGIT ANALYSE 1 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_1, same_key_hash_digit_analysis_1, total_digit_analysis_1);
  puts("# DIGIT ANALYSE 2 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_2, same_key_hash_digit_analysis_2, total_digit_analysis_2);

  // "zeroing" the variables for the next array
  free(sort_array_minor_deviations);
  init_array(size_array, array_division_method);
  init_array(size_array, array_fold_method);
  init_array(size_array, array_multiplication_method);
  init_array(size_array, array_digit_analysis_method_1);
  init_array(size_array, array_digit_analysis_method_2);
  colision_total_hash_division = same_key_hash_division = total_division = colision_total_hash_fold = same_key_hash_fold = total_fold = colision_total_hash_multiplication = same_key_hash_multiplication = total_multiplication = colision_total_hash_digit_analysis_1 = same_key_hash_digit_analysis_1 = total_digit_analysis_1 = colision_total_hash_digit_analysis_2 = same_key_hash_digit_analysis_2 = total_digit_analysis_2 = 0;
  sort_array_minor_deviations = NULL;

  /* ARRAY SIZE: 800.000 */
  printf("# [ARRAY SIZE 800.000] Making the inserts for the 5 methods:\n[%d%%]", 0);
  fflush(stdout);
  print = EIGHT_HUNDRED_THOUSAND / 10;
  percent = 10;
  for (int i = 0; i < EIGHT_HUNDRED_THOUSAND; i++)
  {

    if (i == print - 1)
    {
      printf("[%d%%]", percent);
      fflush(stdout);
      percent += 10;
      print += EIGHT_HUNDRED_THOUSAND / 10;
    }

    int key = array_size_800000[i];
    int hash_division = method_hash_division(key, HUNDRED_THOUSAND);
    // clock_t begin = clock();
    int hash_multiplication = method_hash_multiplication(key);
    int hash_fold = method_hash_fold(key);
    int hash_digit_analysis_1 = method_digit_analysis_method_1(key, TOTAL_DIGITS_IN_KEY, array_size_800000, EIGHT_HUNDRED_THOUSAND);
    int hash_digit_analysis_2 = method_digit_analysis_method_2(key, TOTAL_DIGITS_IN_KEY, array_size_800000, EIGHT_HUNDRED_THOUSAND);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("Duração do HeapSort: %f segundos\n", time_spent);
    // printf("key: %d\n", i);
    analyse_insertion(array_division_method, hash_division, key, &total_division, &same_key_hash_division, &colision_total_hash_division);
    analyse_insertion(array_fold_method, hash_fold, key, &total_fold, &same_key_hash_fold, &colision_total_hash_fold);
    analyse_insertion(array_multiplication_method, hash_multiplication, key, &total_multiplication, &same_key_hash_multiplication, &colision_total_hash_multiplication);
    analyse_insertion(array_digit_analysis_method_1, hash_digit_analysis_1, key, &total_digit_analysis_1, &same_key_hash_digit_analysis_1, &colision_total_hash_digit_analysis_1);
    analyse_insertion(array_digit_analysis_method_2, hash_digit_analysis_2, key, &total_digit_analysis_2, &same_key_hash_digit_analysis_2, &colision_total_hash_digit_analysis_2);
  }

  puts("\n# RESULTS: ");
  puts("# DIVISION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_division_method), colision_total_hash_division, same_key_hash_division, total_division);
  puts("# FOLD METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_fold_method), colision_total_hash_fold, same_key_hash_fold, total_fold);
  puts("# MULTIPLICATION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_multiplication_method), colision_total_hash_multiplication, same_key_hash_multiplication, total_multiplication);
  puts("# DIGIT ANALYSE 1 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_1), colision_total_hash_digit_analysis_1, same_key_hash_digit_analysis_1, total_digit_analysis_1);
  puts("# DIGIT ANALYSE 2 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_2, same_key_hash_digit_analysis_2, total_digit_analysis_2);

  // "zeroing" the variables for the next array
  free(sort_array_minor_deviations);
  init_array(size_array, array_division_method);
  init_array(size_array, array_fold_method);
  init_array(size_array, array_multiplication_method);
  init_array(size_array, array_digit_analysis_method_1);
  init_array(size_array, array_digit_analysis_method_2);

  colision_total_hash_division = same_key_hash_division = total_division = colision_total_hash_fold = same_key_hash_fold = total_fold = colision_total_hash_multiplication = same_key_hash_multiplication = total_multiplication = colision_total_hash_digit_analysis_1 = same_key_hash_digit_analysis_1 = total_digit_analysis_1 = colision_total_hash_digit_analysis_2 = same_key_hash_digit_analysis_2 = total_digit_analysis_2 = 0;
  sort_array_minor_deviations = NULL;

  /* ARRAY SIZE: 1.000.000 */
  printf("# [ARRAY SIZE 1.000.000] Making the inserts for the 5 methods:\n[%d%%]", 0);
  fflush(stdout);
  print = ONE_MILLION / 10;
  percent = 10;
  for (int i = 0; i < ONE_MILLION; i++)
  {

    if (i == print - 1)
    {
      printf("[%d%%]", percent);
      fflush(stdout);
      percent += 10;
      print += ONE_MILLION / 10;
    }

    int key = array_size_1000000[i];
    int hash_division = method_hash_division(key, HUNDRED_THOUSAND);
    // clock_t begin = clock();
    int hash_multiplication = method_hash_multiplication(key);
    int hash_fold = method_hash_fold(key);
    int hash_digit_analysis_1 = method_digit_analysis_method_1(key, TOTAL_DIGITS_IN_KEY, array_size_1000000, ONE_MILLION);
    int hash_digit_analysis_2 = method_digit_analysis_method_2(key, TOTAL_DIGITS_IN_KEY, array_size_1000000, ONE_MILLION);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("Duração do HeapSort: %f segundos\n", time_spent);
    // printf("key: %d\n", i);
    analyse_insertion(array_division_method, hash_division, key, &total_division, &same_key_hash_division, &colision_total_hash_division);
    analyse_insertion(array_fold_method, hash_fold, key, &total_fold, &same_key_hash_fold, &colision_total_hash_fold);
    analyse_insertion(array_multiplication_method, hash_multiplication, key, &total_multiplication, &same_key_hash_multiplication, &colision_total_hash_multiplication);
    analyse_insertion(array_digit_analysis_method_1, hash_digit_analysis_1, key, &total_digit_analysis_1, &same_key_hash_digit_analysis_1, &colision_total_hash_digit_analysis_1);
    analyse_insertion(array_digit_analysis_method_2, hash_digit_analysis_2, key, &total_digit_analysis_2, &same_key_hash_digit_analysis_2, &colision_total_hash_digit_analysis_2);
  }
  free(sort_array_minor_deviations);

  puts("\n# RESULTS: ");
  puts("# DIVISION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_division_method), colision_total_hash_division, same_key_hash_division, total_division);
  puts("# FOLD METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_fold_method), colision_total_hash_fold, same_key_hash_fold, total_fold);
  puts("# MULTIPLICATION METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_multiplication_method), colision_total_hash_multiplication, same_key_hash_multiplication, total_multiplication);
  puts("# DIGIT ANALYSE 1 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_1), colision_total_hash_digit_analysis_1, same_key_hash_digit_analysis_1, total_digit_analysis_1);
  puts("# DIGIT ANALYSE 2 METHOD:");
  print_info_method(total_empty_positions_in_array(HUNDRED_THOUSAND, array_digit_analysis_method_2), colision_total_hash_digit_analysis_2, same_key_hash_digit_analysis_2, total_digit_analysis_2);

  // Dealocating memories from arrays
  free(array_division_method);
  free(array_fold_method);
  free(array_multiplication_method);
  free(array_digit_analysis_method_1);
  free(array_digit_analysis_method_2);
  free(array_size_200000);
  free(array_size_400000);
  free(array_size_600000);
  free(array_size_800000);
  free(array_size_1000000);

  return 0;
}