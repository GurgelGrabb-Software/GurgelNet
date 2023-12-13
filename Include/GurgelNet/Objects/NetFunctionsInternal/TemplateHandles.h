#pragma once
#include "GurgelNet/Objects/NetFunctionsInternal/Locality.h"

#include <functional>

// ------------------------------------------------------------
// 0 arguments
// ------------------------------------------------------------

template<ENetFuncLocality loc>
class TNetFuncHandle_0 : public CNetFuncHandle
{
	using FType = std::function<void()>;

public:
	TNetFuncHandle_0(FType f) : CNetFuncHandle(loc), _f(f) {}

	void operator()()
	{
		if (InvokeRemote())
		{
			DoInvokeRemote();
			return;
		}
		_f();
	}

	void RunInvoke(INetMessageReader& r) override
	{
		_f();
	}
private:
	FType _f;
};

// ------------------------------------------------------------
// 1 argument
// ------------------------------------------------------------

template<ENetFuncLocality loc, typename T1>
class TNetFuncHandle_1 : public CNetFuncHandle
{
	using FType = std::function<void(const T1&)>;
public:
	TNetFuncHandle_1(FType f) : CNetFuncHandle(loc), _f(f) {}
	void operator()(const T1& a)
	{
		if (InvokeRemote())
		{
			Write(a);
			DoInvokeRemote();
			return;
		}
		_f(a);
	}
	void RunInvoke(INetMessageReader& r) override
	{
		T1 a;
		r.Read(a);
		_f(a);
	}
private:
	FType _f;
};

// ------------------------------------------------------------
// 2 arguments
// ------------------------------------------------------------

template<ENetFuncLocality loc, typename T1, typename T2>
class TNetFuncHandle_2 : public CNetFuncHandle
{
	using FType = std::function<void(const T1&, const T2&)>;
public:
	TNetFuncHandle_2(FType f) : CNetFuncHandle(loc), _f(f) {}
	void operator()(const T1& a, const T2& b)
	{
		if (InvokeRemote())
		{
			Write(a);
			Write(b);
			DoInvokeRemote();
			return;
		} _f(a, b);
	}

	void RunInvoke(INetMessageReader& r) override
	{
		T1 a; T2 b;
		r.Read(a);
		r.Read(b);
		_f(a, b);
	}
private:
	FType _f;
};

// ------------------------------------------------------------
// 3 arguments
// ------------------------------------------------------------

template<ENetFuncLocality loc, typename T1, typename T2, typename T3>
class TNetFuncHandle_3 : public CNetFuncHandle
{
	using FType = std::function<void(const T1&, const T2&, const T3&)>;
public:
	TNetFuncHandle_3(FType f) : CNetFuncHandle(loc), _f(f) {}
	void operator()(const T1& a, const T2& b, const T3& c) {
		if (InvokeRemote())
		{
			Write(a);
			Write(b);
			Write(c);
			DoInvokeRemote();
		}
		_f(a, b, c);
	}

	void RunInvoke(INetMessageReader& r) override
	{
		T1 a; T2 b; T3 c;
		r.Read(a);
		r.Read(b);
		r.Read(c);
		_f(a, b, c);
	}

private:
	FType _f;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------