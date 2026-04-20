#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <cassert>
#include <iomanip>
#include "Function.h"

class Json {
public:
    enum class Type {
        Null,
        Int,
        Double,
        Bool,
        String,
        Array,
        Object,
        Function
    };

private:
    Type type_ = Type::Null;

    union ScalarValue {
        int i;
        double d;
        bool b;
        ScalarValue() {}
        ~ScalarValue() {}
    } scalar_;

    // Arrays and objects use shared_ptr so copies share the same underlying data.
    // This gives reference semantics needed for member assignment (arr[i] = val).
    std::shared_ptr<std::string>                  str_    = nullptr;
    std::shared_ptr<std::vector<Json>>            array_  = nullptr;
    std::shared_ptr<std::map<std::string, Json>>  object_ = nullptr;
    json::Function*                               function_ = nullptr;

    void clear() {
        switch(type_) {
            case Type::String:   str_.reset();    break;
            case Type::Array:    array_.reset();   break;
            case Type::Object:   object_.reset();  break;
            case Type::Function: delete function_; break;
            default: break;
        }
        type_ = Type::Null;
    }

    void copy_from(const Json& other) {
        type_ = other.type_;
        switch(type_) {
            case Type::Int:      scalar_.i = other.scalar_.i; break;
            case Type::Double:   scalar_.d = other.scalar_.d; break;
            case Type::Bool:     scalar_.b = other.scalar_.b; break;
            case Type::String:   str_    = other.str_;    break;  // shared
            case Type::Array:    array_  = other.array_;  break;  // shared
            case Type::Object:   object_ = other.object_; break;  // shared
            case Type::Function: function_ = new json::Function(*other.function_); break;
            default: break;
        }
    }

public:
    Json() : type_(Type::Null) {}

    Json(int v)           : type_(Type::Int)    { scalar_.i = v; }
    Json(double v)        : type_(Type::Double) { scalar_.d = v; }
    Json(bool v)          : type_(Type::Bool)   { scalar_.b = v; }
    Json(const char* v)   : type_(Type::String), str_(std::make_shared<std::string>(v)) {}
    Json(const std::string& v) : type_(Type::String), str_(std::make_shared<std::string>(v)) {}
    Json(const std::vector<Json>& v) : type_(Type::Array), array_(std::make_shared<std::vector<Json>>(v)) {}
    Json(std::vector<Json>&& v) : type_(Type::Array), array_(std::make_shared<std::vector<Json>>(std::move(v))) {}
    Json(const std::map<std::string, Json>& v) : type_(Type::Object), object_(std::make_shared<std::map<std::string, Json>>(v)) {}
    Json(std::map<std::string, Json>&& v) : type_(Type::Object), object_(std::make_shared<std::map<std::string, Json>>(std::move(v))) {}
    Json(const json::Function& v) : type_(Type::Function), function_(new json::Function(v)) {}

    Json(const Json& other) { copy_from(other); }

    Json& operator=(const Json& other) {
        if (this != &other) { clear(); copy_from(other); }
        return *this;
    }

    Json(Json&& other) noexcept {
        type_     = other.type_;
        scalar_   = other.scalar_;
        str_      = std::move(other.str_);
        array_    = std::move(other.array_);
        object_   = std::move(other.object_);
        function_ = other.function_;
        other.type_     = Type::Null;
        other.function_ = nullptr;
    }

    Json& operator=(Json&& other) noexcept {
        if (this != &other) {
            clear();
            type_     = other.type_;
            scalar_   = other.scalar_;
            str_      = std::move(other.str_);
            array_    = std::move(other.array_);
            object_   = std::move(other.object_);
            function_ = other.function_;
            other.type_     = Type::Null;
            other.function_ = nullptr;
        }
        return *this;
    }

    ~Json() { clear(); }

    inline Type type() const { return type_; }

    inline bool is_null()     const { return type_ == Type::Null; }
    inline bool is_int()      const { return type_ == Type::Int; }
    inline bool is_double()   const { return type_ == Type::Double; }
    inline bool is_bool()     const { return type_ == Type::Bool; }
    inline bool is_string()   const { return type_ == Type::String; }
    inline bool is_array()    const { return type_ == Type::Array; }
    inline bool is_object()   const { return type_ == Type::Object; }
    inline bool is_function() const { return type_ == Type::Function; }

    inline int                get_int()    const { assert(is_int());    return scalar_.i; }
    inline double             get_double() const { assert(is_double()); return scalar_.d; }
    inline bool               get_bool()   const { assert(is_bool());   return scalar_.b; }
    inline const std::string& get_string() const { assert(is_string()); return *str_; }

    inline const std::vector<Json>& get_array() const { assert(is_array()); return *array_; }
    inline       std::vector<Json>& get_array()       { assert(is_array()); return *array_; }

    inline const std::map<std::string, Json>& get_object() const { assert(is_object()); return *object_; }
    inline       std::map<std::string, Json>& get_object()       { assert(is_object()); return *object_; }

    inline const json::Function& get_function() const { assert(is_function()); return *function_; }
    inline       json::Function& get_function()       { assert(is_function()); return *function_; }

    Json& operator[](size_t idx) {
        assert(is_array());
        return (*array_)[idx];
    }
    const Json& operator[](size_t idx) const {
        assert(is_array());
        return (*array_)[idx];
    }

    Json& operator[](const std::string& key) {
        if (!is_object()) {
            clear();
            type_   = Type::Object;
            object_ = std::make_shared<std::map<std::string, Json>>();
        }
        return (*object_)[key];
    }
    const Json& operator[](const std::string& key) const {
        assert(is_object());
        auto it = object_->find(key);
        assert(it != object_->end());
        return it->second;
    }

public:
    class iterator {
        using array_iter_t = typename std::vector<Json>::iterator;
        using obj_iter_t   = typename std::map<std::string, Json>::iterator;
        enum class IterType { None, Array, Object };
        IterType iter_type_ = IterType::None;
        union { array_iter_t array_it_; obj_iter_t obj_it_; };
    public:
        using value_type        = Json;
        using difference_type   = ptrdiff_t;
        using reference         = Json&;
        using pointer           = Json*;
        using iterator_category = std::forward_iterator_tag;

        iterator() : iter_type_(IterType::None) {}
        iterator(array_iter_t it) : iter_type_(IterType::Array), array_it_(it) {}
        iterator(obj_iter_t it)   : iter_type_(IterType::Object), obj_it_(it) {}

        iterator(const iterator& other) : iter_type_(other.iter_type_) {
            switch(iter_type_) {
                case IterType::Array:  new(&array_it_) array_iter_t(other.array_it_); break;
                case IterType::Object: new(&obj_it_)   obj_iter_t(other.obj_it_);     break;
                default: break;
            }
        }
        iterator& operator=(const iterator& other) {
            if (this != &other) {
                this->~iterator();
                iter_type_ = other.iter_type_;
                switch(iter_type_) {
                    case IterType::Array:  new(&array_it_) array_iter_t(other.array_it_); break;
                    case IterType::Object: new(&obj_it_)   obj_iter_t(other.obj_it_);     break;
                    default: break;
                }
            }
            return *this;
        }
        ~iterator() {
            switch(iter_type_) {
                case IterType::Array:  array_it_.~array_iter_t(); break;
                case IterType::Object: obj_it_.~obj_iter_t();     break;
                default: break;
            }
            iter_type_ = IterType::None;
        }
        reference operator*() const {
            switch(iter_type_) {
                case IterType::Array:  return *array_it_;
                case IterType::Object: return obj_it_->second;
                default: throw std::runtime_error("Invalid iterator");
            }
        }
        pointer operator->() const {
            switch(iter_type_) {
                case IterType::Array:  return &(*array_it_);
                case IterType::Object: return &(obj_it_->second);
                default: throw std::runtime_error("Invalid iterator");
            }
        }
        iterator& operator++() {
            switch(iter_type_) {
                case IterType::Array:  ++array_it_; break;
                case IterType::Object: ++obj_it_;   break;
                default: throw std::runtime_error("Invalid iterator");
            }
            return *this;
        }
        iterator operator++(int) { iterator tmp(*this); ++(*this); return tmp; }
        bool operator==(const iterator& rhs) const {
            if (iter_type_ != rhs.iter_type_) return false;
            switch(iter_type_) {
                case IterType::Array:  return array_it_ == rhs.array_it_;
                case IterType::Object: return obj_it_ == rhs.obj_it_;
                default: return true;
            }
        }
        bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
        std::string key() const {
            if (iter_type_ != IterType::Object) throw std::runtime_error("Not an object iterator");
            return obj_it_->first;
        }
    };

    class const_iterator {
        using array_citer_t = typename std::vector<Json>::const_iterator;
        using obj_citer_t   = typename std::map<std::string, Json>::const_iterator;
        enum class IterType { None, Array, Object };
        IterType iter_type_ = IterType::None;
        union { array_citer_t array_it_; obj_citer_t obj_it_; };
    public:
        using value_type        = Json;
        using difference_type   = ptrdiff_t;
        using reference         = const Json&;
        using pointer           = const Json*;
        using iterator_category = std::forward_iterator_tag;

        const_iterator() : iter_type_(IterType::None) {}
        const_iterator(array_citer_t it) : iter_type_(IterType::Array), array_it_(it) {}
        const_iterator(obj_citer_t it)   : iter_type_(IterType::Object), obj_it_(it) {}

        const_iterator(const const_iterator& other) : iter_type_(other.iter_type_) {
            switch(iter_type_) {
                case IterType::Array:  new(&array_it_) array_citer_t(other.array_it_); break;
                case IterType::Object: new(&obj_it_)   obj_citer_t(other.obj_it_);     break;
                default: break;
            }
        }
        const_iterator& operator=(const const_iterator& other) {
            if (this != &other) {
                this->~const_iterator();
                iter_type_ = other.iter_type_;
                switch(iter_type_) {
                    case IterType::Array:  new(&array_it_) array_citer_t(other.array_it_); break;
                    case IterType::Object: new(&obj_it_)   obj_citer_t(other.obj_it_);     break;
                    default: break;
                }
            }
            return *this;
        }
        ~const_iterator() {
            switch(iter_type_) {
                case IterType::Array:  array_it_.~array_citer_t(); break;
                case IterType::Object: obj_it_.~obj_citer_t();     break;
                default: break;
            }
            iter_type_ = IterType::None;
        }
        reference operator*() const {
            switch(iter_type_) {
                case IterType::Array:  return *array_it_;
                case IterType::Object: return obj_it_->second;
                default: throw std::runtime_error("Invalid iterator");
            }
        }
        pointer operator->() const {
            switch(iter_type_) {
                case IterType::Array:  return &(*array_it_);
                case IterType::Object: return &(obj_it_->second);
                default: throw std::runtime_error("Invalid iterator");
            }
        }
        const_iterator& operator++() {
            switch(iter_type_) {
                case IterType::Array:  ++array_it_; break;
                case IterType::Object: ++obj_it_;   break;
                default: throw std::runtime_error("Invalid iterator");
            }
            return *this;
        }
        const_iterator operator++(int) { const_iterator tmp(*this); ++(*this); return tmp; }
        bool operator==(const const_iterator& rhs) const {
            if (iter_type_ != rhs.iter_type_) return false;
            switch(iter_type_) {
                case IterType::Array:  return array_it_ == rhs.array_it_;
                case IterType::Object: return obj_it_ == rhs.obj_it_;
                default: return true;
            }
        }
        bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }
        std::string key() const {
            if (iter_type_ != IterType::Object) throw std::runtime_error("Not an object iterator");
            return obj_it_->first;
        }
    };

public:
    iterator begin() {
        if (is_array())  return iterator(array_->begin());
        if (is_object()) return iterator(object_->begin());
        return iterator();
    }
    iterator end() {
        if (is_array())  return iterator(array_->end());
        if (is_object()) return iterator(object_->end());
        return iterator();
    }
    const_iterator begin() const {
        if (is_array())  return const_iterator(array_->begin());
        if (is_object()) return const_iterator(object_->begin());
        return const_iterator();
    }
    const_iterator end() const {
        if (is_array())  return const_iterator(array_->end());
        if (is_object()) return const_iterator(object_->end());
        return const_iterator();
    }
};

inline std::ostream& operator<<(std::ostream& os, const Json& j) {
    switch (j.type()) {
        case Json::Type::Null:   os << "null"; break;
        case Json::Type::Int:    os << j.get_int(); break;
        case Json::Type::Double: os << j.get_double(); break;
        case Json::Type::Bool:   os << (j.get_bool() ? "true" : "false"); break;
        case Json::Type::Function:
            os << "{\"type\":\"function\",\"text\":\"" << j.get_function().get_text() << "\"}";
            break;
        case Json::Type::String: {
            os << '"';
            for (char c : j.get_string()) {
                switch (c) {
                    case '"':  os << "\"";  break;
                    case '\b': os << "\b";  break;
                    case '\f': os << "\f";  break;
                    case '\n': os << "\n";  break;
                    case '\r': os << "\r";  break;
                    case '\t': os << "\t";  break;
                    default:
                        if (static_cast<unsigned char>(c) < 0x20) {
                            os << "\\u" << std::hex << std::uppercase
                               << std::setfill('0') << std::setw(4)
                               << static_cast<int>(static_cast<unsigned char>(c))
                               << std::dec << std::nouppercase;
                        } else { os << c; }
                }
            }
            os << '"';
            break;
        }
        case Json::Type::Array: {
            os << '[';
            bool first = true;
            for (const auto& el : j.get_array()) {
                if (!first) os << ", ";
                os << el;
                first = false;
            }
            os << ']';
            break;
        }
        case Json::Type::Object: {
            os << '{';
            bool first = true;
            for (const auto& kv : j.get_object()) {
                if (!first) os << ", ";
                os << '"' << kv.first << "\": " << kv.second;
                first = false;
            }
            os << '}';
            break;
        }
    }
    return os;
}

inline Json operator+(const Json& lhs, const Json& rhs) {
    if (!lhs.is_double() && !lhs.is_int() && !lhs.is_bool()) return {};
    if (!rhs.is_double() && !rhs.is_int() && !rhs.is_bool()) return {};
    if (lhs.is_double()) {
        if (rhs.is_double()) return Json{lhs.get_double() + rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_double() + rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_double() + rhs.get_bool()};
    } else if (lhs.is_int()) {
        if (rhs.is_double()) return Json{lhs.get_int() + rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_int() + rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_int() + rhs.get_bool()};
    } else if (lhs.is_bool()) {
        if (rhs.is_double()) return Json{lhs.get_bool() + static_cast<bool>(rhs.get_double())};
        if (rhs.is_int())    return Json{lhs.get_bool() + static_cast<bool>(rhs.get_int())};
        if (rhs.is_bool())   return Json{lhs.get_bool() + static_cast<bool>(rhs.get_bool())};
    }
    return {};
}

inline Json operator-(const Json& lhs, const Json& rhs) {
    if (!lhs.is_double() && !lhs.is_int() && !lhs.is_bool()) return {};
    if (!rhs.is_double() && !rhs.is_int() && !rhs.is_bool()) return {};
    if (lhs.is_double()) {
        if (rhs.is_double()) return Json{lhs.get_double() - rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_double() - rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_double() - rhs.get_bool()};
    } else if (lhs.is_int()) {
        if (rhs.is_double()) return Json{lhs.get_int() - rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_int() - rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_int() - rhs.get_bool()};
    } else if (lhs.is_bool()) {
        if (rhs.is_double()) return Json{lhs.get_bool() - static_cast<bool>(rhs.get_double())};
        if (rhs.is_int())    return Json{lhs.get_bool() - static_cast<bool>(rhs.get_int())};
        if (rhs.is_bool())   return Json{lhs.get_bool() - static_cast<bool>(rhs.get_bool())};
    }
    return {};
}

inline Json operator*(const Json& lhs, const Json& rhs) {
    if (!lhs.is_double() && !lhs.is_int() && !lhs.is_bool()) return {};
    if (!rhs.is_double() && !rhs.is_int() && !rhs.is_bool()) return {};
    if (lhs.is_double()) {
        if (rhs.is_double()) return Json{lhs.get_double() * rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_double() * rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_double() * rhs.get_bool()};
    } else if (lhs.is_int()) {
        if (rhs.is_double()) return Json{lhs.get_int() * rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_int() * rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_int() * rhs.get_bool()};
    } else if (lhs.is_bool()) {
        if (rhs.is_double()) return Json{lhs.get_bool() * static_cast<bool>(rhs.get_double())};
        if (rhs.is_int())    return Json{lhs.get_bool() * static_cast<bool>(rhs.get_int())};
        if (rhs.is_bool())   return Json{lhs.get_bool() * static_cast<bool>(rhs.get_bool())};
    }
    return {};
}

inline Json operator/(const Json& lhs, const Json& rhs) {
    if (!lhs.is_double() && !lhs.is_int() && !lhs.is_bool()) return {};
    if (!rhs.is_double() && !rhs.is_int() && !rhs.is_bool()) return {};
    if (lhs.is_double()) {
        if (rhs.is_double()) return Json{lhs.get_double() / rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_double() / rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_double() / rhs.get_bool()};
    } else if (lhs.is_int()) {
        if (rhs.is_double()) return Json{lhs.get_int() / rhs.get_double()};
        if (rhs.is_int())    return Json{lhs.get_int() / rhs.get_int()};
        if (rhs.is_bool())   return Json{lhs.get_int() / rhs.get_bool()};
    } else if (lhs.is_bool()) {
        if (rhs.is_double()) return Json{lhs.get_bool() / static_cast<bool>(rhs.get_double())};
        if (rhs.is_int())    return Json{lhs.get_bool() / static_cast<bool>(rhs.get_int())};
        if (rhs.is_bool())   return Json{lhs.get_bool() / static_cast<bool>(rhs.get_bool())};
    }
    return {};
}

inline bool operator==(const Json& lhs, const Json& rhs) {
    if (lhs.is_double() && rhs.is_double()) return lhs.get_double() == rhs.get_double();
    if (lhs.is_bool()   && rhs.is_bool())   return lhs.get_bool()   == rhs.get_bool();
    if (lhs.is_int()    && rhs.is_int())    return lhs.get_int()    == rhs.get_int();
    if (lhs.is_string() && rhs.is_string()) return lhs.get_string() == rhs.get_string();
    if (lhs.is_array()  && rhs.is_array())  return lhs.get_array()  == rhs.get_array();
    if (lhs.is_object() && rhs.is_object()) return lhs.get_object() == rhs.get_object();
    if (lhs.is_function() && rhs.is_function()) return lhs.get_function() == rhs.get_function();
    if (lhs.is_double() && rhs.is_int()) return static_cast<int>(lhs.get_double()) == rhs.get_int();
    if (lhs.is_int() && rhs.is_double()) return lhs.get_int() == static_cast<int>(rhs.get_double());
    return false;
}

inline bool operator!=(const Json& lhs, const Json& rhs) { return !(lhs == rhs); }

inline bool operator<(const Json& lhs, const Json& rhs) {
    if (lhs.is_double() && rhs.is_double()) return lhs.get_double() < rhs.get_double();
    if (lhs.is_bool()   && rhs.is_bool())   return lhs.get_bool()   < rhs.get_bool();
    if (lhs.is_int()    && rhs.is_int())    return lhs.get_int()    < rhs.get_int();
    if (lhs.is_string() && rhs.is_string()) return lhs.get_string() < rhs.get_string();
    if (lhs.is_double() && rhs.is_int())    return static_cast<int>(lhs.get_double()) < rhs.get_int();
    if (lhs.is_int()    && rhs.is_double()) return lhs.get_int()    < static_cast<int>(rhs.get_double());
    return false;
}

inline bool operator<=(const Json& lhs, const Json& rhs) {
    if (lhs.is_double() && rhs.is_double()) return lhs.get_double() <= rhs.get_double();
    if (lhs.is_bool()   && rhs.is_bool())   return lhs.get_bool()   <= rhs.get_bool();
    if (lhs.is_int()    && rhs.is_int())    return lhs.get_int()    <= rhs.get_int();
    if (lhs.is_string() && rhs.is_string()) return lhs.get_string() <= rhs.get_string();
    if (lhs.is_double() && rhs.is_int())    return static_cast<int>(lhs.get_double()) <= rhs.get_int();
    if (lhs.is_int()    && rhs.is_double()) return lhs.get_int()    <= static_cast<int>(rhs.get_double());
    return false;
}

inline bool operator>(const Json& lhs, const Json& rhs) {
    if (lhs.is_double() && rhs.is_double()) return lhs.get_double() > rhs.get_double();
    if (lhs.is_bool()   && rhs.is_bool())   return lhs.get_bool()   > rhs.get_bool();
    if (lhs.is_int()    && rhs.is_int())    return lhs.get_int()    > rhs.get_int();
    if (lhs.is_string() && rhs.is_string()) return lhs.get_string() > rhs.get_string();
    if (lhs.is_double() && rhs.is_int())    return static_cast<int>(lhs.get_double()) > rhs.get_int();
    if (lhs.is_int()    && rhs.is_double()) return lhs.get_int()    > static_cast<int>(rhs.get_double());
    return false;
}

inline bool operator>=(const Json& lhs, const Json& rhs) {
    if (lhs.is_double() && rhs.is_double()) return lhs.get_double() >= rhs.get_double();
    if (lhs.is_bool()   && rhs.is_bool())   return lhs.get_bool()   >= rhs.get_bool();
    if (lhs.is_int()    && rhs.is_int())    return lhs.get_int()    >= rhs.get_int();
    if (lhs.is_string() && rhs.is_string()) return lhs.get_string() >= rhs.get_string();
    if (lhs.is_double() && rhs.is_int())    return static_cast<int>(lhs.get_double()) >= rhs.get_int();
    if (lhs.is_int()    && rhs.is_double()) return lhs.get_int()    >= static_cast<int>(rhs.get_double());
    return false;
}
