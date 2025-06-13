#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>

template<class T>
class Queue {
private:
    class Node {
    public:
        T data;
        Node* next;
        
        Node();
        Node(const T& value);
        Node(T&& value);
        
        // Destrutor
        ~Node() = default;
    };
    
    Node* frontNode;    // Primeiro elemento (para remoção)
    Node* rearNode;     // Último elemento (para inserção)
    size_t queueSize;
    
public:
    // ==================== CONSTRUTORES E DESTRUTOR ====================
    // Construtor padrão
    Queue();
    
    // Construtor de cópia
    Queue(const Queue& other);
    
    // Construtor de movimento
    Queue(Queue&& other) noexcept;
    
    // Construtor com lista de inicialização
    Queue(std::initializer_list<T> init);
    
    // Destrutor
    ~Queue();
    
    // ==================== OPERADORES DE ATRIBUIÇÃO ====================
    
    // Operador de atribuição por cópia
    Queue& operator=(const Queue& other);
    
    // Operador de atribuição por movimento
    Queue& operator=(Queue&& other) noexcept;
    
    // ==================== MÉTODOS PRINCIPAIS ====================
    
    // Adiciona elemento no final da fila
    void enqueue(const T& value);
    
    // Adiciona elemento no final da fila (versão move)
    void enqueue(T&& value);
    
    // Constrói elemento in-place no final da fila
    template<typename... Args>
    void emplace(Args&&... args);
    
    // Remove elemento do início da fila
    void dequeue();
    
    // Remove e retorna elemento do início da fila
    T dequeueAndReturn();
    
    // Acessa elemento do início da fila (referência)
    T& front();
    
    // Acessa elemento do início da fila (const)
    const T& front() const;
    
    // Acessa elemento do final da fila (referência)
    T& rear();
    
    // Acessa elemento do final da fila (const)
    const T& rear() const;
    
    // ==================== MÉTODOS DE CONSULTA ====================
    
    // Verifica se está vazia
    bool empty() const;
    
    // Retorna o tamanho
    size_t size() const;
    
    // Verifica se contém um elemento
    bool contains(const T& value) const;
    
    // Conta ocorrências de um elemento
    size_t count(const T& value) const;
    
    // Acessa elemento por índice (0 = frente)
    T& at(size_t index);
    
    // Acessa elemento por índice (const)
    const T& at(size_t index) const;
    
    // ==================== OPERAÇÕES DE MANIPULAÇÃO ====================
    
    // Limpa toda a fila
    void clear();
    
    // Troca conteúdo com outra fila
    void swap(Queue& other) noexcept;
    
    // Remove todas as ocorrências de um valor
    size_t removeAll(const T& value);
    
    // Remove primeira ocorrência de um valor
    bool removeFirst(const T& value);
    
    // Duplica o elemento da frente
    void duplicate();
    
    // Inverte a fila
    void reverse();
    
    // ==================== MÉTODOS DE BUSCA ====================
    
    // Encontra índice da primeira ocorrência
    int findFirst(const T& value) const;
    
    // Encontra índice da última ocorrência
    int findLast(const T& value) const;
    
    // ==================== MÉTODOS FUNCIONAIS ====================
    
    // Aplica função a todos os elementos
    void forEach(std::function<void(T&)> func);
    
    // Aplica função a todos os elementos (const)
    void forEach(std::function<void(const T&)> func) const;
    
    // Verifica se todos os elementos satisfazem condição
    bool allOf(std::function<bool(const T&)> predicate) const;
    
    // Verifica se algum elemento satisfaz condição
    bool anyOf(std::function<bool(const T&)> predicate) const;
    
    // ==================== CONVERSÕES ====================
    
    // Converte para vetor (frente primeiro)
    std::vector<T> toVector() const;
    
    // Converte para vetor invertido (final primeiro)
    std::vector<T> toVectorReversed() const;
    
    // ==================== OPERADORES DE COMPARAÇÃO ====================
    
    bool operator==(const Queue& other) const;
    bool operator!=(const Queue& other) const;
    
    // ==================== OPERADOR DE SAÍDA ====================
    
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const Queue<U>& queue);
    
    // ==================== MÉTODOS DE DEBUG ====================
    
    // Imprime estrutura da fila
    void print() const;
    
    // Verifica integridade da estrutura
    bool checkIntegrity() const;
};

// ==================== IMPLEMENTAÇÕES INLINE ====================

// Construtores da classe Node
template<class T>
Queue<T>::Node::Node() : next(nullptr) {}

template<class T>
Queue<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

template<class T>
Queue<T>::Node::Node(T&& value) : data(std::move(value)), next(nullptr) {}

// Construtor padrão
template<class T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}

// Construtor de cópia
template<class T>
Queue<T>::Queue(const Queue& other) : frontNode(nullptr), rearNode(nullptr), queueSize(0) {
    *this = other;
}

// Construtor de movimento
template<class T>
Queue<T>::Queue(Queue&& other) noexcept 
    : frontNode(other.frontNode), rearNode(other.rearNode), queueSize(other.queueSize) {
    other.frontNode = nullptr;
    other.rearNode = nullptr;
    other.queueSize = 0;
}

// Construtor com lista de inicialização
template<class T>
Queue<T>::Queue(std::initializer_list<T> init) : frontNode(nullptr), rearNode(nullptr), queueSize(0) {
    for (const auto& item : init) {
        enqueue(item);
    }
}

// Destrutor
template<class T>
Queue<T>::~Queue() {
    clear();
}

// Operador de atribuição por cópia
template<class T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        clear();
        
        Node* current = other.frontNode;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }
    return *this;
}

// Operador de atribuição por movimento
template<class T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
    if (this != &other) {
        clear();
        frontNode = other.frontNode;
        rearNode = other.rearNode;
        queueSize = other.queueSize;
        other.frontNode = nullptr;
        other.rearNode = nullptr;
        other.queueSize = 0;
    }
    return *this;
}

// Enqueue com cópia
template<class T>
void Queue<T>::enqueue(const T& value) {
    Node* newNode = new Node(value);
    
    if (empty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    ++queueSize;
}

// Enqueue com movimento
template<class T>
void Queue<T>::enqueue(T&& value) {
    Node* newNode = new Node(std::move(value));
    
    if (empty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    ++queueSize;
}

// Emplace
template<class T>
template<typename... Args>
void Queue<T>::emplace(Args&&... args) {
    Node* newNode = new Node();
    newNode->data = T(std::forward<Args>(args)...);
    
    if (empty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    ++queueSize;
}

// Dequeue
template<class T>
void Queue<T>::dequeue() {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    
    Node* temp = frontNode;
    frontNode = frontNode->next;
    
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    
    delete temp;
    --queueSize;
}

// Dequeue and return
template<class T>
T Queue<T>::dequeueAndReturn() {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    T value = frontNode->data;
    dequeue();
    return value;
}

// Front (referência)
template<class T>
T& Queue<T>::front() {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    return frontNode->data;
}

// Front (const)
template<class T>
const T& Queue<T>::front() const {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    return frontNode->data;
}

// Rear (referência)
template<class T>
T& Queue<T>::rear() {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    return rearNode->data;
}

// Rear (const)
template<class T>
const T& Queue<T>::rear() const {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    return rearNode->data;
}

// Empty
template<class T>
bool Queue<T>::empty() const {
    return frontNode == nullptr;
}

// Size
template<class T>
size_t Queue<T>::size() const {
    return queueSize;
}

// Contains
template<class T>
bool Queue<T>::contains(const T& value) const {
    Node* current = frontNode;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Count
template<class T>
size_t Queue<T>::count(const T& value) const {
    size_t counter = 0;
    Node* current = frontNode;
    while (current != nullptr) {
        if (current->data == value) {
            ++counter;
        }
        current = current->next;
    }
    return counter;
}

// At (referência)
template<class T>
T& Queue<T>::at(size_t index) {
    if (index >= queueSize) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = frontNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

// At (const)
template<class T>
const T& Queue<T>::at(size_t index) const {
    if (index >= queueSize) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = frontNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

// Clear
template<class T>
void Queue<T>::clear() {
    while (!empty()) {
        dequeue();
    }
}

// Swap
template<class T>
void Queue<T>::swap(Queue& other) noexcept {
    std::swap(frontNode, other.frontNode);
    std::swap(rearNode, other.rearNode);
    std::swap(queueSize, other.queueSize);
}

// Remove all
template<class T>
size_t Queue<T>::removeAll(const T& value) {
    size_t removed = 0;
    Queue<T> temp;
    
    while (!empty()) {
        if (front() != value) {
            temp.enqueue(dequeueAndReturn());
        } else {
            dequeue();
            ++removed;
        }
    }
    
    while (!temp.empty()) {
        enqueue(temp.dequeueAndReturn());
    }
    
    return removed;
}

// Remove first
template<class T>
bool Queue<T>::removeFirst(const T& value) {
    Queue<T> temp;
    bool found = false;
    
    while (!empty() && !found) {
        if (front() == value) {
            dequeue();
            found = true;
        } else {
            temp.enqueue(dequeueAndReturn());
        }
    }
    
    while (!temp.empty()) {
        enqueue(temp.dequeueAndReturn());
    }
    
    return found;
}

// Duplicate
template<class T>
void Queue<T>::duplicate() {
    if (empty()) {
        throw std::underflow_error("Queue is empty");
    }
    T value = front();
    
    // Precisamos inserir na frente, então removemos tudo, inserimos duplicata e reinsere tudo
    Queue<T> temp;
    while (!empty()) {
        temp.enqueue(dequeueAndReturn());
    }
    
    enqueue(value);
    while (!temp.empty()) {
        enqueue(temp.dequeueAndReturn());
    }
}

// Reverse
template<class T>
void Queue<T>::reverse() {
    if (queueSize <= 1) return;
    
    std::vector<T> temp;
    while (!empty()) {
        temp.push_back(dequeueAndReturn());
    }
    
    for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
        enqueue(*it);
    }
}

// Find first
template<class T>
int Queue<T>::findFirst(const T& value) const {
    Node* current = frontNode;
    int index = 0;
    while (current != nullptr) {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        ++index;
    }
    return -1;
}

// Find last
template<class T>
int Queue<T>::findLast(const T& value) const {
    Node* current = frontNode;
    int index = 0;
    int lastFound = -1;
    while (current != nullptr) {
        if (current->data == value) {
            lastFound = index;
        }
        current = current->next;
        ++index;
    }
    return lastFound;
}

// For each (não const)
template<class T>
void Queue<T>::forEach(std::function<void(T&)> func) {
    Node* current = frontNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

// For each (const)
template<class T>
void Queue<T>::forEach(std::function<void(const T&)> func) const {
    Node* current = frontNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

// All of
template<class T>
bool Queue<T>::allOf(std::function<bool(const T&)> predicate) const {
    Node* current = frontNode;
    while (current != nullptr) {
        if (!predicate(current->data)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

// Any of
template<class T>
bool Queue<T>::anyOf(std::function<bool(const T&)> predicate) const {
    Node* current = frontNode;
    while (current != nullptr) {
        if (predicate(current->data)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// To vector
template<class T>
std::vector<T> Queue<T>::toVector() const {
    std::vector<T> result;
    result.reserve(queueSize);
    Node* current = frontNode;
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

// To vector reversed
template<class T>
std::vector<T> Queue<T>::toVectorReversed() const {
    std::vector<T> result = toVector();
    std::reverse(result.begin(), result.end());
    return result;
}

// Operator ==
template<class T>
bool Queue<T>::operator==(const Queue& other) const {
    if (queueSize != other.queueSize) {
        return false;
    }
    
    Node* current1 = frontNode;
    Node* current2 = other.frontNode;
    
    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data != current2->data) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    
    return current1 == nullptr && current2 == nullptr;
}

// Operator !=
template<class T>
bool Queue<T>::operator!=(const Queue& other) const {
    return !(*this == other);
}

// Print
template<class T>
void Queue<T>::print() const {
    std::cout << "Queue [size=" << queueSize << "]: ";
    if (empty()) {
        std::cout << "(empty)";
    } else {
        std::cout << "FRONT -> ";
        Node* current = frontNode;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " <- REAR";
    }
    std::cout << std::endl;
}

// Check integrity
template<class T>
bool Queue<T>::checkIntegrity() const {
    if (queueSize == 0) {
        return frontNode == nullptr && rearNode == nullptr;
    }
    
    if (queueSize == 1) {
        return frontNode == rearNode && frontNode != nullptr;
    }
    
    size_t count = 0;
    Node* current = frontNode;
    Node* last = nullptr;
    
    while (current != nullptr) {
        ++count;
        last = current;
        current = current->next;
    }
    
    return count == queueSize && last == rearNode;
}

// Operador de saída
template<class T>
std::ostream& operator<<(std::ostream& os, const Queue<T>& queue) {
    os << "[";
    typename Queue<T>::Node* current = queue.frontNode;
    bool first = true;
    while (current != nullptr) {
        if (!first) {
            os << ", ";
        }
        os << current->data;
        first = false;
        current = current->next;
    }
    os << "]";
    return os;
}

#endif // QUEUE_H