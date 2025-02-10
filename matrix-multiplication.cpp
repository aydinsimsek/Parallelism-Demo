#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>

void sequentialMatrixMultiply(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C,
                              unsigned int num_rows_a, unsigned int num_cols_a, unsigned int num_cols_b)
{
    for(unsigned int i = 0; i < num_rows_a; i++)
    {
        for(size_t j = 0; j < num_cols_b; j++)
        {
			C[i][j] = 0; 
            for(size_t k = 0; k < num_cols_a; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
		}
	}
}

void calculateResultByChunks(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C, 
                             unsigned int startRow, unsigned int endRow, unsigned int num_cols_a, unsigned int num_cols_b)
{
    for(unsigned int i = startRow; i < endRow; i++)
    {
        for(unsigned int j = 0; j < num_cols_b; j++)
        {
            unsigned int res = 0;
            for(unsigned int k = 0; k < num_cols_a; ++k)
            {
                res += A[i][k] * B[k][j];
            }
            C[i][j] = res;
        }
    }
}

void parallelMatrixMultiply(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &C,
                            unsigned int num_rows_a, unsigned int num_cols_a, unsigned int num_cols_b) 
{
    unsigned int numThreads = std::thread::hardware_concurrency(); 
    std::vector<std::thread> threads;
    unsigned int rowsPerThread = num_rows_a / numThreads;
    unsigned int numExtraRows = num_rows_a % numThreads;
    unsigned int startRow = 0;
    for(unsigned int i = 0; i < numThreads; i++)
    {
        unsigned int endRow = startRow + rowsPerThread + (i < numExtraRows ? 1 : 0);
        threads.push_back(std::thread(calculateResultByChunks, std::ref(A), std::ref(B), std::ref(C), startRow, endRow, num_cols_a, num_cols_b));
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
	const unsigned int NUM_ROWS_A = 1000;
	const unsigned int NUM_COLS_A = 1000;
	const unsigned int NUM_ROWS_B = NUM_COLS_A;
	const unsigned int NUM_COLS_B = 1000;

    std::vector<std::vector<int>> A(NUM_ROWS_A, std::vector<int>(NUM_COLS_A));
    std::vector<std::vector<int>> B(NUM_ROWS_B, std::vector<int>(NUM_COLS_B));
    std::vector<std::vector<int>> sequentialResult(NUM_ROWS_A, std::vector<int>(NUM_COLS_B));
    std::vector<std::vector<int>> parallelResult(NUM_ROWS_A, std::vector<int>(NUM_COLS_B));

    for(unsigned int i = 0; i < NUM_ROWS_A; i++)
    {
        for(unsigned int j = 0; j < NUM_COLS_A; j++)
        {
            A[i][j] = rand() % 100 + 1;
        }
    }
    for(unsigned int i = 0; i < NUM_ROWS_B; i++)
    {
        for(unsigned int j = 0; j < NUM_COLS_B; j++)
        {
            B[i][j] = rand() % 100 + 1;
        }
    }

    std::cout << "Evaluating Sequential Implementation..." << std::endl;
	std::chrono::duration<double, std::milli> sequentialDuration(0);
    double averageSequentialDuration = 0.0;
    sequentialMatrixMultiply(A, B, sequentialResult, NUM_ROWS_A, NUM_COLS_A, NUM_COLS_B); 
    for(unsigned int i = 0; i < NUM_EVAL_RUNS; i++)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        sequentialMatrixMultiply(A, B, sequentialResult, NUM_ROWS_A, NUM_COLS_A, NUM_COLS_B);
        sequentialDuration += std::chrono::high_resolution_clock::now() - startTime;
    }
    averageSequentialDuration = sequentialDuration.count() / NUM_EVAL_RUNS;

    std::cout << "Evaluating Parallel Implementation..." << std::endl;
	std::chrono::duration<double, std::milli> parallelDuration(0);
    double averageParallelDuration = 0.0;
    parallelMatrixMultiply(A, B, parallelResult, NUM_ROWS_A, NUM_COLS_A, NUM_COLS_B); 
    for(unsigned i = 0; i < NUM_EVAL_RUNS; i++)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        parallelMatrixMultiply(A, B, parallelResult, NUM_ROWS_A, NUM_COLS_A, NUM_COLS_B);
        parallelDuration += std::chrono::high_resolution_clock::now() - startTime;
    }
    averageParallelDuration = parallelDuration.count() / NUM_EVAL_RUNS;
	
    for(unsigned int i = 0; i < NUM_ROWS_A; i++)
    {
        for(unsigned int j = 0; j < NUM_COLS_B; j++)
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