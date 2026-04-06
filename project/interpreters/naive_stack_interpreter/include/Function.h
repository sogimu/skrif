#pragma once

#include <memory>
#include <iostream>
#include "../../parser/include/i_syntax_node.h"

// Предварительное объявление класса EnvScope
class EnvScope;

namespace json
{
  class Function
  {
  public:
      ~Function();
      Function(const Function&) = default;
      Function& operator=(const Function&) = default;
      std::shared_ptr<EnvScope> getScope() const;

      Function(std::shared_ptr<EnvScope> outer_env_scope, const std::string& text, std::shared_ptr<ISyntaxNode> ast);
      const std::string& get_text() const { return mText; }
      std::shared_ptr<ISyntaxNode> get_ast() const { return mAst; }

  private:
      std::string mText;
      std::shared_ptr<ISyntaxNode> mAst;
      std::weak_ptr<EnvScope> mOuter_env_scope;
  };
}
