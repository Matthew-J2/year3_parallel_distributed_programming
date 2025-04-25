#ifndef PARTC_HPP
#define PARTC_HPP

#include "mpi.h"
#include <cstring>

struct ExampleStruct1;
struct ExampleStruct2;
struct ExampleStruct3;

// Struct definitions, friend function to override output operator for easier printing
struct ExampleStruct1{
    float list[15];
    int size;
    char letter;
    double number;
    int x;
    int y;
    friend std::ostream& operator<<(std::ostream& os, const ExampleStruct1& obj);
};
// Number of fields.
#define FIELDS_EXAMPLE1 6
// Corresponding MPI types for each field.
MPI_Datatype TYPES1[FIELDS_EXAMPLE1] = {
    MPI_FLOAT, 
    MPI_INT,
    MPI_CHAR,
    MPI_DOUBLE,
    MPI_INT,
    MPI_INT,
};   

// Same for other structs.
struct ExampleStruct2{
    int playerlist[30];
    int playerlistSize;
    char levelName[50];
    int levelNameSize;
    long double randVar;
    int gameBoardWidth;
    int gameBoardHeight;
    friend std::ostream& operator<<(std::ostream& os, const ExampleStruct2& obj);  
};
#define FIELDS_EXAMPLE2 7
MPI_Datatype TYPES2[FIELDS_EXAMPLE2] = {
    MPI_INT, 
    MPI_INT,
    MPI_CHAR,
    MPI_INT,
    MPI_LONG_DOUBLE,
    MPI_INT,
    MPI_INT,
};  

struct ExampleStruct3{
    char playerName[100];
    int playerNameSize;
    float playerHealth;
    float playerMana;
    char itemList[50];
    int itemListSize;
    double warewolfPercentage;
    friend std::ostream& operator<<(std::ostream& os, const ExampleStruct3& obj);
};
#define FIELDS_EXAMPLE3 7
MPI_Datatype TYPES3[FIELDS_EXAMPLE3] = {
    MPI_CHAR, 
    MPI_INT,
    MPI_FLOAT,
    MPI_FLOAT,
    MPI_CHAR,
    MPI_INT,
    MPI_DOUBLE,
};  

template <typename T>
void create_MPI_struct(T& example, int num_fields, void** field_addresses, MPI_Datatype* types, const int* blocklengths, MPI_Datatype& mpi_struct);

#endif