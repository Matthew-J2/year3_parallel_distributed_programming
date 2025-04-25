#include <iostream>
#include "mpi.h"
#include "omp.h"
#include "partc.hpp"
#include <cstring>
#include <vector>

// Overload output operators for each struct to make printing easier
std::ostream& operator<<(std::ostream& os, const ExampleStruct1& obj) {
    for (int i = 0; i < 15; ++i) {
      os << "list[" << i << "]: " << obj.list[i] << std::endl;
    }
    os << "size: " << obj.size << std::endl;
    os << "letter: " << obj.letter << std::endl;
    os << "number: " << obj.number << std::endl;
    os << "x: " << obj.x << std::endl;
    os << "y: " << obj.y << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const ExampleStruct2& obj) {
    os << "playerlist: ";
    for (int i = 0; i < obj.playerlistSize; ++i) {
      os << obj.playerlist[i] << " ";
    }
    os << std::endl;

    os << "playerlistSize: " << obj.playerlistSize << std::endl;
    os << "levelName: " << obj.levelName << std::endl;
    os << "levelNameSize: " << obj.levelNameSize << std::endl;
    os << "randVar: " << obj.randVar << std::endl;
    os << "gameBoardWidth: " << obj.gameBoardWidth << std::endl;
    os << "gameBoardHeight: " << obj.gameBoardHeight << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const ExampleStruct3& obj) {
    os << "playerName: " << obj.playerName << std::endl;
    os << "playerNameSize: " << obj.playerNameSize << std::endl;
    os << "playerHealth: " << obj.playerHealth << std::endl;
    os << "playerMana: " << obj.playerMana << std::endl;
    os << "itemList: " << obj.itemList << std::endl;
    os << "itemListSize: " << obj.itemListSize << std::endl;
    os << "warewolfPercentage: " << obj.warewolfPercentage << std::endl;
    return os;
}

/* Function to abstract creating and committing an MPI struct
  Arguments:
    cpp_struct: The cpp struct the MPI struct is created from.
    num_fields: The number of fields in the struct.
    field_addresses: Array of void pointers to the fields of the c++ structure
    types: Array of MPI datatypes for each field in the struct.
    blocklengths: Array of integers representing the number of elements in each field.
    mpi_struct: Resulting MPI datatype passed by reference.
*/
template <typename T>
void create_MPI_struct(T& cpp_struct, int num_fields, void** field_addresses, MPI_Datatype* types, const int* blocklengths, MPI_Datatype& mpi_struct) {
    // MPI addresses for base address and current address
    // A vector of MPI addresses to store each field's offset.
    MPI_Aint base_address, current_address;
    std::vector<MPI_Aint> displacements(num_fields);

    // Calculate displacements for each field
    // This approach was taken instead of e.g. using std::offset_of
    // because it accounts for compilers inserting padding etc at run-time,
    // and uses the absolute memory address.
    MPI_Get_address(&cpp_struct, &base_address);
    for (int i = 0; i < num_fields; i++) {
      MPI_Get_address(field_addresses[i], &current_address);
      displacements[i] = current_address - base_address;
    }

    // Create the MPI datatype and commit it
    MPI_Type_create_struct(num_fields, blocklengths, displacements.data(), types, &mpi_struct);
    MPI_Type_commit(&mpi_struct);
}


int main(int argc, char** argv) {

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Define the rank of the process, destination and source node
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int dest = 1;
  int src = 0;

  // Initialize list of number of elements in each field
  const int blocklengths1[FIELDS_EXAMPLE1] = {15, 1, 1, 1, 1, 1};
  // Declare empty ExampleStruct1
  ExampleStruct1 Example1;
  // Resets memory, removes garbage data.
  memset(&Example1, 0, sizeof(ExampleStruct1));
  // Initialize pointers to addresses of each cpp struct field
  void* field_addresses1[FIELDS_EXAMPLE1] = {
    (void*)&(Example1.list),
    (void*)&(Example1.size),
    (void*)&(Example1.letter),
    (void*)&(Example1.number),
    (void*)&(Example1.x),
    (void*)&(Example1.y)
  };
  // Declare MPI struct
  MPI_Datatype MPI_struct1;
  // Create and commit MPI struct
  create_MPI_struct(Example1, FIELDS_EXAMPLE1, field_addresses1, TYPES1, blocklengths1, MPI_struct1);

  // Data initialization for structs 2 and 3
  const int blocklengths2[FIELDS_EXAMPLE2] = {30, 1, 50, 1, 1, 1, 1};
  ExampleStruct2 Example2;
  memset(&Example2, 0, sizeof(ExampleStruct2));
  void* field_addresses2[FIELDS_EXAMPLE2] = {
    (void*)&(Example2.playerlist),
    (void*)&(Example2.playerlistSize),
    (void*)&(Example2.levelName),
    (void*)&(Example2.levelNameSize),
    (void*)&(Example2.randVar),
    (void*)&(Example2.gameBoardWidth),
    (void*)&(Example2.gameBoardHeight),
  };
  MPI_Datatype MPI_struct2;
  create_MPI_struct(Example2, FIELDS_EXAMPLE2, field_addresses2, TYPES2, blocklengths2, MPI_struct2);
  
  const int blocklengths3[FIELDS_EXAMPLE3] = {100, 1, 1, 1, 50, 1, 1};
  ExampleStruct3 Example3;
  memset(&Example3, 0, sizeof(ExampleStruct3));
  void* field_addresses3[FIELDS_EXAMPLE3] = {
    (void*)&(Example3.playerName),
    (void*)&(Example3.playerNameSize),
    (void*)&(Example3.playerHealth),
    (void*)&(Example3.playerMana),
    (void*)&(Example3.itemList),
    (void*)&(Example3.itemListSize),
    (void*)&(Example3.warewolfPercentage),
  };
  MPI_Datatype MPI_struct3;
  create_MPI_struct(Example3, FIELDS_EXAMPLE3, field_addresses3, TYPES3, blocklengths3, MPI_struct3);
  
  // Head node
  if (rank == src) {   
  // Initialize the empty structs created earlier (only on the head node)
    Example1 = {
      .list = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5},
      .size = 459,
      .letter = 'b',
      .number = 54.252,
      .x = 53,
      .y = 41
    };
    
    Example2 = {
      .playerlist = {0,1,2,3,4,5,6,7,8},
      .playerlistSize = 9,
      .levelName = {'l', 'e', 'v', 'e', 'l', '1'},
      .levelNameSize = 6,
      .randVar = 8.533,
      .gameBoardWidth = 45,
      .gameBoardHeight = 67
    };

    Example3 = {
      .playerName = {'b','r','i','a','n'},
      .playerNameSize = 5,
      .playerHealth = 12.3,
      .playerMana = 6.7,
      .itemList = {'l','i','s','t',' ','o','f',' ','i','t','e','m','s'},
      .itemListSize = 13,
      .warewolfPercentage = 67.3
    };

    // Send them
    MPI_Send(&Example1, 1, MPI_struct1, dest, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &Example1 << " to node "<< dest << std::endl;
    MPI_Send(&Example2, 1, MPI_struct2, dest, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &Example2 << " to node "<< dest << std::endl;
    MPI_Send(&Example3, 1, MPI_struct3, dest, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &Example3 << " to node "<< dest << std::endl;

    // New ExampleStructs to receive back new data from the other node.
    ExampleStruct1 received4;
    ExampleStruct2 received5;
    ExampleStruct3 received6;
    // Receive MPI structs, which are loaded into the c++ structs, then print them.
    MPI_Recv(&received4, 1, MPI_struct1, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct1 at address " << &received4 << " back from node " << src << std::endl;
    std::cout << received4;
    MPI_Recv(&received5, 1, MPI_struct2, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct2 at address " << &received5 << " back from node " << src << std::endl;
    std::cout << received5;
    MPI_Recv(&received6, 1, MPI_struct3, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct3 at address " << &received6 << " back from node " << src << std::endl;
    std::cout << received6;
  }

  // Destination node (node 1)
  if(rank == dest) {
    // Declare empty structs to be received
    ExampleStruct1 received1;
    ExampleStruct2 received2;
    ExampleStruct3 received3;
    // Receive MPI structs, which are loaded into the c++ structs, then print them.
    MPI_Recv(&received1, 1, MPI_struct1, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct1 at address " << &received1 << " from node " << src << std::endl;
    std::cout << received1;
    MPI_Recv(&received2, 1, MPI_struct2, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct2 at address " << &received2 << " from node " << src << std::endl;
    std::cout << received2;
    MPI_Recv(&received3, 1, MPI_struct3, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "> Received ExampleStruct3 at address " << &received3 << " from node " << src << std::endl;
    std::cout << received3;

    // Change the data inside the received structs.
    received1 = {
      .list = {1.5, 1.4, 1.3, 1.2, 1.1, 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1},
      .size = 23,
      .letter = 'z',
      .number = 86.915,
      .x = 3,
      .y = 91
    };
    
    received2 = {
      .playerlist = {573,774,285,321},
      .playerlistSize = 4,
      .levelName = {'m','y','l','e','v','e','l'},
      .levelNameSize = 7,
      .randVar = 23.724,
      .gameBoardWidth = 97,
      .gameBoardHeight = 63
    };

    received3 = {
      .playerName = {'l','o','i','s'},
      .playerNameSize = 4,
      .playerHealth = 464.3,
      .playerMana = 62.1,
      .itemList = {'i','t','e','m','1','i','t','e','m','2'},
      .itemListSize = 10,
      .warewolfPercentage = 2.1
    };

    // Send the received structs back to the head node with new data.
    MPI_Send(&received1, 1, MPI_struct1, src, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &received1 << " to node "<< src << std::endl;
    MPI_Send(&received2, 1, MPI_struct2, src, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &received2 << " to node "<< src << std::endl;
    MPI_Send(&received3, 1, MPI_struct3, src, 0, MPI_COMM_WORLD);
    std::cout << "> Node " <<rank<<" sent struct at address " << &received3 << " to node "<< src << std::endl;
  }
  MPI_Type_free(&MPI_struct1);
  MPI_Type_free(&MPI_struct2);
  MPI_Type_free(&MPI_struct3);
  MPI_Finalize();
}
