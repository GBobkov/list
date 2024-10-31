#include "list.h"

#include <stdio.h>
#include <stdlib.h>



// Создаёт массив элементов длинны size_lst.
int Init_List(LIST* lst, int size_lst)
{
    lst->size_lst = size_lst;
    lst->data = (elem_t *) calloc(size_lst, sizeof(elem_t));
    lst->next = (elem_t *) calloc(size_lst, sizeof(elem_t));
    lst->prev = (elem_t *) calloc(size_lst, sizeof(elem_t));

    return 0;
}



// Уничтожает массив элементов.
int Destroy_List(LIST lst)
{
    free(lst.data);
    free(lst.next);
    free(lst.prev);

    return 0;
}



// Добавить элемент(value) на следующее место после указанного(anchor_elem).
int Add_Elem(LIST* lst, int anchor_elem, int value)
{
    //TODO - Рассмотреть случай, если после anchor_elem ничего не следует.


    int new_elem_ind = lst->free_ind; // индекс нового элемента.
    lst->free_ind = lst->next[lst->free_ind]; // массив next хранит также информацию о следующем свободном элементе, сохраняем какой свободный элемент следующий.
                                            // устанавливаем следующий свободный элемент.

    lst->data[new_elem_ind] = value; // кладём новый элемент в свободную ячейку массива data.
    int next_after_new_ind_ind = lst->next[anchor_elem]; // сохраняем, какой элемент шёл после anchor_elem.(назовём этот элемент after_ind)
    lst->next[anchor_elem] = new_elem_ind; // устанавливаем связь, что после anchor_elem теперь следует элемент value по индексу lst->free_ind.
    lst->next[new_elem_ind] = next_after_new_ind_ind; // устанавливаем связь между новым элементом и after_ind.



    int prev_before_new_elem_ind = lst->prev[anchor_elem];  // сохраняем какой элемент шёл перед anchor_elem (назовём его prev_elem)
    lst->prev[anchor_elem] = new_elem_ind;                 // устанавливаем связь anchor_elem->new_elem
    lst->prev[new_elem_ind] = prev_before_new_elem_ind;    // устанавливаем связь new_elem->prev_elem    

    return 0;
} 




// Удалить элемент стоящий по индексу ind.
int Del_Elem(LIST* lst, int anch_ind)
{
    

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
