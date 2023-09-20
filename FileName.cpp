#include <iostream>
#include <vector>
#include <mpi.h>

//int main(int argc, char* argv[]) {
//    MPI_Init(&argc, &argv);
//
//    int rank, size;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    int N;
//    std::vector<double> data;
//
//    if (rank == 0) {
//        // В главном процессе введите N и данные в зависимости от количества процессов
//        if (size % 2 == 0) {
//            N = size; // Если четное количество процессов, используйте вещественные числа
//            for (int i = 0; i < N; ++i) {
//                data.push_back(static_cast<double>(i));
//            }
//        }
//        else {
//            N = size; // Если нечетное количество процессов, используйте целые числа
//            for (int i = 0; i < N; ++i) {
//                data.push_back(static_cast<int>(i));
//            }
//        }
//    }
//
//    // Рассылка данных из главного процесса
//    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    data.resize(N);
//    MPI_Bcast(data.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//
//    // Найти минимальный или максимальный элемент в зависимости от ранга процесса
//    double result;
//    if (rank % 2 == 0) {
//        // Четные ранги (главный процесс и другие с четными номерами) находят минимум
//        result = *std::min_element(data.begin(), data.end());
//    }
//    else {
//        // Нечетные ранги находят максимум
//        result = *std::max_element(data.begin(), data.end());
//    }
//
//    // Вывод результата в каждом процессе
//    std::cout << "Process " << rank << ": " << result << std::endl;
//
//    MPI_Finalize();
//    return 0;
//}

//
//int main(int argc, char* argv[]) {
//    MPI_Init(&argc, &argv);
//
//    int rank, size;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    int N = 0;
//    std::vector<double> data;
//
//    if (rank == 0) {
//        // Главный процесс
//        std::vector<std::pair<int, std::vector<double>>> received_data;
//
//        // Принимаем данные от всех подчиненных процессов с MPI_ANY_SOURCE
//        for (int i = 1; i < size; ++i) {
//            MPI_Recv(&N, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            if (N > 0) {
//                data.resize(N);
//                MPI_Recv(data.data(), N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//                received_data.emplace_back(i, data);
//            }
//        }
//
//        // Выводим данные и ранг процесса, отправившего их
//        for (const auto& entry : received_data) {
//            std::cout << "Процесс " << entry.first << " отправил данные: ";
//            for (double val : entry.second) {
//                std::cout << val << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//    else {
//        // Подчиненные процессы
//        if (rank % 2 != 0) {
//            N = 5; // Задаем N (здесь, например, равное 5) для нечетных рангов
//            data.resize(N);
//            for (int i = 0; i < N; ++i) {
//                data[i] = static_cast<double>(rank * 100 + i); // Пример данных
//            }
//
//            // Отправляем данные в главный процесс
//            MPI_Send(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//            MPI_Send(data.data(), N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
//        }
//    }
//
//    MPI_Finalize();
//    return 0;
//}


int main(int argc, char* argv[])
{
	int myid, numprocs;
	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);
	if (myid == 0) {
		int M;
		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &M);
		//MPI_Recv(&M, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		int* buf = (int*)malloc(M * sizeof(int));
		MPI_Recv(buf, M, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		int R = status.MPI_SOURCE;
		fprintf(stdout, "process %d recived %d numbers from process %d \n", myid, M, R);
		for (int i = 0; i < M; i++)
			fprintf(stdout, "%d\n", *(buf + i));
		fflush(stdout);
	}
	else {
		if (myid == 2) {
			int N;
			srand(myid);
			N = (int)(8 * rand() / RAND_MAX + 1);
			int* A = (int*)malloc(N * sizeof(int));
			for (int i = 0; i < N; i++)
				*(A + i) = rand();
			//MPI_Send(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(A, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}