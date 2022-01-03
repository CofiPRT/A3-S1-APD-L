#include <mpi.h>
#include <vector>

enum State { CANDIDATE, LEADER, LOST };

int main(int argc, char* argv[]) {
	int  numtasks, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<int> list = { rank };
	State state = CANDIDATE;

	int recv_val, val = rank;
	MPI_Status status;

	MPI_Send(&val, 1, MPI_INT, (rank + 1) % numtasks, 0, MPI_COMM_WORLD);
	MPI_Recv(&recv_val, 1, MPI_INT, rank == 0 ? numtasks - 1 : rank - 1, 0, MPI_COMM_WORLD, &status);

	val = recv_val;


	while (recv_val != rank) {
		list.push_back(recv_val);

		MPI_Send(&val, 1, MPI_INT, (rank + 1) % numtasks, 0, MPI_COMM_WORLD);
		MPI_Recv(&recv_val, 1, MPI_INT, rank == 0 ? numtasks - 1 : rank - 1, 0, MPI_COMM_WORLD, &status);

		val = recv_val;
	}

	int max_value = -1;
	for (auto value : list)
		max_value = std::max(max_value, value);

	state = rank == max_value ? LEADER : LOST;

	printf("Node %d is %s\n", rank, (state == LEADER ? "LEADER" : "LOST"));

	MPI_Finalize();

	return 0;
}