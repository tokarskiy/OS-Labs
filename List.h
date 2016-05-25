
#pragma once
#include <cstddef>

template<class T>
class Node {
private:
	Node<T>* _next;
	Node<T>* _prev;
public:
	///<summary>
	///	Хранимое значение
	///</summary>
	T Element;

	Node(T element) {
		this->Element = element;
		this->_next = NULL;
		this->_prev = NULL;
	}

	///<summary>
	///	Установить указатель на следующий узел		
	///</summary>
	void SetNext(Node<T>* node) {
		_next = node;
		if (node != NULL) {
			node->_prev = this;
		}
	}

	///<summary>
	///	Установить указатель на предыдущий узел
	///</summary>
	void SetPrev(Node<T>* node) {
		_prev = node;
		if (node != NULL) {
			node->_next = this;
		}
	}

	///<summary>
	///	Получить указатель на следующий узел
	///</summary>
	Node<T>* GetNext() {
		return _next;
	}

	///<summary>
	///	Получить указатель на предыдущий узел
	///</summary>
	Node<T>* GetPrev() {
		return _prev;
	}
};
template<class T>
class List {
private:
	Node<T>* _head;
	Node<T>* _tail;
	Node<T>* _current;
	size_t _size;

	
public:
	List() {
		_head = NULL;
		_tail = NULL;
		_size = 0;
	}

	///<summary>
	///	Добавить элемент в конец списка
	///</summary>
	void PushBack(T element) {
		if (_size == 0) {
			_head = new Node<T>(element);
			_tail = _head;
			_current = _head;
			_size++;
			return;
		}

		Node<T>* newnode = new Node<T>(element);
		_tail->SetNext(newnode);
		_tail = newnode;
		_size++;
		_current = _head;
	}

	///<summary>
 	///	Добавить элемент в начало списка
	///</summary>
	void PushFront(T element) {
		if (_size == 0) {
			_head = new Node<T>(element);
			_tail = _head;
			_current = _head;
			_size++;
			return;
		}
		Node<T>* newnode = new Node<T>(element);
		_head->SetPrev(newnode);
		_head = newnode;
		_size++;
		_current = _head;
	}

	///<summary>
	/// 	Удалить элемент из конца списка
	///</summary>
	void PopBack() {
		Node<T>* newTail = _tail->GetPrev();
		_size = _size == 0 ? 0 : _size - 1;
		
		if (_size == 0) {
			_head = NULL;
			_tail = NULL;
			_current = NULL;
			return;
		}
		newTail->SetNext(NULL);
		delete _tail;
		_tail = newTail;
		_current = _head;
	}

	///<summary>
	///	Удалить элемент из начала списка
	///</summary>
	void PopFront() {
		Node<T>* newHead = _head->GetNext();
		_size = _size == 0 ? 0 : _size - 1;
		
		if (_size == 0) {
			_head = NULL;
			_tail = NULL;
			_current = NULL;
			return;
		}
		newHead->SetPrev(NULL);
		delete _head;
		_head = newHead;
		_current = _head;
	}

	///<summary>
	///	Перейти к следующему элементу
	///</summary>
	void IncCurrent() {
		_current = _current->GetNext();
	}

	///<summary>
	///	Перейти к предыдущему элементу
	///</summary>
	void DecCurrent() {
		_current = _current->GetPrev();
	}

	///<summary>
	///	Проверить, является ли текущий элемент NULL
	///</summary>
	bool IsCurrentNull() {
		if (_current == NULL) {
			_current = _head;
			return true;
		}
		return false;
	}

	///<summary>
	///	Установить индекс текущего элемента
	///</summary>
	bool SetCurrentIndex(size_t index) {
		if (index >= _size) {
			return false;
		}

		_current = _head;
		for (size_t i = 0; i < index; i++) {
			this->IncCurrent();
		}
	}

	///<summary>
	///	К началу
	///</summary>
	bool GoToStart() {
		return this->SetCurrentIndex(0);
	}

	///<summary>
	///	К концу
	///</summary>
	bool GoToEnd() {
		return this->SetCurrentIndex(_size - 1);
	}

	///<summary>
	///	Возвратить текущий элемент
	///</summary>
	T Current() {
		return _current->Element;
	}

	///<summary>
	///	Размер
	///</summary>
	size_t Size() {
		return _size;
	}
	
	///<summary>
	///	Проверить, является ли список пустым
	///</summary>
	bool Empty() {
		return _size == 0;
	}
};
