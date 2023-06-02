#include "excercises.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include <QQueue>
#include <QStack>
#include <QString>
#include <QVector>
#include <iostream>
#include <limits>

// класс для 5 задания
class NetworkGraph {
 public:
  NetworkGraph(int vertices) {
    this->vertices = vertices;
    adjacencyMatrix =
        QVector<QVector<int>>(vertices, QVector<int>(vertices, 0));
  }

  // Добавление ребра в граф
  void addEdge(int from, int to, int capacity) {
    if (from >= 0 && from < vertices && to >= 0 && to < vertices)
      adjacencyMatrix[from][to] = capacity;
    else
      qDebug() << "Ошибка: некорректные индексы вершин";
  }

  // Поиск максимального потока в сети с помощью алгоритма Форда-Фалкерсона
  int findMaxFlow(int source, int sink) {
    // Создание остаточной сети
    QVector<QVector<int>> residualGraph(adjacencyMatrix);
    QVector<int> parent(vertices);

    int maxFlow = 0;

    while (bfs(source, sink, parent, residualGraph)) {
      int pathFlow = std::numeric_limits<int>::max();

      // Нахождение минимальной пропускной способности ребер на пути
      for (int v = sink; v != source; v = parent[v]) {
        int u = parent[v];
        pathFlow = qMin(pathFlow, residualGraph[u][v]);
      }

      // Обновление остаточной сети
      for (int v = sink; v != source; v = parent[v]) {
        int u = parent[v];
        residualGraph[u][v] -= pathFlow;
        residualGraph[v][u] += pathFlow;
      }

      maxFlow += pathFlow;
    }

    return maxFlow;
  }

 private:
  int vertices;
  QVector<QVector<int>> adjacencyMatrix;

  // Поиск пути в остаточной сети с помощью алгоритма поиска в ширину (BFS)
  bool bfs(int source, int sink, QVector<int>& parent,
           const QVector<QVector<int>>& residualGraph) {
    QVector<bool> visited(vertices, false);
    QQueue<int> queue;

    visited[source] = true;
    queue.enqueue(source);
    parent[source] = -1;

    while (!queue.isEmpty()) {
      int u = queue.front();
      queue.dequeue();

      for (int v = 0; v < vertices; ++v) {
        if (!visited[v] && residualGraph[u][v] > 0) {
          visited[v] = true;
          queue.enqueue(v);
          parent[v] = u;
        }
      }
    }

    return visited[sink];
  }
};

bool Excercises::task1(QString answer, QString exercise_data) {
  if (answer == exercise_data) {
    return true;
  }
  return false;
}

bool Excercises::task2(QString answer, QString exercise_data) {
  if (answer == exercise_data) {
    return true;
  }
  return false;
}

bool Excercises::task3(QString answer, QString exercise_data) {
  QStringList inputList = exercise_data.split(' ', Qt::SkipEmptyParts);
  int numNodes = inputList.takeFirst().toInt();

  QList<QPair<int, int>> edges;
  while (!inputList.isEmpty()) {
    int node1 = inputList.takeFirst().toInt();
    int node2 = inputList.takeFirst().toInt();
    edges.append(qMakePair(node1, node2));
  }

  QMap<int, int> degrees;
  for (int i = 0; i < numNodes - 2; ++i) {
    degrees[i] = 0;
  }

  for (const auto& edge : edges) {
    degrees[edge.first]++;
    degrees[edge.second]++;
  }

  QString prueferCode;

  for (int i = 0; i < numNodes - 2; ++i) {
    int leafNode = -1;
    for (const auto& node : degrees.keys()) {
      if (degrees[node] == 1) {
        leafNode = node;
        break;
      }
    }

    if (leafNode == -1) {
      qWarning() << "Invalid input. Unable to build Pruefer code.";
      return false;  // тут может быть баг
    }

    degrees.remove(leafNode);

    for (const auto& edge : edges) {
      if (edge.first == leafNode || edge.second == leafNode) {
        degrees[edge.first]--;
        degrees[edge.second]--;
      }
    }

    prueferCode.append(QString::number(leafNode));
  }
  qDebug() << prueferCode;

  // вот тут надо сравнить результат алгоритма и ответа пользователя
  return prueferCode == answer;
}

bool Excercises::task4(QString answer, QString exercise_data) {
  if (answer == exercise_data) {
    return true;
  }
  return false;
}

bool Excercises::task5(QString answer, QString exercise_data) {
  QStringList tokens = exercise_data.split(' ');
  int numNodes = tokens.takeFirst().toInt();

  NetworkGraph graph(numNodes);

  int from, to, capacity;
  while (!tokens.isEmpty()) {
    from = tokens.takeFirst().toInt();
    to = tokens.takeFirst().toInt();
    capacity = tokens.takeFirst().toInt();
    graph.addEdge(from, to, capacity);
  }

  int source = 0;
  int sink = numNodes - 1;
  int maxFlow = graph.findMaxFlow(source, sink);
  qDebug() << maxFlow;

  return QString::number(maxFlow) == answer;
}
