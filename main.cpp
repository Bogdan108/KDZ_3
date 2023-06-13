#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <fstream>

//  инициализирую начальную точку и конечную
int start, end;


void dfs(int v, std::vector<std::vector<int>> &graph_jjj, std::vector<bool> &used) {
    used[v] = true;
    for (int u: graph_jjj[v]) {
        if (!used[u]) {
            dfs(u, graph_jjj, used);
        }
    }
}

void dijkstra(std::vector<std::vector<int>> &init_information) {
    int n, m, first, second, data;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int64_t, int>>> graph(n,
                                                            std::vector<std::pair<int64_t, int>>());

    for (int j = 0; j < m; ++j) {
        std::cin >> first >> second >> data;
        graph[first].push_back({data, second});
    }

    int start = 0;

    std::vector<int64_t> distance(n, INT64_MAX);
    distance[start] = 0;
    std::set<std::pair<int64_t, int>> queue;
    queue.insert({distance[start], start});

    while (!queue.empty()) {
        int front = queue.begin()->second;
        queue.erase((queue.begin()));
        for (size_t j = 0; j < graph[front].size(); ++j) {
            int to = graph[front][j].second;
            int64_t len = graph[front][j].first;
            if (distance[front] + len < distance[to]) {
                queue.erase({distance[to], to});
                distance[to] = distance[front] + len;
                queue.insert({distance[to], to});
            }
        }
    }

    for (size_t i = 1; i < distance.size(); ++i) {
        std::cout << distance[i] << '\n';
    }
}

void BellmanFord(std::vector<std::vector<int>> &init_information) {
    // вспомогательные функции и данные для Беллмана-Форда
    std::vector<std::pair<int64_t, std::pair<int, int>>> graph;
    int n, m, first, second, cost;
    std::cin >> n >> m;
    std::vector<int64_t> distance(n, INT64_MAX);
    std::vector<bool> used(n);
    std::vector<std::vector<int>> graph_jjj(n);
    std::vector<bool> cycle(n, false);
    distance[0] = 0;

    for (int i = 0; i < m; ++i) {
        std::cin >> first >> second >> cost;
        graph.push_back({cost, {first, second}});
        graph_jjj[first].push_back(second);
    }

    bool negative = false;
    for (int j = 0; j <= n; ++j) {
        bool indicate = false;
        for (int i = 0; i < m; ++i) {
            if (distance[graph[i].second.first] < INT64_MAX) {
                if (distance[graph[i].second.second] >
                    distance[graph[i].second.first] + graph[i].first) {
                    if (j == n) {
                        cycle[graph[i].second.second] = true;
                        negative = true;
                    }
                    distance[graph[i].second.second] =
                            (distance[graph[i].second.first] + graph[i].first);
                    indicate = true;
                }
            }
        }
        if (!indicate) {
            break;
        }
    }
    if (negative) {
        for (int i = 1; i < n; ++i) {
            if (cycle[i]) {
                dfs(i, graph_jjj, used);
            }
        }
        for (int i = 1; i < n; ++i) {
            if (used[i]) {
                std::cout << "-inf" << '\n';
            } else {
                std::cout << distance[i] << '\n';
            }
        }

    } else {
        for (int i = 1; i < n; ++i) {
            std::cout << distance[i] << '\n';
        }
    }
}

void FloydWarshall(std::vector<std::vector<int>> &init_information) {
    int n, m, first, second, data;
    std::cin >> n >> m;
    std::vector<std::vector<int64_t>> graph(n, std::vector<int64_t>(n, INT64_MAX));

    for (int j = 0; j < m; ++j) {
        std::cin >> first >> second >> data;
        graph[first][second] = data;
    }
    for (int i = 0; i < n; ++i) {
        graph[i][i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int s = 0; s < n; ++s) {
                if (graph[j][i] < INT64_MAX && graph[i][s] < INT64_MAX) {
                    graph[j][s] = std::min(graph[j][s], graph[j][i] + graph[i][s]);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                if (graph[i][j] == INT64_MAX) {
                    graph[i][j] = -1;
                }
                std::cout << i << ' ' << j << ' ' << graph[i][j] << '\n';
            }
        }
    }
}

uint64_t analyse(const std::function<void(std::vector<std::vector<int>>)>&  algo,
                 int range_og_graph, const std::string& type_of_graph) {
    uint64_t sum = 0;

    for (int i = 0; i < number_of_launches; ++i) {
        if (type_of_graph == "binary") {
            randomChars(reference, 2);
        } else {
            randomChars(reference, 4);
        }

        auto time_start = std::chrono::high_resolution_clock::now();
        algo(reference, reference_size, example, indicator);
        auto time_elapsed = std::chrono::high_resolution_clock::now();
        int64_t nanoseconds =
                std::chrono::duration_cast<std::chrono::nanoseconds>(time_elapsed - time_start).count();
        sum += nanoseconds;
    }
    sum /= number_of_launches;
    return sum;
}

void printAndAnalyse(
        std::vector<std::pair<std::string, std::function<void(std::vector<std::vector<int>> &)>>> &
        searchers) {
// директория в которую пишу результаты
    std::string directory = "/Users/bogdanlukancuk/Desktop/KDZ-3/";
// открытие и запись в файл для маленьких диапазонов сортировки
    std::ofstream file(directory + "data" + ".csv");
    file << "algo,graph_type,vertex_size,time\n";

// прохожу по всем видам алгоритмов в цикле
    for (const auto& algo: searchers) {
// завожу цикл для заполнения эталлонного массива (reference) значениями
        for (int marker = 0; marker < 3; marker++) {
// маркер типа массива
            std::string type_of_graph;
            int vertex_size;
            switch (marker) {
// генерирую массив из 100000 символов  в бинарном алфавите
                case 0: {
// случайныe значения в бинарном алфавите 10000 символов
                    type_of_graph = "binary";
                    break;
                }
                case 1: {
// случайныe значения в бинарном алфавите 100000 символов
                    type_of_graph = "binary";
                    break;
                }
// генерирую массив из 100000 символов  в небинарном алфавите
                case 2: {
// случайныe значения в небинарном алфавите 10000 символов
                    type_of_graph = "nonbinary";
                    break;
                }
            }

// функция analyse выдает уредненное значение на заданном диапазоне
            for (int range_og_graph = 10; range_og_graph <= 1010; range_og_graph += 50) {
                uint64_t answer = analyse(algo.second, range_og_graph, type_of_graph);
                file << algo.first << ',' << type_of_graph << ','
                     << range_og_graph << ',' << answer << '\n';
            }
        }
    }
    file.close();
}

int main() {
    srand(time(nullptr));

    // создаю массив пар с названием алгоритмов и функциями, реализующих их
    std::vector<std::pair<std::string, std::function<void(std::vector<std::vector<int>> &)>>>
            searchers = {
            std::make_pair("Dijkstra", dijkstra),
            std::make_pair("Bellman-Ford", BellmanFord),
            std::make_pair("Floyd–Warshall", FloydWarshall),
    };

    // передаю инициализированнный массив функций в функцию, которая анализирует каждую функцию и
    // записывает резултат в файл
    printAndAnalyse(searchers);

    return 0;
}
