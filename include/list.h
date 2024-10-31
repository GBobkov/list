#ifndef LIST_H
#define LIST_H

// struct ELEM_LST
// {
//     int value;
//     int next_ind;
//     int anchor_elem; 
// };

typedef int elem_t;

struct LIST
{
    int size_lst;
    int free_ind;
    elem_t* data;
    elem_t* next;
    elem_t* prev;
};



// Создаёт массив элементов длинны size_lst.
int Init_List(LIST* lst, int size_lst);

// Уничтожает массив элементов.
int Destroy_List(LIST lst);


// Добавить элемент(value) на следующее место после указанного(anchor_elem).
int Add_Elem(LIST* lst, int anchor_elem, int value);


// Удалить элемент стоящий по индексу ind.
int Del_Elem(LIST* lst, int ind);




#endif