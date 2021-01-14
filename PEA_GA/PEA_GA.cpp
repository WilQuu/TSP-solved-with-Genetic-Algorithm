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
using namespace std;

void randomPath(int* path, int citiesNum) {
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
        cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
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
void order_crossover(int* firstPath,int* secondPath,int* newPath,int citiesNum) {
    int firstIndex = rand() % (citiesNum-1) + 1;
    int secondIndex = rand() % (citiesNum - 1) + 1;
    if (firstIndex == secondIndex)
        order_crossover(firstPath,secondPath,newPath, citiesNum);
    else {
        //firstIndex = 1;
        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);
        cout << firstIndex << " <-first index--second index ->" << secondIndex << endl;
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
        int* path = new int[citiesNum + 1];
        int* secondPath = new int[citiesNum + 1];
        for (int i = 0; i < citiesNum; i++) {
            path[i] = i;
            secondPath[i] = i;
        }
        path[citiesNum] = 0;
        secondPath[citiesNum] = 0;

        clock_t start = clock();
        int* newPath = new int[citiesNum + 1];
        newPath[citiesNum] = 0;
        newPath[0] = 0;
        /*   tutaj zaczyna sie algorytm GA    */
        for (int i = 0; i < 200; i++) {
            cout << i << " : iteration" << endl;
            cout << "first path below :" << endl;
            printPath(path, citiesNum + 1);
            randomPath(secondPath, citiesNum);
            cout << "second path below :" << endl;
            printPath(secondPath, citiesNum + 1);
            cout << "new path below :" << endl;
            order_crossover(path, secondPath, newPath, citiesNum);
            printPath(newPath, citiesNum + 1);
        }   

        /*   tutaj konczy  sie algorytm GA      */

        clock_t stop = clock();

        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]" << endl;

        /*        zwalnianie pamieci               */
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        delete[]path;
        delete[]newPath;
        delete[]secondPath;
        cout << "end" << endl;
    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
}
