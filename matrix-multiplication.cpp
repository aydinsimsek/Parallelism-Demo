#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>

constexpr size_t NUM_ROWS_A = 1000;
constexpr size_t NUM_COLS_A = 1000;
constexpr size_t NUM_ROWS_B = NUM_COLS_A;
constexpr size_t NUM_COLS_B = 1000;

void sequentialMatrixMultiply(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C)
{
    for(size_t i = 0; i < NUM_ROWS_A; i++)
    {
        for(size_t j = 0; j < NUM_COLS_B; j++)
        {
			C[i][j] = 0; 
            for(size_t k = 0; k < NUM_COLS_A; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
		}
	}
}

void calculateResultByChunks(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C, 
                             size_t startRow, size_t endRow)
{
    for(size_t i = startRow; i < endRow; i++)
    {
        for(size_t j = 0; j < NUM_COLS_B; j++)
        {
            int res = 0;
            for(size_t k = 0; k < NUM_COLS_A; ++k)
            {
                res += A[i][k] * B[k][j];
            }
            C[i][j] = res;
        }
    }
}

void parallelMatrixMultiply(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C) 
{
    unsigned int numThreads = std::thread::hardware_concurrency(); 
    std::vector<std::thread> threads;
    unsigned int rowsPerThread = NUM_ROWS_A / numThreads;
    unsigned int numExtraRows = NUM_ROWS_A % numThreads;
    size_t startRow = 0;
    for(unsigned int i = 0; i < numThreads; i++)
    {
        size_t endRow = startRow + rowsPerThread + (i < numExtraRows ? 1 : 0);
        threads.push_back(std::thread(calculateResultByChunks, std::ref(A), std::ref(B), std::ref(C), startRow, endRow));
        startRow = endRow; 
    }
    for(auto &thread : threads)
    {
        thread.join();
    }
}

int main()
{
    srand(time(NULL));
	const unsigned int NUM_EVAL_RUNS = 3;

    std::vector<std::vector<int>> A(NUM_ROWS_A, std::vector<int>(NUM_COLS_A));
    std::vector<std::vector<int>> B(NUM_ROWS_B, std::vector<int>(NUM_COLS_B));
    std::vector<std::vector<int>> sequentialResult(NUM_ROWS_A, std::vector<int>(NUM_COLS_B));
    std::vector<std::vector<int>> parallelResult(NUM_ROWS_A, std::vector<int>(NUM_COLS_B));

    for(size_t i = 0; i < NUM_ROWS_A; i++)
    {
        for(size_t j = 0; j < NUM_COLS_A; j++)
        {
            A[i][j] = rand() % 100 + 1;
        }
    }
    for(size_t i = 0; i < NUM_ROWS_B; i++)
    {
        for(size_t j = 0; j < NUM_COLS_B; j++)
        {
            B[i][j] = rand() % 100 + 1;
        }
    }

    std::cout << "Evaluating Sequential Implementation..." << std::endl;
	std::chrono::duration<double, std::milli> sequentialDuration(0);
    double averageSequentialDuration = 0.0;
    sequentialMatrixMultiply(A, B, sequentialResult); 
    for(unsigned int i = 0; i < NUM_EVAL_RUNS; i++)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        sequentialMatrixMultiply(A, B, sequentialResult);
        sequentialDuration += std::chrono::high_resolution_clock::now() - startTime;
    }
    averageSequentialDuration = sequentialDuration.count() / NUM_EVAL_RUNS;

    std::cout << "Evaluating Parallel Implementation..." << std::endl;
	std::chrono::duration<double, std::milli> parallelDuration(0);
    double averageParallelDuration = 0.0;
    parallelMatrixMultiply(A, B, parallelResult); 
    for(unsigned i = 0; i < NUM_EVAL_RUNS; i++)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        parallelMatrixMultiply(A, B, parallelResult);
        parallelDuration += std::chrono::high_resolution_clock::now() - startTime;
    }
    averageParallelDuration = parallelDuration.count() / NUM_EVAL_RUNS;
	
    for(size_t i = 0; i < NUM_ROWS_A; i++)
    {
        for(size_t j = 0; j < NUM_COLS_B; j++)
        {
            if(sequentialResult[i][j] != parallelResult[i][j])
            {
                std::cerr << "ERROR: Result mismatch betweem sequential and parallel executions!" << std::endl;
                return -1; 
            }
        }
    }
    std::cout << "Average Sequential Execution Duration: " << averageSequentialDuration << " ms" << std::endl;
    std::cout << "Average Parallel Execution Duration: " << averageParallelDuration << " ms" << std::endl;
    std::cout << "Parallel execution is " << std::fixed << std::setprecision(2) << averageSequentialDuration / averageParallelDuration << " times faster than sequential execution!" << std::endl;
    return 0; 
}