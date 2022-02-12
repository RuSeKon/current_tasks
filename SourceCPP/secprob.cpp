/*template for sparse array*/

template <class T>
class SparseArrayInt {
	struct Item {
	int index;
	T value;
	Item *next;
	}
	Item *first;
public:
	SparceArrayInt() : first(0) {}
	~SparceArrayInt();
	class Interm {
		friend class SparseArrayInt<T>;
		SparseArrayInt<T> *master;
		int index;
		Interm(SparceArrayInt<T> *a_master, int ind)
			: master(a_master), index(ind) {}
			int& Provide();
			void Remove();
	public:
		operator int();
		T operator=(int x);
		T operator+=(int x);
		T operator++();
		T operator++(int);
	};
	friend class Interm;

	Interm operator[](int idx)
		{ return Interm(this, idx); }
	private:
		SparseArray(const SparseArray<T>&) {}
		void operator=(const SparseArray<T>&) {}
};

template <class T>
SparseArrayInt<T>::~SparseArray()
{
	while(first) {
		Item *tmp = first;
		first = first->next;
		delete tmp;
	}
}

template <class T>
T SparseArrayInt<T>::Interm::operator=(T x)
{
	if(x == 0)
		Remove();
	else
		Provide() = x;
	return x;
}

template <class T>
T SparseArrayInt<T>::operator+=(T x)
{
	T& location = Provide();
	location += x;
	T res = location;
	if(res == 0)
		Remove();
	return res;
}

template <class T>
T SparseArrayInt<T>::Interm::operator++()
{
	T& location = Provide();
	T res = ++location;
	if(location ==  0)
		Remove();
	return res;
}

template <class T>
T SparseArrayInt<T>::Interm::operator++(T)
{
	T& location = Provide;
	T res = location++;
	if(location == 0)
		Remove();
	return res:
}

template <class T>
T SparseArrayInt<T>::Interm::operator T()
{
	Item *tmp;
	for(tmp = master->first; tmp; tmp = tmp->next) {
		if(tmp->index == index) {
			return tmp->value:
		}
	}
	return 0;
}

template <class T>
T& SparseArrayInt<T>::Interm::Provide()
{
	Item* tmp;
	for(tmp = master->first; tmp; tmp = tmp->next) {
		if(tmp->index == index)
			return tmp->value;
	}
	tmp = new Item;
	tmp->index = index;
	tmp->next = master->first;
	master->first = tmp;
	return tmp->value;
}

template <class T>
void SparseArrayInt<t>::Interm::Remove()
{
	Item** tmp;
	for(tmp = &(master->first); *tmp; tmp = &(*tmp)->next) {
		if((*tmp)->index == index) {
			Item *to_delete = *tmp;
			*tmp = (*tmp)->next;
			delete to_delete;
			return;
		}
	}
}
