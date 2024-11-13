#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define ON_DEBUG(code) code
#else 
#define ON_DEBUG(code)
#endif



ON_DEBUG(// заполняет нулями size байт.
static void fill_zeros(elem_t* first, int size)
{
    for (int i = 0; i < size; i++)
        first[i] = 0;
    
})


// Если массив data переполнен.
static int Increase_Realloc(LIST* lst)
{
    int last_size = lst->size_lst;
    lst->size_lst = last_size * 2;
    
    lst->data = (elem_t *) realloc((void *) lst->data, lst->size_lst * sizeof(lst->data[0]));
    lst->next = (elem_t *) realloc((void *) lst->next, lst->size_lst * sizeof(lst->next[0]));
    lst->prev = (elem_t *) realloc((void *) lst->prev, lst->size_lst * sizeof(lst->prev[0]));

    ON_DEBUG(
    fill_zeros(lst->data + last_size, lst->size_lst - last_size);
    fill_zeros(lst->next + last_size, lst->size_lst - last_size);
    fill_zeros(lst->prev + last_size, lst->size_lst - last_size);)

    for (int i = last_size; i < lst->size_lst; i++)  // Заполняем массив next последовательностью свободных ячеек.
        lst->next[i] = (i + 1) % lst->size_lst;
    lst->free_ind = last_size;

    return 0;
}


// Создаёт массив элементов длинны size_lst.
int Init_List(LIST* lst, int size_lst)
{
    assert (lst != NULL);

    lst->amout_elems = 0;
    lst->size_lst = size_lst;
    lst->free_ind = 1;
    lst->data = (elem_t *) calloc(size_lst, sizeof(elem_t)); // Массив data заполнен нулями изначально
    lst->next = (elem_t *) calloc(size_lst, sizeof(elem_t));
    lst->prev = (elem_t *) calloc(size_lst, sizeof(elem_t));

    for (int i = 1; i < size_lst; i++)  // Заполняем массив next последовательностью свободных ячеек.
        lst->next[i] = (i + 1) % size_lst;
    

    return 0;
}


static void printf_array(elem_t* nums, int size)
{
    for (int i = 0; i < size ; i++)
        printf("%d ", nums[i]);
    
    printf("\n");

}

static void List_Dump(LIST* lst)
{
    printf("DATA:\n");
    printf_array(lst->data, lst->size_lst);

    printf("NEXT:\n");
    printf_array(lst->next, lst->size_lst);

    printf("PREV:\n");
    printf_array(lst->prev, lst->size_lst);
    printf("\n");

}


// Уничтожает массив элементов.
int Destroy_List(LIST lst)
{
    free(lst.data);
    free(lst.next);
    free(lst.prev);

    return 0;
}



// Добавить элемент(value) на следующее место после указанного(anch_ind).
int Add_Elem(LIST* lst, int anch_ind, int value)
{
    assert (lst != NULL);
    if (lst->free_ind == 0) Increase_Realloc(lst); // массив может быть переполнен
    
    lst->amout_elems += 1;

    int new_elem_ind = lst->free_ind;          // индекс нового элемента.
    lst->free_ind = lst->next[lst->free_ind];  // массив next хранит также информацию о следующем свободном элементе, сохраняем какой свободный элемент следующий.
                                               // устанавливаем следующий свободный элемент.    
    
    lst->data[new_elem_ind] = value;           // кладём новый элемент в свободную ячейку массива data.

    // Обновление индексов
    int next_anch_ind = lst->next[anch_ind];  // Индекс элемента который шёл после anchor до вставки (назовём этот элемент next_anch_ind).
    lst->prev[next_anch_ind] = new_elem_ind;  // устанавливаем связь    new_elem<-next_anch_ind
    lst->prev[new_elem_ind] = anch_ind;        // устанавливаем связь   anch_ind<-new_elem    


    lst->next[anch_ind] = new_elem_ind;        // устанавливаем связь anch_ind->new_elem_ind.
    lst->next[new_elem_ind] = next_anch_ind;   // устанавливаем связь new_elem_ind->next_anch_ind.
    
    // DBG
    // printf("next_anch_ind=%d. new_elem_ind=%d.\n", next_anch_ind, new_elem_ind);
    List_Dump(lst);
    return 0;
} 




// Удалить элемент стоящий по индексу ind.
int Del_Elem(LIST* lst, int anch_ind)
{
    assert (lst != NULL);

    int ind_after_anch = lst->next[anch_ind];
    int ind_before_anch = lst->prev[anch_ind];


    // мы удалили элемент по индексу anch_ind, значит освободилась память, обновим lst->free.
    lst->next[anch_ind] = lst->free_ind;  // следующий после нового элемента индексу anch_ind.
    lst->free_ind = anch_ind;            // новый свободный элемент по индексу ind.


    // пусть имеется цепочка   prev<->anchor<->next.
    lst->next[ind_before_anch] = ind_after_anch; // Заменяем связь prev->anchor на связь  prev->next. 
    lst->prev[ind_after_anch] = ind_before_anch; // Заменяем связь anchor<-next, на prev<-nex.

    return 0;
} 



void Lst_Dump (FILE* dump_file, LIST* lst)
{
    assert (dump_file != NULL);
    assert (lst != NULL);

    #define FREE_COLOR  "\"lightgreen\""
    #define BUSY_COLOR  "\"coral\""
    #define TITLE_COLOR "\"lightblue\""

    fprintf (dump_file, "digraph G\n"
                        "{\n"
                        "splines=ortho;\n"
                        "nodesep=0.5;\n"
                        "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    fprintf (dump_file, "free[label = \"free_ind = %d\", style=\"rounded,filled\", fillcolor = " TITLE_COLOR "]\n", lst->free_ind);
    fprintf (dump_file, "0[label = \"{ <i>0|<d>data = %d|<n>next = %d|<p>prev = %d }\", fillcolor =" TITLE_COLOR "];\n", lst->data[0], lst->next[0], lst->prev[0]);
    for (int i = 1; i < lst->size_lst; ++i)
        fprintf (dump_file, "%d[label = \"{ <i>%d|<d>data = %d|<n>next = %d|<p>prev = %d }\", fillcolor = " BUSY_COLOR "];\n", i, i, lst->data[i], lst->next[i], lst->prev[i]);
    fprintf (dump_file, "\n");

    // выстраиваем ячейки в строчку
    fprintf (dump_file, "{ rank = same; ");
    for (int i = 0 ; i < lst->size_lst; i++)
        fprintf (dump_file, "%d; ", i);
    fprintf (dump_file, "}\n");

    // соединяем невидимыми линиями
    for (int i = 0; i < lst->size_lst; ++i)
        fprintf (dump_file, "%d->%d [weight = 5000, style=invis, shape=inv]; \n", i, i + 1);
    fprintf (dump_file, "\n");

    // соединяем стрелками next
    int cur_ind = 0;
    do
    {
        fprintf (dump_file, "%d->%d [weight = 0, color = blueviolet, dir=\"both\"];\n", cur_ind, lst->next[cur_ind]);
        cur_ind = lst->next[cur_ind];
    } while(cur_ind);
    fprintf (dump_file, "\n");


    // // соединяем стрелками prev
    // cur_ind = 0;
    // do
    // {
    //     fprintf (dump_file, "%d->%d [weight = 0, color = deeppink];\n", cur_ind, lst->prev[cur_ind]);
    //     cur_ind = lst->prev[cur_ind];
    // } while (cur_ind);
    
    // fprintf (dump_file, "\n");

    // красим свободные ячейки в зеленый
    fprintf (dump_file, "free->%d;\n", lst->free_ind);
    int free_block = lst->free_ind;
    printf("free_block=%d\n", free_block);
    printf_array(lst->next, lst->size_lst);
    while (free_block != 0)
    {
        fprintf (dump_file, "%d[fillcolor = " FREE_COLOR "];\n", free_block);
        free_block = lst->next[free_block];
    }

    // указываем на head и tail
    // fprintf (dump_file, "head->%d;\n", lst->head);
    // fprintf (dump_file, "tail->%d;\n", lst->tail);

    fprintf (dump_file, "}\n");

    #undef FREE_COLOR
    #undef BUSY_COLOR
    #undef TITLE_COLOR
}