// (C) Sebastian Fiault 2026
#ifndef DATA_UTILS_H
#define DATA_UTILS_H

    #include <stdarg.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>

    #include "../Constants.h"
    #include "../GalaxyCollision.h"
    #include "../Types.h"

    /*
    SYNOPSIS:
        Takes an input of a string that contains format specifiers and a variable amount of arguments to fill
        in for the format specifiers and returns the assembled string.
        Literally like printf only instead of printing it returns a string.
    ARGS:
        toFormat:
            The string with the format specifiers inside it
        ...:
            The values to fill in for the format specifier
    RETURNS:
        Either NULL if something went wrong or
        the assembled string
    REMARKS:
        Only works on Windows machines
    */
    char* formatString(const char* toFormat, ...);

    /*
    SYNOPSIS:
        Frees the allocated memory AND sets the pointer to 0 to avoid issues with dangling pointers
    ARGS:
        pointer:
            A POINTER to the pointer that we want to free the memory of and set to zero.
            Note that this must be a pointer to the pointer in question so that it can be modified (the pointer being set to 0)
    REMARKS:
        Remember to pass this method a pointer to a pointer with the address-of operator "&"
    */
    void safer_free(void** pointer);

    /*
    SYNOPSIS:
    Generates a random int of the specified size in bytes,
        meaning that the returned int will at most be as large as the largest possible unsigned
        int of the specified size.
        DESCRIPTION:
        The function takes the size of the int to be generated in bytes,
        calculates how many bytes of regular ints are needed to create
        ann int of the specified size and then generates random integers
        and concatenates them to be a single unsigned long long which then
        can be cast into smaller types.
    ARGS:
        size:
            How large the generated int is going to be in Bytes
    RETURNS:
        Returns an random unsigned long long that is maximally as large as
        an unsigned int of the specified size in bytes.
    REMARKS:
        Even though the method returns and unsigned long long this long is intended to be castable
        to smaller types
    EXAMPLE:
        long long int randomNumber = wideRandint(8); ==> 7435984
        int16_t randomNumberSmall = (int16_t)wideRandint(2); ==> -12089
        */
    unsigned long long wideRandint(short int size);

    /*
    SYNOPSIS:
        Generates a random int with 32 bits
    DESCRIPTION:
        Uses the regular rand() function and adapts the output
        to be 32 bits to avoid platform specific errors where rand() does
        not yield 32 bits
    ARGS:
        The maximum absolute size of the generated random number
    RETURNS:
        A random int32_t
    EXAMPLE:
        int32_t myRandInt = randint32(500);
    */
    int32_t randint32(int32_t max);

    /*
    SYNOPSIS:
        Generates a random int with 64 bits
    DESCRIPTION:
        Uses the regular rand() function and adapts the output
        to be 64 bits to avoid platform specific errors where rand() does
        not yield 64 bits
        ARGS:
        The maximum absolute size of the generated random number
    RETURNS:
        A random int64_t
    EXAMPLE:
        int64_t myRandInt = randint64(9040);
    */
    int64_t randint64(int64_t max);

    /*
    SYNOPSIS:
        Casts a Vector3Int32 struct to a Vector3Float32 struct
    DESCRIPTION:
        Casts all individual components of a Vector3Int32 struct to floats,
        then builds a Vector3Float32 struct from them and returns the new Vector3Float32 struct
    ARGS:
        vectorToConvert:
            The vector to convert
    RETURNS:
        The provided Vector3Int32 struct with it's components cast to float, creating a Vector3Float32 struct
    EXAMPLE:
        Vector3Int32 myVector = {1,3,4};
        Vector3Float32 = Vector3Int32_To_Vector3Float32(myVector); ==> {1.0f, 3.0f, 4.0f}
        */
    Vector3_Float32 Vector3Int32_To_Vector3Float32(Vector3_Int32 vectorToConvert);

    /*
    SYNOPSIS:
    Casts a Vector3Int64 struct to a Vector3Double64 struct
    DESCRIPTION:
    Casts all individual components of a Vector3Int64 struct to doubles,
    then builds a Vector3Double64 struct from them and returns the new Vector3Float32 struct
    ARGS:
    vectorToConvert:
            The vector to convert
    RETURNS:
        The provided Vector3Int64 struct with it's components cast to doubles, creating a Vector3Double64 struct
        EXAMPLE:
        Vector3Int64 myVector = {1,3,4};
        Vector3Double64 = Vector3Int64_To_Vector3Double64(myVector); ==> {1.0, 3.0, 4.0}
        */
    Vector3_Double64 Vector3Int64_To_Vector3Double64(Vector3_Int64 vectorToConvert);

    /*
    SYNOPSIS:
    A wrapper function for memcpy/memcpy_s depending on wether the system has the function available or not
    DESCRIPTION:
        If the System has the safer memcpy_s function available that function will be used, if not the regular memcpy function will be used.
        Since memcpy_s needs 4 parameters this function has 4 parameters as well even though memcpy does only need 3.
        Meaning that if the system does not have memcpy_s the unneeded parameters simply do not get used.
        The function itself also adds some error checking to remain safer than memcpy.
    ARGS:
        destination:
            A pointer to the destination where the data is supposed to get copied to.
        destinationSize:
        The size of the object pointed to by destination
        source:
            The pointer to the source from which the data is supposed to be copied.
        count:
        How many bytes should be copied from source to destination
    */
    errno_t gamagos_memcpy_s(void* destination, size_t destinationSize, void* source, size_t count);

    /*
    SYNOPSIS:
        Takes a list of smaller ints and combines them to bigger int
    DESCRIPTION:
        Takes a list of smaller ints and chains them together to form a bigger int from first to last.
        Cannot create ints larger than 8 bytes
    ARGS:
        narrowInts:
            Pointer to an array of integers of variable width,
            which is why a void pointer is used here. These are the integers that are to be merged into a single
            unsigned long long.
        intSize:
        How large the ints passed actually are, in Bytes
        intCount:
            The amount of ints in the array that the narrowInts pointer points to
    RETURNS:
        The unsigned long long representation of the concatenation of the raw bits of the provided integers.
        If the provided bits are fewer than 64 than the returned value will not be larger than 2^(amount of bits),
        meaning it can be cast to smaller integer types and still fit.
    REMARKS:
        DOES NOT ARITHMETICALLY ADD TOGETHER INTS, it just chains the raw bytes and returns them as one int.
        The integers all need to have the same size.
        Cannot create integers beyond 8 bytes (unsigned long long int).
        Concatenates ints from first to last
    EXAMPLE:
        int narrowint1, narrowint2 = INT_MAX, INT_MAX;
        unsigned long long newint = buildWideIntFromNarrowInts();
    */
    unsigned long long buildWideIntFromNarrowInts(void* narrowInts, size_t intSize, unsigned long long intCount);

#endif