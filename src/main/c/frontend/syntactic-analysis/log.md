# Mini-JS Frontend – Grammar Status Report
*(Bison + Flex parser for a JavaScript-like language)*

---

## 1 · Current coverage

| Feature area | Status & notes |
|--------------|---------------|
| **Programs & blocks** | `program` root, nested statement lists, `{ … }` blocks |
| **Declarations** | `let` / `const` lexical declarations (terminated with `;`) &nbsp;·&nbsp; classic `function foo (…) { … }` declarations |
| **Expressions** | • literals: integer & string<br>• identifiers, member access `obj.prop`, computed access `obj[expr]`<br>• `new` expressions (with or without argument list) – precedence issues fixed<br>• unary `+ - ! ~ ++ --` (pre & post)<br>• binary `** * / % + -`<br>• relational `< <= > >=`<br>• (strict) equality `== != === !==`<br>• comma operator |
| **Statements** | expression, `return`, `break`, `continue`, `throw`, `if / else`, classic `for ( ; ; )`, `while (…)`, `do … while`, `try / catch / finally` |
| **Literals / collections** | array literals `[1, , 3]` (sparse allowed) |
| **AST & memory** | All node types implemented; every node has a matching `release…()` and is wired into Bison `%destructor` directives |
| **Error handling** | Fatal error on `const` without initializer; otherwise Bison’s default abort on first syntax error |

---

## 2 · Remaining conflicts

| Kind | Count | Cause / comment |
|------|-------|-----------------|
| **shift / reduce** | **2** | Both involve the two-token form of `new`:<br>  • `new lhs (` vs reduction<br>  • nested `new new lhs (` vs reduction<br><br>They parse to the correct JavaScript structure, but Bison still reports the ambiguity. |

*(All previous conflicts on `.` or `[` were eliminated after introducing
a dedicated low-precedence `%prec NEW_PREC` token.)*

---

## 3 · Still missing / planned work

| Area | Gaps |
|------|------|
| **Operators** | Logical `&& ||`, bitwise `& | ^ << >>`, compound assignments `+= -= …`, `instanceof`, `in`, nullish `??`, ternary `? :` |
| **Object literals** | `{ a: 1, ["k"]: v }`, shorthand & computed properties |
| **Function expressions & arrows** | Only *function declarations* parsed; arrow, anonymous, generators pending |
| **Classes & modules** | `class`, `extends`, `super`, `import`, `export`, `this` |
| **Destructuring & spread** | `[a, ...rest] = arr`, `{ x, ...obj }`, spread in calls |
| **Automatic semicolon insertion (ASI)** | Currently hard-coded only for a few sites |
| **Literal variants** | Hex / binary / octal numerics, BigInt, template literals, unicode escapes |
| **Error recovery** | Parser stops at first error – more robust recovery desirable for IDE tooling |

---

_Last updated: 2025-05-22_
