#include <vector>
#include <complex>
#include <iostream>
#include <fstream>
#include <time.h>
#include <thread>

const int SIZE = 500;
const int RESOLUTION = 20;
std::vector<std::vector<int>> image(SIZE);

std::vector<int> generateRow(int r)
{
	std::vector<int> v;
	for (int col = 0; col < SIZE; col++)
	{
		std::complex<double> Z(0, 0);
		std::complex<double> C((double)col*3.5/SIZE-2.5, (double)r*2/SIZE - 1);

		int itr = 0;
		while (std::abs(Z) < 2 && itr <= RESOLUTION) {
			Z = Z*Z+C;
			itr++;
		}
		v.push_back(itr);
		v.push_back(itr*2);
		v.push_back(itr);
	}
	return v;
}

void generateChunk(int chunkID, int chunkSize)
{
	for (int i = 0; i < chunkSize; i++) {
		int index = chunkID * chunkSize + i;
		if (index < (int)image.size())
			image[index] = generateRow(index);
	}
}

void draw() {
	std::cout<<"Drawing..."<<std::endl;

	std::ofstream out;
	out.open("image.ppm");
	out << "P3\n" << SIZE << " " << SIZE << "\n" << RESOLUTION << "\n";

	time_t s = clock();
	for (int r = 0; r < SIZE; r++){
		for (int c = 0; c < SIZE; c++){
			for (int digit = 0; digit < 3; digit++)		
				out << image.at(r).at(3*c+digit) << " ";			
		}
		out << "\n";
	}
	std::cout << "Drawn in " << (double)(clock() - s)/CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout<<"IMAGE READY!"<<std::endl;
	std::cin.get();
}

void singleThread() {
	time_t s = clock();
	for (int row = 0; row < SIZE; row++)	
		image[row] = (generateRow(row));
	std::cout << "Generated in " << (double)(clock() - s)/CLOCKS_PER_SEC << " seconds" << std::endl;
}

void multiThread(int threads) {
	std::vector<std::thread> threadv;

	time_t s = clock();
	for (int i = 0; i < threads; i++)	
		threadv.emplace_back([=](){generateChunk(i, ceil((double)SIZE / threads));});

	std::cout<<"Joining " << threads << " threads..."<<std::endl;
	for (int i = 0; i < threads; i++)
		threadv.at(i).join();

	std::cout << "Generated in " << (double)(clock() - s)/CLOCKS_PER_SEC << " seconds" << std::endl;
}


int main(int argc, char** argv)
{
	if (argc >= 2)
		multiThread(atoi(argv[1]));
	else
		singleThread();
	draw();
}