#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <iterator>

template<class T>
class List {
private:
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;
        
        Node();
        Node(const T& value);
        Node(T&& value);
        
        // Destrutor
        ~Node() = default;
    };
    
    Node* headNode;
    Node* tailNode;
    size_t listSize;
    bool isSorted; // Flag para otimizar operações em listas ordenadas
    
    // Métodos auxiliares privados
    Node* getNodeAt(size_t index) const;
    void insertAfter(Node* node, Node* newNode);
    void insertBefore(Node* node, Node* newNode);
    void removeNode(Node* node);
    Node* partition(Node* low, Node* high);
    void quickSortRec(Node* low, Node* high);
    Node* merge(Node* left, Node* right);
    Node* mergeSort(Node* head);
    size_t getNodeIndex(Node* node) const;
    
public:
    // ==================== ITERADORES ====================
    class Iterator {
    private:
        Node* current;
        friend class List;
        
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator(Node* node = nullptr) : current(node) {}
        
        T& operator*() { return current->data; }
        T* operator->() { return &current->data; }
        
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            current = current->next;
            return temp;
        }
        
        Iterator& operator--() {
            current = current->prev;
            return *this;
        }
        
        Iterator operator--(int) {
            Iterator temp = *this;
            current = current->prev;
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    class ConstIterator {
    private:
        const Node* current;
        friend class List;
        
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        ConstIterator(const Node* node = nullptr) : current(node) {}
        
        const T& operator*() const { return current->data; }
        const T* operator->() const { return &current->data; }
        
        ConstIterator& operator++() {
            current = current->next;
            return *this;
        }
        
        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            current = current->next;
            return temp;
        }
        
        ConstIterator& operator--() {
            current = current->prev;
            return *this;
        }
        
        ConstIterator operator--(int) {
            ConstIterator temp = *this;
            current = current->prev;
            return temp;
        }
        
        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };
    
    // ==================== CONSTRUTORES E DESTRUTOR ====================
    // Construtor padrão
    List();
    
    // Construtor de cópia
    List(const List& other);
    
    // Construtor de movimento
    List(List&& other) noexcept;
    
    // Construtor com lista de inicialização
    List(std::initializer_list<T> init);
    
    // Construtor com tamanho e valor padrão
    List(size_t count, const T& value = T{});
    
    // Destrutor
    ~List();
    
    // ==================== OPERADORES DE ATRIBUIÇÃO ====================
    
    // Operador de atribuição por cópia
    List& operator=(const List& other);
    
    // Operador de atribuição por movimento
    List& operator=(List&& other) noexcept;
    
    // ==================== ITERADORES ====================
    
    Iterator begin() { return Iterator(headNode); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(headNode); }
    ConstIterator end() const { return ConstIterator(nullptr); }
    ConstIterator cbegin() const { return ConstIterator(headNode); }
    ConstIterator cend() const { return ConstIterator(nullptr); }
    
    // Iteradores reversos
    Iterator rbegin() { return Iterator(tailNode); }
    Iterator rend() { return Iterator(nullptr); }
    ConstIterator rbegin() const { return ConstIterator(tailNode); }
    ConstIterator rend() const { return ConstIterator(nullptr); }
    
    // ==================== MÉTODOS DE INSERÇÃO ====================
    
    // Insere no início
    void pushFront(const T& value);
    void pushFront(T&& value);
    
    // Insere no final
    void pushBack(const T& value);
    void pushBack(T&& value);
    
    // Insere em posição específica
    void insert(size_t index, const T& value);
    void insert(size_t index, T&& value);
    Iterator insert(Iterator pos, const T& value);
    Iterator insert(Iterator pos, T&& value);
    
    // Construção in-place
    template<typename... Args>
    void emplaceFront(Args&&... args);
    
    template<typename... Args>
    void emplaceBack(Args&&... args);
    
    template<typename... Args>
    Iterator emplace(Iterator pos, Args&&... args);
    
    // Inserção ordenada (para listas ordenadas)
    void insertSorted(const T& value);
    void insertSorted(T&& value);
    
    // ==================== MÉTODOS DE REMOÇÃO ====================
    
    // Remove do início
    void popFront();
    T popFrontAndReturn();
    
    // Remove do final
    void popBack();
    T popBackAndReturn();
    
    // Remove por índice
    void removeAt(size_t index);
    T removeAtAndReturn(size_t index);
    
    // Remove por iterador
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);
    
    // Remove primeira ocorrência
    bool removeFirst(const T& value);
    
    // Remove última ocorrência
    bool removeLast(const T& value);
    
    // Remove todas as ocorrências
    size_t removeAll(const T& value);
    
    // Remove elementos que satisfazem condição
    template<typename Predicate>
    size_t removeIf(Predicate pred);
    
    // ==================== MÉTODOS DE ACESSO ====================
    
    // Acesso por índice
    T& at(size_t index);
    const T& at(size_t index) const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    
    // Primeiro e último elemento
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    
    // ==================== MÉTODOS DE CONSULTA ====================
    
    // Tamanho e estado
    size_t size() const;
    bool empty() const;
    bool sorted() const;
    
    // Busca linear
    bool contains(const T& value) const;
    size_t count(const T& value) const;
    int findFirst(const T& value) const;
    int findLast(const T& value) const;
    Iterator find(const T& value);
    ConstIterator find(const T& value) const;
    
    // Busca binária (apenas para listas ordenadas)
    bool binarySearch(const T& value) const;
    int binarySearchIndex(const T& value) const;
    Iterator binaryFind(const T& value);
    ConstIterator binaryFind(const T& value) const;
    
    // ==================== MÉTODOS DE ORDENAÇÃO ====================
    
    // Ordenação
    void sort();
    void sort(std::function<bool(const T&, const T&)> comparator);
    
    // Verifica se está ordenada
    bool isSortedCheck() const;
    bool isSortedCheck(std::function<bool(const T&, const T&)> comparator) const;
    
    // Merge de duas listas ordenadas
    void merge(List& other);
    void merge(List& other, std::function<bool(const T&, const T&)> comparator);
    
    // ==================== OPERAÇÕES DE MANIPULAÇÃO ====================
    
    // Limpa a lista
    void clear();
    
    // Inverte a lista
    void reverse();
    
    // Troca conteúdo
    void swap(List& other) noexcept;
    
    // Redimensiona
    void resize(size_t newSize, const T& value = T{});
    
    // Remove duplicatas
    void unique();
    void unique(std::function<bool(const T&, const T&)> comparator);
    
    // ==================== MÉTODOS FUNCIONAIS ====================
    
    // Aplica função a todos os elementos
    void forEach(std::function<void(T&)> func);
    void forEach(std::function<void(const T&)> func) const;
    
    // Predicados
    bool allOf(std::function<bool(const T&)> predicate) const;
    bool anyOf(std::function<bool(const T&)> predicate) const;
    bool noneOf(std::function<bool(const T&)> predicate) const;
    
    // Transformação
    template<typename U>
    List<U> map(std::function<U(const T&)> mapper) const;
    
    // Filtro
    List<T> filter(std::function<bool(const T&)> predicate) const;
    
    // Redução
    template<typename U>
    U reduce(U initial, std::function<U(U, const T&)> reducer) const;
    
    // ==================== CONVERSÕES ====================
    
    // Converte para vetor
    std::vector<T> toVector() const;
    std::vector<T> toVectorReverse() const;
    
    // ==================== OPERADORES DE COMPARAÇÃO ====================
    
    bool operator==(const List& other) const;
    bool operator!=(const List& other) const;
    bool operator<(const List& other) const;
    bool operator<=(const List& other) const;
    bool operator>(const List& other) const;
    bool operator>=(const List& other) const;
    
    // ==================== OPERADOR DE SAÍDA ====================
    
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const List<U>& list);
    
    // ==================== MÉTODOS DE DEBUG ====================
    
    // Imprime estrutura da lista
    void print() const;
    void printReverse() const;
    
    // Verifica integridade da estrutura
    bool checkIntegrity() const;
    
    // Estatísticas
    void printStats() const;
};

// ==================== IMPLEMENTAÇÕES INLINE ====================

// Construtores da classe Node
template<class T>
List<T>::Node::Node() : next(nullptr), prev(nullptr) {}

template<class T>
List<T>::Node::Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

template<class T>
List<T>::Node::Node(T&& value) : data(std::move(value)), next(nullptr), prev(nullptr) {}

// Construtor padrão
template<class T>
List<T>::List() : headNode(nullptr), tailNode(nullptr), listSize(0), isSorted(true) {}

// Construtor de cópia
template<class T>
List<T>::List(const List& other) 
    : headNode(nullptr), tailNode(nullptr), listSize(0), isSorted(other.isSorted) {
    *this = other;
}

// Construtor de movimento
template<class T>
List<T>::List(List&& other) noexcept 
    : headNode(other.headNode), tailNode(other.tailNode), listSize(other.listSize), isSorted(other.isSorted) {
    other.headNode = nullptr;
    other.tailNode = nullptr;
    other.listSize = 0;
    other.isSorted = true;
}

// Construtor com lista de inicialização
template<class T>
List<T>::List(std::initializer_list<T> init) 
    : headNode(nullptr), tailNode(nullptr), listSize(0), isSorted(true) {
    for (const auto& item : init) {
        pushBack(item);
    }
}

// Construtor com tamanho e valor
template<class T>
List<T>::List(size_t count, const T& value) 
    : headNode(nullptr), tailNode(nullptr), listSize(0), isSorted(true) {
    for (size_t i = 0; i < count; ++i) {
        pushBack(value);
    }
}

// Destrutor
template<class T>
List<T>::~List() {
    clear();
}

// Operador de atribuição por cópia
template<class T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        clear();
        Node* current = other.headNode;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
        isSorted = other.isSorted;
    }
    return *this;
}

// Operador de atribuição por movimento
template<class T>
List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
        clear();
        headNode = other.headNode;
        tailNode = other.tailNode;
        listSize = other.listSize;
        isSorted = other.isSorted;
        other.headNode = nullptr;
        other.tailNode = nullptr;
        other.listSize = 0;
        other.isSorted = true;
    }
    return *this;
}

// Método auxiliar para obter nó por índice
template<class T>
typename List<T>::Node* List<T>::getNodeAt(size_t index) const {
    if (index >= listSize) {
        throw std::out_of_range("Index out of range");
    }
    
    Node* current;
    // Otimização: começar do lado mais próximo
    if (index < listSize / 2) {
        current = headNode;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tailNode;
        for (size_t i = listSize - 1; i > index; --i) {
            current = current->prev;
        }
    }
    return current;
}

// Push front
template<class T>
void List<T>::pushFront(const T& value) {
    Node* newNode = new Node(value);
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        newNode->next = headNode;
        headNode->prev = newNode;
        headNode = newNode;
        
        // Verifica se ainda está ordenada
        if (isSorted && listSize > 0 && value > newNode->next->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

template<class T>
void List<T>::pushFront(T&& value) {
    Node* newNode = new Node(std::move(value));
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        newNode->next = headNode;
        headNode->prev = newNode;
        headNode = newNode;
        
        // Verifica se ainda está ordenada
        if (isSorted && listSize > 0 && newNode->data > newNode->next->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

// Push back
template<class T>
void List<T>::pushBack(const T& value) {
    Node* newNode = new Node(value);
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        tailNode->next = newNode;
        newNode->prev = tailNode;
        tailNode = newNode;
        
        // Verifica se ainda está ordenada
        if (isSorted && listSize > 0 && value < newNode->prev->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

template<class T>
void List<T>::pushBack(T&& value) {
    Node* newNode = new Node(std::move(value));
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        tailNode->next = newNode;
        newNode->prev = tailNode;
        tailNode = newNode;
        
        // Verifica se ainda está ordenada
        if (isSorted && listSize > 0 && newNode->data < newNode->prev->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

// Insert por índice
template<class T>
void List<T>::insert(size_t index, const T& value) {
    if (index > listSize) {
        throw std::out_of_range("Index out of range");
    }
    
    if (index == 0) {
        pushFront(value);
    } else if (index == listSize) {
        pushBack(value);
    } else {
        Node* current = getNodeAt(index);
        Node* newNode = new Node(value);
        
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        
        ++listSize;
        isSorted = false; // Inserção no meio quebra ordenação
    }
}

template<class T>
void List<T>::insert(size_t index, T&& value) {
    if (index > listSize) {
        throw std::out_of_range("Index out of range");
    }
    
    if (index == 0) {
        pushFront(std::move(value));
    } else if (index == listSize) {
        pushBack(std::move(value));
    } else {
        Node* current = getNodeAt(index);
        Node* newNode = new Node(std::move(value));
        
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        
        ++listSize;
        isSorted = false;
    }
}

// Insert por iterador
template<class T>
typename List<T>::Iterator List<T>::insert(Iterator pos, const T& value) {
    if (pos.current == nullptr) {
        pushBack(value);
        return Iterator(tailNode);
    }
    
    if (pos.current == headNode) {
        pushFront(value);
        return Iterator(headNode);
    }
    
    Node* newNode = new Node(value);
    Node* current = pos.current;
    
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    
    ++listSize;
    isSorted = false;
    
    return Iterator(newNode);
}

template<class T>
typename List<T>::Iterator List<T>::insert(Iterator pos, T&& value) {
    if (pos.current == nullptr) {
        pushBack(std::move(value));
        return Iterator(tailNode);
    }
    
    if (pos.current == headNode) {
        pushFront(std::move(value));
        return Iterator(headNode);
    }
    
    Node* newNode = new Node(std::move(value));
    Node* current = pos.current;
    
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    
    ++listSize;
    isSorted = false;
    
    return Iterator(newNode);
}

// Emplace methods
template<class T>
template<typename... Args>
void List<T>::emplaceFront(Args&&... args) {
    Node* newNode = new Node();
    newNode->data = T(std::forward<Args>(args)...);
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        newNode->next = headNode;
        headNode->prev = newNode;
        headNode = newNode;
        
        if (isSorted && listSize > 0 && newNode->data > newNode->next->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

template<class T>
template<typename... Args>
void List<T>::emplaceBack(Args&&... args) {
    Node* newNode = new Node();
    newNode->data = T(std::forward<Args>(args)...);
    
    if (empty()) {
        headNode = tailNode = newNode;
    } else {
        tailNode->next = newNode;
        newNode->prev = tailNode;
        tailNode = newNode;
        
        if (isSorted && listSize > 0 && newNode->data < newNode->prev->data) {
            isSorted = false;
        }
    }
    ++listSize;
}

template<class T>
template<typename... Args>
typename List<T>::Iterator List<T>::emplace(Iterator pos, Args&&... args) {
    Node* newNode = new Node();
    newNode->data = T(std::forward<Args>(args)...);
    
    if (pos.current == nullptr) {
        if (empty()) {
            headNode = tailNode = newNode;
        } else {
            tailNode->next = newNode;
            newNode->prev = tailNode;
            tailNode = newNode;
        }
    } else if (pos.current == headNode) {
        newNode->next = headNode;
        headNode->prev = newNode;
        headNode = newNode;
    } else {
        Node* current = pos.current;
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }
    
    ++listSize;
    isSorted = false;
    
    return Iterator(newNode);
}

// Insert sorted
template<class T>
void List<T>::insertSorted(const T& value) {
    if (!isSorted) {
        sort();
    }
    
    if (empty() || value <= headNode->data) {
        pushFront(value);
        return;
    }
    
    if (value >= tailNode->data) {
        pushBack(value);
        return;
    }
    
    Node* current = headNode;
    while (current != nullptr && current->data < value) {
        current = current->next;
    }
    
    Node* newNode = new Node(value);
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    
    ++listSize;
    // Mantém isSorted = true pois inserimos ordenadamente
}

template<class T>
void List<T>::insertSorted(T&& value) {
    if (!isSorted) {
        sort();
    }
    
    if (empty() || value <= headNode->data) {
        pushFront(std::move(value));
        return;
    }
    
    if (value >= tailNode->data) {
        pushBack(std::move(value));
        return;
    }
    
    Node* current = headNode;
    while (current != nullptr && current->data < value) {
        current = current->next;
    }
    
    Node* newNode = new Node(std::move(value));
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    
    ++listSize;
}

// Pop front
template<class T>
void List<T>::popFront() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    
    Node* temp = headNode;
    
    if (listSize == 1) {
        headNode = tailNode = nullptr;
    } else {
        headNode = headNode->next;
        headNode->prev = nullptr;
    }
    
    delete temp;
    --listSize;
}

template<class T>
T List<T>::popFrontAndReturn() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    
    T value = headNode->data;
    popFront();
    return value;
}

// Pop back
template<class T>
void List<T>::popBack() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    
    Node* temp = tailNode;
    
    if (listSize == 1) {
        headNode = tailNode = nullptr;
    } else {
        tailNode = tailNode->prev;
        tailNode->next = nullptr;
    }
    
    delete temp;
    --listSize;
}

template<class T>
T List<T>::popBackAndReturn() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    
    T value = tailNode->data;
    popBack();
    return value;
}

// Remove at
template<class T>
void List<T>::removeAt(size_t index) {
    if (index >= listSize) {
        throw std::out_of_range("Index out of range");
    }
    
    if (index == 0) {
        popFront();
    } else if (index == listSize - 1) {
        popBack();
    } else {
        Node* nodeToRemove = getNodeAt(index);
        nodeToRemove->prev->next = nodeToRemove->next;
        nodeToRemove->next->prev = nodeToRemove->prev;
        delete nodeToRemove;
        --listSize;
    }
}

template<class T>
T List<T>::removeAtAndReturn(size_t index) {
    if (index >= listSize) {
        throw std::out_of_range("Index out of range");
    }
    
    T value = at(index);
    removeAt(index);
    return value;
}

// Erase
template<class T>
typename List<T>::Iterator List<T>::erase(Iterator pos) {
    if (pos.current == nullptr) {
        return end();
    }
    
    Node* nodeToRemove = pos.current;
    Node* nextNode = nodeToRemove->next;
    
    if (nodeToRemove == headNode && nodeToRemove == tailNode) {
        headNode = tailNode = nullptr;
    } else if (nodeToRemove == headNode) {
        headNode = headNode->next;
        headNode->prev = nullptr;
    } else if (nodeToRemove == tailNode) {
        tailNode = tailNode->prev;
        tailNode->next = nullptr;
    } else {
        nodeToRemove->prev->next = nodeToRemove->next;
        nodeToRemove->next->prev = nodeToRemove->prev;
    }
    
    delete nodeToRemove;
    --listSize;
    
    return Iterator(nextNode);
}

template<class T>
typename List<T>::Iterator List<T>::erase(Iterator first, Iterator last) {
    while (first != last) {
        first = erase(first);
    }
    return last;
}

// Remove first/last/all
template<class T>
bool List<T>::removeFirst(const T& value) {
    Node* current = headNode;
    while (current != nullptr) {
        if (current->data == value) {
            if (current == headNode && current == tailNode) {
                headNode = tailNode = nullptr;
            } else if (current == headNode) {
                headNode = headNode->next;
                headNode->prev = nullptr;
            } else if (current == tailNode) {
                tailNode = tailNode->prev;
                tailNode->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            --listSize;
            return true;
        }
        current = current->next;
    }
    return false;
}

template<class T>
bool List<T>::removeLast(const T& value) {
    Node* current = tailNode;
    while (current != nullptr) {
        if (current->data == value) {
            if (current == headNode && current == tailNode) {
                headNode = tailNode = nullptr;
            } else if (current == headNode) {
                headNode = headNode->next;
                headNode->prev = nullptr;
            } else if (current == tailNode) {
                tailNode = tailNode->prev;
                tailNode->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            --listSize;
            return true;
        }
        current = current->prev;
    }
    return false;
}

template<class T>
size_t List<T>::removeAll(const T& value) {
    size_t removed = 0;
    Node* current = headNode;
    
    while (current != nullptr) {
        Node* next = current->next;
        if (current->data == value) {
            if (current == headNode && current == tailNode) {
                headNode = tailNode = nullptr;
            } else if (current == headNode) {
                headNode = headNode->next;
                headNode->prev = nullptr;
            } else if (current == tailNode) {
                tailNode = tailNode->prev;
                tailNode->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            --listSize;
            ++removed;
        }
        current = next;
    }
    
    return removed;
}

// Remove if
template<class T>
template<typename Predicate>
size_t List<T>::removeIf(Predicate pred) {
    size_t removed = 0;
    Node* current = headNode;
    
    while (current != nullptr) {
        Node* next = current->next;
        if (pred(current->data)) {
            if (current == headNode && current == tailNode) {
                headNode = tailNode = nullptr;
            } else if (current == headNode) {
                headNode = headNode->next;
                headNode->prev = nullptr;
            } else if (current == tailNode) {
                tailNode = tailNode->prev;
                tailNode->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            --listSize;
            ++removed;
        }
        current = next;
    }
    
    return removed;
}

// Access methods
template<class T>
T& List<T>::at(size_t index) {
    return getNodeAt(index)->data;
}

template<class T>
const T& List<T>::at(size_t index) const {
    return getNodeAt(index)->data;
}

template<class T>
T& List<T>::operator[](size_t index) {
    return at(index);
}

template<class T>
const T& List<T>::operator[](size_t index) const {
    return at(index);
}

template<class T>
T& List<T>::front() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    return headNode->data;
}

template<class T>
const T& List<T>::front() const {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    return headNode->data;
}

template<class T>
T& List<T>::back() {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    return tailNode->data;
}

template<class T>
const T& List<T>::back() const {
    if (empty()) {
        throw std::underflow_error("List is empty");
    }
    return tailNode->data;
}

// Query methods
template<class T>
size_t List<T>::size() const {
    return listSize;
}

template<class T>
bool List<T>::empty() const {
    return listSize == 0;
}

template<class T>
bool List<T>::sorted() const {
    return isSorted;
}

// Linear search
template<class T>
bool List<T>::contains(const T& value) const {
    Node* current = headNode;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<class T>
size_t List<T>::count(const T& value) const {
    size_t counter = 0;
    Node* current = headNode;
    while (current != nullptr) {
        if (current->data == value) {
            ++counter;
        }
        current = current->next;
    }
    return counter;
}

template<class T>
int List<T>::findFirst(const T& value) const {
    Node* current = headNode;
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

template<class T>
int List<T>::findLast(const T& value) const {
    Node* current = tailNode;
    int index = listSize - 1;
    while (current != nullptr) {
        if (current->data == value) {
            return index;
        }
        current = current->prev;
        --index;
    }
    return -1;
}

template<class T>
typename List<T>::Iterator List<T>::find(const T& value) {
    Node* current = headNode;
    while (current != nullptr) {
        if (current->data == value) {
            return Iterator(current);
        }
        current = current->next;
    }
    return end();
}

template<class T>
typename List<T>::ConstIterator List<T>::find(const T& value) const {
    Node* current = headNode;
    while (current != nullptr) {
        if (current->data == value) {
            return ConstIterator(current);
        }
        current = current->next;
    }
    return end();
}

// Binary search (apenas para listas ordenadas)
template<class T>
bool List<T>::binarySearch(const T& value) const {
    if (!isSorted) {
        throw std::logic_error("List must be sorted for binary search");
    }
    
    if (empty()) {
        return false;
    }
    
    size_t left = 0;
    size_t right = listSize - 1;
    
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        T midValue = at(mid);
        
        if (midValue == value) {
            return true;
        } else if (midValue < value) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    
    return false;
}

template<class T>
int List<T>::binarySearchIndex(const T& value) const {
    if (!isSorted) {
        throw std::logic_error("List must be sorted for binary search");
    }
    
    if (empty()) {
        return -1;
    }
    
    size_t left = 0;
    size_t right = listSize - 1;
    
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        T midValue = at(mid);
        
        if (midValue == value) {
            return static_cast<int>(mid);
        } else if (midValue < value) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    
    return -1;
}

template<class T>
typename List<T>::Iterator List<T>::binaryFind(const T& value) {
    int index = binarySearchIndex(value);
    if (index == -1) {
        return end();
    }
    return Iterator(getNodeAt(index));
}

template<class T>
typename List<T>::ConstIterator List<T>::binaryFind(const T& value) const {
    int index = binarySearchIndex(value);
    if (index == -1) {
        return end();
    }
    return ConstIterator(getNodeAt(index));
}

// Sort methods
template<class T>
void List<T>::sort() {
    if (listSize <= 1) {
        isSorted = true;
        return;
    }
    
    headNode = mergeSort(headNode);
    
    // Reconstrói o tailNode e links prev
    tailNode = headNode;
    Node* current = headNode;
    current->prev = nullptr;
    
    while (current->next != nullptr) {
        current->next->prev = current;
        current = current->next;
        tailNode = current;
    }
    
    isSorted = true;
}

template<class T>
void List<T>::sort(std::function<bool(const T&, const T&)> comparator) {
    if (listSize <= 1) {
        isSorted = false; // Não sabemos se está ordenada com comparador customizado
        return;
    }
    
    // Implementação usando vector temporário para ordenação customizada
    std::vector<T> temp;
    temp.reserve(listSize);
    
    Node* current = headNode;
    while (current != nullptr) {
        temp.push_back(current->data);
        current = current->next;
    }
    
    std::sort(temp.begin(), temp.end(), comparator);
    
    current = headNode;
    for (const auto& item : temp) {
        current->data = item;
        current = current->next;
    }
    
    isSorted = false; // Não sabemos se está ordenada com comparador padrão
}

// Merge sort recursivo
template<class T>
typename List<T>::Node* List<T>::mergeSort(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // Encontra o meio da lista
    Node* slow = head;
    Node* fast = head;
    Node* prev = nullptr;
    
    while (fast != nullptr && fast->next != nullptr) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // Quebra a lista no meio
    prev->next = nullptr;
    
    // Ordena recursivamente as duas metades
    Node* left = mergeSort(head);
    Node* right = mergeSort(slow);
    
    // Merge das duas metades ordenadas
    return merge(left, right);
}

template<class T>
typename List<T>::Node* List<T>::merge(Node* left, Node* right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;
    
    Node* result = nullptr;
    
    if (left->data <= right->data) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

// Is sorted check
template<class T>
bool List<T>::isSortedCheck() const {
    if (listSize <= 1) {
        return true;
    }
    
    Node* current = headNode;
    while (current->next != nullptr) {
        if (current->data > current->next->data) {
            return false;
        }
        current = current->next;
    }
    
    return true;
}

template<class T>
bool List<T>::isSortedCheck(std::function<bool(const T&, const T&)> comparator) const {
    if (listSize <= 1) {
        return true;
    }
    
    Node* current = headNode;
    while (current->next != nullptr) {
        if (!comparator(current->data, current->next->data)) {
            return false;
        }
        current = current->next;
    }
    
    return true;
}

// Merge with another list
template<class T>
void List<T>::merge(List& other) {
    if (!isSorted) sort();
    if (!other.isSorted) other.sort();
    
    List<T> result;
    
    Node* current1 = headNode;
    Node* current2 = other.headNode;
    
    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data <= current2->data) {
            result.pushBack(current1->data);
            current1 = current1->next;
        } else {
            result.pushBack(current2->data);
            current2 = current2->next;
        }
    }
    
    while (current1 != nullptr) {
        result.pushBack(current1->data);
        current1 = current1->next;
    }
    
    while (current2 != nullptr) {
        result.pushBack(current2->data);
        current2 = current2->next;
    }
    
    *this = std::move(result);
    other.clear();
}

template<class T>
void List<T>::merge(List& other, std::function<bool(const T&, const T&)> comparator) {
    List<T> result;
    
    Node* current1 = headNode;
    Node* current2 = other.headNode;
    
    while (current1 != nullptr && current2 != nullptr) {
        if (comparator(current1->data, current2->data)) {
            result.pushBack(current1->data);
            current1 = current1->next;
        } else {
            result.pushBack(current2->data);
            current2 = current2->next;
        }
    }
    
    while (current1 != nullptr) {
        result.pushBack(current1->data);
        current1 = current1->next;
    }
    
    while (current2 != nullptr) {
        result.pushBack(current2->data);
        current2 = current2->next;
    }
    
    *this = std::move(result);
    other.clear();
}

// Clear
template<class T>
void List<T>::clear() {
    while (!empty()) {
        popFront();
    }
    isSorted = true;
}

// Reverse
template<class T>
void List<T>::reverse() {
    if (listSize <= 1) {
        return;
    }
    
    Node* current = headNode;
    Node* temp = nullptr;
    
    while (current != nullptr) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    if (temp != nullptr) {
        headNode = temp->prev;
    }
    
    std::swap(headNode, tailNode);
    isSorted = false;
}

// Swap
template<class T>
void List<T>::swap(List& other) noexcept {
    std::swap(headNode, other.headNode);
    std::swap(tailNode, other.tailNode);
    std::swap(listSize, other.listSize);
    std::swap(isSorted, other.isSorted);
}

// Resize
template<class T>
void List<T>::resize(size_t newSize, const T& value) {
    if (newSize < listSize) {
        while (listSize > newSize) {
            popBack();
        }
    } else if (newSize > listSize) {
        while (listSize < newSize) {
            pushBack(value);
        }
    }
}

// Unique
template<class T>
void List<T>::unique() {
    if (listSize <= 1) {
        return;
    }
    
    Node* current = headNode;
    while (current != nullptr && current->next != nullptr) {
        if (current->data == current->next->data) {
            Node* duplicate = current->next;
            current->next = duplicate->next;
            if (duplicate->next != nullptr) {
                duplicate->next->prev = current;
            } else {
                tailNode = current;
            }
            delete duplicate;
            --listSize;
        } else {
            current = current->next;
        }
    }
}

template<class T>
void List<T>::unique(std::function<bool(const T&, const T&)> comparator) {
    if (listSize <= 1) {
        return;
    }
    
    Node* current = headNode;
    while (current != nullptr && current->next != nullptr) {
        if (comparator(current->data, current->next->data)) {
            Node* duplicate = current->next;
            current->next = duplicate->next;
            if (duplicate->next != nullptr) {
                duplicate->next->prev = current;
            } else {
                tailNode = current;
            }
            delete duplicate;
            --listSize;
        } else {
            current = current->next;
        }
    }
}

// Functional methods
template<class T>
void List<T>::forEach(std::function<void(T&)> func) {
    Node* current = headNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

template<class T>
void List<T>::forEach(std::function<void(const T&)> func) const {
    Node* current = headNode;
    while (current != nullptr) {
        func(current->data);
        current = current->next;
    }
}

template<class T>
bool List<T>::allOf(std::function<bool(const T&)> predicate) const {
    Node* current = headNode;
    while (current != nullptr) {
        if (!predicate(current->data)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

template<class T>
bool List<T>::anyOf(std::function<bool(const T&)> predicate) const {
    Node* current = headNode;
    while (current != nullptr) {
        if (predicate(current->data)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<class T>
bool List<T>::noneOf(std::function<bool(const T&)> predicate) const {
    return !anyOf(predicate);
}

template<class T>
template<typename U>
List<U> List<T>::map(std::function<U(const T&)> mapper) const {
    List<U> result;
    Node* current = headNode;
    while (current != nullptr) {
        result.pushBack(mapper(current->data));
        current = current->next;
    }
    return result;
}

template<class T>
List<T> List<T>::filter(std::function<bool(const T&)> predicate) const {
    List<T> result;
    Node* current = headNode;
    while (current != nullptr) {
        if (predicate(current->data)) {
            result.pushBack(current->data);
        }
        current = current->next;
    }
    return result;
}

template<class T>
template<typename U>
U List<T>::reduce(U initial, std::function<U(U, const T&)> reducer) const {
    U result = initial;
    Node* current = headNode;
    while (current != nullptr) {
        result = reducer(result, current->data);
        current = current->next;
    }
    return result;
}

// Conversions
template<class T>
std::vector<T> List<T>::toVector() const {
    std::vector<T> result;
    result.reserve(listSize);
    Node* current = headNode;
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

template<class T>
std::vector<T> List<T>::toVectorReverse() const {
    std::vector<T> result;
    result.reserve(listSize);
    Node* current = tailNode;
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->prev;
    }
    return result;
}

// Comparison operators
template<class T>
bool List<T>::operator==(const List& other) const {
    if (listSize != other.listSize) {
        return false;
    }
    
    Node* current1 = headNode;
    Node* current2 = other.headNode;
    
    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data != current2->data) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    
    return current1 == nullptr && current2 == nullptr;
}

template<class T>
bool List<T>::operator!=(const List& other) const {
    return !(*this == other);
}

template<class T>
bool List<T>::operator<(const List& other) const {
    Node* current1 = headNode;
    Node* current2 = other.headNode;
    
    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data < current2->data) {
            return true;
        } else if (current1->data > current2->data) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    
    return current1 == nullptr && current2 != nullptr;
}

template<class T>
bool List<T>::operator<=(const List& other) const {
    return *this < other || *this == other;
}

template<class T>
bool List<T>::operator>(const List& other) const {
    return !(*this <= other);
}

template<class T>
bool List<T>::operator>=(const List& other) const {
    return !(*this < other);
}

// Print methods
template<class T>
void List<T>::print() const {
    std::cout << "List [size=" << listSize << ", sorted=" << 
                 (isSorted ? "true" : "false") << "]: ";
    if (empty()) {
        std::cout << "(empty)";
    } else {
        std::cout << "HEAD <-> ";
        Node* current = headNode;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " <-> ";
            }
            current = current->next;
        }
        std::cout << " <-> TAIL";
    }
    std::cout << std::endl;
}

template<class T>
void List<T>::printReverse() const {
    std::cout << "List (reverse) [size=" << listSize << "]: ";
    if (empty()) {
        std::cout << "(empty)";
    } else {
        std::cout << "TAIL <-> ";
        Node* current = tailNode;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->prev != nullptr) {
                std::cout << " <-> ";
            }
            current = current->prev;
        }
        std::cout << " <-> HEAD";
    }
    std::cout << std::endl;
}

// Check integrity
template<class T>
bool List<T>::checkIntegrity() const {
    if (listSize == 0) {
        return headNode == nullptr && tailNode == nullptr;
    }
    
    if (listSize == 1) {
        return headNode == tailNode && headNode != nullptr && 
               headNode->prev == nullptr && headNode->next == nullptr;
    }
    
    // Verifica se o tamanho está correto
    size_t forwardCount = 0;
    Node* current = headNode;
    while (current != nullptr) {
        ++forwardCount;
        if (current->next == nullptr && current != tailNode) {
            return false;
        }
        current = current->next;
    }
    
    if (forwardCount != listSize) {
        return false;
    }
    
    // Verifica links reversos
    size_t backwardCount = 0;
    current = tailNode;
    while (current != nullptr) {
        ++backwardCount;
        if (current->prev == nullptr && current != headNode) {
            return false;
        }
        current = current->prev;
    }
    
    if (backwardCount != listSize) {
        return false;
    }
    
    // Verifica consistência dos links
    current = headNode;
    while (current != nullptr && current->next != nullptr) {
        if (current->next->prev != current) {
            return false;
        }
        current = current->next;
    }
    
    return true;
}

// Print stats
template<class T>
void List<T>::printStats() const {
    std::cout << "=== List Statistics ===" << std::endl;
    std::cout << "Size: " << listSize << std::endl;
    std::cout << "Empty: " << (empty() ? "Yes" : "No") << std::endl;
    std::cout << "Sorted: " << (isSorted ? "Yes" : "No") << std::endl;
    if (!empty()) {
        std::cout << "Front element: " << headNode->data << std::endl;
        std::cout << "Back element: " << tailNode->data << std::endl;
    }
    std::cout << "Integrity check: " << (checkIntegrity() ? "PASSED" : "FAILED") << std::endl;
    std::cout << "==================================" << std::endl;
}

// Output operator
template<class T>
std::ostream& operator<<(std::ostream& os, const List<T>& list) {
    os << "[";
    typename List<T>::Node* current = list.headNode;
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

#endif // LIST_H