/*
 * Document_Word_List.c
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#include "Document_Word_List.h"

#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Misc.h"
#include "Print_Tools.h"
#include "Intersection_Approaches.h"



//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Create_Document_Word_List
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = (struct Document_Word_List*) CALLOC(1, sizeof (struct Document_Word_List));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List));

    // Aeussere Dimension
    new_object->data = (uint_fast32_t**) CALLOC(number_of_arrays, sizeof (uint_fast32_t*));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (uint_fast32_t*) * number_of_arrays);

    // Innere Dimension
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data [i] = (uint_fast32_t*) CALLOC(max_array_length, sizeof (uint_fast32_t));
        ASSERT_ALLOC(new_object->data [i], "Cannot create new Document_Word_List !",
                sizeof (uint_fast32_t) * max_array_length);
    }

    // Laengenliste
    new_object->arrays_lengths = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (size_t) * number_of_arrays);

    new_object->max_array_length = max_array_length;
    new_object->number_of_arrays = number_of_arrays;
    new_object->next_free_array = 0;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Delete_Document_Word_List
(
        struct Document_Word_List* object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // Innere Dimension
    for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i)
    {
        FREE_AND_SET_TO_NULL(object->data [i]);
    }

    // Aeussere Dimension
    FREE_AND_SET_TO_NULL(object->data);

    FREE_AND_SET_TO_NULL(object->arrays_lengths);
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Append_Data_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(new_data != NULL, "New data is NULL !");
    ASSERT_MSG(data_length != 0, "New data length is 0 !");

    ASSERT_FMSG(data_length <= object->max_array_length, "New data is too large ! Value %zu; max. valid: %zu",
            data_length, object->max_array_length);
    ASSERT_FMSG(object->number_of_arrays > (size_t) object->next_free_array, "All arrays are in use ! (%zu arrays)",
            object->number_of_arrays);

    // Daten kopieren
    memcpy (object->data [object->next_free_array], new_data, sizeof (uint_fast32_t) * data_length);
    object->arrays_lengths [object->next_free_array] = data_length;
    object->next_free_array ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Show_Data_From_Document_Word_List
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    puts ("> Data <");
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        printf ("%2zu: { ", (i + 1));
        for (size_t i2 = 0; i2 < object->arrays_lengths [i]; ++ i2)
        {
            printf ("%2" PRIuFAST32, object->data [i][i2]);

            if ((i2 + 1) < object->arrays_lengths [i])
            {
                printf (", ");
            }
        }
        puts(" }");
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Show_Data_And_Attributes_From_Document_Word_List
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    Show_Data_From_Document_Word_List(object);

    const int formatter_int = (int) MAX(Count_Number_Of_Digits(object->number_of_arrays),
            Count_Number_Of_Digits(object->max_array_length));

    puts ("> Attributes <");
    printf ("Intersection data: %s\n", (object->intersection_data /* == true */) ? "YES" : "NO");
    printf ("Number of arrays:  %*zu\n", formatter_int, object->number_of_arrays);
    printf ("Max. array length: %*zu\n", formatter_int, object->max_array_length);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Intersect_Data_With_Document_Word_List
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length,

    const enum Intersection_Mode mode
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(data != NULL, "data is NULL !");
    ASSERT_MSG(data_length != 0, "data length is 0 !");
    // ASSERT_MSG(mode == INTERSECTION_MODE_2_NESTED_LOOPS, "Invalid intersection mode !");
    ASSERT_FMSG(data_length <= object->max_array_length, "data is too large ! Value %zu; max. valid: %zu",
            data_length, object->max_array_length)

    // Je nach Modus die passende Variante fuer die Bestimmung der Schnittmenge verwenden
    struct Document_Word_List* intersection_result = NULL;
    switch (mode)
    {
    case INTERSECTION_MODE_2_NESTED_LOOPS:
        intersection_result = Intersection_Approach_2_Nested_Loops(object, data, data_length);
        break;
    case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
        intersection_result = Intersection_Approach_QSort_And_Binary_Search(object, data, data_length);
        break;
    case INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH:
        intersection_result = Intersection_Approach_HeapSort_And_Binary_Search(object, data, data_length);
        break;
    default:
        ASSERT_MSG(false, "Default path executed !");
    }
    ASSERT_MSG(intersection_result != NULL, "Intersection result is NULL !");

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------
