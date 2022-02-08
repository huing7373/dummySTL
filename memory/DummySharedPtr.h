#include <atomic>
class SharedCount
{
public:
	SharedCount() :count_{ 1 } {};
	void add() { ++count_; }
	void minus() { --count_; }
	int get() const { return count_; }
private:
	std::atomic<int> count_;
};

template <typename T>
class DummySharedPtr {
public:
	DummySharedPtr(T* ptr) :ptr_{ ptr }, ref_count_{ new SharedCount } {}
	DummySharedPtr() :ptr_{ nullptr }, ref_count_{ new SharedCount } {}
	~DummySharedPtr() { clean(); }
	DummySharedPtr(const DummySharedPtr& p)
	{
		this->ptr_ = p.ptr_;
		this->ref_count_ = p.ref_count_;
		ref_count_->add();
	}
	DummySharedPtr(DummySharedPtr&& p)
	{
		this->ptr_ = p.ptr_;
		this->ref_count_ = p.ref_count_;
		p.ptr_ = nullptr;
		p.ref_count_ = nullptr;
	}
	DummySharedPtr& operator=(const DummySharedPtr& p)
	{
		clean();
		this->ptr_ = p.ptr_;
		this->ref_count_ = p.ref_count_;
		ref_count_->add();
		return *this;
	}
	DummySharedPtr& operator=(DummySharedPtr&& p)
	{
		clean();
		this->ptr_ = p.ptr_;
		this->ref_count_ = p.ref_count_;
		p.ptr_ = nullptr;
		p.ref_count_ = nullptr;
		return *this;
	}
	int use_count() const { return ref_count_->get(); }
	T* get() const { return ptr_; }
	T* operator->()const { return ptr_; }
	T& operator*()const { return *PTR; }
	operator bool() const { return ptr_; }
private:
	void clean()
	{
		if (ref_count_)
		{
			ref_count_->minus();
			if (ref_count_->get() == 0)
			{
				if (ptr_) delete ptr_;
				delete ref_count_;
			}
		}
	}
	T* ptr_;
	SharedCount* ref_count_;
};

template<typename T>
class DummyUniquePtr
{
public:
	DummyUniquePtr(T* ptr) :ptr_{ ptr } {}
	DummyUniquePtr() :ptr_{ nullptr } {}

	DummyUniquePtr(const DummyUniquePtr&) = delete;
	DummyUniquePtr& operator=(const DummyUniquePtr&) = delete;

	DummyUniquePtr(DummyUniquePtr&& p)
	{
		this->ptr = p.ptr;
		p.ptr = nullptr;
	}
	DummyUniquePtr& operator=(DummyUniquePtr&& p)
	{
		clean();
		this->ptr = p.ptr;
		p.ptr = nullptr;
		return *this;
	}
	~DummyUniquePtr() { if (ptr) delete ptr; }

	T* get() const { return ptr; }
	T* operator->() const { return ptr; }
	T& operator*() const { return *ptr; }
	operator bool() const { return ptr; }
private:
	void clean() { if (ptr_) delete ptr_; }
	T* ptr;
};