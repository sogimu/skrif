#pragma once

#include <memory>
#include <iostream>
#include <variant>
#include "../../parser/include/i_syntax_node.h"

// Предварительное объявление класса EnvScope
class EnvScope;

namespace json
{
  class Function
  {
  public:
      Function( const Function& function );
      Function( Function&& function );
      Function( const std::shared_ptr< EnvScope >& global_scope );
      ~Function();
      Function& operator=(const Function&);
      Function& operator=(Function&&);
      // Метод для преобразования слабой ссылки в сильную
      void makeStrongReference();
      
      // Метод для преобразования сильной ссылки в слабую
      void makeWeakReference();
      
      // Метод для получения scope (независимо от типа ссылки)
      std::shared_ptr< EnvScope > getScope() const;
      
      // Проверка, является ли ссылка сильной
      bool isStrongReference() const;
          Function(const std::shared_ptr< EnvScope >& global_scope, const std::string& text, std::shared_ptr<ISyntaxNode> ast) : mEnvScope{global_scope}, mText(text), mAst(ast) {}
      //
          const std::string& get_text() const { return mText; }
          std::shared_ptr<ISyntaxNode> get_ast() const { return mAst; }

          bool operator==(const Function& other) const {
              return mText == other.mText && mAst == other.mAst;
          }

  private:
      std::string mText;
      std::shared_ptr<ISyntaxNode> mAst;
      std::variant< std::weak_ptr< EnvScope >, std::shared_ptr< EnvScope > > mEnvScope;
  };
}
