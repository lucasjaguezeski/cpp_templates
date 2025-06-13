#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>

template<class T>
class Stack {
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
    
    Node* topNode;      
    size_t stackSize;
    
public:
    // ==================== CONSTRUTORES E DESTRUTOR ====================
    // Construtor padrão
    Stack();
    
    // Construtor de cópia
    Stack(const Stack& other);
    
    // Construtor de movimento
    Stack(Stack&& other) noexcept;
    
    // Construtor com lista de inicialização
    Stack(std::initializer_list<T> init);
    
    // Destrutor
    ~Stack();
    
    // ==================== OPERADORES DE ATRIBUIÇÃO ====================
    
    // Operador de atribuição por cópia
    Stack& operator=(const Stack& other);
    
    // Operador de atribuição por movimento
    Stack& operator=(Stack&& other) noexcept;
    
    // ==================== MÉTODOS PRINCIPAIS ====================
    
    // Adiciona elemento no topo
    void push(const T& value);
    
    // Adiciona elemento no topo (versão move)
    void push(T&& value);
    
    // Constrói elemento in-place no topo
    template<typename... Args>
    void emplace(Args&&... args);
    
    // Remove elemento do topo
    void pop();
    
    // Remove e retorna elemento do topo
    T popAndReturn();
    
    // Acessa elemento do topo (referência)
    T& top();
    
    // Acessa elemento do topo (const)
    const T& top() const;
    
    // ==================== MÉTODOS DE CONSULTA ====================
    
    // Verifica se está vazia
    bool empty() const;
    
    // Retorna o tamanho
    size_t size() const;
    
    // Verifica se contém um elemento
    bool contains(const T& value) const;
    
    // Conta ocorrências de um elemento
    size_t count(const T& value) const;
    
    // Acessa elemento por índice (0 = topo)
    T& at(size_t index);
    
    // Acessa elemento por índice (const)
    const T& at(size_t index) const;
    
    // ==================== OPERAÇÕES DE MANIPULAÇÃO ====================
    
    // Limpa toda a pilha
    void clear();
    
    // Troca conteúdo com outra pilha
    void swap(Stack& other) noexcept;
    
    // Remove todas as ocorrências de um valor
    size_t removeAll(const T& value);
    
    // Remove primeira ocorrência de um valor
    bool removeFirst(const T& value);
    
    // Duplica o elemento do topo
    void duplicate();
    
    // Inverte a pilha
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
    
    // Converte para vetor (topo primeiro)
    std::vector<T> toVector() const;
    
    // Converte para vetor invertido (base primeiro)
    std::vector<T> toVectorReversed() const;
    
    // ==================== OPERADORES DE COMPARAÇÃO ====================
    
    bool operator==(const Stack& other) const;
    bool operator!=(const Stack& other) const;
    
    // ==================== OPERADOR DE SAÍDA ====================
    
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const Stack<U>& stack);
    
    // ==================== MÉTODOS DE DEBUG ====================
    
    // Imprime estrutura da pilha
    void print() const;
    
    // Verifica integridade da estrutura
    bool checkIntegrity() const;
};

// ==================== IMPLEMENTAÇÕES INLINE ====================

// Construtores da classe Node
template<class T>
Stack<T>::Node::Node() : next(nullptr) {}

template<class T>
Stack<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

template<class T>
Stack<T>::Node::Node(T&& value) : data(std::move(value)), next(nullptr) {}

// Construtor padrão
template<class T>
Stack<T>::Stack() : topNode(nullptr), stackSize(0) {}

// Construtor de cópia
template<class T>
Stack<T>::Stack(const Stack& other) : topNode(nullptr), stackSize(0) {
    *this = other;
}

// Construtor de movimento
template<class T>
Stack<T>::Stack(Stack&& other) noexcept : topNode(other.topNode), stackSize(other.stackSize) {
    other.topNode = nullptr;
    other.stackSize = 0;
}

// Construtor com lista de inicialização
template<class T>
Stack<T>::Stack(std::initializer_list<T> init) : topNode(nullptr), stackSize(0) {
    for (const auto& item : init) {
        push(item);
    }
}

// Destrutor
template<class T>
Stack<T>::~Stack() {
    clear();
}

// Operador de atribuição por cópia
template<class T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {
        clear();
        
        // Copia os elementos na ordem correta
        std::vector<T> temp;
        Node* current = other.topNode;
        while (current != nullptr) {
            temp.push_back(current->data);
            current = current->next;
        }
        
        // Insere na ordem inversa para manter a estrutura
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            push(*it);
        }
    }
    return *this;
}

// Operador de atribuição por movimento
template<class T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
    if (this != &other) {
        clear();
        topNode = other.topNode;
        stackSize = other.stackSize;
        other.topNode = nullptr;
        other.stackSize = 0;
    }
    return *this;
}

// Push com cópia
template<class T>
void Stack<T>::push(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = topNode;
    topNode = newNode;
    ++stackSize;
}

// Push com movimento
template<class T>
void Stack<T>::push(T&& value) {
    Node* newNode = new Node(std::move(value));
    newNode->next = topNode;
    topNode = newNode;
    ++stackSize;
}

// Emplace
template<class T>
template<typename... Args>
void Stack<T>::emplace(Args&&... args) {
    Node* newNode = new Node();
    newNode->data = T(std::forward<Args>(args)...);
    newNode->next = topNode;
    topNode = newNode;
    ++stackSize;
}

// Pop
template<class T>
void Stack<T>::pop() {
    if (empty()) {
        throw std::underflow_error("Stack is empty");
    }
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
    --stackSize;
}

// Pop and return
template<class T>
T Stack<T>::popAndReturn() {
    if (empty()) {
        throw std::underflow_error("Stack is empty");
    }
    T value = topNode->data;
    pop();
    return value;
}

// Top (referência)
template<class T>
T& Stack<T>::top() {
    if (empty()) {
        throw std::underflow_error("Stack is empty");
    }
    return topNode->data;
}

// Top (const)
template<class T>
const T& Stack<T>::top() const {
    if (empty()) {
        throw std::underflow_error("Stack is empty");
    }
    return topNode->data;
}

// Empty
template<class T>
bool Stack<T>::empty() const {
    return topNode == nullptr;
}

// Size
template<class T>
size_t Stack<T>::size() const {
    return stackSize;
}

// Contains
template<class T>
bool Stack<T>::contains(const T& value) const {
    Node* current = topNode;
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
size_t Stack<T>::count(const T& value) const {
    size_t counter = 0;
    Node* current = topNode;
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
T& Stack<T>::at(size_t index) {
    if (index >= stackSize) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = topNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

// At (const)
template<class T>
const T& Stack<T>::at(size_t index) const {
    if (index >= stackSize) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = topNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

// Clear
template<class T>
void Stack<T>::clear() {
    while (!empty()) {
        pop();
    }
}

// Swap
template<class T>
void Stack<T>::swap(Stack& other) noexcept {
    std::swap(topNode, other.topNode);
    std::swap(stackSize, other.stackSize);
}

// Remove all
template<class T>
size_t Stack<T>::removeAll(const T& value) {
    size_t removed = 0;
    Stack<T> temp;
    
    while (!empty()) {
        if (top() != value) {
            temp.push(popAndReturn());
        } else {
            pop();
            ++removed;
        }
    }
    
    while (!temp.empty()) {
        push(temp.popAndReturn());
    }
    
    return removed;
}

// Remove first
template<class T>
bool Stack<T>::removeFirst(const T& value) {
    Stack<T> temp;
    bool found = false;
    
    while (!empty() && !found) {
        if (top() == value) {
            pop();
            found = true;
        } else {
            temp.push(popAndReturn());
        }
    }
    
    while (!temp.empty()) {
        push(temp.popAndReturn());
    }
    
    return found;
}

// Duplicate
template<class T>
void Stack<T>::duplicate() {
    if (empty()) {
        throw std::underflow_error("Stack is empty");
    }
    push(top());
}

// Reverse
template<class T>
void Stack<T>::reverse() {
    if (stackSize <= 1) return;
    
    std::vector<T> temp;
    while (!empty()) {
        temp.push_back(popAndReturn());
    }
    
    for (const auto& item : temp) {
        push(item);
    }
}

// Find first
template<class T>
int Stack<T>::findFirst(const T& value) const {
    Node* current = topNode;
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
int Stack<T>::findLast(const T& value) const {
    Node* current = topNode;
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
void Stack<T>::forEach(std::function<void(T&)> func) {
    Node* current = topNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

// For each (const)
template<class T>
void Stack<T>::forEach(std::function<void(const T&)> func) const {
    Node* current = topNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

// All of
template<class T>
bool Stack<T>::allOf(std::function<bool(const T&)> predicate) const {
    Node* current = topNode;
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
bool Stack<T>::anyOf(std::function<bool(const T&)> predicate) const {
    Node* current = topNode;
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
std::vector<T> Stack<T>::toVector() const {
    std::vector<T> result;
    result.reserve(stackSize);
    Node* current = topNode;
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

// To vector reversed
template<class T>
std::vector<T> Stack<T>::toVectorReversed() const {
    std::vector<T> result = toVector();
    std::reverse(result.begin(), result.end());
    return result;
}

// Operator ==
template<class T>
bool Stack<T>::operator==(const Stack& other) const {
    if (stackSize != other.stackSize) {
        return false;
    }
    
    Node* current1 = topNode;
    Node* current2 = other.topNode;
    
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
bool Stack<T>::operator!=(const Stack& other) const {
    return !(*this == other);
}

// Print
template<class T>
void Stack<T>::print() const {
    std::cout << "Stack [size=" << stackSize << "]: ";
    if (empty()) {
        std::cout << "(empty)";
    } else {
        std::cout << "TOP -> ";
        Node* current = topNode;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " -> BASE";
    }
    std::cout << std::endl;
}

// Check integrity
template<class T>
bool Stack<T>::checkIntegrity() const {
    size_t count = 0;
    Node* current = topNode;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count == stackSize;
}

// Operador de saída
template<class T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& stack) {
    os << "[";
    typename Stack<T>::Node* current = stack.topNode;
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

#endif // STACK_H