#include <iostream>
#include "mpi.h"
#include "omp.h"
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <sys/sysinfo.h>
#include <cstdlib>

// Prints the info for the head node.
// Arguments:
// size: Number of processes.
// node_name: name of current node.
void print_info_head(int size, char* node_name) {
  std::cout << "Number of processes in the cluster: " << size << "." << std::endl;
  std::cout << "Head node name: " << node_name << "." << std::endl;
  // Uses standard library to get number of cores.
  std::cout << "Number of cores in head node: " << std::thread::hardware_concurrency() << "." << std::endl;
  std::cout << "Number of nodes: (assuming each node has the same number of cores) " << size / std::thread::hardware_concurrency() << "." << std::endl;
  std::cout << "Core clock speed and model:" << std::endl;
  // Greps lscpu to get CPU model informaton.
  system("lscpu | grep 'Model name'");
  // Gets system information with sysinfo, converts RAM value into GB.
  struct sysinfo info;
  if (sysinfo(&info) == 0) {
    std::cout << "System RAM of the head node: " << info.totalram / std::pow(2, 30) << " GB." << std::endl;
  }
  else {
    std::cout << "Couldn't retrieve system info" << std::endl;
  }
}

// Prints the info for other nodes.
// Similar to print_info_head() without displaying total number of processes or total number of cores.
void print_info(int size, char* node_name){
  std::cout << "Node name: " << node_name << "." << std::endl;
  std::cout << "Number of cores in node: " << std::thread::hardware_concurrency() << "." << std::endl;
  struct sysinfo info;
  if (sysinfo(&info) == 0) {
    std::cout << "System RAM of node: " << info.totalram / std::pow(2, 30) << " GB." << std::endl;
  }
  else {
    std::cout << "Couldn't retrieve system info" << std::endl;
  }
  std::cout << "Core clock speed and model:" << std::endl;
  system("lscpu | grep 'Model name'");
}

int main(int argc, char** argv) {
  
  // Initialize the MPI environment.
  // Define a list of the node names, the current node rank, number of processes, and the number to be sent.
  MPI_Init(NULL, NULL);
  char node_name[MPI_MAX_PROCESSOR_NAME];
  int rank,size, namelen;
  int send_num = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Get_processor_name(node_name, &namelen);
  
  // Remove garbage data from memory where the node names are.
  memset(node_name+namelen,0,MPI_MAX_PROCESSOR_NAME-namelen);
  // Declare head node as node0 (this is making an assumption).
  int src = 0; 
  // Head node.
  if (rank == src) { 
    // Print info for head node  .
    print_info_head(size, node_name);
  
    // Send and receive so that information is printed in the correct order.
    // Sends to the next node number.
    if (size > 1) {
      MPI_Send(&send_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
  }
  else {
    // Receives from the previous node number.
    MPI_Recv(&send_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Number is arbitrary, just to send something (nullptr results in an error).
    std::cout << "> Number " << send_num << " received by " << node_name << std::endl;
    // Print info.
    print_info(size, node_name);
    // Send to next node number.
    if (rank < size - 1) {
      MPI_Send(&send_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
}

