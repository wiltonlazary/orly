/* <orly/expr/sign.cc>

   Implements <orly/expr/sign.h>

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <orly/expr/sign.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/impl.h>
#include <orly/type/unwrap.h>
#include <orly/type/unwrap_visitor.h>


using namespace Orly;
using namespace Orly::Expr;

class TSignTypeVisitor
    : public Type::TUnwrapVisitor {
  NO_COPY(TSignTypeVisitor);
  public:

  TSignTypeVisitor(Type::TType &type, const TPosRange &pos_range)
      : Type::TUnwrapVisitor(type, pos_range) {}

  virtual void operator()(const Type::TAddr     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TBool     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TDict     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TId       *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TInt      *) const {
    Type = Type::TInt::Get();
  }
  virtual void operator()(const Type::TList     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TObj      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TReal     *) const {
    Type = Type::TReal::Get();
  }
  virtual void operator()(const Type::TSet      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TStr      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TTimeDiff *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TTimePnt  *) const { throw TExprError(HERE, PosRange); }

};  // TSignTypeVisitor

TExpr::TPtr TNegative::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TNegative::TPtr(new TNegative(expr, pos_range));
}

TNegative::TNegative(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TNegative::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TNegative::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TSignTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TPositive::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TPositive::TPtr(new TPositive(expr, pos_range));
}

TPositive::TPositive(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TPositive::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TPositive::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TSignTypeVisitor(type, GetPosRange()));
  return type;
}
