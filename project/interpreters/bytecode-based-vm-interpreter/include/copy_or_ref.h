#pragma once

#include <utility>
#include <variant>

template<typename A>
class CopyOrRef {
public:
    enum class Type
    {
        COPY,
        REFERENCE
    };
    CopyOrRef(const A& a)
        : data{ A(a) }
        , mType{ Type::COPY }
    {}

    CopyOrRef(A& a)
        : data{ &a }
        , mType{ Type::REFERENCE }
    {}

    A& get()
    {
        if (std::holds_alternative<A>(data))
            return std::get<A>(data);
        else
            return *std::get<A*>(data);
    }

    const A& get() const
    {
        if (std::holds_alternative<A>(data))
            return std::get<A>(data);
        else
            return *std::get<A*>(data);
    }

    operator A&()
    {
        return get();
    }

    operator const A&() const
    {
        return get();
    }

    CopyOrRef<A> copy()
    {
        return std::as_const(*this).get();
    }

    Type type() const
    {
        return mType;
    }
private:
    std::variant<A, A*> data;
    Type mType;
};
