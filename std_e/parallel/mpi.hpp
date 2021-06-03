in#pragma once


#include <limits>
#include <mpi.h>
#include <vector>
#include <numeric>
#include "std_e/utils/tuple.hpp"
#include "std_e/parallel/mpi_exception.hpp"
#include "std_e/future/make_array.hpp"
#include "std_e/future/contract.hpp"

#include <iostream>

namespace std_e {


// to_mpi_type {
template<class T> constexpr auto
to_mpi_type__impl() -> MPI_Datatype {
  constexpr int sz = sizeof(T);
  static_assert(sz==1 || sz==2 || sz==4 || sz==8,"no corresponding MPI type");
       if constexpr (sz==1) return MPI_BYTE;
  else if constexpr (sz==2) return MPI_INT16_T;
  else if constexpr (sz==4) return MPI_INT32_T;
  else if constexpr (sz==8) return MPI_INT64_T;
}

template<class T> constexpr MPI_Datatype to_mpi_type = to_mpi_type__impl<T>();
// to_mpi_type }


inline auto
rank(MPI_Comm comm) -> int {
  int i;
  MPI_Comm_rank(comm, &i);
  return i;
}
inline auto
nb_ranks(MPI_Comm comm) -> int {
  int n;
  MPI_Comm_size(comm, &n);
  return n;
}

/// Can be called before MPI_Init (useful for initializing globals because MPI_Init cannot be called before main())
inline auto
mpi_comm_world_rank() -> int {
  #if defined(OPEN_MPI)
    const char* rank_str = std::getenv("OMPI_COMM_WORLD_RANK");
  #elif defined(I_MPI_VERSION) || defined(MPI_VERSION)
    const char* rank_str = std::getenv("MPI_LOCALRANKID"); // env name used at least by Intel MPI
  #else
    #error "Unknown MPI implementation"
  #endif
  if (rank_str==nullptr) return 0; // not launched with mpirun/mpiexec...
  return std::stoi(rank_str);
}


template<class T> auto
min_global(T local_min, MPI_Comm comm) -> T {
  T global_min = std::numeric_limits<T>::max();

  int err = MPI_Allreduce(&local_min, &global_min, 1, to_mpi_type<T>, MPI_MIN, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return global_min;
}
template<class T> auto
max_global(T local_max, MPI_Comm comm) -> T {
  T global_max = std::numeric_limits<T>::min();

  int err = MPI_Allreduce(&local_max, &global_max, 1, to_mpi_type<T>, MPI_MAX, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return global_max;
}
template<class T> auto
minmax_global(T local_min, T local_max, MPI_Comm comm) -> std::pair<T,T> {
  return {min_global(local_min,comm),max_global(local_max,comm)};
}


template<class T> auto
all_gather(T value, T* rbuf, MPI_Comm comm) -> void {
  int err = MPI_Allgather(&value, 1, to_mpi_type<T>,
                          rbuf  , 1, to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}

template<class T> auto
all_gather(T value, MPI_Comm comm) -> std::vector<T> {

  int comm_size;
  MPI_Comm_size(comm, &comm_size);

  std::vector<T> data_received(comm_size);

  int err = MPI_Allgather(&value, 1, to_mpi_type<T>,
                          data_received.data(), 1, to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return data_received;
}

template<class T> auto
all_gather(const std::vector<T> & value,MPI_Comm comm) -> std::vector<T> {
  
  int comm_size;
  MPI_Comm_size(comm, &comm_size);

  int sz = value.size();
  std::vector<int> recvcounts = std_e::all_gather(sz,comm);
  
  int total_sz = std::reduce(recvcounts.begin(),recvcounts.end());
  std::vector<T> data_received(total_sz);

  std::vector<int> displs(comm_size);
  std::partial_sum(recvcounts.begin(),recvcounts.end(),displs.begin());
  displs.insert(displs.begin(),0);
  displs.pop_back();

  int err = MPI_Allgatherv(value.data(), value.size(), to_mpi_type<T>, data_received.data(), recvcounts.data(), displs.data(),
                           to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return data_received;
}

template<class T> auto
all_reduce(const T& value, MPI_Op op, MPI_Comm comm) -> T {
  T res;
  int err = MPI_Allreduce(&value, &res, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return res;
}


} // std_e
