/**
 * @file Document_Word_List.c
 *
 * @brief The Document_Word_List object is a two dimensional dynamic container for integer values (-> The mapped data).
 *
 * This object can be one of the input data of a intersection operation or a result object of them.
 *
 * The object name indicates, that it is a list with data from a whole document.
 *
 * @date 22.05.2022
 * @author Gyps
 */

#include "Document_Word_List.h"

#include <string.h>
#include <limits.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/_Generics.h"
#include "Misc.h"
#include "Print_Tools.h"
#include "Intersection_Approaches.h"



/**
 * @brief Allocation step size for the inner data arrays. (For the real data; not for management objects)
 */
#ifndef INT_ALLOCATION_STEP_SIZE
#define INT_ALLOCATION_STEP_SIZE 5
#else
#error "The macro \"INT_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* INT_ALLOCATION_STEP_SIZE */

/**
 * @brief Check, whether the macro value are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(INT_ALLOCATION_STEP_SIZE > 0, "The marco \"INT_ALLOCATION_STEP_SIZE\" is zero !");

IS_TYPE(INT_ALLOCATION_STEP_SIZE, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

/**
 * @brief Create a new Document_Word_List and initialize the main structure.
 *
 * Additional operations (e.g. data for a intersection result) will be done in the specific creation function.
 *
 * The variable "max_array_length" is actual not in use. In the past the idea of this value was to make all dynamic
 * memory blocks large enough, that every block could hold all data. -> Reduction of realloc calls. But it is to
 * inefficient in the memory usage. This value is nevertheless available, because the idea with the worst case memory
 * block could be useful in the future. But not currently!
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of arrays (Subsets)
 * @param[in] max_array_length Maximum length of the subsets
 *
 * @return Pointer to the new dynamic allocated Document_Word_List
 */
static struct Document_Word_List*
Create_Main_Object_Structure
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

/**
 * @brief Increase the size of a data array.
 *
 * increase_number_of_objects is NOT the sum of the array objects ! It is the number of new objects ! This sounds
 * trivial with the name "increase_number_of_objects". But it should be 100% clear for everyone.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *      increase_number_of_objects > 0
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 * @param[in] increase_number_of_objects Number of new objects
 */
static void Increase_Data_Array_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index,
        const size_t increase_number_of_objects
);

/**
 * @brief Increase the size of a data array with the INT_ALLOCATION_STEP_SIZE macro.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 */
static inline void Increase_Data_Array_Size_Allocation_Step_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Create new document word list.
 *
 * The variable "max_array_length" is actual not in use. In the past the idea of this value was to make all dynamic
 * memory blocks large enough, that every block could hold all data. -> Reduction of realloc calls. But it is to
 * inefficient in the memory usage. This value is nevertheless available, because the idea with the worst case memory
 * block could be useful in the future. But not currently!
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of arrays (Subsets)
 * @param[in] max_array_length Maximum length of the subsets
 *
 * @return Pointer to the new dynamic allocated Document_Word_List
 */
extern struct Document_Word_List*
DocumentWordList_CreateObject
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    // Avoid a unused warning
    (void) Increase_Data_Array_Size_Allocation_Step_Size;
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    // Value check although this value is not in use !
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = Create_Main_Object_Structure (number_of_arrays, max_array_length);

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Create new document word list as result for an intersection process.
 *
 * The variable "max_array_length" is actual not in use. In the past the idea of this value was to make all dynamic
 * memory blocks large enough, that every block could hold all data. -> Reduction of realloc calls. But it is to
 * inefficient in the memory usage. This value is nevertheless available, because the idea with the worst case memory
 * block could be useful in the future. But not currently!
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of arrays (Subsets)
 * @param[in] max_array_length Maximum length of the subsets
 *
 * @return Pointer to the new dynamic allocated Document_Word_List
 */
extern struct Document_Word_List*
DocumentWordList_CreateObjectAsIntersectionResult
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    // Avoid a unused warning
    (void) Increase_Data_Array_Size_Allocation_Step_Size;
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    // Value check although this value is not in use !
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = Create_Main_Object_Structure (number_of_arrays, max_array_length);

    // Outer dimension for char offsets
    new_object->data_struct.char_offsets = (CHAR_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (CHAR_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.char_offsets, "Cannot create new Document_Word_List !",
            sizeof (CHAR_OFFSET_TYPE*) * number_of_arrays);

    // Outer dimension for sentence offsets
    new_object->data_struct.sentence_offsets =
            (SENTENCE_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (SENTENCE_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.sentence_offsets, "Cannot create new Document_Word_List !",
            sizeof (SENTENCE_OFFSET_TYPE*) * number_of_arrays);

    // Outer dimension for word offsets
    new_object->data_struct.word_offsets =
            (WORD_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (WORD_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.word_offsets, "Cannot create new Document_Word_List !",
            sizeof (WORD_OFFSET_TYPE*) * number_of_arrays);

    new_object->malloc_calloc_calls += 3;

    // Inner dimension for char, sentence and word offsets
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data_struct.char_offsets [i] = (CHAR_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.char_offsets [i], "Cannot create new Document_Word_List !",
                sizeof (CHAR_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);

        new_object->data_struct.sentence_offsets [i] = (SENTENCE_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.sentence_offsets [i], "Cannot create new Document_Word_List !",
                sizeof (SENTENCE_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);

        new_object->data_struct.word_offsets [i] = (WORD_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (WORD_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.word_offsets [i], "Cannot create new Document_Word_List !",
                sizeof (WORD_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);
    }
    new_object->malloc_calloc_calls += number_of_arrays * 3;

    // Show, that this object was created for intersection results
    new_object->intersection_data = true;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Delete a Document_Word_List object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List, which will be deleted
 */
extern void
DocumentWordList_DeleteObject
(
        struct Document_Word_List* object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // Inner dimension
    for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i)
    {
        free(object->data_struct.data [i]);
        // FREE_AND_SET_TO_NULL(object->data_struct.data [i]);
    }
    GLOBAL_free_calls += object->number_of_arrays;
    if (object->intersection_data)
    {
        for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i)
        {
            free(object->data_struct.char_offsets [i]);
            // FREE_AND_SET_TO_NULL(object->data_struct.char_offsets [i]);
            free(object->data_struct.sentence_offsets [i]);
            // FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets [i]);
            free(object->data_struct.word_offsets [i]);
            // FREE_AND_SET_TO_NULL(object->data_struct.word_offsets [i]);
        }
        GLOBAL_free_calls += 3 * object->number_of_arrays;
    }
    free(object->data_struct.data);
    // FREE_AND_SET_TO_NULL(object->data_struct.data);
    ++ GLOBAL_free_calls;

    if (object->intersection_data)
    {
        free(object->data_struct.char_offsets);
        // FREE_AND_SET_TO_NULL(object->data_struct.char_offsets);
        free(object->data_struct.sentence_offsets);
        // FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets);
        free(object->data_struct.word_offsets);
        // FREE_AND_SET_TO_NULL(object->data_struct.word_offsets);
        GLOBAL_free_calls += 3;
    }

    free(object->allocated_array_size);
    // FREE_AND_SET_TO_NULL(object->allocated_array_size)
    free(object->arrays_lengths);
    // FREE_AND_SET_TO_NULL(object->arrays_lengths);
    free(object);
    // FREE_AND_SET_TO_NULL(object);
    GLOBAL_free_calls += 3;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add data to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendData
(
        struct Document_Word_List* const object,
        const DATA_TYPE* const new_data,
        const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(new_data != NULL, "New data is NULL !");
    ASSERT_MSG(data_length != 0, "New data length is 0 !");

    //ASSERT_FMSG(data_length <= object->max_array_length, "New data is too large ! Value %zu; max. valid: %zu",
    //        data_length, object->max_array_length);
    ASSERT_FMSG(object->number_of_arrays > (size_t) object->next_free_array, "All arrays are in use ! (%zu arrays)",
            object->number_of_arrays);

    // Increase data, if necessary
    if (data_length > object->allocated_array_size [object->next_free_array])
    {
        Increase_Data_Array_Size(object, object->next_free_array,
                data_length - object->allocated_array_size [object->next_free_array]);
    }

    // Copy the new data
    memcpy (object->data_struct.data [object->next_free_array], new_data, sizeof (DATA_TYPE) * data_length);
    object->arrays_lengths [object->next_free_array] = data_length;
    object->next_free_array ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add data with up to two offset arrays to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] new_offsets Offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithOffsets
(
        struct Document_Word_List* const object,
        const DATA_TYPE* const new_data,
        const CHAR_OFFSET_TYPE* const new_offsets,
        const size_t data_length
)
{
    DocumentWordList_AppendData(object, new_data, data_length);

    // Because the last function alters the array length value, it is necessary to undo this
    const uint_fast32_t next_free_array = object->next_free_array - 1;
    if (new_offsets != NULL)
    {
        memcpy (object->data_struct.char_offsets [next_free_array], new_offsets, sizeof (CHAR_OFFSET_TYPE) * data_length);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add data with up to three offsets (char, sentence and word offset) to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] new_char_offsets Char offset array
 * @param[in] new_sentence_offsets Sentence offset array
 * @param[in] new_word_offsets Word offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithThreeTypeOffsets
(
        struct Document_Word_List* const object,
        const DATA_TYPE* const new_data,
        const CHAR_OFFSET_TYPE* const new_char_offsets,
        const SENTENCE_OFFSET_TYPE* const new_sentence_offsets,
        const WORD_OFFSET_TYPE* const new_word_offsets,
        const size_t data_length
)
{
    DocumentWordList_AppendDataWithOffsets(object, new_data, new_char_offsets, data_length);

    // Copy the data, if available
    if (new_sentence_offsets != NULL)
    {
        memcpy (object->data_struct.sentence_offsets [object->next_free_array - 1], new_sentence_offsets,
                sizeof (SENTENCE_OFFSET_TYPE) * data_length);
    }
    if (new_word_offsets != NULL)
    {
        memcpy (object->data_struct.word_offsets [object->next_free_array - 1], new_word_offsets,
                sizeof (WORD_OFFSET_TYPE) * data_length);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add one value to a Document_Word_List.
 *
 * ! Caution: If you use this function the container manages this single value as an data set with the length of one.
 * The effect is, that one array in the container will be used like an full array !
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 */
extern void
DocumentWordList_AppendOneValueAsNewDataSet
(
        struct Document_Word_List* const object,
        const DATA_TYPE new_value
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // The function redirects the appending. =>  The new value is a dataset with one element
    DocumentWordList_AppendData(object, &new_value, 1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Put one value to a Document_Word_List.
 *
 * The new value will be appended to the next free data array. Instead of Append_One_Value_To_Document_Word_List, the
 * new value will NOT be interpreted as new data set !
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 */
extern void
Put_One_Value_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const DATA_TYPE new_value
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    const uint_fast32_t next_free_array = object->next_free_array;

    // Is enough memory available ?
    if (object->allocated_array_size [next_free_array] <= object->arrays_lengths [next_free_array])
    {
        Increase_Data_Array_Size_Allocation_Step_Size(object, next_free_array);
    }
    object->data_struct.data [next_free_array][object->arrays_lengths [next_free_array]] = new_value;
    object->arrays_lengths [next_free_array] ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Put one value with offsets of the three types to a Document_Word_List.
 *
 * The new value and the given offsets will be appended to the next free data array. Instead of
 * DocumentWordList_AppendOneValueAsNewDataSet, the new value will NOT be interpreted as new data set !
 * @see DocumentWordList_AppendOneValueAsNewDataSet
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 * @param[in] new_char_offset First new char offset value
 * @param[in] new_sentence_offset First new sentence offset value
 * @param[in] new_word_offset First new word offset value
 */
extern void
Put_One_Value_And_Offset_Types_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const DATA_TYPE new_value,
        const CHAR_OFFSET_TYPE new_char_offset,
        const SENTENCE_OFFSET_TYPE new_sentence_offset,
        const WORD_OFFSET_TYPE new_word_offset
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    Put_One_Value_To_Document_Word_List(object, new_value);
    const uint_fast32_t next_free_array = object->next_free_array;

    // Because the last function alters the array length value, it is necessary to undo this
    object->arrays_lengths [next_free_array] --;
    object->data_struct.char_offsets [next_free_array][object->arrays_lengths [next_free_array]] = new_char_offset;
    object->data_struct.sentence_offsets [next_free_array][object->arrays_lengths [next_free_array]] = new_sentence_offset;
    object->data_struct.word_offsets [next_free_array][object->arrays_lengths [next_free_array]] = new_word_offset;
    object->arrays_lengths [next_free_array] ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print data of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowData
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
            printf ("%2" PRIuFAST32, (uint_fast32_t) object->data_struct.data [i][i2]);

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

/**
 * @brief Determine the full memory usage in byte.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] container Document_Word_List object
 *
 * @return Size of the full object in bytes
 */
extern size_t
DocumentWordList_GetAllocatedMemSize
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    size_t result = sizeof (size_t) * object->number_of_arrays;
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        result += object->allocated_array_size [i] * sizeof (uint_fast32_t);
    }
    result += sizeof (struct Document_Word_List);
    result += object->number_of_arrays * sizeof (uint_fast32_t*);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print attributes of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowAttributes
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    size_t count_tokens = 0;
    for (uint_fast32_t i = 0; i < object->next_free_array; i ++) { count_tokens += object->arrays_lengths [i]; }
    const size_t avg_array_length = count_tokens / object->number_of_arrays;

    // Int formatter for the output
    int formatter_int = (int) MAX_WITH_TYPE_CHECK(Count_Number_Of_Digits(object->number_of_arrays),
                Count_Number_Of_Digits(object->max_array_length));
    formatter_int = MAX_WITH_TYPE_CHECK((int) Count_Number_Of_Digits(object->malloc_calloc_calls), formatter_int);
    formatter_int = MAX_WITH_TYPE_CHECK((int) Count_Number_Of_Digits(count_tokens), formatter_int);
    formatter_int = MAX_WITH_TYPE_CHECK((int) Count_Number_Of_Digits(avg_array_length), formatter_int);


    puts("");
    printf("Full document word list container size: ");
    Print_Memory_Size_As_B_KB_MB(DocumentWordList_GetAllocatedMemSize(object));

    puts ("> Attributes <");
    printf ("Intersection data:     %*s\n",  formatter_int, (object->intersection_data /* == true */) ? "YES" : "NO");
    printf ("Number of tokens:      %*zu\n", formatter_int, count_tokens);
    printf ("Number of arrays:      %*zu\n", formatter_int, object->number_of_arrays);
    printf ("Max. array length:     %*zu\n", formatter_int, object->max_array_length);
    printf ("Avg. array length:     %*zu\n", formatter_int, avg_array_length);
    printf ("Malloc / calloc calls: %*zu\n", formatter_int, object->malloc_calloc_calls);
    printf ("Realloc calls:         %*zu\n", formatter_int, object->realloc_calls);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print data and the attribute information of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowDataAndAttributes
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    DocumentWordList_ShowData(object);
    DocumentWordList_ShowAttributes(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersection with a Document_Word_List and a data set.
 *
 * You can change the intersection mode with the enum parameter Intersection_Mode. Actual following modes are
 * available
 * - 2 nested loops
 * - QSort with binary search(data and char_offsets (1 and 2))
 * - Heapsort with binary search
 *
 * Asserts:
 *      object != NULL
 *      data != NULL
 *      data_length > 0
 *      data_length <= object->max_array_length
 *
 * @param[in] object Document_Word_List (1. Set for the intersection calculation)
 * @param[in] data Data set (2. Set for the intersection calculation)
 * @param[in] data_length Size of the data set
 * @param[in] mode Mode for the intersection calculation (Which approach should be used ?)
 *
 * @return New Document_Word_List with the intersections in the respective subsets
 */
extern struct Document_Word_List*
DocumentWordList_IntersectWithData
(
        const struct Document_Word_List* const restrict object,
        const DATA_TYPE* const restrict data,
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

    // Execute the proper function for the selected mode
    struct Document_Word_List* intersection_result = NULL;
    switch (mode)
    {
    case INTERSECTION_MODE_2_NESTED_LOOPS:
        intersection_result = IntersectionApproach_TwoNestedLoops(object, data, data_length);
        break;
    case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
        intersection_result = IntersectionApproach_QSortAndBinarySearch(object, data, data_length);
        break;
    case INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH:
        intersection_result = IntersectionApproach_HeapSortAndBinarySearch(object, data, data_length);
        break;
    default:
        ASSERT_MSG(false, "Default path executed !");
    }
    ASSERT_MSG(intersection_result != NULL, "Intersection result is NULL !");

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Is there data in a Document_Word_List?
 *
 * After an intersection variables like "next_free_array" are set to the value of the data, which was used in the
 * intersection. The reason: in every array could be an intersection result. So these values are necessary. But you
 * cannot expect, when "next_free_array" is > 0, that then data is in the object. (At lest when the object was created
 * as result of an intersection process)
 *
 * This function closes the gap. It shows, if there are data in an Document_Word_List object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 *
 * @return true, if there is data in the object, otherwise false
 */
extern _Bool
DocumentWordList_IsDataInObject
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    _Bool result = false;

    // For the test is one exception: When the object only contains one array or when the object uses the first array
    if (object->next_free_array == 0 && object->number_of_arrays > 0)
    {
        if (object->arrays_lengths [0] > 0)
        {
            result = true;
        }
    }
    else
    {
        for (uint_fast32_t i = 0; i < object->next_free_array; ++ i)
        {
            if (object->arrays_lengths [i] > 0)
            {
                result = true;
                break;
            }
        }
    }

    return result;
}

//=====================================================================================================================

/**
 * @brief Create a new Document_Word_List and initialize the main structure.
 *
 * Additional operations (e.g. data for a intersection result) will be done in the specific creation function.
 *
 * The variable "max_array_length" is actual not in use. In the past the idea of this value was to make all dynamic
 * memory blocks large enough, that every block could hold all data. -> Reduction of realloc calls. But it is to
 * inefficient in the memory usage. This value is nevertheless available, because the idea with the worst case memory
 * block could be useful in the future. But not currently!
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of arrays (Subsets)
 * @param[in] max_array_length Maximum length of the subsets
 *
 * @return Pointer to the new dynamic allocated Document_Word_List
 */
static struct Document_Word_List*
Create_Main_Object_Structure
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    // Value check although this value is not in use !
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = (struct Document_Word_List*) CALLOC(1, sizeof (struct Document_Word_List));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List));

    // Outer dimension
    new_object->data_struct.data = (DATA_TYPE**) CALLOC(number_of_arrays, sizeof (DATA_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.data, "Cannot create new Document_Word_List !",
            sizeof (DATA_TYPE*) * number_of_arrays);

    // New management data
    new_object->allocated_array_size = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object->data_struct.data, "Cannot create new Document_Word_List !",
                sizeof (size_t) * number_of_arrays);

    new_object->malloc_calloc_calls += 3;

    // Inner dimension
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data_struct.data [i] = (DATA_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE, sizeof (DATA_TYPE));
        ASSERT_ALLOC(new_object->data_struct.data [i], "Cannot create new Document_Word_List !",
                sizeof (DATA_TYPE) * INT_ALLOCATION_STEP_SIZE);

        new_object->allocated_array_size [i] = INT_ALLOCATION_STEP_SIZE;
    }
    new_object->malloc_calloc_calls += number_of_arrays;

    // Length list
    new_object->arrays_lengths = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (size_t) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    new_object->max_array_length    = INT_ALLOCATION_STEP_SIZE;
    new_object->number_of_arrays    = number_of_arrays;
    new_object->next_free_array     = 0;

    new_object->data_struct.char_offsets        = NULL;
    new_object->data_struct.sentence_offsets    = NULL;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the size of a data array.
 *
 * increase_number_of_objects is NOT the sum of the array objects ! It is the number of new objects ! This sounds
 * trivial with the name "increase_number_of_objects". But it should be 100% clear for everyone.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *      increase_number_of_objects > 0
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 * @param[in] increase_number_of_objects Number of new objects
 */
static void Increase_Data_Array_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index,
        const size_t increase_number_of_objects
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_FMSG(data_array_index < object->number_of_arrays, "Data array index is invald ! Got: %zu; max valid: %zu !",
            data_array_index, object->number_of_arrays - 1);
    ASSERT_MSG(increase_number_of_objects > 0, "Number of increase objects ins 0 !");

    ++ object->realloc_calls;

    DATA_TYPE* tmp_ptr = (DATA_TYPE*) REALLOC(object->data_struct.data [data_array_index],
            (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (DATA_TYPE));
    ASSERT_ALLOC(tmp_ptr, "Cannot increase the data array size !",
            (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (DATA_TYPE))
    object->data_struct.data [data_array_index] = tmp_ptr;

    // Set the new range of the memory block to '\0'
    memset (&(object->data_struct.data [data_array_index][object->allocated_array_size [data_array_index]]), '\0',
            increase_number_of_objects * sizeof (DATA_TYPE));

    // If the object is a intersection data, then the offset arrays also need a resize operation
    if (object->intersection_data)
    {
        CHAR_OFFSET_TYPE* tmp_ptr_offset = (CHAR_OFFSET_TYPE*) REALLOC(object->data_struct.char_offsets [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_offset, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE))

        object->data_struct.char_offsets [data_array_index] = tmp_ptr_offset;

        SENTENCE_OFFSET_TYPE* tmp_ptr_sentence_offset = (SENTENCE_OFFSET_TYPE*) REALLOC(object->data_struct.sentence_offsets [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_sentence_offset, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE))

        object->data_struct.sentence_offsets [data_array_index] = tmp_ptr_sentence_offset;

        WORD_OFFSET_TYPE* tmp_ptr_word_offset = (WORD_OFFSET_TYPE*) REALLOC(object->data_struct.word_offsets [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (WORD_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_word_offset, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (WORD_OFFSET_TYPE))

        object->data_struct.word_offsets [data_array_index] = tmp_ptr_word_offset;

        // Init the new memory (No zero, because a zero is a valid offset !)
        for (size_t i = object->allocated_array_size [data_array_index];
                i < (object->allocated_array_size [data_array_index] + increase_number_of_objects); ++ i)
        {
            object->data_struct.char_offsets [data_array_index][i]      = CHAR_OFFSET_TYPE_MAX;
            object->data_struct.sentence_offsets [data_array_index][i]  = SENTENCE_OFFSET_TYPE_MAX;
            object->data_struct.word_offsets [data_array_index][i]      = WORD_OFFSET_TYPE_MAX;
        }

        object->realloc_calls += 3;
    }

    object->allocated_array_size [data_array_index] += increase_number_of_objects;

    // If the new allocated array size bigger than the saved max allocated size
    if (object->allocated_array_size [data_array_index] > object->max_array_length)
    {
        object->max_array_length = object->allocated_array_size [data_array_index];
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the size of a data array with the INT_ALLOCATION_STEP_SIZE macro.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 */
static inline void Increase_Data_Array_Size_Allocation_Step_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_FMSG(data_array_index < object->number_of_arrays, "Data array index is invald ! Got: %zu; max valid: %zu !",
            data_array_index, object->number_of_arrays - 1);

    Increase_Data_Array_Size(object, data_array_index, INT_ALLOCATION_STEP_SIZE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------



#ifdef INT_ALLOCATION_STEP_SIZE
#undef INT_ALLOCATION_STEP_SIZE
#endif /* INT_ALLOCATION_STEP_SIZE */
