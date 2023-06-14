#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <fstream>
#include <map>

//  инициализирую начальную точку и конечную
int starter = 0, end;
int number_of_launches = 5;


void dfs(int v, std::vector<std::vector<int>> &graph_jjj, std::vector<bool> &used) {
    used[v] = true;
    for (int u: graph_jjj[v]) {
        if (!used[u]) {
            dfs(u, graph_jjj, used);
        }
    }
}

void dijkstra(const std::vector<std::vector<int>> &init_information) {

    std::vector<std::vector<std::pair<int64_t, int>>> graph(init_information.size(),
                                                            std::vector<std::pair<int64_t, int>>());


    for (int i = 0; i < init_information.size(); ++i) {
        for (int j = 0; j < init_information[i].size(); ++j) {
            int price = rand() % 10 + 1;
            graph[i].push_back({price, init_information[i][j]});
            graph[init_information[i][j]].push_back({price, i});
        }
    }

    int start = starter;

    std::vector<int64_t> distance(init_information.size(), INT64_MAX);
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

}

void BellmanFord(const std::vector<std::vector<int>> &init_information) {
    // вспомогательные функции и данные для Беллмана-Форда
    std::vector<std::pair<int64_t, std::pair<int, int>>> graph;

    std::vector<int64_t> distance(init_information.size(), INT64_MAX);
    std::vector<bool> used(init_information.size());
    std::vector<std::vector<int>> graph_jjj(init_information.size());
    std::vector<bool> cycle(init_information.size(), false);
    distance[starter] = starter;

    int m = 0;
    for (int i = 0; i < init_information.size(); ++i) {
        for (int j = 0; j < init_information[i].size(); ++j) {
            int price = rand() % 10 + 1;
            graph.push_back({price, {i, init_information[i][j]}});
            graph_jjj[i].push_back(init_information[i][j]);
            graph.push_back({price, {init_information[i][j], i}});
            graph_jjj[init_information[i][j]].push_back(i);
            ++m;
        }
    }


    bool negative = false;
    for (int j = 0; j <= init_information.size(); ++j) {
        bool indicate = false;
        for (int i = 0; i < m; ++i) {
            if (distance[graph[i].second.first] < INT64_MAX) {
                if (distance[graph[i].second.second] >
                    distance[graph[i].second.first] + graph[i].first) {
                    if (j == init_information.size()) {
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
}

void FloydWarshall(const std::vector<std::vector<int>> &init_information) {

    std::vector<std::vector<int64_t>> graph(init_information.size(),
                                            std::vector<int64_t>(init_information.size(), INT64_MAX));

    for (int i = 0; i < init_information.size(); ++i) {
        for (int j = 0; j < init_information[i].size(); ++j) {
            int price = rand() % 10 + 1;
            graph[i][init_information[i][j]] = price;
            graph[init_information[i][j]][i] = price;
        }
    }

    for (int i = 0; i < graph.size(); ++i) {
        graph[i][i] = 0;
    }
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph.size(); ++j) {
            for (int s = 0; s < graph.size(); ++s) {
                if (graph[j][i] < INT64_MAX && graph[i][s] < INT64_MAX) {
                    graph[j][s] = std::min(graph[j][s], graph[j][i] + graph[i][s]);
                }
            }
        }
    }
}

uint32_t graphGenerator(std::vector<std::vector<int>> &reference, int graph_size, const std::string &type_of_graph) {

    if (type_of_graph == "Tree graph") {
        for (int i = 1; i < graph_size; ++i) {
            reference[0].push_back(i);
            reference[i].push_back(0);
        }
        return graph_size - 1;
    }

    if (type_of_graph == "Sparse graph") {
        int local_size = 0;
        std::map<std::pair<int, int>, bool> additional;
        for (int i = 1; i < graph_size; ++i) {
            reference[0].push_back(i);
            reference[i].push_back(0);
            additional[{0, i}] = true;
            additional[{i, 0}] = true;
            local_size++;
        }
        double k = static_cast<double>(local_size) / ((graph_size * (graph_size - 1)) / 2);
        while (k < 0.4) {
            int i, j;
            i = rand() % graph_size;
            j = rand() % graph_size;
            if (additional.find({i, j}) != additional.end()) {
                reference[i].push_back(j);
                reference[j].push_back(i);
                additional[{i, j}] = true;
                additional[{j, i}] = true;
                local_size++;
                k = static_cast<double>(local_size) / ((graph_size * (graph_size - 1)) / 2);
            }
        }
        return local_size;
    }

    if (type_of_graph == "Complete graph") {
        for (int i = 0; i < graph_size; ++i) {
            for (int j = i + 1; j < graph_size; ++j) {
                reference[i].push_back(j);
                reference[j].push_back(i);
            }
        }
        return ((graph_size - 1) * graph_size) / 2;
    }
    return 0;
}

uint64_t analyse(const std::function<void(std::vector<std::vector<int>> &)> &algo,
                 int range_of_graph, std::string &type_of_graph, uint32_t &count_of_edges) {
    uint64_t sum = 0;

    // завожу эталонный граф для тестирования алгоритмов
    std::vector<std::vector<int>> reference_graph(range_of_graph, std::vector<int>());

    // в зависимости от типа генерирую соответсвующий граф
    count_of_edges = graphGenerator(reference_graph, range_of_graph, type_of_graph);

    for (int i = 0; i < number_of_launches; ++i) {
        auto time_start = std::chrono::high_resolution_clock::now();
        algo(reference_graph);
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
    file << "algo,graph_type,count_of_edges,count_of_vertex,time\n";

// прохожу по всем видам алгоритмов в цикле
    for (const auto &algo: searchers) {
// завожу цикл для генерирования эталлонного графа определенного типа
        for (int marker = 0; marker < 3; marker++) {
// маркер типа графа
            std::string type_of_graph;
            switch (marker) {
                case 0: {
                    type_of_graph = "Tree graph";
                    break;
                }
                case 1: {
                    type_of_graph = "Sparse graph";
                    break;
                }
                case 2: {
                    type_of_graph = "Complete graph";
                    break;
                }
            }

// функция analyse выдает уредненное значение на заданном диапазоне и типе графа
            for (int range_og_graph = 10; range_og_graph <= 1010; range_og_graph += 50) {
                uint32_t count_of_edges = 0;
                uint64_t answer = analyse(algo.second, range_og_graph, type_of_graph, count_of_edges);
                file << algo.first << ',' << type_of_graph << ',' << count_of_edges << ','
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
    // записывает результат в файл
    printAndAnalyse(searchers);

    return 0;
}
