/*
 * Token_Int_Mapping.h
 *
 *  Created on: 23.06.2022
 *      Author: am1
 */

#ifndef TOKEN_INT_MAPPING_H
#define TOKEN_INT_MAPPING_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>



#ifndef C_STR_ARRAYS
#define C_STR_ARRAYS 100
#else
#error "The macro \"C_STR_ARRAYS\" is already defined !"
#endif /* C_STR_ARRAYS */

//=====================================================================================================================

struct Token_Int_Mapping
{
    char* c_str_arrays [C_STR_ARRAYS];                  // C-String Arrays, die die Tokens beinhalten
                                                        // Flaches Speichermodell !
    uint_fast32_t* int_mapping [C_STR_ARRAYS];          // Mapping Nummern

    size_t allocated_c_strings_in_array;                // Anzahl an allokierten Tokens-Objekte
    uint_fast32_t c_str_array_lengths [C_STR_ARRAYS];   // Laenge der C-String Arrays
};

//=====================================================================================================================

/**
 * @brief Neues Token_Int_Mapping-Objekt dynamisch erzeugen.
 */
extern struct Token_Int_Mapping*
Create_Token_Int_Mapping
(
        void
);

/**
 * @brief Dynamisch erzeugtes Token_Int_Mapping-Objekt loeschen.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 */
extern void
Delete_Token_Int_Mapping
(
        struct Token_Int_Mapping* object
);

/**
 * @brief Token zur Mapping Tabelle hinzufuegen.
 *
 * Der Rueckgabewert ist ein Flag, welches anzeigt, ob die Operation erfolgreich war. Die Operation ist nicht
 * erfolgreich, wenn das neue Token bereits in der Mapping Tabelle ist. In diesem Fall wird es NICHT nochmal eingebaut !
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 * @param[in] new_token Neues Token
 * @param[in] new_token_length Laenge des neuen Tokens
 *
 * @return Flag, welches anzeigt, ob die Operation erfolgreich war
 */
extern _Bool
Add_Token_To_Mapping
(
        struct Token_Int_Mapping* const restrict object,
        const char* const restrict new_token,
        const size_t new_token_length
);

/**
 * @brief Anzahl an Tokens in allen Untercontainern ausgeben.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 */
extern void
Show_C_Str_Array_Usage
(
        const struct Token_Int_Mapping* const object
);

/**
 * @brief Den gemappten Wert fuer das uebergebene Token ermitteln und zurueckgeben.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 * @param[in] search_token Token wofuer der gemappte Wert gesucht wird
 * @param[in] search_token_length Laenge vom uebergebenen Token
 *
 * @return Gemappter Wert oder UINT_FAST32_MAX, falls das Token nicht in der Mapping-Liste vorhanden ist.
 */
extern uint_fast32_t
Token_To_Int
(
        const struct Token_Int_Mapping* const restrict object,
        const char* const restrict search_token,
        const size_t search_token_length
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TOKEN_INT_MAPPING_H */
