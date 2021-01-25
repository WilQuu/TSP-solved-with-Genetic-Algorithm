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

vector<unsigned> random_path(int citiesNum) {
    vector<unsigned> random_path;
    vector<int> tempNums;
    for (int i = 1; i < citiesNum; i++)
        tempNums.push_back(i);

    random_path.push_back(0);
    int currentVertex;
    for (int i = 1; i < citiesNum; i++) {
        currentVertex = rand() % tempNums.size();
        vector<int>::iterator it;
        it = tempNums.begin() + currentVertex;
        random_path.push_back(tempNums.at(currentVertex));
        tempNums.erase(it);

    }
    random_path.push_back(0);
    return random_path;
}
void print_path(vector<unsigned> path) {

    for (int i = 0; i < path.size(); i++) {
        if (i == path.size() - 1) {
            cout << path.at(i) << endl;
        }
        else
            cout << path.at(i) << " -> ";

    }
}

int count_cost(vector<unsigned> path, int** cityMatrix) {
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        cost += cityMatrix[path.at(i)][path.at(i + 1)];
    }

    cost += cityMatrix[path.at(path.size() - 1)][path.at(0)];
    return cost;
}

void inversion_mutation(vector<unsigned>& path) {
    int firstIndex, secondIndex;
    do {
        firstIndex = rand() % (path.size() - 2) + 1;
        secondIndex = rand() % (path.size() - 2) + 1;
    } while (firstIndex == secondIndex);

    if (firstIndex > secondIndex)
        swap(firstIndex, secondIndex);

    for (int low = firstIndex, high = secondIndex; low < high; low++, high--) {
        swap(path[low], path[high]);
    }
}

void swap_mutation(vector<unsigned>& path) {
    int firstIndex, secondIndex;
    do {
        firstIndex = rand() % (path.size() - 2) + 1;
        secondIndex = rand() % (path.size() - 2) + 1;
    } while (firstIndex == secondIndex);

    if (firstIndex > secondIndex)
        swap(firstIndex, secondIndex);

    swap(path.at(firstIndex), path.at(secondIndex));
}
bool contain(vector<unsigned> vec, int value, int start, int end) {
    for (int i = start; i <= end; i++) {
        if (vec.at(i) == value) {
            return true;
        }
    }
    return false;
}

vector<unsigned>order_crossover(vector<unsigned> first_path, vector<unsigned> second_path) {
    vector<unsigned> new_path = random_path(first_path.size() - 1);
    int firstIndex, secondIndex;
    do {
        firstIndex = rand() % (first_path.size() - 2) + 1;
        secondIndex = rand() % (first_path.size() - 2) + 1;
    } while (firstIndex == secondIndex);

    if (firstIndex > secondIndex)
        swap(firstIndex, secondIndex);

    // copied random area from firstPath to new path
    for (int i = firstIndex; i <= secondIndex; i++)
        new_path.at(i) = first_path.at(i);


    bool copyTrue = true;
    int positionOfSecondPathIndex = secondIndex + 1;
    int positionOfFirstPathIndex = positionOfSecondPathIndex;
    if (firstIndex == 1 && secondIndex == first_path.size() - 2)
        copyTrue = false;

    if (secondIndex == first_path.size() - 2) {
        positionOfFirstPathIndex = 1;
        positionOfSecondPathIndex = 1;
    }


    while (copyTrue) {

        if (positionOfFirstPathIndex > firstIndex) {
            while (contain(first_path, second_path[positionOfSecondPathIndex], firstIndex, secondIndex)) {
                positionOfSecondPathIndex++;
                if (positionOfSecondPathIndex == first_path.size() - 1) {
                    positionOfSecondPathIndex = 1;
                }
            }
            new_path[positionOfFirstPathIndex] = second_path[positionOfSecondPathIndex];
        }
        else if (positionOfFirstPathIndex < firstIndex) {
            while (contain(first_path, second_path[positionOfSecondPathIndex], firstIndex, secondIndex)) {
                positionOfSecondPathIndex++;
                if (positionOfSecondPathIndex >= first_path.size() - 1) {
                    positionOfSecondPathIndex = 1;
                }
            }
            new_path[positionOfFirstPathIndex] = second_path[positionOfSecondPathIndex];
        }
        positionOfFirstPathIndex++;
        positionOfSecondPathIndex++;
        if (positionOfSecondPathIndex >= first_path.size() - 1) {
            positionOfSecondPathIndex = 1;
        }
        if (positionOfFirstPathIndex >= first_path.size() - 1)
            positionOfFirstPathIndex = 1;

        if (positionOfFirstPathIndex == firstIndex)
            copyTrue = false;

    }



    return new_path;

}

double fitness_function(vector<unsigned> path, int** cityMatrix) {
    double fitness_function_value = 1;
    fitness_function_value = fitness_function_value / double(count_cost(path, cityMatrix));

    return fitness_function_value;
}

bool compare_paths(vector<unsigned> vec_a, vector<unsigned> vec_b) {
    for (int i = 0; i < vec_a.size(); i++) {
        if (vec_a.at(i) != vec_b.at(i))
            return false;
    }

    return true;
}
bool if_exists(vector<vector<unsigned>> population, vector<unsigned> path) {
    for (int i = 0; i < population.size(); i++) {
        if (compare_paths(population.at(i), path))
            return true;
    }
    return false;
}
void tournament_selection_parents(vector<unsigned>& first_path, vector<unsigned>& second_path, vector<vector<unsigned>>& population, int** cityMatrix) {
    int firstIndex, secondIndex;
    do {
        firstIndex = rand() % population.size();
        secondIndex = rand() % population.size();
    } while (firstIndex == secondIndex);

    if (firstIndex > secondIndex)
        swap(firstIndex, secondIndex);

    /* selekcjonowanie 1 osobnika */
    vector<unsigned> best_path;
    best_path = population.at(firstIndex);
    double best_fitness_value = fitness_function(best_path, cityMatrix);
    int index_of_path_in_population = firstIndex;
    for (int i = firstIndex; i <= secondIndex; i++) {
        if (fitness_function(population.at(i), cityMatrix) > best_fitness_value) {
            best_fitness_value = fitness_function(population.at(i), cityMatrix);
            best_path = population.at(i);
            index_of_path_in_population = i;
        }
    }
    first_path = best_path;
    //population.erase(population.begin() + index_of_path_in_population);


    /* selekcjonowanie 2 osobnika */

    do {
        firstIndex = rand() % population.size();
        secondIndex = rand() % population.size();
    } while (firstIndex == secondIndex);

    if (firstIndex > secondIndex)
        swap(firstIndex, secondIndex);

    if (compare_paths(population.at(firstIndex), first_path)) {
        best_path = population.at(firstIndex + 1);
        best_fitness_value = fitness_function(best_path, cityMatrix);
        index_of_path_in_population = firstIndex + 1;
    }
    else {
        best_path = population.at(firstIndex);
        best_fitness_value = fitness_function(best_path, cityMatrix);
        index_of_path_in_population = firstIndex;
    }

    for (int i = firstIndex; i <= secondIndex; i++) {
        if (fitness_function(population.at(i), cityMatrix) > best_fitness_value && !compare_paths(first_path, population.at(i))) {
            best_fitness_value = fitness_function(population.at(i), cityMatrix);
            best_path = population.at(i);
            index_of_path_in_population = i;
        }
    }
    second_path = best_path;
    //population.erase(population.begin() + index_of_path_in_population);
    //cout << "i work " << endl;
}


bool time_stop(clock_t start_time, clock_t now_time, double max_time) {
    double  elapsed = (double)(now_time - start_time) / CLOCKS_PER_SEC;
    if (max_time > elapsed)
        return true;
    else
        return false;
}

vector<vector<unsigned>> tournament_selection(vector<vector<unsigned>>& population, int** cityMatrix, int citiesNum) {
    vector<vector<unsigned>> next_generation_population;
    int temp_size = 0;
    int firstIndex, secondIndex, index_of_path;
    double best_fitness_value;

    while (temp_size < citiesNum) {
        do {
            firstIndex = rand() % population.size();
            secondIndex = rand() % population.size();
        } while (firstIndex == secondIndex);

        if (firstIndex > secondIndex)
            swap(firstIndex, secondIndex);

        vector<unsigned> best_path;
        best_path = population.at(firstIndex);
        best_fitness_value = fitness_function(best_path, cityMatrix);
        index_of_path = firstIndex;

        for (int i = firstIndex; i <= secondIndex; i++) {
            if (fitness_function(population.at(i), cityMatrix) > best_fitness_value) {
                best_fitness_value = fitness_function(population.at(i), cityMatrix);
                best_path = population.at(i);
                index_of_path = i;
            }
        }
        if (!if_exists(next_generation_population, best_path)) {
            next_generation_population.push_back(best_path);
            
        }
        else
            temp_size--;

        population.erase(population.begin() + index_of_path);

        temp_size++;
    }

    return next_generation_population;
}
vector<unsigned> get_best_path(vector<vector<unsigned>> population, int** cityMatrix) {
    vector<unsigned> best_path;
    best_path = population.at(0);
    for (int i = 1; i < population.size(); i++) {
        if (fitness_function(best_path, cityMatrix) < fitness_function(population.at(i), cityMatrix))
            best_path = population.at(i);
    }
    return best_path;
}
void sort_vector(vector<vector<unsigned>>& vec, int** cityMatrix) {
    sort(vec.begin(), vec.end(),
        [&](const std::vector<unsigned>& a, const std::vector<unsigned>& b) {
            return fitness_function(a, cityMatrix) > fitness_function(b, cityMatrix);
        });
}
vector<vector<unsigned>> custom_selection(vector<vector<unsigned>>& population, int** cityMatrix, int population_size) {
    vector<vector<unsigned>> new_pop;
    sort_vector(population, cityMatrix);
    population.resize(population_size);
    new_pop = population;
    return new_pop;
}
vector<vector<unsigned>> selection(vector<vector<unsigned>>& population, int** cityMatrix, int population_size, int sel_type) {
    if (sel_type == 0)
        return tournament_selection(population, cityMatrix, population_size);
    else if (sel_type == 1)
        return custom_selection(population, cityMatrix, population_size);
}
void mutation(vector<unsigned>& path, int sel_type) {
    if (sel_type == 0)
        inversion_mutation(path);
    else if (sel_type == 1)
        swap_mutation(path);
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
        cout << "Max time value [s] :";
        double max_time;
        cin >> max_time;
        cout << "mutation :" << endl;
        cout << "0 - inversion_mutation" << endl;
        cout << "1 - swap_mutation" << endl;
        int mutation_choice;
        do {
            cout << "enter mutation type :";
            cin >> mutation_choice;
        } while (mutation_choice != 0 && mutation_choice != 1);
        cout << "selection :" << endl;
        cout << "0 - tournament_selection " << endl;
        cout << "1 - custom_selection " << endl;
        int selection_choice;
        do {
            cout << "enter selection type :";
            cin >> selection_choice;
        } while (selection_choice != 0 && selection_choice != 1);
        cout << "starting algorithm ..." << endl;
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

        int iterations = 0;
        double PRD;


        /*   tutaj zaczyna sie algorytm GA    */


        /* generowanie populacji startowej*/
        vector<unsigned> first_path, second_path, new_path, best_path;
        vector<vector<unsigned>> population, new_population, temp_population;
        int population_size = citiesNum * 2;
        clock_t start = clock();
        best_path = random_path(citiesNum);
        for (int i = 0; i < population_size; i++) {
            new_path = random_path(citiesNum);
            if (!if_exists(population, new_path) && fitness_function(best_path, cityMatrix) < 1.2 * fitness_function(new_path, cityMatrix)) {
                population.push_back(new_path);
                if (fitness_function(best_path, cityMatrix) < fitness_function(new_path, cityMatrix))
                    best_path = new_path;
            }
            else
                i--;
        }
        best_path = get_best_path(population, cityMatrix);
        double start_prd = 100 * (double)count_cost(best_path, cityMatrix) / hamiltonOpt;
        cout << "start best_path : " << start_prd << "%" << endl;
        int generations = 0;
        double crossingRate = 0.9;
        double mutationRate = 0.05;
        int no_improvement = 0;
        bool stop_condition = true;

        do {
            vector<vector<unsigned>> new_population;
            vector<vector<unsigned>> available_parents = population;
            sort_vector(available_parents, cityMatrix);

            while (new_population.size() < population_size * crossingRate) {
                tournament_selection_parents(first_path, second_path, available_parents, cityMatrix);
                new_path = order_crossover(first_path, second_path);
                if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= mutationRate)
                    mutation(new_path, mutation_choice);
                if (!if_exists(new_population, new_path) && !if_exists(population, new_path))
                    new_population.push_back(new_path);
            }

            for (int i = 0; i < new_population.size(); i++) {
                population.push_back(new_population.at(i));
            }

            if (fitness_function(best_path, cityMatrix) < fitness_function(get_best_path(population, cityMatrix), cityMatrix)) {
                best_path = get_best_path(population, cityMatrix);
                PRD = (double)count_cost(best_path, cityMatrix) / hamiltonOpt;
                cout << generations << " " << "(" << PRD * 100 << " %)" << endl;
                no_improvement = 0;
            }
            else
                no_improvement++;
            vector<vector<unsigned>> next_gen_population = selection(population, cityMatrix, population_size, selection_choice);
            population.clear();
            population = next_gen_population;
            generations++;
            clock_t now_time = clock();
            stop_condition = time_stop(start, now_time, max_time);


        } while (generations < citiesNum*5 );




        clock_t stop = clock();

        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]" << endl;
        PRD = (double)count_cost(best_path, cityMatrix) / hamiltonOpt;
        cout << "PRD = " << PRD * 100 << " %" << endl;
        print_path(best_path);

        /*        zwalnianie pamieci               */
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        cout << "end" << endl;
    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
}
