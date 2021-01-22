#include <iostream>
#include <fstream>
#include<string>
#include<cstdlib>
#include <sstream>
#include <cstring>
#include<time.h>
#include<math.h>
#include<ctime>
#include<vector>
#include <algorithm>
using namespace std;

int* randomPath(int citiesNum) {
    int* path = new int[citiesNum + 1];
    vector<int> tempNums;
    for (int i = 1; i < citiesNum; i++)
        tempNums.push_back(i);

    path[0] = 0;
    path[citiesNum] = 0;
    int currentVertex;
    vector<int> cleaningVector;
    for (int i = 1; i < citiesNum; i++) {
        currentVertex = rand() % tempNums.size();
        vector<int>::iterator it;
        it = tempNums.begin() + currentVertex;
        path[i] = tempNums.at(currentVertex);
        tempNums.erase(it);

    }
    return path;
}

void printPath(int arr[], int size) {

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            cout << arr[i] << endl;
        }
        else
            cout << arr[i] << " -> ";

    }
}
void copyArray(int* arrA, int* arrB, int size) {
    for (int i = 0; i < size; i++)
        arrA[i] = arrB[i];
}

int countCost(int* path, int citiesNum, int** cityMatrix) {
    int cost = 0;
    for (int i = 0; i < citiesNum; i++) {
        cost += cityMatrix[path[i]][path[i + 1]];
    }
    cost += cityMatrix[path[citiesNum]][path[0]];
    return cost;
}

void inversion_mutation(int* path, int citiesNum) {
    int firstIndex = rand() % (citiesNum-1) + 1;
    int secondIndex = rand() % (citiesNum - 1) + 1;
    if (firstIndex == secondIndex)
        inversion_mutation(path, citiesNum);
    else {
        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);
        //cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
        for (int low = firstIndex, high = secondIndex; low < high; low++, high--) {
            swap(path[low], path[high]);
        }

    }

}
bool contain(int* arr,int value, int start,int end) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}
int* order_crossover(int* firstPath,int* secondPath,int citiesNum) {
    int* newPath = new int[citiesNum + 1];
    newPath[0] = 0;
    newPath[citiesNum] = 0;
    int firstIndex = rand() % (citiesNum-1) + 1;
    int secondIndex = rand() % (citiesNum - 1) + 1;
    if (firstIndex == secondIndex)
        order_crossover(firstPath,secondPath, citiesNum);
    else {
        //firstIndex = 1;
        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);
        //cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
        // copied random area from firstPath to new path
        for (int i = firstIndex; i <= secondIndex; i++)
            newPath[i] = firstPath[i];


        bool copyTrue = true;
       // copyArray(newPath, firstPath, citiesNum + 1);
        int positionOfSecondPathIndex = secondIndex + 1;
        int positionOfFirstPathIndex = positionOfSecondPathIndex;
        if (firstIndex == 1 && secondIndex == citiesNum - 1)
            copyTrue = false;

        if (secondIndex == citiesNum - 1) {
            positionOfFirstPathIndex = 1;
            positionOfSecondPathIndex = 1;
        }


        while (copyTrue) {

            if (positionOfFirstPathIndex > firstIndex) {
                while (contain(firstPath, secondPath[positionOfSecondPathIndex], firstIndex, secondIndex)) {
                    positionOfSecondPathIndex++;
                    if (positionOfSecondPathIndex == citiesNum) {
                        positionOfSecondPathIndex= 1;
                    }
                }
                newPath[positionOfFirstPathIndex] = secondPath[positionOfSecondPathIndex];
            }else if (positionOfFirstPathIndex < firstIndex) {
                while (contain(firstPath, secondPath[positionOfSecondPathIndex], firstIndex, secondIndex)) {
                    positionOfSecondPathIndex++;
                    if (positionOfSecondPathIndex >= citiesNum) {
                        positionOfSecondPathIndex = 1;
                    }
                }
                newPath[positionOfFirstPathIndex] = secondPath[positionOfSecondPathIndex];
            }
            positionOfFirstPathIndex++;
            positionOfSecondPathIndex++;
            if (positionOfSecondPathIndex >= citiesNum) {
                positionOfSecondPathIndex = 1;
            }
            if (positionOfFirstPathIndex >= citiesNum)
                positionOfFirstPathIndex = 1;

            if (positionOfFirstPathIndex == firstIndex)
                copyTrue = false;

        }

    }

    return newPath;

}

double fitness_function(int* path,int citiesNum,int** cityMatrix) {
    double fitness_function_value = 1;
    fitness_function_value = fitness_function_value / double(countCost(path,citiesNum,cityMatrix));

    return fitness_function_value;
}

bool compare_arrays(int* arrA, int* arrB,int size) {
    for (int i = 0; i < size; i++) {
        if (arrA[i] != arrB[i])
            return false;
    }
    
    return true;
}

void tournament_selection_parents(int* firstPath, int* secondPath, vector<int*> population, int citiesNum, int** cityMatrix) {
    int firstIndex = rand() % population.size();
    int secondIndex = rand() % population.size();
    if (firstIndex == secondIndex)
        tournament_selection_parents(firstPath, secondPath, population, citiesNum, cityMatrix);
    else {
        /* selekcjonowanie 1 osobnika */
        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);
        //cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
        int* bestPath = new int[citiesNum + 1];
        copyArray(bestPath, population.at(firstIndex), citiesNum + 1);
        double best_fitness_value = fitness_function(bestPath, citiesNum, cityMatrix);
      

        for (int i = firstIndex; i <= secondIndex; i++) {
            if (fitness_function(population.at(i), citiesNum, cityMatrix) > best_fitness_value) {
                best_fitness_value = fitness_function(population.at(i), citiesNum, cityMatrix);
                copyArray(bestPath, population.at(i), citiesNum + 1);
            }
        }
        copyArray(firstPath, bestPath, citiesNum + 1);


        /* selekcjonowanie 2 osobnika */

        do {
            firstIndex = rand() % population.size();
            secondIndex = rand() % population.size();
        } while (firstIndex == secondIndex);

        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);

        //cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
        if (compare_arrays(population.at(firstIndex), firstPath, citiesNum + 1)) {
            copyArray(bestPath, population.at(firstIndex + 1), citiesNum + 1);
            best_fitness_value = fitness_function(bestPath, citiesNum, cityMatrix);
        }
        else {
            copyArray(bestPath, population.at(firstIndex), citiesNum + 1);
            best_fitness_value = fitness_function(bestPath, citiesNum, cityMatrix);
        }

        for (int i = firstIndex; i <= secondIndex; i++) {
            if (fitness_function(population.at(i), citiesNum, cityMatrix) > best_fitness_value && !compare_arrays(firstPath, population.at(i), citiesNum + 1)) {
                copyArray(bestPath, population.at(i), citiesNum + 1);
                best_fitness_value = fitness_function(bestPath, citiesNum, cityMatrix);
            }
        }
        copyArray(secondPath, bestPath, citiesNum + 1);

        delete[]bestPath;
    }
}
int* pass_arr(int* arr, int size) {
    int* new_arr = new int[size];
    copyArray(new_arr, arr, size);

    return new_arr;
}
/* do przetestowania */
vector<int*> tournament_selection(vector<int*> population,int citiesNum,int** cityMatrix) {
    vector<int*> next_generation_population;
    int temp_size = 0;
    int firstIndex, secondIndex;
    while (temp_size < citiesNum) {
        do {
            firstIndex = rand() % population.size();
            secondIndex = rand() % population.size();
        } while (firstIndex == secondIndex);

        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);

        int* bestPath = new int[citiesNum + 1];
        copyArray(bestPath, population.at(firstIndex), citiesNum + 1);
        double best_fitness_value = fitness_function(bestPath, citiesNum, cityMatrix);
        int index_of_path = firstIndex;

        for (int i = firstIndex; i <= secondIndex; i++) {
            if (fitness_function(population.at(i), citiesNum, cityMatrix) > best_fitness_value) {
                best_fitness_value = fitness_function(population.at(i), citiesNum, cityMatrix);
                copyArray(bestPath, population.at(i), citiesNum + 1);
                index_of_path = i;
            }
        }
        next_generation_population.push_back(pass_arr(bestPath,citiesNum+1));
        population.erase(population.begin() + index_of_path);

        temp_size++;
        delete[]bestPath;
    }
    return next_generation_population;
}

int main() {
    srand(time(NULL));
    fstream file;
    string tempLine;
    int hamiltonOpt;
    int citiesNum;
    string fileName;
    cout << "Name of instance : ";
    cin >> fileName;
    file.open(fileName, std::ios::in | std::ios::out);
    if (file.is_open()) {
        getline(file, tempLine);
        getline(file, tempLine);
        citiesNum = atoi(tempLine.c_str());
        int** cityMatrix = new int* [citiesNum];
        for (int i = 0; i < citiesNum; i++)
            cityMatrix[i] = new int[citiesNum];


        for (int i = 0; i < citiesNum; i++) {
            getline(file, tempLine);
            stringstream ss;
            ss << tempLine;
            int found;
            string temp;
            int y = 0;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    cityMatrix[i][y] = found;
                    y++;
                }
            }

            temp = "";
        }
        getline(file, tempLine);
        hamiltonOpt = atoi(tempLine.c_str());

        cout << "------------city matrix------------" << endl;

        for (int i = 0; i < citiesNum; i++) {
            for (int j = 0; j < citiesNum; j++)
                cout << *(*(cityMatrix + i) + j) << " ";
            cout << endl;
        }

        /*    deklarowanie zmiennych potrzebnych do wykonania algorytmu   */
        int* firstPath = new int[citiesNum + 1];
        int* secondPath = new int[citiesNum + 1];
        //for (int i = 0; i < citiesNum; i++) {
          //  path[i] = i;
            //secondPath[i] = i;
       // }
        //path[citiesNum] = 0;
        //secondPath[citiesNum] = 0;

        clock_t start = clock();
        int* newPath = new int[citiesNum + 1];
        newPath[citiesNum] = 0;
        newPath[0] = 0;
        int* bestPath = new int[citiesNum + 1];
        
        /*   tutaj zaczyna sie algorytm GA    */

        /* inicjalizacja populacji poczatkowej */

        int generations = 0;
        vector<int*>population;
        vector<int*> temp_population;
            for (int i = 0; i <citiesNum; i++) {
                population.push_back(randomPath(citiesNum));
            }
        double crossingRate = 0.8;
        double mutationRate = 0.01;
        int randomInteger;
        vector<int*> new_population;
        while (generations < 1) {
            /*          faza krzyzowan       */
            for (int i = 0; i < crossingRate*citiesNum; i++) {
                tournament_selection_parents(firstPath, secondPath, population, citiesNum, cityMatrix);
                population.push_back(order_crossover(firstPath, secondPath, citiesNum));
            }
            /*          faza mutacji        */ 
            for (int i = 0; i < mutationRate * citiesNum; i++) {
                inversion_mutation(population.at(rand() % citiesNum), citiesNum);
              //  cout << "i mutated!" << endl;
            }

            /* kopiuje aktualna populacje do tymczasowej listy/vectora*/
            //new_population.reserve(population.size());
           
            new_population = tournament_selection(population, citiesNum, cityMatrix);
            population.clear();
            population = new_population;
            cout << "generation :" << generations << endl;
            generations++;
       }
        cout << "------------first_generation-------------" << endl;
        for (int i = 0; i < population.size(); i++) {
            cout << "iteration :"<<i << endl;
            printPath(population.at(i), citiesNum + 1);
        }
        cout << "------------next_generation-------------" << endl;
        for (int i = 0; i < new_population.size(); i++) {
            cout << "iteration :" << i << endl;
            printPath(new_population.at(i), citiesNum + 1);
        }
       

        /*   tutaj konczy  sie algorytm GA      */

        clock_t stop = clock();

        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]" << endl;

        /*        zwalnianie pamieci               */
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        delete[]firstPath;
        delete[]newPath;
        delete[]secondPath;
        delete[]bestPath;
        cout << "end" << endl;
    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
}
