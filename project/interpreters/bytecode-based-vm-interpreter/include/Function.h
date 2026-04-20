#pragma once
#include <memory>
#include <string>
#include <vector>

class EnvScope;
struct Chunk;

namespace json {

class Function {
public:
    Function(std::shared_ptr<EnvScope> closure,
             std::string name,
             std::vector<std::string> params,
             std::shared_ptr<Chunk> body);
    ~Function() = default;
    Function(const Function&) = default;
    Function& operator=(const Function&) = default;

    std::shared_ptr<EnvScope> getScope() const;
    void clearScope();

    const std::string&              get_text()       const { return mName; }
    const std::vector<std::string>& get_params()     const { return mParams; }
    const Chunk&                    get_body()       const { return *mBody; }
    std::shared_ptr<Chunk>          get_body_chunk() const { return mBody; }

private:
    std::string              mName;
    std::vector<std::string> mParams;
    std::shared_ptr<Chunk>   mBody;
    std::shared_ptr<EnvScope> mClosure;
};

inline bool operator==(const Function& lhs, const Function& rhs) { return &lhs == &rhs; }

} // namespace json
